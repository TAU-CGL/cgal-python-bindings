// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <vector>
#include <utility>

#include <boost/graph/graph_traits.hpp>

#include <nanobind/stl/vector.h>

#include <CGAL/Polygon_mesh_processing/connected_components.h>

#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/Named_parameter_dry_run.hpp"
#include "cgalpy/Named_parameter_edge_is_constrained_map.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/Pmp_docstrings.hpp"

namespace PMP = CGAL::Polygon_mesh_processing;
namespace py = nanobind;
namespace pmp_doc = cgalpy::pmp::docstrings;

namespace cgalpy {
namespace pmp {

/*! A class template that wraps the function template
 * CGAL::Polygon_mesh_processing::connected_components()
 */
template <typename NamedParameter, typename... Args>
struct Connected_components_wrapper {
  static auto call(NamedParameter& np, Args&&... args)
  {
    return PMP::connected_components(std::forward<Args>(args)..., np);
  }
};

/*! A class template that wraps the function template
 * CGAL::Polygon_mesh_processing::keep_large_connected_components()
 */
template <typename NamedParameter, typename... Args>
struct Keep_large_connected_components_wrapper {
  static auto call(NamedParameter& np, Args&&... args)
  {
    return PMP::keep_large_connected_components(std::forward<Args>(args)...,
                                                np);
  }
};

/*! A class template that wraps the function template
 * CGAL::Polygon_mesh_processing::keep_largest_connected_components()
 */
template <typename NamedParameter, typename... Args>
struct Keep_largest_connected_components_wrapper {
  static auto call(NamedParameter& np, Args&&... args)
  {
    return PMP::keep_largest_connected_components(std::forward<Args>(args)...,
                                                  np);
  }
};

//!
template <typename PolygonMesh>
auto connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor seed_face, PolygonMesh& pm,
                         const py::dict& np) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  std::vector<Fd> lst;
  auto it = std::back_inserter(lst);

  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());

  PMP::connected_component(seed_face, pm, it);

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

  auto default_np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_edge_is_constrained_map<Pm> eicm_op;
  cgalpy::Named_parameter_wrapper<Connected_components_wrapper, Pm&, Fcm&>
    wrapper(pm, fccmap);
  return cgalpy::named_parameter_applicator(wrapper, default_np, np, eicm_op);
}

//!
template <typename PolygonMesh, typename FaceComponentMap>
auto keep_connected_components_map(PolygonMesh& pm, std::vector<std::size_t> components_to_keep,
                                   FaceComponentMap& fccmap, const py::dict& np = py::dict()) {
  if (np.contains("vertex_index_map")) {
    auto vim = get_vertex_prop_map<PolygonMesh, std::size_t>(pm, "INTERNAL_MAP0",
                                                             np.contains("vertex_index_map") ?
                                                             np["vertex_internal_map"] : py::none());
    PMP::keep_connected_components(pm, components_to_keep, fccmap);
  }
  else {
    PMP::keep_connected_components(pm, components_to_keep, fccmap);
  }
}

//!
template <typename PolygonMesh>
auto keep_connected_components(PolygonMesh& pm, const std::vector<
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
    PMP::keep_connected_components(pm, components_to_keep);
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    PMP::keep_connected_components(pm, components_to_keep);
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::keep_connected_components(pm, components_to_keep);
  }
  else {
    PMP::keep_connected_components(pm, components_to_keep);
  }
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
}

//!
template <typename PolygonMesh, typename ThresholdValueType>
auto keep_large_connected_components(PolygonMesh& pmesh, const ThresholdValueType threshold_value,
                                     const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  auto fsm = get_face_prop_map<Pm, ThresholdValueType>(pmesh, "INTERNAL_MAP1",
    np.contains("face_size_map") ? np["face_size_map"] : py::none(), 1);
  bool vimap = np.contains("vertex_index_map");
  bool fimap = np.contains("face_index_map");
  auto call_pmp = [&]() {
    auto default_np = CGAL::parameters::default_values();
    cgalpy::Named_parameter_dry_run dry_run_op;
    cgalpy::Named_parameter_wrapper<Keep_large_connected_components_wrapper,
                                    Pm&, const ThresholdValueType&>
      wrapper(pmesh, threshold_value);
    return cgalpy::named_parameter_applicator(wrapper, default_np, np,
                                             dry_run_op);
  };
  std::size_t retv;

  if (vimap && fimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP3",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = call_pmp();
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    retv = call_pmp();
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = call_pmp();
  }
  else {
    retv = call_pmp();
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (! np.contains("face_size_map")) pmesh.remove_property_map(fsm);
#endif

  return retv;
}

//!
template <typename PolygonMesh>
auto keep_largest_connected_components(PolygonMesh& pmesh, std::size_t nb_components_to_keep,
                                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_internal_map"] : py::none());
  auto fsm = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
                                                np.contains("face_size_map") ? np["face_size_map"] : py::none(), 1);
  bool vimap = np.contains("vertex_index_map");
  bool fimap = np.contains("face_index_map");
  auto call_pmp = [&]() {
    auto default_np = CGAL::parameters::default_values();
    cgalpy::Named_parameter_dry_run dry_run_op;
    cgalpy::Named_parameter_wrapper<Keep_largest_connected_components_wrapper,
                                    Pm&, const std::size_t&>
      wrapper(pmesh, nb_components_to_keep);
    return cgalpy::named_parameter_applicator(wrapper, default_np, np,
                                             dry_run_op);
  };
  std::size_t retv;

  if (vimap && fimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
                                                    np.contains("vertex_index_map") ?
                                                    np["vertex_internal_map"] : py::none());
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP3",
                                                  np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = call_pmp();
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
                                                    np.contains("vertex_index_map") ?
                                                    np["vertex_internal_map"] : py::none());
    retv = call_pmp();
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
                                                  np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    retv = call_pmp();
  }
  else {
    retv = call_pmp();
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (! np.contains("face_size_map")) pmesh.remove_property_map(fsm);
#endif

  return retv;
}

