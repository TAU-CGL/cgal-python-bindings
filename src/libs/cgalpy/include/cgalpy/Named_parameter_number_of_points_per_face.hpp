// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_NUMBER_OF_POINTS_PER_FACE_HPP
#define CGALPY_NAMED_PARAMETER_NUMBER_OF_POINTS_PER_FACE_HPP

#include <stdexcept>
#include <string>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {

struct Named_parameter_number_of_points_per_face {
  static constexpr const char* m_name = "number_of_points_per_face";

  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    try {
      return np.number_of_points_per_face(
        py::cast<unsigned int>(value));
    }
    catch (const std::exception&) {
      throw std::invalid_argument(
        "Named parameter 'number_of_points_per_face' "
        "must be a non-negative integer.");
    }
  }
};

} // namespace cgalpy

#endif
