// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_DO_OVERLAP_TEST_OF_BOUNDED_SIDES_HPP
#define CGALPY_NAMED_PARAMETER_DO_OVERLAP_TEST_OF_BOUNDED_SIDES_HPP

#include <stdexcept>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {

struct Named_parameter_do_overlap_test_of_bounded_sides {
  static constexpr const char* m_name = "do_overlap_test_of_bounded_sides";

  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    try {
      return np.do_overlap_test_of_bounded_sides(
        py::cast<bool>(value));
    }
    catch (const std::exception&) {
      throw std::invalid_argument(
        "Named parameter "
        "'do_overlap_test_of_bounded_sides' "
        "must be a boolean.");
    }
  }
};

} // namespace cgalpy

#endif
