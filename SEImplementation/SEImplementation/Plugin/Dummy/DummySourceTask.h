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
 * @file DummySourceTask.h
 *
 * @date
 * @author
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_DUMMYSOURCETASK_H_
#define _SEIMPLEMENTATION_PLUGIN_DUMMYSOURCETASK_H_

#include "SEFramework/Task/SourceTask.h"
#include "SEImplementation/Plugin/Dummy/Dummy.h"
#include "SEImplementation/Plugin/DetectionFramePixelValues/DetectionFramePixelValues.h"

namespace SourceXtractor {
class DummySourceTask : public SourceTask {
public:
  DummySourceTask(float dummy_value): m_dummy_value(dummy_value){}
  virtual ~DummySourceTask() = default;
  virtual void computeProperties(SourceInterface& source) const {
    const auto& pixel_values = source.getProperty<DetectionFramePixelValues>().getValues();
    int n_pixels = (int)pixel_values.size()*(int)m_dummy_value;
    source.setProperty<Dummy>(n_pixels);
};
private:
  float m_dummy_value;
}; // End of DummySourceTask class
} // namespace SourceXtractor

#endif /* _SEIMPLEMENTATION_PLUGIN_DUMMYSOURCETASK_H_ */



