/**
 * @file SEFramework/Pipeline/Segmentation.h
 * @date 05/27/16
 * @author mschefer
 */

#ifndef _SEFRAMEWORK_PIPELINE_SEGMENTATION_H
#define _SEFRAMEWORK_PIPELINE_SEGMENTATION_H

#include <memory>
#include <type_traits>

#include "SEFramework/Property/DetectionFrame.h"
#include "SEFramework/Frame/Frame.h"

#include "SEUtils/Observable.h"
#include "SEFramework/Source/SourceInterface.h"
#include "SEFramework/Image/Image.h"
#include "SEFramework/CoordinateSystem/CoordinateSystem.h"
#include "SEFramework/Frame/Frame.h"


namespace SExtractor {

/**
 * @struct SegmentationProgress
 * @brief Used to notify observers of the progress of the processing of the image.
 */
struct SegmentationProgress {
  int position, total;
  std::string unit;
};

/**
 * @class Segmentation
 * @brief Segmentation takes an image and splits it into individual Sources for further refinement. Each Source
 * results in a notification of the Segmentation's Observers.
 *
 */
class Segmentation : public Observable<std::shared_ptr<SourceInterface>>, public Observable<SegmentationProgress> {

public:
  class LabellingListener;
  class Labelling;

  /// Destructor
  virtual ~Segmentation() = default;

  Segmentation(std::shared_ptr<DetectionImageFrame::ImageFilter> image_processing);

  template<class LabellingType, typename ... Args>
  void setLabelling(Args... args) {
    static_assert(std::is_base_of<Labelling, LabellingType>::value,
        "LabellingType must inherit from SExtractor::Segmentation::Labelling");
    static_assert(std::is_constructible<LabellingType, Args...>::value,
        "LabellingType must be constructible from args");

    m_labelling.reset(new LabellingType(std::forward<Args>(args)...));
  }

  /// Processes a Frame notifying Observers with a Source object for each detection
  void processFrame(std::shared_ptr<DetectionImageFrame> frame) const;

protected:
  void publishSource(std::shared_ptr<SourceInterface> source) const {
    Observable<std::shared_ptr<SourceInterface>>::notifyObservers(source);
  }

private:
  std::unique_ptr<Labelling> m_labelling;
  std::shared_ptr<DetectionImageFrame::ImageFilter> m_filter_image_processing;

}; /* End of Segmentation class */

class Segmentation::LabellingListener {
public:
  LabellingListener(const Segmentation& segmentation, std::shared_ptr<DetectionImageFrame> detection_frame) :
    m_segmentation(segmentation),
    m_detection_frame(detection_frame) {}

  void publishSource(std::shared_ptr<SourceInterface> source) const {
    source->setProperty<DetectionFrame>(m_detection_frame);
    m_segmentation.Observable<std::shared_ptr<SourceInterface>>::notifyObservers(source);
  }

  void notifyProgress(int position, int total, const std::string &unit) {
    m_segmentation.Observable<SegmentationProgress>::notifyObservers(SegmentationProgress{position, total, unit});
  }

private:
  const Segmentation& m_segmentation;
  std::shared_ptr<DetectionImageFrame> m_detection_frame;
};

class Segmentation::Labelling {
public:
  virtual ~Labelling() = default;
  Labelling() {}

  virtual void labelImage(Segmentation::LabellingListener& listener, std::shared_ptr<const DetectionImageFrame> frame) = 0;
};

} /* namespace SExtractor */

#endif
