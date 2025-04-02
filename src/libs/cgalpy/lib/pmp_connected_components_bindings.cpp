// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/stl/vector.h>

#include <CGAL/Polygon_mesh_processing/connected_components.h>

//! \remove
#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/pmp_helpers.hpp"

#include "CGALPY/polygon_mesh_processing_types.hpp"

namespace PMP = CGAL::Polygon_mesh_processing;
namespace py = nanobind;

namespace pmp {

//!
template <typename PolygonMesh>
auto connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor
                         seed_face,
                         PolygonMesh& pm,
                         const py::dict& np) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<Fd> lst;
  auto it = std::back_inserter(lst);

  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());

  PMP::connected_component(seed_face, pm, it,
                           internal::parse_pmp_np<PolygonMesh>(np)
                           .edge_is_constrained_map(eicm));

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) {
    pm.remove_property_map(eicm);
  }
#endif

  return lst;
}

//!
template <typename PolygonMesh, typename FaceComponentMap>
auto connected_components_map(PolygonMesh& pm,
                              FaceComponentMap& fccmap,
                              const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Fcm = FaceComponentMap;
  using value_type = typename boost::property_traits<Fcm>::value_type;
  value_type retv;
  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = PMP::connected_components(pm, fccmap,
                                     internal::parse_pmp_np<PolygonMesh>(np)
                                     .edge_is_constrained_map(eicm)
                                     .face_index_map(fim));
  }
  else {
    retv = PMP::connected_components(pm, fccmap,
                                     internal::parse_pmp_np<PolygonMesh>(np)
                                     .edge_is_constrained_map(eicm));
  }
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif

  return retv;
}

//!
template <typename PolygonMesh, typename FaceComponentMap>
auto keep_connected_components_map(PolygonMesh& pm,
                                   std::vector<std::size_t> components_to_keep,
                                   FaceComponentMap& fccmap,
                                   const py::dict& np = py::dict()) {
  if (np.contains("vertex_index_map")) {
    auto vim = get_vertex_prop_map<PolygonMesh, std::size_t>(pm, "INTERNAL_MAP0",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    PMP::keep_connected_components(pm, components_to_keep, fccmap,
                                   internal::parse_pmp_np<PolygonMesh>(np)
                                   .vertex_index_map(vim));
  }
  else {
    PMP::keep_connected_components(pm, components_to_keep, fccmap,
                                          internal::parse_pmp_np<PolygonMesh>(np));
  }
}

//!
template <typename PolygonMesh>
auto keep_connected_components(PolygonMesh& pm,
                               const std::vector<
                               typename boost::graph_traits<PolygonMesh>::face_descriptor>& components_to_keep,
                               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());

  bool vimap = np.contains("vertex_index_map");
  bool fimap = np.contains("face_index_map");
  if (vimap && fimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP2",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::keep_connected_components(pm, components_to_keep,
                                   internal::parse_pmp_np<PolygonMesh>(np)
                                   .edge_is_constrained_map(eicm)
                                   .vertex_index_map(vim)
                                   .face_index_map(fim));
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    PMP::keep_connected_components(pm, components_to_keep,
                                   internal::parse_pmp_np<PolygonMesh>(np)
                                   .edge_is_constrained_map(eicm)
                                   .vertex_index_map(vim));
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::keep_connected_components(pm, components_to_keep,
                                   internal::parse_pmp_np<PolygonMesh>(np)
                                   .edge_is_constrained_map(eicm)
                                   .face_index_map(fim));
  }
  else {
    PMP::keep_connected_components(pm, components_to_keep,
                                   internal::parse_pmp_np<PolygonMesh>(np));
  }
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
}

