// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>

#include <CGAL/Polygon_mesh_processing/manifoldness.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/merge_border_vertices.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/polygon_mesh_to_polygon_soup.h>

//! \todo remove
#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/pmp_helpers.hpp"

#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

using Point_3_vec = std::vector<Point_3>;
using Size_t_vec = std::vector<std::size_t>;

//!
template <typename PolygonMesh>
auto duplicate_non_manifold_vertices(PolygonMesh& pm,
                                     const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  std::vector<std::vector<Vd>> out;
  auto vicm = get_vertex_prop_map<Pm, Vd>
    (pm, "INTERNAL_MAP0",
     np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  auto nb = PMP::duplicate_non_manifold_vertices(pm,
                                                 internal::parse_pmp_np<Pm>(np)
                                                 .output_iterator(std::back_inserter(out)));
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("vertex_is_constrained_map")) {
    pm.remove_property_map(vicm);
  }
#endif

  return std::make_tuple(nb, out);
}

//!
auto is_polygon_soup_a_polygon_mesh(std::vector<Size_t_vec>& polygons)
{ return PMP::is_polygon_soup_a_polygon_mesh(polygons); }

//!
auto merge_duplicate_points_in_polygon_soup(Point_3_vec& pointvec,
                                            std::vector<Size_t_vec >& polyvec,
                                            const py::dict& np = py::dict()) {
  return PMP::merge_duplicate_points_in_polygon_soup(pointvec, polyvec,
                                                     internal::parse_named_parameters(np));
}

//!
auto merge_duplicate_polygons_in_polygon_soup(Point_3_vec& points,
                                              std::vector<Size_t_vec >& polygons,
                                              const py::dict& np = py::dict()) {
  return PMP::merge_duplicate_polygons_in_polygon_soup(points, polygons,
                                                       internal::parse_named_parameters(np));
}

