#ifndef CGALPY_GET_HPP
#define CGALPY_GET_HPP

#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#include <CGAL/boost/graph/properties.h>

#include <nanobind/nanobind.h>

namespace internal {

template <typename T1, typename T2>
auto get(const T1& p, const T2& t) {
  return get(p, t);
}

}  // namespace internal

#endif  // CGALPY_GET_HPP
