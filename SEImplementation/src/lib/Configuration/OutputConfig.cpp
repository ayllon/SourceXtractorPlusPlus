/**
 * @file src/lib/Configuration/OutputConfig.cpp
 * @date 06/23/16
 * @author mschefer
 */

#include <boost/tokenizer.hpp>

#include "SEImplementation/Property/PixelCentroid.h"
#include "SEImplementation/Property/PixelBoundaries.h"

#include "SEImplementation/Configuration/OutputConfig.h"

using namespace Euclid::Configuration;
namespace po = boost::program_options;

namespace SExtractor {

static const std::string OUTPUT_PIXEL_CENTROID {"output-pixel-centroid"};
static const std::string OUTPUT_PIXEL_BOUNDARIES {"output-pixel-boundaries"};

OutputConfig::OutputConfig(long manager_id) : Configuration(manager_id) {
}

std::map<std::string, Configuration::OptionDescriptionList> OutputConfig::getProgramOptions() {
  return { {"Output configuration", {
      {OUTPUT_PIXEL_CENTROID.c_str(), po::bool_switch(),
          "Flag for adding the centroid (in pixels) to the output catalog"},
      {OUTPUT_PIXEL_BOUNDARIES.c_str(), po::bool_switch(),
          "Flag for adding the boundaries (in pixels) to the output catalog"}
  }}};
}

void OutputConfig::initialize(const UserValues& args) {
  m_pixel_centroid = (args.at(OUTPUT_PIXEL_CENTROID).as<bool>());
  m_pixel_boundaries = (args.at(OUTPUT_PIXEL_BOUNDARIES).as<bool>());
}

void OutputConfig::setEnabledOutputs(OutputRegistry& output_registry) {
  if (m_pixel_centroid) {
    output_registry.enableOutput<PixelCentroid>();
  }
  if (m_pixel_boundaries) {
    output_registry.enableOutput<PixelBoundaries>();
  }
}

} // SEImplementation namespace


