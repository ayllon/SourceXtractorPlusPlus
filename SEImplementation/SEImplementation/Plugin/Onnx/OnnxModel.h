/** Copyright © 2020 Université de Genève, LMU Munich - Faculty of Physics, IAP-CNRS/Sorbonne Université
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


#ifndef _SEIMPLEMENTATION_PLUGIN_ONNXMODEL_H_
#define _SEIMPLEMENTATION_PLUGIN_ONNXMODEL_H_

#include <string>
#include <vector>
#include <onnxruntime_cxx_api.h>

namespace SourceXtractor {

/**
 * Wraps some metadata about a loaded ONNX model, so the Task
 * knows what it needs to pass and receive data from it
 */
struct OnnxModel {
  std::string m_prop_name;   ///< Name that will be written into the catalog
  std::string m_input_name;  ///< Input tensor name
  std::string m_output_name; ///< Output tensor name
  ONNXTensorElementDataType m_input_type;  ///< Input type
  ONNXTensorElementDataType m_output_type; ///< Output type
  std::vector<std::int64_t> m_input_shape;  ///< Input tensor shape
  std::vector<std::int64_t> m_output_shape; ///< Output tensor shape
  std::string m_model_path; ///< Path to the ONNX model
  std::unique_ptr<Ort::Session> m_session; ///< Session, one per model. In theory, it is thread-safe
};

} // end of namespace SourceXtractor

#endif // _SEIMPLEMENTATION_PLUGIN_ONNXMODEL_H_
