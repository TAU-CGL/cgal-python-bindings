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

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

// Export a two-dimensional point of a kernel.
template <typename Kernel, typename C>
void export_plane_3(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Pnt = typename Kernel::Point_3;
  using Vec = typename Kernel::Vector_3;
  using Pln = typename Kernel::Plane_3;

  c.def(py::init<>(), doc::Plane_3_Plane_3)
    .def(py::init<const RT&, const RT&, const RT&, const RT&>(), doc::Plane_3_Plane_3_1)
    .def(py::init<const Pnt&, const Pnt&, const Pnt&>(), doc::Plane_3_Plane_3_2)
    .def(py::init<const Pnt&, const Vec&>(), doc::Plane_3_Plane_3_3)
    // .def(py::init<double, FT>(), doc::Plane_3_Plane_3_4)
    // .def(py::init<FT, double>(), doc::Plane_3_Plane_3_5)
    // .def(py::init<FT&, FT&>(), doc::Plane_3_Plane_3_6)
    // .def(py::init<RT&, RT&>(), doc::Plane_3_Plane_3_7)
    .def("a", &Pln::a, doc::Plane_3_a)    .def("b", &Pln::b, doc::Plane_3_b)    .def("c", &Pln::c, doc::Plane_3_c)    .def("d", &Pln::d, doc::Plane_3_d)    .def("perpendicular_line", &Pln::perpendicular_line, doc::Plane_3_perpendicular_line)    .def("projection", &Pln::projection, doc::Plane_3_projection)    .def("opposite", &Pln::opposite, doc::Plane_3_opposite)    .def("point", &Pln::point, doc::Plane_3_point)    .def("orthogonal_vector", &Pln::orthogonal_vector, doc::Plane_3_orthogonal_vector)    .def("orthogonal_direction", &Pln::orthogonal_direction, doc::Plane_3_orthogonal_direction)    .def("base1", &Pln::base1, doc::Plane_3_base1)    .def("base2", &Pln::base2, doc::Plane_3_base2)    .def(py::self == py::self,
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
