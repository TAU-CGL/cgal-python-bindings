#ifndef CGALPY_BOOST_UTILS_EXPORT_MESH_SELECTION_FUNCTIONS_HPP
#define CGALPY_BOOST_UTILS_EXPORT_MESH_SELECTION_FUNCTIONS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>

namespace py = nanobind;

namespace boost_utils {


template <typename C, typename Graph>
C define_boost_selection_functions(py::module_& m) {
  using namespace boost_utils;
  // these contain OutputIterator which is Legacy in cppreference so I left them out



  return m;
}

} // namespace boost_utils

#endif // CGALPY_BOOST_UTILS_EXPORT_MESH_ITERATORS_HPP
