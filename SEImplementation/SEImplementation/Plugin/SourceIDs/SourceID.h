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
/*
 * SourceID.h
 *
 *  Created on: May 30, 2018
 *      Author: mschefer
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_SOURCEIDS_SOURCEID_H_
#define _SEIMPLEMENTATION_PLUGIN_SOURCEIDS_SOURCEID_H_


#include "SEUtils/Types.h"
#include "SEFramework/Property/Property.h"

namespace SourceXtractor {

class SourceID : public Property {
public:

  virtual ~SourceID() = default;

  SourceID(unsigned int id, unsigned int detection_id) : m_id(id), m_detection_id(detection_id) {}

  int  getId() const {
    return m_id;
  }

  int  getDetectionId() const {
    return m_detection_id;
  }


private:
  unsigned int m_id;
  unsigned int m_detection_id;
};

} /* namespace SourceXtractor */



#endif /* _SEIMPLEMENTATION_PLUGIN_SOURCEIDS_SOURCEID_H_ */
