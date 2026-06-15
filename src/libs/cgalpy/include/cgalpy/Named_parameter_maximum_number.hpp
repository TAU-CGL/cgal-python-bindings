// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_MAXIMUM_NUMBER_HPP
#define CGALPY_NAMED_PARAMETER_MAXIMUM_NUMBER_HPP

#include <stdexcept>
#include <string>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {

struct Named_parameter_maximum_number {
  const std::string m_name = "maximum_number";
  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    try {
      return np.maximum_number(py::cast<unsigned int>(value));
    }
    catch (const std::exception&) {
      throw std::invalid_argument("Named parameter 'maximum_number' must be a non-negative integer.");
    }
  }
};

}

#endif
