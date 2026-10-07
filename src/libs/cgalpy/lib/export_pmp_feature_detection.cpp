// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <set>
#include <utility>

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/set.h>

#include <CGAL/Polygon_mesh_processing/detect_features.h>

#include "cgalpy/Named_parameter_first_index.hpp"
#include "cgalpy/Named_parameter_geom_traits.hpp"
#include "cgalpy/Named_parameter_vertex_feature_degree_map.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

//! Apply simple named parameters for feature detection.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_feature_detection_named_parameters(const py::dict& params,
                                              Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_vertex_feature_degree_map<PolygonMesh>
    vertex_feature_degree_map_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, geom_traits_op, vertex_feature_degree_map_op);
}

//! Apply simple named parameters for sharp-edge segmentation.
template <typename PolygonMesh, template <typename...> class Wrapper,
          typename... Args>
auto apply_feature_segmentation_named_parameters(const py::dict& params,
                                                 Args&&... args)
{
  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_geom_traits geom_traits_op;
  cgalpy::Named_parameter_vertex_feature_degree_map<PolygonMesh>
    vertex_feature_degree_map_op;
  cgalpy::Named_parameter_first_index first_index_op;

  cgalpy::Named_parameter_wrapper<Wrapper, Args...>
    wrapper(std::forward<Args>(args)...);
  return cgalpy::named_parameter_applicator
    (wrapper, np, params, geom_traits_op, vertex_feature_degree_map_op,
     first_index_op);
}

//! Wrap CGAL::Polygon_mesh_processing::detect_sharp_edges().
template <typename NamedParameter, typename... Args>
struct Detect_sharp_edges_wrapper;

template <typename NamedParameter, typename PolygonMesh, typename Angle,
          typename EdgeIsFeatureMap>
struct Detect_sharp_edges_wrapper<NamedParameter, PolygonMesh, Angle,
                                  EdgeIsFeatureMap> {
  static auto call(NamedParameter& np, PolygonMesh&& pmesh, Angle&& angle,
                   EdgeIsFeatureMap&& edge_is_feature_map)
  {
    return PMP::detect_sharp_edges
      (std::forward<PolygonMesh>(pmesh), std::forward<Angle>(angle),
       std::forward<EdgeIsFeatureMap>(edge_is_feature_map), np);
  }
};

//! Wrap CGAL::Polygon_mesh_processing::sharp_edges_segmentation().
template <typename NamedParameter, typename... Args>
struct Sharp_edges_segmentation_wrapper;

template <typename NamedParameter, typename PolygonMesh, typename Angle,
          typename EdgeIsFeatureMap, typename PatchIdMap>
struct Sharp_edges_segmentation_wrapper<NamedParameter, PolygonMesh, Angle,
                                        EdgeIsFeatureMap, PatchIdMap> {
  static auto call(NamedParameter& np, PolygonMesh&& pmesh, Angle&& angle,
                   EdgeIsFeatureMap&& edge_is_feature_map,
                   PatchIdMap&& patch_id_map)
  {
    return PMP::sharp_edges_segmentation
      (std::forward<PolygonMesh>(pmesh), std::forward<Angle>(angle),
       std::forward<EdgeIsFeatureMap>(edge_is_feature_map),
       std::forward<PatchIdMap>(patch_id_map), np);
  }
};

//!
template <typename PolygonMesh, typename EBMap>
void detect_sharp_edges(PolygonMesh& pmesh, double angle_in_deg, EBMap ebmap, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  apply_feature_detection_named_parameters
    <Pm, Detect_sharp_edges_wrapper>(np, pmesh, angle_in_deg, ebmap);
}

