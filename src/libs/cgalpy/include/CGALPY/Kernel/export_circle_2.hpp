// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_CIRCLE_2_HPP
#define CGALPY_EXPORT_CIRCLE_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

// Export a two-dimensional point of a kernel.
template <typename Kernel, typename C>
void export_circle_2(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Pnt = typename Kernel::Point_2;
  using Circle = typename Kernel::Circle_2;

  c.def(py::init<>(), doc::Circle_2_Circle_2)
    .def(py::init<Pnt&, FT&, CGAL::Orientation>(),
         py::arg("center"), py::arg("squared_radius"),
         py::arg("orientation").noconvert())
    .def(py::init<Pnt&, CGAL::Orientation>(),
         py::arg("center"), py::arg("orientation").noconvert())
    .def(py::init<Pnt&, FT&>(), doc::Circle_2_Circle_2_3)
    .def(py::init<Pnt&>(), doc::Circle_2_Circle_2)
    .def(py::init<Pnt&, Pnt&, CGAL::Orientation>(),
         py::arg("p"), py::arg("q"), py::arg("orientation").noconvert())
    .def(py::init<Pnt&, Pnt&, FT&>(), doc::Circle_2_Circle_2_1)
    .def(py::init<Pnt&, Pnt&>(), doc::Circle_2_Circle_2_2)
    .def(py::init<Pnt&, Pnt&, Pnt&>(), doc::Circle_2_Circle_2_3)
    .def("center", &Circle::center, doc::Circle_2_center)    .def("squared_radius", &Circle::squared_radius, doc::Circle_2_squared_radius)    .def("orientation", &Circle::orientation, doc::Circle_2_orientation)    .def("is_degenerate", &Circle::is_degenerate, doc::Circle_2_is_degenerate)    .def("oriented_side", &Circle::oriented_side, doc::Circle_2_oriented_side)    .def("bounded_side", &Circle::bounded_side, doc::Circle_2_bounded_side)    .def("has_on_positive_side", &Circle::has_on_positive_side)
    .def("has_on_negative_side", &Circle::has_on_negative_side)
    .def("has_on_boundary", &Circle::has_on_boundary)
    .def("has_on_bounded_side", &Circle::has_on_bounded_side)
    .def("has_on_unbounded_side",
         py::overload_cast<const Pnt&>(&Circle::has_on_unbounded_side, py::const_))
    .def("orthogonal_transform", &Circle::orthogonal_transform, doc::Circle_2_orthogonal_transform)    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    //.setattr("__hash__", &hash<Circle>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
