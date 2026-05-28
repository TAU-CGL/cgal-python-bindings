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
#include "cgalpy/Tri2_docstrings.hpp"

namespace py = nanobind;
namespace tri2_doc = cgalpy::tri2::docstrings;

namespace cgalpy {
namespace tri2 {

}
} // namespace cgalpy

//!
void export_tri2_face(py::class_<cgalpy::tri2::Triangulation_2, CGAL::Triangulation_cw_ccw_2>& tri_c) {
  using Face = cgalpy::tri2::Face;
  using Vertex = cgalpy::tri2::Vertex;

  if (add_attr<Face>(tri_c, "Face")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Face>(tri_c, "Face")
    .def("is_valid", &Face::is_valid,
         py::arg("verbose") = false, py::arg("level") = 0)
    .def("neighbor", [](const Face& f, int i)->const Face& { return *(f.neighbor(i)); }, ri)
    .def("vertex", [](const Face& f, int i)->const Vertex& { return *(f.vertex(i)); }, ri)

#ifdef CGALPY_TRI2_FACE_WITH_INFO
    .def("info", [](const Face& f)->py::object { return f.info(); })
    .def("set_info", [](Face& f, py::object obj) { f.info() = obj; })
#endif

#if ((CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED) ||        \
     (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY))
    .def("is_constrained", [](const Face& f, int i)->bool { return f.is_constrained(i); },
         py::arg("i"),
         tri2_doc::ConstrainedTriangulationFaceBase_2_is_constrained)
#endif
    ;
}
