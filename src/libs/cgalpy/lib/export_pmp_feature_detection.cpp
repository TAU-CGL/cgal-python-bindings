// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <set>

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/set.h>

#include <CGAL/Polygon_mesh_processing/detect_features.h>

//! \todo remove
#include "CGALPY/pmp_np_parser.hpp"
# include "CGALPY/pmp_helpers.hpp"

#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

//!
template <typename PolygonMesh, typename EBMap>
void detect_sharp_edges(PolygonMesh& pmesh,
                        double angle_in_deg,
                        EBMap ebmap,
                        const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  if (np.contains("vertex_face_degree_map")) {
    auto vfdm = get_vertex_prop_map<Pm, int>
      (pmesh, "INTERNAL_MAP0",
       np.contains("vertex_face_degree_map") ?
       np["vertex_face_degree_map"] : py::none());
    PMP::detect_sharp_edges(pmesh, angle_in_deg, ebmap,
                            internal::parse_pmp_np<Pm>(np)
                            .vertex_feature_degree_map(vfdm));
  }
  else {
    PMP::detect_sharp_edges(pmesh, angle_in_deg, ebmap,
                            internal::parse_pmp_np<Pm>(np));
  }
}

//!
template<typename PolygonMesh, typename EdgeIsFeatureMap, typename PatchIdMap>
typename boost::graph_traits<PolygonMesh>::faces_size_type
sharp_edges_segmentation(PolygonMesh& pmesh,
                         FT angle_in_deg,
                         EdgeIsFeatureMap edge_is_feature_map,
                         PatchIdMap patch_id_map,
                         const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using faces_size_type = typename boost::graph_traits<Pm>::faces_size_type;

  auto vfdm = get_vertex_prop_map<Pm, int>(pmesh, "INTERNAL_MAP0",
    np.contains("vertex_face_degree_map") ? np["vertex_face_degree_map"] : py::none());
  faces_size_type num_patches;
  bool fimap = np.contains("face_index_map");
  bool vimap = np.contains("vertex_index_map");

  if (fimap && vimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    auto vipm = get_vertex_prop_map<Pm, std::set<int>>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    num_patches = PMP::sharp_edges_segmentation(pmesh, angle_in_deg, edge_is_feature_map, patch_id_map,
                                                internal::parse_pmp_np<Pm>(np)
                                                .vertex_feature_degree_map(vfdm)
                                                .face_index_map(fim)
                                                .vertex_incident_patches_map(vipm));
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    num_patches = PMP::sharp_edges_segmentation(pmesh, angle_in_deg, edge_is_feature_map, patch_id_map,
                                                internal::parse_pmp_np<Pm>(np)
                                                .vertex_feature_degree_map(vfdm)
                                                .face_index_map(fim));
  }
  else if (vimap) {
    auto vipm = get_vertex_prop_map<Pm, std::set<int>>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    num_patches = PMP::sharp_edges_segmentation(pmesh, angle_in_deg, edge_is_feature_map, patch_id_map,
                                                internal::parse_pmp_np<Pm>(np)
                                                .vertex_feature_degree_map(vfdm)
                                                .vertex_incident_patches_map(vipm));
  }
  else {
    num_patches = PMP::sharp_edges_segmentation(pmesh, angle_in_deg, edge_is_feature_map, patch_id_map,
                                                internal::parse_pmp_np<Pm>(np)
                                                .vertex_feature_degree_map(vfdm));
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("vertex_face_degree_map")) {
    pmesh.remove_property_map(vfdm);
  }
#endif
  return num_patches;
}

//!
template<typename PolygonMesh, typename PatchIdMap, typename VdSetMap,
         typename EdgeIsFeatureMap>
auto detect_vertex_incident_patches(PolygonMesh& pmesh,
                                    const PatchIdMap patch_id_map,
                                    const EdgeIsFeatureMap edge_is_feature_map,
                                    const py::dict& np = py::dict()) {
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

//!
void export_pmp_feature_detection(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Edge_bool_map = Pm::Property_map<Ed, bool>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Edge_bool_tag = CGAL::dynamic_edge_property_t<bool>;
  using Edge_bool_map = boost::property_map<Pm, Edge_bool_tag>::type;
#endif

  // Feature Detection Functions
  m.def("detect_sharp_edges", &pmp::detect_sharp_edges<Pm, Edge_bool_map>,
        py::arg("pm"), py::arg("angle_in_deg"), py::arg("edge_is_feature_map"),
        py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("sharp_edges_segmentation",
        &pmp::sharp_edges_segmentation<Pm, Pm::Property_map<Ed, bool>, Pm::Property_map<Fd, int>>,
        py::arg("pmesh"), py::arg("angle_in_deg"),
        py::arg("edge_is_feature_map"),
        py::arg("patch_id_map"), py::arg("np") = py::dict());
  m.def("detect_vertex_incident_patches",
        &pmp::detect_vertex_incident_patches<Pm, Pm::Property_map<Fd, int>, Pm::Property_map<Vd, py::set>, Pm::Property_map<Ed, bool>>,
        py::arg("pmesh"), py::arg("patch_id_map"),
        py::arg("vertex_incident_patches_map"), py::arg("np") = py::dict());
#endif
}
