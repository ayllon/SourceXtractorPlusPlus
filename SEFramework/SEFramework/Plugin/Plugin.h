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
 * Plugin.h
 *
 *  Created on: Jul 26, 2016
 *      Author: mschefer
 */

#ifndef _SEFRAMEWORK_PLUGIN_PLUGIN_H_
#define _SEFRAMEWORK_PLUGIN_PLUGIN_H_

#include <SEFramework/Plugin/PluginAPI.h>

namespace SourceXtractor {

/**
 * @class Plugin
 * @brief Plugins must implement this interface.
 *
 * @details registerPlugin() is going to be called to give the plugin a chance to register what it needs to.
 */

class Plugin {
public:
  virtual ~Plugin() = default;

  virtual std::string getIdString() const = 0;
  virtual void registerPlugin(PluginAPI& plugin_api) = 0;
};

} // namespace SourceXtractor

#endif /* _SEFRAMEWORK_PLUGIN_PLUGIN_H_ */
