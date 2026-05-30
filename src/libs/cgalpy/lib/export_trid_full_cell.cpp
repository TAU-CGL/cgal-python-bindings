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

#include "cgalpy/add_attr.hpp"
#include "cgalpy/make_iterator.hpp"
#include "cgalpy/triangulation_d_types.hpp"
#include "cgalpy/Trid_docstrings.hpp"

namespace trid_doc = cgalpy::trid::docstrings;

namespace cgalpy {
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
Full_cell& neighbor(const Full_cell& c, const int i) {
  auto res = c.neighbor(i);
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
} // namespace cgalpy

//!
void export_trid_full_cell(py::class_<cgalpy::trid::Triangulation_d>& tri_c) {
  using Fc = cgalpy::trid::Full_cell;
  using Vertex = cgalpy::trid::Vertex;

  if (add_attr<Fc>(tri_c, "Full_cell")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Fc> fc_c(tri_c, "Full_cell",
                          trid_doc::Triangulation_full_cell_class);

  fc_c.def(py::init<int>(), py::arg("dim"),
           trid_doc::Triangulation_full_cell_Triangulation_full_cell)
    .def(py::init<const Fc&>(), py::arg("other"), "Copy constructor.")

    .def("has_neighbor", &cgalpy::trid::has_neighbor, py::arg("s"),
         trid_doc::TriangulationDataStructure_FullCell_has_neighbor)
    .def("has_neighbor_get_index", &cgalpy::trid::has_neighbor_get_index,
         py::arg("s"),
         trid_doc::TriangulationDataStructure_FullCell_has_neighbor_1)
    .def("has_vertex", &cgalpy::trid::has_vertex, py::arg("v"),
         trid_doc::TriangulationDataStructure_FullCell_has_vertex)
    .def("has_vertex_get_index", &cgalpy::trid::has_vertex_get_index,
         py::arg("v"),
         trid_doc::TriangulationDataStructure_FullCell_has_vertex_1)
    .def("index", &cgalpy::trid::index1, py::arg("s"),
         trid_doc::TriangulationDataStructure_FullCell_index)
    .def("index", &cgalpy::trid::index2, py::arg("v"),
         trid_doc::TriangulationDataStructure_FullCell_index_1)
    .def("maximal_dimension", &Fc::maximal_dimension,
         trid_doc::TriangulationDataStructure_FullCell_maximal_dimension)
    .def("mirror_index", py::overload_cast<int>(&Fc::mirror_index, py::const_),
         py::arg("i"), trid_doc::TriangulationDataStructure_FullCell_mirror_index)
    .def("mirror_vertex", &cgalpy::trid::mirror_vertex, ri,
         py::arg("i"), py::arg("cur_dim"),
         trid_doc::TriangulationDataStructure_FullCell_mirror_vertex)
    .def("neighbor", &cgalpy::trid::neighbor, ri, py::arg("i"),
         trid_doc::TriangulationDataStructure_FullCell_neighbor)
    .def("set_mirror_index", &Fc::set_mirror_index, py::arg("i"), py::arg("j"),
         trid_doc::TriangulationDataStructure_FullCell_set_mirror_index)
    .def("set_neighbor", &cgalpy::trid::set_neighbor,
         py::arg("i"), py::arg("s"),
         trid_doc::TriangulationDataStructure_FullCell_set_neighbor)
    .def("set_vertex", &cgalpy::trid::set_vertex,
         py::arg("i"), py::arg("v"),
         trid_doc::TriangulationDataStructure_FullCell_set_vertex)
    .def("swap_vertices", &Fc::swap_vertices, py::arg("i"), py::arg("j"),
         trid_doc::TriangulationDataStructure_FullCell_swap_vertices)
    .def("vertex", &cgalpy::trid::vertex, ri, py::arg("i"),
         trid_doc::TriangulationDataStructure_FullCell_vertex)
    .def("is_valid", &Fc::is_valid,
         py::arg("verbose") = true, py::arg("level") = 0,
         trid_doc::TriangulationDataStructure_FullCell_is_valid)
    .def("vertices", &cgalpy::trid::vertices, py::keep_alive<0, 1>(),
         "Iterate over the vertices of the full cell.")

#ifdef CGALPY_TRID_FULL_CELL_WITH_DATA
    .def("data", py::overload_cast<>(&Fc::data, py::const_), ri,
         trid_doc::Triangulation_full_cell_data)
    .def("set_data", [](Fc& c, py::object data)->void{ c.data() = data; },
         py::arg("data"), "Set the user data stored in the full cell.")
#endif
    ;

  using Vhi = Fc::Vertex_handle_iterator;

  add_dereference_iterator<Vhi, Vhi, const Vertex&>("Vertex_handle_iterator", fc_c);
}
