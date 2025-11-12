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

namespace py = nanobind;

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
        py::arg("h"), py::arg("g"),
        "joins the two vertices incident to h, (that is source(h, g) and target(h, g)) and removes source(h,g).");
  c.def("split_vertex", &CGAL::Euler::split_vertex<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        "splits the target vertex v of h1 and h2, and connects the new vertex and v with a new edge.");
  c.def("split_edge", &CGAL::Euler::split_edge<Graph>,
        py::arg("h"), py::arg("g"),
        "splits the halfedge h into two halfedges inserting a new vertex that is a copy of vertex(opposite(h,g),g).");
  c.def("join_face", &CGAL::Euler::join_face<Graph>,
        py::arg("h"), py::arg("g"),
        "joins the two faces incident to h and opposite(h,g).");
  c.def("split_face", &CGAL::Euler::split_face<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        "splits the face incident to h1 and h2.");
  c.def("join_loop", &CGAL::Euler::join_loop<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        "glues the cycle of halfedges of h1 and h2 together.");
  c.def("split_loop", &CGAL::Euler::split_loop<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("h3"), py::arg("g"),
        "cuts the graph along the cycle (h1,h2,h3) changing the genus (halfedge h3 runs on the backside of the three dimensional figure below).");
  c.def("remove_face", &CGAL::Euler::remove_face<Graph>,
        py::arg("h"), py::arg("g"),
        "removes the incident face of h and changes all halfedges incident to the face into border halfedges or removes them from the graph if they were already border halfedges.");
  c.def("add_edge", &CGAL::Euler::add_edge<Graph>,
        py::arg("s"), py::arg("t"), py::arg("g"),
        "adds and returns the edge e connecting s and t halfedge(e, g) has s as source and t as target");
  c.def("can_add_face", &CGAL::Euler::can_add_face<VertexRange, Graph>,
        py::arg("vrange"), py::arg("sm"),
#if (CGAL_VERSION_NR > 1060200000)
        py::arg("verbose") = false,
#endif
        "checks whether a new face defined by a range of vertices (identified by their descriptors, boost::graph_traits<Graph>::vertex_descriptor) can be added.");
  c.def("add_face", &CGAL::Euler::add_face<Graph, VertexRange>,
        py::arg("vr"), py::arg("g"),
        "adds a new face defined by a range of vertices (identified by their descriptors, boost::graph_traits<Graph>::vertex_descriptor).");
  c.def("make_hole", &CGAL::Euler::make_hole<Graph>,
        py::arg("h"), py::arg("g"),
        "removes the incident face of h and changes all halfedges incident to the face into border halfedges.");
  c.def("fill_hole", &CGAL::Euler::fill_hole<Graph>,
        py::arg("h"), py::arg("g"),
        "fills the hole incident to h.");
  c.def("add_center_vertex", &CGAL::Euler::add_center_vertex<Graph>,
        py::arg("h"), py::arg("g"),
        "creates a barycentric triangulation of the face incident to h.");
  c.def("remove_center_vertex", &CGAL::Euler::remove_center_vertex<Graph>,
        py::arg("h"), py::arg("g"),
        "removes the vertex target(h, g) and all incident halfedges thereby merging all incident faces.");
  c.def("add_vertex_and_face_to_border", &CGAL::Euler::add_vertex_and_face_to_border<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        "appends a new face to the border halfedge h2 by connecting the tip of h2 with the tip of h1 with two new halfedges and a new vertex and creating a new face that is incident to h2.");
  c.def("add_face_to_border", &CGAL::Euler::add_face_to_border<Graph>,
        py::arg("h1"), py::arg("h2"), py::arg("g"),
        "appends a new face incident to the border halfedge h1 and h2 by connecting the vertex target(h2,g) and the vertex target(h1,g) with a new halfedge, and filling this separated part of the hole with a new face, such that the new face is incident to h2.");
  c.def("does_satisfy_link_condition", &CGAL::Euler::does_satisfy_link_condition<Graph>,
        py::arg("e"), py::arg("g"));
  c.def("collapse_edge", &CGAL::Euler::collapse_edge<Graph>,
        py::arg("e"), py::arg("g"),
        "collapses an edge in a graph.");
  c.def("collapse_edge", &CGAL::Euler::collapse_edge<Graph, EdgeIsConstrainedMap>,
        py::arg("v0v1"), py::arg("g"), py::arg("Edge_is_constrained_map"),
        "collapses an edge in a graph having non-collapsable edges.");
  c.def("flip_edge", &CGAL::Euler::flip_edge<Graph>,
        py::arg("h"), py::arg("g"),
        "performs an edge flip, rotating the edge pointed by h by one vertex in the direction of the face orientation.");
  return c;
}

} // namespace boost_utils

#endif
