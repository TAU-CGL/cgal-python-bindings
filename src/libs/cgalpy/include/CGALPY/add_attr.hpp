// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ADD_ATTR_HPP
#define CGALPY_ADD_ATTR_HPP

#include <nanobind/nanobind.h>

#include <boost/static_assert.hpp>

namespace py = nanobind;

template <typename Type, typename PyClass>
bool add_attr(PyClass& cls, const char* name) {
  const py::handle info = py::type<Type>();
  if (! info.is_valid() || ! py::type_check(info)) return false;
  cls.attr(name) = info;
  return true;
}

#endif