//!
template <typename PolygonMesh, typename FaceComponentMap>
auto remove_connected_components_map(PolygonMesh& pm, const std::vector<std::size_t>& components_to_remove,
                                     const FaceComponentMap& fccmap, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using faces_size_type = typename Gt::faces_size_type;

  if (np.contains("vertex_index_map")) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP0",
                                                    np.contains("vertex_index_map") ?
                                                    np["vertex_internal_map"] : py::none());
    return PMP::remove_connected_components(pm, components_to_remove, fccmap);
  }
  else {
    return PMP::remove_connected_components(pm, components_to_remove, fccmap);
  }
}

//!
template <typename PolygonMesh>
auto remove_connected_components(PolygonMesh& pm,
                                 const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& components_to_remove,
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
    PMP::remove_connected_components(pm, components_to_remove);
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    PMP::remove_connected_components(pm, components_to_remove);
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::remove_connected_components(pm, components_to_remove);
  }
  else {
    PMP::remove_connected_components(pm, components_to_remove);
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
}

//!
template <typename PolygonMesh>
auto split_connected_components(PolygonMesh& pmesh, std::vector<PolygonMesh>& cc_meshes,
                                const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;

  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
    np.contains("face_patch_map") ? np["face_internal_map"] : py::none());
  // TODO: add index maps
  PMP::split_connected_components(pmesh, cc_meshes);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (! np.contains("face_patch_map")) pmesh.remove_property_map(fpm);
#endif

  return cc_meshes;
}

}
} // namespace cgalpy // namespace pmp

//!
void export_pmp_connected_components(py::module_& m) {
  using Pm = cgalpy::pmp::Polygonal_mesh;
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
  // m.def("connected_component", &cgalpy::pmp::connected_component<Pm>,
  //       py::arg("seed_face"), py::arg("pm"),
  //       py::arg("np") = py::dict());
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  m.def("connected_components",
        &cgalpy::pmp::connected_components_map<Pm, boost::property_map<Pm, CGAL::dynamic_face_property_t<std::size_t>>::type>,
        py::arg("pmesh"), py::arg("fcm"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_connected_components);
  // m.def("connected_components",
  //       &cgalpy::pmp::connected_components_map<Pm, boost::property_map<Pm, CGAL::dynamic_face_property_t<std::uint32_t>>::type>,
  //       py::arg("pm"), py::arg("fcm"), py::arg("np") = py::dict());
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("connected_components", &cgalpy::pmp::connected_components_map<Pm, Pm::Property_map<Fd, std::size_t>>,
        py::arg("pmesh"), py::arg("fcm"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_connected_components);
  m.def("keep_connected_components", &cgalpy::pmp::keep_connected_components_map<Pm, Face_component_map>,
        py::arg("pmesh"), py::arg("components_to_keep"), py::arg("fcm"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_keep_connected_components);
#endif
  m.def("keep_connected_components", &cgalpy::pmp::keep_connected_components<Pm>,
        py::arg("pmesh"), py::arg("components_to_keep"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_keep_connected_components_1);
  m.def("keep_large_connected_components", &cgalpy::pmp::keep_large_connected_components<Pm, std::size_t>,
        py::arg("pmesh"), py::arg("threshold_value"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_keep_large_connected_components);
  m.def("keep_large_connected_components", &cgalpy::pmp::keep_large_connected_components<Pm, double>,
        py::arg("pmesh"), py::arg("threshold_value"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_keep_large_connected_components);
  m.def("keep_largest_connected_components", &cgalpy::pmp::keep_largest_connected_components<Pm>,
        py::arg("pmesh"), py::arg("nb_components_to_keep"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_keep_largest_connected_components);

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("remove_connected_components", &cgalpy::pmp::remove_connected_components_map<Pm, Face_component_map>,
        py::arg("pmesh"), py::arg("components_to_remove"), py::arg("fcm"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_remove_connected_components);
#endif

  m.def("remove_connected_components", &cgalpy::pmp::remove_connected_components<Pm>,
        py::arg("pmesh"), py::arg("components_to_remove"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_remove_connected_components_1);
  m.def("split_connected_components", &cgalpy::pmp::split_connected_components<Pm>,
        py::arg("pmesh"), py::arg("cc_meshes"), py::arg("np") = py::dict(),
        pmp_doc::Polygon_mesh_processing_split_connected_components);
}
