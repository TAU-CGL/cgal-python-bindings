// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <functional>
#include <vector>
#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup_extension.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>

#include "CGALPY/Default_orientation_visitor.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//!
template <typename Point_3, typename Visitor>
auto orient_polygon_soup(std::vector<Point_3>& points, std::vector<std::vector<std::size_t>>& polygons,
                         const py::dict& np = py::dict()) {
  auto visitor = np.contains("visitor") ? py::cast<Visitor>(np["visitor"]) : Visitor();
  if (PMP::orient_polygon_soup(points, polygons)) return std::make_pair(points, polygons);
  else throw std::runtime_error("Could not orient the polygon soup");
}

//!
template <typename TriangleMesh>
auto orient(TriangleMesh& tm, const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP0",
                                                            np.contains("face_index_map") ?
                                                            np["face_internal_map"] : py::none());
    PMP::orient(tm);
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("face_index_map")) tm.remove_property_map(fim);
#endif
  }
  else {
    PMP::orient(tm);
  }
}

//!
template <typename TriangleMesh>
auto does_bound_a_volume(TriangleMesh& tm,
                         const py::dict& np = py::dict()) {
  bool retv;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<TriangleMesh, bool>(tm, "INTERNAL_MAP0",
                                                     np.contains("face_index_map") ? np["face_index_map"] : py::none());
    retv = PMP::does_bound_a_volume(tm);
  }
  else {
    retv = PMP::does_bound_a_volume(tm);
  }
  return retv;
}

//!
template <typename PolygonMesh>
void orient_to_bound_a_volume(PolygonMesh& tm,
                              const py::dict& np = py::dict()) {
  if(np.contains("face_index_map")) {
    auto fim = get_face_prop_map<PolygonMesh, std::size_t>(tm, "INTERNAL_MAP0",
                                                           np.contains("face_index_map") ?
                                                           np["face_index_map"] : py::none());
    PMP::orient_to_bound_a_volume(tm);
  }
  else {
    PMP::orient_to_bound_a_volume(tm);
  }
}

//!
template <typename TriangleMesh, typename VolumeFaceIndexMap>
auto volume_connected_components(TriangleMesh& tm, VolumeFaceIndexMap volume_id_map, const py::dict& np = py::dict()) {
  std::vector<std::size_t> ccitvi, nl;
  std::vector<bool> icoo;
  std::vector<std::vector<std::size_t>> vi;
  std::vector<std::pair<std::size_t, std::size_t>> ivpot;
  std::size_t retv;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP0",
                                                            np.contains("face_index_map") ?
                                                            np["face_index_map"] : py::none());
    if (np.contains("face_connected_component_map")) {
      auto fccm = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP1",
                                                               np.contains("face_connected_component_map") ?
                                                               np["face_connected_component_map"] : py::none());
        retv = PMP::volume_connected_components(tm, volume_id_map);
    }
    else {
      retv = PMP::volume_connected_components(tm, volume_id_map);
    }
  }
  else {
    if (np.contains("face_connected_component_map")) {
      auto fccm = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP1",
                                                               np.contains("face_connected_component_map") ?
                                                               np["face_connected_component_map"] : py::none());
      retv = PMP::volume_connected_components(tm, volume_id_map);
    }
    else {
      retv = PMP::volume_connected_components(tm, volume_id_map);
    }
  }
  return std::make_tuple(retv, ccitvi, nl, icoo, vi, ivpot);
}

//!
template <typename TriangleMesh>
auto is_outward_oriented(TriangleMesh& tm, const py::dict& np = py::dict()) {
  return PMP::is_outward_oriented(tm);
}

//!
template <typename PolygonMesh>
auto reverse_face_orientations(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                               PolygonMesh& pm) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<Fd> out;
  PMP::reverse_face_orientations(face_range, pm);
}

//!
auto duplicate_non_manifold_edges_in_polygon_soup(std::vector<Point_3>& points,
                                                       std::vector<std::vector<std::size_t>>& polygons) {
  bool duplicated = PMP::duplicate_non_manifold_edges_in_polygon_soup(points, polygons);
  return std::make_tuple(points, polygons, duplicated);
}

//!
template <typename PolygonMesh>
auto orient_triangle_soup_with_reference_triangle_mesh(const PolygonMesh& tm_ref, std::vector<Point_3>& points,
                                                       std::vector<std::vector<std::size_t>>& triangles,
                                                       const py::dict& np1 = py::dict(),
                                                       const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;
  // auto vpm = get_vertex_point_map(tm_ref, np1);
  PMP::orient_triangle_soup_with_reference_triangle_mesh<TAG>(tm_ref, points, triangles);
  return std::make_tuple(points, triangles);
}

//!
auto orient_triangle_soup_with_reference_triangle_soup(const std::vector<Point_3>& ref_points,
                                                       const std::vector<std::vector<std::size_t>>& ref_faces,
                                                       std::vector<Point_3>& points,
                                                       std::vector<std::vector<std::size_t>>& faces,
                                                       const py::dict& np1 = py::dict(),
                                                       const py::dict& np2 = py::dict()) {
  PMP::orient_triangle_soup_with_reference_triangle_soup(ref_points, ref_faces, points, faces); // doesn't work for some reason
}

