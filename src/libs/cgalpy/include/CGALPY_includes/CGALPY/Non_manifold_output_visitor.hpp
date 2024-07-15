#ifndef CGALPY_NON_MANIFOLD_OUTPUT_VISITOR_HPP
#define CGALPY_NON_MANIFOLD_OUTPUT_VISITOR_HPP

#include <nanobind/nanobind.h>
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace COREFINEMENT = PMP::Corefinement;

namespace pmp {

template <typename PolygonalMesh>
struct Non_manifold_output_visitor: public COREFINEMENT::Non_manifold_output_visitor<PolygonalMesh> {

  // call parent constructor
  Non_manifold_output_visitor(PolygonalMesh& mesh1, PolygonalMesh& mesh2)
    : COREFINEMENT::Non_manifold_output_visitor<PolygonalMesh>(mesh1, mesh2) {}
};

} // namespace pmp

#endif // CGALPY_NON_MANIFOLD_OUTPUT_VISITOR_HPP
