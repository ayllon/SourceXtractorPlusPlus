/**
 * @file NeutralConverter.h
 *
 * Created on: July 28, 2015
 *     Author: Pierre Dubath
 */

#ifndef NEUTRAL_CONVERTER_H_
#define NEUTRAL_CONVERTER_H_

#include "ModelFitting/Parameters/CoordinateConverter.h"


namespace ModelFitting {

/**
 * @class NeutralConverter
 * @brief
 *    CoordinateConverter implementation which performs no conversion
 *
 */
class NeutralConverter : public CoordinateConverter{

public:

  /**
   * @brief
   *    World to engine coordinate converter
   *
   * @param world_value
   *    The value of the parameter in world coordinate
   *
   * @return engine_value
   *    The value of the parameter in engine coordinate
   *
   */
  double worldToEngine(const double world_value) const override;

  /**
   * @brief
   *    Engine to world coordinate converter
   *
   * @param engine_value
   *
   * @return world_value
   *
   */
  double engineToWorld(const double engine_value) const override;

  double getEngineToWorldDerivative(const double value) const override;

  /**
   * @brief Destructor
   */
  virtual ~NeutralConverter();

};

} // namespace ModelFitting

#endif /* NEUTRAL_CONVERTER_H_ */
