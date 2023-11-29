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

  c.def(py::init<Pnt_2&, Pnt_2&>())
    .def(py::init<Pnt_2&, Pnt_2&, int>())
    .def(py::init<Pnt_2&, Pnt_2&, Pnt_2&, Pnt_2&>())
    .def(py::init<RT&, RT&, RT&, RT&, RT&>())
    .def(py::init<RT, RT, RT, RT>())
    .def(py::init<CGAL::Bbox_2&>())
    .def("vertex", &Iso_rect_2::vertex)
    .def("__getitem__", &Iso_rect_2::operator[])
    .def("xmin", &Iso_rect_2::xmin)
    .def("ymin", &Iso_rect_2::ymin)
    .def("xmax", &Iso_rect_2::xmax)
    .def("ymax", &Iso_rect_2::ymax)
    .def("min", &Iso_rect_2::min)
    .def("max", &Iso_rect_2::max)
    .def("min_coord", &Iso_rect_2::min_coord)
    .def("max_coord", &Iso_rect_2::max_coord)
    .def("is_degenerate", &Iso_rect_2::is_degenerate)
    .def("bounded_side", &Iso_rect_2::bounded_side)
    .def("has_on_boundary", &Iso_rect_2::has_on_boundary)
    .def("has_on_bounded_side", &Iso_rect_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Iso_rect_2::has_on_unbounded_side)
    .def("__str__", to_string<Iso_rect_2>)
    .def("__repr__", to_string<Iso_rect_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    //.setattr("__hash__", &hash<Iso_rect_2>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
