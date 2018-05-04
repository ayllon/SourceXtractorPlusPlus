/*
 * MultiframeSourceModel.cpp
 *
 *  Created on: Nov 24, 2017
 *      Author: mschefer
 */

#include "ModelFitting/utils.h"

#include "ModelFitting/Parameters/ExpSigmoidConverter.h"
#include "ModelFitting/Parameters/SigmoidConverter.h"
#include "ModelFitting/Parameters/NormalizedConverter.h"

#include "ModelFitting/Models/OnlySmooth.h"
#include "ModelFitting/Models/OldSharp.h"
#include "ModelFitting/Models/AutoSharp.h"

#include "ModelFitting/Models/CircularlySymmetricModelComponent.h"

#include "SEFramework/Property/DetectionFrame.h"

#include "SEImplementation/Plugin/ShapeParameters/ShapeParameters.h"
#include "SEImplementation/Plugin/IsophotalFlux/IsophotalFlux.h"
#include "SEImplementation/Plugin/PixelCentroid/PixelCentroid.h"
#include "SEImplementation/Plugin/PixelBoundaries/PixelBoundaries.h"

#include "SEImplementation/Plugin/MultiframeModelFitting/MultiframeSourceModel.h"

namespace SExtractor {

using namespace ModelFitting;

MultiframeSourceModel::MultiframeSourceModel(const SourceInterface& source) :
    m_size(getSize(source)),
    m_center_x(getCenterX(source)),
    m_center_y(getCenterY(source)),
    m_ref_coordinate_system(getRefCoordinateSystem(source)),

    m_radius_guess(getRadiusGuess(source)),
    m_aspect_guess(getAspectGuess(source)),
    m_exp_flux_guess(getFluxGuess(source)),
    m_dev_flux_guess(getFluxGuess(source)),

    dx(0, make_unique<SigmoidConverter>(-m_radius_guess, m_radius_guess)),
    dy(0, make_unique<SigmoidConverter>(-m_radius_guess, m_radius_guess)),

    exp_effective_radius(m_radius_guess, make_unique<ExpSigmoidConverter>(0.01, m_radius_guess * 10)),
    dev_effective_radius(m_radius_guess, make_unique<ExpSigmoidConverter>(0.01, m_radius_guess * 10)),

    exp_k(
        [](double eff_radius) { return 1.7 / eff_radius; },
        exp_effective_radius),
    dev_k(
        [](double eff_radius) { return pow(3459.0 / eff_radius, .25); },
        dev_effective_radius),

    exp_aspect(m_aspect_guess, make_unique<SigmoidConverter>(0, 1.0)),
    exp_rot(getRotGuess(source), make_unique<SigmoidConverter>(-M_PI, M_PI)),
    dev_aspect(m_aspect_guess, make_unique<SigmoidConverter>(0, 1.0)),
    dev_rot(getRotGuess(source), make_unique<SigmoidConverter>(-M_PI, M_PI)),

    m_number_of_parameters(0)
  {
  }

void MultiframeSourceModel::createPlaceholderForInactiveBand() {
  exp_fluxes.emplace_back(nullptr);
  dev_fluxes.emplace_back(nullptr);
  exp_i0s.emplace_back(nullptr);
  dev_i0s.emplace_back(nullptr);
}

void MultiframeSourceModel::createParamsForBand() {
    exp_fluxes.emplace_back(new EngineParameter(m_exp_flux_guess, make_unique<ExpSigmoidConverter>(m_exp_flux_guess * .00001, m_exp_flux_guess * 20)));
    dev_fluxes.emplace_back(new EngineParameter(m_dev_flux_guess, make_unique<ExpSigmoidConverter>(m_dev_flux_guess * .00001, m_dev_flux_guess * 20)));

    exp_i0s.emplace_back(new DependentParameter<EngineParameter, EngineParameter, EngineParameter>(
       [](double flux, double radius, double aspect) { return flux / (M_PI * 2.0 * 0.346 * radius * radius * aspect); },
       *exp_fluxes.back(), exp_effective_radius, exp_aspect));
    dev_i0s.emplace_back(new DependentParameter<EngineParameter, EngineParameter, EngineParameter>(
       [](double flux, double radius, double aspect) { return flux * pow(10, 3.33) / (7.2 * M_PI * radius * radius *  aspect); },
       *dev_fluxes.back(), dev_effective_radius, dev_aspect));
  }

