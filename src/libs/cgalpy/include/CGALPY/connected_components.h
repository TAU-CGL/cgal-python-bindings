#include <boost/iterator/function_output_iterator.hpp>
#include <boost/graph/graph_traits.hpp>
#include <nanobind/nanobind.h>
#include <CGAL/Polygon_mesh_processing/connected_components.h>

namespace py = nanobind;

namespace pmp {

template <typename PolygonMesh>
py::list connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor
                      seed_face,
                    PolygonMesh& pm,
                    const py::dict& np = py::dict());

} // namespace pmp


