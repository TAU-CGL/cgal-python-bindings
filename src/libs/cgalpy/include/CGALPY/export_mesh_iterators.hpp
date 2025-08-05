#ifndef CGALPY_BOOST_UTILS_EXPORT_MESH_ITERATORS_HPP
#define CGALPY_BOOST_UTILS_EXPORT_MESH_ITERATORS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>

#include <boost/graph/graph_traits.hpp>

#include <CGAL/boost/graph/iterator.h>

namespace py = nanobind;

namespace boost_utils {


template<typename Graph>
auto my_halfedges_around_source_h(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_source_iterator<Graph> >(),
                           "Halfedge_around_source_iterator", halfedges_around_source(h, g).first, halfedges_around_source(h, g).second);
}

template<typename Graph>
auto my_halfedges_around_source_v(typename boost::graph_traits<Graph>::vertex_descriptor v, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_source_iterator<Graph> >(),
                           "Halfedge_around_source_iterator", halfedges_around_source(v, g).first, halfedges_around_source(v, g).second);
}

template<typename Graph>
auto my_halfedges_around_target_h(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_target_iterator<Graph> >(),
                           "Halfedge_around_target_iterator", halfedges_around_target(h, g).first, halfedges_around_target(h, g).second);
}

template<typename Graph>
auto my_halfedges_around_target_v(typename boost::graph_traits<Graph>::vertex_descriptor v, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_target_iterator<Graph> >(),
                           "Halfedge_around_target_iterator", halfedges_around_target(v, g).first, halfedges_around_target(v, g).second);
}

template<typename Graph>
auto my_halfedges_around_face(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_face_iterator<Graph> >(),
                           "Halfedge_around_face_iterator", halfedges_around_face(h, g).first, halfedges_around_face(h, g).second);
}

template<typename Graph>
auto my_faces_around_target(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Face_around_target_iterator<Graph> >(),
                           "Face_around_target_iterator", faces_around_target(h, g).first, faces_around_target(h, g).second);
}

template<typename Graph>
auto my_faces_around_face(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Face_around_face_iterator<Graph> >(),
                           "Face_around_face_iterator", faces_around_face(h, g).first, faces_around_face(h, g).second);
}

template<typename Graph>
auto my_vertices_around_target(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Vertex_around_target_iterator<Graph> >(),
                           "Vertex_around_target_iterator", vertices_around_target(h, g).first, vertices_around_target(h, g).second);
}

template<typename Graph>
auto my_vertices_around_face(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Vertex_around_face_iterator<Graph> >(),
                           "Vertex_around_face_iterator", vertices_around_face(h, g).first, vertices_around_face(h, g).second);
}

template <typename C, typename Graph>
C define_boost_iterators(py::module_& m) {
  using namespace boost_utils;
  m.def("halfedges_around_source", &my_halfedges_around_source_h<Graph>,
        py::arg("h"), py::arg("g"),
        "returns an iterator range over all halfedges with vertex source(h,g) as source.");
  m.def("halfedges_around_source", &my_halfedges_around_source_v<Graph>,
        py::arg("v"), py::arg("g"),
        "returns an iterator range over all halfedges with vertex v as source.");
  m.def("halfedges_around_target", &my_halfedges_around_target_h<Graph>,
        py::arg("h"), py::arg("g"),
        "returns an iterator range over all halfedges with vertex target(h,g) as target.");
  m.def("halfedges_around_target", &my_halfedges_around_target_v<Graph>,
        py::arg("v"), py::arg("g"),
        "returns an iterator range over all halfedges with vertex v as target.");
  m.def("halfedges_around_face", &my_halfedges_around_face<Graph>,
        py::arg("h"), py::arg("g"),
        "returns an iterator range over all halfedges incident to the same face or border as h.");
  m.def("faces_around_target", &my_faces_around_target<Graph>,
        py::arg("h"), py::arg("g"),
        "returns an iterator range over all faces around vertex target(h,g).");
  m.def("faces_around_face", &my_faces_around_face<Graph>,
        py::arg("h"), py::arg("g"),
        "returns an iterator range over all edge-adjacent faces to the same face face(h,g).");
  m.def("vertices_around_target", &my_vertices_around_target<Graph>,
        py::arg("h"), py::arg("g"),
        "returns an iterator range over all vertices adjacent to the vertex target(h,g).");
  m.def("vertices_around_face", &my_vertices_around_face<Graph>,
        py::arg("h"), py::arg("g"),
        "returns an iterator range over all vertices adjacent to the face face(h,g).");
  return m;
}

} // namespace boost_utils

#endif
