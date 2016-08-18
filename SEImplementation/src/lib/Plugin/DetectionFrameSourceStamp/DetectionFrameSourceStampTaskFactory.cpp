/**
 * @file src/lib/Task/DetectionFrameSourceStampTaskFactory.cpp
 * @date 06/16/16
 * @author mschefer
 */

#include "Configuration/ConfigManager.h"

#include "SEImplementation/Configuration/DetectionImageConfig.h"
#include "SEImplementation/Plugin/DetectionFrameSourceStamp/DetectionFrameSourceStamp.h"
#include "SEImplementation/Plugin/DetectionFrameSourceStamp/DetectionFrameSourceStampTask.h"
#include "SEImplementation/Plugin/DetectionFrameSourceStamp/DetectionFrameSourceStampTaskFactory.h"

using namespace Euclid::Configuration;

namespace SExtractor {

void DetectionFrameSourceStampTaskFactory::reportConfigDependencies(Euclid::Configuration::ConfigManager& manager) const {
  manager.registerConfiguration<DetectionImageConfig>();
}

void DetectionFrameSourceStampTaskFactory::configure(Euclid::Configuration::ConfigManager& manager) {
  m_detection_image = manager.getConfiguration<DetectionImageConfig>().getDetectionImage();
}

std::shared_ptr<Task> DetectionFrameSourceStampTaskFactory::createTask(const PropertyId& property_id) const {
  if (property_id == PropertyId::create<DetectionFrameSourceStamp>()) {
    return std::make_shared<DetectionFrameSourceStampTask>(m_detection_image);
  } else {
    return nullptr;
  }
}

} // SEImplementation namespace


