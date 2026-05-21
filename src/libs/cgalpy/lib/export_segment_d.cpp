// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <cgalpy/Kernel_d_docstrings.hpp>

#include "CGALPY/add_insertion.hpp"
#include "CGALPY/kernel_d_types.hpp"

namespace doc = cgalpy::docstrings::Kernel_d;

//!
void export_segment_d(py::class_<Segment_d>& segd_c) {
  segd_c.doc() = doc::CGAL_Segment_d__class__;
  using Segd = Segment_d;

  segd_c.def(py::init<Point_d&, Point_d&>(), doc::CGAL_Segment_d_Segment_d_1)
    .def("source", &Segd::source, doc::CGAL_Segment_d_source)
    .def("target", &Segd::target, doc::CGAL_Segment_d_target)
#if (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D)
    .def("opposite", &Segd::opposite, doc::CGAL_Segment_d_opposite)
    .def("__getitem__", &Segd::operator[], doc::CGAL_Segment_d_operator)
#endif
#if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) &&     \
     (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
    .def("vertex", &Segd::vertex, doc::CGAL_Segment_d_vertex)
    .def("point", &Segd::point, doc::CGAL_Segment_d_point)
    .def("min", &Segd::min, doc::CGAL_Segment_d_min)
    .def("max", &Segd::max, doc::CGAL_Segment_d_max)
    .def("supporting_line", &Segd::supporting_line, doc::CGAL_Segment_d_supporting_line)
    .def("squared_length", &Segd::squared_length, doc::CGAL_Segment_d_squared_length)
    .def("direction", &Segd::direction, doc::CGAL_Segment_d_direction)
    .def("has_on", &Segd::has_on, doc::CGAL_Segment_d_has_on)
    .def("is_degenerate", &Segd::is_degenerate, doc::CGAL_Segment_d_is_degenerate)
    .def(py::self == py::self, doc::CGAL_Segment_d_operator_1)
    .def(py::self != py::self, doc::CGAL_Segment_d_operator_2)
#endif
    // .setattr("__hash__", &hash<Segd>)
    ;

#if ((CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPIC_D) && \
     (CGALPY_KERNEL_D != CGALPY_KERNEL_D_EPEC_D))
  add_insertion(segd_c, "__str__");
  add_insertion(segd_c, "__repr__");
#endif
}
