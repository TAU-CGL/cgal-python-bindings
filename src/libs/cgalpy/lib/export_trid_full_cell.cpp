// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>

namespace py = nanobind;

#include "CGALPY/add_attr.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/triangulation_d_types.hpp"

namespace trid {

//!
bool has_neighbor(const Full_cell& fc, const Full_cell& s)
{ return fc.has_neighbor(Full_cell_const_handle(&s)); }

//!
auto has_neighbor_get_index(const Full_cell& fc, const Full_cell& s) {
  int i;
  auto res = fc.has_neighbor(Full_cell_const_handle(&s), i);
  return py::make_tuple(res, i);
}

//!
bool has_vertex(const Full_cell& fc, const Vertex& v)
{ return fc.has_vertex(Vertex_const_handle(&v)); }

//!
auto has_vertex_get_index(const Full_cell& fc, const Vertex& v) {
  int i;
  auto res = fc.has_vertex(Vertex_const_handle(&v), i);
  return py::make_tuple(res, i);
}

//!
int index1(const Full_cell& fc, const Full_cell& s) { return fc.index(Full_cell_const_handle(&s)); }

//!
int index2(const Full_cell& fc, const Vertex& v) { return fc.index(Vertex_const_handle(&v)); }

//!
Full_cell& neighbor(const Full_cell& fc, const int i) {
  auto res = fc.neighbor(i);
  return *res;
}

//!
Vertex& mirror_vertex(const Full_cell& fc, int i, int cur_dim) {
  auto res =  fc.mirror_vertex(i, cur_dim);
  return *res;
}

//!
void set_neighbor(Full_cell& fc, const int i, Full_cell& s) { fc.set_neighbor(i, Full_cell_handle(&s)); }

//!
void set_vertex(Full_cell& fc, const int i, Vertex& v) { fc.set_vertex(i, Vertex_handle(&v)); }

//!
Vertex& vertex(const Full_cell& fc, int i) {
  auto res = fc.vertex(i);
  return *res;
}

//!
py::object vertices(const Full_cell& fc)
{ return make_iterator(fc.vertices_begin(), fc.vertices_end()); }

}

//!
void export_trid_full_cell(py::class_<trid::Triangulation_d>& tri_c) {
  using Fc = trid::Full_cell;
  using Vertex = trid::Vertex;

  if (add_attr<Fc>(tri_c, "Full_cell")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Fc> fc_c(tri_c, "Full_cell");

  fc_c.def(py::init<int>())
    .def(py::init<const Fc&>())

    .def("has_neighbor", &trid::has_neighbor)
    .def("has_neighbor_get_index", &trid::has_neighbor_get_index)
    .def("has_vertex", &trid::has_vertex)
    .def("has_vertex_get_index", &trid::has_vertex_get_index)
    .def("index", &trid::index1)
    .def("index", &trid::index2)
    .def("maximal_dimension", &Fc::maximal_dimension)
    .def("mirror_index", py::overload_cast<int>(&Fc::mirror_index, py::const_))
    .def("mirror_vertex", &trid::mirror_vertex, ri)
    .def("neighbor", &trid::neighbor, ri)
    .def("set_mirror_index", &Fc::set_mirror_index)
    .def("set_neighbor", &trid::set_neighbor)
    .def("set_vertex", &trid::set_vertex)
    .def("swap_vertices", &Fc::swap_vertices)
    .def("vertex", &trid::vertex, ri)
    .def("is_valid", &Fc::is_valid, py::arg("verbose") = true, py::arg("level") = 0)
    .def("vertices", &trid::vertices, py::keep_alive<0, 1>())

#ifdef CGALPY_TRID_FULL_CELL_WITH_DATA
    .def("data", py::overload_cast<>(&Fc::data, py::const_), ri)
    .def("set_data", [](Fc& c, py::object data)->void{ c.data() = data; })
#endif
    ;

  using Vhi = Fc::Vertex_handle_iterator;

  add_dereference_iterator<Vhi, Vhi, const Vertex&>("Vertex_handle_iterator", fc_c);
}
