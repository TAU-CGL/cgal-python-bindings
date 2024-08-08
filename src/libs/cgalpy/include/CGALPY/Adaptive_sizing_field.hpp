#ifndef CGALPY_ADAPTIVE_SIZING_FIELD_HPP
#define CGALPY_ADAPTIVE_SIZING_FIELD_HPP

#include <nanobind/nanobind.h>
#include <CGAL/Polygon_mesh_processing/Adaptive_sizing_field.h>
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/helpers.hpp"
#include "CGALPY/pmp_np_parser.hpp"

namespace py = nanobind;

namespace pmp {

template <typename PolygonMesh>
struct Adaptive_sizing_field : public CGAL::Polygon_mesh_processing::Adaptive_sizing_field<PolygonMesh> {
  using vertex_descriptor = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using halfedge_descriptor = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;
  using face_descriptor = typename boost::graph_traits<PolygonMesh>::face_descriptor;
  Adaptive_sizing_field(const FT tol, const py::tuple& edge_len_min_max, const py::list& face_range, PolygonMesh& pmesh,
                        const py::dict& np = py::dict()) :
    CGAL::Polygon_mesh_processing::Adaptive_sizing_field<PolygonMesh>
    (tol, {py::cast<FT>(edge_len_min_max[0]), py::cast<FT>(edge_len_min_max[1])},
    list2vec<face_descriptor>(face_range), pmesh, internal::parse_pmp_np<PolygonMesh>(np)) {};
  // py::object is_too_long(const vertex_descriptor va, const vertex_descriptor vb, const PolygonMesh& mesh) {
  //   auto res = CGAL::Polygon_mesh_processing::Adaptive_sizing_field<PolygonMesh>::is_too_long(va, vb, mesh);
  //   if (res) {
  //     return py::cast(res.value());
  //   } else {
  //     return py::none();
  //   }
  // }
  // py::object is_too_short(const halfedge_descriptor h, const PolygonMesh& mesh) {
  //   auto res = CGAL::Polygon_mesh_processing::Adaptive_sizing_field<PolygonMesh>::is_too_short(h, mesh);
  //   if (res) {
  //     return py::cast(res.value());
  //   } else {
  //     return py::none();
  //   }
  // }
};

} // namespace pmp

#endif // CGALPY_ADAPTIVE_SIZING_FIELD_HPP