//!
template <typename PolygonMesh, typename ThresholdValueType>
auto keep_large_connected_components(PolygonMesh& pmesh,
                                     const ThresholdValueType threshold_value,
                                     const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  auto fsm = get_face_prop_map<Pm, ThresholdValueType>(pmesh, "INTERNAL_MAP1",
    np.contains("face_size_map") ? np["face_size_map"] : py::none(), 1);
  bool vimap = np.contains("vertex_index_map");
  bool fimap = np.contains("face_index_map");
  std::size_t retv;

  if (vimap && fimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP3",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = PMP::keep_large_connected_components(pmesh, threshold_value,
                                                internal::parse_pmp_np<PolygonMesh>(np)
                                                .edge_is_constrained_map(eicm)
                                                .face_size_map(fsm)
                                                .vertex_index_map(vim)
                                                .face_index_map(fim));
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    retv = PMP::keep_large_connected_components(pmesh, threshold_value,
                                                internal::parse_pmp_np<PolygonMesh>(np)
                                                .edge_is_constrained_map(eicm)
                                                .face_size_map(fsm)
                                                .vertex_index_map(vim));
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = PMP::keep_large_connected_components(pmesh, threshold_value,
                                                internal::parse_pmp_np<PolygonMesh>(np)
                                                .edge_is_constrained_map(eicm)
                                                .face_size_map(fsm)
                                                .face_index_map(fim));
  }
  else {
    retv = PMP::keep_large_connected_components(pmesh, threshold_value,
                                                internal::parse_pmp_np<PolygonMesh>(np)
                                                .edge_is_constrained_map(eicm)
                                                .face_size_map(fsm));
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (! np.contains("face_size_map")) pmesh.remove_property_map(fsm);
#endif

  return retv;
}

//!
template <typename PolygonMesh>
auto keep_largest_connected_components(PolygonMesh& pmesh,
                                       std::size_t nb_components_to_keep,
                                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  auto fsm = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
    np.contains("face_size_map") ? np["face_size_map"] : py::none(), 1);
  bool vimap = np.contains("vertex_index_map");
  bool fimap = np.contains("face_index_map");
  std::size_t retv;

  if (vimap && fimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP3",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = PMP::keep_largest_connected_components(pmesh, nb_components_to_keep,
                                                  internal::parse_pmp_np<PolygonMesh>(np)
                                                  .edge_is_constrained_map(eicm)
                                                  .face_size_map(fsm)
                                                  .vertex_index_map(vim)
                                                  .face_index_map(fim));
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    retv = PMP::keep_largest_connected_components(pmesh, nb_components_to_keep,
                                                  internal::parse_pmp_np<PolygonMesh>(np)
                                                  .edge_is_constrained_map(eicm)
                                                  .face_size_map(fsm)
                                                  .vertex_index_map(vim));
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = PMP::keep_largest_connected_components(pmesh, nb_components_to_keep,
                                                  internal::parse_pmp_np<PolygonMesh>(np)
                                                  .edge_is_constrained_map(eicm)
                                                  .face_size_map(fsm)
                                                  .face_index_map(fim));
  }
  else {
    retv = PMP::keep_largest_connected_components(pmesh, nb_components_to_keep,
                                                  internal::parse_pmp_np<PolygonMesh>(np)
                                                  .edge_is_constrained_map(eicm)
                                                  .face_size_map(fsm));
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (! np.contains("face_size_map")) pmesh.remove_property_map(fsm);
#endif

  return retv;
}

//!
template <typename PolygonMesh, typename FaceComponentMap>
auto remove_connected_components_map(PolygonMesh& pm,
                                     const std::vector<std::size_t>& components_to_remove,
                                     const FaceComponentMap& fccmap,
                                     const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using faces_size_type = typename Gt::faces_size_type;

  if (np.contains("vertex_index_map")) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP0",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    return PMP::remove_connected_components(pm, components_to_remove, fccmap,
                                            internal::parse_pmp_np<PolygonMesh>(np)
                                            .vertex_index_map(vim));
  }
  else {
    return PMP::remove_connected_components(pm, components_to_remove, fccmap,
                                            internal::parse_pmp_np<PolygonMesh>(np));
  }
}

//!
template <typename PolygonMesh>
auto remove_connected_components(PolygonMesh& pm,
                                 const std::vector<
                                 typename boost::graph_traits<PolygonMesh>::face_descriptor>& components_to_remove,
                                 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  bool vimap = np.contains("vertex_index_map");
  bool fimap = np.contains("face_index_map");
  if (vimap && fimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP2",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::remove_connected_components(pm, components_to_remove,
                                     internal::parse_pmp_np<PolygonMesh>(np)
                                     .edge_is_constrained_map(eicm)
                                     .vertex_index_map(vim)
                                     .face_index_map(fim));
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    PMP::remove_connected_components(pm, components_to_remove,
                                            internal::parse_pmp_np<PolygonMesh>(np)
                                            .edge_is_constrained_map(eicm)
                                            .vertex_index_map(vim));
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::remove_connected_components(pm, components_to_remove,
                                     internal::parse_pmp_np<PolygonMesh>(np)
                                     .edge_is_constrained_map(eicm)
                                     .face_index_map(fim));
  }
  else {
    PMP::remove_connected_components(pm, components_to_remove,
                                     internal::parse_pmp_np<PolygonMesh>(np));
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
}