//!
template<typename PolygonMesh, typename EdgeIsFeatureMap, typename PatchIdMap>
typename boost::graph_traits<PolygonMesh>::faces_size_type
sharp_edges_segmentation(PolygonMesh& pmesh, FT angle_in_deg, EdgeIsFeatureMap edge_is_feature_map,
                         PatchIdMap patch_id_map, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using faces_size_type = typename boost::graph_traits<Pm>::faces_size_type;

  auto vfdm = get_vertex_prop_map<Pm, int>(pmesh, "INTERNAL_MAP0", py::none());
  faces_size_type num_patches;
  bool fimap = np.contains("face_index_map");
  bool vimap = np.contains("vertex_index_map");

  if (fimap && vimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    auto vipm = get_vertex_prop_map<Pm, std::set<int>>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    num_patches = apply_feature_segmentation_named_parameters
      <Pm, Sharp_edges_segmentation_wrapper>
      (np, pmesh, angle_in_deg, edge_is_feature_map, patch_id_map);
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    num_patches = apply_feature_segmentation_named_parameters
      <Pm, Sharp_edges_segmentation_wrapper>
      (np, pmesh, angle_in_deg, edge_is_feature_map, patch_id_map);
  }
  else if (vimap) {
    auto vipm = get_vertex_prop_map<Pm, std::set<int>>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    num_patches = apply_feature_segmentation_named_parameters
      <Pm, Sharp_edges_segmentation_wrapper>
      (np, pmesh, angle_in_deg, edge_is_feature_map, patch_id_map);
  }
  else {
    num_patches = apply_feature_segmentation_named_parameters
      <Pm, Sharp_edges_segmentation_wrapper>
      (np, pmesh, angle_in_deg, edge_is_feature_map, patch_id_map);
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  pmesh.remove_property_map(vfdm);
#endif
  return num_patches;
}

//!
template<typename PolygonMesh, typename PatchIdMap, typename VdSetMap,
         typename EdgeIsFeatureMap>
auto detect_vertex_incident_patches(PolygonMesh& pmesh, const PatchIdMap patch_id_map,
                                    const EdgeIsFeatureMap edge_is_feature_map, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Vd = typename Gt::vertex_descriptor;
  using Fd = typename Gt::face_descriptor;
  using Vt = typename boost::property_traits<PatchIdMap>::value_type;
  using Vipt = CGAL::vertex_incident_patches_t<int>;
  using VIP = typename boost::property_map<Pm, Vipt>::type;

  VIP vip = get(CGAL::vertex_incident_patches_t<Vt>(), pmesh);
  PMP::detect_vertex_incident_patches(pmesh, patch_id_map, vip,
                                      edge_is_feature_map);
  // go back to the py::set for the map
  VdSetMap retv;
  for (auto v : vertices(pmesh)) {
    py::set s;
    for (auto p : vip[v]) s.add(p);
    retv[v] = s;
  }
  return retv;
}

}
} // namespace cgalpy

//!
void export_pmp_feature_detection(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Gt = boost::graph_traits<Pm>;
  using Vd = typename Gt::vertex_descriptor;
  using Ed = typename Gt::edge_descriptor;
  using Fd = typename Gt::face_descriptor;

  using Edge_bool_map = Pm::Property_map<Ed, bool>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Edge_bool_tag = CGAL::dynamic_edge_property_t<bool>;
  using Edge_bool_map = boost::property_map<Pm, Edge_bool_tag>::type;
#endif

  // Feature Detection Functions
  m.def("detect_sharp_edges", &cgalpy::pmp::detect_sharp_edges<Pm, Edge_bool_map>,
        py::arg("pmesh"), py::arg("angle_in_deg"), py::arg("edge_is_feature_map"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_detect_sharp_edges);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("sharp_edges_segmentation",
        &cgalpy::pmp::sharp_edges_segmentation<Pm, Pm::Property_map<Ed, bool>, Pm::Property_map<Fd, int>>,
        py::arg("pmesh"), py::arg("angle_in_deg"), py::arg("edge_is_feature_map"),
        py::arg("patch_id_map"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_sharp_edges_segmentation);
  m.def("detect_vertex_incident_patches",
        &cgalpy::pmp::detect_vertex_incident_patches<Pm, Pm::Property_map<Fd, int>, Pm::Property_map<Vd, py::set>,
                                             Pm::Property_map<Ed, bool>>,
        py::arg("pmesh"), py::arg("patch_id_map"), py::arg("edge_is_feature_map"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_detect_vertex_incident_patches);
#endif
}
