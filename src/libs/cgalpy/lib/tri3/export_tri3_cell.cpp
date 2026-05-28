// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_3_types.hpp"

#include "cgalpy/Tri3_docstrings.hpp"

namespace py = nanobind;
namespace tri3_doc = cgalpy::tri3::docstrings;

namespace cgalpy {
namespace tri3 {

//!
bool cell_is_valid1(const Cell& cell, bool verbose, int level)
{ return cell.is_valid(verbose, level); }

//!
bool cell_is_valid2(const Cell& cell, bool verbose)
{ return cell.is_valid(verbose); }

//!
bool cell_is_valid3(const Cell& cell) { return cell.is_valid(); }

//!
Vertex& vertex(const Cell& cell, int index) {
  auto vh = cell.vertex(index);
  return *vh;
}

//!
py::tuple has_vertex(const Cell& cell, Vertex& v) {
  int i;
  auto res = cell.has_vertex(Vertex_handle(&v), i);
  if (res) return py::make_tuple(res, i);
  return py::make_tuple(false);
}

//!
Cell& neighbor(const Cell& cell, int index) {
  auto ch = cell.neighbor(index);
  return *ch;
}

}
} // namespace cgalpy

//!
void export_tri3_cell(py::class_<cgalpy::tri3::Triangulation_3>& tri_c) {
  using Cell = cgalpy::tri3::Cell;
  using Vh = cgalpy::tri3::Vertex_handle;
  using Ch = cgalpy::tri3::Cell_handle;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Cell>(tri_c, "Cell")) return;

  void(cgalpy::tri3::Cell::*set_vertices)(Vh, Vh, Vh, Vh) =
    &cgalpy::tri3::Cell::set_vertices;
  void(cgalpy::tri3::Cell::*set_neighbors)(Ch, Ch, Ch, Ch) =
    &cgalpy::tri3::Cell::set_neighbors;

  py::class_<Cell>(tri_c, "Cell", tri3_doc::TriangulationCellBase_3_class)
    .def(py::init<>())
    .def("vertex", &cgalpy::tri3::vertex, py::arg("index"), ri)
    // .def("index", py::overload_cast<Vh>(&Cell::index, py::const_))
    // .def("index", py::overload_cast<Ch>(&Cell::index, py::const_))
    .def("has_vertex", &cgalpy::tri3::has_vertex, py::arg("vertex"))
    .def("neighbor", &cgalpy::tri3::neighbor, py::arg("index"), ri)
    // .def("has_neighbor", py::overload_cast<Ch>(&Cell::has_neighbor, py::const_))
    // .def("has_neighbor", py::overload_cast<Ch, int&>(&Cell::has_neighbor, py::const_))
    .def("set_vertex", &Cell::set_vertex, py::arg("index"), py::arg("vertex"))
    .def("set_vertices", set_vertices,
         py::arg("v0"), py::arg("v1"), py::arg("v2"), py::arg("v3"))
    .def("set_neighbor", &Cell::set_neighbor, py::arg("index"), py::arg("neighbor"))
    .def("set_neighbors", set_neighbors,
         py::arg("n0"), py::arg("n1"), py::arg("n2"), py::arg("n3"))
    .def("is_valid", &cgalpy::tri3::cell_is_valid1,
         py::arg("verbose"), py::arg("level"))
    .def("is_valid", &cgalpy::tri3::cell_is_valid2, py::arg("verbose"))
    .def("is_valid", &cgalpy::tri3::cell_is_valid3,
         "Return whether this cell is valid.")

#ifdef CGALPY_TRI3_CELL_WITH_INFO
    .def("set_info", [](Cell& c, py::object obj) { c.info() = obj; },
         py::arg("info"))
    .def("info", [](const Cell& c)->py::object { return c.info(); },
         "Return the user information stored in this cell.")
#endif

    ;
}
