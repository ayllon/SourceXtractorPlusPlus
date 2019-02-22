/*
 * AperturePhotometryArray.h
 *
 *  Created on: Nov 23, 2018
 *      Author: Alejandro Alvarez Ayllon
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_APERTUREPHOTOMETRY_APERTUREPHOTOMETRYARRAY_H_
#define _SEIMPLEMENTATION_PLUGIN_APERTUREPHOTOMETRY_APERTUREPHOTOMETRYARRAY_H_

#include "SEImplementation/Plugin/AperturePhotometry/AperturePhotometry.h"
#include "ModelFitting/utils.h"
#include "NdArray/NdArray.h"
#include <memory>
#include <cassert>


namespace SExtractor {

/**
 * @class AperturePhotometryArray
 * @brief Merges all AperturePhotometries into a multidimensional property
 */
class AperturePhotometryArray : public Property {
public:
  template<typename T>
  using NdArray = Euclid::NdArray::NdArray<T>;

  /**
   * @brief Destructor
   */
  virtual ~AperturePhotometryArray() = default;

  AperturePhotometryArray(const std::vector<AperturePhotometry> &measurements) {
    assert(measurements.size() > 0);
    size_t nentries = measurements.size();
    size_t napertures = measurements.front().getFlags().size();
    std::vector<size_t> shape{nentries, napertures};

    m_fluxes = make_unique<NdArray<SeFloat>>(shape);
    m_flux_errors = make_unique<NdArray<SeFloat>>(shape);
    m_mags = make_unique<NdArray<SeFloat>>(shape);
    m_mag_errors = make_unique<NdArray<SeFloat>>(shape);
    m_flags = make_unique<NdArray<int64_t>>(shape);

    for (int entry_idx = 0; entry_idx < nentries; ++entry_idx) {
      const auto& entry = measurements[entry_idx];
      for (int ap_idx = 0; ap_idx < napertures; ++ap_idx) {
        m_fluxes->at(entry_idx, ap_idx) = entry.getFluxes()[ap_idx];
        m_flux_errors->at(entry_idx, ap_idx) = entry.getFluxErrors()[ap_idx];
        m_mags->at(entry_idx, ap_idx) = entry.getMags()[ap_idx];
        m_mag_errors->at(entry_idx, ap_idx) = entry.getMagErrors()[ap_idx];
        m_flags->at(entry_idx, ap_idx) = flags2long(entry.getFlags()[ap_idx]);
      }
    }
  }

  const NdArray<SeFloat> &getFluxes() const {
    return *m_fluxes;
  }

  const NdArray<SeFloat> &getFluxErrors() const {
    return *m_flux_errors;
  }

  const NdArray<SeFloat> &getMags() const {
    return *m_mags;
  }

  const NdArray<SeFloat> &getMagErrors() const {
    return *m_mag_errors;
  }

  const NdArray<int64_t> &getFlags() const {
    return *m_flags;
  }

private:
  std::unique_ptr<NdArray<SeFloat>> m_fluxes;
  std::unique_ptr<NdArray<SeFloat>> m_flux_errors;
  std::unique_ptr<NdArray<SeFloat>> m_mags;
  std::unique_ptr<NdArray<SeFloat>> m_mag_errors;
  std::unique_ptr<NdArray<int64_t>> m_flags;
};

} /* namespace SExtractor */

#endif /* _SEIMPLEMENTATION_PLUGIN_APERTUREPHOTOMETRY_APERTUREPHOTOMETRY_H_ */