//!
template <typename PolygonMesh>
auto merge_duplicated_vertices_in_boundary_cycle
(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& h,
 PolygonMesh& pm,
 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::merge_duplicated_vertices_in_boundary_cycle
    (h, pm, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
auto merge_duplicated_vertices_in_boundary_cycles(PolygonMesh& pm,
                                                  const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::merge_duplicated_vertices_in_boundary_cycles
    (pm, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
auto non_manifold_vertices(PolygonMesh& pm) {
  using Hd = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;
  std::vector<Hd> out;
  PMP::non_manifold_vertices(pm, std::back_inserter(out));
  return out;
}

//!
template <typename PolygonMesh>
auto polygon_mesh_to_polygon_soup(const PolygonMesh& pm,
                                  const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Vd = typename Gt::vertex_descriptor;
  using Fd = typename Gt::face_descriptor;
  Point_3_vec pts;
  std::vector<Size_t_vec> polys;
  PMP::polygon_mesh_to_polygon_soup(pm, pts, polys,
                                    internal::parse_pmp_np<Pm>(np));
  return std::make_tuple(pts, polys);
}

//!
template <typename PolygonMesh>
auto polygon_soup_to_polygon_mesh(const Point_3_vec& points,
                                  const std::vector<Size_t_vec>& polygons,
                                  const py::dict& np_ps = py::dict(),
                                  const py::dict& np_pm = py::dict()) {
  using Pm = PolygonMesh;
  using vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using fd = typename boost::graph_traits<Pm>::face_descriptor;
  Pm output;

  std::vector<std::pair<int, vd>> pvvec;
  std::vector<std::pair<int, fd>> pfvec;
  PMP::polygon_soup_to_polygon_mesh(points, polygons, output,
                                    internal::parse_pmp_np<Pm>(np_ps)
                                    .point_to_vertex_output_iterator(std::back_inserter(pvvec))
                                    .polygon_to_face_output_iterator(std::back_inserter(pfvec)),
                                    internal::parse_pmp_np<Pm>(np_pm));
  return std::make_tuple(output, pvvec, pfvec);
}

//!
auto remove_isolated_points_in_polygon_soup(Point_3_vec& points,
                                            std::vector<Size_t_vec>& polygons)
{ return PMP::remove_isolated_points_in_polygon_soup(points, polygons); }

//!
auto repair_polygon_soup(Point_3_vec& points,
                         std::vector<Size_t_vec>& polygons,
                         const py::dict& np = py::dict()) {
  PMP::repair_polygon_soup(points, polygons,
                           internal::parse_named_parameters(np));
  return std::make_tuple(points, polygons);
}

//!
template <typename PolygonMesh>
auto stitch_borders_bc(const std::vector<typename boost::graph_traits<PolygonMesh>::halfedge_descriptor>& boundary_cycle_representatives,
                       PolygonMesh& pmesh,
                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<Pm, std::size_t>
      (pmesh, "INTERNAL_MAP0",
       np.contains("face_index_map") ? np["face_index_map"] : py::none());
    return PMP::stitch_borders(boundary_cycle_representatives, pmesh,
                               internal::parse_pmp_np<Pm>(np)
                               .face_index_map(fim));
  }
  else {
    return PMP::stitch_borders(boundary_cycle_representatives, pmesh,
                               internal::parse_pmp_np<Pm>(np));
  }
}

//!
template <typename PolygonMesh>
auto stitch_borders_he(PolygonMesh& pmesh,
                       const std::vector<std::pair<typename boost::graph_traits<PolygonMesh>::halfedge_descriptor,
                       typename boost::graph_traits<PolygonMesh>::halfedge_descriptor>>& hedge_pairs_to_stitch,
                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;
  return PMP::stitch_borders(pmesh, hedge_pairs_to_stitch,
                             internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
auto stitch_borders(PolygonMesh& pmesh,
                    const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  // return PMP::stitch_borders(pmesh, internal::parse_pmp_np<Pm>(np));
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<Pm, std::size_t>
      (pmesh, "INTERNAL_MAP0",
       np.contains("face_index_map") ? np["face_index_map"] : py::none());
    return PMP::stitch_borders(pmesh, internal::parse_pmp_np<Pm>(np)
                               .face_index_map(fim));
  }
  else {
    return PMP::stitch_borders(pmesh, internal::parse_pmp_np<Pm>(np));
  }
}

//!
template <typename PolygonMesh>
auto stitch_boundary_cycle(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& h,
                           PolygonMesh& pmesh,
                           const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::stitch_boundary_cycle(h, pmesh, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
auto stitch_boundary_cycles(const std::vector<typename boost::graph_traits<PolygonMesh>::halfedge_descriptor>& boundary_cycle_representatives,
                            PolygonMesh& pmesh,
                            const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;
  return PMP::stitch_boundary_cycles(boundary_cycle_representatives, pmesh,
                                     internal::parse_pmp_np<Pm>(np));
}

}

//!
void export_pmp_combinatorial_repair(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

  m.def("duplicate_non_manifold_vertices",
        &pmp::duplicate_non_manifold_vertices<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("is_non_manifold_vertex", &PMP::is_non_manifold_vertex<Pm>,
        py::arg("v"), py::arg("pm"));
  m.def("is_polygon_soup_a_polygon_mesh", &pmp::is_polygon_soup_a_polygon_mesh,
        py::arg("polygons"));
  m.def("merge_duplicate_points_in_polygon_soup",
        &pmp::merge_duplicate_points_in_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());
  m.def("merge_duplicate_polygons_in_polygon_soup",
        &pmp::merge_duplicate_polygons_in_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());
  m.def("merge_duplicated_vertices_in_boundary_cycle",
        &pmp::merge_duplicated_vertices_in_boundary_cycle<Pm>,
        py::arg("h"), py::arg("pm"), py::arg("np") = py::dict());
  m.def("merge_duplicated_vertices_in_boundary_cycles",
        &pmp::merge_duplicated_vertices_in_boundary_cycles<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("non_manifold_vertices", &pmp::non_manifold_vertices<Pm>,
        py::arg("pm"));
  m.def("polygon_mesh_to_polygon_soup", &pmp::polygon_mesh_to_polygon_soup<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("polygon_soup_to_polygon_mesh", &pmp::polygon_soup_to_polygon_mesh<Pm>, // TODO: point_map, ptvm, ptfm
        py::arg("points"), py::arg("polygons"), py::arg("np_ps") = py::dict(),
        py::arg("np_pm") = py::dict());
  m.def("remove_isolated_points_in_polygon_soup",
        &pmp::remove_isolated_points_in_polygon_soup,
        py::arg("points"), py::arg("polygons"));
  m.def("repair_polygon_soup", &pmp::repair_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());
  m.def("stitch_borders", &pmp::stitch_borders_bc<Pm>,
        py::arg("boundary_cycle_representatives"), py::arg("pmesh"),
        py::arg("np") = py::dict());
  m.def("stitch_borders", &pmp::stitch_borders_he<Pm>,
        py::arg("pmesh"), py::arg("hedge_pairs_to_stitch"),
        py::arg("np") = py::dict());
  m.def("stitch_borders", &pmp::stitch_borders<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("stitch_boundary_cycle", &pmp::stitch_boundary_cycle<Pm>,
        py::arg("h"), py::arg("pmesh"),
        py::arg("np") = py::dict());
  m.def("stitch_boundary_cycles", &pmp::stitch_boundary_cycles<Pm>,
        py::arg("boundary_cycle_representatives"), py::arg("pmesh"),
        py::arg("np") = py::dict());
}
