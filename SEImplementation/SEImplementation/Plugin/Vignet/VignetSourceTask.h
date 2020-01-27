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
 * @file VignetSourceTask.h
 *
 * @date Jan. 23, 2020
 * @author mkuemmel@usm.lmu.de
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_VIGNETSOURCETASK_H_
#define _SEIMPLEMENTATION_PLUGIN_VIGNETSOURCETASK_H_

#include "SEFramework/Task/SourceTask.h"
#include "SEFramework/Property/DetectionFrame.h"
#include "SEImplementation/Plugin/PixelCentroid/PixelCentroid.h"
#include "SEImplementation/Plugin/Vignet/Vignet.h"
#include "SEImplementation/Plugin/ShapeParameters/ShapeParameters.h"
#include "SEImplementation/Measurement/MultithreadedMeasurement.h"

namespace SourceXtractor {
class VignetSourceTask : public SourceTask {
public:
  VignetSourceTask(std::vector<int> vignet_size, double vignet_default_pixval):
    m_vignet_size(vignet_size),
    m_vignet_default_pixval((SeFloat)vignet_default_pixval) {};

  virtual ~VignetSourceTask() = default;

  virtual void computeProperties(SourceInterface& source) const {
    std::lock_guard<std::recursive_mutex> lock(MultithreadedMeasurement::g_global_mutex);

    const int& x_pix = (int)source.getProperty<PixelCentroid>().getCentroidX()+0.5;
    const int& y_pix = (int)source.getProperty<PixelCentroid>().getCentroidY()+0.5;

    // get the detection and the variance frames
    const auto& sub_image     = source.getProperty<DetectionFrame>().getFrame()->getSubtractedImage();
    const auto& var_image     = source.getProperty<DetectionFrame>().getFrame()->getVarianceMap();
    const auto& var_threshold = source.getProperty<DetectionFrame>().getFrame()->getVarianceThreshold();

    // get the sub-image boundaries
    int x_start = x_pix-m_vignet_size[0]/2;
    int y_start = y_pix-m_vignet_size[1]/2;
    int x_end   = x_start+ m_vignet_size[0];
    int y_end   = y_start+ m_vignet_size[1];

    // create and fill the vignet vector
    std::vector<SeFloat> vignet_vector(m_vignet_size[0]*m_vignet_size[1], m_vignet_default_pixval);
    int index=0;
    for (int ix=x_start; ix<x_end; ix++){
      for (int iy=y_start; iy<y_end; iy++, index++){
        //if (ix>-1 && iy>-1 && ix<sub_image->getWidth() && iy<sub_image->getHeight()) {
        //  if (var_image->getValue(ix, iy)<var_threshold)
        //    vignet_vector[index] =  sub_image->getValue(ix, iy);
        //}
        if (ix<0 || iy<0 || ix>=sub_image->getWidth() || iy>=sub_image->getHeight()) {
          //vignet_vector[index] = m_vignet_default_pixval;
          continue;
        }
        //else {
         if (var_image->getValue(ix, iy)<var_threshold)
           vignet_vector[index] =  sub_image->getValue(ix, iy);
         // else
            //vignet_vector[index] = m_vignet_default_pixval;
         //   continue;
        //}
      }
    }

    // set the property
    source.setProperty<Vignet>(vignet_vector);
};
private:
  std::vector<int> m_vignet_size;
  SeFloat  m_vignet_default_pixval;
}; // End of VignetSourceTask class
} // namespace SourceXtractor

#endif /* _SEIMPLEMENTATION_PLUGIN_VIGNETSOURCETASK_H_ */
