// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_SPHERE_3_HPP
#define CGALPY_EXPORT_SPHERE_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "cgalpy/config.hpp"
#include "cgalpy/Hash_rational_point.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/add_extraction.hpp"
#include "cgalpy/make_iterator.hpp"

namespace py = nanobind;

// Export a two-dimensional point of a kernel.
template <typename Kernel, typename C>
void export_sphere_3(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Pnt = typename Kernel::Point_3;
  using Circle = typename Kernel::Circle_3;
  using Sphere = typename Kernel::Sphere_3;

  c.def(py::init<>())
    .def(py::init<const Pnt&, const FT&, CGAL::Orientation>())
    .def(py::init<const Pnt&, const Pnt&, const Pnt&, const Pnt&>())
    .def(py::init<const Pnt&, const Pnt&, const Pnt&, CGAL::Orientation>())
    .def(py::init<const Pnt&, const Pnt&, CGAL::Orientation>())
    .def(py::init<const Pnt&, CGAL::Orientation>())
    .def(py::init<const Circle&>())
    .def("center", &Sphere::center)
    .def("squared_radius", &Sphere::squared_radius)
    .def("orientation", &Sphere::orientation)
    .def("is_degenerate", &Sphere::is_degenerate)
    .def("oriented_side", &Sphere::oriented_side)
    .def("bounded_side", &Sphere::bounded_side)
    .def("has_on_positive_side", &Sphere::has_on_positive_side)
    .def("has_on_negative_side", &Sphere::has_on_negative_side)
    .def("has_on_boundary", &Sphere::has_on_boundary)
    .def("has_on_bounded_side", &Sphere::has_on_bounded_side)
    .def("has_on_unbounded_side",
         py::overload_cast<const Pnt&>(&Sphere::has_on_unbounded_side, py::const_))
    .def("has_on", py::overload_cast<const Pnt&>(&Sphere::has_on, py::const_))
    .def("has_on", py::overload_cast<const Circle&>(&Sphere::has_on, py::const_))
    .def("opposite", &Sphere::opposite)
    .def("orthogonal_transform", &Sphere::orthogonal_transform)
    .def("bbox", &Sphere::bbox)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    //.setattr("__hash__", &hash<Sphere>)
    ;

  add_extraction(c);
}

#endif
