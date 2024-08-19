#ifndef CGALPY_ADAPTIVE_SIZING_FIELD_HPP
#define CGALPY_ADAPTIVE_SIZING_FIELD_HPP

#include <nanobind/nanobind.h>
#include <CGAL/Polygon_mesh_processing/Adaptive_sizing_field.h>
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/pmp_np_parser.hpp"

namespace py = nanobind;

namespace pmp {

template <typename PolygonMesh>
struct Adaptive_sizing_field : public CGAL::Polygon_mesh_processing::Adaptive_sizing_field<PolygonMesh> {
  using Gt = typename boost::graph_traits<PolygonMesh>;
  using vertex_descriptor = typename Gt::vertex_descriptor;
  using halfedge_descriptor = typename Gt::halfedge_descriptor;
  using face_descriptor = typename Gt::face_descriptor;
  Adaptive_sizing_field(const FT tol, const std::tuple<FT, FT>& edge_len_min_max, const std::vector<face_descriptor>& face_range, PolygonMesh& pmesh,
                        const py::dict np = {}) :
    CGAL::Polygon_mesh_processing::Adaptive_sizing_field<PolygonMesh>
    (tol, {std::get<0>(edge_len_min_max), std::get<1>(edge_len_min_max)},
    face_range, pmesh, internal::parse_pmp_np<PolygonMesh>(np)) {};
};

} // namespace pmp

#endif // CGALPY_ADAPTIVE_SIZING_FIELD_HPP
