// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_VERBOSE_HPP
#define CGALPY_NAMED_PARAMETER_VERBOSE_HPP

#include <string>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace CGALPY {

struct Named_parameter_verbose {
  const std::string m_name = "verbose";
  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    return np.verbose(py::cast<bool>(value));
  }
};

}

#endif
