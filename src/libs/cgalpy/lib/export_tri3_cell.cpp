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

namespace py = nanobind;

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

//!
void export_tri3_cell(py::class_<tri3::Triangulation_3>& tri_c) {
  using Cell = tri3::Cell;
  using Vh = tri3::Vertex_handle;
  using Ch = tri3::Cell_handle;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Cell>(tri_c, "Cell")) return;

  void(tri3::Cell::*set_vertices)(Vh, Vh, Vh, Vh) =
    &tri3::Cell::set_vertices;
  void(tri3::Cell::*set_neighbors)(Ch, Ch, Ch, Ch) =
    &tri3::Cell::set_neighbors;

  py::class_<Cell>(tri_c, "Cell")
    .def(py::init<>())
    .def("vertex", &tri3::vertex, ri)
    // .def("index", py::overload_cast<Vh>(&Cell::index, py::const_))
    // .def("index", py::overload_cast<Ch>(&Cell::index, py::const_))
    .def("has_vertex", &tri3::has_vertex)
    .def("neighbor", &tri3::neighbor)
    // .def("has_neighbor", py::overload_cast<Ch>(&Cell::has_neighbor, py::const_))
    // .def("has_neighbor", py::overload_cast<Ch, int&>(&Cell::has_neighbor, py::const_))
    .def("set_vertex", &Cell::set_vertex)
    .def("set_vertices", set_vertices)
    .def("set_neighbor", &Cell::set_neighbor)
    .def("set_neighbors", set_neighbors)
    .def("is_valid", &tri3::cell_is_valid1)
    .def("is_valid", &tri3::cell_is_valid2)
    .def("is_valid", &tri3::cell_is_valid3)

#ifdef CGALPY_TRI3_CELL_WITH_INFO
    .def("set_info", [](Cell& c, py::object obj) { c.info() = obj; })
    .def("info", [](const Cell& c)->py::object { return c.info(); })
#endif

    ;
}
