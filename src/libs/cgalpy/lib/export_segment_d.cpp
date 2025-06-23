// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/add_insertion.hpp"
#include "CGALPY/kernel_d_types.hpp"

//!
void export_segment_d(py::class_<Segment_d>& segd_c) {
  using Segd = Segment_d;

  segd_c.def(py::init<Point_d&, Point_d&>())
    .def("source", &Segd::source)
    .def("target", &Segd::target)
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def("opposite", &Segd::opposite)
    .def("__getitem__", &Segd::operator[])
#endif
#if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) &&     \
     (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
    .def("vertex", &Segd::vertex)
    .def("point", &Segd::point)
    .def("min", &Segd::min)
    .def("max", &Segd::max)
    .def("supporting_line", &Segd::supporting_line)
    .def("squared_length", &Segd::squared_length)
    .def("direction", &Segd::direction)
    .def("has_on", &Segd::has_on)
    .def("is_degenerate", &Segd::is_degenerate)
    .def(py::self == py::self)
    .def(py::self != py::self)
#endif
    // .setattr("__hash__", &hash<Segd>)
    ;

#if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) && \
     (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
  add_insertion(segd_c, "__str__");
  add_insertion(segd_c, "__repr__");
#endif
}
