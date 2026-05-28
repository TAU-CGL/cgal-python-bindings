// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <iostream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/export_circulator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_dereference_forward_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/triangulation_2_types.hpp"
#include "CGALPY/types.hpp"

namespace py = nanobind;

namespace cgalpy {
namespace tri2 {

//!
Face& create_face1(Triangulation_data_structure_2& tds) { return *(tds.create_face()); }

//!
Face& create_face2(Triangulation_data_structure_2& tds) { return *(tds.create_face()); }

//!
Face& create_face3(Triangulation_data_structure_2& tds) { return *(tds.create_face()); }

//!
Face& create_face4(Triangulation_data_structure_2& tds) { return *(tds.create_face()); }

//!
Face& create_face5(Triangulation_data_structure_2& tds) { return *(tds.create_face()); }

//!
Face& create_face6(Triangulation_data_structure_2& tds) { return *(tds.create_face()); }

//!
Vertex& create_vertex(Triangulation_data_structure_2& tds) { return *(tds.create_vertex()); }

//!
size_type degree(const Triangulation_data_structure_2& tds, Vertex& v) { return tds.degree(Vertex_handle(&v)); }

//!
py::list faces(py::handle self) {
  constexpr auto ri(py::rv_policy::reference_internal);
  auto& tds = py::cast<Triangulation_data_structure_2&>(self);
  py::list res;
  for (auto& f : tds.faces()) res.append(py::cast(f, ri, self));
  return res;
}

//!
py::list vertices(py::handle self) {
  constexpr auto ri(py::rv_policy::reference_internal);
  auto& tds = py::cast<Triangulation_data_structure_2&>(self);
  py::list res;
  for (auto& v : tds.vertices()) res.append(py::cast(v, ri, self));
  return res;
}

}
} // namespace cgalpy // End of namespace tri2

//!
void export_triangulation_2(py::module_& m) {
  constexpr auto ri(py::rv_policy::reference_internal);

  using Tricc = CGAL::Triangulation_cw_ccw_2;
  if (! add_attr<Tricc>(m, "Triangulation_cw_ccw_2")) {
    py::class_<Tricc>(m, "Triangulation_cw_ccw_2")
      .def(py::init<Tricc&>())
      .def_prop_ro_static("ccw", [](py::handle /*unused*/, int i) { return Tricc::ccw(i); })
      .def_prop_ro_static("cw", [](py::handle /*unused*/, int i) { return Tricc::cw(i); })
      ;
  }

  using Tds = cgalpy::tri2::Triangulation_data_structure_2;
  if (add_attr<Tds>(m, "Triangulation_data_structure_2")) return;

  py::class_<Tds, Tricc> tds_c(m, "Triangulation_data_structure_2");

  tds_c.def(py::init<>())
    .def(py::init<Tds&>())
    .def("clear", &Tds::clear, "Delete all faces and all finite vertices\n")
    // .def("copy_tds", &copy_tds),
    // .def("copy_tds", &copy_tds),
    .def("create_face", &cgalpy::tri2::create_face1, ri)
    .def("create_face", &cgalpy::tri2::create_face2, ri)
    .def("create_face", &cgalpy::tri2::create_face3, ri)
    .def("create_face", &cgalpy::tri2::create_face4, ri)
    .def("create_face", &cgalpy::tri2::create_face5, ri)
    .def("create_face", &cgalpy::tri2::create_face6, ri)
    .def("create_vertex", &cgalpy::tri2::create_vertex, ri)
    .def("degree", &cgalpy::tri2::degree, py::arg("v"),
         "Obtain the degree of a vertex\n"
         "The infinite vertex is counted"
         "Parameters:\n"
         " v: the vertex\n"
         "Return:\n"
         "  int\n")
    // .def("delete_face", &delete_face),
    // .def("delete_vertex", &delete_vertex),
    // .def("dim_down", &dim_down),
    // .def("dimension", &dimension),
    // .def("file_input", &file_input),
    // .def("file_output", &file_output),
    // .def("flip", &flip),
    // .def("incident_edges", &incident_edges),
    // .def("incident_faces", &incident_faces),
    // .def("incident_vertices", &incident_vertices),
    // .def("insert_dim_up", &insert_dim_up),
    // .def("insert_first", &insert_first),
    // .def("insert_in_edge", &insert_in_edge),
    // .def("insert_in_face", &insert_in_face),
    // .def("insert_in_hole", &insert_in_hole),
    // .def("insert_in_hole", &insert_in_hole),
    // .def("insert_second", &insert_second),
    // .def("is_edge", &is_edge),
    // .def("is_edge", &is_edge),
    // .def("is_edge", &is_edge),
    // .def("is_face", &is_face),
    // .def("is_face", &is_face),
    // .def("is_face", &is_face),
    // .def("is_valid", &is_valid),
    // .def("is_vertex", &is_vertex),
    // .def("make_hole", &make_hole),
    // .def("mirror_edge", &mirror_edge),
    // .def("mirror_index", &mirror_index),
    // .def("mirror_vertex", &mirror_vertex),
    .def("number_of_edges", &Tds::number_of_edges,
         "Obtain the number of edges\n"
         "Return:\n"
         "  int\n")
    .def("number_of_faces", &Tds::number_of_faces,
         "Obtain the number of two dimensional faces\n"
         "Return:\n"
         "  int\n")
    .def("number_of_full_dim_faces", &Tds::number_of_full_dim_faces,
         "Obtain the number of full dimensional faces\n"
         "Return:\n"
         "  int\n")
    .def("number_of_vertices", &Tds::number_of_vertices,
         "Obtain the number of vertices\n"
         "Return:\n"
         "  int\n")
    // .def("remove_degree_3", &remove_degree_3),
    // .def("remove_dim_down", &remove_dim_down),
    // .def("remove_first", &remove_first),
    // .def("remove_second", &remove_second),
    // .def("set_dimension", &set_dimension),
    // .def("star_hole", &star_hole),
    // .def("star_hole", &star_hole),
    // .def("star_hole", &star_hole),
    // .def("star_hole", &star_hole),
    // .def("swap", &swap),

    // Non concept
    .def("faces", &cgalpy::tri2::faces)
    .def("vertices", &cgalpy::tri2::vertices)
    ;

    // operator<<
    // operator=
    // operator>>

    // edges_begin
    // edges_end
    // faces_begin
    // faces_end
    // vertices_begin
    // vertices_end
}
