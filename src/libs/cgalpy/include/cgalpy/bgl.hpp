// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com
//            Efi Fogel          <efifogel@gmail.com>

#ifndef CGALPY_BGL_HPP
#define CGALPY_BGL_HPP

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>

#include <boost/graph/graph_traits.hpp>

namespace cgalpy {
namespace bgl {

//!
template <typename PolygonMesh>
auto adjacent_vertices(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p) {
  std::vector<typename boost::graph_traits<PolygonMesh>::vertex_descriptor> vs;
  for (auto u : adjacent_vertices(v, p)) vs.push_back(u);
  return vs;
}

//!
template <typename PolygonMesh>
auto add_vertex_p(const typename boost::graph_traits<PolygonMesh>::vertex_property_type& p, PolygonMesh& sm)
{ return add_vertex(p, sm); }

//!
template <typename PolygonMesh>
auto degree_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{ return degree(v, p); }

//!
template <typename PolygonMesh>
auto degree_f(typename boost::graph_traits<PolygonMesh>::face_descriptor f, const PolygonMesh& p)
{ return degree(f, p); }

//!
template <typename PolygonMesh>
auto edge(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{ return edge(h, p); }

//!
template <typename PolygonMesh>
auto edge_vv(typename boost::graph_traits<PolygonMesh>::vertex_descriptor u,
             typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{ return std::make_tuple(edge(u, v, p).first, edge(u, v, p).second); }

//!
template <typename PolygonMesh>
auto face(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{ return face(h, p); }

//!
template <typename PolygonMesh>
auto halfedge_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor e, const PolygonMesh& p)
{ return halfedge(e, p); }

//!
template <typename PolygonMesh>
auto halfedge_f(typename boost::graph_traits<PolygonMesh>::face_descriptor f, const PolygonMesh& p)
{ return halfedge(f, p); }

//!
template <typename PolygonMesh>
auto halfedge_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{ return halfedge(v, p); }

//!
template <typename PolygonMesh>
auto halfedge_vv(typename boost::graph_traits<PolygonMesh>::vertex_descriptor u,
                 typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{ return std::make_tuple(halfedge(u, v, p).first, halfedge(u, v, p).second); }

//!
template <typename PolygonMesh>
auto in_degree(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{ return in_degree(v, p); }

//!
template <typename PolygonMesh>
auto my_is_border_h(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor hd, const PolygonMesh& g)
{ return is_border(hd, g); }

//!
template <typename PolygonMesh>
auto my_is_border_edge(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor hd, const PolygonMesh& g)
{ return is_border_edge(hd, g); }

//!
template <typename PolygonMesh>
auto my_is_border_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor ed, const PolygonMesh& g)
{ return is_border(ed, g); }

//!
template <typename PolygonMesh>
auto my_is_border_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor vd, const PolygonMesh& g)
{ return is_border(vd, g); }

//!
template <typename PolygonMesh>
auto my_is_valid_vertex_descriptor(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                                   const PolygonMesh& g, const bool verbose = false)
{ return is_valid_vertex_descriptor(v, g, verbose); }

//!
template <typename PolygonMesh>
auto my_is_valid_halfedge_descriptor(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h,
                                     const PolygonMesh& g, const bool verbose = false)
{ return is_valid_halfedge_descriptor(h, g, verbose); }

//!
template <typename PolygonMesh>
auto my_is_valid_edge_descriptor(typename boost::graph_traits<PolygonMesh>::edge_descriptor e,
                                 const PolygonMesh& g, const bool verbose = false)
{ return is_valid_edge_descriptor(e, g, verbose); }

//!
template <typename PolygonMesh>
auto my_is_valid_face_descriptor(typename boost::graph_traits<PolygonMesh>::face_descriptor f,
                                 const PolygonMesh& g, const bool verbose = false)
{ return is_valid_face_descriptor(f, g, verbose); }

//!
template <typename PolygonMesh>
auto next(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{ return next(h, p); }

//!
template <typename PolygonMesh>
auto null_face() { return boost::graph_traits<PolygonMesh>::null_face(); }

//!
template <typename PolygonMesh>
auto null_halfedge() { return boost::graph_traits<PolygonMesh>::null_halfedge(); }

//!
template <typename PolygonMesh>
auto null_vertex() { return boost::graph_traits<PolygonMesh>::null_vertex(); }

//!
template <typename PolygonMesh>
auto opposite(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{ return opposite(h, p); }

//!
template <typename PolygonMesh>
auto out_degree(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{ return out_degree(v, p); }

//!
template <typename PolygonMesh>
auto prev(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{ return prev(h, p); }

//!
template <typename PolygonMesh>
auto remove_edge_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor e, PolygonMesh& p)
{ remove_edge(e, p); }

//!
template <typename PolygonMesh>
auto
remove_edge_vv(typename boost::graph_traits<PolygonMesh>::vertex_descriptor u,
               typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, PolygonMesh& p)
{ remove_edge(u, v, p); }

//!
template <typename PolygonMesh>
auto remove_face(typename boost::graph_traits<PolygonMesh>::face_descriptor f, PolygonMesh& p)
{ remove_face(f, p); }

//!
template <typename PolygonMesh>
auto
remove_vertex(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, PolygonMesh& p)
{ remove_vertex(v, p); }

//!
template <typename PolygonMesh>
auto source_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor e, const PolygonMesh& p)
{ return source(e, p); }

//!
template <typename PolygonMesh>
auto source_h(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{ return source(h, p); }

//!
template <typename PolygonMesh>
auto target_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor e, const PolygonMesh& p)
{ return target(e, p); }

//!
template <typename PolygonMesh>
auto target_h(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{ return target(h, p); }

//!
template <typename PolygonMesh>
auto set_face(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h,
              typename boost::graph_traits<PolygonMesh>::face_descriptor f, PolygonMesh& p)
{ set_face(h, f, p); }

//!
template <typename PolygonMesh>
auto
set_halfedge_fh(typename boost::graph_traits<PolygonMesh>::face_descriptor f,
                typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, PolygonMesh& p)
{ set_halfedge(f, h, p); }

//!
template <typename PolygonMesh>
auto set_halfedge_vh(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                     typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, PolygonMesh& p)
{ set_halfedge(v, h, p); }

//!
template <typename PolygonMesh>
auto set_next(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h1,
              typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h2, PolygonMesh& p)
{ set_next(h1, h2, p); }

//!
template <typename PolygonMesh>
auto set_target(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h,
                typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, PolygonMesh& p)
{ set_target(h, v, p); }

}
} // namespace cgalpy

#endif
