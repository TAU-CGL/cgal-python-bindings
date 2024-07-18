#include <nanobind/nanobind.h>
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

namespace boost_utils {
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertices_size_type
// num_vertices(const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.num_vertices();
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::edges_size_type
// num_edges(const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.num_edges();
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::degree_size_type
// degree(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//        const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.degree(v);
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::degree_size_type
// degree(typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor f,
//        const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.degree(f);
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::degree_size_type
// out_degree(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//            const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.degree(v);
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::degree_size_type
// in_degree(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//           const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.degree(v);
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor
// source(typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor e,
//        const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.source(e.halfedge());
// }
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor
// source(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//        const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.source(h);
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor
// target(typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor e,
//        const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.target(e.halfedge());
// }
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor
// target(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//        const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.target(h);
// }
//
// template <typename P>
// Iterator_range<typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_iterator>
// vertices(const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.vertices();
// }
//
//
// template <typename P>
// Iterator_range<typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_iterator>
// edges(const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.edges();
// }
//
//
// template <typename P>
// Iterator_range<typename boost::graph_traits<CGAL::Surface_mesh<P> >::in_edge_iterator>
// in_edges(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//          const CGAL::Surface_mesh<P>& sm)
// {
//   typedef typename boost::graph_traits<CGAL::Surface_mesh<P> >::in_edge_iterator Iter;
//
//   return make_range(Iter(halfedge(v,sm),sm), Iter(halfedge(v,sm),sm,1));
// }
//
//
// template <typename P>
// Iterator_range<typename boost::graph_traits<CGAL::Surface_mesh<P> >::out_edge_iterator>
// out_edges(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//           const CGAL::Surface_mesh<P>& sm)
// {
//   typedef typename boost::graph_traits<CGAL::Surface_mesh<P> >::out_edge_iterator Iter;
//   return make_range(Iter(halfedge(v,sm),sm), Iter(halfedge(v,sm),sm,1));
// }
//
// template <typename P>
// Iterator_range<typename boost::graph_traits<CGAL::Surface_mesh<P> >::adjacency_iterator>
// adjacent_vertices(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//                   const CGAL::Surface_mesh<P>& sm)
// {
//   return CGAL::vertices_around_target(v,sm);
// }
//
// template<typename P>
// std::pair<typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor,
//           bool>
// edge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor u,
//      typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//      const CGAL::Surface_mesh<P>& sm) {
//   typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor
//     he(sm.halfedge(u, v));
//   return std::make_pair(he, he.is_valid());
// }
//
//
// //
// // HalfedgeGraph
// //
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor
// next(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//      const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.next(h);
// }
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor
// prev(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//      const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.prev(h);
// }
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor
// opposite(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//          const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.opposite(h);
// }
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor
// edge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//      const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.edge(h);
// }
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor
// halfedge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor e,
//          const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.halfedge(e);
// }
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor
// halfedge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//          const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.halfedge(v);
// }
//
//
// template <typename P>
// std::pair<
//   typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor,
//   bool
// >
// halfedge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor u,
//          typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//          const CGAL::Surface_mesh<P>& sm)
// {
//   typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h = sm.halfedge(u, v);
//   return std::make_pair(h, h.is_valid());
// }
//
//
//
// //
// // HalfedgeListGraph
// //
// template <typename P>
// Iterator_range<typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_iterator>
// halfedges(const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.halfedges();
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedges_size_type
// num_halfedges(const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.num_halfedges();
// }
//
//
//
// //
// // MutableHalfedgeGraph
// //
// template<typename P>
// void
// set_next(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h1,
//          typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h2,
//          CGAL::Surface_mesh<P>& sm)
// {
//   sm.set_next(h1, h2);
// }
//
//
//
// template<typename P>
// void
// set_target(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//            typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//            CGAL::Surface_mesh<P>& sm)
// {
//   sm.set_target(h, v);
// }
//
//
// template<typename P>
// void
// set_halfedge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//              typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//              CGAL::Surface_mesh<P>& sm)
// {
//   sm.set_halfedge(v, h);
// }
//
//
// template<typename P>
// void
// collect_garbage(CGAL::Surface_mesh<P>& sm)
// {
//   sm.collect_garbage();
// }
//
// template<typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor
// add_edge(CGAL::Surface_mesh<P>& sm)
// {
//   return sm.edge(sm.add_edge());
// }
//
//
// //
// // FaceGraph
// //
// template<typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor
// halfedge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor f,
//      const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.halfedge(f);
// }
//
// template<typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor
// face(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//      const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.face(h);
// }
//
//
//
// //
// // MutableFaceGraph
// //
// template<typename P>
// void
// set_face(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//          typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor f,
//          CGAL::Surface_mesh<P>& sm)
// {
//   sm.set_face(h, f);
// }
//
//
// template<typename P>
// void
// set_halfedge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor f,
//              typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//              CGAL::Surface_mesh<P>& sm)
// {
//   sm.set_halfedge(f, h);
// }
//
//
// //
// // FaceListGraph
// //
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::faces_size_type
// num_faces(const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.num_faces();
// }
//
// template <typename P>
// Iterator_range<typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_iterator>
// faces(const CGAL::Surface_mesh<P>& sm)
// {
//   return sm.faces();
// }
//
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor
// add_vertex(CGAL::Surface_mesh<P>& sm) {
//   return sm.add_vertex();
// }
//
// template <typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor
// add_vertex(const typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_property_type& p, CGAL::Surface_mesh<P>& sm) {
//   return sm.add_vertex(p);
// }
//
// // MutableGraph
// template<typename P>
// void
// reserve(CGAL::Surface_mesh<P>& sm,
//         typename boost::graph_traits< CGAL::Surface_mesh<P> >::vertices_size_type nv,
//         typename boost::graph_traits< CGAL::Surface_mesh<P> >::edges_size_type ne,
//         typename boost::graph_traits< CGAL::Surface_mesh<P> >::faces_size_type nf)
// {
//   sm.reserve(nv, ne, nf);
// }
//
//
// template <typename P>
// void
// remove_vertex(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//               CGAL::Surface_mesh<P>& sm) {
//
//   sm.remove_vertex(v);
// }
//
// template <typename P>
// void
// remove_edge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor u,
//             typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//             CGAL::Surface_mesh<P>& sm)
// {
//   typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor e = edge(u, v, sm);
//   remove_edge(e,sm);
// }
//
// template <typename P>
// void
// remove_edge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor e,
//             CGAL::Surface_mesh<P>& sm)
// {
//   sm.remove_edge(e);
// }
//
// template <typename P>
// void
// remove_edge(typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_iterator eiter,
//             CGAL::Surface_mesh<P>& sm)
// {
//   remove_edge(*eiter, sm);
// }
//
// template<typename P>
// void
// remove_face(typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor f,
//             CGAL::Surface_mesh<P>& sm)
// {
//   sm.remove_face(f);
// }
//
// template<typename P>
// void
// remove_all_elements(CGAL::Surface_mesh<P>& sm)
// {
//   sm.clear_without_removing_property_maps();
// }
//
// template<typename P>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor
// add_face(CGAL::Surface_mesh<P>& sm)
// {
//   return sm.add_face();
// }
//
// template<typename P, typename InputIterator>
// typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor
// add_face(InputIterator begin, InputIterator end, CGAL::Surface_mesh<P>& sm)
// {
//   std::vector<typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor>
//     v(begin, end);
//   return sm.add_face(v);
// }
//
// template<typename P>
// void normalize_border(const CGAL::Surface_mesh<P>&)
// {}
//
//
// template <typename P>
// bool is_valid_vertex_descriptor(typename boost::graph_traits<CGAL::Surface_mesh<P> >::vertex_descriptor v,
//                                 const CGAL::Surface_mesh<P>& g,
//                                 const bool verbose = false)
// {
//   if(!g.is_valid(v, verbose))
//     return false;
//
//   return BGL::is_valid_vertex_descriptor(v, g, verbose);
// }
//
// template <typename P>
// bool is_valid_halfedge_descriptor(typename boost::graph_traits<CGAL::Surface_mesh<P> >::halfedge_descriptor h,
//                                   const CGAL::Surface_mesh<P>& g,
//                                   const bool verbose = false)
// {
//   if(!g.is_valid(h, verbose))
//     return false;
//
//   return BGL::is_valid_halfedge_descriptor(h, g, verbose);
// }
//
// template <typename P>
// bool is_valid_edge_descriptor(typename boost::graph_traits<CGAL::Surface_mesh<P> >::edge_descriptor e,
//                               const CGAL::Surface_mesh<P>& g,
//                               const bool verbose = false)
// {
//   if(!g.is_valid(e, verbose))
//     return false;
//
//   return BGL::is_valid_edge_descriptor(e, g, verbose);
// }
//
// template <typename P>
// bool is_valid_face_descriptor(typename boost::graph_traits<CGAL::Surface_mesh<P> >::face_descriptor f,
//                               const CGAL::Surface_mesh<P>& g,
//                               const bool verbose = false)
// {
//   if(!g.is_valid(f, verbose))
//     return false;
//
//   return BGL::is_valid_face_descriptor(f, g, verbose);
// }

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

// template <typename PolygonMesh>
// auto normalize_border(const PolygonMesh&) ??
// {
//   normalize_border();
// }

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


} // namespace boost_utils
