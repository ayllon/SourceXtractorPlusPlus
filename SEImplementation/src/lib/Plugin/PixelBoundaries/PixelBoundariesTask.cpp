/** Copyright © 2019 Université de Genève, LMU Munich - Faculty of Physics, IAP-CNRS/Sorbonne Université
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
/**
 * @file src/lib/Task/PixelBoundariesTask.cpp
 * @date 06/16/16
 * @author mschefer
 */

#include <climits>

#include "SEImplementation/Property/PixelCoordinateList.h"
#include "SEImplementation/Plugin/PixelBoundaries/PixelBoundaries.h"
#include "SEImplementation/Plugin/DetectionFramePixelValues/DetectionFramePixelValues.h"
#include "SEImplementation/Plugin/PeakValue/PeakValue.h"
#include "SEImplementation/Plugin/PixelBoundaries/PixelBoundariesTask.h"

namespace SourceXtractor {

void PixelBoundariesTask::computeProperties(SourceInterface& source) const {
  int min_x = INT_MAX;
  int min_y = INT_MAX;
  int max_x = INT_MIN;
  int max_y = INT_MIN;

  for (auto pixel_coord : source.getProperty<PixelCoordinateList>().getCoordinateList()) {
    min_x = std::min(min_x, pixel_coord.m_x);
    min_y = std::min(min_y, pixel_coord.m_y);
    max_x = std::max(max_x, pixel_coord.m_x);
    max_y = std::max(max_y, pixel_coord.m_y);
  }

  source.setProperty<PixelBoundaries>(min_x, min_y, max_x, max_y);
}

void PixelBoundariesTaskHalfMaximum::computeProperties(SourceInterface& source) const {
  const auto& pixel_values = source.getProperty<DetectionFramePixelValues>().getFilteredValues();
  SeFloat half_maximum = source.getProperty<PeakValue>().getMaxValue() / 2.0;

  int min_x_half = INT_MAX;
  int min_y_half = INT_MAX;
  int max_x_half = INT_MIN;
  int max_y_half = INT_MIN;

  auto i = pixel_values.begin();
  for (auto pixel_coord : source.getProperty<PixelCoordinateList>().getCoordinateList()) {
    SeFloat value = *i++;

    if (value >= half_maximum) {
      min_x_half = std::min(min_x_half, pixel_coord.m_x);
      min_y_half = std::min(min_y_half, pixel_coord.m_y);
      max_x_half = std::max(max_x_half, pixel_coord.m_x);
      max_y_half = std::max(max_y_half, pixel_coord.m_y);
    }
  }

  source.setProperty<PixelBoundariesHalfMaximum>(min_x_half, min_y_half, max_x_half, max_y_half);
}

} // SEImplementation namespace

