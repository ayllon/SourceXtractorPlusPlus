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
 * @file SEImplementation/PixelCentroid.h
 * @date 05/27/16
 * @author mschefer
 */

#ifndef _SEIMPLEMENTATION_PROPERTY_PIXELCENTROID_H
#define _SEIMPLEMENTATION_PROPERTY_PIXELCENTROID_H

#include "SEUtils/Types.h"
#include "SEFramework/Property/Property.h"

namespace SourceXtractor {

/**
 * @class PixelCentroid
 * @brief The centroid of all the pixels in the source, weighted by their DetectionImage pixel values.
 * @details Please note that we consider pixels to be centered around their coordinates
 *
 */
class PixelCentroid : public Property {
public:

  /**
   * @brief Destructor
   */
  virtual ~PixelCentroid() = default;

  PixelCentroid(SeFloat centroid_x, SeFloat centroid_y) : m_centroid_x(centroid_x), m_centroid_y(centroid_y) {}

  /// X coordinate of centroid
  SeFloat getCentroidX() const {
    return m_centroid_x;
  }

  /// Y coordinate of centroid
  SeFloat getCentroidY() const {
    return m_centroid_y;
  }

private:
  SeFloat m_centroid_x, m_centroid_y;

}; /* End of PixelCentroid class */

} /* namespace SourceXtractor */


#endif
