// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_DIR_2_HPP
#define CGALPY_EXPORT_DIR_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

// Export a two-dimensional direction of a kernel.
template <typename Kernel, typename C>
void export_dir_2(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Dir = typename Kernel::Direction_2;
  using Line = typename Kernel::Line_2;
  using Pnt = typename Kernel::Point_2;
  using Ray = typename Kernel::Ray_2;
  using Seg = typename Kernel::Segment_2;
  using Vec = typename Kernel::Vector_2;

  c.def(py::init<Vec>())
    .def(py::init<Line>())
    .def(py::init<Ray>())
    .def(py::init<Seg>())
    .def(py::init<RT&, RT&>())
    .def(py::init<double, double>())
    .def("dx", &Dir::dx)
    .def("dy", &Dir::dy)
    .def("vector", &Dir::vector)
    .def("transform", &Dir::transform)
    .def("counterclockwise_in_between", &Dir::counterclockwise_in_between)
    .def("delta", &Direction_2::delta)
    .def("__str__", to_string<Dir>)
    .def("__repr__", to_string<Dir>)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(-py::self)
    //.setattr("__hash__", &hash<Direction_2>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
