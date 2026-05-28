// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_BGL_ITERATORS_HPP
#define CGALPY_EXPORT_BGL_ITERATORS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>

#include <boost/graph/graph_traits.hpp>

#include <CGAL/boost/graph/iterator.h>

#include "CGALPY/export_circulator.hpp"

namespace py = nanobind;

namespace cgalpy {
namespace bgl {

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto halfedges_around_target_v_circulator(typename boost::graph_traits<Graph>::vertex_descriptor v, const Graph& g) {
  using Hatc = CGAL::Halfedge_around_target_circulator<Graph>;
  return Hatc(v, g);
}

//! Wrap the iterator
template<typename Graph>
auto halfedges_around_target_v(typename boost::graph_traits<Graph>::vertex_descriptor v, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_target_iterator<Graph>>(),
                           "Halfedge_around_target_iterator",
                           CGAL::halfedges_around_target(v, g).first, CGAL::halfedges_around_target(v, g).second);
}

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto halfedges_around_target_h_circulator(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  using Hatc = CGAL::Halfedge_around_target_circulator<Graph>;
  return Hatc(h, g);
}

//! Wrap the iterator
template<typename Graph>
auto halfedges_around_target_h(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_target_iterator<Graph>>(),
                           "Halfedge_around_target_iterator",
                           CGAL::halfedges_around_target(h, g).first, CGAL::halfedges_around_target(h, g).second);
}

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto halfedges_around_source_v_circulator(typename boost::graph_traits<Graph>::vertex_descriptor v, const Graph& g) {
  using Hasc = CGAL::Halfedge_around_source_circulator<Graph>;
  return Hasc(v, g);
}

//! Wrap the iterator
template<typename Graph>
auto halfedges_around_source_v(typename boost::graph_traits<Graph>::vertex_descriptor v, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_source_iterator<Graph>>(),
                           "Halfedge_around_source_iterator",
                           CGAL::halfedges_around_source(v, g).first, CGAL::halfedges_around_source(v, g).second);
}

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto halfedges_around_source_h_circulator(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  using Hasc = CGAL::Halfedge_around_source_circulator<Graph>;
  return Hasc(h, g);
}

//! Wrap the iterator
template<typename Graph>
auto halfedges_around_source_h(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_source_iterator<Graph>>(),
                           "Halfedge_around_source_iterator",
                           CGAL::halfedges_around_source(h, g).first, CGAL::halfedges_around_source(h, g).second);
}

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto halfedges_around_face_circulator(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  using Hafc = CGAL::Halfedge_around_face_circulator<Graph>;
  return Hafc(h, g);
}

//! Wrap the iterator
template<typename Graph>
auto halfedges_around_face(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Halfedge_around_face_iterator<Graph>>(),
                           "Halfedge_around_face_iterator",
                           CGAL::halfedges_around_face(h, g).first, CGAL::halfedges_around_face(h, g).second);
}

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto faces_around_target_circulator(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  using Fatc = CGAL::Face_around_target_circulator<Graph>;
  return Fatc(h, g);
}

//! Wrap the iterator
template<typename Graph>
auto faces_around_target(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Face_around_target_iterator<Graph>>(),
                           "Face_around_target_iterator",
                           CGAL::faces_around_target(h, g).first, CGAL::faces_around_target(h, g).second);
}

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto faces_around_face_circulator(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  using Fafc = CGAL::Face_around_face_circulator<Graph>;
  return Fafc(h, g);
}

//! Wrap the iterator
template<typename Graph>
auto faces_around_face(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Face_around_face_iterator<Graph>>(),
                           "Face_around_face_iterator",
                           CGAL::faces_around_face(h, g).first, CGAL::faces_around_face(h, g).second);
}

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto vertices_around_target_circulator(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  using Vatc = CGAL::Vertex_around_target_circulator<Graph>;
  return Vatc(h, g);
}

//! Wrap the iterator
template<typename Graph>
auto vertices_around_target(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Vertex_around_target_iterator<Graph>>(),
                           "Vertex_around_target_iterator",
                           CGAL::vertices_around_target(h, g).first, CGAL::vertices_around_target(h, g).second);
}

//! Wrap the function that obtains the real circulator
template<typename Graph>
auto vertices_around_face_circulator(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  using Vafc = CGAL::Vertex_around_face_circulator<Graph>;
  return Vafc(h, g);
}

//! Wrap the iterator
template<typename Graph>
auto vertices_around_face(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  return py::make_iterator(py::type<CGAL::Vertex_around_face_iterator<Graph>>(),
                           "Vertex_around_face_iterator",
                           CGAL::vertices_around_face(h, g).first, CGAL::vertices_around_face(h, g).second);
}

}
} // namespace cgalpy

