// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_GEOM_TRAITS_HPP
#define CGALPY_NAMED_PARAMETER_GEOM_TRAITS_HPP

#include <string>

#include <nanobind/nanobind.h>

#include "cgalpy/kernel_types.hpp"

namespace py = nanobind;

namespace cgalpy {

struct Named_parameter_geom_traits {
  const std::string m_name = "geom_traits";
  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    return np.geom_traits(py::cast<const Kernel&>(value));
  }
};

}

#endif
