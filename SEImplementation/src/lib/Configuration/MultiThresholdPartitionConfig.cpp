/*
 * MultiThresholdPartitionConfig.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: mschefer
 */

#include "SEImplementation/Configuration/MultiThresholdPartitionConfig.h"
#include "SEImplementation/Configuration/MinAreaPartitionConfig.h"
#include "SEImplementation/Configuration/PartitionStepConfig.h"

#include "SEImplementation/Partition/MultiThresholdPartitionStep.h"

using namespace Euclid::Configuration;
namespace po = boost::program_options;

namespace SExtractor {

static const std::string MTHRESH_USE {"partition-multithreshold"};
static const std::string MTHRESH_THRESHOLDS_NB {"partition-thresholds-nb"};
static const std::string MTHRESH_MIN_AREA {"partition-min-area"};
static const std::string MTHRESH_MIN_CONTRAST {"partition-min-contrast"};

MultiThresholdPartitionConfig::MultiThresholdPartitionConfig(long manager_id) : Configuration(manager_id) {
  declareDependency<PartitionStepConfig>();

  ConfigManager::getInstance(manager_id).registerDependency<MultiThresholdPartitionConfig, MinAreaPartitionConfig>();
}

auto MultiThresholdPartitionConfig::getProgramOptions() -> std::map<std::string, OptionDescriptionList> {
  return { {"Multi-thresholding", {
      {MTHRESH_USE.c_str(), po::bool_switch(), "activates multithreshold partitioning"},
      {MTHRESH_THRESHOLDS_NB.c_str(), po::value<int>()->default_value(32), "# of thresholds"},
      {MTHRESH_MIN_AREA.c_str(), po::value<int>()->default_value(3), "min area in pixels to consider partitioning"},
      {MTHRESH_MIN_CONTRAST.c_str(), po::value<double>()->default_value(0.005), "min contrast of for partitioning"}
  }}};

  return {};
}

void MultiThresholdPartitionConfig::initialize(const UserValues& args) {
  if (args.at(MTHRESH_USE).as<bool>()) {
    auto threshold_nb = args.at(MTHRESH_THRESHOLDS_NB).as<int>();
    auto min_area = args.at(MTHRESH_MIN_AREA).as<int>();
    auto min_contrast = args.at(MTHRESH_MIN_CONTRAST).as<double>();

    if (min_area <= 0) {
        throw Elements::Exception() << "Invalid " << MTHRESH_MIN_AREA << " value: " << min_area;
    }
    if (threshold_nb <= 0) {
        throw Elements::Exception() << "Invalid " << MTHRESH_THRESHOLDS_NB << " value: " << threshold_nb;
    }

    getDependency<PartitionStepConfig>().addPartitionStepCreator(
      [=](std::shared_ptr<SourceFactory> source_factory) {
        return std::make_shared<MultiThresholdPartitionStep>(source_factory, min_contrast, threshold_nb, min_area);
      }
    );
  }
}

} // SExtractor namespace