  void MultiframeSourceModel::createParamsForFrame(int band_nb, int frame_nb, std::shared_ptr<CoordinateSystem> coordinates, PixelCoordinate offset) {
    m_frame_map[frame_nb] = pixel_x.size();
    m_frame_band_map[frame_nb] = band_nb;

    auto cx = m_center_x;
    auto cy = m_center_y;
    auto cs = m_ref_coordinate_system;

    pixel_x.emplace_back(new DependentParameter<EngineParameter, EngineParameter>(
        [coordinates, cx, cy, offset, cs](double dx, double dy) {
          return coordinates->worldToImage(cs->imageToWorld(ImageCoordinate(cx + dx, cy + dy))).m_x - offset.m_x;
        }, dx, dy));
    pixel_y.emplace_back(new DependentParameter<EngineParameter, EngineParameter>(
        [coordinates, cx, cy, offset, cs](double dx, double dy) {
          return coordinates->worldToImage(cs->imageToWorld(ImageCoordinate(cx + dx, cy + dy))).m_y - offset.m_y;
        }, dx, dy));
  }

  void MultiframeSourceModel::addModelsForFrame(int frame_nb, std::vector<ModelFitting::ExtendedModel>& extended_models) {
    // exponential
    {
      std::vector<std::unique_ptr<ModelComponent>> component_list {};
      auto exp = make_unique<SersicModelComponent>(make_unique<OldSharp>(), *(exp_i0s[m_frame_band_map[frame_nb]]), exp_n, exp_k);
      component_list.clear();
      component_list.emplace_back(std::move(exp));
      extended_models.emplace_back(
          std::move(component_list), exp_xs, exp_aspect, exp_rot, m_size, m_size, *(pixel_x[m_frame_map[frame_nb]]), *(pixel_y[m_frame_map[frame_nb]]));
    }
    // devaucouleurs
    {
      std::vector<std::unique_ptr<ModelComponent>> component_list {};
      auto dev = make_unique<SersicModelComponent>(make_unique<OldSharp>(), *(dev_i0s[m_frame_band_map[frame_nb]]), dev_n, dev_k);
      component_list.clear();
      component_list.emplace_back(std::move(dev));
      extended_models.emplace_back(
          std::move(component_list), dev_xs, dev_aspect, dev_rot, m_size, m_size, *(pixel_x[m_frame_map[frame_nb]]), *(pixel_y[m_frame_map[frame_nb]]));
    }
  }

  void MultiframeSourceModel::registerParameters(EngineParameterManager& manager) {
    manager.registerParameter(dx);
    manager.registerParameter(dy);
    manager.registerParameter(exp_effective_radius);
    manager.registerParameter(dev_effective_radius);

    manager.registerParameter(exp_aspect);
    manager.registerParameter(dev_aspect);
    manager.registerParameter(exp_rot);
    manager.registerParameter(dev_rot);

    m_number_of_parameters += 8;

    for (auto& exp_flux : exp_fluxes) {
      if (exp_flux != nullptr) {
        manager.registerParameter(*exp_flux);
        m_number_of_parameters++;
      }
    }

    for (auto& dev_flux : dev_fluxes) {
      if (dev_flux != nullptr) {
        manager.registerParameter(*dev_flux);
        m_number_of_parameters++;
      }
    }
  }

  void MultiframeSourceModel::debugPrint() const {
    //std::cout << "\tsize: " << m_size << "\n";
    std::cout << "\tx: " << dx.getValue() << "\ty: " << dy.getValue() << "\n";
    for (auto& exp_flux : exp_fluxes) {
      if (exp_flux != nullptr) {
        std::cout << exp_flux->getValue() << " ";
      }
    }
    std::cout << "\n";
  }

