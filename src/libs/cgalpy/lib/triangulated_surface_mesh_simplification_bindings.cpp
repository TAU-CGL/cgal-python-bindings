#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_policies.h>
#include <nanobind/nanobind.h>

#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_stop_predicate.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_profile.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Face_count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Face_count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_plane_policies.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_probabilistic_plane_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_probabilistic_triangle_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_triangle_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Polyhedral_envelope_filter.h>
// #include <CGAL/Surface_mesh_simplification/Edge_collapse_visitor_base.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_filter.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>


#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/pmp_np_parser.hpp"

namespace py = nanobind;

namespace SMS = CGAL::Surface_mesh_simplification;

namespace sms {

template <typename TriangleMesh, typename StopPolicy, typename PlacementType, typename CostType> // this assumes cost
std::optional<int> edge_collapse_placement_cost(TriangleMesh& pm, StopPolicy stop_policy,
                   const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
  using Tm = TriangleMesh;
  // auto him = get_halfedge_prop_map<Tm, std::size_t>(pm, "halfedge_index_map",
  //   np.contains("halfedge_index_map") ? np["halfedge_index_map"] : py::none());
  auto eicm = get_edge_prop_map<Tm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
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
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                  .get_cost(py::cast<CostType>(np["get_cost"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                  .get_cost(py::cast<CostType>(np["get_cost"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else { //yyn
      retv = SMS::edge_collapse(pm, stop_policy,
                                internal::parse_pmp_np<TriangleMesh>(np)
                                .edge_is_constrained_map(eicm)
                                // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                .vertex_index_map(vimap)
                                .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                .get_cost(py::cast<CostType>(np["get_cost"]))
                                );
    }
  }
  else {
    if (filter) { // nyy
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                  .get_cost(py::cast<CostType>(np["get_cost"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                  .get_cost(py::cast<CostType>(np["get_cost"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else { // nyn
      retv = SMS::edge_collapse(pm, stop_policy,
                                internal::parse_pmp_np<TriangleMesh>(np)
                                .edge_is_constrained_map(eicm)
                                // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                .get_cost(py::cast<CostType>(np["get_cost"]))
                                );
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
  // auto him = get_halfedge_prop_map<Tm, std::size_t>(pm, "halfedge_index_map",
  //   np.contains("halfedge_index_map") ? np["halfedge_index_map"] : py::none());
  auto eicm = get_edge_prop_map<Tm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
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
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else { //yyn
      retv = SMS::edge_collapse(pm, stop_policy,
                                internal::parse_pmp_np<TriangleMesh>(np)
                                .edge_is_constrained_map(eicm)
                                // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                .vertex_index_map(vimap)
                                .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                );
    }
  }
  else {
    if (filter) { // nyy
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else { // nyn
      retv = SMS::edge_collapse(pm, stop_policy,
                                internal::parse_pmp_np<TriangleMesh>(np)
                                .edge_is_constrained_map(eicm)
                                // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                .get_placement(py::cast<PlacementType>(np["get_placement"]))
                                );
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
  // auto him = get_halfedge_prop_map<Tm, std::size_t>(pm, "halfedge_index_map",
  //   np.contains("halfedge_index_map") ? np["halfedge_index_map"] : py::none());
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
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  .get_cost(py::cast<CostType>(np["get_cost"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  .get_cost(py::cast<CostType>(np["get_cost"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else {
      retv = SMS::edge_collapse(pm, stop_policy,
                                internal::parse_pmp_np<TriangleMesh>(np)
                                .edge_is_constrained_map(eicm)
                                // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                .vertex_index_map(vimap)
                                .get_cost(py::cast<CostType>(np["get_cost"]))
                                );
    }
  }
  else {
    if (filter) {
      try {
        auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .get_cost(py::cast<CostType>(np["get_cost"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
      try {
        auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .get_cost(py::cast<CostType>(np["get_cost"]))
                                  .filter(f)
                                  );
      }
      catch (const py::cast_error& e) {
      }
#endif
    }
    else {
      retv = SMS::edge_collapse(pm, stop_policy,
                                internal::parse_pmp_np<TriangleMesh>(np)
                                .edge_is_constrained_map(eicm)
                                // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                .get_cost(py::cast<CostType>(np["get_cost"]))
                                );
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

  using Elc = SMS::Edge_length_cost<Tm>;
  using Ltc = SMS::LindstromTurk_cost<Tm>;
  using Ghcost = typename SMS::GarlandHeckbert_policies<Tm, Kernel>::Get_cost;


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
    throw std::runtime_error("Invalid placement and cost type");
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
          retv = SMS::edge_collapse(pm, stop_policy,
                                    internal::parse_pmp_np<TriangleMesh>(np)
                                    .edge_is_constrained_map(eicm)
                                    // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                    .vertex_index_map(vimap)
                                    .filter(f)
                                    );
        }
        catch (const py::cast_error& e) {
        }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
        try {
          auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy,
                                    internal::parse_pmp_np<TriangleMesh>(np)
                                    .edge_is_constrained_map(eicm)
                                    // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                    .vertex_index_map(vimap)
                                    .filter(f)
                                    );
        }
        catch (const py::cast_error& e) {
        }
#endif
      }
      else {
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  );
      }
    }
    else {
      if (filter) {
        try {
          auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy,
                                    internal::parse_pmp_np<TriangleMesh>(np)
                                    .edge_is_constrained_map(eicm)
                                    // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                    .filter(f)
                                    );
        }
        catch (const py::cast_error& e) {
        }
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only works for surface mesh
        try {
          auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy,
                                    internal::parse_pmp_np<TriangleMesh>(np)
                                    .edge_is_constrained_map(eicm)
                                    // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                    .filter(f)
                                    );
        }
        catch (const py::cast_error& e) {
        }
#endif
      }
      else {
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  );
      }
    }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
  if (retv) return retv.value();
  else throw std::runtime_error("Invalid named parameters");
  }
}



} // namespace sms

// Export Polygon_mesh_processing
void export_triangulated_surface_mesh_simplification(py::module_& m) {
  using Tm = pmp::Polygonal_mesh;
  using Fd = boost::graph_traits<Tm>::face_descriptor;
  using Hd = boost::graph_traits<Tm>::halfedge_descriptor;
  using Vd = boost::graph_traits<Tm>::vertex_descriptor;
  using Ed = boost::graph_traits<Tm>::edge_descriptor;
  using faces_size_type = boost::graph_traits<Tm>::faces_size_type;
#if CGALPY_PMP_POLYGONAL_MESH == 1
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
#if CGALPY_PMP_POLYGONAL_MESH == 0
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

  using Ecsp = SMS::Edge_count_stop_predicate<Tm>;
  py::class_<Ecsp>(m, "Edge_count_stop_predicate")
    .def(py::init<edges_size_type>(), py::arg("threshold"))
    ;

  using Ecrsp = SMS::Edge_count_ratio_stop_predicate<Tm>;
  py::class_<Ecrsp>(m, "Edge_count_ratio_stop_predicate")
    .def(py::init<double>(), py::arg("ratio"));
    ;

  using Elsp = SMS::Edge_length_stop_predicate<FT>;
  py::class_<Elsp>(m, "Edge_length_stop_predicate")
    .def(py::init<const FT>(), py::arg("threshold"))
    ;

  using Fcsp = SMS::Face_count_stop_predicate<Tm>;
  py::class_<Fcsp>(m, "Face_count_stop_predicate")
    .def(py::init<edges_size_type>(), py::arg("threshold"))
    ;

  using Fcrsp = SMS::Face_count_ratio_stop_predicate<Tm>;
  py::class_<Fcrsp>(m, "Face_count_ratio_stop_predicate")
    .def(py::init<double, const Tm&>(), py::arg("ratio"), py::arg("tmesh"))
    ;

  using Elc = SMS::Edge_length_cost<Tm>;
  py::class_<Elc>(m, "Edge_length_cost")
    .def(py::init<>())
    ;

  using Ltc = SMS::LindstromTurk_cost<Tm>;
  py::class_<Ltc>(m, "LindstromTurk_cost")
    .def(py::init<>())
    ;

  using Mp = SMS::Midpoint_placement<Tm>;
  py::class_<Mp>(m, "Midpoint_placement")
    .def(py::init<>())
    ;

  using Ltp = SMS::LindstromTurk_placement<Tm>;
  py::class_<Ltp>(m, "LindstromTurk_placement")
    .def(py::init<>())
    ;

  using Bncp = SMS::Bounded_normal_change_placement<Mp>;
  py::class_<Bncp>(m, "Bounded_normal_change_placement")
    .def(py::init<Mp>(), py::arg("get_placement") = Mp())
    ;

  using Ghplacement = SMS::GarlandHeckbert_policies<Tm, Kernel>::Get_placement;
  py::class_<Ghplacement>(m, "GarlandHeckbert_policies_Get_placement")
    .def(py::init<Tm&>(), py::arg("tmesh"))
    ;

  using MpBicm = SMS::Constrained_placement<Mp, edge_bool_map>;
  py::class_<MpBicm>(m, "Constrained_placement_Midpoint_placement_Edge_bool_map")
    .def(py::init<edge_bool_map, Mp>(), py::arg("edge_is_constrained_map") = edge_bool_map(), py::arg("get_placement") = Mp())
    ;
  
  using LtpBicm = SMS::Constrained_placement<Ltp, edge_bool_map>;
  py::class_<LtpBicm>(m, "Constrained_placement_LindstromTurk_placement_Edge_bool_map")
    .def(py::init<edge_bool_map, Ltp>(), py::arg("edge_is_constrained_map") = edge_bool_map(), py::arg("get_placement") = Ltp())
    ;

  using BncpBicm = SMS::Constrained_placement<Bncp, edge_bool_map>;
  py::class_<BncpBicm>(m, "Constrained_placement_Bounded_normal_change_placement_Edge_bool_map")
    .def(py::init<edge_bool_map, Bncp>(), py::arg("edge_is_constrained_map") = edge_bool_map(), py::arg("get_placement") = Bncp())
    ;

  using GhplacementBicm = SMS::Constrained_placement<Ghplacement, edge_bool_map>;
  py::class_<GhplacementBicm>(m, "Constrained_placement_GarlandHeckbert_policies_Edge_bool_map")
    .def(py::init<edge_bool_map, Ghplacement>(), py::arg("edge_is_constrained_map"), py::arg("get_placement"))
    ;

  using Bncf = SMS::Bounded_normal_change_filter<>;
  py::class_<Bncf>(m, "Bounded_normal_change_filter")
    .def(py::init<>())
    ;

  using Pef = SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>;
  py::class_<Pef>(m, "Polyhedral_envelope_filter")
    .def(py::init<FT>(), py::arg("dist"))
    ;


  m.def("edge_collapse", &sms::edge_collapse<Tm, Ecsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
  m.def("edge_collapse", &sms::edge_collapse<Tm, Ecrsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
  m.def("edge_collapse", &sms::edge_collapse<Tm, Elsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
  m.def("edge_collapse", &sms::edge_collapse<Tm, Fcsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
  m.def("edge_collapse", &sms::edge_collapse<Tm, Fcrsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
}

