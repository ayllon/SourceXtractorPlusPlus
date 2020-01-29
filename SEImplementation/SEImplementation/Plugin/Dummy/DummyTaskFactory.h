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
 * @file DummyTaskFactory.h
 *
 * @date
 * @author
 */
#ifndef _SEIMPLEMENTATION_PLUGIN_DUMMYTASKFACTORY_H_
#define _SEIMPLEMENTATION_PLUGIN_DUMMYTASKFACTORY_H_

#include "SEFramework/Task/TaskFactory.h"
#include "SEImplementation/Plugin/Dummy/DummyConfig.h"
#include "SEImplementation/Plugin/Dummy/DummySourceTask.h"

namespace SourceXtractor {
class DummyTaskFactory : public TaskFactory {
public:
  DummyTaskFactory() {}
  virtual ~DummyTaskFactory() = default;

  void reportConfigDependencies(Euclid::Configuration::ConfigManager& manager) const {
    manager.registerConfiguration<DummyConfig>();
  };

  void configure(Euclid::Configuration::ConfigManager& manager) {
    auto dummy_config = manager.getConfiguration<DummyConfig>();
    m_dummy_value = dummy_config.getDummy();
  };

  // TaskFactory implementation
  virtual std::shared_ptr<Task> createTask(const PropertyId& property_id) const {
    if (property_id == PropertyId::create<Dummy>()) {
      return std::make_shared<DummySourceTask>(m_dummy_value);
    }
    else{
      return nullptr;
    }
  }
private:
  float m_dummy_value;
}; // end of DummyTaskFactory class
}  // namespace SourceXtractor
#endif /* _SEIMPLEMENTATION_PLUGIN_DUMMYTASKFACTORY_H_ */