  int MultiframeSourceModel::getSize(const SourceInterface& source) const {
    auto& boundaries = source.getProperty<PixelBoundaries>();
    int size = std::max(boundaries.getWidth(), boundaries.getHeight());
    return size*2;
  }

  double MultiframeSourceModel::getFluxGuess(const SourceInterface& source) const {
    auto iso_flux = source.getProperty<IsophotalFlux>().getFlux();
    return iso_flux / 2;
  }

  double MultiframeSourceModel::getRadiusGuess(const SourceInterface& source) const {
    auto& shape_parameters = source.getProperty<ShapeParameters>();
    double radius_guess = shape_parameters.getEllipseA() / 2.0;

    return radius_guess;
  }

  double MultiframeSourceModel::getAspectGuess(const SourceInterface& source) const {
    auto& shape_parameters = source.getProperty<ShapeParameters>();
    double aspect_guess = std::max<double>(shape_parameters.getEllipseB() / shape_parameters.getEllipseA(), 0.01);

    return aspect_guess;
  }

  double MultiframeSourceModel::getRotGuess(const SourceInterface& source) const {
    auto& shape_parameters = source.getProperty<ShapeParameters>();
    double rot_guess = shape_parameters.getEllipseTheta();

    return -rot_guess;
  }

  double MultiframeSourceModel::getCenterX(const SourceInterface& source) const {
    auto& centroid = source.getProperty<PixelCentroid>();
    return centroid.getCentroidX();
  }

  double MultiframeSourceModel::getCenterY(const SourceInterface& source) const {
    auto& centroid = source.getProperty<PixelCentroid>();
    return centroid.getCentroidY();
  }

  std::shared_ptr<CoordinateSystem> MultiframeSourceModel::getRefCoordinateSystem(const SourceInterface& source) const {
    auto frame = source.getProperty<DetectionFrame>().getFrame();
    return frame->getCoordinateSystem();
  }

  int MultiframeSourceModel::getNumberOfParameters() const {
    return m_number_of_parameters;
  }

  WorldCoordinate MultiframeSourceModel::getFittedWorldCoordinate() const {
    return m_ref_coordinate_system->imageToWorld(ImageCoordinate(m_center_x + dx.getValue(), m_center_y + dy.getValue()));
  }

  ImageCoordinate MultiframeSourceModel::getFittedCoordinate() const {
    return ImageCoordinate(m_center_x + dx.getValue(), m_center_y + dy.getValue());
  }

  double MultiframeSourceModel::getExpFluxForBand(int band_nb) const {
    return exp_fluxes[band_nb] != nullptr ? exp_fluxes[band_nb]->getValue() : nan("");
  }

  double MultiframeSourceModel::getDevFluxForBand(int band_nb) const {
    return dev_fluxes[band_nb] != nullptr ? dev_fluxes[band_nb]->getValue() : nan("");
  }

  std::vector<double> MultiframeSourceModel::getFluxes() const {
    std::vector<double> fluxes;

    for (unsigned int i=0; i<exp_fluxes.size(); i++) {
      fluxes.push_back(getExpFluxForBand(i) + getDevFluxForBand(i));
    }

    return fluxes;
  }

  std::vector<double> MultiframeSourceModel::getExpFluxes() const {
    std::vector<double> fluxes;

    for (unsigned int i=0; i<exp_fluxes.size(); i++) {
      fluxes.push_back(getExpFluxForBand(i));
    }

    return fluxes;
  }

  std::vector<double> MultiframeSourceModel::getDevFluxes() const {
    std::vector<double> fluxes;

    for (unsigned int i=0; i<dev_fluxes.size(); i++) {
      fluxes.push_back(getDevFluxForBand(i));
    }

    return fluxes;
  }

  SeFloat MultiframeSourceModel::getExpRadius() const {
    return exp_effective_radius.getValue();
  }

  SeFloat MultiframeSourceModel::getDevRadius() const {
    return dev_effective_radius.getValue();
  }
}

