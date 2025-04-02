// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <stdexcept>
#include <iterator>

#include <boost/graph/graph_traits.hpp>
#include <boost/iterator/function_output_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Kernel/Dimension_utils.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/boost/graph/Face_filtered_graph.h>
#include <CGAL/Dynamic_property_map.h>
#include <CGAL/Mesh_constant_domain_field_3.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/iterator.h>
#include <CGAL/Named_function_parameters.h>
#include <CGAL/tags.h>
#include <CGAL/Mesh_facet_topology.h>
#include <CGAL/Polygon_mesh_processing/detect_features.h>
#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#include <CGAL/Polygon_mesh_processing/refine_mesh_at_isolevel.h>
#include <CGAL/Polygon_mesh_processing/region_growing.h>
#include <CGAL/Polygon_mesh_processing/repair.h>
#include <CGAL/Polygon_mesh_processing/locate.h>
#include <CGAL/Polygon_mesh_processing/triangle.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/HFDefault_visitor.hpp"
#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/parse_named_parameters.hpp"
#include "CGALPY/Polyhedral_envelope.hpp"
#include "CGALPY/pmp_helpers.hpp"
#include "CGALPY/Internal_face_plane_3_map.hpp"
#include "CGALPY/merge_coplanar_facets.hpp"
#include "CGALPY/Named_parameter_wrapper.hpp"
#include "CGALPY/named_parameter_applicator.hpp"
#include "CGALPY/Named_parameter_geom_traits.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

using Point_3_vec = std::vector<Point_3>;
using Size_t_vec = std::vector<std::size_t>;

//!
template <typename PolygonMesh>
auto triangulate_and_refine_hole(PolygonMesh& pmesh,
                                 typename boost::graph_traits<PolygonMesh>::halfedge_descriptor border_halfedge,
                                 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<Fd> faces;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      PMP::triangulate_and_refine_hole(pmesh, border_halfedge,
                                       internal::parse_pmp_np<Pm>(np)
                                       .visitor(visitor)
                                       .output_iterator(std::back_inserter(faces)));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::triangulate_and_refine_hole(pmesh, border_halfedge,
                                     internal::parse_pmp_np<PolygonMesh>(np)
                                     .output_iterator(std::back_inserter(faces)));
  }
  return faces;
}

//!
template <typename PolygonMesh>
auto triangulate_hole(PolygonMesh& pmesh,
                      typename boost::graph_traits<PolygonMesh>::halfedge_descriptor border_halfedge,
    const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      PMP::triangulate_hole(pmesh, border_halfedge,
                            internal::parse_pmp_np<Pm>(np).visitor(visitor));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::triangulate_hole(pmesh, border_halfedge, // this returned an emptyset iterator
                          internal::parse_pmp_np<PolygonMesh>(np));
  }
}

//!
template <typename PolygonMesh>
auto edge_bbox(const typename boost::graph_traits<PolygonMesh>::edge_descriptor ed,
               const PolygonMesh& pm,
               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;

  return PMP::edge_bbox(ed, pm, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
auto face_bbox(const typename boost::graph_traits<PolygonMesh>::face_descriptor fd,
               const PolygonMesh& pm,
               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::face_bbox(fd, pm, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh, typename ValueMap>
auto refine_mesh_at_isolevel(PolygonMesh& pm,
                             ValueMap value_map,
                             typename boost::property_traits<ValueMap>::value_type isovalue,
                             const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;

  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  return PMP::refine_mesh_at_isolevel(pm, value_map, isovalue,
                                      internal::parse_pmp_np<Pm>(np)
                                      .edge_is_constrained_map(eicm));
}

//!
template <typename TriangleMesh>
auto triangle(typename boost::graph_traits<TriangleMesh>::face_descriptor fd,
              const TriangleMesh& tmesh,
              const py::dict& np = py::dict()) {
  return PMP::triangle(fd, tmesh, internal::parse_pmp_np<TriangleMesh>(np));
}

//!
template <typename PolygonMesh>
auto vertex_bbox(typename boost::graph_traits<PolygonMesh>::vertex_descriptor vd,
                 const PolygonMesh& pm,
                 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::vertex_bbox(vd, pm, internal::parse_pmp_np<Pm>(np));
}

//
template <typename PolygonMesh>
auto extract_boundary_cycles(PolygonMesh& pm) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;

  std::vector<Hd> result;
  auto it = std::back_inserter(result);
  PMP::extract_boundary_cycles(pm, it);
  return result;
}

//!
template <typename PolygonMesh>
auto add_bbox(PolygonMesh& pmesh, const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  PMP::add_bbox(pmesh, internal::parse_pmp_np<Pm>(np));
}

//!
template <typename PolygonMesh>
auto bbox(PolygonMesh& pmesh,
          const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  return PMP::bbox(pmesh, internal::parse_pmp_np<Pm>(np));
}

//
template <typename PolygonMesh>
auto triangulate_hole_polyline_2(const Point_3_vec& polyline1,
                                 const Point_3_vec& polyline2,
                                 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Triangle_int = CGAL::Triple<int, int, int>;
  std::vector<Triangle_int> out;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      PMP::triangulate_hole_polyline(polyline1, std::back_inserter(out),
                                     internal::parse_pmp_np<Pm>(np).visitor(visitor));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::triangulate_hole_polyline(polyline1, polyline2, std::back_inserter(out),
                                   internal::parse_pmp_np<PolygonMesh>(np));
  }
  std::array<int, 3> out2 = {out[0].first, out[0].second, out[0].third};
  return out2;
}

//!
template <typename PolygonMesh>
auto triangulate_hole_polyline(const Point_3_vec& polyline,
                               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Triangle_int = CGAL::Triple<int, int, int>;
  std::vector<Triangle_int> out;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      PMP::triangulate_hole_polyline(polyline, std::back_inserter(out),
                                     internal::parse_pmp_np<Pm>(np).visitor(visitor));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::triangulate_hole_polyline(polyline, std::back_inserter(out),
                                   internal::parse_pmp_np<PolygonMesh>(np));
  }
  std::array<int, 3> out2 = {out[0].first, out[0].second, out[0].third};
  return out2;
}

//
template <typename PolygonMesh>
auto triangulate_refine_and_fair_hole(PolygonMesh& pmesh,
                                      typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& border_halfedge,
                                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Graph_traits = boost::graph_traits<Pm>;
  using halfedge_descriptor = typename Graph_traits::halfedge_descriptor;
  using Fd = typename Graph_traits::face_descriptor;
  using My_visitor = pmp::HFDefault_visitor;
  using Visitor = CGAL::Polygon_mesh_processing::Hole_filling::Default_visitor;

  ///// change this to a more general type
  using Vertex_identifier = typename Graph_traits::vertex_descriptor;
  using Face_identifier = typename Graph_traits::face_descriptor;
  /////

  std::vector<Face_identifier> fids;
  std::vector<Vertex_identifier> vids;
  auto it1 = std::back_inserter(fids);
  auto it2 = std::back_inserter(vids);
  if (np.contains("visitor")) {
    My_visitor visitor = py::cast<My_visitor>(np["visitor"]);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge,
                                                     internal::parse_pmp_np<Pm>(np)
                                                     .face_output_iterator(it1).vertex_output_iterator(it2)
                                                     .visitor(visitor));
    return py::make_tuple(std::get<0>(res), fids, vids);
  } else {
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge,
                                                     internal::parse_pmp_np<Pm>(np)
                                                     .face_output_iterator(it1).vertex_output_iterator(it2));
    return py::make_tuple(std::get<0>(res), fids, vids);
  }
}

