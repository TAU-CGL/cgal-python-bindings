// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_PY_LIST_RANGE
#define CGALPY_PY_LIST_RANGE

#include <nanobind/nanobind.h>

namespace py = nanobind;

/* Do not pass or store py::list as a reference, because it will result in a
 * dangling reference and a segfault.  `py::list` is already a zero-copy
 * pointer. It is essentially just a lightweight, smart pointer wrapped around
 * the underlying C Python object (PyObject*)
 */
template <typename T>
struct py_list_range {
  // Standard STL-like typedefs required by CGAL's template traits
  using value_type = T;
  using iterator = stl_forward_iterator<T>;
  using const_iterator = stl_forward_iterator<T>;

  py::list m_lst;

  py_list_range(py::list lst) : m_lst(lst) {}

  // Return the types CGAL expects
  const_iterator begin() const { return stl_forward_iterator<T>(m_lst, true); }

  const_iterator end() const { return stl_forward_iterator<T>(m_lst, false); }
};

#endif
