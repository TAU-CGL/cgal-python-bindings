// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_NAMED_PARAMETER_RANDOM_SEED_HPP
#define CGALPY_NAMED_PARAMETER_RANDOM_SEED_HPP

#include <stdexcept>
#include <string>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace cgalpy {

struct Named_parameter_random_seed {
  const std::string m_name = "random_seed";
  template <typename NamedParameters, typename Value>
  auto operator()(NamedParameters& np, Value& value) const {
    try {
      return np.random_seed(py::cast<unsigned int>(value));
    }
    catch (const std::exception&) {
      throw std::invalid_argument("Named parameter 'random_seed' must be a non-negative integer.");
    }
  }
};

}

#endif
