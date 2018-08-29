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

/*
 * SourceIDsPlugin.h
 *
 *  Created on: Apr 27, 2018
 *      Author: mkuemmel@usm.lmu.de
 */

#ifndef __SEIMPLEMENTATION_PLUGIN_SOURCEIDS_SOURCEIDSPLUGIN_H_
#define __SEIMPLEMENTATION_PLUGIN_SOURCEIDS_SOURCEIDSPLUGIN_H_

#include "SEImplementation/Property/SourceId.h"
#include "SEFramework/Plugin/Plugin.h"

namespace SExtractor {
class SourceIDsPlugin : public Plugin {
public:

  /**
   * @brief Destructor
   */
  virtual ~SourceIDsPlugin() = default;

  virtual void registerPlugin(PluginAPI& plugin_api) {
    plugin_api.getTaskFactoryRegistry().registerTaskFactory<SourceIDTaskFactory, SourceID>();

    //TODO: "unsigned int" seems not to work as column
    plugin_api.getOutputRegistry().registerColumnConverter<SourceID, int>(
            "source_id",
            [](const SourceID& prop){
              return prop.getId();
            }
    );
    plugin_api.getOutputRegistry().registerColumnConverter<SourceID, int>(
            "detection_id",
            [](const SourceID& prop){
              return prop.getDetectionId();
            }
    );
    plugin_api.getOutputRegistry().optionalOutput<SourceID>("SourceIDs");
  }

  virtual std::string getIdString() const {
    return "source_IDs_plugin";
  }
private:
};
} /* namespace SExtractor */
#endif /* __SEIMPLEMENTATION_PLUGIN_SOURCEIDS_SOURCEIDSPLUGIN_H_ */