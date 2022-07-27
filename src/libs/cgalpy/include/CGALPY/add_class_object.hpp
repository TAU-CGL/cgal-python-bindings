// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ADD_CLASS_OBJECT_HPP
#define CGALPY_ADD_CLASS_OBJECT_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

// Introduce a new class object
// Return true iff the class object was not already registered
template <typename Type, const char* Name, typename C>
bool add_class_object(C c, py::class_<Type>*& co) {
  const py::handle info = py::type<Type>();
  if (info.is_valid() && py::type_check(info)) {
    c.attr(Name) = info;
    return false;
  }
  co = new py::class_<Type>(c, Name);
  return true;
}

#endif // CGALPY_ADD_CLASS_OBJECT_HPP
