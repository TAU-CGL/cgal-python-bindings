// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_PLANE_3_HPP
#define CGALPY_EXPORT_PLANE_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "cgalpy/config.hpp"
#include "cgalpy/Hash_rational_point.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/make_iterator.hpp"
#include "cgalpy/add_extraction.hpp"

namespace py = nanobind;

// Export a two-dimensional point of a kernel.
template <typename Kernel, typename C>
void export_plane_3(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Pnt = typename Kernel::Point_3;
  using Vec = typename Kernel::Vector_3;
  using Pln = typename Kernel::Plane_3;

  c.def(py::init<>())
    .def(py::init<const RT&, const RT&, const RT&, const RT&>())
    .def(py::init<const Pnt&, const Pnt&, const Pnt&>())
    .def(py::init<const Pnt&, const Vec&>())
    // .def(py::init<double, FT>())
    // .def(py::init<FT, double>())
    // .def(py::init<FT&, FT&>())
    // .def(py::init<RT&, RT&>())
    .def("a", &Pln::a)
    .def("b", &Pln::b)
    .def("c", &Pln::c)
    .def("d", &Pln::d)
    .def("perpendicular_line", &Pln::perpendicular_line)
    .def("projection", &Pln::projection)
    .def("opposite", &Pln::opposite)
    .def("point", &Pln::point)
    .def("orthogonal_vector", &Pln::orthogonal_vector)
    .def("orthogonal_direction", &Pln::orthogonal_direction)
    .def("base1", &Pln::base1)
    .def("base2", &Pln::base2)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    // .setattr("__doc__", "Plane_3") NB
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
