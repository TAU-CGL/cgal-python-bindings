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
template <typename Kernel_, typename C>
void export_line_2(C& c) {
  using Ker = Kernel_;
  // using Ft = typename Kernel::FT;
  using Rt = typename Ker::RT;
  using Dir = typename Ker::Direction_2;
  using Pnt = typename Ker::Point_2;
  using Ray = typename Ker::Ray_2;
  using Seg = typename Ker::Segment_2;
  using Vec = typename Ker::Vector_2;
  using Lin = typename Ker::Line_2;

  c.def(py::init<Rt&, Rt&, Rt&>())
    .def(py::init<Pnt&, Pnt&>())
    .def(py::init<Pnt&, Dir&>())
    .def(py::init<Pnt&, Vec&>())
    .def(py::init<Seg&>())
    .def(py::init<Ray&>())
    .def("a", &Lin::a)
    .def("b", &Lin::b)
    .def("c", &Lin::c)
    .def("is_degenerate", &Lin::is_degenerate)
    .def("is_horizontal", &Lin::is_horizontal)
    .def("is_vertical", &Lin::is_vertical)
    .def("oriented_side", &Lin::oriented_side)
    .def("has_on", &Lin::has_on)
    .def("has_on_boundary", &Lin::has_on_boundary)
    .def("has_on_negative_side", &Lin::has_on_negative_side)
    .def("has_on_positive_side", &Lin::has_on_positive_side)
    .def("projection", &Lin::projection)
    .def("direction", &Lin::direction)
    .def("to_vector", &Lin::to_vector)
    .def("opposite", &Lin::opposite)
    .def("transform", &Lin::transform)
    .def("perpendicular", &Lin::perpendicular)
    .def("x_at_y", &Lin::x_at_y)
    .def("y_at_x", &Lin::y_at_x)
    .def("__str__", to_string<Lin>)
    .def("__repr__", to_string<Lin>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    //.setattr("__hash__", &hash<Lin>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
