#include <nanobind/nanobind.h>

#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_stop_predicate.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_profile.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Face_count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Face_count_stop_predicate.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_plane_policies.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_policies.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_probabilistic_plane_policies.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_probabilistic_triangle_policies.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_triangle_policies.h>
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
// template<class TM_>
// class Edge_count_ratio_stop_predicate
// {
// public:
//   typedef TM_                                                 TM;
//   typedef typename boost::graph_traits<TM>::edges_size_type   size_type;
//
//   Edge_count_ratio_stop_predicate(const double ratio)
//     : m_ratio(ratio)
//   {
//     CGAL_warning(0. < ratio && ratio <= 1.);
//   }
//
//   template <typename F, typename Profile>
//   bool operator()(const F& /*current_cost*/,
//                   const Profile& /*profile*/,
//                   size_type initial_edge_count,
//                   size_type current_edge_count) const
//   {
//     return (static_cast<double>(current_edge_count) / static_cast<double>(initial_edge_count)) < m_ratio;
//   }
//
// private:
//   double m_ratio;
// };

namespace sms {

template <typename TriangleMesh, typename StopPolicy>
auto edge_collapse(TriangleMesh& pm, StopPolicy stop_policy,
                   const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
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
  using Mp = SMS::Midpoint_placement<Tm>;
  using MpBicm = SMS::Constrained_placement<Mp, edge_bool_map>;
  // auto him = get_halfedge_prop_map<Tm, std::size_t>(pm, "halfedge_index_map",
  //   np.contains("halfedge_index_map") ? np["halfedge_index_map"] : py::none());
  auto eicm = get_edge_prop_map<Tm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
  bool relaxed_order = np.contains("relaxed_order") && py::cast<bool>(np["relaxed_order"]);
  bool vim = np.contains("vertex_index_map");
  bool place = np.contains("placement");
  bool cost = np.contains("cost");
  auto filter = np.contains("filter");
  int retv;
  if (vim) {
    auto vimap = get_vertex_prop_map<Tm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    if (place) {
      MpBicm placement;
      try {
        placement = py::cast<MpBicm>(np["placement"]);
      }
      catch (const py::cast_error& e) {
        throw std::runtime_error("failed to cast placement");
      }
      if (filter) { // yyy
        try {
          auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy,
                                    internal::parse_pmp_np<TriangleMesh>(np)
                                    .edge_is_constrained_map(eicm)
                                    // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                    .vertex_index_map(vimap)
                                    .get_placement(placement)
                                    .get_cost(SMS::LindstromTurk_cost<TriangleMesh>())
                                    .filter(f)
                                    );
        }
        catch (const py::cast_error& e) {
        }
        try {
          auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy,
                                    internal::parse_pmp_np<TriangleMesh>(np)
                                    .edge_is_constrained_map(eicm)
                                    // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                    .vertex_index_map(vimap)
                                    .get_placement(placement)
                                    .get_cost(SMS::LindstromTurk_cost<TriangleMesh>())
                                    .filter(f)
                                    );
        }
        catch (const py::cast_error& e) {
          throw std::runtime_error("failed to cast filter");
        }
      }
      else { //yyn
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  .get_placement(placement)
                                  );
      }
    }
    else {
      // return SMS::edge_collapse(pm, stop_policy,
      //                           internal::parse_pmp_np<TriangleMesh>(np)
      //                           .edge_is_constrained_map(eicm)
      //                           // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
      //                           .vertex_index_map(vimap)
      //                           );
      if (filter) { // yny
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
          throw std::runtime_error("failed to cast filter");
        }
      }
      else { // ynn
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .vertex_index_map(vimap)
                                  );
      }
    }
  }
  else {
    if (place) {
      MpBicm placement;
      try {
        placement = py::cast<MpBicm>(np["placement"]);
      }
      catch (const py::cast_error& e) {
        throw std::runtime_error("failed to cast placement");
      }
      if (filter) { // nyy
        try {
          auto f = py::cast<SMS::Bounded_normal_change_filter<>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy,
                                    internal::parse_pmp_np<TriangleMesh>(np)
                                    .edge_is_constrained_map(eicm)
                                    // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                    .get_placement(placement)
                                    .get_cost(SMS::LindstromTurk_cost<TriangleMesh>())
                                    .filter(f)
                                    );
        }
        catch (const py::cast_error& e) {
        }
        try {
          auto f = py::cast<SMS::Polyhedral_envelope_filter<Kernel, SMS::Bounded_normal_change_filter<>>>(np["filter"]);
          retv = SMS::edge_collapse(pm, stop_policy,
                                    internal::parse_pmp_np<TriangleMesh>(np)
                                    .edge_is_constrained_map(eicm)
                                    // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                    .get_placement(placement)
                                    .get_cost(SMS::LindstromTurk_cost<TriangleMesh>())
                                    .filter(f)
                                    );
        }
        catch (const py::cast_error& e) {
          throw std::runtime_error("failed to cast filter");
        }
      }
      else { // nyn
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  .get_placement(placement)
                                  );
      }
    }
    else {
      if (filter) { // nny
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
          throw std::runtime_error("failed to cast filter");
        }
      }
      else { // nnn
        retv = SMS::edge_collapse(pm, stop_policy,
                                  internal::parse_pmp_np<TriangleMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                                  );
      }
    }
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
  return retv;
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
  // template<class TM_>
  // class Edge_count_stop_predicate
  // {
  // public:
  //   typedef TM_                                                                   TM;
  //   typedef typename boost::graph_traits<TM>::edges_size_type                     size_type;
  //
  //   Edge_count_stop_predicate(const std::size_t edge_count_threshold)
  //     : m_edge_count_threshold(edge_count_threshold)
  //   { }
  //
  //   template <typename F, typename Profile>
  //   bool operator()(const F& /*current_cost*/,
  //                   const Profile& /*profile*/,
  //                   std::size_t /*initial_edge_count*/,
  //                   std::size_t current_edge_count) const
  //   {
  //     return current_edge_count < m_edge_count_threshold;
  //   }
  //
  // private:
  //   std::size_t m_edge_count_threshold;
  // };

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

  using Mp = SMS::Midpoint_placement<Tm>;
  py::class_<Mp>(m, "Midpoint_placement")
    .def(py::init<>())
    ;
  
  using MpBicm = SMS::Constrained_placement<Mp, edge_bool_map>;
  py::class_<MpBicm>(m, "Constrained_placement_Midpoint_placement_Edge_bool_map")
    .def(py::init<edge_bool_map, Mp>(), py::arg("edge_is_constrained_map") = edge_bool_map(), py::arg("get_placement") = Mp())
    ;

  using Ltp = SMS::LindstromTurk_placement<Tm>;
  py::class_<Ltp>(m, "LindstromTurk_placement")
    .def(py::init<>())
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

