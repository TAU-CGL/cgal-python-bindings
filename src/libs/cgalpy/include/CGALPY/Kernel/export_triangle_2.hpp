// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_TRIANGLE_2_HPP
#define CGALPY_EXPORT_TRIANGLE_2_HPP

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
void export_triangle_2(C& c) {
  using Ker = Kernel_;
  using Pnt_2 = typename Ker::Point_2;
  using Tri_2 = typename Ker::Triangle_2;

  c.def(py::init < Pnt_2&, Pnt_2&, Pnt_2&>(), doc::Triangle_2_Triangle_2)
    .def("vertex", &Tri_2::vertex, doc::Triangle_2_vertex)    .def("__getitem__", &Tri_2::operator[], doc::Triangle_2_operator)    .def("is_degenerate", &Tri_2::is_degenerate, doc::Triangle_2_is_degenerate)    .def("orientation", &Tri_2::orientation, doc::Triangle_2_orientation)    .def("oriented_side", &Tri_2::oriented_side, doc::Triangle_2_oriented_side)    .def("bounded_side", &Tri_2::bounded_side, doc::Triangle_2_bounded_side)    .def("has_on_positive_side", &Tri_2::has_on_positive_side)
    .def("has_on_negative_side", &Tri_2::has_on_negative_side)
    .def("has_on_boundary", &Tri_2::has_on_boundary)
    .def("has_on_bounded_side", &Tri_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Tri_2::has_on_unbounded_side, doc::Triangle_2_has_on_unbounded_side)    .def("opposite", &Tri_2::opposite, doc::Triangle_2_opposite)    .def("area", &Tri_2::area, doc::Triangle_2_area)    .def("bbox", &Tri_2::bbox, doc::Triangle_2_bbox)    .def("transform", &Tri_2::transform, doc::Triangle_2_transform)    .def("__str__", to_string<Tri_2>)
    .def("__repr__", to_string<Tri_2>)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    //.setattr("__hash__", &hash<Tri_2>)
    ;

  add_extraction(c);
}

#endif
