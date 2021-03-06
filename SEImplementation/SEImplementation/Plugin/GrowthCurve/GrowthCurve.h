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

#ifndef _SEIMPLEMENTATION_PLUGIN_GROWTHCURVE_GROWTHCURVE_H_
#define _SEIMPLEMENTATION_PLUGIN_GROWTHCURVE_GROWTHCURVE_H_

#include <vector>
#include "SEFramework/Property/Property.h"

namespace SourceXtractor {

/**
 * Stores the growth curve as an interpolated function. The output column is
 * generated by GrowthCurveResampled
 */
class GrowthCurve: public Property {
public:
  virtual ~GrowthCurve() = default;

  GrowthCurve(std::vector<double>&& growth_curve, double end)
    : m_growth_curve{std::move(growth_curve)}, m_max{end}, m_step_size{end / m_growth_curve.size()} {
  }

  const std::vector<double>& getCurve() const {
    return m_growth_curve;
  }

  double getMax() const {
    return m_max;
  }

  double getStepSize() const {
    return m_step_size;
  }

private:
  std::vector<double> m_growth_curve;
  double m_max, m_step_size;
};

} // end of namespace SourceXtractor

#endif /* _SEIMPLEMENTATION_PLUGIN_GROWTHCURVE_GROWTHCURVE_H_ */

