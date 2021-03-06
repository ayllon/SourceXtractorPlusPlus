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
 * PluginConfig.cpp
 *
 *  Created on: Jul 27, 2016
 *      Author: mschefer
 */

#include "SEMain/PluginConfig.h"

using namespace Euclid::Configuration;
namespace po = boost::program_options;

namespace SourceXtractor {

static const std::string PLUGIN_DIRECTORY { "plugin-directory" };
static const std::string PLUGIN { "plugin" };

PluginConfig::PluginConfig(long manager_id)
    : Configuration(manager_id) {
}

std::map<std::string, Configuration::OptionDescriptionList> PluginConfig::getProgramOptions() {
  return { {"Plugin configuration", {
      {PLUGIN_DIRECTORY.c_str(), po::value<std::string>()->default_value(""),
          "Path to a directory that contains the plugins"},
      {PLUGIN.c_str(), po::value<std::vector<std::string>>()->default_value(std::vector<std::string>(), ""),
          "Defines a plugin to load (without file extension). Can be used multiple times."}
  }}};
}

void PluginConfig::initialize(const UserValues& args) {
  m_plugin_path = args.at(PLUGIN_DIRECTORY).as<std::string>();
  m_plugin_list = args.at(PLUGIN).as<std::vector<std::string>>();
}

std::string PluginConfig::getPluginPath() const {
  return m_plugin_path;
}

std::vector<std::string> PluginConfig::getPluginList() const {
  return m_plugin_list;
}

}
