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

} // namespace boost_utils

#endif // CGALPY_BOOST_UTILS_EXPORT_MESH_ITERATORS_HPP
