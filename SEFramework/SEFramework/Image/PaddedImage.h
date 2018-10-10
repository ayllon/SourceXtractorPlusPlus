/*
 * @file SEFramework/Image/PaddedImage.h
 * @date 10/09/18
 * @author aalvarez
 */

#ifndef _SEFRAMEWORK_IMAGE_PADDEDIMAGE_H
#define _SEFRAMEWORK_IMAGE_PADDEDIMAGE_H

#include "SEFramework/Image/ImageBase.h"

namespace SExtractor {

inline int ReplicateCoordinates(int N, int v) {
  if (v < 0) return 0;
  if (v > N - 1) return N - 1;
  return v;
}

inline int ReflectCoordinates(int N, int v) {
  if (v >= 0 && v < N) {
    return v;
  }

  if (v < 0) ++v;
  v = std::abs(v);

  int offset = v % N;
  int ntimes = v / N;

  if (ntimes % 2 == 0) {
    return offset;
  }
  return N - offset - 1;
}

inline int Reflect101Coordinates(int N, int v) {
  if (v >= 0 && v < N) {
    return v;
  }

  int max = N - 1;
  v = std::abs(v);
  int offset = v % max;
  int ntimes = v / max;

  if (ntimes % 2 == 0) {
    return offset;
  }
  return max - offset;
}

inline int WrapCoordinates(int N, int v) {
  return (v % N + N) % N;
}

template<typename T, int CoordinateInterpolation(int, int) = nullptr>
class PaddedImage : public ImageBase<T> {
protected:
  PaddedImage(std::shared_ptr<const Image<T>> img, int width, int height)
    : m_img{img}, m_width{width}, m_height{height} {
    auto wdiff = m_width - img->getWidth();
    auto hdiff = m_height - img->getHeight();

    m_lpad = wdiff / 2;
    m_tpad = hdiff / 2;
  }

public:
  template<typename... Args>
  static std::shared_ptr<PaddedImage<T, CoordinateInterpolation>> create(Args &&... args) {
    return std::shared_ptr<PaddedImage<T, CoordinateInterpolation>>(new PaddedImage{std::forward<Args>(args)...});
  }

  T getValue(int x, int y) const override {
    int tx = CoordinateInterpolation(m_img->getWidth(), x - m_lpad);
    int ty = CoordinateInterpolation(m_img->getHeight(), y - m_tpad);
    return m_img->getValue(tx, ty);
  }

  int getWidth() const override {
    return m_width;
  }

  int getHeight() const override {
    return m_height;
  }

private:
  std::shared_ptr<const Image<T>> m_img;
  int m_width, m_height;
  int m_lpad, m_tpad;
};


template<typename T>
class PaddedImage<T, nullptr> : public ImageBase<T> {
protected:
  PaddedImage(std::shared_ptr<const Image<T>> img, int width, int height, T default_value)
    : m_img{img}, m_width{width}, m_height{height}, m_default{default_value} {
    auto wdiff = m_width - img->getWidth();
    auto hdiff = m_height - img->getHeight();

    m_lpad = wdiff / 2;
    m_tpad = hdiff / 2;
  }

  PaddedImage(std::shared_ptr<const Image<T>> img, int width, int height): PaddedImage(img, width, height, {}) {
  }

public:
  template<typename... Args>
  static std::shared_ptr<PaddedImage<T, nullptr>> create(Args &&... args) {
    return std::shared_ptr<PaddedImage<T, nullptr>>(new PaddedImage{std::forward<Args>(args)...});
  }

  T getValue(int x, int y) const override {
    if (x < m_lpad || y < m_tpad || x >= m_img->getWidth() + m_lpad || y >= m_img->getHeight() + m_tpad) {
      return m_default;
    }
    return m_img->getValue(x - m_lpad, y - m_tpad);
  }

  int getWidth() const override {
    return m_width;
  }

  int getHeight() const override {
    return m_height;
  }

private:
  std::shared_ptr<const Image<T>> m_img;
  int m_width, m_height;
  int m_lpad, m_tpad;
  T m_default;
};

} // end SExtractor

#endif // _SEFRAMEWORK_IMAGE_PADDEDIMAGE_H