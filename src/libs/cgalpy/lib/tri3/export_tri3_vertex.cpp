// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_3_types.hpp"

#include "cgalpy/Tri3_docstrings.hpp"

namespace py = nanobind;
namespace tri3_doc = cgalpy::tri3::docstrings;

namespace cgalpy {
namespace tri3 {

//!
Cell& cell(const Vertex& v) { return *(v.cell()); }

//!
void set_cell(Vertex& v, Cell& c) { v.set_cell(Cell_handle(&c)); }

//!
bool vertex_is_valid1(const Vertex& vertex, bool verbose, int level) { return vertex.is_valid(verbose, level); }

//!
bool vertex_is_valid2(const Vertex& vertex, bool verbose) { return vertex.is_valid(verbose); }

//!
bool vertex_is_valid3(const Vertex& vertex) { return vertex.is_valid(); }

}
} // namespace cgalpy

//!
void export_tri3_vertex(py::class_<cgalpy::tri3::Triangulation_3>& tri_c) {
  using Vertex = cgalpy::tri3::Vertex;

  if (add_attr<Vertex>(tri_c, "Vertex")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Vertex>(tri_c, "Vertex", tri3_doc::TriangulationVertexBase_3_class)
    .def(py::init<>())
    .def("cell", &cgalpy::tri3::cell, ri,
         "Return an incident cell of this vertex.")
    .def("point", py::overload_cast<>(&Vertex::point, py::const_), ri,
         tri3_doc::TriangulationVertexBase_3_point)
    .def("set_cell", &cgalpy::tri3::set_cell, py::arg("cell"))
    .def("set_point", &Vertex::set_point, py::arg("point"),
         tri3_doc::TriangulationVertexBase_3_set_point)
    .def("is_valid", &cgalpy::tri3::vertex_is_valid1,
         py::arg("verbose"), py::arg("level"))
    .def("is_valid", &cgalpy::tri3::vertex_is_valid2, py::arg("verbose"))
    .def("is_valid", &cgalpy::tri3::vertex_is_valid3,
         "Return whether this vertex is valid.")

#ifdef CGALPY_TRI3_VERTEX_WITH_INFO
    .def("set_info", [](Vertex& v, py::object obj) { v.info() = obj; },
         py::arg("info"))
    .def("info", [](const Vertex& v)->py::object { return v.info(); },
         "Return the user information stored in this vertex.")
#endif

    ;
}
