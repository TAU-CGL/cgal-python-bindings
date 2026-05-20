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

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

// Export a two-dimensional point of a kernel.
template <typename Kernel, typename C>
void export_sphere_3(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Pnt = typename Kernel::Point_3;
  using Circle = typename Kernel::Circle_3;
  using Sphere = typename Kernel::Sphere_3;

  c.def(py::init<>(), doc::Sphere_3_Sphere_3)
    .def(py::init<const Pnt&, const FT&, CGAL::Orientation>(), doc::Sphere_3_Sphere_3_1)
    .def(py::init<const Pnt&, const Pnt&, const Pnt&, const Pnt&>(), doc::Sphere_3_Sphere_3_2)
    .def(py::init<const Pnt&, const Pnt&, const Pnt&, CGAL::Orientation>(), doc::Sphere_3_Sphere_3_3)
    .def(py::init<const Pnt&, const Pnt&, CGAL::Orientation>(), doc::Sphere_3_Sphere_3_4)
    .def(py::init<const Pnt&, CGAL::Orientation>(), doc::Sphere_3_Sphere_3_5)
    .def(py::init<const Circle&>())
    .def("center", &Sphere::center, doc::Sphere_3_center)    .def("squared_radius", &Sphere::squared_radius, doc::Sphere_3_squared_radius)    .def("orientation", &Sphere::orientation, doc::Sphere_3_orientation)    .def("is_degenerate", &Sphere::is_degenerate, doc::Sphere_3_is_degenerate)    .def("oriented_side", &Sphere::oriented_side, doc::Sphere_3_oriented_side)    .def("bounded_side", &Sphere::bounded_side, doc::Sphere_3_bounded_side)    .def("has_on_positive_side", &Sphere::has_on_positive_side)
    .def("has_on_negative_side", &Sphere::has_on_negative_side)
    .def("has_on_boundary", &Sphere::has_on_boundary)
    .def("has_on_bounded_side", &Sphere::has_on_bounded_side)
    .def("has_on_unbounded_side",
         py::overload_cast<const Pnt&>(&Sphere::has_on_unbounded_side, py::const_))
    .def("has_on", py::overload_cast<const Pnt&>(&Sphere::has_on, py::const_))
    .def("has_on", py::overload_cast<const Circle&>(&Sphere::has_on, py::const_))
    .def("opposite", &Sphere::opposite, doc::Sphere_3_opposite)    .def("orthogonal_transform", &Sphere::orthogonal_transform, doc::Sphere_3_orthogonal_transform)    .def("bbox", &Sphere::bbox, doc::Sphere_3_bbox)    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    //.setattr("__hash__", &hash<Sphere>)
    ;

  add_extraction(c);
}

#endif
