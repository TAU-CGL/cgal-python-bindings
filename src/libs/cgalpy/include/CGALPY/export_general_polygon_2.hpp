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

#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

namespace bso2 {

// Initialize a general polygon from a list of x-monotone curves.
template <typename T>
void init_polygon_2(T& pgn, py::list& lst) {
  using Xcv = typename T::X_monotone_curve_2;
  auto begin = stl_input_iterator<Xcv>(lst);
  auto end = stl_input_iterator<Xcv>(lst, false);
  new (&pgn) T(begin, end);
}

}

//
template <typename Type>
void export_general_polygon_2(py::class_<Type>& co) {
  co.def(py::init<>())
    .def(py::init<const Type&>())
    .def("__init__", &bso2::init_polygon_2<Type>)
    .def("push_back", &Type::push_back)
    .def("orientation", &Type::orientation)
    .def("is_empty", &Type::is_empty)
    .def("size", &Type::size)
    .def("bbox", &Type::bbox)
    .def("clear", &Type::clear)
    .def("reverse_orientation", &Type::reverse_orientation)
    ;

  add_insertion(co, "__str__");
  add_insertion(co, "__repr__");
}

#endif
