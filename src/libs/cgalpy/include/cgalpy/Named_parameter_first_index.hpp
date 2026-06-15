// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_FIRST_INDEX_HPP
#define CGALPY_NAMED_PARAMETER_FIRST_INDEX_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {

struct Named_parameter_first_index {
  const std::string m_name = "first_index";
  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    try {
      return np.first_index(py::cast<std::size_t>(value));
    }
    catch (const std::exception&) {
      throw std::invalid_argument("Named parameter 'first_index' must be a non-negative integer.");
    }
  }
};

}

#endif
