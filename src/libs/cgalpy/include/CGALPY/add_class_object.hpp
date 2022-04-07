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

template <typename Type, const char* Name, bool no_init>
struct New_class_object {};

template<typename Type, const char* Name>
struct New_class_object<Type, Name, false> {
  py::class_<Type>* operator()() {
    return new py::class_<Type>(Name);
  }
};

template<typename Type, const char* Name>
struct New_class_object<Type, Name, true> {
  py::class_<Type>* operator()() {
    return new py::class_<Type>(Name, py::no_init);
  }
};

// Introduce a new class object
// Return true iff the class object was not already registered
template <typename Type, const char* Name, bool no_init=false>
bool add_class_object(py::scope& my_scope, py::class_<Type>*& co) {
  py::handle<> tco(py::objects::registered_class_object(py::type_id<Type>()));
  if (tco.get() != 0) {
    my_scope.attr(Name) = tco;
    return false;
  } else {
    co = New_class_object<Type, Name, no_init>()();
  }
  return true;
}

#endif // CGALPY_ADD_CLASS_OBJECT_HPP
