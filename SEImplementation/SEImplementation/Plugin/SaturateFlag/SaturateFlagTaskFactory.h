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
 * Copyright (C) 2012-2020 Euclid Science Ground Segment    
 *  
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General  
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)  
 * any later version.  
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied  
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more  
 * details.  
 *  
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to  
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA  
 */

/**
 * @file SourceFlagsTaskFactory.h
 *
 * @date Jul 13, 2018
 * @author mkuemmel@usm.lmu.de
 */
#ifndef _SEIMPLEMENTATION_PLUGIN_SATURATEFLAGTASKFACTORY_H_
#define _SEIMPLEMENTATION_PLUGIN_SATURATEFLAGTASKFACTORY_H_

#include "SEFramework/Task/TaskFactory.h"
#include "SEImplementation/Plugin/SaturateFlag/SaturateFlagSourceTask.h"

namespace SourceXtractor {

class SaturateFlagTaskFactory : public TaskFactory {
public:
  SaturateFlagTaskFactory() {}

  virtual ~SaturateFlagTaskFactory() = default;

  // TaskFactory implementation
  virtual std::shared_ptr<Task> createTask(const PropertyId &property_id) const {
    if (property_id.getTypeId() == typeid(SaturateFlag)) {
      return std::make_shared<SaturateFlagSourceTask>(property_id.getIndex());
    } else {
      return nullptr;
    }
  }
}; // end of SourceFlagsTaskFactory class

}  // namespace SourceXtractor
#endif /* _SEIMPLEMENTATION_PLUGIN_SATURATEFLAGTASKFACTORY_H_ */
