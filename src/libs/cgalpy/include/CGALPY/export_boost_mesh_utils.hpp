#ifndef CGALPY_EXPORT_BOOST_MESH_UTILS_HPP
#define CGALPY_EXPORT_BOOST_MESH_UTILS_HPP

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>

#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

namespace boost_utils {

template <typename PolygonMesh>
auto num_vertices(const PolygonMesh& p)
{
  return num_vertices(p);
}

template <typename PolygonMesh>
auto num_edges(const PolygonMesh& p)
{
  return num_edges(p);
}

template <typename PolygonMesh>
auto degree_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{
  return degree(v, p);
}

template <typename PolygonMesh>
auto degree_f(typename boost::graph_traits<PolygonMesh>::face_descriptor f, const PolygonMesh& p)
{
  return degree(f, p);
}

template <typename PolygonMesh>
auto out_degree(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{
  return out_degree(v, p);
}

template <typename PolygonMesh>
auto in_degree(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{
  return in_degree(v, p);
}

template <typename PolygonMesh>
auto source_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor e, const PolygonMesh& p)
{
  return source(e, p);
}

template <typename PolygonMesh>
auto source_h(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{
  return source(h, p);
}

template <typename PolygonMesh>
auto target_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor e, const PolygonMesh& p)
{
  return target(e, p);
}

template <typename PolygonMesh>
auto target_h(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{
  return target(h, p);
}

template <typename PolygonMesh>
auto my_vertices(const PolygonMesh& p)
{
  // return a python list
  py::list l;
  for (auto v : vertices(p))
  {
    l.append(v);
  }
  return l;
}

template <typename PolygonMesh>
auto my_edges(const PolygonMesh& p)
{
  // return a python list
  py::list l;
  for (auto e : edges(p))
  {
    l.append(e);
  }
  return l;
}

template <typename PolygonMesh>
auto my_in_edges(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{
  // return a python list
  py::list l;
  for (auto e : in_edges(v, p))
  {
    l.append(e);
  }
  return l;
}

template <typename PolygonMesh>
auto my_out_edges(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{
  // return a python list
  py::list l;
  for (auto e : out_edges(v, p))
  {
    l.append(e);
  }
  return l;
}

template <typename PolygonMesh>
auto my_adjacent_vertices(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{
  // return a python list
  py::list l;
  for (auto v : adjacent_vertices(v, p))
  {
    l.append(v);
  }
  return l;
}

template <typename PolygonMesh>
auto edge(typename boost::graph_traits<PolygonMesh>::vertex_descriptor u,
          typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
          const PolygonMesh& p)
{
  return py::make_tuple(edge(u, v, p).first, edge(u, v, p).second);
}

template <typename PolygonMesh>
auto next(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{
  return next(h, p);
}

template <typename PolygonMesh>
auto prev(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{
  return prev(h, p);
}

template <typename PolygonMesh>
auto opposite(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{
  return opposite(h, p);
}

template <typename PolygonMesh>
auto edge_h(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{
  return edge(h, p);
}

template <typename PolygonMesh>
auto halfedge_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor e, const PolygonMesh& p)
{
  return halfedge(e, p);
}

template <typename PolygonMesh>
auto halfedge_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, const PolygonMesh& p)
{
  return halfedge(v, p);
}

template <typename PolygonMesh>
auto halfedge_vv(typename boost::graph_traits<PolygonMesh>::vertex_descriptor u,
                 typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                 const PolygonMesh& p)
{
  return py::make_tuple(halfedge(u, v, p).first, halfedge(u, v, p).second);
}

template <typename PolygonMesh>
auto my_halfedges(const PolygonMesh& p)
{
  // return a python list
  py::list l;
  for (auto h : halfedges(p))
  {
    l.append(h);
  }
  return l;
}

template <typename PolygonMesh>
auto num_halfedges(const PolygonMesh& p)
{
  return num_halfedges(p);
}

template <typename PolygonMesh>
auto set_next(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h1,
              typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h2,
              PolygonMesh& p)
{
  set_next(h1, h2, p);
}

template <typename PolygonMesh>
auto set_target(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h,
                typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                PolygonMesh& p)
{
  set_target(h, v, p);
}

template <typename PolygonMesh>
auto set_halfedge_vh(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                  typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h,
                  PolygonMesh& p)
{
  set_halfedge(v, h, p);
}

template <typename PolygonMesh>
auto my_collect_garbage(PolygonMesh& p)
{
  collect_garbage(p);
}

template <typename PolygonMesh>
auto add_edge(PolygonMesh& p)
{
  return add_edge(p);
}

template <typename PolygonMesh>
auto halfedge_f(typename boost::graph_traits<PolygonMesh>::face_descriptor f, const PolygonMesh& p)
{
  return halfedge(f, p);
}

template <typename PolygonMesh>
auto face_h(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h, const PolygonMesh& p)
{
  return face(h, p);
}

template <typename PolygonMesh>
auto set_face(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h,
              typename boost::graph_traits<PolygonMesh>::face_descriptor f,
              PolygonMesh& p)
{
  set_face(h, f, p);
}

template <typename PolygonMesh>
auto set_halfedge_fh(typename boost::graph_traits<PolygonMesh>::face_descriptor f,
                  typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h,
                  PolygonMesh& p)
{
  set_halfedge(f, h, p);
}

template <typename PolygonMesh>
auto num_faces(const PolygonMesh& p)
{
  return num_faces(p);
}

template <typename PolygonMesh>
auto my_faces(const PolygonMesh& p)
{
  // return a python list
  py::list l;
  for (auto f : faces(p))
  {
    l.append(f);
  }
  return l;
}

template <typename PolygonMesh>
auto add_vertex(PolygonMesh& p)
{
  return add_vertex(p);
}

template <typename PolygonMesh>
auto add_vertex_p(const typename boost::graph_traits<PolygonMesh>::vertex_property_type& p, PolygonMesh& sm)
{
  return add_vertex(p, sm);
}

template <typename PolygonMesh>
auto reserve(PolygonMesh& p,
             typename boost::graph_traits<PolygonMesh>::vertices_size_type nv,
             typename boost::graph_traits<PolygonMesh>::edges_size_type ne,
             typename boost::graph_traits<PolygonMesh>::faces_size_type nf)
{
  reserve(p, nv, ne, nf);
}

template <typename PolygonMesh>
auto remove_vertex(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v, PolygonMesh& p)
{
  remove_vertex(v, p);
}

template <typename PolygonMesh>
auto remove_edge_vv(typename boost::graph_traits<PolygonMesh>::vertex_descriptor u,
                 typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                 PolygonMesh& p)
{
  remove_edge(u, v, p);
}

template <typename PolygonMesh>
auto remove_edge_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor e, PolygonMesh& p)
{
  remove_edge(e, p);
}

// skip remove_edge(eiter, sm)

template <typename PolygonMesh>
auto remove_face(typename boost::graph_traits<PolygonMesh>::face_descriptor f, PolygonMesh& p)
{
  remove_face(f, p);
}

template <typename PolygonMesh>
auto remove_all_elements(PolygonMesh& p)
{
  remove_all_elements(p);
}

template <typename PolygonMesh>
auto add_face(PolygonMesh& p)
{
  return add_face(p);
}

// skip add_face with iterator

template <typename PolygonMesh>
auto my_is_valid_vertex_descriptor(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                                const PolygonMesh& g,
                                const bool verbose = false)
{
  return is_valid_vertex_descriptor(v, g, verbose);
}

template <typename PolygonMesh>
auto my_is_valid_halfedge_descriptor(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor h,
                                  const PolygonMesh& g,
                                  const bool verbose = false)
{
  return is_valid_halfedge_descriptor(h, g, verbose);
}

template <typename PolygonMesh>
auto my_is_valid_edge_descriptor(typename boost::graph_traits<PolygonMesh>::edge_descriptor e,
                              const PolygonMesh& g,
                              const bool verbose = false)
{
  return is_valid_edge_descriptor(e, g, verbose);
}

template <typename PolygonMesh>
auto my_is_valid_face_descriptor(typename boost::graph_traits<PolygonMesh>::face_descriptor f,
                              const PolygonMesh& g,
                              const bool verbose = false)
{
  return is_valid_face_descriptor(f, g, verbose);
}

// helpers from helpers.h

template <typename PolygonMesh>
auto my_is_border_h(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor hd, const PolygonMesh& g)
{
  return is_border(hd, g);
}

template <typename PolygonMesh>
auto my_is_border_edge(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor hd, const PolygonMesh& g)
{
  return is_border_edge(hd, g);
}

template <typename PolygonMesh>
auto my_is_border_e(typename boost::graph_traits<PolygonMesh>::edge_descriptor ed, const PolygonMesh& g)
{
  return is_border(ed, g);
}

template <typename PolygonMesh>
py::object my_is_border_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor vd, const PolygonMesh& g)
{
  auto h = is_border(vd, g);
  return h.has_value() ? py::cast(h.value()) : py::none();
}

} // namespace boost_utils

#endif // EXPORT_BOOST_MESH_UTILS_HPP
