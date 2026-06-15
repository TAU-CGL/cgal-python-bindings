// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_USE_2D_CONSTRAINED_DELAUNAY_TRIANGULATION_HPP
#define CGALPY_NAMED_PARAMETER_USE_2D_CONSTRAINED_DELAUNAY_TRIANGULATION_HPP

#include <stdexcept>
#include <string>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {

struct Named_parameter_use_2d_constrained_delaunay_triangulation {
  const std::string m_name = "use_2d_constrained_delaunay_triangulation";
  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    try {
      return np.use_2d_constrained_delaunay_triangulation(py::cast<bool>(value));
    }
    catch (const std::exception&) {
      throw std::invalid_argument("Named parameter 'use_2d_constrained_delaunay_triangulation' must be a bool.");
    }
  }
};

}

#endif
