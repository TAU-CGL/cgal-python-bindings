// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

namespace py = nanobind;

#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_d_types.hpp"
#include "cgalpy/Trid_docstrings.hpp"

namespace trid_doc = cgalpy::trid::docstrings;

namespace cgalpy {
namespace trid {
}
} // namespace cgalpy

//!
void export_trid_vertex(py::class_<cgalpy::trid::Triangulation_d>& tri_c) {
  using Fc = cgalpy::trid::Full_cell;
  using Point = cgalpy::trid::Point;
  using Vertex = cgalpy::trid::Vertex;

  if (add_attr<Vertex>(tri_c, "Vertex")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Vertex>(tri_c, "Vertex", trid_doc::Triangulation_vertex_class)
    .def(py::init<const Point&>(), py::arg("p"),
         trid_doc::Triangulation_vertex_Triangulation_vertex)
    .def("point", &Vertex::point, ri,
         trid_doc::TriangulationVertex_point)
    .def("set_point", &Vertex::set_point, py::arg("p"),
         trid_doc::TriangulationVertex_set_point)
    .def("full_cell", [](const Vertex& v)->Fc& { return *(v.full_cell()); }, ri,
         trid_doc::TriangulationDataStructure_Vertex_full_cell)
    .def("set_full_cell",
         [](Vertex& v, Fc& c)->void
         {v.set_full_cell(cgalpy::trid::Full_cell_handle(&c)); },
         py::arg("c"), trid_doc::TriangulationDataStructure_Vertex_set_full_cell)
    .def("is_valid",
         [](const Vertex& v, bool verbose, int level)->bool
         { return v.is_valid(verbose, level); },
         py::arg("verbose") = false, py::arg("level") = 0,
         trid_doc::TriangulationDataStructure_Vertex_is_valid)

#ifdef CGALPY_TRID_VERTEX_WITH_DATA
    .def("data", py::overload_cast<>(&Vertex::data, py::const_), ri,
         trid_doc::Triangulation_vertex_data)
    .def("set_data",
         [](Vertex& v, py::object data)->void{ v.data() = data; },
         py::arg("data"), "Set the user data stored in the vertex.")
#endif
    ;
}
