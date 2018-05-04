/*
 * ImageSource.h
 *
 *  Created on: Feb 14, 2018
 *      Author: mschefer
 */

#ifndef _SEFRAMEWORK_IMAGE_IMAGESOURCE_H_
#define _SEFRAMEWORK_IMAGE_IMAGESOURCE_H_

#include "SEFramework/Image/Image.h"
#include "SEFramework/Image/ImageTile.h"

namespace SExtractor {

class ImageSourceBase {
public:
  virtual ~ImageSourceBase() = default;
};

template <typename T>
class ImageSource : public ImageSourceBase {
public:

  ImageSource() {}

  virtual ~ImageSource() = default;

  virtual std::shared_ptr<ImageTile<T>> getImageTile(int x, int y, int width, int height) const = 0;

  virtual void saveTile(ImageTile<T>& tile) = 0;

  /// Returns the width of the image in pixels
  virtual int getWidth() const = 0;

  /// Returns the height of the image in pixels
  virtual int getHeight() const = 0;

private:

};

}

#endif /* _SEFRAMEWORK_IMAGE_IMAGESOURCE_H_ */
