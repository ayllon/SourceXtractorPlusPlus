/*
 * SimpleBackgroundRMSAnalyzer.h
 *
 *  Created on: Apr 25, 2017
 *      Author: mschefer
 */

#ifndef _SEIMPLEMENTATION_BACKGROUND_SIMPLEBACKGROUNDRMSANALYZER_H_
#define _SEIMPLEMENTATION_BACKGROUND_SIMPLEBACKGROUNDRMSANALYZER_H_

#include "SEFramework/Frame/Frame.h"
#include "SEFramework/Image/Image.h"
#include "SEFramework/Background/BackgroundAnalyzer.h"

namespace SExtractor {

class SimpleBackgroundRMSAnalyzer : public BackgroundAnalyzer {
public:

  virtual ~SimpleBackgroundRMSAnalyzer() = default;

  void analyzeBackground(std::shared_ptr<DetectionImageFrame> frame) const override;

private:
  SeFloat getRMS(std::shared_ptr<DetectionImageFrame> frame) const;
};

}

#endif /* _SEIMPLEMENTATION_BACKGROUND_SIMPLEBACKGROUNDRMSANALYZER_H_ */