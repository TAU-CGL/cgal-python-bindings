// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_2_types.hpp"
#include "cgalpy/Triangulation_2_docstrings.hpp"

namespace py = nanobind;
namespace doc = cgalpy::docstrings;

namespace tri2 {

}

//!
void export_tri2_vertex(py::class_<tri2::Triangulation_2, CGAL::Triangulation_cw_ccw_2>& tri_c) {
  using Pnt = tri2::Point;
  using Vertex = tri2::Vertex;

  if (add_attr<Vertex>(tri_c, "Vertex")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Vertex>(tri_c, "Vertex",
                     doc::Triangulation_2::Triangulation_2_Vertex)
    .def(py::init<>())
    .def("point", [](const Vertex& v)->const Pnt& { return v.point(); }, ri,
         doc::Triangulation_2::Triangulation_2_point_1)

#ifdef CGALPY_TRI2_VERTEX_WITH_INFO
    .def("info", [](const Vertex& v)->py::object { return v.info(); })
    .def("set_info", [](Vertex& v, py::object obj) { v.info() = obj; })
#endif
    ;

}
