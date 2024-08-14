// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_DIR_3_HPP
#define CGALPY_EXPORT_DIR_3_HPP

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
void export_dir_3(C& c) {
  using Ker = Kernel_;
  // using Ft = typename Ker::FT;
  using Rt = typename Ker::RT;
  using Dir = typename Ker::Direction_3;
  using Line = typename Ker::Line_3;
  using Pnt = typename Ker::Point_3;
  using Ray = typename Ker::Ray_3;
  using Seg = typename Ker::Segment_3;
  using Vec = typename Ker::Vector_3;

  c.def(py::init<Vec>())
    .def(py::init<Line>())
    .def(py::init<Ray>())
    .def(py::init<Seg>())
    .def(py::init<Rt&, Rt&, Rt&>())
    .def("dx", &Dir::dx)
    .def("dy", &Dir::dy)
    .def("dz", &Dir::dz)
    .def("vector", &Dir::vector)
    .def("transform", &Dir::transform)
    .def("delta", &Dir::delta)
    .def("__str__", to_string<Dir>)
    .def("__repr__", to_string<Dir>)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    .def(-py::self)
    //.setattr("__hash__", &hash<Dir>)
    ;

  add_extraction(c);
}

#endif
