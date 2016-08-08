/*
 * PixelBoundariesPlugin.h
 *
 *  Created on: Aug 5, 2016
 *      Author: mschefer
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_PIXELBOUNDARIESPLUGIN_H_
#define _SEIMPLEMENTATION_PLUGIN_PIXELBOUNDARIESPLUGIN_H_

#include "SEFramework/Plugin/Plugin.h"

namespace SExtractor {

class PixelBoundariesPlugin : public Plugin {

public:

  /**
   * @brief Destructor
   */
  virtual ~PixelBoundariesPlugin() = default;

  virtual void registerPlugin(PluginAPI& plugin_api) override;
  virtual std::string getIdString() const override;

private:

}; /* End of TestPlugin class */

}


#endif /* _SEIMPLEMENTATION_PLUGIN_PIXELBOUNDARIESPLUGIN_H_ */