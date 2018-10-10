/*
 * FlexibleModelFittingTask.cpp
 *
 *  Created on: Sep 17, 2018
 *      Author: mschefer
 */

#include <mutex>

#include "ModelFitting/Models/PointModel.h"
#include "ModelFitting/Models/ExtendedModel.h"
#include "ModelFitting/Models/TransformedModel.h"
#include "ModelFitting/Models/FrameModel.h"
#include "ModelFitting/Engine/ResidualEstimator.h"
#include "ModelFitting/Engine/LevmarEngine.h"
#include "ModelFitting/Engine/LogChiSquareComparator.h"
#include "ModelFitting/Engine/DataVsModelResiduals.h"

#include "SEImplementation/Measurement/MultithreadedMeasurement.h"

#include "SEImplementation/Image/ImageInterfaceTraits.h"
#include "SEImplementation/Image/VectorImageDataVsModelInputTraits.h"
#include <SEImplementation/Image/ImagePsf.h>

#include "SEFramework/Property/DetectionFrame.h"
#include "SEImplementation/Plugin/ShapeParameters/ShapeParameters.h"
#include "SEImplementation/Plugin/MeasurementFrame/MeasurementFrame.h"
#include "SEImplementation/Plugin/MeasurementFramePixelCentroid/MeasurementFramePixelCentroid.h"
#include "SEImplementation/Plugin/DetectionFrameGroupStamp/DetectionFrameGroupStamp.h"
#include <SEImplementation/Plugin/Psf/PsfProperty.h>
#include <SEImplementation/Plugin/MeasurementFrameGroupRectangle/MeasurementFrameGroupRectangle.h>

#include "SEImplementation/Plugin/FlexibleModelFitting/FlexibleModelFittingParameterManager.h"
#include "SEImplementation/Plugin/FlexibleModelFitting/FlexibleModelFittingTask.h"

