// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com
//            Efi Fogel          <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_BOOST_MESH_UTILS_HPP
#define CGALPY_EXPORT_BOOST_MESH_UTILS_HPP

#include <vector>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/pair.h>

#include <CGAL/boost/graph/Euler_operations.h>

#include "cgalpy/Bgl_docstrings.hpp"

namespace py = nanobind;
namespace bgl_doc = cgalpy::bgl::docstrings;

namespace boost_utils {

//!
template <typename PolygonMesh>
auto my_vertices(const PolygonMesh& p) {
  std::vector<typename boost::graph_traits<PolygonMesh>::vertex_descriptor> vs;
  for (auto v : vertices(p)) vs.push_back(v);
  return vs;
}

//!
template <typename PolygonMesh>
auto my_edges(const PolygonMesh& p) {
  std::vector<typename boost::graph_traits<PolygonMesh>::edge_descriptor> es;
  for (auto e : edges(p)) es.push_back(e);
  return es;
}

//!
template <typename PolygonMesh>
auto my_in_edges(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                 const PolygonMesh& p) {
  std::vector<typename boost::graph_traits<PolygonMesh>::edge_descriptor> es;
  for (auto e : in_edges(v, p)) es.push_back(e);
  return es;
}

//!
template <typename PolygonMesh>
auto my_out_edges(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                  const PolygonMesh& p) {
  std::vector<typename boost::graph_traits<PolygonMesh>::edge_descriptor> es;
  for (auto e : out_edges(v, p)) es.push_back(e);
  return es;
}

//!
template <typename PolygonMesh>
auto my_halfedges(const PolygonMesh& p) {
  std::vector<typename boost::graph_traits<PolygonMesh>::halfedge_descriptor> hs;
  for (auto h : halfedges(p)) hs.push_back(h);
  return hs;
}

//!
template <typename PolygonMesh>
auto my_faces(const PolygonMesh& p) {
  std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor> fs;
  for (auto f : faces(p)) fs.push_back(f);
  return fs;
}

//!
template <typename C, typename Graph, typename EdgeIsConstrainedMap>
C define_euler_operations(C& c) {
  using Gt = boost::graph_traits<Graph>;
  using VertexRange = std::vector<typename Gt::vertex_descriptor>;
  c.def("join_vertex", &CGAL::Euler::join_vertex<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_join_vertex);
  c.def("split_vertex", &CGAL::Euler::split_vertex<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        bgl_doc::Euler_split_vertex);
  c.def("split_edge", &CGAL::Euler::split_edge<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_split_edge);
  c.def("join_face", &CGAL::Euler::join_face<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_join_face);
  c.def("split_face", &CGAL::Euler::split_face<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        bgl_doc::Euler_split_face);
  c.def("join_loop", &CGAL::Euler::join_loop<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        bgl_doc::Euler_join_loop);
  c.def("split_loop", &CGAL::Euler::split_loop<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("h3"), py::arg("g"),
        bgl_doc::Euler_split_loop);
  c.def("remove_face", &CGAL::Euler::remove_face<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_remove_face);
  c.def("add_edge", &CGAL::Euler::add_edge<Graph>,
        py::arg("s"), py::arg("t"), py::arg("g"),
        bgl_doc::Euler_add_edge);
  c.def("can_add_face", &CGAL::Euler::can_add_face<VertexRange, Graph>,
        py::arg("vrange"), py::arg("sm"),
#if (CGAL_VERSION_NR > 1060200000)
        py::arg("verbose") = false,
#endif
        bgl_doc::Euler_can_add_face);
  c.def("add_face", &CGAL::Euler::add_face<Graph, VertexRange>,
        py::arg("vr"), py::arg("g"), bgl_doc::Euler_add_face);
  c.def("make_hole", &CGAL::Euler::make_hole<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_make_hole);
  c.def("fill_hole", &CGAL::Euler::fill_hole<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_fill_hole);
  c.def("add_center_vertex", &CGAL::Euler::add_center_vertex<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_add_center_vertex);
  c.def("remove_center_vertex", &CGAL::Euler::remove_center_vertex<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_remove_center_vertex);
  c.def("add_vertex_and_face_to_border", &CGAL::Euler::add_vertex_and_face_to_border<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        bgl_doc::Euler_add_vertex_and_face_to_border);
  c.def("add_face_to_border", &CGAL::Euler::add_face_to_border<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        bgl_doc::Euler_add_face_to_border);
  c.def("does_satisfy_link_condition", &CGAL::Euler::does_satisfy_link_condition<Graph>,
        py::arg("e"), py::arg("g"), bgl_doc::Euler_does_satisfy_link_condition);
  c.def("collapse_edge", &CGAL::Euler::collapse_edge<Graph>,
        py::arg("e"), py::arg("g"), bgl_doc::Euler_collapse_edge);
  c.def("collapse_edge", &CGAL::Euler::collapse_edge<Graph, EdgeIsConstrainedMap>,
        py::arg("e"), py::arg("g"), py::arg("edge_is_constrained_map"),
        bgl_doc::Euler_collapse_edge_1);
  c.def("flip_edge", &CGAL::Euler::flip_edge<Graph>,
        py::arg("h"), py::arg("g"), bgl_doc::Euler_flip_edge);
  return c;
}

} // namespace boost_utils

#endif
