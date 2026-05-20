// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ISO_RECTANGLE_2_HPP
#define CGALPY_EXPORT_ISO_RECTANGLE_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Iso_rectangle_2.h>

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/to_string.hpp"

namespace py = nanobind;

// Export a 2D iso-rectangle
template <typename Kernel_, typename C>
void export_iso_rectangle_2(C& c) {
  using Ker = Kernel_;
  using Rt = typename Ker::RT;
  using Pnt_2 = typename Ker::Point_2;
  using Iso_rect_2 = typename Ker::Iso_rectangle_2;

  c.def(py::init<const Pnt_2&, const Pnt_2&>(), doc::Iso_rectangle_2_Iso_rectangle_2)
    .def(py::init<const Pnt_2&, const Pnt_2&, int>(), doc::Iso_rectangle_2_Iso_rectangle_2_1)
    .def(py::init<const Pnt_2&, const Pnt_2&, const Pnt_2&, const Pnt_2&>(), doc::Iso_rectangle_2_Iso_rectangle_2_2)
    .def(py::init<const Rt&, const Rt&, const Rt&, const Rt&, const Rt&>(), doc::Iso_rectangle_2_Iso_rectangle_2_3)
    .def(py::init<const CGAL::Bbox_2&>(), doc::Iso_rectangle_2_Iso_rectangle_2_4)
    .def("vertex", &Iso_rect_2::vertex, doc::Iso_rectangle_2_vertex)    .def("__getitem__", &Iso_rect_2::operator[], doc::Iso_rectangle_2_operator)    .def("xmin", &Iso_rect_2::xmin, doc::Iso_rectangle_2_xmin)    .def("ymin", &Iso_rect_2::ymin, doc::Iso_rectangle_2_ymin)    .def("xmax", &Iso_rect_2::xmax, doc::Iso_rectangle_2_xmax)    .def("ymax", &Iso_rect_2::ymax, doc::Iso_rectangle_2_ymax)    .def("min", &Iso_rect_2::min, doc::Iso_rectangle_2_min)    .def("max", &Iso_rect_2::max, doc::Iso_rectangle_2_max)    .def("min_coord", &Iso_rect_2::min_coord, doc::Iso_rectangle_2_min_coord)    .def("max_coord", &Iso_rect_2::max_coord, doc::Iso_rectangle_2_max_coord)    .def("is_degenerate", &Iso_rect_2::is_degenerate, doc::Iso_rectangle_2_is_degenerate)    .def("bounded_side", &Iso_rect_2::bounded_side, doc::Iso_rectangle_2_bounded_side)    .def("has_on_boundary", &Iso_rect_2::has_on_boundary)
    .def("has_on_bounded_side", &Iso_rect_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Iso_rect_2::has_on_unbounded_side)
    .def("__str__", to_string<Iso_rect_2>)
    .def("__repr__", to_string<Iso_rect_2>)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    //.setattr("__hash__", &hash<Iso_rect_2>)
    ;

  add_extraction(c);
}

#endif
