#ifndef CGALPY_BOOST_UTILS_EXPORT_MESH_ITERATORS_HPP
#define CGALPY_BOOST_UTILS_EXPORT_MESH_ITERATORS_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace boost_utils {

template<typename Graph>
auto my_halfedges_around_face(typename boost::graph_traits<Graph>::halfedge_descriptor h, const Graph& g) {
  py::list result;
  for (auto h : halfedges_around_face(h, g)) {
    result.append(h);
  }
  return result;
}

//   /**
//  * \ingroup PkgBGLIterators
//  * returns an iterator range over all halfedges with vertex `v` as target.
//  */
// template<typename Graph>
// Iterator_range<Halfedge_around_target_iterator<Graph> >
// halfedges_around_target(typename boost::graph_traits<Graph>::vertex_descriptor v, const Graph& g)
// {
//   return halfedges_around_target(halfedge(v,g),g);
// }

template <typename Graph>
auto my_halfedges_around_target(typename boost::graph_traits<Graph>::vertex_descriptor v, const Graph& g) {
  py::list result;
  for (auto h : halfedges_around_target(v, g)) {
    result.append(h);
  }
  return result;
}



} // namespace boost_utils

#endif // CGALPY_BOOST_UTILS_EXPORT_MESH_ITERATORS_HPP
