/**
 * @file SEImplementation/Configuration/DetectionImageConfig.h
 * @date 06/06/16
 * @author mschefer
 */

#ifndef _SEIMPLEMENTATION_DETECTIONIMAGECONFIG_H
#define _SEIMPLEMENTATION_DETECTIONIMAGECONFIG_H

#include "Configuration/Configuration.h"
#include "SEFramework/Image/Image.h"
#include "SEFramework/CoordinateSystem/CoordinateSystem.h"

namespace SExtractor {

/**
 * @class DetectionImageConfig
 * @brief Provides the detection image
 *
 */
class DetectionImageConfig : public Euclid::Configuration::Configuration {
 public:

  /**
   * @brief Destructor
   */
  virtual ~DetectionImageConfig() = default;

  /// Constructs a new DetectionImageConfig object
  DetectionImageConfig(long manager_id);

  std::map<std::string, Configuration::OptionDescriptionList> getProgramOptions() override;
  
  void initialize(const UserValues& args) override;

  std::string getDetectionImagePath() const;
  std::shared_ptr<DetectionImage> getDetectionImage() const;
  std::shared_ptr<CoordinateSystem> getCoordinateSystem() const;
  
  double getGain() const { return m_gain; }
  double getSaturation() const { return m_saturation; }
  int getInterpolationGap() const { return m_interpolation_gap; }


  // Note: flux scale is already applied to all values returned,
  // we still need to know what it was to adjust the weight map
  double getOriginalFluxScale() const { return m_flux_scale; }

private:
  std::string m_detection_image_path;
  std::shared_ptr<DetectionImage> m_detection_image;
  std::shared_ptr<CoordinateSystem> m_coordinate_system;
  double m_gain;
  double m_saturation;

  double m_flux_scale;

  int m_interpolation_gap;

}; /* End of DetectionImageConfig class */

} /* namespace SExtractor */


#endif
