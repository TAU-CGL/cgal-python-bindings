#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>

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
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>
#if CGAL_VERSION_NR < 1060300900
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_placement.h>
#endif

#include "cgalpy/Edge_collapse_visitor_base.hpp"
#include "cgalpy/cartesian_product.hpp"
#include "cgalpy/helpers.hpp"
#include "cgalpy/pmp_helpers.hpp"
#include "cgalpy/polygon_mesh_processing_types.hpp"
#include "cgalpy/sm/surface_mesh_simplification_types.hpp"
#include "cgalpy/Smsi_docstrings.hpp"

namespace py = nanobind;
namespace smsi_doc = cgalpy::smsi::docstrings;

namespace SMS = CGAL::Surface_mesh_simplification;

namespace cgalpy {
namespace sms {

struct No_policy {};

template <typename Policy>
inline constexpr bool is_no_policy_v = std::is_same_v<Policy, No_policy>;

inline bool has_get_placement(const py::dict& np) {
  return np.contains("get_placement") || np.contains("placement");
}

template <typename Placement>
Placement get_placement_from_named_parameters(const py::dict& np) {
  if constexpr (is_no_policy_v<Placement>) {
    return {};
  }
  else {
    if (np.contains("get_placement"))
      return py::cast<Placement>(np["get_placement"]);
    if (np.contains("placement"))
      return py::cast<Placement>(np["placement"]);
    throw std::runtime_error(
      "Named parameter 'get_placement' must be a placement policy.");
  }
}

template <typename Cost>
Cost get_cost_from_named_parameters(const py::dict& np) {
  if constexpr (is_no_policy_v<Cost>) return {};
  else return py::cast<Cost>(np["get_cost"]);
}

template <typename Filter>
Filter get_filter_from_named_parameters(const py::dict& np) {
  if constexpr (is_no_policy_v<Filter>) return {};
  else return py::cast<Filter>(np["filter"]);
}

template <bool UseEdgeIsConstrainedMap, typename NamedParameters,
          typename EdgeIsConstrainedMap>
auto apply_edge_is_constrained_map(NamedParameters np,
                                   const EdgeIsConstrainedMap& eicm) {
  if constexpr (UseEdgeIsConstrainedMap)
    return np.edge_is_constrained_map(eicm);
  else
    return np;
}

template <typename NamedParameters, typename Cost>
auto apply_cost(NamedParameters np, const Cost& cost) {
  if constexpr (is_no_policy_v<Cost>) return np;
  else return np.get_cost(cost);
}

template <typename NamedParameters, typename Filter>
auto apply_filter(NamedParameters np, const Filter& filter) {
  if constexpr (is_no_policy_v<Filter>) return np;
  else return np.filter(filter);
}

template <typename NamedParameters, typename Placement>
auto apply_placement(NamedParameters np, const Placement& placement) {
  if constexpr (is_no_policy_v<Placement>) return np;
  else return np.get_placement(placement);
}

template <bool UseVisitor, typename NamedParameters, typename Visitor>
auto apply_visitor(NamedParameters np, const Visitor& visitor) {
  if constexpr (UseVisitor) return np.visitor(visitor);
  else return np;
}

template <bool UseEdgeIsConstrainedMap, bool UseVisitor,
          typename TriangleMesh, typename StopPolicy, typename Placement,
          typename Cost, typename Filter, typename EdgeIsConstrainedMap,
          typename Visitor>
int invoke_edge_collapse(TriangleMesh& pm, StopPolicy stop_policy,
                         const Placement& placement, const Cost& cost,
                         const Filter& filter,
                         const EdgeIsConstrainedMap& eicm,
                         const Visitor& visitor) {
  auto np0 = CGAL::parameters::default_values();
  auto np1 = apply_edge_is_constrained_map<UseEdgeIsConstrainedMap>(np0, eicm);
  auto np2 = apply_cost(np1, cost);
  auto np3 = apply_filter(np2, filter);
  auto np4 = apply_placement(np3, placement);
  auto np5 = apply_visitor<UseVisitor>(np4, visitor);
  return SMS::edge_collapse(pm, stop_policy, np5);
}

template <typename TriangleMesh, typename StopPolicy, typename Placement,
          typename Cost, typename Filter>
std::optional<int> try_edge_collapse(TriangleMesh& pm, StopPolicy stop_policy,
                                     const py::dict& np) {
  using Tm = TriangleMesh;
  using V = sms::My_ec_visitor<Tm>;
  using Ed = typename boost::graph_traits<Tm>::edge_descriptor;

#if CGALPY_PMP_POLYGONAL_MESH == 1
  using edge_bool_map = typename Tm::template Property_map<Ed, bool>;
#else
  using edge_bool_map =
    typename boost::property_map<Tm,
                                 CGAL::dynamic_edge_property_t<bool>>::type;
#endif

  try {
    auto placement = get_placement_from_named_parameters<Placement>(np);
    auto cost = get_cost_from_named_parameters<Cost>(np);
    auto filter = get_filter_from_named_parameters<Filter>(np);
    V visitor = np.contains("visitor") ? py::cast<V>(np["visitor"]) : V();

    const bool has_eicm = np.contains("edge_is_constrained_map");
    const bool has_visitor = np.contains("visitor");
    auto eicm = get_edge_prop_map<Tm, bool>(
      pm, "INTERNAL_MAP0",
      has_eicm ? np["edge_is_constrained_map"] : py::none());

    int result;
    if (has_eicm && has_visitor)
      result = invoke_edge_collapse<true, true>(
        pm, stop_policy, placement, cost, filter, eicm, visitor);
    else if (has_eicm)
      result = invoke_edge_collapse<true, false>(
        pm, stop_policy, placement, cost, filter, eicm, visitor);
    else if (has_visitor)
      result = invoke_edge_collapse<false, true>(
        pm, stop_policy, placement, cost, filter, eicm, visitor);
    else
      result = invoke_edge_collapse<false, false>(
        pm, stop_policy, placement, cost, filter, eicm, visitor);

#if CGALPY_PMP_POLYGONAL_MESH == 1
    if (! has_eicm) pm.remove_property_map(eicm);
#endif
    return result;
  }
  catch (const py::cast_error&) {
    return std::nullopt;
  }
}

template <typename TriangleMesh, typename StopPolicy>
struct Edge_collapse_context {
  using Triangle_mesh = TriangleMesh;
  using Stop_policy = StopPolicy;

