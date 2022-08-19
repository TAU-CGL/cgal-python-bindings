// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_GENERAL_POLYGON_2_HPP
#define CGALPY_EXPORT_GENERAL_POLYGON_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

template <typename Type>
void export_general_polygon_2(py::class_<Type>& co) {
  co.def(py::init<>());
  co.def(py::init<Type>());
  co.def("push_back", &Type::push_back);
  co.def("orientation", &Type::orientation);
  co.def("is_empty", &Type::is_empty);
  co.def("size", &Type::size);
  co.def("bbox", &Type::bbox);
  co.def("clear", &Type::clear);
  co.def("reverse_orientation", &Type::reverse_orientation);

  add_insertion(co, "__str__");
  add_insertion(co, "__repr__");
}

#endif
