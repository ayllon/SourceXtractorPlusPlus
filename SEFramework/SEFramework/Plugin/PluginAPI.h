/*
 * PluginAPI.h
 *
 *  Created on: Aug 5, 2016
 *      Author: mschefer
 */

#ifndef _SEFRAMEWORK_PLUGIN_PLUGINAPI_H_
#define _SEFRAMEWORK_PLUGIN_PLUGINAPI_H_

#include "SEFramework/Task/TaskFactoryRegistry.h"
#include "SEFramework/Registration/OutputRegistry.h"

namespace SExtractor {

class PluginAPI {
public:
  virtual TaskFactoryRegistry& getTaskFactoryRegistry() const = 0;
  virtual OutputRegistry& getOutputRegistry() const = 0;
};

}

#endif /* _SEFRAMEWORK_PLUGIN_PLUGINAPI_H_ */