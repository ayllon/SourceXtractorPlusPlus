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
 * CoreThresholdPartitionConfig.cpp
 *
 * @date
 * @author
 */

#include <Configuration/ProgramOptionsHelper.h>

#include "SEImplementation/Plugin/Dummy/DummyConfig.h"

//#include "SEImplementation/Configuration/DetectionImageConfig.h"
//#include "SEImplementation/Configuration/MultiThresholdPartitionConfig.h"


namespace po = boost::program_options;
using namespace Euclid::Configuration;

namespace SourceXtractor {

static const std::string DUMMY_VALUE   {"dummy-value" };

DummyConfig::DummyConfig(long manager_id)
  : Configuration(manager_id), m_dummy(0.) { }

std::map<std::string, Configuration::OptionDescriptionList> DummyConfig::getProgramOptions() {
  return {{"Dummy part:", {
      {DUMMY_VALUE.c_str(), po::value<float>()->default_value(10.0), "One dummy value"},
  }}};
}

void DummyConfig::initialize(const UserValues &args) {
  m_dummy = args.find(DUMMY_VALUE)->second.as<float>();

  if (m_dummy < 0.) {
    throw Elements::Exception() << "Invalid " << DUMMY_VALUE << " value: " << m_dummy;
  }
}

const float& DummyConfig::getDummy() const {
  return m_dummy;
}
} // end SExtractor
