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
/*  
 * Copyright (C) 2012-2020 Euclid Science Ground Segment    
 *  
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General  
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)  
 * any later version.  
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied  
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more  
 * details.  
 *  
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to  
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA  
 */    

/**
 * @file NDetectedPixelsSourceTask.h
 *
 * @date Apr 27, 2018
 * @author mkuemmel@usm.lmu.de
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_NDETECTEDPIXELSSOURCETASK_H_
#define _SEIMPLEMENTATION_PLUGIN_NDETECTEDPIXELSSOURCETASK_H_

#include "SEFramework/Task/SourceTask.h"
#include "SEImplementation/Plugin/NDetectedPixels/NDetectedPixels.h"
#include "SEImplementation/Plugin/DetectionFramePixelValues/DetectionFramePixelValues.h"

namespace SourceXtractor {
class NDetectedPixelsSourceTask : public SourceTask {
public:
  virtual ~NDetectedPixelsSourceTask() = default;
  virtual void computeProperties(SourceInterface& source) const {
    const auto& pixel_values = source.getProperty<DetectionFramePixelValues>().getValues();
    long int n_pixels = (long int)pixel_values.size();
    source.setProperty<NDetectedPixels>(n_pixels);
};
private:
}; // End of NDetectedPixelsSourceTask class
} // namespace SourceXtractor

#endif /* _SEIMPLEMENTATION_PLUGIN_NDETECTEDPIXELSSOURCETASK_H_ */