//!
template <typename PolygonMesh>
auto split_connected_components(PolygonMesh& pmesh,
                                std::vector<PolygonMesh>& cc_meshes,
                                const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;

  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
    np.contains("face_patch_map") ? np["face_internal_map"] : py::none());
  // TODO: add index maps
  PMP::split_connected_components(pmesh, cc_meshes,
                                  internal::parse_pmp_np<PolygonMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  .face_patch_map(fpm));

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (! np.contains("face_patch_map")) pmesh.remove_property_map(fpm);
#endif

  return cc_meshes;
}

} // namespace pmp

//!
void export_pmp_connected_components(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;
  using Np = CGAL::parameters::Default_named_parameters;
  using Fd = boost::graph_traits<Pm>::face_descriptor;
  using Hd = boost::graph_traits<Pm>::halfedge_descriptor;
  using Vd = boost::graph_traits<Pm>::vertex_descriptor;
  using Ed = boost::graph_traits<Pm>::edge_descriptor;
  using faces_size_type = boost::graph_traits<Pm>::faces_size_type;

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Face_component_map = Pm::Property_map<Fd, faces_size_type>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Face_component_map = boost::property_map<Pm, CGAL::dynamic_face_property_t<std::size_t>>::type;
#endif

  // Connected Components
  // m.def("connected_component", &pmp::connected_component<Pm>,
  //       py::arg("seed_face"), py::arg("pm"),
  //       py::arg("np") = py::dict());
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("connected_components",
        &pmp::connected_components_map<Pm, boost::property_map<Pm, CGAL::dynamic_face_property_t<std::size_t>>::type>,
        py::arg("pm"), py::arg("fcm"), py::arg("np") = py::dict());
  // m.def("connected_components",
  //       &pmp::connected_components_map<Pm, boost::property_map<Pm, CGAL::dynamic_face_property_t<std::uint32_t>>::type>,
  //       py::arg("pm"), py::arg("fcm"), py::arg("np") = py::dict());
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("connected_components",
        &pmp::connected_components_map<Pm, Pm::Property_map<Fd, std::size_t>>,
        py::arg("pm"), py::arg("fcm"), py::arg("np") = py::dict());
  m.def("keep_connected_components",
        &pmp::keep_connected_components_map<Pm, Face_component_map>,
        py::arg("pm"), py::arg("components_to_keep"), py::arg("fcm"),
        py::arg("np") = py::dict());
#endif
  m.def("keep_connected_components", &pmp::keep_connected_components<Pm>,
        py::arg("pm"), py::arg("components_to_keep"),
        py::arg("np") = py::dict());
  m.def("keep_large_connected_components",
        &pmp::keep_large_connected_components<Pm, std::size_t>,
        py::arg("pm"), py::arg("min_size"), py::arg("np") = py::dict());
  m.def("keep_large_connected_components",
        &pmp::keep_large_connected_components<Pm, double>,
        py::arg("pm"), py::arg("min_size"), py::arg("np") = py::dict());
  m.def("keep_largest_connected_components",
        &pmp::keep_largest_connected_components<Pm>,
        py::arg("pm"), py::arg("nb_components_to_keep"),
        py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("remove_connected_components",
        &pmp::remove_connected_components_map<Pm, Face_component_map>,
        py::arg("pm"), py::arg("components_to_remove"), py::arg("fccmap"),
        py::arg("np") = py::dict());
#endif
  m.def("remove_connected_components", &pmp::remove_connected_components<Pm>,
        py::arg("pm"), py::arg("components_to_remove"),
        py::arg("np") = py::dict());
  m.def("split_connected_components", &pmp::split_connected_components<Pm>,
        py::arg("pmesh"), py::arg("cc_meshes"), py::arg("np") = py::dict());
}
