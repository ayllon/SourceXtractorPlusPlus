/*
 * ShapeParametersTask.cpp
 *
 *  Created on: Jan 27, 2017
 *      Author: mschefer
 */

#include <iostream>

#include "SEImplementation/Property/PixelCoordinateList.h"
#include "SEImplementation/Plugin/DetectionFramePixelValues/DetectionFramePixelValues.h"
#include "SEImplementation/Plugin/ShapeParameters/ShapeParameters.h"
#include "SEImplementation/Plugin/PixelCentroid/PixelCentroid.h"
#include "SEImplementation/Plugin/PeakValue/PeakValue.h"

#include "SEImplementation/Plugin/ShapeParameters/ShapeParametersTask.h"

namespace SExtractor {

void ShapeParametersTask::computeProperties(SourceInterface& source) const {
  const auto& pixel_values = source.getProperty<DetectionFramePixelValues>().getFilteredValues();
  const auto& centroid_x = source.getProperty<PixelCentroid>().getCentroidX();
  const auto& centroid_y = source.getProperty<PixelCentroid>().getCentroidY();
  auto min_value = source.getProperty<PeakValue>().getMinValue();
  auto peak_value = source.getProperty<PeakValue>().getMaxValue();
  auto& coordinates = source.getProperty<PixelCoordinateList>().getCoordinateList();

  SeFloat x_2 = 0.0;
  SeFloat y_2 = 0.0;
  SeFloat x_y = 0.0;

  SeFloat total_intensity = 0;

  DetectionImage::PixelType half_peak_threshold = (peak_value + min_value) / 2.0;
  int nb_of_pixels_above_half = 0;
  int nb_of_pixels = coordinates.size();

  auto i = pixel_values.begin();
  for (auto pixel_coord : coordinates) {
    SeFloat value = *i++;

    if (value > half_peak_threshold) {
      nb_of_pixels_above_half++;
    }

    x_2 += (pixel_coord.m_x - centroid_x) * (pixel_coord.m_x - centroid_x) * value;
    y_2 += (pixel_coord.m_y - centroid_y) * (pixel_coord.m_y - centroid_y) * value;
    x_y += (pixel_coord.m_x - centroid_x) * (pixel_coord.m_y - centroid_y) * value;

    total_intensity += value;
  }

  x_2 /= total_intensity;
  y_2 /= total_intensity;
  x_y /= total_intensity;

  SeFloat theta;
  if (fabs(x_2 - y_2) > 0.0) {
    theta = atan2(2.0 * x_y, x_2 - y_2) / 2.0;
  } else {
    theta = M_PI/4.0;
  }

  float temp = x_2 - y_2;
  temp = sqrt(0.25 * temp * temp + x_y * x_y);
  SeFloat a = sqrt((x_2 + y_2) / 2 + temp);
  SeFloat b = ((x_2 + y_2) / 2) > temp ? sqrt((x_2 + y_2) / 2 - temp) : 0;

  // From original SExtractor: Handle fully correlated x/y (which cause a singularity...)
  SeFloat tmp = x_2 * y_2 - x_y * x_y;

  if (tmp < 0.00694) {
    x_2 += 0.0833333;
    y_2 += 0.0833333;
    tmp = x_2 * y_2 - x_y * x_y;
  }

  SeFloat cxx = y_2 / tmp;
  SeFloat cyy = x_2 / tmp;
  SeFloat cxy = -2.0 * x_y / tmp;

  SeFloat area_under_half = (double) pixel_values.size() - nb_of_pixels_above_half;
  SeFloat t1t2 = min_value / half_peak_threshold;

  //std::cout << "a "<< a << " b " << b << " t1t2 " << t1t2 << " area " << area_under_half << "     " << min_value << " " << half_peak_threshold << std::endl;

  SeFloat abcor = 1.0;
  if (t1t2 > 0.0) { // && !prefs.dweight_flag
    abcor = (area_under_half > 0.0 ? area_under_half : 1.0) / (2 * M_PI * -log(t1t2 < 1.0 ? t1t2 : 0.99) * a * b);
    if (abcor > 1.0) {
      abcor = 1.0;
    }
  }

  //std::cout << "abcor " << abcor << std::endl;
  source.setProperty<ShapeParameters>(a, b, theta, abcor, cxx, cyy, cxy, nb_of_pixels);
}


}