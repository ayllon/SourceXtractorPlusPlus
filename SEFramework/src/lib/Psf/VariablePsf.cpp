/*
 * VariablePsf.cpp
 *
 *  Created on: Jun 25, 2018
 *      Author: Alejandro Álvarez Ayllón
 */

#include <ElementsKernel/Exception.h>
#include <algorithm>
#include "SEFramework/Psf/VariablePsf.h"


namespace SExtractor {

VariablePsf::VariablePsf(double pixel_scale, const std::vector<Component> &components,
            const std::vector<int> &group_degrees,
            const std::vector<std::shared_ptr<VectorImage<SeFloat>>> &coefficients):
  m_pixel_scale(pixel_scale), m_components(components), m_group_degrees(group_degrees), m_coefficients(coefficients)
{
  selfTest();
  calculateExponents();
}

VariablePsf::VariablePsf(double pixel_scale, const std::shared_ptr<VectorImage<SeFloat>> &constant):
  m_pixel_scale(pixel_scale), m_coefficients{constant}
{
  selfTest();
  calculateExponents();
}

int VariablePsf::getWidth() const {
  return m_coefficients[0]->getWidth();
}

int VariablePsf::getHeight() const {
  return m_coefficients[0]->getHeight();
}

double VariablePsf::getPixelScale() const {
  return m_pixel_scale;
}

const std::vector<VariablePsf::Component>& VariablePsf::getComponents() const {
  return m_components;
}

std::shared_ptr<VectorImage<SeFloat>> VariablePsf::getPsf(const std::vector<double> &values) const
{
  // If we only have the constant, avoid a copy
  if (m_coefficients.size() == 1) {
    return m_coefficients[0];
  }

  auto scaled_props = scaleProperties(values);

  // Initialize with the constant component
  auto result = VectorImage<SeFloat>::create(*m_coefficients[0]);

  // Add the rest of the components
  for (auto i = 1; i < m_coefficients.size(); ++i) {
    const auto &exp = m_exponents[i];
    const auto &coef = m_coefficients[i];

    double acc = 1.;
    for (auto j = 0; j < scaled_props.size(); ++j) {
      acc *= std::pow(scaled_props[j], exp[j]);
    }

    for (auto x = 0; x < getWidth(); ++x) {
      for (auto y = 0; y < getHeight(); ++y) {
        result->at(x, y) += acc * coef->at(x, y);
      }
    }
  }

  return result;
}

void VariablePsf::selfTest() {
  // Pre-condition: There is at least a constant component
  if (m_coefficients.size() == 0) {
    throw Elements::Exception() << "A variable PSF needs at least one set of coefficients";
  }

  // Pre-condition: There is a degree value per unique group
  std::vector<int> n_component_per_group(m_group_degrees.size());
  for (auto &component : m_components) {
    if (component.group_id >= m_group_degrees.size()) {
      throw Elements::Exception() << "Component group out of range for " << component.name;
    }
    ++n_component_per_group[component.group_id];
  }

  // Pre-condition: There are enough coefficients - (n+d)!/(n!d!) per group
  int n_coefficients = 1;
  for (int g = 0; g < n_component_per_group.size(); ++g) {
    int dmax = m_group_degrees[g];
    int n = n_component_per_group[g];
    int d = std::min<int>(dmax, n);
    int num, den;

    for (num = 1, den = 1; d > 0; num *= (n+dmax--), den*= d--);

    n_coefficients *= num / den;
  }

  if (n_coefficients != m_coefficients.size()) {
    throw Elements::Exception() << "Invalid number of coefficients. Got " << m_coefficients.size()
                                << " expected " << n_coefficients;
  }

  // Pre-condition: All components have the same size
  auto psf_width = m_coefficients[0]->getWidth();
  auto psf_height = m_coefficients[0]->getHeight();

  for (auto coeff : m_coefficients) {
    if (coeff->getWidth() != psf_width || coeff->getHeight() != psf_height) {
      throw Elements::Exception() << "Malformed variable PSF, coefficient matrices do not have the same dimensions";
    }
  }
}

std::vector<double> VariablePsf::scaleProperties(const std::vector<double> &values) const
{
  if (values.size() != m_components.size()) {
    throw Elements::Exception()
        << "Expecting " << m_components.size() << " values, got " << values.size();
  }
  std::vector<double> scaled(values.size());
  for (auto i = 0; i < values.size(); ++i) {
    scaled[i] = (values[i] - m_components[i].offset) / m_components[i].scale;
  }
  return scaled;
}

void VariablePsf::calculateExponents() {
  std::vector<int> group_exponents(m_group_degrees);
  std::vector<int> exp(m_components.size());

  m_exponents.resize(m_coefficients.size());
  if (m_components.empty()) {
    return;
  }

  // Constant
  m_exponents[0].resize(m_components.size());
  --group_exponents[m_components[0].group_id];

  // Polynomial
  exp[0] = 1;
  for (auto e = m_exponents.begin() + 1; e != m_exponents.end(); ++e) {
    *e = exp;

    size_t ei = 0;
    for (auto component : m_components) {
      if (group_exponents[component.group_id] > 0) {
        --group_exponents[component.group_id];
        ++exp[ei];
        break;
      }
      else {
        group_exponents[component.group_id] = exp[ei];
        exp[ei++] = 0;
      }
    }
  }
}

} // end SExtractor