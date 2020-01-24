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
#include "SEImplementation/Plugin/Vignet/Vignet.h"
#include "SEImplementation/Plugin/ShapeParameters/ShapeParameters.h"

namespace SourceXtractor {
class VignetSourceTask : public SourceTask {
public:
  VignetSourceTask(std::vector<int> vignet_size, double vignet_default_pixval):
    m_vignet_size(vignet_size),
    m_vignet_default_pixval((SeFloat)vignet_default_pixval) {};

  virtual ~VignetSourceTask() = default;

  virtual void computeProperties(SourceInterface& source) const {
    // get the relevant properties
    const auto& a_image = source.getProperty<ShapeParameters>().getEllipseA();
    const auto& b_image = source.getProperty<ShapeParameters>().getEllipseB();

    // compute and set the property
    std::vector<SeFloat> vignet_vector(m_vignet_size[0]*m_vignet_size[0], (SeFloat)a_image/b_image);
    source.setProperty<Vignet>(vignet_vector);
};
private:
  std::vector<int> m_vignet_size;
  SeFloat  m_vignet_default_pixval;
}; // End of VignetSourceTask class
} // namespace SourceXtractor

#endif /* _SEIMPLEMENTATION_PLUGIN_VIGNETSOURCETASK_H_ */