//!
template <typename PolygonMesh>
void merge_reversible_connected_components(PolygonMesh& pm, const py::dict& np = py::dict()) {
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<PolygonMesh, std::size_t>(pm, "INTERNAL_MAP0",
                                                           np.contains("face_index_map") ?
                                                           np["face_internal_map"] : py::none());
    PMP::merge_reversible_connected_components(pm);
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("face_index_map")) pm.remove_property_map(fim);
#endif
  }
  else {
    PMP::merge_reversible_connected_components(pm);
  }
}

//!
template <typename PolygonMesh, typename FaceBitMap>
auto compatible_orientations(PolygonMesh& pm, FaceBitMap fbm, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto fpim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP0",
                                                 np.contains("face_patch_index_map") ?
                                                 np["face_patch_index_map"] : py::none());

  auto retv = PMP::compatible_orientations(pm, fbm);
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (!np.contains("face_patch_index_map")) {
    pm.remove_property_map(fpim);
  }
#endif
  return retv;
}

// visitor stuff
//!
void set_polygon_orientation_reversed(Default_orientation_visitor& v,
                                      const std::function<void(std::size_t)>& f) {
  v.set_polygon_orientation_reversed(f);
}

//!
void set_vertex_id_in_polygon_replaced(Default_orientation_visitor& v,
                                       const std::function<void(std::size_t, std::size_t, std::size_t)>& f) {
  v.set_vertex_id_in_polygon_replaced(f);
}

//!
void set_duplicated_vertex(Default_orientation_visitor& v,
                           const std::function<void(std::size_t, std::size_t)>& f) {
  v.set_duplicated_vertex(f);
}

//!
void set_non_manifold_edge(Default_orientation_visitor& v,
                           const std::function<void(std::size_t, std::size_t, std::size_t)>& f) {
  v.set_non_manifold_edge(f);
}

//!
void set_non_manifold_vertex(Default_orientation_visitor& v,
                             const std::function<void(std::size_t, std::size_t)>& f) {
  v.set_non_manifold_vertex(f);
}

//!
void set_link_connected_polygons(Default_orientation_visitor& v,
                                 const std::function<void(std::size_t, std::vector<std::size_t>)>& f) {
  v.set_link_connected_polygons(f);
}

} // namespace pmp

//!
void export_pmp_orientation(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = boost::graph_traits<Pm>::face_descriptor;
  using faces_size_type = boost::graph_traits<Pm>::faces_size_type;
  using Dov = pmp::Default_orientation_visitor;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Face_size_type_map = Pm::Property_map<Fd, faces_size_type>;
  using Face_bit_map = Pm::Property_map<Fd, bool>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Dfp_fst = CGAL::dynamic_face_property_t<faces_size_type>;
  using Dfp_bool = CGAL::dynamic_face_property_t<bool>;
  using Face_size_type_map = boost::property_map<Pm, Dfp_fst>;
  using Face_bit_map = boost::property_map<Pm, Dfp_bool>;
#endif

  m.def("orient_polygon_soup", &pmp::orient_polygon_soup<Point_3, Dov>,
      py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());
  m.def("orient", &pmp::orient<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("does_bound_a_volume", &pmp::does_bound_a_volume<Pm>, // TODO: is_cc_outward_oriented
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("orient_to_bound_a_volume", &pmp::orient_to_bound_a_volume<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
#if CGALPY_PMP_POLYGONAL_MESH == 1
  m.def("volume_connected_components", &pmp::volume_connected_components<Pm, Face_size_type_map>,
        py::arg("tm"), py::arg("volume_id_map"), py::arg("np") = py::dict());
#endif
  m.def("is_outward_oriented", &pmp::is_outward_oriented<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("reverse_face_orientations", &pmp::reverse_face_orientations<Pm>,
        py::arg("face_range"), py::arg("pmesh"));
  m.def("reverse_face_orientations", &PMP::reverse_face_orientations<Pm>,
        py::arg("pmesh"));
  m.def("duplicate_non_manifold_edges_in_polygon_soup", &pmp::duplicate_non_manifold_edges_in_polygon_soup,
        py::arg("points"), py::arg("polygons"));
  m.def("orient_triangle_soup_with_reference_triangle_mesh",
        &pmp::orient_triangle_soup_with_reference_triangle_mesh<Pm>, // TODO: point_map
        py::arg("tm_ref"), py::arg("points"), py::arg("triangles"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  // m.def("orient_triangle_soup_with_reference_triangle_soup", &pmp::orient_triangle_soup_with_reference_triangle_soup, // TODO: point_map
  //       py::arg("ref_points"), py::arg("ref_faces"), py::arg("points"), py::arg("faces"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("merge_reversible_connected_components", &pmp::merge_reversible_connected_components<Pm>,
         py::arg("pm"), py::arg("np") = py::dict());
#if CGALPY_PMP_POLYGONAL_MESH == 1
  m.def("compatible_orientations", &pmp::compatible_orientations<Pm, Face_bit_map>,
        py::arg("pm"), py::arg("face_bit_map"), py::arg("np") = py::dict());
#endif

  m.def("set_non_manifold_edge", &pmp::set_non_manifold_edge);
  m.def("set_non_manifold_vertex", &pmp::set_non_manifold_vertex);
  m.def("set_duplicated_vertex", &pmp::set_duplicated_vertex);
  m.def("set_vertex_id_in_polygon_replaced", &pmp::set_vertex_id_in_polygon_replaced);
  m.def("set_polygon_orientation_reversed", &pmp::set_polygon_orientation_reversed);
  m.def("set_link_connected_polygons", &pmp::set_link_connected_polygons);

  py::class_<Dov>(m, "Default_orientation_visitor")
    .def(py::init<>())
    ;
}