//!
template <typename Graph>
void export_bgl_iterators(py::module_& m) {
  // Iterators
  m.def("halfedges_around_target", &cgalpy::bgl::halfedges_around_target_h<Graph>, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex target(h,g) as target.");
  m.def("halfedges_around_target", &cgalpy::bgl::halfedges_around_target_v<Graph>, py::arg("v"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex v as target.");
  m.def("halfedges_around_source", &cgalpy::bgl::halfedges_around_source_v<Graph>, py::arg("v"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex v as source.");
  m.def("halfedges_around_source", &cgalpy::bgl::halfedges_around_source_h<Graph>, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex source(h,g) as source.");
  m.def("halfedges_around_face", &cgalpy::bgl::halfedges_around_face<Graph>, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all halfedges incident to the same face or border as h.");
  m.def("faces_around_target", &cgalpy::bgl::faces_around_target<Graph>, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all faces around vertex target(h,g).");
  m.def("faces_around_face", &cgalpy::bgl::faces_around_face<Graph>, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all edge-adjacent faces to the same face face(h,g).");
  m.def("vertices_around_target", &cgalpy::bgl::vertices_around_target<Graph>, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all vertices adjacent to the vertex target(h,g).");
  m.def("vertices_around_face", &cgalpy::bgl::vertices_around_face<Graph>, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all vertices adjacent to the face face(h,g).");

  // Circulators
  using Vd = typename boost::graph_traits<Graph>::vertex_descriptor;
  using Hd = typename boost::graph_traits<Graph>::halfedge_descriptor;
  using Fd = typename boost::graph_traits<Graph>::face_descriptor;

  using Hatc = CGAL::Halfedge_around_target_circulator<Graph>;
  export_circulator<Hatc, Hd>(m, "Halfedge_around_target_circulator");
  m.def("halfedges_around_target_circulator", &cgalpy::bgl::halfedges_around_target_h_circulator<Graph>,
        py::arg("h"), py::arg("g"),
        "Obtain a circulator over all halfedges with vertex target(h,g) as target.");
  m.def("halfedges_around_target_circulator", &cgalpy::bgl::halfedges_around_target_v_circulator<Graph>,
        py::arg("v"), py::arg("g"),
        "Obtain a circulator over all halfedges with vertex v as target.");

  using Hasc = CGAL::Halfedge_around_source_circulator<Graph>;
  export_circulator<Hasc, Hd>(m, "Halfedge_around_source_circulator");
  m.def("halfedges_around_source_circulator", &cgalpy::bgl::halfedges_around_source_v_circulator<Graph>,
        py::arg("v"), py::arg("g"),
        "Obtain a circulator over all halfedges with vertex v as source.");
  m.def("halfedges_around_source_circulator", &cgalpy::bgl::halfedges_around_source_h_circulator<Graph>,
        py::arg("h"), py::arg("g"),
        "Obtain a circulator over all halfedges with vertex source(h,g) as source.");

  using Hafc = CGAL::Halfedge_around_face_circulator<Graph>;
  export_circulator<Hafc, Hd>(m, "Halfedge_around_face_circulator");
  m.def("halfedges_around_face_circulator", &cgalpy::bgl::halfedges_around_face_circulator<Graph>, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all halfedges incident to the same face or border as h.");

  using Fatc = CGAL::Face_around_target_circulator<Graph>;
  export_circulator<Fatc, Fd>(m, "Face_around_target_circulator");
  m.def("faces_around_target_circulator", &cgalpy::bgl::faces_around_target_circulator<Graph>, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all faces around vertex target(h,g).");

#if CGAL_VERSION_NR > 1060100900
  using Fafc = CGAL::Face_around_face_circulator<Graph>;
  export_circulator<Fafc, Fd>(m, "Face_around_face_circulator");
  m.def("faces_around_face_circulator", &cgalpy::bgl::faces_around_face_circulator<Graph>, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all edge-adjacent faces to the same face face(h,g).");
#endif

  using Vatc = CGAL::Vertex_around_target_circulator<Graph>;
  export_circulator<Vatc, Vd>(m, "Vertex_around_target_circulator");
  m.def("vertices_around_target_circulator", &cgalpy::bgl::vertices_around_target_circulator<Graph>, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all vertices adjacent to the vertex target(h,g).");

  using Vafc = CGAL::Vertex_around_face_circulator<Graph>;
  export_circulator<Vafc, Vd>(m, "Vertex_around_face_circulator");
  m.def("vertices_around_face_circulator", &cgalpy::bgl::vertices_around_face_circulator<Graph>, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all vertices adjacent to the face face(h,g).");
}

#endif
