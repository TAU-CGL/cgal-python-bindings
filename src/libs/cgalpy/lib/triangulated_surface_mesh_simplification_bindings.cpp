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
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_cost.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_placement.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Polyhedral_envelope_filter.h>
// #include <CGAL/Surface_mesh_simplification/Edge_collapse_visitor_base.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_filter.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_placement.h>
// #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>


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
  using Hd = typename Gt::halfedge_descriptor;
  // auto him = get_halfedge_prop_map<Tm, std::size_t>(pm, "halfedge_index_map",
  //   np.contains("halfedge_index_map") ? np["halfedge_index_map"] : py::none());
  auto eicm = get_edge_prop_map<Tm, bool>(pm, "edge_is_constrained_map",
    np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
  bool relaxed_order = np.contains("relaxed_order") && py::cast<bool>(np["relaxed_order"]);
  bool vim = np.contains("vertex_index_map");
  if (vim) {
    return SMS::edge_collapse(pm, stop_policy,
                              internal::parse_pmp_np<TriangleMesh>(np)
                              .edge_is_constrained_map(eicm)
                              // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                              .vertex_index_map(vim)
                              );
  }
  else {
    return SMS::edge_collapse(pm, stop_policy,
                              internal::parse_pmp_np<TriangleMesh>(np)
                              .edge_is_constrained_map(eicm)
                              // .use_relaxed_order(relaxed_order ? CGAL::Tag_true() : CGAL::Tag_false())
                              );
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif
}

} // namespace sms

// Export Polygon_mesh_processing
void export_triangulated_surface_mesh_simplification(py::module_& m) {
  using Tm = pmp::Polygonal_mesh;
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
    .def(py::init<edges_size_type>(), py::arg("threshold"));
    ;

  using Ecrsp = SMS::Edge_count_ratio_stop_predicate<Tm>;
  py::class_<Ecrsp>(m, "Edge_count_ratio_stop_predicate")
    .def(py::init<double>(), py::arg("ratio"));
    ;

  using Fcsp = SMS::Face_count_stop_predicate<Tm>;
  py::class_<Fcsp>(m, "Face_count_stop_predicate")
    .def(py::init<edges_size_type>(), py::arg("threshold"));
    ;

  using Fcrsp = SMS::Face_count_ratio_stop_predicate<Tm>;
  py::class_<Fcrsp>(m, "Face_count_ratio_stop_predicate")
    .def(py::init<double, const Tm&>(), py::arg("ratio"), py::arg("tmesh"));
    ;


  m.def("edge_collapse", &sms::edge_collapse<Tm, Ecsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
  m.def("edge_collapse", &sms::edge_collapse<Tm, Ecrsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
  m.def("edge_collapse", &sms::edge_collapse<Tm, Fcsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
  m.def("edge_collapse", &sms::edge_collapse<Tm, Fcrsp>,
    py::arg("pm"), py::arg("stop_policy"), py::arg("np") = py::dict());
}

