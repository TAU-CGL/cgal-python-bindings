// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_SEGMENT_2_HPP
#define CGALPY_EXPORT_SEGMENT_2_HPP

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
void export_segment_2(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Pnt = typename Kernel::Point_2;
  using Seg = typename Kernel::Segment_2;

  c.def(py::init<Pnt&, Pnt&>())
    .def("source", &Seg::source)
    .def("target", &Seg::target)
    .def("vertex", &Seg::vertex)
    .def("point", &Seg::point)
    .def("__getitem__", &Seg::operator[])
    .def("min", &Seg::min)
    .def("max", &Seg::max)
    .def("opposite", &Seg::opposite)
    .def("to_vector", &Seg::to_vector)
    .def("supporting_line", &Seg::supporting_line)
    .def("squared_length", &Seg::squared_length)
    .def("direction", &Seg::direction)
    .def("has_on", &Seg::has_on)
    .def("collinear_has_on", &Seg::collinear_has_on)
    .def("is_degenerate", &Seg::is_degenerate)
    .def("is_horizontal", &Seg::is_horizontal)
    .def("is_vertical", &Seg::is_vertical)
    .def("bbox", &Seg::bbox)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    // .setattr("__hash__", &hash<Seg>)
    ;

  add_extraction(c);
}

#endif
