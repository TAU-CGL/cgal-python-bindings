#ifndef CGALPY_CONNECTED_COMPONENTS_HPP
#define CGALPY_CONNECTED_COMPONENTS_HPP

#include <nanobind/nanobind.h>

#include <boost/graph/graph_traits.hpp>
#include <boost/iterator/function_output_iterator.hpp>

#include <CGAL/Polygon_mesh_processing/connected_components.h>

namespace py = nanobind;

namespace pmp {

template <typename PolygonMesh>
auto connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor seed_face,
                         PolygonMesh& pm,
                         const py::dict& np = py::dict());

}  // namespace pmp

#endif  // CGALPY_CONNECTED_COMPONENTS_HPP
