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
 * @file SourceFlags.h
 *
 * @date Jul 13, 2018
 * @author mkuemmel@usm.lmu.de
 */

#ifndef _SEIMPLEMENTATION_PLUGIN_BOUNDARYFLAG_H_
#define _SEIMPLEMENTATION_PLUGIN_BOUNDARYFLAG_H_

#include "SEFramework/Property/Property.h"

namespace SExtractor {
class BoundaryFlag : public Property {
public:
  virtual ~BoundaryFlag() = default;
  BoundaryFlag(unsigned long boundary_flag) : m_boundary_flag(boundary_flag) {}
  long int getBoundaryFlag () const {
    return m_boundary_flag;
  }
private:
  long int m_boundary_flag;
}; // end of BoundaryFlag class
} // namespace SExtractor

#endif /* _SEIMPLEMENTATION_PLUGIN_BOUNDARYFLAG_H_*/