//!
template <typename PolygonMesh>
auto remove_almost_degenerate_faces_r(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                                      PolygonMesh& pmesh,
                                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Pnt = Point_3;
  auto eicm = get_edge_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP0",
     np.contains("edge_is_constrained_map") ?
     np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>
    (pmesh, "INTERNAL_MAP1",
     np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  std::function<bool(Pnt, Pnt, Pnt)> filter =
    [](const Point_3&, const Pnt&, const Pnt&) { return true; };
  if (np.contains("filter")) {
    try {
      filter = py::cast<std::function<bool(Pnt, Pnt, Pnt)>>(np["filter"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast filter to std::function<bool(Point_3, Point_3, Point_3)>");
    }
  }

  struct Filter {
    std::function<bool(Point_3, Point_3, Point_3)> filter;
    bool operator()(const Point_3& p0, const Point_3& p1,
                    const Point_3& p2) const
    { return filter(p0, p1, p2); }
  };

  Filter f;
  f.filter = filter;
  auto retv = PMP::remove_almost_degenerate_faces(face_range, pmesh,
                                     internal::parse_pmp_np<PolygonMesh>(np)
                                     .edge_is_constrained_map(eicm)
                                     .vertex_is_constrained_map(vicm)
                                     .filter(f));

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ?
    pmesh.remove_property_map(eicm) : void();
  ! np.contains("vertex_is_constrained_map") ?
    pmesh.remove_property_map(vicm) : void();
#endif

  return retv;
}

//!
template <typename TriangleMesh>
auto remove_almost_degenerate_faces(TriangleMesh& tmesh,
                                    const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  auto eicm = get_edge_prop_map<Tm, bool>
    (tmesh, "INTERNAL_MAP0",
     np.contains("edge_is_constrained_map") ?
     np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<TriangleMesh, bool>
    (tmesh, "INTERNAL_MAP1",
     np.contains("vertex_is_constrained_map") ?
     np["vertex_is_constrained_map"] : py::none());
  std::function<bool(Point_3, Point_3, Point_3)> filter =
    [](const Point_3&, const Point_3&, const Point_3&) { return true; };
  if (np.contains("filter")) {
    try {
      filter = py::cast<std::function<bool(Point_3, Point_3, Point_3)>>(np["filter"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast filter to std::function<bool(Point_3, Point_3, Point_3)>");
    }
  }
  struct Filter {
    std::function<bool(Point_3, Point_3, Point_3)> filter;
    bool operator()(const Point_3& p0, const Point_3& p1, const Point_3& p2) const
    { return filter(p0, p1, p2); }
  };
  Filter f; f.filter = filter;
  auto retv = PMP::remove_almost_degenerate_faces(tmesh,
                                                  internal::parse_pmp_np<Tm>(np)
                                                  .edge_is_constrained_map(eicm)
                                                  .vertex_is_constrained_map(vicm)
                                                  .filter(f));

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ? tmesh.remove_property_map(eicm) : void();
  ! np.contains("vertex_is_constrained_map") ? tmesh.remove_property_map(vicm) : void();
#endif

  return retv;
}

//!
template <typename TriangleMesh>
auto remove_connected_components_of_negligible_size(TriangleMesh& tmesh,
                                                    const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  auto eicm = get_edge_prop_map<Tm, bool>(tmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ?
                                          np["edge_is_constrained_map"] : py::none());
  bool fim_flag = np.contains("face_index_map");
  std::size_t retv;
  if (fim_flag) {
    auto fim = get_face_prop_map<Tm, std::size_t>(tmesh, "INTERNAL_MAP1",
                                                  np["face_index_map"]);
    retv = PMP::remove_connected_components_of_negligible_size(tmesh,
                                                               internal::parse_pmp_np<TriangleMesh>(np)
                                                               .edge_is_constrained_map(eicm)
                                                               .face_index_map(fim));
  }
  else {
    retv = PMP::remove_connected_components_of_negligible_size(tmesh,
                                                               internal::parse_pmp_np<TriangleMesh>(np)
                                                               .edge_is_constrained_map(eicm));
  }

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  ! np.contains("edge_is_constrained_map") ? tmesh.remove_property_map(eicm) : void();
#endif

  return retv;
}

//!
template <typename PolygonMesh, typename FaceNormalMap>
void merge_coplanar_facets(PolygonMesh& mesh,
                           FaceNormalMap face_normals,
                           const py::dict& params = py::dict()) {
  using Pm = PolygonMesh;
  for (const auto& item : params) {
    const std::string& key = py::cast<std::string>(item.first);
    if (key == "geom_traits") {
      auto np = CGAL::parameters::geom_traits(py::cast<const Kernel&>(item.second));
      return PMP::merge_coplanar_facets(mesh, face_normals, np);
    }
  }
  PMP::merge_coplanar_facets(mesh, face_normals);
}

//!
template <typename TriangleMesh>
auto area(const TriangleMesh& tm, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::area(tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename TriangleMesh>
auto area_f(const std::vector<typename boost::graph_traits<TriangleMesh>::face_descriptor>& face_range,
            const TriangleMesh& tm,
            const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<TriangleMesh>;
  using Fd = typename Gt::face_descriptor;
  return PMP::area(face_range, tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename TriangleMesh>
auto centroid(const TriangleMesh& tm,
              const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::centroid(tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename TriangleMesh>
auto edge_length(typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& e,
                 const TriangleMesh& tm,
                 const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::edge_length(e, tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename TriangleMesh>
auto face_area(typename boost::graph_traits<TriangleMesh>::face_descriptor& f,
               const TriangleMesh& tm,
               const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::face_area(f, tm, internal::parse_pmp_np<Tm>(np));
}

template <typename TriangleMesh>
auto face_aspect_ratio(typename boost::graph_traits<TriangleMesh>::face_descriptor& f,
                       const TriangleMesh& tm,
                       const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::face_aspect_ratio(f, tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename TriangleMesh>
auto face_border_length(typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& h,
                        const TriangleMesh& tm,
                        const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::face_border_length(h, tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename TriangleMesh>
auto longest_border(const TriangleMesh& tm,
                    const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::longest_border(tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename TriangleMesh>
auto match_faces(const TriangleMesh& tm1,
                 const TriangleMesh& tm2,
                 const py::dict& np1 = py::dict(),
                 const py::dict& np2 = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<std::pair<Fd, Fd>> common;
  std::vector<Fd> m1_only, m2_only;
  PMP::match_faces(tm1, tm2, std::back_inserter(common),
                   std::back_inserter(m1_only), std::back_inserter(m2_only),
                   internal::parse_pmp_np<Tm>(np1),
                   internal::parse_pmp_np<Tm>(np2));
  return std::make_tuple(common, m1_only, m2_only);
}

//!
template <typename TriangleMesh>
auto squared_edge_length(typename boost::graph_traits<TriangleMesh>::edge_descriptor& e,
                         const TriangleMesh& tm,
                         const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::squared_edge_length(e, tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename TriangleMesh>
auto squared_face_area(typename boost::graph_traits<TriangleMesh>::face_descriptor& f,
                       const TriangleMesh& tm,
                       const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::squared_face_area(f, tm, internal::parse_pmp_np<Tm>(np));
}

template <typename TriangleMesh>
auto volume(const TriangleMesh& tm,
            const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  return PMP::volume(tm, internal::parse_pmp_np<Tm>(np));
}

//!
template <typename PolygonMesh>
void interpolated_corrected_curvatures(PolygonMesh& pmesh,
                                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;

  auto vmcm = get_vertex_prop_map<Pm, FT>
    (pmesh, "INTERNAL_MAP0",
     np.contains("vertex_mean_curvature_map") ?
     np["vertex_mean_curvature_map"] : py::none());
  auto vgcm = get_vertex_prop_map<Pm, FT>
    (pmesh, "INTERNAL_MAP1",
     np.contains("vertex_Gaussian_curvature_map") ?
     np["vertex_Gaussian_curvature_map"] : py::none());

  using Pcad = PMP::Principal_curvatures_and_directions<Kernel>;
  auto vpcdm = get_vertex_prop_map<Pm, Pcad>
    (pmesh, "INTERNAL_MAP2",
     np.contains("vertex_principal_curvatures_and_directions_map") ?
     np["vertex_principal_curvatures_and_directions_map"] : py::none());


  if (np.contains("vertex_normal_map")) {
    auto vnm = get_vertex_prop_map<Pm, Vector_3>
      (pmesh, "INTERNAL_MAP3", np.contains("vertex_normal_map") ?
       np["vertex_normal_map"] : py::none());
    // auto vpmap = get_vertex_point_map(pm, np); // does not work
    PMP::interpolated_corrected_curvatures(pmesh, internal::parse_pmp_np<Pm>(np)
                                           .vertex_mean_curvature_map(vmcm)
                                           .vertex_Gaussian_curvature_map(vgcm)
                                           .vertex_principal_curvatures_and_directions_map(vpcdm)
                                           .vertex_normal_map(vnm)
                                           );
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
    if (np.contains("vertex_normal_map")) {
      pmesh.remove_property_map(vnm);
    }
#endif
  }
  else {
    PMP::interpolated_corrected_curvatures(pmesh, internal::parse_pmp_np<Pm>(np)
                                           .vertex_mean_curvature_map(vmcm)
                                           .vertex_Gaussian_curvature_map(vgcm)
                                           .vertex_principal_curvatures_and_directions_map(vpcdm)
                                           );
  }
  // delete the internal maps
#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("vertex_mean_curvature_map")) {
    pmesh.remove_property_map(vmcm);
  }
  if (! np.contains("vertex_Gaussian_curvature_map")) {
    pmesh.remove_property_map(vgcm);
  }
  if (! np.contains("vertex_principal_curvatures_and_directions_map")) {
    pmesh.remove_property_map(vpcdm);
  }
#endif
}

//!
template <typename PolygonMesh>
auto interpolated_corrected_curvatures_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                                         PolygonMesh& pm,
                                         const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  py::object vmc = py::none();
  py::object vgc = py::none();

  auto pcad = PMP::Principal_curvatures_and_directions<Kernel>();

  double vmc_d, vGc;
  if (np.contains("vertex_normal_map")) {
    auto vnm = get_vertex_prop_map<Pm, Vector_3>
      (pm, "INTERNAL_MAP1", np.contains("vertex_normal_map") ? np["vertex_normal_map"] : py::none());
    PMP::interpolated_corrected_curvatures(v, pm,
    internal::parse_pmp_np<Pm>(np)
                                           .vertex_mean_curvature(std::ref(vmc_d))
                                           .vertex_Gaussian_curvature(std::ref(vGc))
                                           .vertex_principal_curvatures_and_directions(std::ref(pcad))
                                           .vertex_normal_map(vnm)
                                           );
  #if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
    pm.remove_property_map(vnm);
  #endif
  } else {
    PMP::interpolated_corrected_curvatures(v, pm,
    internal::parse_pmp_np<Polygonal_mesh>(np)
                                           .vertex_mean_curvature(std::ref(vmc_d))
                                           .vertex_Gaussian_curvature(std::ref(vGc))
                                           .vertex_principal_curvatures_and_directions(std::ref(pcad))
                                           );
  }
  return std::make_tuple(vmc_d, vGc, pcad);
}

//!
template <typename PolygonMesh>
auto border_halfedges(const std::vector<typename boost::graph_traits<PolygonMesh>::face_descriptor>& face_range,
                      PolygonMesh& pmesh,
                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Hd = typename Gt::halfedge_descriptor;

  std::vector<Hd> out;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP0",
      np.contains("face_index_map") ? np["face_patch_index_map"] : py::none());
    PMP::border_halfedges(face_range, pmesh, std::back_inserter(out),
                          internal::parse_pmp_np<Pm>(np)
                          .face_index_map(fim)
                          );
  }
  else {
    PMP::border_halfedges(face_range, pmesh, std::back_inserter(out),
                          internal::parse_pmp_np<Pm>(np)
                          );
  }
  return out;
}

//!
py::list ptvec2ptlist(const Point_3_vec& ptvec) {
  py::list ptlist;
  for (auto pt : ptvec) {
    ptlist.append(pt);
  }
  return ptlist;
}

//!
Point_3_vec ptlist2ptvec(const py::list& ptlist) {
  Point_3_vec ptvec;
  ptvec.reserve(py::len(ptlist));
  for (auto pt : ptlist) {
    try {
      ptvec.push_back(py::cast<Point_3>(pt));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast to a Point_3");
    }
  }
  return ptvec;
}

//!
auto polylist2polyvec(const py::list& polylist) {
  std::vector<Size_t_vec> polyvec;
  polyvec.reserve(py::len(polylist));
  for (auto poly : polylist) {
    std::vector<std::size_t> poly_ids;
    // poyl_ids.reserve(py::len(poly));
    for (auto polyid : poly) {
      std::size_t id;
      try {
        id = py::cast<size_t>(polyid);
        poly_ids.push_back(id);
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Failed to cast to a size_t");
      }
      poly_ids.push_back(id);
    }
    polyvec.push_back(poly_ids);
  }
  return polyvec;
}

//!
auto polyvec2polylist(const std::vector<std::vector<std::size_t>> polyvec) {
  py::list polylist;
  for (auto poly : polyvec) {
    py::list p;
    for (auto pid : poly) {
      p.append(pid);
    }
    polylist.append(p);
  }
  return polylist;
}

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

using Pm = Polygonal_mesh;
using Gt = boost::graph_traits<Polygonal_mesh>;
using Vd = typename Gt::vertex_descriptor;
using Hd = typename Gt::halfedge_descriptor;
using Fd = typename Gt::face_descriptor;

//!
template <typename PolygonMesh, typename RegionMap>
auto region_growing_of_planes_on_faces(PolygonMesh& pmesh,
                                       RegionMap region_map,
                                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  std::size_t num_regions;

  if (np.contains("region_primitive_map")) {
    boost::vector_property_map<Vector_3> rpm;
    try {
      rpm = py::cast<boost::vector_property_map<Vector_3>>(np["region_primitive_map"]);
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Failed to cast to a vector property map");
    }
    num_regions = PMP::region_growing_of_planes_on_faces(pmesh, region_map,
                                                         internal::parse_pmp_np<Pm>(np)
                                                         .region_primitive_map(rpm));
  }
  else {
    num_regions = PMP::region_growing_of_planes_on_faces(pmesh, region_map,
                                                         internal::parse_pmp_np<Pm>(np));
  }
  return num_regions;
}

//!
template <typename PolygonMesh, typename RegionMap, typename CornerIdMap>
auto detect_corners_of_regions(PolygonMesh& pmesh,
                               RegionMap region_map,
                               std::size_t nb_regions,
                               CornerIdMap corner_id_map,
                               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  std::size_t num_corners;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
                                          np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
  std::size_t r = PMP::detect_corners_of_regions(pmesh, region_map, nb_regions, corner_id_map,
                                                 internal::parse_pmp_np<Pm>(np)
                                                 .edge_is_constrained_map(eicm));

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  if (! np.contains("edge_is_constrained_map")) {
    pmesh.remove_property_map(eicm);
  }
#endif
  return r;
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

//!
auto barycentic_coordinates(const Point_3& p, const Point_3& a,
                            const Point_3& b, const Point_3& c) {
  return PMP::barycentric_coordinates(p, a, b, c, Kernel());
}

//!
template <typename TriangleMesh>
auto get_descriptor_from_location(const std::pair<
                                  typename boost::graph_traits<TriangleMesh>::face_descriptor,
                                  std::array<FT, 3> >& loc,
                                  const TriangleMesh& tm) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;
  using Hd = typename Gt::halfedge_descriptor;
  using Barycentric_coordinates = std::array<FT, 3>;
  // returns a variant of vertex_descriptor, halfedge_descriptor, face_descriptor
  try {
    return py::cast(std::get<Vd>(PMP::get_descriptor_from_location(loc, tm)));
  }
  catch (const std::bad_variant_access& e) {
    try {
      return py::cast(std::get<Hd>(PMP::get_descriptor_from_location(loc, tm)));
    }
    catch (const std::bad_variant_access& e) {
      try {
        return py::cast(std::get<Fd>(PMP::get_descriptor_from_location(loc, tm)));
      }
      catch (const std::bad_variant_access& e) {
        throw std::runtime_error("get_descriptor_from_location failed");
      }
    }
  }
}

//!
template <typename TriangleMesh>
auto is_in_face_bar(const std::array<FT, 3>& bar, const TriangleMesh& tm)
{ return PMP::is_in_face(bar, tm); }

//!
template <typename TriangleMesh>
auto is_in_face_loc(const std::pair<
                    typename boost::graph_traits<TriangleMesh>::face_descriptor,
                    std::array<FT, 3>>& loc,
                    const TriangleMesh& tm)
{ return PMP::is_in_face(loc, tm); }

//!
template <typename TriangleMesh>
auto is_on_face_border(const std::pair<
                       typename boost::graph_traits<TriangleMesh>::face_descriptor,
                       std::array<FT, 3> >& loc,
                       const TriangleMesh& tm)
{ return PMP::is_on_face_border(loc, tm); }

//!
template <typename TriangleMesh>
auto is_on_halfedge(const std::pair<
                typename boost::graph_traits<TriangleMesh>::face_descriptor,
                std::array<FT, 3> >& loc,
                    const typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& hd,
                    const TriangleMesh& tm)
{ return PMP::is_on_halfedge(loc, hd, tm); }

//!
template <typename TriangleMesh>
auto is_on_mesh_border(const std::pair<
                typename boost::graph_traits<TriangleMesh>::face_descriptor,
                std::array<FT, 3> >& loc,
                       const TriangleMesh& tm)
{ return PMP::is_on_mesh_border(loc, tm); }

//!
template <typename TriangleMesh>
auto is_on_vertex(const std::pair<
                  typename boost::graph_traits<TriangleMesh>::face_descriptor,
                  std::array<FT, 3> >& loc,
                   const typename boost::graph_traits<TriangleMesh>::vertex_descriptor& vd,
                   const TriangleMesh& tm)
{ return PMP::is_on_vertex(loc, vd, tm); }

//!
template <typename TriangleMesh>
auto degenerate_edges_r(const std::vector<
                        typename boost::graph_traits<TriangleMesh>::edge_descriptor>& edges,
                        const TriangleMesh& tmesh,
                        const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Ed = typename Gt::edge_descriptor;
  std::vector<Ed> out;
  PMP::degenerate_edges(edges, tmesh, std::back_inserter(out),
                        internal::parse_pmp_np<Tm>(np));
  return out;
}

//!
template <typename TriangleMesh>
auto degenerate_edges(TriangleMesh& tmesh, const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Ed = typename Gt::edge_descriptor;
  std::vector<Ed> out;
  PMP::degenerate_edges(tmesh, std::back_inserter(out),
                        internal::parse_pmp_np<Tm>(np));
  return out;
}

//!
template <typename TriangleMesh>
auto degenerate_faces_r(const std::vector<typename boost::graph_traits<TriangleMesh>::face_descriptor>& faces,
                        const TriangleMesh& tmesh,
                        const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<Fd> out;
  PMP::degenerate_faces(faces, tmesh, std::back_inserter(out),
                        internal::parse_pmp_np<Tm>(np));
  return out;
}

//!
template <typename TriangleMesh>
auto degenerate_faces(TriangleMesh& tmesh,
                      const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<Fd> out;
  PMP::degenerate_faces(tmesh, std::back_inserter(out),
                        internal::parse_pmp_np<Tm>(np));
  return out;
}

//!
template <typename TriangleMesh>
auto is_cap_triangle_face(typename boost::graph_traits<TriangleMesh>::face_descriptor f,
                          const TriangleMesh& tm,
                          const double threshold,
                          const py::dict& np = py::dict()) {
  auto retv = PMP::is_cap_triangle_face(f, tm, threshold,
                                        internal::parse_pmp_np<TriangleMesh>(np));
  // return retv != boost::graph_traits<TriangleMesh>::null_halfedge() ? py::cast(retv) : py::none();
  if (retv == boost::graph_traits<TriangleMesh>::null_halfedge()) {
    throw std::runtime_error("is_cap_triangle_face failed");
  }
  return retv;
}

//!
template <typename TriangleMesh>
auto is_degenerate_edge(typename boost::graph_traits<TriangleMesh>::edge_descriptor e,
                        const TriangleMesh& tm,
                        const py::dict& np = py::dict()) {
  return PMP::is_degenerate_edge(e, tm,
                                 internal::parse_pmp_np<TriangleMesh>(np));
}

//!
template <typename TriangleMesh>
auto is_degenerate_triangle_face(typename boost::graph_traits<TriangleMesh>::face_descriptor f,
                                 const TriangleMesh& tm,
                                 const py::dict& np = py::dict()) {
  return PMP::is_degenerate_triangle_face(f, tm,
                                          internal::parse_pmp_np<TriangleMesh>(np));
}

//!
template <typename TriangleMesh>
auto is_needle_triangle_face(typename boost::graph_traits<TriangleMesh>::face_descriptor f,
                             const TriangleMesh& tm,
                             const double threshold,
                             const py::dict& np = py::dict()) {
  return PMP::is_needle_triangle_face(f, tm, threshold,
                                      internal::parse_pmp_np<TriangleMesh>(np));
}

// using Boolean_operation_type = COREFINEMENT::Boolean_operation_type;

// HFDefault_visitor
void set_start_planar_phase(HFDefault_visitor& v,
                            const std::function<void()>& f)
{ v.set_start_planar_phase(f); }

//!
void set_end_planar_phase(HFDefault_visitor& v,
                          const std::function<void(bool)>& f)
{ v.set_end_planar_phase(f); }

//!
void set_start_quadratic_phase(HFDefault_visitor& v,
                               const std::function<void(std::size_t)>& f)
{ v.set_start_quadratic_phase(f); }

//!
void set_quadratic_step(HFDefault_visitor& v,
                        const std::function<void()>& f)
{ v.set_quadratic_step(f); }

//!
void set_end_quadratic_phase(HFDefault_visitor& v,
                             const std::function<void(bool)>& f)
{ v.set_end_quadratic_phase(f); }

//!
void set_start_cubic_phase(HFDefault_visitor& v,
                           const std::function<void(std::size_t)>& f)
{ v.set_start_cubic_phase(f); }

//!
void set_cubic_step(HFDefault_visitor& v,
                    const std::function<void()>& f)
{ v.set_cubic_step(f); }

//!
void set_end_cubic_phase(HFDefault_visitor& v,
                         const std::function<void()>& f)
{ v.set_end_cubic_phase(f); }

//!
void set_start_refine_phase(HFDefault_visitor& v,
                            const std::function<void()>& f)
{ v.set_start_refine_phase(f); }

//!
void set_end_refine_phase(HFDefault_visitor& v,
                          const std::function<void()>& f)
{ v.set_end_refine_phase(f); }

//!
void set_start_fair_phase(HFDefault_visitor& v,
                          const std::function<void()>& f)
{ v.set_start_fair_phase(f); }

//!
void set_end_fair_phase(HFDefault_visitor& v,
                        const std::function<void()>& f)
{ v.set_end_fair_phase(f); }

} // namespace pmp

// Export Polygon_mesh_processing
void export_polygon_mesh_processing(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;
  // using Vid = pmp::Vertex_identifier;
  // using Hid = pmp::Halfedge_identifier;
  // using Fid = pmp::Face_identifier;
  using Polyline = std::vector<Kernel::Point_3>;
  using Np = CGAL::parameters::Default_named_parameters;
  namespace PMP = CGAL::Polygon_mesh_processing;
  using Fd = boost::graph_traits<Pm>::face_descriptor;
  using Hd = boost::graph_traits<Pm>::halfedge_descriptor;
  using Vd = boost::graph_traits<Pm>::vertex_descriptor;
  using Ed = boost::graph_traits<Pm>::edge_descriptor;
  using faces_size_type = boost::graph_traits<Pm>::faces_size_type;

  using Np_t = bool;
  using Np_tag = CGAL::internal_np::all_default_t;
  using Np_base = CGAL::internal_np::No_property;
  using Np_class = CGAL::Named_function_parameters<Np_t, Np_tag, Np_base>;
  using Concurrency_tag = CGAL::Sequential_tag;
// FacePatchMap	a class model of ReadablePropertyMap with boost::graph_traits<TriangleMeshIn>::face_descriptor as key type and std::size_t as value type
// EdgeIsConstrainedMap	a class model of ReadablePropertyMap with boost::graph_traits<TriangleMeshIn>::edge_descriptor as key type and bool as value type
// VertexCornerMap	a class model of ReadablePropertyMap with boost::graph_traits<TriangleMeshIn>::vertex_descriptor as key type and std::size_t as value type

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  using Vertex_size_map = Pm::Property_map<Vd, std::size_t>;
  using Vertex_double_map = Pm::Property_map<Vd, double>;
  using Edge_bool_map = Pm::Property_map<Ed, bool>;
  using Face_bool_map = Pm::Property_map<Fd, bool>;
  using Face_size_type_map = Pm::Property_map<Fd, faces_size_type>;
  using Face_size_map = Pm::Property_map<Fd, std::size_t>;
  using Face_plane_map = Pm::Property_map<Fd, Plane_3>;
  using Face_vector_map = Pm::Property_map<Fd, Vector_3>;
#endif

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH
  using Vertex_size_tag = CGAL::dynamic_vertex_property_t<std::size_t>;
  using Vertex_size_map = boost::property_map<Pm, Vertex_size_tag>::type;

  using Vector_double_tag = CGAL::dynamic_vertex_property_t<double>;
  using Vertex_double_map = boost::property_map<Pm, Vector_double_tag>::type;

  using Edge_bool_tag = CGAL::dynamic_edge_property_t<bool>;
  using Edge_bool_map = boost::property_map<Pm, Edge_bool_tag>::type;

  using Face_size_tag = CGAL::dynamic_face_property_t<std::size_t>;
  using Face_size_map = boost::property_map<Pm, Face_size_tag>::type;

  using Face_size_type_tag = CGAL::dynamic_face_property_t<faces_size_type>;
  using Face_size_type_map = boost::property_map<Pm, Face_size_type_tag>::type;

  using Face_bool_tag = CGAL::dynamic_face_property_t<bool>;
  using Face_bool_map = boost::property_map<Pm, Face_bool_tag>::type;

  using Face_plane_tag = CGAL::dynamic_face_property_t<Plane_3>;
  using Face_plane_map = boost::property_map<Pm, Face_plane_tag>::type;

  using Face_vector_tag = CGAL::dynamic_face_property_t<Vector_3>;
  using Face_vector_map = boost::property_map<Pm, Face_vector_tag>::type;
#endif

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Np_class>(m, "Named_function_parameters")
    .def(py::init<>());

  // Hole Filling
  m.def("triangulate_and_refine_hole", &pmp::triangulate_and_refine_hole<Pm>,
        py::arg("pm"), py::arg("hole_boundary"),
        py::arg("np") = py::dict());
  m.def("triangulate_hole", &pmp::triangulate_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"),
        py::arg("np") = py::dict());
  m.def("triangulate_hole_polyline", &pmp::triangulate_hole_polyline<Pm>,
        py::arg("points"), py::arg("np") = py::dict());
  m.def("triangulate_hole_polyline", &pmp::triangulate_hole_polyline_2<Pm>,
        py::arg("points"), py::arg("third_points"),
        py::arg("np") = py::dict());
  m.def("triangulate_refine_and_fair_hole",
        &pmp::triangulate_refine_and_fair_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"),
        py::arg("np") = py::dict());

  // Meshing

#if ((CGALPY_KERNEL != CGALPY_KERNEL_EPEC) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EPEC_WITH_SQRT) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL) && \
     (CGALPY_KERNEL != CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2))
  //! \todo Fix interpolated_corrected_curvatures to use epeck
  // Corrected Curvature Computation
  m.def("interpolated_corrected_curvatures",
        &pmp::interpolated_corrected_curvatures<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());

  m.def("interpolated_corrected_curvatures",
        &pmp::interpolated_corrected_curvatures_v<Pm>,
        py::arg("v"), py::arg("pm"), py::arg("np") = py::dict());
#endif

  // Geometric Measure Functions
  m.def("area", &pmp::area<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("area", &pmp::area_f<Pm>,
        py::arg("face_range"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("centroid", &pmp::centroid<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("edge_length", &pmp::edge_length<Pm>,
        py::arg("h"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("face_area", &pmp::face_area<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("face_aspect_ratio", &pmp::face_aspect_ratio<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("face_border_length", &pmp::face_border_length<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("longest_border", &pmp::longest_border<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("match_faces", &pmp::match_faces<Pm>,
        py::arg("m1"), py::arg("m2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("squared_edge_length", &pmp::squared_edge_length<Pm>,
        py::arg("h"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("squared_face_area", &pmp::squared_face_area<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("volume", &pmp::volume<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict());

  // Geometric Repair
  m.def("remove_almost_degenerate_faces",
        &pmp::remove_almost_degenerate_faces_r<Pm>,
        py::arg("face_range"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("remove_almost_degenerate_faces",
        &pmp::remove_almost_degenerate_faces<Pm>,
        py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("remove_connected_components_of_negligible_size",
        &pmp::remove_connected_components_of_negligible_size<Pm>, // TODO: output_iterator
        py::arg("tmesh"), py::arg("np") = py::dict());
  m.def("remove_isolated_vertices", &PMP::remove_isolated_vertices<Pm>,
        py::arg("pmesh"));

  // Feature Detection Functions
  m.def("detect_sharp_edges", &pmp::detect_sharp_edges<Pm, Edge_bool_map>,
        py::arg("pm"), py::arg("angle_in_deg"), py::arg("edge_is_feature_map"),
        py::arg("np") = py::dict());
  // only for sm
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

  // Location Functions
  m.def("barycentic_coordinates", &pmp::barycentic_coordinates,
        py::arg("p"), py::arg("q"), py::arg("r"), py::arg("query"));
  m.def("get_descriptor_from_location", &pmp::get_descriptor_from_location<Pm>,
        py::arg("loc"), py::arg("tm"));
  m.def("is_in_face", &pmp::is_in_face_bar<Pm>,
        py::arg("bar"), py::arg("tm"));
  m.def("is_on_face_border", &pmp::is_on_face_border<Pm>,
        py::arg("loc"), py::arg("tm"));
  m.def("is_on_halfedge", &pmp::is_on_halfedge<Pm>,
        py::arg("loc"), py::arg("hd"), py::arg("tm"));
  m.def("is_on_mesh_border", &pmp::is_on_mesh_border<Pm>,
        py::arg("loc"), py::arg("tm"));
  m.def("is_on_vertex", &pmp::is_on_vertex<Pm>,
        py::arg("loc"), py::arg("vd"), py::arg("tm"));

  // Predicates
  m.def("degenerate_edges", &pmp::degenerate_edges_r<Pm>,
        py::arg("edges"), py::arg("tm"),
        py::arg("np") = py::dict());
  m.def("degenerate_edges", &pmp::degenerate_edges<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("degenerate_faces", &pmp::degenerate_faces_r<Pm>,
        py::arg("faces"), py::arg("tm"),
        py::arg("np") = py::dict());
  m.def("degenerate_faces", &pmp::degenerate_faces<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("is_cap_triangle_face", &pmp::is_cap_triangle_face<Pm>,
        py::arg("f"), py::arg("tm"),
        py::arg("threshold"), py::arg("np") = py::dict());
  m.def("is_degenerate_edge", &pmp::is_degenerate_edge<Pm>,
        py::arg("e"), py::arg("pm"),
        py::arg("np") = py::dict());
  m.def("is_degenerate_triangle_face", &pmp::is_degenerate_triangle_face<Pm>,
        py::arg("f"), py::arg("tm"),
        py::arg("np") = py::dict());
  m.def("is_needle_triangle_face", &pmp::is_needle_triangle_face<Pm>,
        py::arg("f"), py::arg("tm"),
        py::arg("threshold"), py::arg("np") = py::dict());

  // other
  m.def("add_bbox", &pmp::add_bbox<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("bbox", &pmp::bbox<Pm>,
        py::arg("pmesh"),
        py::arg("np") = py::dict());
  m.def("border_halfedges", &pmp::border_halfedges<Pm>,
        py::arg("face_range"), py::arg("pm"),
        py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("detect_corners_of_regions",
        &pmp::detect_corners_of_regions<Pm, Face_size_map, Vertex_size_map>,
        py::arg("pmesh"), py::arg("region_map"), py::arg("nb_regions"),
        py::arg("corner_id_map"),
        py::arg("np") = py::dict());
#endif

  m.def("edge_bbox", &pmp::edge_bbox<Pm>,
        py::arg("ed"), py::arg("pmesh"),
        py::arg("np") = py::dict());
  m.def("extract_boundary_cycles", &pmp::extract_boundary_cycles<Pm>,
        py::arg("pmesh"));
  m.def("face_bbox", &pmp::face_bbox<Pm>,
        py::arg("f"), py::arg("pmesh"),
        py::arg("np") = py::dict());

#if CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_SURFACE_MESH_POLYGONAL_MESH
  m.def("refine_mesh_at_isolevel", &pmp::refine_mesh_at_isolevel<Pm, Vertex_double_map>,
        py::arg("pm"), py::arg("value_map"), py::arg("isovalue"),
        py::arg("np") = py::dict());
  m.def("region_growing_of_planes_on_faces",
        &pmp::region_growing_of_planes_on_faces<Pm, Face_size_map>,
        py::arg("pmesh"), py::arg("region_map"), py::arg("np") = py::dict());
#endif

  // region growing only for sm
  // TODO: transform
  m.def("triangle", &pmp::triangle<Pm>,
        py::arg("f"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("vertex_bbox", &pmp::vertex_bbox<Pm>,
        py::arg("vd"), py::arg("pmesh"),
        py::arg("np") = py::dict());

  // HFDefault_visitor
  m.def("set_start_planar_phase", &pmp::set_start_planar_phase);
  m.def("set_end_planar_phase", &pmp::set_end_planar_phase);
  m.def("set_start_quadratic_phase", &pmp::set_start_quadratic_phase);
  m.def("set_quadratic_step", &pmp::set_quadratic_step);
  m.def("set_end_quadratic_phase", &pmp::set_end_quadratic_phase);
  m.def("set_start_cubic_phase", &pmp::set_start_cubic_phase);
  m.def("set_cubic_step", &pmp::set_cubic_step);
  m.def("set_end_cubic_phase", &pmp::set_end_cubic_phase);
  m.def("set_start_refine_phase", &pmp::set_start_refine_phase);
  m.def("set_end_refine_phase", &pmp::set_end_refine_phase);
  m.def("set_start_fair_phase", &pmp::set_start_fair_phase);
  m.def("set_end_fair_phase", &pmp::set_end_fair_phase);

  // Custom sizing field
  // m.def("set_at", &pmp::set_at);
  // m.def("set_is_too_long", &pmp::set_is_too_long);
  // m.def("set_is_too_short", &pmp::set_is_too_short);
  // m.def("set_split_placement", &pmp::set_split_placement);
  // m.def("set_register_split_vertex", &pmp::set_register_split_vertex);

  using Pe = pmp::Polyhedral_envelope<Pm, Kernel>;
  py::class_<Pe>(m, "Polyhedral_envelope")
    .def(py::init<Pm&, double, const py::dict&>(),
         py::arg("tmesh"), py::arg("epsilon"), py::arg("np") = py::dict())
    .def(py::init<const std::vector<Fd>&, Pm&, double, const py::dict&>(),
         py::arg("face_range"), py::arg("tmesh"), py::arg("epsilon"),
         py::arg("np") = py::dict())
    .def(py::init<const pmp::Point_3_vec&, const std::vector<pmp::Size_t_vec>&, double, const py::dict&>(),
         py::arg("points"), py::arg("polygons"), py::arg("epsilon"),
         py::arg("np") = py::dict()) // TODO: handle face_epsilon_map
    .def("is_empty", &Pe::is_empty)
    .def("inside", [](const Pe& i, const Point_3& query) { return i(query); },
         py::arg("query"))
    .def("inside",
         [](const Pe& i, const Point_3& source, const Point_3& target)
         { return i(source, target); },
         py::arg("source"), py::arg("target"))
    .def("inside",
         [](const Pe& i, const Point_3& t0, const Point_3& t1, const Point_3& t2)
         { return i(t0, t1, t2); },
         py::arg("t0"), py::arg("t1"), py::arg("t2"))
    .def("inside",
         [](const Pe& i, const Pm& tmesh, const py::dict& np)
         { return i(tmesh, internal::parse_pmp_np<Pm>(np)); },
         py::arg("tmesh"), py::arg("np") = py::dict())
    // TODO: inside triangle range
    ;

  using Pcad = PMP::Principal_curvatures_and_directions<Kernel>;
  py::class_<Pcad>(m, "Principal_curvatures_and_directions")
    .def(py::init<FT, FT, Vector_3, Vector_3>())
    .def_ro("min_curvature", &Pcad::min_curvature)
    .def_ro("max_curvature", &Pcad::max_curvature)
    .def_ro("min_direction", &Pcad::min_direction)
    .def_ro("max_direction", &Pcad::max_direction)
    ;

  // using Csf = pmp::Custom_sizing_field<Pm>;
  // py::class_<Csf>(m, "Custom_sizing_field")
  //   .def(py::init<>())
  //   ;

  // hole filling
  using Hfv = pmp::HFDefault_visitor;
  py::class_<Hfv>(m, "Hole_filling_default_visitor")
    .def(py::init<>())
    ;

#if ((CGALPY_PMP_POLYGONAL_MESH == CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH) && \
     (CGALPY_POL3_GEOMETRY_TRAITS == CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS))
  using Face_normal_map = pol3::Internal_face_plane_3_map<Pm>;
#else
  using Face_normal_map = Face_vector_map;
#endif

  m.def("merge_coplanar_facets",
        &pmp::merge_coplanar_facets<Pm, Face_normal_map>,
        py::arg("pmesh"), py::arg("face_normals"),
        py::arg("np") = py::dict());
}
