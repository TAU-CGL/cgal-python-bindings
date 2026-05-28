#ifndef CGALPY_UNIFORM_SIZING_FIELD_HPP
#define CGALPY_UNIFORM_SIZING_FIELD_HPP

#include <nanobind/nanobind.h>
#include <CGAL/Polygon_mesh_processing/Uniform_sizing_field.h>
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace cgalpy {
namespace pmp {

template <typename PolygonMesh>
struct Uniform_sizing_field : public CGAL::Polygon_mesh_processing::Uniform_sizing_field<PolygonMesh> {
  using vertex_descriptor = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using halfedge_descriptor = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;
  Uniform_sizing_field(const FT size, const PolygonMesh& mesh) :
    CGAL::Polygon_mesh_processing::Uniform_sizing_field<PolygonMesh>(size, mesh) {};
  // py::object is_too_long(const vertex_descriptor va, const vertex_descriptor vb, const PolygonMesh& mesh) {
  //   auto res = CGAL::Polygon_mesh_processing::Uniform_sizing_field<PolygonMesh>::is_too_long(va, vb, mesh);
  //   if (res) {
  //     return py::cast(res.value());
  //   } else {
  //     return py::none();
  //   }
  // }
  // py::object is_too_short(const halfedge_descriptor h, const PolygonMesh& mesh) {
  //   auto res = CGAL::Polygon_mesh_processing::Uniform_sizing_field<PolygonMesh>::is_too_short(h, mesh);
  //   if (res) {
  //     return py::cast(res.value());
  //   } else {
  //     return py::none();
  //   }
  // }
};

}
} // namespace cgalpy // namespace pmp

#endif // CGALPY_UNIFORM_SIZING_FIELD_HPP