namespace SExtractor {

using namespace ModelFitting;

FlexibleModelFittingTask::FlexibleModelFittingTask(unsigned int max_iterations)
  : m_max_iterations(max_iterations) {}

bool FlexibleModelFittingTask::isFrameValid(SourceGroupInterface& group, int frame_index) const {
  auto stamp_rect = group.getProperty<MeasurementFrameGroupRectangle>(frame_index);
  return stamp_rect.getWidth() > 0 && stamp_rect.getHeight() > 0;
}

std::tuple<double, double, double, double> FlexibleModelFittingTask::computeJacobianForFrame(SourceGroupInterface& group, int frame_index) const {
  auto frame = group.begin()->getProperty<MeasurementFrame>(frame_index).getFrame();
  auto frame_coordinates = frame->getCoordinateSystem();
  auto& detection_group_stamp = group.getProperty<DetectionFrameGroupStamp>();
  auto detection_frame_coordinates = group.begin()->getProperty<DetectionFrame>().getFrame()->getCoordinateSystem();

  double x = detection_group_stamp.getTopLeft().m_x + detection_group_stamp.getStamp().getWidth() / 2.0;
  double y = detection_group_stamp.getTopLeft().m_y + detection_group_stamp.getStamp().getHeight() / 2.0;

  auto frame_origin = frame_coordinates->worldToImage(detection_frame_coordinates->imageToWorld(ImageCoordinate(x, y)));
  auto frame_dx = frame_coordinates->worldToImage(detection_frame_coordinates->imageToWorld(ImageCoordinate(x+1.0, y)));
  auto frame_dy = frame_coordinates->worldToImage(detection_frame_coordinates->imageToWorld(ImageCoordinate(x, y+1.0)));

  return std::make_tuple(frame_dx.m_x - frame_origin.m_x, frame_dx.m_y - frame_origin.m_y,
      frame_dy.m_x - frame_origin.m_x, frame_dy.m_y - frame_origin.m_y);
}

std::shared_ptr<VectorImage<SeFloat>> FlexibleModelFittingTask::createImageCopy(
    SourceGroupInterface& group, int frame_index) const {
  std::lock_guard<std::recursive_mutex> lock(MultithreadedMeasurement::g_global_mutex);

  auto frame = group.begin()->getProperty<MeasurementFrame>(frame_index).getFrame();
  auto frame_image = frame->getSubtractedImage();

  auto rect =  group.getProperty<MeasurementFrameGroupRectangle>(frame_index);
  auto image = VectorImage<SeFloat>::create(rect.getWidth(), rect.getHeight());
  for (int y=0; y < rect.getHeight(); y++) {
    for (int x=0; x < rect.getWidth(); x++) {
      image->at(x, y) = frame_image->getValue(rect.getTopLeft().m_x + x, rect.getTopLeft().m_y + y);
    }
  }

  return image;
}

std::shared_ptr<VectorImage<SeFloat>> FlexibleModelFittingTask::createWeightImage(
    SourceGroupInterface& group, int frame_index) const {
  std::lock_guard<std::recursive_mutex> lock(MultithreadedMeasurement::g_global_mutex);

  auto frame = group.begin()->getProperty<MeasurementFrame>(frame_index).getFrame();
  auto frame_image = frame->getSubtractedImage();
  auto frame_image_thresholded = frame->getThresholdedImage();
  auto variance_map = frame->getVarianceMap();

  auto rect = group.getProperty<MeasurementFrameGroupRectangle>(frame_index);
  auto weight = VectorImage<SeFloat>::create(rect.getWidth(), rect.getHeight());
  std::fill(weight->getData().begin(), weight->getData().end(), 1);


  auto measurement_frame = group.begin()->getProperty<MeasurementFrame>(frame_index).getFrame();
  SeFloat gain = measurement_frame->getGain();
  SeFloat saturation = measurement_frame->getSaturation();
  //std::cout << "Saturation: " << saturation << " gain: " << gain << "\n";

  for (int y=0; y < rect.getHeight(); y++) {
    for (int x=0; x < rect.getWidth(); x++) {
      auto back_var = variance_map->getValue(rect.getTopLeft().m_x + x, rect.getTopLeft().m_y + y);
      if (saturation > 0 && frame_image->getValue(rect.getTopLeft().m_x + x, rect.getTopLeft().m_y + y) > saturation) {
        weight->at(x, y) = 0;
      } else if (weight->at(x, y) > 0) {
        if (gain > 0.0) {
          weight->at(x, y) = sqrt(1.0 / (back_var + frame_image->getValue(rect.getTopLeft().m_x + x, rect.getTopLeft().m_y + y) / gain));
        } else {
          weight->at(x, y) = sqrt(1.0 / back_var); // infinite gain
        }
      }
    }
  }

  return weight;
}



void FlexibleModelFittingTask::computeProperties(SourceGroupInterface& group) const {
  std::cout << "FlexibleModelFittingTask::computeProperties()\n";

  double pixel_scale = 1;
  FlexibleModelFittingParameterManager manager;

  // Prepare parameters
  for (auto& source : group) {
    for (auto parameter : m_parameters) {
      manager.add(source, parameter);
    }
  }

  // Add models for all frames
  ResidualEstimator res_estimator {};
  std::vector<std::shared_ptr<Image<SeFloat>>> images;
  std::vector<std::shared_ptr<Image<SeFloat>>> weights;

  for (auto frame : m_frames) {
    std::vector<PointModel> point_models;
    std::vector<TransformedModel> extended_models;

    int frame_index = frame->getFrameNb();
    // Validate that each frame covers the model fitting region
    if (isFrameValid(group, frame_index)) {
      auto frame_coordinates =
          group.begin()->getProperty<MeasurementFrame>(frame_index).getFrame()->getCoordinateSystem();

      auto stamp_rect = group.getProperty<MeasurementFrameGroupRectangle>(frame_index);
      auto image = createImageCopy(group, frame_index);
      auto weight = createWeightImage(group, frame_index);
      auto group_psf = group.getProperty<PsfProperty>(frame_index).getPsf();
      auto jacobian = computeJacobianForFrame(group, frame_index);

      for (auto& source : group) {
        for (auto model : frame->getModels()) {
          // FIXME we need the correct offset
          model->addForSource(manager, source, point_models, extended_models, jacobian, frame_coordinates, stamp_rect.getTopLeft());
        }
      }

      // Full frame model with all sources
      FrameModel<ImagePsf, std::shared_ptr<VectorImage<SExtractor::SeFloat>>> frame_model(
        pixel_scale, (size_t) stamp_rect.getWidth(), (size_t) stamp_rect.getHeight(),
        {}, {}, std::move(extended_models), group_psf);

      // Setup residuals
      auto data_vs_model =
          createDataVsModelResiduals(image, std::move(frame_model), weight, LogChiSquareComparator{});
      res_estimator.registerBlockProvider(std::move(data_vs_model));

      images.emplace_back(image);
      weights.emplace_back(weight);
    }
  }

  // Model fitting

  LevmarEngine engine {m_max_iterations, 1E-6, 1E-6, 1E-6, 1E-6, 1E-4};
  auto solution = engine.solveProblem(manager.getEngineParameterManager(), res_estimator);
  size_t iterations = (size_t) boost::any_cast<std::array<double,10>>(solution.underlying_framework_info)[5];


  // Get results


}

FlexibleModelFittingTask::~FlexibleModelFittingTask() {
}

}