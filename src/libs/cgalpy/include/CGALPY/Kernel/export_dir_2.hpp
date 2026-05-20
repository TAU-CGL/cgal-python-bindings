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
#include "CGALPY/to_string.hpp"

namespace py = nanobind;

// Export a two-dimensional direction of a kernel.
template <typename Kernel_, typename C>
void export_dir_2(C& c) {
  using Ker = Kernel_;
  // using Ft = typename Ker::FT;
  using Rt = typename Ker::RT;
  using Dir = typename Ker::Direction_2;
  using Line = typename Ker::Line_2;
  using Pnt = typename Ker::Point_2;
  using Ray = typename Ker::Ray_2;
  using Seg = typename Ker::Segment_2;
  using Vec = typename Ker::Vector_2;

  c.def(py::init<Vec>(), doc::Direction_2_Direction_2)
    .def(py::init<Line>(), doc::Direction_2_Direction_2_1)
    .def(py::init<Ray>(), doc::Direction_2_Direction_2_2)
    .def(py::init<Seg>(), doc::Direction_2_Direction_2_3)
    .def(py::init<Rt&, Rt&>(), doc::Direction_2_Direction_2_4)
    .def("dx", &Dir::dx, doc::Direction_2_dx)
    .def("dy", &Dir::dy, doc::Direction_2_dy)
    .def("vector", &Dir::vector, doc::Direction_2_vector)
    .def("transform", &Dir::transform, doc::Direction_2_transform)
    .def("counterclockwise_in_between", &Dir::counterclockwise_in_between, doc::Direction_2_counterclockwise_in_between)
    .def("delta", &Dir::delta, doc::Direction_2_delta)
    .def("__str__", to_string<Dir>)
    .def("__repr__", to_string<Dir>)
    .def(py::self == py::self,
        py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
        py::sig("def __ne__(self, arg: object, /) -> bool"))
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(-py::self)
    //.setattr("__hash__", &hash<Direction_2>)
    ;

  add_extraction(c);
}

#endif
