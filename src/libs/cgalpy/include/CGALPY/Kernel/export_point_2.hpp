// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_POINT_2_HPP
#define CGALPY_EXPORT_POINT_2_HPP

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
void export_point_2(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Pnt = typename Kernel::Point_2;
  using Vec = typename Kernel::Vector_2;

  c.def(py::init<>())
    .def(py::init<Pnt&>())
    .def(py::init<double, double>())
    .def(py::init<double, FT>())
    .def(py::init<FT, double>())
    .def(py::init<FT&, FT&>())
    .def(py::init<RT&, RT&>())
    .def("x", &Pnt::x)
    .def("y", &Pnt::y)
    .def("hx", &Pnt::hx)
    .def("hy", &Pnt::hy)
    .def("hw", &Pnt::hw)
    .def("bbox", &Pnt::bbox)
    .def("cartesian", &Pnt::cartesian)
    .def("__getitem__", &Pnt::operator[])
    .def("dimension", &Pnt::dimension)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self > py::self)
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self - py::self)
    .def(py::self += Vec())
    .def(py::self -= Vec())
    .def(py::self + Vec())
    .def(py::self - Vec())
    // .setattr("__hash__", &hash_rational_point<is_epec_type(), Pnt>) NB
    // .setattr("__doc__", "Pnt") NB
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
