/**
 * @file SEUtils/PixelCoordinate.h
 * @date 06/07/16
 * @author mschefer
 */

#ifndef _SEUTILS_PIXELCOORDINATE_H
#define _SEUTILS_PIXELCOORDINATE_H

#include <functional>
#include <boost/functional/hash.hpp>

namespace SExtractor {

/**
 * @class PixelCoordinate
 * @brief A pixel coordinate made of two integers m_x and m_y.
 *
 */

struct PixelCoordinate {
  int m_x, m_y;

  PixelCoordinate() : m_x(0), m_y(0) {}

  PixelCoordinate(int x, int y) : m_x(x), m_y(y) {}

  bool operator==(const PixelCoordinate& other) const {
    return m_x == other.m_x && m_y == other.m_y;
  }

  bool operator!=(const PixelCoordinate& other) const {
    return !(*this == other);
  }

  PixelCoordinate operator*(double scalar) const {
    return PixelCoordinate(m_x * scalar, m_y  * scalar);
  }

  PixelCoordinate operator+(const PixelCoordinate& other) const {
    return PixelCoordinate(m_x + other.m_x, m_y + other.m_y);
  }

  PixelCoordinate& operator+=(const PixelCoordinate& other) {
    m_x += other.m_x;
    m_y += other.m_y;
    return *this;
  }

  PixelCoordinate operator-(const PixelCoordinate& other) const {
    return PixelCoordinate(m_x - other.m_x, m_y - other.m_y);
  }

  PixelCoordinate& operator-=(const PixelCoordinate& other) {
    m_x -= other.m_x;
    m_y -= other.m_y;
    return *this;
  }
};


} /* namespace SExtractor */


namespace std {

template <>
struct hash<SExtractor::PixelCoordinate>
{
  std::size_t operator()(const SExtractor::PixelCoordinate& coord) const {
    std::size_t hash = 0;
    boost::hash_combine(hash, coord.m_x);
    boost::hash_combine(hash, coord.m_y);
    return hash;
  }
};

} // namespace std


#endif
