#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_profile.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Face_count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Face_count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_plane_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_probabilistic_plane_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_probabilistic_triangle_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_triangle_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Polyhedral_envelope_filter.h>
// #include <CGAL/Surface_mesh_simplification/Edge_collapse_visitor_base.h> // custom
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_filter.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>

#include "CGALPY/Edge_collapse_visitor_base.hpp"
#include "CGALPY/helpers.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "cgalpy/Sms_docstrings.hpp"

namespace py = nanobind;
namespace sms_doc = cgalpy::sms::docstrings;

namespace SMS = CGAL::Surface_mesh_simplification;

namespace cgalpy {
namespace sms {

template <typename TriangleMesh, typename StopPolicy, typename PlacementType, typename CostType> // this assumes cost
std::optional<int> edge_collapse_placement_cost(TriangleMesh& pm, StopPolicy stop_policy,
                                                const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
  using Tm = TriangleMesh;
  using V = sms::My_ec_visitor<Tm>;
  // auto him = get_halfedge_prop_map<Tm, std::size_t>(pm, "halfedge_index_map",
  //   np.contains("halfedge_index_map") ? np["halfedge_index_map"] : py::none());
  V visitor = np.contains("visitor") ? py::cast<V>(np["visitor"]) : V();
  auto eicm = get_edge_prop_map<Tm, bool>(pm, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  bool relaxed_order = np.contains("relaxed_order") && py::cast<bool>(np["relaxed_order"]);
  bool vim = np.contains("vertex_index_map");
  bool place = np.contains("get_placement");
  bool cost = np.contains("get_cost");
  auto filter = np.contains("filter");
  std::optional<int> retv = std::nullopt;
  if (vim) {
    auto vimap = get_vertex_prop_map<Tm, std::size_t>(pm, "INTERNAL_MAP1",
    np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    if (filter) { // yyy
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else { //yyn
      try {
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
    }
  }
  else {
    if (filter) { // nyy
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else { // nyn
      try {
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
    }
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
  return retv;
}

template <typename TriangleMesh, typename StopPolicy, typename PlacementType> // this assumes no cost
std::optional<int> edge_collapse_placement(TriangleMesh& pm, StopPolicy stop_policy,
                   const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
  using Tm = TriangleMesh;
  using V = sms::My_ec_visitor<Tm>;
  // auto him = get_halfedge_prop_map<Tm, std::size_t>(pm, "halfedge_index_map",
  //   np.contains("halfedge_index_map") ? np["halfedge_index_map"] : py::none());
  auto eicm = get_edge_prop_map<Tm, bool>(pm, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  V visitor = np.contains("visitor") ? py::cast<V>(np["visitor"]) : V();
  bool relaxed_order = np.contains("relaxed_order") && py::cast<bool>(np["relaxed_order"]);
  bool vim = np.contains("vertex_index_map");
  bool place = np.contains("get_placement");
  bool cost = np.contains("get_cost");
  auto filter = np.contains("filter");
  std::optional<int> retv = std::nullopt;
  if (vim) {
    auto vimap = get_vertex_prop_map<Tm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    if (filter) { // yyy
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else { //yyn
      try {
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
    }
  }
  else {
    if (filter) { // nyy
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else { // nyn
      try {
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
    }
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
  return retv;
}

template <typename TriangleMesh, typename StopPolicy, typename CostType> // this assumes cost but no placement
std::optional<int> edge_collapse_cost(TriangleMesh& pm, StopPolicy stop_policy,
                   const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
  using Tm = TriangleMesh;
  using V = sms::My_ec_visitor<Tm>;
  // auto him = get_halfedge_prop_map<Tm, std::size_t>(pm, "halfedge_index_map",
  //   np.contains("halfedge_index_map") ? np["halfedge_index_map"] : py::none());
  V visitor = np.contains("visitor") ? py::cast<V>(np["visitor"]) : V();
  auto eicm = get_edge_prop_map<Tm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
  bool relaxed_order = np.contains("relaxed_order") && py::cast<bool>(np["relaxed_order"]);
  bool vim = np.contains("vertex_index_map");
  auto filter = np.contains("filter");
  std::optional<int> retv = std::nullopt;

  if (vim) {
    auto vimap = get_vertex_prop_map<Tm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    if (filter) {
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else {
      try {
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
    }
  }
  else {
    if (filter) {
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else {
      try {
        retv = SMS::edge_collapse(pm, stop_policy);
      }
      catch (const py::cast_error& e) {
      }
    }
  }


#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
  return retv;
}
template <typename TriangleMesh, typename StopPolicy>
auto edge_collapse(TriangleMesh& pm, StopPolicy stop_policy, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = typename boost::graph_traits<Tm>;
  using Fd = typename boost::graph_traits<Tm>::face_descriptor;
  using Hd = typename boost::graph_traits<Tm>::halfedge_descriptor;
  using Vd = typename boost::graph_traits<Tm>::vertex_descriptor;
  using Ed = typename boost::graph_traits<Tm>::edge_descriptor;
  using faces_size_type = typename boost::graph_traits<Tm>::faces_size_type;
#if CGALPY_PMP_POLYGONAL_MESH == 1
  using edge_bool_map = typename Tm::template Property_map<Ed, bool>;
#endif
#if CGALPY_PMP_POLYGONAL_MESH == 0
  using edge_bool_map = typename boost::property_map<Tm, CGAL::dynamic_edge_property_t<bool>>::type;
#endif
  using Pef = SMS::Polyhedral_envelope_filter<Kernel,SMS::Bounded_normal_change_filter<>>;
  using Mp = SMS::Midpoint_placement<Tm>;
  using Ltp = SMS::LindstromTurk_placement<Tm>;
  using Bncp = SMS::Bounded_normal_change_placement<Mp>;
  using Ghplacement = typename SMS::GarlandHeckbert_policies<Tm, Kernel>::Get_placement;
  using MpBicm = SMS::Constrained_placement<Mp, edge_bool_map>;
  using LtpBicm = SMS::Constrained_placement<Ltp, edge_bool_map>;
  using BncpBicm = SMS::Constrained_placement<Bncp, edge_bool_map>;
  using GhplacementBicm = SMS::Constrained_placement<Ghplacement, edge_bool_map>;
  using Ghpp = SMS::GarlandHeckbert_plane_policies<Tm, Kernel>;
  using BncpGhpp = SMS::Bounded_normal_change_placement<Ghpp>;
  using Ghtp = SMS::GarlandHeckbert_triangle_policies<Tm, Kernel>;
  using BncpGhtp = SMS::Bounded_normal_change_placement<Ghtp>;

  using Elc = SMS::Edge_length_cost<Tm>;
  using Ltc = SMS::LindstromTurk_cost<Tm>;
  using Ghcost = typename SMS::GarlandHeckbert_policies<Tm, Kernel>::Get_cost;

  using V = sms::My_ec_visitor<Tm>;


  bool place = np.contains("get_placement");
  bool vim = np.contains("vertex_index_map");
  bool cost = np.contains("get_cost");
  bool filter = np.contains("filter");
  if (place && !cost) {
    std::optional<int> r;
    r = edge_collapse_placement<TriangleMesh, StopPolicy, MpBicm>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement<TriangleMesh, StopPolicy, LtpBicm>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement<TriangleMesh, StopPolicy, MpBicm>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement<TriangleMesh, StopPolicy, GhplacementBicm>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement<TriangleMesh, StopPolicy, Mp>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement<TriangleMesh, StopPolicy, Ltp>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement<TriangleMesh, StopPolicy, Bncp>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement<TriangleMesh, StopPolicy, BncpGhpp>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement<TriangleMesh, StopPolicy, BncpGhtp>(pm, stop_policy, np);
    if (r) return r.value();
    throw std::runtime_error("Invalid placement type");
  }
  else if (place && cost) {
    // every combination of placement and cost
    std::optional<int> r;
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, MpBicm, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, MpBicm, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, MpBicm, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, LtpBicm, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, LtpBicm, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, LtpBicm, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpBicm, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpBicm, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpBicm, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, GhplacementBicm, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, GhplacementBicm, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, GhplacementBicm, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Mp, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Mp, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Mp, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Ltp, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Ltp, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Ltp, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Bncp, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Bncp, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, Bncp, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpGhpp, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpGhpp, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpGhpp, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpGhtp, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpGhtp, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_placement_cost<TriangleMesh, StopPolicy, BncpGhtp, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    throw std::runtime_error("Invalid placement or cost type");
  }
  else if (!place && cost) {
    auto eicm = get_edge_prop_map<Tm, bool>(pm, "INTERNAL_MAP0",
      np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
    std::optional<int> r;
    r = edge_collapse_cost<TriangleMesh, StopPolicy, Elc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_cost<TriangleMesh, StopPolicy, Ltc>(pm, stop_policy, np);
    if (r) return r.value();
    r = edge_collapse_cost<TriangleMesh, StopPolicy, Ghcost>(pm, stop_policy, np);
    if (r) return r.value();
    throw std::runtime_error("Invalid cost type");
  }
  else {
    V visitor = np.contains("visitor") ? py::cast<V>(np["visitor"]) : V();
    auto eicm = get_edge_prop_map<Tm, bool>(pm, "INTERNAL_MAP0",
      np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
    bool relaxed_order = np.contains("relaxed_order") && py::cast<bool>(np["relaxed_order"]);
    bool vim = np.contains("vertex_index_map");
    bool filter = np.contains("filter");
    std::optional<int> retv = std::nullopt;
    if (vim) {
      auto vimap = get_vertex_prop_map<Tm, std::size_t>(pm, "INTERNAL_MAP1",
        np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
      if (filter) {
        try {
          auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy);
        }
        catch (const py::cast_error& e) {
        }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
        try {
          auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy);
        }
        catch (const py::cast_error& e) {
        }
#endif
      }
      else {
        retv = SMS::edge_collapse(pm, stop_policy);
      }
    }
    else {
      if (filter) {
        try {
          auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy);
        }
        catch (const py::cast_error& e) {
        }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
        try {
          auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy);
        }
        catch (const py::cast_error& e) {
        }
#endif
      }
      else {
        retv = SMS::edge_collapse(pm, stop_policy);
      }
    }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
  if (retv) return retv.value();
  else throw std::runtime_error("Invalid named parameters");
  }
}

template <typename Tm, typename... PolicyTypes>
void define_edge_collapses(py::module_& m) {
    (m.def("edge_collapse", &sms::edge_collapse<Tm, PolicyTypes>,
          py::arg("tmesh"), py::arg("should_stop"), py::arg("np") = py::dict(),
          sms_doc::Surface_mesh_simplification_edge_collapse), ...);
}

}
} // namespace cgalpy // namespace sms

// Export Polygon_mesh_processing
void export_triangulated_surface_mesh_simplification(py::module_& m) {
  using Tm = cgalpy::pmp::Polygonal_mesh;
  using Fd = boost::graph_traits<Tm>::face_descriptor;
  using Hd = boost::graph_traits<Tm>::halfedge_descriptor;
  using Vd = boost::graph_traits<Tm>::vertex_descriptor;
  using Ed = boost::graph_traits<Tm>::edge_descriptor;
  using faces_size_type = boost::graph_traits<Tm>::faces_size_type;
#if CGALPY_PMP_POLYGONAL_MESH == 1 // Surface_mesh
  using edge_bool_map = Tm::Property_map<Ed, bool>;
  using RegionMap = Tm::Property_map<Fd, std::size_t>;
  using CornerIdMap = Tm::Property_map<Vd, std::size_t>;
  using FacePatchMap = Tm::Property_map<Fd, std::size_t>;
  using FaceSizeTypeMap = Tm::Property_map<Fd, faces_size_type>;
  using VertexCornerMap = Tm::Property_map<Vd, std::size_t>;
  using EdgeIsConstrainedMap = Tm::Property_map<Ed, bool>;
  using FaceBitMap = Tm::Property_map<Fd, bool>;
  using FaceComponentMap = Tm::Property_map<Fd, faces_size_type>;
  using FaceVectorMap = Tm::Property_map<Fd, Vector_3>;
  using VertexVectorMap = Tm::Property_map<Vd, Vector_3>;
  using ValueMap = Tm::Property_map<Vd, double>;
#endif
#if CGALPY_PMP_POLYGONAL_MESH == 0 // Polyhedron
  using edge_bool_map = boost::property_map<Tm, CGAL::dynamic_edge_property_t<bool>>::type;
  using RegionMap = boost::property_map<Tm, CGAL::dynamic_face_property_t<std::size_t>>;
  using CornerIdMap = boost::property_map<Tm, CGAL::dynamic_vertex_property_t<std::size_t>>;
  using FacePatchMap = boost::property_map<Tm, CGAL::dynamic_face_property_t<std::size_t>>;
  using FaceSizeTypeMap = boost::property_map<Tm, CGAL::dynamic_face_property_t<faces_size_type>>;
  using VertexCornerMap = boost::property_map<Tm, CGAL::dynamic_vertex_property_t<std::size_t>>;
  using EdgeIsConstrainedMap = boost::property_map<Tm, CGAL::dynamic_edge_property_t<bool>>::type;
  using FaceBitMap = boost::property_map<Tm, CGAL::dynamic_face_property_t<bool>>;
  using FaceComponentMap = boost::property_map<Tm, CGAL::dynamic_face_property_t<faces_size_type>>;
  using FaceVectorMap = boost::property_map<Tm, CGAL::dynamic_face_property_t<Vector_3>>;
  using VertexVectorMap = boost::property_map<Tm, CGAL::dynamic_vertex_property_t<Vector_3>>;
  using ValueMap = boost::property_map<Tm, CGAL::dynamic_vertex_property_t<double>>;
#endif

  using edges_size_type = boost::graph_traits<Tm>::edges_size_type;

  using Ep = SMS::Edge_profile<Tm>;
  py::class_<Ep>(
      m, "Edge_profile", sms_doc::Surface_mesh_simplification_Edge_profile_class)
    .def("v0", &Ep::v0, sms_doc::Surface_mesh_simplification_Edge_profile_v0)
    .def("v1", &Ep::v1, sms_doc::Surface_mesh_simplification_Edge_profile_v1)
    .def("v0_v1", &Ep::v0_v1, sms_doc::Surface_mesh_simplification_Edge_profile_v0_v1)
    .def("v1_v0", &Ep::v1_v0, sms_doc::Surface_mesh_simplification_Edge_profile_v1_v0)
    .def("p0", &Ep::p0, sms_doc::Surface_mesh_simplification_Edge_profile_p0)
    .def("p1", &Ep::p1, sms_doc::Surface_mesh_simplification_Edge_profile_p1)
    .def("vL", &Ep::vL, sms_doc::Surface_mesh_simplification_Edge_profile_vL)
    .def("v1_vL", &Ep::v1_vL, sms_doc::Surface_mesh_simplification_Edge_profile_v1_vL)
    .def("vL_v0", &Ep::vL_v0, sms_doc::Surface_mesh_simplification_Edge_profile_vL_v0)
    .def("vR", &Ep::vR, sms_doc::Surface_mesh_simplification_Edge_profile_vR)
    .def("v0_vR", &Ep::v0_vR, sms_doc::Surface_mesh_simplification_Edge_profile_v0_vR)
    .def("vR_v1", &Ep::vR_v1, sms_doc::Surface_mesh_simplification_Edge_profile_vR_v1)
    .def("link", &Ep::link, sms_doc::Surface_mesh_simplification_Edge_profile_link)
    .def("border_edges", &Ep::border_edges, sms_doc::Surface_mesh_simplification_Edge_profile_border_edges)
    .def("left_face_exists", &Ep::left_face_exists, sms_doc::Surface_mesh_simplification_Edge_profile_left_face_exists)
    .def("right_face_exists", &Ep::right_face_exists, sms_doc::Surface_mesh_simplification_Edge_profile_right_face_exists)
    .def("surface_mesh", &Ep::surface_mesh, sms_doc::Surface_mesh_simplification_Edge_profile_surface_mesh)
    // .def("vertex_point_map", &Ep::vertex_point_map,
    //      sms_doc::Surface_mesh_simplification_Edge_profile_vertex_point_map)
    // .def("geom_traits", &Ep::geom_traits, // not supported
    //      sms_doc::Surface_mesh_simplification_Edge_profile_geom_traits)
    ;

  using Ecvb = cgalpy::sms::My_ec_visitor<Tm>;
  py::class_<Ecvb>(
      m, "Edge_collapse_visitor_base",
      sms_doc::Surface_mesh_simplification_Edge_collapse_visitor_base_class)
    .def(py::init<>(), "Construct an edge collapse visitor.")
    ;
  m.def("set_OnStarted", &Ecvb::set_started,
        py::arg("visitor"), py::arg("OnStarted"),
        sms_doc::EdgeCollapseSimplificationVisitor_OnStarted);
  m.def("set_OnFinished", &Ecvb::set_finished,
        py::arg("visitor"), py::arg("OnFinished"),
        sms_doc::EdgeCollapseSimplificationVisitor_OnFinished);
  m.def("set_OnStopConditionReached", &Ecvb::set_stop_condition_reached,
        py::arg("visitor"), py::arg("OnStopConditionReached"),
        sms_doc::EdgeCollapseSimplificationVisitor_OnStopConditionReached);
  m.def("set_OnCollected", &Ecvb::set_collected,
        py::arg("visitor"), py::arg("OnCollected"),
        py::sig("def set_OnCollected(visitor: Edge_collapse_visitor_base, OnCollected: Callable[[Edge_profile, float | None], None])"),
        sms_doc::EdgeCollapseSimplificationVisitor_OnCollected);
  m.def("set_OnSelected", &Ecvb::set_selected,
        py::arg("visitor"), py::arg("OnSelected"),
        py::sig("def set_OnSelected(visitor: Edge_collapse_visitor_base, OnSelected: Callable[[Edge_profile, float | None, int, int], None])"),
        sms_doc::EdgeCollapseSimplificationVisitor_OnSelected);
  m.def("set_OnCollapsing", &Ecvb::set_collapsing,
        py::arg("visitor"), py::arg("OnCollapsing"),
        py::sig("def set_OnCollapsing(visitor: Edge_collapse_visitor_base, OnCollapsing: Callable[[Edge_profile, CGALPY.Kernel.Point_3 | None], None])"),
        sms_doc::EdgeCollapseSimplificationVisitor_OnCollapsing);
  m.def("set_OnCollapsed", &Ecvb::set_collapsed,
        py::arg("visitor"), py::arg("OnCollapsed"),
        sms_doc::EdgeCollapseSimplificationVisitor_OnCollapsed);
  m.def("set_OnNonCollapsable", &Ecvb::set_non_collapsable,
        py::arg("visitor"), py::arg("OnNonCollapsable"),
        sms_doc::EdgeCollapseSimplificationVisitor_OnNonCollapsable);


  // Predicates //

  using Ecsp = SMS::Edge_count_stop_predicate<Tm>;
  py::class_<Ecsp>(
      m, "Edge_count_stop_predicate",
      sms_doc::Surface_mesh_simplification_Edge_count_stop_predicate_class)
    .def(py::init<edges_size_type>(), py::arg("threshold"),
         sms_doc::Surface_mesh_simplification_Edge_count_stop_predicate_Edge_count_stop_predicate)
    .def("__call__",
         [](Ecsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         sms_doc::Surface_mesh_simplification_Edge_count_stop_predicate_operator_call)
    ;

  using Ecrsp = SMS::Edge_count_ratio_stop_predicate<Tm>;
  py::class_<Ecrsp>(
      m, "Edge_count_ratio_stop_predicate",
      sms_doc::Surface_mesh_simplification_Edge_count_ratio_stop_predicate_class)
    .def(py::init<double>(), py::arg("ratio"),
         sms_doc::Surface_mesh_simplification_Edge_count_ratio_stop_predicate_Edge_count_ratio_stop_predicate)
    .def("__call__",
         [](Ecrsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         sms_doc::Surface_mesh_simplification_Edge_count_ratio_stop_predicate_operator_call)
    ;

  using Elsp = SMS::Edge_length_stop_predicate<FT>;
  py::class_<Elsp>(
      m, "Edge_length_stop_predicate",
      sms_doc::Surface_mesh_simplification_Edge_length_stop_predicate_class)
    .def(py::init<const FT>(), py::arg("threshold"),
         sms_doc::Surface_mesh_simplification_Edge_length_stop_predicate_Edge_length_stop_predicate)
    .def("__call__",
         [](Elsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         sms_doc::Surface_mesh_simplification_Edge_length_stop_predicate_operator_call)
    ;

  using Fcsp = SMS::Face_count_stop_predicate<Tm>;
  py::class_<Fcsp>(
      m, "Face_count_stop_predicate",
      sms_doc::Surface_mesh_simplification_Face_count_stop_predicate_class)
    .def(py::init<edges_size_type>(), py::arg("threshold"),
         sms_doc::Surface_mesh_simplification_Face_count_stop_predicate_Face_count_stop_predicate)
    .def("__call__",
         [](Fcsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         sms_doc::Surface_mesh_simplification_Face_count_stop_predicate_operator_call)
    ;

  using Fcrsp = SMS::Face_count_ratio_stop_predicate<Tm>;
  py::class_<Fcrsp>(
      m, "Face_count_ratio_stop_predicate",
      sms_doc::Surface_mesh_simplification_Face_count_ratio_stop_predicate_class)
    .def(py::init<double, const Tm&>(), py::arg("ratio"), py::arg("tmesh"),
         sms_doc::Surface_mesh_simplification_Face_count_ratio_stop_predicate_Face_count_ratio_stop_predicate)
    .def("__call__",
         [](Fcrsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         sms_doc::Surface_mesh_simplification_Face_count_ratio_stop_predicate_operator_call)
    ;

  // Policies //

  using Ghpp = SMS::GarlandHeckbert_plane_policies<Tm, Kernel>;
  py::class_<Ghpp>(
      m, "GarlandHeckbert_plane_policies",
      sms_doc::Surface_mesh_simplification_GarlandHeckbert_plane_policies_class)
    .def(py::init<Tm&>(), py::arg("tmesh"),
         sms_doc::Surface_mesh_simplification_GarlandHeckbert_plane_policies_GarlandHeckbert_plane_policies)
    /*.def("get_placement", &Ghpp::get_placement,
           sms_doc::Surface_mesh_simplification_GarlandHeckbert_plane_policies_get_placement)*/
    /*.def("get_cost", &Ghpp::get_cost,
           sms_doc::Surface_mesh_simplification_GarlandHeckbert_plane_policies_get_cost)*/
    ;

  // using Ghppp = SMS::GarlandHeckbert_probabilistic_plane_policies<Tm, Kernel>;
  // py::class_<Ghppp>(m, "GarlandHeckbert_probabilistic_plane_policies")
  //   .def(py::init<Tm&>(), py::arg("tmesh"))
  //   ;

  using Ghtp = SMS::GarlandHeckbert_triangle_policies<Tm, Kernel>;
  py::class_<Ghtp>(
      m, "GarlandHeckbert_triangle_policies",
      sms_doc::Surface_mesh_simplification_GarlandHeckbert_triangle_policies_class)
    .def(py::init<Tm&>(), py::arg("tmesh"),
         sms_doc::Surface_mesh_simplification_GarlandHeckbert_triangle_policies_GarlandHeckbert_triangle_policies)
    .def("get_placement", &Ghtp::get_placement,
         sms_doc::Surface_mesh_simplification_GarlandHeckbert_triangle_policies_get_placement)
    .def("get_cost", &Ghtp::get_cost,
         sms_doc::Surface_mesh_simplification_GarlandHeckbert_triangle_policies_get_cost)
    ;

  // using Ghtpp = SMS::GarlandHeckbert_probabilistic_triangle_policies<Tm, Kernel>;
  // py::class_<Ghtpp>(m, "GarlandHeckbert_probabilistic_triangle_policies")
  //  .def(py::init<Tm&>(), py::arg("tmesh"))


  // Costs //

  using Elc = SMS::Edge_length_cost<Tm>;
  py::class_<Elc>(
      m, "Edge_length_cost",
      sms_doc::Surface_mesh_simplification_Edge_length_cost_class)
    .def(py::init<>(),
         sms_doc::Surface_mesh_simplification_Edge_length_cost_Edge_length_cost)
    ;

  using Ltc = SMS::LindstromTurk_cost<Tm>;
  py::class_<Ltc>(
      m, "LindstromTurk_cost",
      sms_doc::Surface_mesh_simplification_LindstromTurk_cost_class)
    .def(py::init<>(),
         sms_doc::Surface_mesh_simplification_LindstromTurk_cost_LindstromTurk_cost)
    ;

  // Placements //

  using Mp = SMS::Midpoint_placement<Tm>;
  py::class_<Mp>(
      m, "Midpoint_placement",
      sms_doc::Surface_mesh_simplification_Midpoint_placement_class)
    .def(py::init<>(),
         sms_doc::Surface_mesh_simplification_Midpoint_placement_Midpoint_placement)
    ;

  using Ltp = SMS::LindstromTurk_placement<Tm>;
  py::class_<Ltp>(
      m, "LindstromTurk_placement",
      sms_doc::Surface_mesh_simplification_LindstromTurk_placement_class)
    .def(py::init<>(),
         sms_doc::Surface_mesh_simplification_LindstromTurk_placement_LindstromTurk_placement)
    ;

  using Bncp = SMS::Bounded_normal_change_placement<Mp>;
  py::class_<Bncp>(
      m, "Bounded_normal_change_placement_Midpoint_placement",
      sms_doc::Surface_mesh_simplification_Bounded_normal_change_placement_class)
    .def(py::init<Mp>(), py::arg("get_placement"),
         sms_doc::Surface_mesh_simplification_Bounded_normal_change_placement_Bounded_normal_change_placement_1)
    ;

  // placements for all
  struct Dummy_placement {};
  // upon calling this classs constructor it gives the correct placement overload
  py::class_<Dummy_placement>(
      m, "Bounded_normal_change_placement",
      sms_doc::Surface_mesh_simplification_Bounded_normal_change_placement_class)
    .def(py::init<>(),
         sms_doc::Surface_mesh_simplification_Bounded_normal_change_placement_Bounded_normal_change_placement)
    .def("__call__",
         [](Dummy_placement& self, Mp& bncp) { return Mp(bncp); },
         py::arg("get_placement"),
         "Create a bounded-normal-change placement from the given placement policy.")
  ;


  // Constrained_placement //

  using MpBicm = SMS::Constrained_placement<Mp, edge_bool_map>;
  py::class_<MpBicm>(
      m, "Constrained_placement_Midpoint_placement_Edge_bool_map",
      sms_doc::Surface_mesh_simplification_Constrained_placement_class)
    .def(py::init<edge_bool_map, Mp>(),
         py::arg("edge_is_constrained_map") = edge_bool_map(),
         py::arg("get_placement") = Mp(),
         sms_doc::Surface_mesh_simplification_Constrained_placement_Constrained_placement)
    ;

  using LtpBicm = SMS::Constrained_placement<Ltp, edge_bool_map>;
  py::class_<LtpBicm>(
      m, "Constrained_placement_LindstromTurk_placement_Edge_bool_map",
      sms_doc::Surface_mesh_simplification_Constrained_placement_class)
    .def(py::init<edge_bool_map, Ltp>(),
         py::arg("edge_is_constrained_map") = edge_bool_map(),
         py::arg("get_placement") = Ltp(),
         sms_doc::Surface_mesh_simplification_Constrained_placement_Constrained_placement)
    ;

  using BncpBicm = SMS::Constrained_placement<Bncp, edge_bool_map>;
  py::class_<BncpBicm>(
      m, "Constrained_placement_Bounded_normal_change_placement_Edge_bool_map",
      sms_doc::Surface_mesh_simplification_Constrained_placement_class)
    .def(py::init<edge_bool_map, Bncp>(),
         py::arg("edge_is_constrained_map") = edge_bool_map(),
         py::arg("get_placement") = Bncp(),
         sms_doc::Surface_mesh_simplification_Constrained_placement_Constrained_placement)
    ;

  using Ghplacement = Ghpp::Get_placement;
  using GhplacementBicm = SMS::Constrained_placement<Ghplacement, edge_bool_map>;
  py::class_<GhplacementBicm>(
      m, "Constrained_placement_GarlandHeckbert_policies_Edge_bool_map",
      sms_doc::Surface_mesh_simplification_Constrained_placement_class)
    .def(py::init<edge_bool_map, Ghplacement>(),
         py::arg("edge_is_constrained_map"), py::arg("get_placement"),
         "Construct a constrained placement policy from an edge constraint map and a Garland-Heckbert placement policy.")
    ;

  // Filters //

  using Bncf = SMS::Bounded_normal_change_filter<>;
  py::class_<Bncf>(
      m, "Bounded_normal_change_filter",
      sms_doc::Surface_mesh_simplification_Bounded_normal_change_filter_class)
    .def(py::init<>(),
         sms_doc::Surface_mesh_simplification_Bounded_normal_change_filter_Bounded_normal_change_filter)
    ;

  using Pef = SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>;
  py::class_<Pef>(
      m, "Polyhedral_envelope_filter",
      sms_doc::Surface_mesh_simplification_Polyhedral_envelope_filter_class)
    .def(py::init<FT>(), py::arg("dist"),
         "Construct a polyhedral envelope filter with the given distance.")
    ;

  /*cgalpy::sms::define_edge_collapses<Tm, Ecsp, Ecrsp, Elsp, Fcsp, Fcrsp>(m); deprecated?*/
}
