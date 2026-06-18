// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <vector>
#include <functional>
#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/function.h>

#include <CGAL/Polygon_mesh_processing/repair_degeneracies.h>
#include <CGAL/Polygon_mesh_processing/repair.h>

#include "cgalpy/Named_parameter_area_threshold.hpp"
#include "cgalpy/Named_parameter_cap_threshold.hpp"
#include "cgalpy/Named_parameter_collapse_length_threshold.hpp"
#include "cgalpy/Named_parameter_dry_run.hpp"
#include "cgalpy/Named_parameter_flip_triangle_height_threshold.hpp"
#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_vertex_point_map.hpp"
#include "cgalpy/Named_parameter_needle_threshold.hpp"
#include "cgalpy/Named_parameter_volume_threshold.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace cgalpy {
namespace pmp {

/*! A class template that wraps the face-range overload of
 * CGAL::Polygon_mesh_processing::remove_almost_degenerate_faces()
 */
template <typename NamedParameter, typename FaceRange, typename TriangleMesh>
struct Remove_almost_degenerate_faces_range_wrapper {
  static auto call(NamedParameter& np, FaceRange&& face_range,
                   TriangleMesh&& tmesh)
  {
    return PMP::remove_almost_degenerate_faces
      (std::forward<FaceRange>(face_range),
       std::forward<TriangleMesh>(tmesh), np);
  }
};

/*! A class template that wraps the full-mesh overload of
 * CGAL::Polygon_mesh_processing::remove_almost_degenerate_faces()
 */
template <typename NamedParameter, typename TriangleMesh>
struct Remove_almost_degenerate_faces_mesh_wrapper {
  static auto call(NamedParameter& np, TriangleMesh&& tmesh)
  {
    return PMP::remove_almost_degenerate_faces
      (faces(tmesh), std::forward<TriangleMesh>(tmesh), np);
  }
};

/*! A class template that wraps the function template
 * CGAL::Polygon_mesh_processing::remove_connected_components_of_negligible_size()
 */
template <typename NamedParameter, typename... Args>
struct Remove_connected_components_of_negligible_size_wrapper {
  static auto call(NamedParameter& np, Args&&... args)
  {
    return PMP::remove_connected_components_of_negligible_size
      (std::forward<Args>(args)..., np);
  }
};

//!
template <typename PolygonMesh>
auto remove_almost_degenerate_faces_r(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                                      PolygonMesh& pmesh,
                                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Pnt = Point_3;
  auto eicm = get_edge_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP0",
     np.contains("edge_is_constrained_map") ?
     np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP1",
     np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  std::function<bool(Pnt, Pnt, Pnt)> filter =
    [](const Point_3&, const Pnt&, const Pnt&) { return true; };
  if (np.contains("filter")) {
    try {
      filter = py::cast<std::function<bool(Pnt, Pnt, Pnt)>>(np["filter"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast filter to std::function<bool(Point_3, Point_3, Point_3)>");
    }
  }

  struct Filter {
    std::function<bool(Point_3, Point_3, Point_3)> filter;
    bool operator()(const Point_3& p0, const Point_3& p1, const Point_3& p2) const { return filter(p0, p1, p2); }
  };

  Filter f;
  f.filter = filter;
  auto default_np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_vertex_point_map<Pm> vertex_point_map_op;
  cgalpy::Named_parameter_cap_threshold cap_threshold_op;
  cgalpy::Named_parameter_needle_threshold needle_threshold_op;
  cgalpy::Named_parameter_collapse_length_threshold collapse_length_threshold_op;
  cgalpy::Named_parameter_flip_triangle_height_threshold
    flip_triangle_height_threshold_op;
  cgalpy::Named_parameter_wrapper
    <Remove_almost_degenerate_faces_range_wrapper,
     const std::vector<Fd>&, Pm&>
    wrapper(face_range, pmesh);
  auto retv = cgalpy::named_parameter_applicator
    (wrapper, default_np, np, geom_traits_op, vertex_point_map_op,
     cap_threshold_op, needle_threshold_op, collapse_length_threshold_op,
     flip_triangle_height_threshold_op);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ?
    pmesh.remove_property_map(eicm) : void();
  ! np.contains("vertex_is_constrained_map") ?
    pmesh.remove_property_map(vicm) : void();
#endif

  return retv;
}

//!
template <typename TriangleMesh>
auto remove_almost_degenerate_faces(TriangleMesh& tmesh, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  auto eicm = get_edge_prop_map<Tm, bool>(tmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<TriangleMesh, bool>(tmesh, "INTERNAL_MAP1",
                                                      np.contains("vertex_is_constrained_map") ?
                                                      np["vertex_is_constrained_map"] : py::none());
  std::function<bool(Point_3, Point_3, Point_3)> filter =
    [](const Point_3&, const Point_3&, const Point_3&) { return true; };
  if (np.contains("filter")) {
    try {
      filter = py::cast<std::function<bool(Point_3, Point_3, Point_3)>>(np["filter"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast filter to std::function<bool(Point_3, Point_3, Point_3)>");
    }
  }
  struct Filter {
    std::function<bool(Point_3, Point_3, Point_3)> filter;
    bool operator()(const Point_3& p0, const Point_3& p1, const Point_3& p2) const
    { return filter(p0, p1, p2); }
  };
  Filter f; f.filter = filter;
  auto default_np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_vertex_point_map<Tm> vertex_point_map_op;
  cgalpy::Named_parameter_cap_threshold cap_threshold_op;
  cgalpy::Named_parameter_needle_threshold needle_threshold_op;
  cgalpy::Named_parameter_collapse_length_threshold collapse_length_threshold_op;
  cgalpy::Named_parameter_flip_triangle_height_threshold
    flip_triangle_height_threshold_op;
  cgalpy::Named_parameter_wrapper
    <Remove_almost_degenerate_faces_mesh_wrapper, Tm&>
    wrapper(tmesh);
  auto retv = cgalpy::named_parameter_applicator
    (wrapper, default_np, np, geom_traits_op, vertex_point_map_op,
     cap_threshold_op, needle_threshold_op, collapse_length_threshold_op,
     flip_triangle_height_threshold_op);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ? tmesh.remove_property_map(eicm) : void();
  ! np.contains("vertex_is_constrained_map") ? tmesh.remove_property_map(vicm) : void();
#endif

  return retv;
}

//!
template <typename TriangleMesh>
auto remove_connected_components_of_negligible_size(TriangleMesh& tmesh,
                                                    const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  auto eicm = get_edge_prop_map<Tm, bool>(tmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  bool fim_flag = np.contains("face_index_map");
  auto call_pmp = [&]() {
    auto default_np = CGAL::parameters::default_values();
    cgalpy::Named_parameter_geom_traits geom_traits_op;
    cgalpy::Named_parameter_vertex_point_map<Tm> vertex_point_map_op;
    cgalpy::Named_parameter_area_threshold area_threshold_op;
    cgalpy::Named_parameter_volume_threshold volume_threshold_op;
    cgalpy::Named_parameter_dry_run dry_run_op;
    cgalpy::Named_parameter_wrapper
      <Remove_connected_components_of_negligible_size_wrapper, Tm&>
      wrapper(tmesh);
    return cgalpy::named_parameter_applicator
      (wrapper, default_np, np, geom_traits_op, vertex_point_map_op,
       area_threshold_op, volume_threshold_op, dry_run_op);
  };
  std::size_t retv;
  if (fim_flag) {
    auto fim = get_face_prop_map<Tm, std::size_t>(tmesh, "INTERNAL_MAP1", np["face_index_map"]);
    retv = call_pmp();
  }
  else {
    retv = call_pmp();
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ? tmesh.remove_property_map(eicm) : void();
#endif

  return retv;
}

}
} // namespace cgalpy

//!
void export_pmp_geometric_repair(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

  m.def("remove_almost_degenerate_faces", &cgalpy::pmp::remove_almost_degenerate_faces_r<Pm>,
        py::arg("face_range"), py::arg("tmesh"), py::arg("np") = py::dict(),
        "Removes almost degenerate faces from the given face range of a triangle mesh.");
  m.def("remove_almost_degenerate_faces", &cgalpy::pmp::remove_almost_degenerate_faces<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict(),
        "Removes almost degenerate faces from a triangle mesh.");
  m.def("remove_connected_components_of_negligible_size",
        &cgalpy::pmp::remove_connected_components_of_negligible_size<Pm>, // TODO: output_iterator
        py::arg("tmesh"), py::arg("np") = py::dict(),
        "Removes connected components of negligible size from a triangle mesh.");
  m.def("remove_isolated_vertices", &PMP::remove_isolated_vertices<Pm>,
        py::arg("pmesh"),
        "Removes isolated vertices from a polygon mesh.");
}
