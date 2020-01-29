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
 * @file DummyPlugin.h
 *
 * @date
 * @author
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_DUMMYPLUGIN_H_
#define _SEIMPLEMENTATION_PLUGIN_DUMMYPLUGIN_H_

#include "Dummy.h"
#include "SEFramework/Plugin/Plugin.h"
#include "SEImplementation/Plugin/Dummy/DummyTaskFactory.h"

namespace SourceXtractor {
class DummyPlugin : public Plugin {
public:
  virtual ~DummyPlugin() = default;
  virtual void registerPlugin(PluginAPI& plugin_api) {
    plugin_api.getTaskFactoryRegistry().registerTaskFactory<DummyTaskFactory, Dummy>();
    plugin_api.getOutputRegistry().registerColumnConverter<Dummy, float>(
            "dummy",
            [](const Dummy& prop){
              return prop.getDummy();
            },
            "[]",
            "A total dummy column"
    );
    plugin_api.getOutputRegistry().enableOutput<Dummy>("Dummy");
  }
  virtual std::string getIdString() const {
    return "dummy";
  }
private:
}; // end of DummyPlugin class
}  // namespace SourceXtractor
#endif /* _SEIMPLEMENTATION_PLUGIN_DUMMYPLUGIN_H_ */