  TriangleMesh& pm;
  StopPolicy& stop_policy;
  const py::dict& np;
  std::optional<int> result = std::nullopt;
};

template <typename Context, typename Placement, typename Cost,
          typename Filter>
struct Edge_collapse_product_wrapper {
  void operator()(Context& context) {
    if (context.result) return;
    context.result = try_edge_collapse<typename Context::Triangle_mesh,
                                       typename Context::Stop_policy,
                                       Placement, Cost, Filter>(
      context.pm, context.stop_policy, context.np);
  }
};

template <typename Context, typename PlacementList, typename CostList>
void dispatch_filter_product(Context& context, PlacementList placements,
                             CostList costs, bool has_filter) {
  if (has_filter) {
    using Bncf = SMS::Bounded_normal_change_filter<>;
#if CGALPY_PMP_POLYGONAL_MESH == 1
    using Pef = SMS::Polyhedral_envelope_filter<Kernel, Bncf>;
    cgalpy::Type_list<Bncf, Pef> filters;
#else
    cgalpy::Type_list<Bncf> filters;
#endif
    cgalpy::cartesian_product<Edge_collapse_product_wrapper>(
      context, placements, costs, filters);
  }
  else {
    cgalpy::Type_list<No_policy> filters;
    cgalpy::cartesian_product<Edge_collapse_product_wrapper>(
      context, placements, costs, filters);
  }
}

template <typename TriangleMesh, typename StopPolicy>
auto edge_collapse(TriangleMesh& pm, StopPolicy stop_policy,
                   const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Ed = typename boost::graph_traits<Tm>::edge_descriptor;
#if CGALPY_PMP_POLYGONAL_MESH == 1
  using edge_bool_map = typename Tm::template Property_map<Ed, bool>;
#else
  using edge_bool_map =
    typename boost::property_map<Tm,
                                 CGAL::dynamic_edge_property_t<bool>>::type;
#endif

  using Mp = SMS::Midpoint_placement<Tm>;
  using Ltp = SMS::LindstromTurk_placement<Tm>;
  using Ghpp = SMS::GarlandHeckbert_plane_policies<Tm, Kernel>;
  using Ghtp = SMS::GarlandHeckbert_triangle_policies<Tm, Kernel>;
  using Ghplacement =
    typename SMS::GarlandHeckbert_policies<Tm, Kernel>::Get_placement;
  using MpBicm = SMS::Constrained_placement<Mp, edge_bool_map>;
  using LtpBicm = SMS::Constrained_placement<Ltp, edge_bool_map>;
  using GhplacementBicm =
    SMS::Constrained_placement<Ghplacement, edge_bool_map>;

#if CGAL_VERSION_NR < 1060300900
  using Bncp = SMS::Bounded_normal_change_placement<Mp>;
  using BncpBicm = SMS::Constrained_placement<Bncp, edge_bool_map>;
  using BncpGhpp = SMS::Bounded_normal_change_placement<Ghpp>;
  using BncpGhtp = SMS::Bounded_normal_change_placement<Ghtp>;
#endif

  using Elc = SMS::Edge_length_cost<Tm>;
  using Ltc = SMS::LindstromTurk_cost<Tm>;
  using Ghcost = typename SMS::GarlandHeckbert_policies<Tm, Kernel>::Get_cost;

  const bool has_placement = has_get_placement(np);
  const bool has_cost = np.contains("get_cost");
  const bool has_filter = np.contains("filter");

  Edge_collapse_context<Tm, StopPolicy> context{pm, stop_policy, np};

  if (has_placement && ! has_cost) {
    cgalpy::Type_list<MpBicm, LtpBicm, GhplacementBicm,
                      Mp, Ltp, Ghpp, Ghtp> placements;
    cgalpy::Type_list<No_policy> costs;
    dispatch_filter_product(context, placements, costs, has_filter);
#if CGAL_VERSION_NR < 1060300900
    cgalpy::Type_list<Bncp, BncpGhpp, BncpGhtp> deprecated_placements;
    dispatch_filter_product(context, deprecated_placements, costs, has_filter);
#endif
    if (context.result) return context.result.value();
    throw std::runtime_error("Invalid placement type");
  }

  if (has_placement && has_cost) {
    cgalpy::Type_list<MpBicm, LtpBicm, GhplacementBicm, Mp, Ltp>
      placements;
    cgalpy::Type_list<Elc, Ltc, Ghcost> costs;
    dispatch_filter_product(context, placements, costs, has_filter);

    cgalpy::Type_list<Ghpp> ghpp;
    dispatch_filter_product(context, ghpp, ghpp, has_filter);
    cgalpy::Type_list<Ghtp> ghtp;
    dispatch_filter_product(context, ghtp, ghtp, has_filter);

#if CGAL_VERSION_NR < 1060300900
    cgalpy::Type_list<Bncp, BncpBicm, BncpGhpp, BncpGhtp>
      deprecated_placements;
    dispatch_filter_product(context, deprecated_placements, costs,
                            has_filter);
#endif
    if (context.result) return context.result.value();
    throw std::runtime_error("Invalid placement or cost type");
  }

  if (has_cost) {
    cgalpy::Type_list<No_policy> placements;
    cgalpy::Type_list<Elc, Ltc, Ghcost, Ghpp, Ghtp> costs;
    dispatch_filter_product(context, placements, costs, has_filter);
    if (context.result) return context.result.value();
    throw std::runtime_error("Invalid cost type");
  }

  cgalpy::Type_list<No_policy> policies;
  dispatch_filter_product(context, policies, policies, has_filter);
  if (context.result) return context.result.value();
  throw std::runtime_error("Invalid named parameters");
}

template <typename Tm, typename... PolicyTypes>
void define_edge_collapses(py::module_& m) {
  (m.def("edge_collapse", &sms::edge_collapse<Tm, PolicyTypes>,
         py::arg("tmesh"), py::arg("should_stop"),
         py::arg("np") = py::dict(),
         smsi_doc::Surface_mesh_simplification_edge_collapse), ...);
}

} // namespace sms
} // namespace cgalpy

