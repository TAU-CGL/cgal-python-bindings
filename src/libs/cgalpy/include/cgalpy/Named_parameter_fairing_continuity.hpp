// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_FAIRING_CONTINUITY_HPP
#define CGALPY_NAMED_PARAMETER_FAIRING_CONTINUITY_HPP

#include <stdexcept>
#include <string>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {

struct Named_parameter_fairing_continuity {
  const std::string m_name = "fairing_continuity";
  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    try {
      return np.fairing_continuity(py::cast<unsigned int>(value));
    }
    catch (const std::exception&) {
      throw std::invalid_argument("Named parameter 'fairing_continuity' must be an unsigned int.");
    }
  }
};

}

#endif
