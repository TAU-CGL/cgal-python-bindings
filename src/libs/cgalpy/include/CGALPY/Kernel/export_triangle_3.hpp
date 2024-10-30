// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_TRIANGLE_3_HPP
#define CGALPY_EXPORT_TRIANGLE_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/to_string.hpp"

namespace py = nanobind;

// Export a two-dimensional triangle
template <typename Kernel_, typename C>
void export_triangle_3(C& c) {
  using Ker = Kernel_;
  using Pnt_3 = typename Ker::Point_3;
  using Tri_3 = typename Ker::Triangle_3;

  c.def(py::init < Pnt_3&, Pnt_3&, Pnt_3&>())
    .def("vertex", &Tri_3::vertex)
    .def("__getitem__", &Tri_3::operator[])
    .def("is_degenerate", &Tri_3::is_degenerate)
    .def("bbox", &Tri_3::bbox)
    .def("transform", &Tri_3::transform)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    //.setattr("__hash__", &hash<Tri_3>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
