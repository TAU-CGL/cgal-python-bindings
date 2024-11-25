// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#ifndef CGALPY_HELPERS_HPP
#define CGALPY_HELPERS_HPP

#include <vector>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace pmp {

//! helper
template <typename T>
py::list vec2list(const T& vec) {
  py::list retv;
  for (const auto& item : vec) {
    retv.append(item);
  }
  return retv;
}

//!
template <typename T>
std::vector<T> list2vec(const py::list& list) {
  std::vector<T> retv;
  retv.reserve(py::len(list));
  for (const auto& item : list) {
    T i;
    try {
      i = py::cast<T>(item);
    }
    catch(const py::cast_error& e) {
      throw std::runtime_error("Failed to cast list item to desired type");
    }
    retv.push_back(i);
  }
  return retv;
}

} // namespace pmp

#endif
