// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/triangulation_2_types.hpp"
#include "cgalpy/Tri2_docstrings.hpp"

namespace py = nanobind;
namespace tri2_doc = cgalpy::tri2::docstrings;

namespace cgalpy {
namespace tri2 {

}
} // namespace cgalpy

//!
void export_tri2_vertex(py::class_<cgalpy::tri2::Triangulation_2, CGAL::Triangulation_cw_ccw_2>& tri_c) {
  using Pnt = cgalpy::tri2::Point;
  using Vertex = cgalpy::tri2::Vertex;

  if (add_attr<Vertex>(tri_c, "Vertex")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Vertex>(tri_c, "Vertex",
                     tri2_doc::Triangulation_2_Vertex)
    .def(py::init<>(),
         "Constructs a triangulation vertex.")
    .def("point", [](const Vertex& v)->const Pnt& { return v.point(); }, ri,
         tri2_doc::Triangulation_2_point_1)

#ifdef CGALPY_TRI2_VERTEX_WITH_INFO
    .def("info", [](const Vertex& v)->py::object { return v.info(); },
         tri2_doc::Triangulation_vertex_base_with_info_2_info)
    .def("set_info", [](Vertex& v, py::object obj) { v.info() = obj; },
         py::arg("info"),
         tri2_doc::Triangulation_vertex_base_with_info_2_info_1)
#endif
    ;

}
