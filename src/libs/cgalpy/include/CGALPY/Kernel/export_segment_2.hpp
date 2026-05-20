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

  c.def(py::init<Pnt&, Pnt&>(), doc::Segment_2_Segment_2)
    .def("source", &Seg::source, doc::Segment_2_source)    .def("target", &Seg::target, doc::Segment_2_target)    .def("vertex", &Seg::vertex, doc::Segment_2_vertex)    .def("point", &Seg::point, doc::Segment_2_point)    .def("__getitem__", &Seg::operator[], doc::Segment_2_operator)    .def("min", &Seg::min, doc::Segment_2_min)    .def("max", &Seg::max, doc::Segment_2_max)    .def("opposite", &Seg::opposite, doc::Segment_2_opposite)    .def("to_vector", &Seg::to_vector, doc::Segment_2_to_vector)    .def("supporting_line", &Seg::supporting_line, doc::Segment_2_supporting_line)    .def("squared_length", &Seg::squared_length, doc::Segment_2_squared_length)    .def("direction", &Seg::direction, doc::Segment_2_direction)    .def("has_on", &Seg::has_on, doc::Segment_2_has_on)    .def("collinear_has_on", &Seg::collinear_has_on, doc::Segment_2_collinear_has_on)    .def("is_degenerate", &Seg::is_degenerate, doc::Segment_2_is_degenerate)    .def("is_horizontal", &Seg::is_horizontal)
    .def("is_vertical", &Seg::is_vertical)
    .def("bbox", &Seg::bbox, doc::Segment_2_bbox)    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    // .setattr("__hash__", &hash<Seg>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
