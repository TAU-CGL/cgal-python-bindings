// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#ifndef CGALPY_BOOST_UTILS_EXPORT_MESH_HELPERS_HPP
#define CGALPY_BOOST_UTILS_EXPORT_MESH_HELPERS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>

#include <CGAL/boost/graph/copy_face_graph.h>
#include <CGAL/boost/graph/helpers.h>

#include "CGALPY/parse_named_parameters.hpp"

namespace py = nanobind;

namespace boost_utils {

template<typename Graph, typename Graph2>
auto my_copy_face_graph(const Graph& sm, Graph2& tm,
                        const py::dict& np1 = py::dict(),
                        const py::dict& np2 = py::dict()) {
  // TODO: handle nps
  return CGAL::copy_face_graph<Graph, Graph>(sm, tm, internal::parse_named_parameters(np1), internal::parse_named_parameters(np2));
}

template<typename C, typename Graph1, typename Graph2>
C define_boost_helpers(py::module_& m) {
  using Graph = Graph1;
  using namespace boost_utils;

  m.def("clear", &CGAL::clear<Graph>,
        "removes all vertices, faces and halfedges from a graph\n"
        "Calls remove_vertex(), remove_edge(), and remove_face(), for each vertex, edge, and face.\n"
        "If the graph has a member function clear(), it will be called instead.\n"
        "\n"
        "Warning\n"
        "If it exists, the clear() function of a graph might do more than simply remove elements. For example, clear() collects garbage and removes all property maps added by a call to add_property_map() for all simplex types.\n"
        "\n"
        "Parameters\n"
        "g  the graph to clear\n",
        py::arg("g"));

  m.def("copy_face_graph", &my_copy_face_graph<Graph1, Graph2>,
        "copies a source model into a target.\n"
        "\n"
        "Parameters\n"
        "sm  the source mesh\n"
        "tm  the target mesh\n"
        "np1  an optional sequence of Named Parameters among the ones listed below\n"
        "np2  an optional sequence of Named Parameters among the ones listed below\n",
        py::arg("sm"), py::arg("tm"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("halfedge_index_in_face", &CGAL::halfedge_index_in_face<Graph>,
        "returns the number of calls to next(hd, tm) one has to apply to hd for hd == he to be true, starting from hd = halfedge(face(he, tm), tm).\n"
        "\n"
        "Parameters\n"
        "he  a halfedge of g whose index in face(he, tm) is sought\n"
        "g  an object of type Graph\n",
        py::arg("he"), py::arg("g"));

  m.def("is_empty", &CGAL::is_empty<Graph>,
        "checks whether the graph is empty, by checking that it does not contain any vertex.\n"
        "\n"
        "Parameters\n"
        "g  the graph to test\n",
        py::arg("g"));

  m.def("is_isolated_triangle", &CGAL::is_isolated_triangle<Graph>,
        "returns true iff the connected component denoted by hd is a triangle.\n"
        "\n"
        "Precondition\n"
        "g must be valid.\n",
        py::arg("hd"), py::arg("g"));

  m.def("is_valid_face_graph", &CGAL::is_valid_face_graph<Graph>,
        "checks the integrity of the graph g.\n"
        "The graph g is a valid face graph if it is a valid halfedge graph, and if it follows the rules of the FaceListGraph concept and all of its associations are reciprocal (for example, face(halfedge(f,g),g) must be f).\n"
        "\n"
        "Parameters\n"
        "g  the graph to test\n"
        "verb  if true, the details of the check will be written in the standard output\n",
        "Returns\n"
        "true if g is valid, false otherwise.\n",
        py::arg("g"), py::arg("verb") = false);

  m.def("is_valid_halfedge_graph", &CGAL::is_valid_halfedge_graph<Graph>,
        "checks the integrity of the graph g.\n"
        "The graph g is valid if it follows the rules of the HalfedgeListGraph concept and all of its associations are reciprocal (for example, prev(next(h, g), g) must be h, and next(prev(h, g), g) must be h).\n"
        "\n"
        "Parameters\n"
        "g  the graph to test\n"
        "verb  if true, the details of the check will be written in the standard output.\n",
        "Returns\n"
        "true if g is valid, false otherwise.\n",
        py::arg("g"), py::arg("verb") = false);

  m.def("is_valid_polygon_mesh", &CGAL::is_valid_polygon_mesh<Graph>,
        "checks the integrity of the mesh g.\n"
        "The mesh g is a valid polygon mesh if it is a valid face graph and if it follows the rules defined in PolygonMesh.\n"
        "\n"
        "Parameters\n"
        "g  the Mesh to test\n"
        "verb  if true, the details of the check will be written in the standard output\n"
        "\n"
        "Returns\n"
        "true if g is valid, false otherwise.\n",
        py::arg("g"), py::arg("verb") = false);

  // m.def("remove_all_elements", &CGAL::remove_all_elements<Graph>,
  //       "removes all vertices, faces and halfedges from a graph.\n"
  //       "Calls remove_vertex(), remove_edge(), and remove_face(), for each vertex, edge, and face.\n"
  //       "Note that some graphs have a specialized version of this function to improve complexity.\n"
  //       "\n"
  //       "Warning\n"
  //       "This function does not perform anything more than what is advertised above. It is up to the user to e.g. clean garbage or remove internal property maps (if relevant, and desired).\n"
  //       "\n"
  //       "Parameters\n"
  //       "g  the graph whose elements will be removed\n",
  //       py::arg("g"));

  m.def("vertex_index_in_face", &CGAL::vertex_index_in_face<Graph>,
        "returns the number of calls to next() one has to apply to the halfedge hd for source(hd, mesh) == vd to be true, starting from hd = halfedge(fd, tm).\n"
        "\n"
        "Parameters\n"
        "vd  a vertex of g whose index is sought\n"
        "fd  a face of g in which the index of vd is sought\n"
        "g  a mesh of type Graph\n"
        "\n"
        "Precondition\n"
        "vd is a vertex of fd.\n",
        py::arg("vd"), py::arg("fd"), py::arg("g"));

  return m;
}

} // namespace boost_utils

#endif
