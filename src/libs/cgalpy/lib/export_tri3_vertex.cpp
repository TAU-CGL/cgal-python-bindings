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

namespace py = nanobind;

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

//!
void export_tri3_vertex(py::class_<tri3::Triangulation_3>& tri_c) {
  using Vertex = tri3::Vertex;

  if (add_attr<Vertex>(tri_c, "Vertex")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Vertex>(tri_c, "Vertex")
    .def(py::init<>())
    .def("cell", &tri3::cell, ri)
    .def("point", py::overload_cast<>(&Vertex::point, py::const_), ri)
    .def("set_cell", &tri3::set_cell)
    .def("set_point", &Vertex::set_point)
    .def("is_valid", &tri3::vertex_is_valid1)
    .def("is_valid", &tri3::vertex_is_valid2)
    .def("is_valid", &tri3::vertex_is_valid3)

#ifdef CGALPY_TRI3_VERTEX_WITH_INFO
    .def("set_info", [](Vertex& v, py::object obj) { v.info() = obj; })
    .def("info", [](const Vertex& v)->py::object { return v.info(); })
#endif

    ;
}
