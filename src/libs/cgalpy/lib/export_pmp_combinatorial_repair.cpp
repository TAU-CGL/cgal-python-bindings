// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <tuple>
#include <utility>
#include <vector>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Polygon_mesh_processing/manifoldness.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/merge_border_vertices.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/polygon_mesh_to_polygon_soup.h>

#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_require_same_orientation.hpp"
#include "cgalpy/Named_parameter_apply_per_connected_component.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace cgalpy {
namespace pmp {

using Point_3_vec = std::vector<Point_3>;
using Size_t_vec = std::vector<std::size_t>;

//!
template <typename PolygonMesh>
auto duplicate_non_manifold_vertices(PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  std::vector<std::vector<Vd>> out;
  auto vicm = get_vertex_prop_map<Pm, Vd>(pm, "INTERNAL_MAP0",
                                          np.contains("vertex_is_constrained_map") ?
                                          np["vertex_is_constrained_map"] : py::none());
  auto nb = PMP::duplicate_non_manifold_vertices(pm);
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

/*! A class template that wraps the function template
 * PMP::merge_duplicate_points_in_polygon_soup()
 */
template <typename T, typename... Args>
struct Merge_duplicate_points_in_polygon_soup_wrapper {
  static auto call(T np, Args&&... args)
  { return PMP::merge_duplicate_points_in_polygon_soup(std::forward<Args>(args)..., std::forward<T>(np)); }
};

//!
auto merge_duplicate_points_in_polygon_soup(Point_3_vec& pointvec, std::vector<Size_t_vec >& polyvec,
                                            const py::dict& params = py::dict()) {
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits op;
  cgalpy::Named_parameter_wrapper<Merge_duplicate_points_in_polygon_soup_wrapper,
                                  Point_3_vec&, std::vector<Size_t_vec>&>
    wrapper(pointvec, polyvec);
  return cgalpy::named_parameter_applicator(wrapper, np, params, op);
}

/*! A class template that wraps the function template
 * PMP::merge_duplicate_polygons_in_polygon_soup()
 */
template <typename T, typename... Args>
struct Merge_duplicate_polygons_in_polygon_soup_wrapper {
  static auto call(T np, Args&&... args)
  { return PMP::merge_duplicate_polygons_in_polygon_soup(std::forward<Args>(args)..., std::forward<T>(np)); }
};

//!
auto merge_duplicate_polygons_in_polygon_soup(Point_3_vec& points, std::vector<Size_t_vec >& polygons,
                                              const py::dict& params = py::dict()) {
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_require_same_orientation require_same_orientation_op;
  cgalpy::Named_parameter_wrapper<Merge_duplicate_polygons_in_polygon_soup_wrapper,
                                  Point_3_vec&, std::vector<Size_t_vec>&>
    wrapper(points, polygons);
  return cgalpy::named_parameter_applicator(wrapper, np, params, geom_traits_op,
                                           require_same_orientation_op);
}

//!
template <typename PolygonMesh>
auto merge_duplicated_vertices_in_boundary_cycle(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& h,
                                                 PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::merge_duplicated_vertices_in_boundary_cycle(h, pm);
}

//!
template <typename PolygonMesh>
auto merge_duplicated_vertices_in_boundary_cycles(PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::merge_duplicated_vertices_in_boundary_cycles(pm);
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
auto polygon_mesh_to_polygon_soup(const PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Vd = typename Gt::vertex_descriptor;
  using Fd = typename Gt::face_descriptor;
  Point_3_vec pts;
  std::vector<Size_t_vec> polys;
  PMP::polygon_mesh_to_polygon_soup(pm, pts, polys);
  return std::make_tuple(pts, polys);
}

//!
template <typename PolygonMesh>
auto polygon_soup_to_polygon_mesh(const Point_3_vec& points, const std::vector<Size_t_vec>& polygons,
                                  const py::dict& np_ps = py::dict(), const py::dict& np_pm = py::dict()) {
  using Pm = PolygonMesh;
  using vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using fd = typename boost::graph_traits<Pm>::face_descriptor;
  Pm output;

  std::vector<std::pair<int, vd>> pvvec;
  std::vector<std::pair<int, fd>> pfvec;
  PMP::polygon_soup_to_polygon_mesh(points, polygons, output);
  return std::make_tuple(output, pvvec, pfvec);
}

//!
auto remove_isolated_points_in_polygon_soup(Point_3_vec& points,
                                            std::vector<Size_t_vec>& polygons)
{ return PMP::remove_isolated_points_in_polygon_soup(points, polygons); }

/*! A class template that wraps the function template
 * PMP::repair_polygon_soup()
 */
template <typename T, typename... Args>
struct Repair_polygon_soup_wrapper {
  static void call(T np, Args&&... args)
  { PMP::repair_polygon_soup(std::forward<Args>(args)..., std::forward<T>(np)); }
};

//!
auto repair_polygon_soup(Point_3_vec& points, std::vector<Size_t_vec>& polygons,
                         const py::dict& params = py::dict()) {
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_require_same_orientation require_same_orientation_op;
  cgalpy::Named_parameter_wrapper<Repair_polygon_soup_wrapper, Point_3_vec&, std::vector<Size_t_vec>&>
    wrapper(points, polygons);
  cgalpy::named_parameter_applicator(wrapper, np, params, geom_traits_op,
                                     require_same_orientation_op);
  return std::make_tuple(points, polygons);
}

//!
template <typename PolygonMesh>
auto stitch_borders_bc(const std::vector<typename boost::graph_traits<PolygonMesh>::halfedge_descriptor>& boundary_cycle_representatives,
                       PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<Pm, std::size_t>
      (pmesh, "INTERNAL_MAP0",
       np.contains("face_index_map") ? np["face_index_map"] : py::none());
    return PMP::stitch_borders(boundary_cycle_representatives, pmesh);
  }
  else {
    return PMP::stitch_borders(boundary_cycle_representatives, pmesh);
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
  return PMP::stitch_borders(pmesh, hedge_pairs_to_stitch);
}

/*! A class template that wraps the function template
 * PMP::stitch_borders()
 */
template <typename T, typename... Args>
struct Stitch_borders_wrapper {
  static auto call(T np, Args&&... args)
  { return PMP::stitch_borders(std::forward<Args>(args)..., std::forward<T>(np)); }
};

//!
template <typename PolygonMesh>
auto stitch_borders(PolygonMesh& pmesh,
                    const py::dict& params = py::dict()) {
  using Pm = PolygonMesh;
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_apply_per_connected_component apply_per_connected_component_op;
  cgalpy::Named_parameter_wrapper<Stitch_borders_wrapper, PolygonMesh&> wrapper(pmesh);

  if (params.contains("face_index_map")) {
    auto fim = get_face_prop_map<Pm, std::size_t>
      (pmesh, "INTERNAL_MAP0",
       params.contains("face_index_map") ? params["face_index_map"] : py::none());
    return cgalpy::named_parameter_applicator(wrapper, np, params,
                                             apply_per_connected_component_op);
  }

  return cgalpy::named_parameter_applicator(wrapper, np, params,
                                           apply_per_connected_component_op);
}

//!
template <typename PolygonMesh>
auto stitch_boundary_cycle(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& h,
                           PolygonMesh& pmesh,
                           const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::stitch_boundary_cycle(h, pmesh);
}

/*! A class template that wraps the function template
 * PMP::stitch_boundary_cycles()
 */
template <typename T, typename... Args>
struct Stitch_boundary_cycles_wrapper {
  static auto call(T np, Args&&... args)
  { return PMP::stitch_boundary_cycles(std::forward<Args>(args)..., std::forward<T>(np)); }
};

//!
template <typename PolygonMesh>
auto stitch_boundary_cycles(const std::vector<typename boost::graph_traits<PolygonMesh>::halfedge_descriptor>& boundary_cycle_representatives,
                            PolygonMesh& pmesh,
                            const py::dict& params = py::dict()) {
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_wrapper<Stitch_boundary_cycles_wrapper,
                                  const std::vector<typename boost::graph_traits<PolygonMesh>::halfedge_descriptor>&,
                                  PolygonMesh&>
    wrapper(boundary_cycle_representatives, pmesh);
  return cgalpy::named_parameter_applicator(wrapper, np, params, geom_traits_op);
}

}
} // namespace cgalpy

//!
void export_pmp_combinatorial_repair(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  m.def("duplicate_non_manifold_vertices", &cgalpy::pmp::duplicate_non_manifold_vertices<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict(),
        "Duplicates non-manifold vertices of a polygon mesh.");
  m.def("is_non_manifold_vertex", &PMP::is_non_manifold_vertex<Pm>,
        py::arg("v"), py::arg("pm"),
        "Returns whether a vertex is non-manifold.");
  m.def("is_polygon_soup_a_polygon_mesh", &cgalpy::pmp::is_polygon_soup_a_polygon_mesh,
        py::arg("polygons"),
        "Returns whether a polygon soup defines a polygon mesh.");
  m.def("merge_duplicate_points_in_polygon_soup", &cgalpy::pmp::merge_duplicate_points_in_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict(),
        "Merges duplicate points in a polygon soup.");
  m.def("merge_duplicate_polygons_in_polygon_soup", &cgalpy::pmp::merge_duplicate_polygons_in_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict(),
        "Merges duplicate polygons in a polygon soup.");
  m.def("merge_duplicated_vertices_in_boundary_cycle", &cgalpy::pmp::merge_duplicated_vertices_in_boundary_cycle<Pm>,
        py::arg("h"), py::arg("pm"), py::arg("np") = py::dict(),
        "Merges duplicated vertices in a boundary cycle.");
  m.def("merge_duplicated_vertices_in_boundary_cycles", &cgalpy::pmp::merge_duplicated_vertices_in_boundary_cycles<Pm>,
        py::arg("pm"), py::arg("np") = py::dict(),
        "Merges duplicated vertices in all boundary cycles.");
  m.def("non_manifold_vertices", &cgalpy::pmp::non_manifold_vertices<Pm>,
        py::arg("pm"),
        "Returns non-manifold vertices of a polygon mesh.");
  m.def("polygon_mesh_to_polygon_soup", &cgalpy::pmp::polygon_mesh_to_polygon_soup<Pm>,
        py::arg("pm"), py::arg("np") = py::dict(),
        "Converts a polygon mesh to a polygon soup.");
  m.def("polygon_soup_to_polygon_mesh", &cgalpy::pmp::polygon_soup_to_polygon_mesh<Pm>, // TODO: point_map, ptvm, ptfm
        py::arg("points"), py::arg("polygons"), py::arg("np_ps") = py::dict(), py::arg("np_pm") = py::dict(),
        "Converts a polygon soup to a polygon mesh.");
  m.def("remove_isolated_points_in_polygon_soup", &cgalpy::pmp::remove_isolated_points_in_polygon_soup,
        py::arg("points"), py::arg("polygons"),
        "Removes isolated points from a polygon soup.");
  m.def("repair_polygon_soup", &cgalpy::pmp::repair_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict(),
        "Repairs a polygon soup.");
  m.def("stitch_borders", &cgalpy::pmp::stitch_borders_bc<Pm>,
        py::arg("boundary_cycle_representatives"), py::arg("pmesh"), py::arg("np") = py::dict(),
        "Stitches borders using boundary cycle representatives.");
  m.def("stitch_borders", &cgalpy::pmp::stitch_borders_he<Pm>,
        py::arg("pmesh"), py::arg("hedge_pairs_to_stitch"), py::arg("np") = py::dict(),
        "Stitches borders using halfedge pairs.");
  m.def("stitch_borders", &cgalpy::pmp::stitch_borders<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict(),
        "Stitches borders of a polygon mesh.");
  m.def("stitch_boundary_cycle", &cgalpy::pmp::stitch_boundary_cycle<Pm>,
        py::arg("h"), py::arg("pmesh"), py::arg("np") = py::dict(),
        "Stitches one boundary cycle of a polygon mesh.");
  m.def("stitch_boundary_cycles", &cgalpy::pmp::stitch_boundary_cycles<Pm>,
        py::arg("boundary_cycle_representatives"), py::arg("pmesh"), py::arg("np") = py::dict(),
        "Stitches boundary cycles of a polygon mesh.");
}
