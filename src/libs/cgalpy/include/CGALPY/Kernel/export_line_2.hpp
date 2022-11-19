// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_LINE_2_HPP
#define CGALPY_EXPORT_LINE_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

// Export a two-dimensional line of a kernel.
template <typename Kernel, typename C>
void export_line_2(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Dir = typename Kernel::Direction_2;
  using Pnt = typename Kernel::Point_2;
  using Ray = typename Kernel::Ray_2;
  using Seg = typename Kernel::Segment_2;
  using Vec = typename Kernel::Vector_2;

  c.def(py::init<RT&, RT&, RT&>())
    .def(py::init<Pnt&, Pnt&>())
    .def(py::init<Pnt&, Dir&>())
    .def(py::init<Pnt&, Vec&>())
    .def(py::init<Seg&>())
    .def(py::init<Ray&>())
    .def("a", &Line_2::a)
    .def("b", &Line_2::b)
    .def("c", &Line_2::c)
    .def("is_degenerate", &Line_2::is_degenerate)
    .def("is_horizontal", &Line_2::is_horizontal)
    .def("is_vertical", &Line_2::is_vertical)
    .def("oriented_side", &Line_2::oriented_side)
    .def("has_on", &Line_2::has_on)
    .def("has_on_boundary", &Line_2::has_on_boundary)
    .def("has_on_negative_side", &Line_2::has_on_negative_side)
    .def("has_on_positive_side", &Line_2::has_on_positive_side)
    .def("projection", &Line_2::projection)
    .def("direction", &Line_2::direction)
    .def("to_vector", &Line_2::to_vector)
    .def("opposite", &Line_2::opposite)
    .def("transform", &Line_2::transform)
    .def("perpendicular", &Line_2::perpendicular)
    .def("x_at_y", &Line_2::x_at_y)
    .def("y_at_x", &Line_2::y_at_x)
    .def("__str__", to_string<Line_2>)
    .def("__repr__", to_string<Line_2>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    //.setattr("__hash__", &hash<Line_2>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