// Export Polygon_mesh_processing
/*!
 */
void export_surface_mesh_simplification(py::module_& m) {
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
  py::class_<Ep>(m, "Edge_profile", smsi_doc::Surface_mesh_simplification_Edge_profile_class)
    .def("v0", &Ep::v0, smsi_doc::Surface_mesh_simplification_Edge_profile_v0)
    .def("v1", &Ep::v1, smsi_doc::Surface_mesh_simplification_Edge_profile_v1)
    .def("v0_v1", &Ep::v0_v1, smsi_doc::Surface_mesh_simplification_Edge_profile_v0_v1)
    .def("v1_v0", &Ep::v1_v0, smsi_doc::Surface_mesh_simplification_Edge_profile_v1_v0)
    .def("p0", &Ep::p0, smsi_doc::Surface_mesh_simplification_Edge_profile_p0)
    .def("p1", &Ep::p1, smsi_doc::Surface_mesh_simplification_Edge_profile_p1)
    .def("vL", &Ep::vL, smsi_doc::Surface_mesh_simplification_Edge_profile_vL)
    .def("v1_vL", &Ep::v1_vL, smsi_doc::Surface_mesh_simplification_Edge_profile_v1_vL)
    .def("vL_v0", &Ep::vL_v0, smsi_doc::Surface_mesh_simplification_Edge_profile_vL_v0)
    .def("vR", &Ep::vR, smsi_doc::Surface_mesh_simplification_Edge_profile_vR)
    .def("v0_vR", &Ep::v0_vR, smsi_doc::Surface_mesh_simplification_Edge_profile_v0_vR)
    .def("vR_v1", &Ep::vR_v1, smsi_doc::Surface_mesh_simplification_Edge_profile_vR_v1)
    .def("link", &Ep::link, smsi_doc::Surface_mesh_simplification_Edge_profile_link)
    .def("border_edges", &Ep::border_edges, smsi_doc::Surface_mesh_simplification_Edge_profile_border_edges)
    .def("left_face_exists", &Ep::left_face_exists, smsi_doc::Surface_mesh_simplification_Edge_profile_left_face_exists)
    .def("right_face_exists", &Ep::right_face_exists, smsi_doc::Surface_mesh_simplification_Edge_profile_right_face_exists)
    .def("surface_mesh", &Ep::surface_mesh, smsi_doc::Surface_mesh_simplification_Edge_profile_surface_mesh)
    // .def("vertex_point_map", &Ep::vertex_point_map,
    //      smsi_doc::Surface_mesh_simplification_Edge_profile_vertex_point_map)
    // .def("geom_traits", &Ep::geom_traits, // not supported
    //      smsi_doc::Surface_mesh_simplification_Edge_profile_geom_traits)
    ;

  using Ecvb = cgalpy::sms::My_ec_visitor<Tm>;
  py::class_<Ecvb>(m, "Edge_collapse_visitor_base",
                   smsi_doc::Surface_mesh_simplification_Edge_collapse_visitor_base_class)
    .def(py::init<>(), "Construct an edge collapse visitor.")
    ;

  m.def("set_OnStarted", &Ecvb::set_started,
        py::arg("visitor"), py::arg("OnStarted"),
        smsi_doc::EdgeCollapseSimplificationVisitor_OnStarted);
  m.def("set_OnFinished", &Ecvb::set_finished,
        py::arg("visitor"), py::arg("OnFinished"),
        smsi_doc::EdgeCollapseSimplificationVisitor_OnFinished);
  m.def("set_OnStopConditionReached", &Ecvb::set_stop_condition_reached,
        py::arg("visitor"), py::arg("OnStopConditionReached"),
        smsi_doc::EdgeCollapseSimplificationVisitor_OnStopConditionReached);
  m.def("set_OnCollected", &Ecvb::set_collected,
        py::arg("visitor"), py::arg("OnCollected"),
        py::sig("def set_OnCollected(visitor: Edge_collapse_visitor_base, OnCollected: Callable[[Edge_profile, float | None], None])"),
        smsi_doc::EdgeCollapseSimplificationVisitor_OnCollected);
  m.def("set_OnSelected", &Ecvb::set_selected,
        py::arg("visitor"), py::arg("OnSelected"),
        py::sig("def set_OnSelected(visitor: Edge_collapse_visitor_base, OnSelected: Callable[[Edge_profile, float | None, int, int], None])"),
        smsi_doc::EdgeCollapseSimplificationVisitor_OnSelected);
  m.def("set_OnCollapsing", &Ecvb::set_collapsing,
        py::arg("visitor"), py::arg("OnCollapsing"),
        py::sig("def set_OnCollapsing(visitor: Edge_collapse_visitor_base, OnCollapsing: Callable[[Edge_profile, CGALPY.Kernel.Point_3 | None], None])"),
        smsi_doc::EdgeCollapseSimplificationVisitor_OnCollapsing);
  m.def("set_OnCollapsed", &Ecvb::set_collapsed,
        py::arg("visitor"), py::arg("OnCollapsed"),
        smsi_doc::EdgeCollapseSimplificationVisitor_OnCollapsed);
  m.def("set_OnNonCollapsable", &Ecvb::set_non_collapsable,
        py::arg("visitor"), py::arg("OnNonCollapsable"),
        smsi_doc::EdgeCollapseSimplificationVisitor_OnNonCollapsable);

  // Predicates //

  using Ecsp = SMS::Edge_count_stop_predicate<Tm>;
  py::class_<Ecsp>(m, "Edge_count_stop_predicate",
                   smsi_doc::Surface_mesh_simplification_Edge_count_stop_predicate_class)
    .def(py::init<edges_size_type>(), py::arg("threshold"),
         smsi_doc::Surface_mesh_simplification_Edge_count_stop_predicate_Edge_count_stop_predicate)
    .def("__call__",
         [](Ecsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         smsi_doc::Surface_mesh_simplification_Edge_count_stop_predicate_operator_call)
    ;

  using Ecrsp = SMS::Edge_count_ratio_stop_predicate<Tm>;
  py::class_<Ecrsp>(m, "Edge_count_ratio_stop_predicate",
                    smsi_doc::Surface_mesh_simplification_Edge_count_ratio_stop_predicate_class)
    .def(py::init<double>(), py::arg("ratio"),
         smsi_doc::Surface_mesh_simplification_Edge_count_ratio_stop_predicate_Edge_count_ratio_stop_predicate)
    .def("__call__",
         [](Ecrsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         smsi_doc::Surface_mesh_simplification_Edge_count_ratio_stop_predicate_operator_call)
    ;

  using Elsp = SMS::Edge_length_stop_predicate<FT>;
  py::class_<Elsp>(m, "Edge_length_stop_predicate",
                   smsi_doc::Surface_mesh_simplification_Edge_length_stop_predicate_class)
    .def(py::init<const FT>(), py::arg("threshold"),
         smsi_doc::Surface_mesh_simplification_Edge_length_stop_predicate_Edge_length_stop_predicate)
    .def("__call__",
         [](Elsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         smsi_doc::Surface_mesh_simplification_Edge_length_stop_predicate_operator_call)
    ;

  using Fcsp = SMS::Face_count_stop_predicate<Tm>;
  py::class_<Fcsp>(m, "Face_count_stop_predicate",
                   smsi_doc::Surface_mesh_simplification_Face_count_stop_predicate_class)
    .def(py::init<edges_size_type>(), py::arg("threshold"),
         smsi_doc::Surface_mesh_simplification_Face_count_stop_predicate_Face_count_stop_predicate)
    .def("__call__",
         [](Fcsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         smsi_doc::Surface_mesh_simplification_Face_count_stop_predicate_operator_call)
    ;

  using Fcrsp = SMS::Face_count_ratio_stop_predicate<Tm>;
  py::class_<Fcrsp>(m, "Face_count_ratio_stop_predicate",
                    smsi_doc::Surface_mesh_simplification_Face_count_ratio_stop_predicate_class)
    .def(py::init<double, const Tm&>(), py::arg("ratio"), py::arg("tmesh"),
         smsi_doc::Surface_mesh_simplification_Face_count_ratio_stop_predicate_Face_count_ratio_stop_predicate)
    .def("__call__",
         [](Fcrsp& self, const Ep& ep, edges_size_type iec, edges_size_type cec)
         { return self(0, ep, iec, cec); },
         py::arg("edge_profile"), py::arg("initial_edge_count"),
         py::arg("current_edge_count"),
         smsi_doc::Surface_mesh_simplification_Face_count_ratio_stop_predicate_operator_call)
    ;

  // Policies //

  using Ghpp = SMS::GarlandHeckbert_plane_policies<Tm, Kernel>;
  py::class_<Ghpp>(m, "GarlandHeckbert_plane_policies",
                   smsi_doc::Surface_mesh_simplification_GarlandHeckbert_plane_policies_class)
    .def(py::init<Tm&>(), py::arg("tmesh"),
         smsi_doc::Surface_mesh_simplification_GarlandHeckbert_plane_policies_GarlandHeckbert_plane_policies)
    .def("get_placement", &Ghpp::get_placement,
         smsi_doc::Surface_mesh_simplification_GarlandHeckbert_plane_policies_get_placement)
    .def("get_cost", &Ghpp::get_cost,
         smsi_doc::Surface_mesh_simplification_GarlandHeckbert_plane_policies_get_cost)
    ;

  // using Ghppp = SMS::GarlandHeckbert_probabilistic_plane_policies<Tm, Kernel>;
  // py::class_<Ghppp>(m, "GarlandHeckbert_probabilistic_plane_policies")
  //   .def(py::init<Tm&>(), py::arg("tmesh"))
  //   ;

  using Ghtp = SMS::GarlandHeckbert_triangle_policies<Tm, Kernel>;
  py::class_<Ghtp>(m, "GarlandHeckbert_triangle_policies",
                   smsi_doc::Surface_mesh_simplification_GarlandHeckbert_triangle_policies_class)
    .def(py::init<Tm&>(), py::arg("tmesh"),
         smsi_doc::Surface_mesh_simplification_GarlandHeckbert_triangle_policies_GarlandHeckbert_triangle_policies)
    .def("get_placement", &Ghtp::get_placement,
         smsi_doc::Surface_mesh_simplification_GarlandHeckbert_triangle_policies_get_placement)
    .def("get_cost", &Ghtp::get_cost,
         smsi_doc::Surface_mesh_simplification_GarlandHeckbert_triangle_policies_get_cost)
    ;

  // using Ghtpp = SMS::GarlandHeckbert_probabilistic_triangle_policies<Tm, Kernel>;
  // py::class_<Ghtpp>(m, "GarlandHeckbert_probabilistic_triangle_policies")
  //  .def(py::init<Tm&>(), py::arg("tmesh"))


  // Costs //

  using Elc = SMS::Edge_length_cost<Tm>;
  py::class_<Elc>(m, "Edge_length_cost",
                  smsi_doc::Surface_mesh_simplification_Edge_length_cost_class)
    .def(py::init<>(),
         smsi_doc::Surface_mesh_simplification_Edge_length_cost_Edge_length_cost)
    ;

  using Ltc = SMS::LindstromTurk_cost<Tm>;
  py::class_<Ltc>(m, "LindstromTurk_cost",
                  smsi_doc::Surface_mesh_simplification_LindstromTurk_cost_class)
    .def(py::init<>(),
         smsi_doc::Surface_mesh_simplification_LindstromTurk_cost_LindstromTurk_cost)
    ;

  // Placements //

  using Mp = SMS::Midpoint_placement<Tm>;
  py::class_<Mp>(m, "Midpoint_placement",
                 smsi_doc::Surface_mesh_simplification_Midpoint_placement_class)
    .def(py::init<>(),
         smsi_doc::Surface_mesh_simplification_Midpoint_placement_Midpoint_placement)
    ;

  using Ltp = SMS::LindstromTurk_placement<Tm>;
  py::class_<Ltp>(m, "LindstromTurk_placement",
                  smsi_doc::Surface_mesh_simplification_LindstromTurk_placement_class)
    .def(py::init<>(),
         smsi_doc::Surface_mesh_simplification_LindstromTurk_placement_LindstromTurk_placement)
    ;

#if CGAL_VERSION_NR < 1060300900
  using Bncp = SMS::Bounded_normal_change_placement<Mp>;
  py::class_<Bncp>(m, "Bounded_normal_change_placement_Midpoint_placement",
                   smsi_doc::Surface_mesh_simplification_Bounded_normal_change_placement_class)
    .def(py::init<Mp>(), py::arg("get_placement"),
         smsi_doc::Surface_mesh_simplification_Bounded_normal_change_placement_Bounded_normal_change_placement_1)
    ;
#endif

  // placements for all
  struct Dummy_placement {};
  // upon calling this classs constructor it gives the correct placement overload
  py::class_<Dummy_placement>(m, "Bounded_normal_change_placement",
                              smsi_doc::Surface_mesh_simplification_Bounded_normal_change_placement_class)
    .def(py::init<>(),
         smsi_doc::Surface_mesh_simplification_Bounded_normal_change_placement_Bounded_normal_change_placement)
    .def("__call__",
         [](Dummy_placement& self, Mp& bncp) { return Mp(bncp); },
         py::arg("get_placement"),
         "Create a bounded-normal-change placement from the given placement policy.")
  ;


  // Constrained_placement //

  using MpBicm = SMS::Constrained_placement<Mp, edge_bool_map>;
  py::class_<MpBicm>(m, "Constrained_placement_Midpoint_placement_Edge_bool_map",
                     smsi_doc::Surface_mesh_simplification_Constrained_placement_class)
    .def(py::init<edge_bool_map, Mp>(),
         py::arg("edge_is_constrained_map") = edge_bool_map(),
         py::arg("get_placement") = Mp(),
         smsi_doc::Surface_mesh_simplification_Constrained_placement_Constrained_placement)
    ;

  using LtpBicm = SMS::Constrained_placement<Ltp, edge_bool_map>;
  py::class_<LtpBicm>(m, "Constrained_placement_LindstromTurk_placement_Edge_bool_map",
                      smsi_doc::Surface_mesh_simplification_Constrained_placement_class)
    .def(py::init<edge_bool_map, Ltp>(),
         py::arg("edge_is_constrained_map") = edge_bool_map(),
         py::arg("get_placement") = Ltp(),
         smsi_doc::Surface_mesh_simplification_Constrained_placement_Constrained_placement)
    ;

#if CGAL_VERSION_NR < 1060300900
  using BncpBicm = SMS::Constrained_placement<Bncp, edge_bool_map>;
  py::class_<BncpBicm>(m, "Constrained_placement_Bounded_normal_change_placement_Edge_bool_map",
                       smsi_doc::Surface_mesh_simplification_Constrained_placement_class)
    .def(py::init<edge_bool_map, Bncp>(),
         py::arg("edge_is_constrained_map") = edge_bool_map(),
         py::arg("get_placement") = Bncp(),
         smsi_doc::Surface_mesh_simplification_Constrained_placement_Constrained_placement)
    ;
#endif

  using Ghplacement = Ghpp::Get_placement;
  using GhplacementBicm = SMS::Constrained_placement<Ghplacement, edge_bool_map>;
  py::class_<GhplacementBicm>(m, "Constrained_placement_GarlandHeckbert_policies_Edge_bool_map",
                              smsi_doc::Surface_mesh_simplification_Constrained_placement_class)
    .def(py::init<edge_bool_map, Ghplacement>(),
         py::arg("edge_is_constrained_map"), py::arg("get_placement"),
         "Construct a constrained placement policy from an edge constraint map and a Garland-Heckbert placement policy.")
    ;

  // Filters //

  using Bncf = SMS::Bounded_normal_change_filter<>;
  py::class_<Bncf>(m, "Bounded_normal_change_filter",
                   smsi_doc::Surface_mesh_simplification_Bounded_normal_change_filter_class)
    .def(py::init<>(),
         smsi_doc::Surface_mesh_simplification_Bounded_normal_change_filter_Bounded_normal_change_filter)
    ;

  using Pef = SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>;
  py::class_<Pef>(m, "Polyhedral_envelope_filter",
                  smsi_doc::Surface_mesh_simplification_Polyhedral_envelope_filter_class)
    .def(py::init<FT>(), py::arg("dist"),
         "Construct a polyhedral envelope filter with the given distance.")
    ;

  if constexpr (! is_exact_ft()) {
    cgalpy::sms::define_edge_collapses<Tm, Ecsp, Ecrsp, Elsp, Fcsp, Fcrsp>(m);
  }
}
