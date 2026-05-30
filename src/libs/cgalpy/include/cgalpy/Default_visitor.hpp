#ifndef CGALPY_DEFAULT_VISITOR_HPP
#define CGALPY_DEFAULT_VISITOR_HPP

#include <nanobind/nanobind.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace COREFINEMENT = PMP::Corefinement;

namespace cgalpy {
namespace pmp {

template <typename PolygonalMesh>
struct Default_visitor : public COREFINEMENT::Default_visitor<PolygonalMesh> {
};

}
} // namespace cgalpy // namespace pmp

#endif // CGALPY_DEFAULT_VISITOR_HPP
