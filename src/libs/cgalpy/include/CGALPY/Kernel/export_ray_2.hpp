// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_RAY_2_HPP
#define CGALPY_EXPORT_RAY_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

// Export a two-dimensional ray of a kernel.
template <typename Kernel, typename C>
void export_ray_2(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Dir = typename Kernel::Direction_2;
  using Line = typename Kernel::Line_2;
  using Pnt = typename Kernel::Point_2;
  using Ray = typename Kernel::Ray_2;
  using Vec = typename Kernel::Vector_2;

  c.def(py::init<Pnt&, Pnt&>(), doc::Ray_2_Ray_2)
    .def(py::init<Pnt&, Dir&>(), doc::Ray_2_Ray_2_1)
    .def(py::init<Pnt&, Vec&>(), doc::Ray_2_Ray_2_2)
    .def(py::init<Pnt&, Line&>(), doc::Ray_2_Ray_2_3)
    .def("is_degenerate", &Ray::is_degenerate, doc::Ray_2_is_degenerate)    .def("is_horizontal", &Ray::is_horizontal)
    .def("is_vertical", &Ray::is_vertical)
    .def("direction", &Ray::direction, doc::Ray_2_direction)    .def("to_vector", &Ray::to_vector, doc::Ray_2_to_vector)    .def("has_on", &Ray::has_on, doc::Ray_2_has_on)    .def("collinear_has_on", &Ray::collinear_has_on, doc::Ray_2_collinear_has_on)    .def("point", &Ray::point, doc::Ray_2_point)    .def("supporting_line", &Ray::supporting_line, doc::Ray_2_supporting_line)    .def("opposite", &Ray::opposite, doc::Ray_2_opposite)    .def("transform", &Ray::transform, doc::Ray_2_transform)    .def("source", &Ray::source, doc::Ray_2_source)    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    //.setattr("__hash__", &hash<Ray>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
