// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <CGAL/Polygon_mesh_processing/measure.h>
#include <limits>
#define CGAL_USE_BASIC_VIEWER

#include <stdexcept>
#include <iterator>

#include <boost/graph/graph_traits.hpp>
#include <boost/iterator/function_output_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>

#include <CGAL/Dynamic_property_map.h>
#include <CGAL/Mesh_constant_domain_field_3.h>
#include <CGAL/Polygon_mesh_processing/angle_and_area_smoothing.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/iterator.h>
#include <CGAL/Named_function_parameters.h>
#include <CGAL/tags.h>
#include <CGAL/Mesh_facet_topology.h>
#include <CGAL/Polygon_mesh_processing/autorefinement.h>
#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/detect_features.h>
#include <CGAL/Polygon_mesh_processing/distance.h>
#include <CGAL/Polygon_mesh_processing/extrude.h>
#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#include <CGAL/Polygon_mesh_processing/intersection.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup_extension.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/merge_border_vertices.h>
#include <CGAL/Polygon_mesh_processing/random_perturbation.h>
#include <CGAL/Polygon_mesh_processing/refine.h>
#include <CGAL/Polygon_mesh_processing/region_growing.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/remesh_planar_patches.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/surface_Delaunay_remeshing.h>

#include "CGALPY/helpers.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/Adaptive_sizing_field.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#include "CGALPY/Non_manifold_output_visitor.hpp"
#include "CGALPY/Default_visitor.hpp"
#include "CGALPY/HFDefault_visitor.hpp"
#include "CGALPY/Default_orientation_visitor.hpp"
#include "CGALPY/Autorefinement_visitor.hpp"
#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/internal.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;

namespace pmp {

// map getters
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh>
auto get_vertex_point_map(const PolygonMesh& pm, const py::dict& np = py::dict()) {
  using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using vpmap = typename PolygonMesh::template Property_map<Vd, Point_3>;
  auto vpm = np.contains("vertex_point_map") ? py::cast<vpmap>(np["vertex_point_map"]) : pm.points();
  return vpm;
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh>
auto get_vertex_point_map(PolygonMesh& pm, const py::dict& np = py::dict()) {
  // TODO: get the map from the dict
  return get(CGAL::vertex_point, pm);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename V>
auto get_vertex_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const V& default_value = V()) {
  using K = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, V>;
  return dict_key.is_none() ? pm.template add_property_map<K, V>(map_name, V()).first :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
  // boost::property_map<Mesh, CGAL::dynamic_vertex_property_t<Epic_kernel::FT>>::type
template <typename PolygonMesh, typename V>
auto get_vertex_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const V& default_value = V()) {
  using dynamic_prop = typename CGAL::dynamic_vertex_property_t<V>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  return dict_key.is_none() ? get(dynamic_prop(), pm) :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename F>
auto get_edge_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using K = typename boost::graph_traits<PolygonMesh>::edge_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, F>;
  return dict_key.is_none() ? pm.template add_property_map<K, F>(map_name, F()).first :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh, typename F>
auto get_edge_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using dynamic_prop = typename CGAL::dynamic_edge_property_t<F>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  return dict_key.is_none() ? get(dynamic_prop(), pm) :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename F>
auto get_face_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using K = typename boost::graph_traits<PolygonMesh>::face_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, F>;
  return dict_key.is_none() ? pm.template add_property_map<K, F>(map_name, F()).first :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
template <typename PolygonMesh, typename F>
auto get_face_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none(), const F& default_value = F()) {
  using dynamic_prop = typename CGAL::dynamic_face_property_t<F>;
  using map_type = typename boost::property_map<PolygonMesh, dynamic_prop>::type;
  return dict_key.is_none() ? get(dynamic_prop(), pm) :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

/*! Determine whether two polylines intersect.
 * It's a shame that we cannot pass the begin1,end1,begin2,end2
 * directly to the CGAL do_intersect function.
 */
bool do_intersect_polylines(const py::list& lst1, const py::list& lst2) {
  auto begin1 = stl_input_iterator<Point_3>(lst1);
  auto end1 = stl_input_iterator<Point_3>(lst1, false);
  std::vector<Point_3> polyline1(begin1, end1);
  auto begin2 = stl_input_iterator<Point_3>(lst2);
  auto end2 = stl_input_iterator<Point_3>(lst2, false);
  std::vector<Point_3> polyline2(begin2, end2);
  return PMP::do_intersect(polyline1, polyline2);
}

/*! Determine whether two ranges of polylines intersect.
 */
bool do_intersect_polyline_ranges(const py::list& lsts1, const py::list& lsts2) {
  std::vector<std::vector<Point_3>> range1, range2;
  for (const auto& lh1 : lsts1) {
    auto begin1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh1));
    auto end1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh1), false);
    range1.emplace_back(begin1, end1);
  }
  for (const auto& lh2 : lsts2) {
    auto begin1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh2));
    auto end1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh2), false);
    range2.emplace_back(begin1, end1);
  }
  return PMP::do_intersect(range1, range2);
}

//
template <typename PolygonMesh>
bool do_intersect_meshes(const PolygonMesh& pm1, const PolygonMesh& pm2,
                         const py::dict& np1 = py::dict(),
                         const py::dict& np2 = py::dict()) {
  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  return PMP::do_intersect(pm1, pm2,
                           internal::parse_pmp_np<PolygonMesh>(np1)
                           // .vertex_point_map(vpm1)
                           ,
                           internal::parse_pmp_np<PolygonMesh>(np2)
                           // .vertex_point_map(vpm2)
                           );
}

//
template <typename PolygonMesh>
bool do_intersect_mesh_polyline(const PolygonMesh& pm, const py::list& lst,
                                const py::dict& np = py::dict()) {
  auto begin = stl_input_iterator<Point_3>(lst);
  auto end = stl_input_iterator<Point_3>(lst, false);
  std::vector<Point_3> polyline(begin, end);
  // auto vpm = get_vertex_point_map(pm, np);
  return PMP::do_intersect(pm, polyline, internal::parse_pmp_np<PolygonMesh>(np)
                                         // .vertex_point_map(vpm)
                                         );
}

//
template <typename PolygonMesh>
bool do_intersect_mesh_polyline_range(const PolygonMesh& pm,
                                      const py::list& lsts,
                                      const py::dict& np = py::dict()) {
  std::vector<std::vector<Point_3>> range;
  for (const auto& lh : lsts) {
    auto begin1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh));
    auto end1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh), false);
    range.emplace_back(begin1, end1);
  }
  // auto vpm = get_vertex_point_map(pm, np);
  return PMP::do_intersect(pm, range, internal::parse_pmp_np<PolygonMesh>(np)
                                      // .vertex_point_map(vpm)
                                      );
}

//
template <typename PolygonMesh>
py::list intersecting_meshes(const py::list& range,
                             const py::dict& np = py::dict(),
                             const py::list& nps = py::list()) {
  using Pm = PolygonMesh;

  py::list result;
  auto begin = stl_input_iterator<Pm>(range);
  auto end = stl_input_iterator<Pm>(range, false);
  auto op = [&] (const std::pair<std::size_t, std::size_t>& res) mutable
            { result.append(res); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::intersecting_meshes(boost::make_iterator_range(begin, end), it,
                            internal::parse_pmp_np<PolygonMesh>(np));

  return result;
}

//
template <typename PolygonMesh>
py::list self_intersections(const PolygonMesh& pm,
                            const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  // auto vpm = get_vertex_point_map(pm, np);

  py::list result;
  auto op = [&] (const std::pair<Fd, Fd>& res) mutable
            { result.append(py::make_tuple(res.first, res.second)); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::self_intersections(pm, it, internal::parse_pmp_np<PolygonMesh>(np)
                          // .vertex_point_map(vpm)
                          );

  return result;
}

//
template <typename PolygonMesh>
py::list self_intersections_faces(const py::list& face_range,
                                  const PolygonMesh& pm,
                                  const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  // auto vpm = get_vertex_point_map(pm, np);

  auto begin = stl_input_iterator<Fd>(face_range);
  auto end = stl_input_iterator<Fd>(face_range, false);

  py::list result;
  auto op = [&] (const std::pair<Fd, Fd>& res) mutable
            { result.append(res); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::self_intersections(boost::make_iterator_range(begin, end), pm, it,
                          internal::parse_pmp_np<PolygonMesh>(np)
                          // .vertex_point_map(vpm)
                          );
  return result;
}

auto triangle_soup_self_intersections(const py::list& points,
                                      const py::list& triangles,
                                      const py::dict& np = py::dict()) {

  const auto ptvec = list2vec<Point_3>(points);
  std::vector<std::array<std::size_t, 3>> trivec;
  for (const auto& tri : triangles) {
    std::array<std::size_t, 3> triarr;
    for (std::size_t i = 0; i < 3; ++i) {
      triarr[i] = py::cast<std::size_t>(tri[i]);
    }
    trivec.push_back(triarr);
  }
  std::vector<std::pair<std::size_t, std::size_t>> result;
  PMP::triangle_soup_self_intersections(ptvec, trivec, std::back_inserter(result));
  py::list lst;
  for (const auto& res : result) {
    lst.append(py::make_tuple(res.first, res.second));
  }
  return lst;
}

//
template <typename PolygonMesh>
bool does_self_intersect(const PolygonMesh& pm,
                         const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
  return PMP::does_self_intersect(pm, internal::parse_pmp_np<PolygonMesh>(np)
                                      // .vertex_point_map(get_vertex_point_map(pm, np))
                                      );
}

auto does_triangle_soup_self_intersect(const py::list& points,
                                       const py::list& triangles,
                                       const py::dict& np = py::dict()) {
  auto begin = stl_input_iterator<Point_3>(points);
  auto end = stl_input_iterator<Point_3>(points, false);
  std::vector<Point_3> points_vec(begin, end);
  auto begin2 = stl_input_iterator<std::array<std::size_t, 3>>(triangles);
  auto end2 = stl_input_iterator<std::array<std::size_t, 3>>(triangles, false);
  std::vector<std::array<std::size_t, 3>> triangles_vec(begin2, end2);
  return PMP::does_triangle_soup_self_intersect(points_vec, triangles_vec);
}

//
template <typename PolygonMesh>
bool does_self_intersect_faces(const py::list& face_range,
                               const PolygonMesh& pm,
                               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  // auto vpm = get_vertex_point_map(pm, np);

  auto begin = stl_input_iterator<Fd>(face_range);
  auto end = stl_input_iterator<Fd>(face_range, false);
  return PMP::does_self_intersect(boost::make_iterator_range(begin, end), pm, internal::parse_pmp_np<PolygonMesh>(np)
                                                                      // .vertex_point_map(get_vertex_point_map(pm, np))
                                                                      );
}

template <typename PolygonMesh>
void corefine(PolygonMesh& tm1, PolygonMesh& tm2,
              const py::dict& np1 = py::dict(),
              const py::dict& np2 = py::dict()) {
  // auto vpm1 = get_vertex_point_map(tm1, np1);
  // auto vpm2 = get_vertex_point_map(tm2, np2);
  auto eicm1 = get_edge_prop_map<PolygonMesh, bool>(tm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<PolygonMesh, bool>(tm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());
  // np1 can have a corefinement visitor
  bool visitor = np1.contains("visitor");
  if (visitor) {
    // try to cast to Non_manifold_output_visitor or Default_visitor
    try {
      auto visitor = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1)
                    // .vertex_point_map(vpm1)
                    .edge_is_constrained_map(eicm1)
                    .visitor(visitor),
                    internal::parse_pmp_np<PolygonMesh>(np2)
                    // .vertex_point_map(vpm2)
                    .edge_is_constrained_map(eicm2)
                    );
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1)
                    // .vertex_point_map(vpm1)
                    .edge_is_constrained_map(eicm1)
                    .visitor(visitor),
                    internal::parse_pmp_np<PolygonMesh>(np2)
                    // .vertex_point_map(vpm2)
                    .edge_is_constrained_map(eicm2)
                    );
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1)
                  // .vertex_point_map(vpm1)
                  .edge_is_constrained_map(eicm1)
                  ,
                  internal::parse_pmp_np<PolygonMesh>(np2)
                  // .vertex_point_map(vpm2)
                  .edge_is_constrained_map(eicm2)
                  );
  }
}

//
template <typename PolygonMesh>
py::list connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor
                      seed_face,
                    PolygonMesh& pm,
                    const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  py::list lst;
  auto op = [&] (Fd face_descriptor) mutable
            { lst.append(py::cast(face_descriptor)); };
  auto it = boost::make_function_output_iterator(std::ref(op));

  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());

  PMP::connected_component(seed_face, pm, it, internal::parse_pmp_np<PolygonMesh>(np));
  return lst;
}

//
template <typename PolygonMesh, typename FaceComponentMap>
auto connected_components_map(PolygonMesh& pm,
                              FaceComponentMap& fccmap,
                              const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using value_type = typename boost::property_traits<FaceComponentMap>::value_type;
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
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  return retv;
}

template <typename PolygonMesh, typename FaceComponentMap>
auto keep_connected_components_map(PolygonMesh& pm,
                               const py::list& components_to_keep,
                               FaceComponentMap& fccmap,
                               const py::dict& np = py::dict()) {
  auto ctk = list2vec<std::size_t>(components_to_keep);
  if (np.contains("vertex_index_map")) {
    auto vim = get_vertex_prop_map<PolygonMesh, std::size_t>(pm, "INTERNAL_MAP0",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    PMP::keep_connected_components(pm, ctk, fccmap,
                                          internal::parse_pmp_np<PolygonMesh>(np)
                                          .vertex_index_map(vim));
  }
  else {
    PMP::keep_connected_components(pm, ctk, fccmap,
                                          internal::parse_pmp_np<PolygonMesh>(np));
  }
}

template <typename PolygonMesh>
auto keep_connected_components(PolygonMesh& pm,
                               const py::list& components_to_keep,
                               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());

  auto ctk = list2vec<Fd>(components_to_keep);

  bool vimap = np.contains("vertex_index_map");
  bool fimap = np.contains("face_index_map");
  if (vimap && fimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP2",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::keep_connected_components(pm, ctk,
                                   internal::parse_pmp_np<PolygonMesh>(np)
                                   .edge_is_constrained_map(eicm)
                                   .vertex_index_map(vim)
                                   .face_index_map(fim));
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    PMP::keep_connected_components(pm, ctk,
                                   internal::parse_pmp_np<PolygonMesh>(np)
                                   .edge_is_constrained_map(eicm)
                                   .vertex_index_map(vim));
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::keep_connected_components(pm, ctk,
                                   internal::parse_pmp_np<PolygonMesh>(np)
                                   .edge_is_constrained_map(eicm)
                                   .face_index_map(fim));
  }
  else {
    PMP::keep_connected_components(pm, ctk,
                                   internal::parse_pmp_np<PolygonMesh>(np));
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
}

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

#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (!np.contains("face_size_map")) pmesh.remove_property_map(fsm);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1

  return retv;
}

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

#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (!np.contains("face_size_map")) pmesh.remove_property_map(fsm);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1

  return retv;
}

template <typename PolygonMesh, typename FaceComponentMap>
auto remove_connected_components_map(PolygonMesh& pm,
                                     const py::list& components_to_remove,
                                     const FaceComponentMap& fccmap,
                                     const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using faces_size_type = typename Gt::faces_size_type;
  auto ctr = list2vec<std::size_t>(components_to_remove);

  if (np.contains("vertex_index_map")) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP0",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    return PMP::remove_connected_components(pm, ctr, fccmap,
                                            internal::parse_pmp_np<PolygonMesh>(np)
                                            .vertex_index_map(vim));
  }
  else {
    return PMP::remove_connected_components(pm, ctr, fccmap,
                                            internal::parse_pmp_np<PolygonMesh>(np));
  }
}

template <typename PolygonMesh>
auto remove_connected_components(PolygonMesh& pm,
                                 const py::list& components_to_remove,
                                 const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  auto ctr = list2vec<Fd>(components_to_remove);
  auto eicm = get_edge_prop_map<Pm, bool>(pm, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  bool vimap = np.contains("vertex_index_map");
  bool fimap = np.contains("face_index_map");
  if (vimap && fimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP2",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::remove_connected_components(pm, ctr,
                                            internal::parse_pmp_np<PolygonMesh>(np)
                                            .edge_is_constrained_map(eicm)
                                            .vertex_index_map(vim)
                                            .face_index_map(fim));
  }
  else if (vimap) {
    auto vim = get_vertex_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("vertex_index_map") ? np["vertex_internal_map"] : py::none());
    PMP::remove_connected_components(pm, ctr,
                                            internal::parse_pmp_np<PolygonMesh>(np)
                                            .edge_is_constrained_map(eicm)
                                            .vertex_index_map(vim));
  }
  else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::remove_connected_components(pm, ctr,
                                            internal::parse_pmp_np<PolygonMesh>(np)
                                            .edge_is_constrained_map(eicm)
                                            .face_index_map(fim));
  }
  else {
    PMP::remove_connected_components(pm, ctr,
                                            internal::parse_pmp_np<PolygonMesh>(np));
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np.contains("edge_is_constrained_map")) pm.remove_property_map(eicm);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
}

template <typename PolygonMesh>
auto split_connected_components(PolygonMesh& pmesh,
                                const py::list& cc_meshes,
                                const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto pmrange = list2vec<Pm>(cc_meshes);

  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_internal_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
    np.contains("face_patch_map") ? np["face_internal_map"] : py::none());
  // TODO: add index maps
  PMP::split_connected_components(pmesh, pmrange,
                                  internal::parse_pmp_np<PolygonMesh>(np)
                                  .edge_is_constrained_map(eicm)
                                  .face_patch_map(fpm));
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np.contains("edge_is_constrained_map")) pmesh.remove_property_map(eicm);
  if (!np.contains("face_patch_map")) pmesh.remove_property_map(fpm);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
}

template <typename PolygonMesh>
auto triangulate_and_refine_hole(PolygonMesh& pmesh,
		typename boost::graph_traits< PolygonMesh >::halfedge_descriptor border_halfedge,
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
                                       internal::parse_pmp_np<PolygonMesh>(np)
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
  return vec2list(faces);
}

template <typename PolygonMesh>
auto triangulate_hole(PolygonMesh& pmesh,
    typename boost::graph_traits< PolygonMesh >::halfedge_descriptor border_halfedge,
    const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  if (np.contains("visitor")) {
    // HFDefault_visitor
    try {
      auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
      return PMP::triangulate_hole(pmesh, border_halfedge,
                                   internal::parse_pmp_np<PolygonMesh>(np).visitor(visitor));
    } catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    return PMP::triangulate_hole(pmesh, border_halfedge,
                                 internal::parse_pmp_np<PolygonMesh>(np));
  }
}

//
template <typename PolygonMesh>
void merge_reversible_connected_components(PolygonMesh& pm,
                              const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<PolygonMesh, std::size_t>(pm, "INTERNAL_MAP0",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::merge_reversible_connected_components(pm, internal::parse_pmp_np<PolygonMesh>(np)
                                              .face_index_map(fim)
                                              // .vertex_point_map(vpm)
                                               );
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np.contains("face_index_map")) pm.remove_property_map(fim);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  }
  else {
    PMP::merge_reversible_connected_components(pm, internal::parse_pmp_np<PolygonMesh>(np)
                                              // .vertex_point_map(vpm)
                                               );
  }
}

template <typename TriangleMesh>
auto orient(TriangleMesh& tm,
            const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP0",
      np.contains("face_index_map") ? np["face_internal_map"] : py::none());
    PMP::orient(tm, internal::parse_pmp_np<TriangleMesh>(np)
                            .face_index_map(fim)
                            // .vertex_point_map(vpm)
                            );
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np.contains("face_index_map")) tm.remove_property_map(fim);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  }
  else {
    PMP::orient(tm, internal::parse_pmp_np<TriangleMesh>(np)
                            // .vertex_point_map(vpm)
                            );
  }
}

auto orient_polygon_soup(const py::list& vertices,
                         const py::list& faces,
                         const py::dict& np = py::dict()) {
  auto v = list2vec<Point_3>(vertices);
  auto f = list2vec<std::vector<std::size_t>>(faces);
  return PMP::orient_polygon_soup(v, f);
}

//
template <typename PolygonMesh>
py::list extract_boundary_cycles(PolygonMesh& pm) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;

  // extract_boundary_cycles return needs output iterator
  py::list result;
  auto op = [&] (const Hd& res) mutable
            { result.append(res); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::extract_boundary_cycles(pm, it);
  return result;
}

template <typename PolygonMesh>
py::tuple corefine_and_compute_boolean_operations(PolygonMesh& pm1, PolygonMesh& pm2,
                                                 const py::dict& np1 = py::dict(),
                                                 const py::dict& np2 = py::dict(),
                                                 const py::tuple& np_out = py::tuple()) {
  using Pm = PolygonMesh;
  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  auto eicm1 = get_edge_prop_map<Pm, bool>(pm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());

  std::array<py::dict, 4> np_out_arr;
  for (std::size_t i = 0; i < np_out.size(); ++i) {
    np_out_arr[i] = py::cast<py::dict>(np_out[i]);
  }

  // auto vpm_out1 = get_vertex_point_map(pm_out1, np_out_arr[0]);
  // auto vpm_out2 = get_vertex_point_map(pm_out2, np_out_arr[1]);
  // auto vpm_out3 = get_vertex_point_map(pm_out3, np_out_arr[2]);
  // auto vpm_out4 = get_vertex_point_map(pm_out4, np_out_arr[3]);
  auto eicm_out1 = get_edge_prop_map<Pm, bool>(pm1, "INTERNAL_MAP2",
    np_out_arr[0].contains("edge_is_constrained_map") ? np_out_arr[0]["edge_internal_map"] : py::none());
  auto eicm_out2 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP3",
    np_out_arr[1].contains("edge_is_constrained_map") ? np_out_arr[1]["edge_internal_map"] : py::none());
  auto eicm_out3 = get_edge_prop_map<Pm, bool>(pm1, "INTERNAL_MAP4",
    np_out_arr[2].contains("edge_is_constrained_map") ? np_out_arr[2]["edge_internal_map"] : py::none());
  auto eicm_out4 = get_edge_prop_map<Pm, bool>(pm2, "INTERNAL_MAP5",
    np_out_arr[3].contains("edge_is_constrained_map") ? np_out_arr[3]["edge_internal_map"] : py::none());

  Pm out_union, out_intersection, tm1_minus_tm2, tm2_minus_tm1;

  std::array<bool, 4> res;
  std::array<std::optional<Pm*>, 4> outputs = {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1};

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visit = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      .face_index_map(fim1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      .face_index_map(fim2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      } catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      .face_index_map(fim1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      .face_index_map(fim2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP7",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                    internal::parse_pmp_np<PolygonMesh>(np1)
                                    .face_index_map(fim1)
                                    // .vertex_point_map(vpm1)
                                    .edge_is_constrained_map(eicm1),
                                    internal::parse_pmp_np<PolygonMesh>(np2)
                                    .face_index_map(fim2)
                                    // .vertex_point_map(vpm2)
                                    .edge_is_constrained_map(eicm2),
                                    std::make_tuple(
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                      // .vertex_point_map(vpm_out1)
                                      .edge_is_constrained_map(eicm_out1),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                      // .vertex_point_map(vpm_out2)
                                      .edge_is_constrained_map(eicm_out2),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                      // .vertex_point_map(vpm_out3)
                                      .edge_is_constrained_map(eicm_out3),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                      // .vertex_point_map(vpm_out4)
                                      .edge_is_constrained_map(eicm_out4)
                                    )
                                    );
    }
    
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visit = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit)
                                      .face_index_map(fim1),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit)
                                      .face_index_map(fim1),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Pm, std::size_t>(pm1, "INTERNAL_MAP6",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                    internal::parse_pmp_np<PolygonMesh>(np1)
                                    // .vertex_point_map(vpm1)
                                    .edge_is_constrained_map(eicm1)
                                    .face_index_map(fim1),
                                    internal::parse_pmp_np<PolygonMesh>(np2)
                                    // .vertex_point_map(vpm2)
                                    .edge_is_constrained_map(eicm2),
                                    std::make_tuple(
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                      // .vertex_point_map(vpm_out1)
                                      .edge_is_constrained_map(eicm_out1),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                      // .vertex_point_map(vpm_out2)
                                      .edge_is_constrained_map(eicm_out2),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                      // .vertex_point_map(vpm_out3)
                                      .edge_is_constrained_map(eicm_out3),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                      // .vertex_point_map(vpm_out4)
                                      .edge_is_constrained_map(eicm_out4)
                                    )
                                    );
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visit = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2)
                                      .face_index_map(fim2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      }
      catch (const py::cast_error&) {
      }
      try {
        auto visit = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                      internal::parse_pmp_np<PolygonMesh>(np1)
                                      // .vertex_point_map(vpm1)
                                      .edge_is_constrained_map(eicm1)
                                      .visitor(visit),
                                      internal::parse_pmp_np<PolygonMesh>(np2)
                                      // .vertex_point_map(vpm2)
                                      .edge_is_constrained_map(eicm2)
                                      .face_index_map(fim2),
                                      std::make_tuple(
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                        // .vertex_point_map(vpm_out1)
                                        .edge_is_constrained_map(eicm_out1),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                        // .vertex_point_map(vpm_out2)
                                        .edge_is_constrained_map(eicm_out2),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                        // .vertex_point_map(vpm_out3)
                                        .edge_is_constrained_map(eicm_out3),
                                        internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                        // .vertex_point_map(vpm_out4)
                                        .edge_is_constrained_map(eicm_out4)
                                      )
                                      );
      }
      catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Pm, std::size_t>(pm2, "INTERNAL_MAP6",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                    internal::parse_pmp_np<PolygonMesh>(np1)
                                    // .vertex_point_map(vpm1)
                                    .edge_is_constrained_map(eicm1),
                                    internal::parse_pmp_np<PolygonMesh>(np2)
                                    // .vertex_point_map(vpm2)
                                    .edge_is_constrained_map(eicm2)
                                    .face_index_map(fim2),
                                    std::make_tuple(
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                      // .vertex_point_map(vpm_out1)
                                      .edge_is_constrained_map(eicm_out1),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                      // .vertex_point_map(vpm_out2)
                                      .edge_is_constrained_map(eicm_out2),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                      // .vertex_point_map(vpm_out3)
                                      .edge_is_constrained_map(eicm_out3),
                                      internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                      // .vertex_point_map(vpm_out4)
                                      .edge_is_constrained_map(eicm_out4)
                                    )
                                    );
    }
  }
  else {
    res = PMP::corefine_and_compute_boolean_operations(pm1, pm2, outputs,
                                  internal::parse_pmp_np<PolygonMesh>(np1)
                                  // .vertex_point_map(vpm1)
                                  .edge_is_constrained_map(eicm1),
                                  internal::parse_pmp_np<PolygonMesh>(np2)
                                  // .vertex_point_map(vpm2)
                                  .edge_is_constrained_map(eicm2),
                                  std::make_tuple(
                                    internal::parse_pmp_np<PolygonMesh>(np_out_arr[0])
                                    // .vertex_point_map(vpm_out1)
                                    .edge_is_constrained_map(eicm_out1),
                                    internal::parse_pmp_np<PolygonMesh>(np_out_arr[1])
                                    // .vertex_point_map(vpm_out2)
                                    .edge_is_constrained_map(eicm_out2),
                                    internal::parse_pmp_np<PolygonMesh>(np_out_arr[2])
                                    // .vertex_point_map(vpm_out3)
                                    .edge_is_constrained_map(eicm_out3),
                                    internal::parse_pmp_np<PolygonMesh>(np_out_arr[3])
                                    // .vertex_point_map(vpm_out4)
                                    .edge_is_constrained_map(eicm_out4)
                                   )
                                   );
  }


  auto retv = py::make_tuple(res[0] ? py::cast(out_union) : py::none(),
                        res[1] ? py::cast(out_intersection) : py::none(),
                        res[2] ? py::cast(tm1_minus_tm2) : py::none(),
                        res[3] ? py::cast(tm2_minus_tm1) : py::none());
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np1.contains("edge_is_constrained_map")) pm1.remove_property_map(eicm1);
  if (!np2.contains("edge_is_constrained_map")) pm2.remove_property_map(eicm2);
  if (!np_out_arr[0].contains("edge_is_constrained_map")) out_union.remove_property_map(eicm_out1);
  if (!np_out_arr[1].contains("edge_is_constrained_map")) out_intersection.remove_property_map(eicm_out2);
  if (!np_out_arr[2].contains("edge_is_constrained_map")) tm1_minus_tm2.remove_property_map(eicm_out3);
  if (!np_out_arr[3].contains("edge_is_constrained_map")) tm2_minus_tm1.remove_property_map(eicm_out4);
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
  return retv;
}

//
template <typename TriangleMesh>
TriangleMesh corefine_and_compute_intersection(TriangleMesh& pm1, TriangleMesh& pm2,
                                          const py::dict& np1 = py::dict(),
                                          const py::dict& np2 = py::dict(),
                                          const py::dict& np_out = py::dict()) {
  using Tm = TriangleMesh;

  Tm out;
  bool valid;

  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  // auto vpm3 = get_vertex_point_map(out, np_out);
  auto eicm1 = get_edge_prop_map<Tm, bool>(pm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
    np_out.contains("edge_is_constrained_map") ? np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else {
    valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                                internal::parse_pmp_np<TriangleMesh>(np1)
                                                // .vertex_point_map(vpm1)
                                                .edge_is_constrained_map(eicm1),
                                                internal::parse_pmp_np<TriangleMesh>(np2)
                                                // .vertex_point_map(vpm2)
                                                .edge_is_constrained_map(eicm2),
                                                internal::parse_pmp_np<TriangleMesh>(np_out)
                                                // .vertex_point_map(vpm3)
                                                .edge_is_constrained_map(eicm_out)
                                                );
  }
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}

//
template <typename TriangleMesh>
TriangleMesh corefine_and_compute_difference(TriangleMesh& pm1, TriangleMesh& pm2,
                                          const py::dict& np1 = py::dict(),
                                          const py::dict& np2 = py::dict(),
                                          const py::dict& np_out = py::dict()) {
  using Tm = TriangleMesh;

  Tm out;
  bool valid;

  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  // auto vpm3 = get_vertex_point_map(out, np_out);
  auto eicm1 = get_edge_prop_map<Tm, bool>(pm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
    np_out.contains("edge_is_constrained_map") ? np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else {
    valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                                internal::parse_pmp_np<TriangleMesh>(np1)
                                                // .vertex_point_map(vpm1)
                                                .edge_is_constrained_map(eicm1),
                                                internal::parse_pmp_np<TriangleMesh>(np2)
                                                // .vertex_point_map(vpm2)
                                                .edge_is_constrained_map(eicm2),
                                                internal::parse_pmp_np<TriangleMesh>(np_out)
                                                // .vertex_point_map(vpm3)
                                                .edge_is_constrained_map(eicm_out)
                                                );
  }
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}

//
template <typename TriangleMesh>
TriangleMesh corefine_and_compute_union(TriangleMesh& pm1, TriangleMesh& pm2,
                                          const py::dict& np1 = py::dict(),
                                          const py::dict& np2 = py::dict(),
                                          const py::dict& np_out = py::dict()) {
  using Tm = TriangleMesh;

  Tm out;
  bool valid;

  // auto vpm1 = get_vertex_point_map(pm1, np1);
  // auto vpm2 = get_vertex_point_map(pm2, np2);
  // auto vpm3 = get_vertex_point_map(out, np_out);
  auto eicm1 = get_edge_prop_map<Tm, bool>(pm1, "INTERNAL_MAP0",
    np1.contains("edge_is_constrained_map") ? np1["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Tm, bool>(pm2, "INTERNAL_MAP1",
    np2.contains("edge_is_constrained_map") ? np2["edge_internal_map"] : py::none());
  auto eicm_out = get_edge_prop_map<Tm, bool>(out, "INTERNAL_MAP2",
    np_out.contains("edge_is_constrained_map") ? np_out["edge_internal_map"] : py::none());

  bool fimb1 = np1.contains("face_index_map");
  bool fimb2 = np2.contains("face_index_map");
  bool visitor_flag = np1.contains("visitor");
  if (fimb1 && fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
          valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                      internal::parse_pmp_np<TriangleMesh>(np1)
                                                      // .vertex_point_map(vpm1)
                                                      .edge_is_constrained_map(eicm1)
                                                      .face_index_map(fim1)
                                                      .visitor(visitor),
                                                      internal::parse_pmp_np<TriangleMesh>(np2)
                                                      // .vertex_point_map(vpm2)
                                                      .face_index_map(fim2)
                                                      .edge_is_constrained_map(eicm2),
                                                      internal::parse_pmp_np<TriangleMesh>(np_out)
                                                      // .vertex_point_map(vpm3)
                                                      .edge_is_constrained_map(eicm_out)
                                                      );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP4",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb1) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
          np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .face_index_map(fim1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim1 = get_face_prop_map<Tm, std::size_t>(pm1, "INTERNAL_MAP3",
        np1.contains("face_index_map") ? np1["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1)
                                                  .face_index_map(fim1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else if (fimb2) {
    if (visitor_flag) {
      try {
        auto visitor = py::cast<pmp::Corefine_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
      }
      try {
        auto visitor = py::cast<pmp::Non_manifold_output_visitor<Tm>>(np1["visitor"]);
        auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
          np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
        valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                    internal::parse_pmp_np<TriangleMesh>(np1)
                                                    // .vertex_point_map(vpm1)
                                                    .edge_is_constrained_map(eicm1)
                                                    .visitor(visitor),
                                                    internal::parse_pmp_np<TriangleMesh>(np2)
                                                    // .vertex_point_map(vpm2)
                                                    .face_index_map(fim2)
                                                    .edge_is_constrained_map(eicm2),
                                                    internal::parse_pmp_np<TriangleMesh>(np_out)
                                                    // .vertex_point_map(vpm3)
                                                    .edge_is_constrained_map(eicm_out)
                                                    );
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      auto fim2 = get_face_prop_map<Tm, std::size_t>(pm2, "INTERNAL_MAP3",
        np2.contains("face_index_map") ? np2["face_internal_map"] : py::none());
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                  internal::parse_pmp_np<TriangleMesh>(np1)
                                                  // .vertex_point_map(vpm1)
                                                  .edge_is_constrained_map(eicm1),
                                                  internal::parse_pmp_np<TriangleMesh>(np2)
                                                  // .vertex_point_map(vpm2)
                                                  .face_index_map(fim2)
                                                  .edge_is_constrained_map(eicm2),
                                                  internal::parse_pmp_np<TriangleMesh>(np_out)
                                                  // .vertex_point_map(vpm3)
                                                  .edge_is_constrained_map(eicm_out)
                                                  );
    }
  }
  else {
    valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                                internal::parse_pmp_np<TriangleMesh>(np1)
                                                // .vertex_point_map(vpm1)
                                                .edge_is_constrained_map(eicm1),
                                                internal::parse_pmp_np<TriangleMesh>(np2)
                                                // .vertex_point_map(vpm2)
                                                .edge_is_constrained_map(eicm2),
                                                internal::parse_pmp_np<TriangleMesh>(np_out)
                                                // .vertex_point_map(vpm3)
                                                .edge_is_constrained_map(eicm_out)
                                                );
  }
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}

//
template <typename PolygonMesh>
auto autorefine(PolygonMesh& tm,
                const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  PMP::autorefine(tm, internal::parse_pmp_np<PolygonMesh>(np)
                  // .vertex_point_map(vpm)
                  );
}
//
template <typename PolygonMesh>
py::list triangulate_hole_polyline(const py::list& lst1, const py::list& lst2,
                                   const py::dict& np = py::dict()) {
  auto begin1 = stl_input_iterator<Point_3>(lst1);
  auto end1 = stl_input_iterator<Point_3>(lst1, false);
  std::vector<Point_3> polyline1(begin1, end1);
  auto begin2 = stl_input_iterator<Point_3>(lst2);
  auto end2 = stl_input_iterator<Point_3>(lst2, false);
  std::vector<Point_3> polyline2(begin2, end2);

  typedef CGAL::Triple<int, int, int> Triangle_int;
  std::vector<Triangle_int> out;
  if (py::len(lst2) == 0) {
    if (np.contains("visitor")) {
      // HFDefault_visitor
      try {
        auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
        PMP::triangulate_hole_polyline(polyline1, std::back_inserter(out),
                                       internal::parse_pmp_np<PolygonMesh>(np).visitor(visitor));
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      PMP::triangulate_hole_polyline(polyline1, std::back_inserter(out),
                                     internal::parse_pmp_np<PolygonMesh>(np));
    }
  }
  else {
    if (np.contains("visitor")) {
      // HFDefault_visitor
      try {
        auto visitor = py::cast<pmp::HFDefault_visitor>(np["visitor"]);
        PMP::triangulate_hole_polyline(polyline1, polyline2, std::back_inserter(out),
                                       internal::parse_pmp_np<PolygonMesh>(np).visitor(visitor));
      } catch (const py::cast_error&) {
        throw std::runtime_error("Visitor type not recognized");
      }
    }
    else {
      PMP::triangulate_hole_polyline(polyline1, polyline2, std::back_inserter(out),
                                     internal::parse_pmp_np<PolygonMesh>(np));
    }
  }
  // convert to a list of integers
  py::list result;
  for (const auto& t : out) {
    result.append(py::make_tuple(t.first, t.second, t.third));
  }
  return result;
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

  py::list facets, vertices;
  bool faces_flag = np.contains("face_output_iterator") && py::cast<bool>(np["face_output_iterator"]);
  bool vertices_flag = np.contains("vertex_output_iterator") && py::cast<bool>(np["vertex_output_iterator"]);


  std::vector<Face_identifier> fids;
  std::vector<Vertex_identifier> vids;
  auto it1 = std::back_inserter(fids);
  auto it2 = std::back_inserter(vids);
  if (np.contains("visitor")) {
    My_visitor visitor = py::cast<My_visitor>(np["visitor"]);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge,
                                                     internal::parse_pmp_np<PolygonMesh>(np)
                                                     .face_output_iterator(it1).vertex_output_iterator(it2)
                                                     .visitor(visitor));
    for (const auto& fid : fids) facets.append(fid);
    for (const auto& vid : vids) vertices.append(vid);
    return py::make_tuple(std::get<0>(res), facets, vertices);
  } else {
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge,
                                                     internal::parse_pmp_np<PolygonMesh>(np)
                                                     .face_output_iterator(it1).vertex_output_iterator(it2));
    for (const auto& fid : fids) facets.append(fid);
    for (const auto& vid : vids) vertices.append(vid);
    return py::make_tuple(std::get<0>(res), facets, vertices);
  }
}

//
template <typename PolygonMesh>
auto triangulate_faces(PolygonMesh& pm,
                              const py::dict& np) {
  using Pm = PolygonMesh;
  // TODO: visitor?
  auto vpm = get_vertex_point_map(pm, np);
  return PMP::triangulate_faces(pm, internal::parse_pmp_np<PolygonMesh>(np)
                                // .vertex_point_map(vpm)
                                );
}

template <typename PolygonMesh>
auto triangulate_faces_r(const py::list& face_range,
                         PolygonMesh& pm,
                         const py::dict& np) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  // auto vpm = get_vertex_point_map(pm, np);
  auto facevec = list2vec<Fd>(face_range);
  return PMP::triangulate_faces(facevec, pm, internal::parse_pmp_np<PolygonMesh>(np)
                                // .vertex_point_map(vpm)
                                );
}

//
// template <typename PolygonMesh>
// auto isotropic_remeshing(const py::list& face_range,
//                                 double target_edge_length,
//                                 PolygonMesh& pmesh,
//                                 const py::dict& np = py::dict()) {
//   using Pm = PolygonMesh;
//   using Gt = boost::graph_traits<Pm>;
//   using Fd = typename Gt::face_descriptor;
//
//   auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
//     np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
//   auto vicm = get_vertex_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP1",
//     np.contains("vertex_is_constrained_map") ? np["vertex_is_constrained_map"] : py::none());
//   auto fpm = get_face_prop_map<Pm, int>(pmesh, "INTERNAL_MAP3",
//     np.contains("face_patch_map") ? np["face_patch_map"] : py::none());
//
//   if (np.contains("face_index_map")) {
//     auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP2",
//       np.contains("face_index_map") ? np["face_index_map"] : py::none());
//     PMP::isotropic_remeshing(boost::make_iterator_range(stl_input_iterator<Fd>(face_range),
//                                                         stl_input_iterator<Fd>(face_range, false)),
//                              target_edge_length, pmesh,
//                              internal::parse_pmp_np<PolygonMesh>(np)
//                              .edge_is_constrained_map(eicm)
//                              .vertex_is_constrained_map(vicm)
//                              .face_index_map(fim)
//                              .face_patch_map(fpm));
//   }
//   else {
//     PMP::isotropic_remeshing(boost::make_iterator_range(stl_input_iterator<Fd>(face_range),
//                                                         stl_input_iterator<Fd>(face_range, false)),
//                              target_edge_length, pmesh,
//                              internal::parse_pmp_np<PolygonMesh>(np)
//                              .edge_is_constrained_map(eicm)
//                              .vertex_is_constrained_map(vicm)
//                              .face_patch_map(fpm));
//
//   }
// #if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
//   if (!np.contains("edge_is_constrained_map")) {
//     pmesh.remove_property_map(eicm);
//   }
//   if (!np.contains("vertex_is_constrained_map")) {
//     pmesh.remove_property_map(vicm);
//   }
//   if (!np.contains("face_patch_map")) {
//     pmesh.remove_property_map(fpm);
//   }
// #endif
// }


//
template <typename PolygonMesh, typename SizingFunction>
auto isotropic_remeshing_sf(const py::list& face_range,
                                SizingFunction& sizing,
                                PolygonMesh& pmesh,
                                const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  PMP::isotropic_remeshing(boost::make_iterator_range(stl_input_iterator<Fd>(face_range),
                                                      stl_input_iterator<Fd>(face_range, false)),
                           sizing, pmesh,
                           internal::parse_pmp_np<PolygonMesh>(np));

}

//
template <typename PolygonMesh>
auto remesh_planar_patches(PolygonMesh& pmesh,
                           const py::dict& np_in = py::dict(),
                           const py::dict& np_out = py::dict()) {

  using Pm = PolygonMesh;

  auto eicm = get_edge_prop_map<Pm, bool>
    (pmesh, "edge_is_constrained_map", np_in.contains("edge_is_constrained_map") ? np_in["edge_is_constrained_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>(pmesh, "face_patch_map",
    np_in.contains("face_patch_map") ? np_in["face_patch_map"] : py::none());
  auto vcm = get_vertex_prop_map<Pm, std::size_t>(pmesh, "vertex_corner_map",
    np_in.contains("vertex_corner_map") ? np_in["vertex_corner_map"] : py::none());

  auto fpm2 = get_face_prop_map<Pm, int>(pmesh, "face_patch_map",
    np_out.contains("face_patch_map") ? np_out["face_patch_map"] : py::none());
  auto vcm2 = get_vertex_prop_map<Pm, std::size_t>(pmesh, "vertex_corner_map",
    np_out.contains("vertex_corner_map") ? np_out["vertex_corner_map"] : py::none());

  Pm out;

  PMP::remesh_planar_patches(pmesh,
                             out,
                             internal::parse_pmp_np<PolygonMesh>(np_in)
                             .edge_is_constrained_map(eicm)
                             .face_patch_map(fpm)
                             .vertex_corner_map(vcm)
                             ,
                             internal::parse_pmp_np<PolygonMesh>(np_out)
                             .face_patch_map(fpm2)
                             .vertex_corner_map(vcm2)
                             );
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  if (!np_in.contains("edge_is_constrained_map")) {
    pmesh.remove_property_map(eicm);
  }
  if (!np_in.contains("face_patch_map")) {
    pmesh.remove_property_map(fpm);
  }
  if (!np_in.contains("vertex_corner_map")) {
    pmesh.remove_property_map(vcm);
  }
#endif
  return out;
}

//
template <typename PolygonMesh>
auto smooth_shape(PolygonMesh& pmesh,
                  const double time,
                        const py::dict& np = py::dict()) {
  using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  auto vpm = get_vertex_point_map(pmesh, np);
  auto propmap = get_vertex_prop_map<PolygonMesh, bool>
    (pmesh, "vertex_is_constrained_map", np.contains("vertex_is_constrained_map") ? np["vertex_is_constrained_map"] : py::none());
  return PMP::smooth_shape(pmesh, time,
                    internal::parse_named_parameters(np)
                    .vertex_is_constrained_map(propmap)
                    .vertex_point_map(vpm));
}

// template <typename PolygonMesh>
// auto tangential_relaxation(PolygonMesh& tm,
//                                   const py::dict& np = py::dict()) {
//   PMP::tangential_relaxation(tm, internal::parse_pmp_np<PolygonMesh>(np));
// }

template <typename PolygonMesh>
double approximate_Hausdorff_distance(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                      const py::dict& np1 = py::dict(),
                                      const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;
  // auto vpm1 = get_vertex_point_map(tm1, np1);
  // auto vpm2 = get_vertex_point_map(tm2, np2);
  return PMP::approximate_Hausdorff_distance<TAG>(tm1, tm2,
                                             internal::parse_pmp_np<PolygonMesh>(np1)
                                             // .vertex_point_map(vpm1)
                                             ,
                                             internal::parse_pmp_np<PolygonMesh>(np2)
                                             // .vertex_point_map(vpm2)
                                             );
}

template <typename TriangleMesh>
auto approximate_max_distance_to_point_set(const TriangleMesh& tm,
                                           const py::list& points,
                                           const double precision,
                                           const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  const auto ptvec = list2vec<Point_3>(points);
  return PMP::approximate_max_distance_to_point_set(tm, ptvec, precision,
                                                    internal::parse_pmp_np<TriangleMesh>(np)
                                                    // .vertex_point_map(vpm)
                                                    );
}

template <typename PolygonMesh>
auto approximate_symmetric_Hausdorff_distance(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                             const py::dict& np1 = py::dict(),
                                             const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;
  // auto vpm1 = get_vertex_point_map(tm1, np1);
  // auto vpm2 = get_vertex_point_map(tm2, np2);
  return PMP::approximate_symmetric_Hausdorff_distance<TAG>(tm1, tm2,
                                                        internal::parse_pmp_np<PolygonMesh>(np1)
                                                        // .vertex_point_map(vpm1)
                                                        ,
                                                        internal::parse_pmp_np<PolygonMesh>(np2)
                                                        // .vertex_point_map(vpm2)
                                                        );
}

template <typename PolygonMesh>
auto bounded_error_Hausdorff_distance(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                      const double error_bound = 0.0001,
                                      const py::dict& np1 = py::dict(),
                                      const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;
  // auto vpm1 = get_vertex_point_map(tm1, np1);
  // auto vpm2 = get_vertex_point_map(tm2, np2);
  return PMP::bounded_error_Hausdorff_distance<TAG>(tm1, tm2, error_bound,
                                                 internal::parse_pmp_np<PolygonMesh>(np1)
                                                 // .vertex_point_map(vpm1)
                                                 ,
                                                 internal::parse_pmp_np<PolygonMesh>(np2)
                                                 // .vertex_point_map(vpm2)
                                                 );
}

template <typename PolygonMesh>
auto is_Hausdorff_distance_larger(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                  const double distance_bound,
                                  const double error_bound,
                                  const py::dict& np1 = py::dict(),
                                  const py::dict& np2 = py::dict()) {
  // auto vpm = get_vertex_point_map(tm1, np1);
  using TAG = CGAL::Sequential_tag;
  return PMP::is_Hausdorff_distance_larger<TAG>(tm1, tm2, distance_bound, error_bound,
                                       internal::parse_pmp_np<PolygonMesh>(np1)
                                       // .vertex_point_map(vpm)
                                       ,
                                       internal::parse_pmp_np<PolygonMesh>(np2)
                                       // .vertex_point_map(vpm)
                                       );
}

template <typename TriangeMesh>
auto max_distance_to_triangle_mesh(const py::list& points,
                                   const TriangeMesh& tm,
                                   const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  using TAG = CGAL::Sequential_tag;
  const auto ptvec = list2vec<Point_3>(points);
  return PMP::max_distance_to_triangle_mesh<TAG>(ptvec, tm,
                                          internal::parse_pmp_np<TriangeMesh>(np)
                                          // .vertex_point_map(vpm)
                                          );
}

template <typename PolygonMesh>
auto surface_intersection(const PolygonMesh& tm1,
                          const PolygonMesh& tm2,
                          const py::dict& np1 = py::dict(),
                          const py::dict& np2 = py::dict()) {
  std::vector< std::vector<Point_3> > polylines;
  PMP::surface_intersection(tm1, tm2, std::back_inserter(polylines),
                            internal::parse_pmp_np<PolygonMesh>(np1),
                            internal::parse_pmp_np<PolygonMesh>(np2));
  py::list retv;
  for (const auto& item : polylines) {
    const auto linelist = vec2list(item);
    retv.append(linelist);
  }
  return retv;
}

template <typename PolygonMesh>
bool clip(PolygonMesh& tm, PolygonMesh& clipper,
          const py::dict& np_tm = py::dict(),
          const py::dict& np_c = py::dict()) {
  using Pm = PolygonMesh;

  // auto vpm1 = get_vertex_point_map(tm, np_tm);
  // auto vpm2 = get_vertex_point_map(clipper, np_c);
  auto eicm1 = get_edge_prop_map<Pm, bool>(tm, "INTERNAL_MAP0",
    np_tm.contains("edge_is_constrained_map") ? np_tm["edge_internal_map"] : py::none());
  auto eicm2 = get_edge_prop_map<Pm, bool>(clipper, "INTERNAL_MAP1",
    np_c.contains("edge_is_constrained_map") ? np_c["edge_internal_map"] : py::none());


  bool visitor1 = np_tm.contains("visitor");
  bool visitor2 = np_c.contains("visitor");
  bool res;
  if (visitor1 && visitor2) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  } else if (visitor1) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_tm["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_tm["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  } else if (visitor2) {
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_c["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).edge_is_constrained_map(eicm1),
                      internal::parse_pmp_np<PolygonMesh>(np_c).visitor(v2).edge_is_constrained_map(eicm2));
    }
    catch (const py::cast_error& e) {
      std::cerr << "Visitor type not recognized\n";
    }
  } else {
    res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np_tm).edge_is_constrained_map(eicm1),
                    internal::parse_pmp_np<PolygonMesh>(np_c).edge_is_constrained_map(eicm2));
  }
  return res;
}

template <typename TriangleMesh>
auto clip_c(TriangleMesh& tm,
            const Iso_cuboid_3& box,
            const py::dict& np = py::dict()) {
  using Pm = TriangleMesh;
  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Corefine_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, box, internal::parse_pmp_np<TriangleMesh>(np).visitor(v));
    } catch (const py::cast_error&) {
    }
    try {
      auto v = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, box, internal::parse_pmp_np<TriangleMesh>(np).visitor(v));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  } else {
    return PMP::clip(tm, box, internal::parse_pmp_np<TriangleMesh>(np));
  }
}

template <typename TriangleMesh>
auto clip_p(TriangleMesh& tm,
            const Plane_3& plane,
            const py::dict& np = py::dict()) {
  using Pm = TriangleMesh;
  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Corefine_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, plane, internal::parse_pmp_np<TriangleMesh>(np).visitor(v));
    } catch (const py::cast_error&) {
    }
    try {
      auto v = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np["visitor"]);
      return PMP::clip(tm, plane, internal::parse_pmp_np<TriangleMesh>(np).visitor(v));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Visitor type not recognized");
    }
  } else {
    return PMP::clip(tm, plane, internal::parse_pmp_np<TriangleMesh>(np));
  }
}

auto repair_polygon_soup(const py::list& points,
                         const py::list& polygons,
                         const py::dict& np = py::dict()) {
  PointRange ptvec = list2vec<Point_3>(points);
  PolygonRange polyvec;
  polyvec.reserve(py::len(polygons));
  for (const auto& item : polygons) {
    polyvec.push_back(list2vec<size_t>(py::cast<py::list>(item)));
  }
  PMP::repair_polygon_soup(ptvec, polyvec, internal::parse_named_parameters(np));
  py::list retpts, retpolys;
  retpts = vec2list(ptvec);
  for (const auto& item : polyvec) {
    retpolys.append(vec2list(item));
  }
  return py::make_tuple(retpts, retpolys);
}

auto triangulate_polygons(const py::list& points,
                         const py::list& polygons,
                         const py::dict& np = py::dict()) {
  PointRange ptvec = list2vec<Point_3>(points);
  PolygonRange polyvec;
  polyvec.reserve(py::len(polygons));
  for (const auto& item : polygons) {
    polyvec.push_back(list2vec<size_t>(py::cast<py::list>(item)));
  }
  PMP::triangulate_polygons(ptvec, polyvec, internal::parse_named_parameters(np));
  py::list retpts, retpolys;
  retpts = vec2list(ptvec);
  for (const auto& item : polyvec) {
    retpolys.append(vec2list(item));
  }
  return py::make_tuple(retpts, retpolys);
}

auto autorefine_triangle_soup(const py::list& points,
                         const py::list& polygons,
                         const py::dict& np = py::dict()) {
  PointRange ptvec = list2vec<Point_3>(points);
  PolygonRange polyvec;
  polyvec.reserve(py::len(polygons));
  for (const auto& item : polygons) {
    polyvec.push_back(list2vec<size_t>(py::cast<py::list>(item)));
  }

  // PMP::autorefine_triangle_soup(ptvec, polyvec, internal::parse_named_parameters(np));
  // check for autorefinement visitor
  bool visitor = np.contains("visitor");
  if (visitor) {
    try {
      auto v = py::cast<pmp::Autorefinement_visitor>(np["visitor"]);
      PMP::autorefine_triangle_soup(ptvec, polyvec, internal::parse_named_parameters(np).visitor(v));
    }
    catch (const py::cast_error& e) {
      throw std::runtime_error("Visitor type not recognized");
    }
  }
  else {
    PMP::autorefine_triangle_soup(ptvec, polyvec, internal::parse_named_parameters(np));
  }

  py::list retpts, retpolys;
  retpts = vec2list(ptvec);
  for (const auto& item : polyvec) {
    retpolys.append(vec2list(item));
  }
  return py::make_tuple(retpts, retpolys);
}

template <typename PolygonMesh>
auto refine(PolygonMesh& tmesh,
            const py::list& faces, // those could be other index types
            const py::dict& np = py::dict()) {
  using Gt = boost::graph_traits<PolygonMesh>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;
  auto faces_vec = list2vec<Fd>(faces);
  std::vector<Fd> faces_out;
  std::vector<Vd> vertices_out;
  PMP::refine(tmesh, faces_vec, std::back_inserter(faces_out), std::back_inserter(vertices_out),
              internal::parse_pmp_np<PolygonMesh>(np));
  return py::make_tuple(vec2list(faces_out), vec2list(vertices_out));
}

template <typename PolygonMesh>
auto stitch_borders_bc(const py::list& boundary_cycle_representatives,
                       PolygonMesh& pmesh,
                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;
  const auto bc_vec = list2vec<Hd>(boundary_cycle_representatives);
  // auto vpm = get_vertex_point_map(pmesh, np);
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP0",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    return PMP::stitch_borders(bc_vec, pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                        // .vertex_point_map(vpm)
                        .face_index_map(fim));
  } else {
    return PMP::stitch_borders(bc_vec, pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                        // .vertex_point_map(vpm)
                        );
  }
}

template <typename PolygonMesh>
auto stitch_borders_he(PolygonMesh& pmesh,
                       const py::list& hedge_pairs_to_stitch,
                       const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;
  std::vector<std::pair<Hd, Hd>> hedge_pairs;
  for (const auto& item : hedge_pairs_to_stitch) {
    std::pair<Hd, Hd> pair;
    pair.first = py::cast<Hd>(item[0]);
    pair.second = py::cast<Hd>(item[1]);
  }
  // auto vpm = get_vertex_point_map(pmesh, np);
  return PMP::stitch_borders(pmesh, hedge_pairs, internal::parse_pmp_np<PolygonMesh>(np)
                      // .vertex_point_map(vpm)
                      );
}



template <typename PolygonMesh>
auto stitch_borders(PolygonMesh& pmesh,
                    const py::dict& np = py::dict()) {
  // return PMP::stitch_borders(pmesh, internal::parse_pmp_np<PolygonMesh>(np));
  // auto vpm = get_vertex_point_map(pmesh, np);
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<PolygonMesh, std::size_t>(pmesh, "INTERNAL_MAP0",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    return PMP::stitch_borders(pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                        // .vertex_point_map(vpm)
                        .face_index_map(fim));
  } else {
    return PMP::stitch_borders(pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                        // .vertex_point_map(vpm)
                        );
  }
}

template <typename PolygonMesh>
auto stitch_boundary_cycle(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& h,
                           PolygonMesh& pmesh,
                           const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pmesh, np);
  return PMP::stitch_boundary_cycle(h, pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                      // .vertex_point_map(vpm)
                      );
}

template <typename PolygonMesh>
auto stitch_boundary_cycles(const py::list& boundary_cycle_representatives,
                            PolygonMesh& pmesh,
                            const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Hd = typename Gt::halfedge_descriptor;
  const auto bc_vec = list2vec<Hd>(boundary_cycle_representatives);
  // auto vpm = get_vertex_point_map(pmesh, np);
  return PMP::stitch_boundary_cycles(bc_vec, pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                      // .vertex_point_map(vpm)
                      );
}

template <typename TriangleMesh>
auto random_perturbation(TriangleMesh& tmesh,
                         const double& perturbation_max_size,
                         const py::dict& np = py::dict()) {
  auto vicm = get_vertex_prop_map<TriangleMesh, Point_3>
    (tmesh, "INTERNAL_MAP0", np.contains("vertex_point_map") ? np["vertex_point_map"] : py::none());
  PMP::random_perturbation(tmesh, perturbation_max_size,
                           internal::parse_pmp_np<TriangleMesh>(np)
                           .vertex_point_map(vicm));
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("vertex_point_map")) {
    tmesh.remove_property_map(vicm);
  }
#endif
}

template <typename PolygonMesh>
auto random_perturbation_v(const py::list& vertices,
                           PolygonMesh& pmesh,
                           const double& perturbation_max_size,
                           const py::dict& np = py::dict()) {
  using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  auto vicm = get_vertex_prop_map<PolygonMesh, Point_3>
    (pmesh, "INTERNAL_MAP0", np.contains("vertex_point_map") ? np["vertex_point_map"] : py::none());
  std::vector<Vd> vertices_vec = list2vec<Vd>(vertices);
  PMP::random_perturbation(vertices_vec, pmesh, perturbation_max_size,
                           internal::parse_pmp_np<PolygonMesh>(np)
                           .vertex_point_map(vicm));
}

template <typename PolygonMesh>
auto sample_triangle_mesh(const PolygonMesh& tm,
                          const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  PointRange pts;
  PMP::sample_triangle_mesh(tm, std::back_inserter(pts), internal::parse_pmp_np<PolygonMesh>(np)
                            // .vertex_point_map(vpm)
                            );
  return vec2list(pts);
}

auto sample_triangle_soup(const py::list& points,
                          const py::list& triangles,
                          const py::dict& np = py::dict()) {
  auto ptvec = list2vec<Point_3>(points);
  std::vector<std::array<std::size_t, 3>> trivec;
  for (const auto& item : triangles) {
    std::array<std::size_t, 3> tri;
    for (std::size_t i = 0; i < 3; ++i) {
      tri[i] = py::cast<std::size_t>(item[i]);
    }
  }
  PointRange pts;
  PMP::sample_triangle_soup(ptvec, trivec, std::back_inserter(pts), internal::parse_named_parameters(np));
  return vec2list(pts);
}

template <typename PolygonMesh>
Vector_3 compute_face_normal(const typename boost::graph_traits<PolygonMesh>::face_descriptor& f,
                             const PolygonMesh& sm,
                             const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(sm, np);
  return PMP::compute_face_normal(f, sm, internal::parse_pmp_np<PolygonMesh>(np)
                                  // .vertex_point_map(vpm)
                                  );
}

template <typename PolygonMesh, typename Face_normal_map>
auto compute_face_normals(const PolygonMesh& pmesh,
                               Face_normal_map face_normals,
                               const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  // auto vpm = get_vertex_point_map(pmesh, np);
  return PMP::compute_face_normals(pmesh, face_normals, internal::parse_pmp_np<Pm>(np)
                            .face_normal_map(face_normals)
                           // .vertex_point_map(vpm)
                           );
}

template <typename PolygonMesh>
Vector_3 compute_vertex_normal(const typename boost::graph_traits<PolygonMesh>::vertex_descriptor& v,
                               const PolygonMesh& sm,
                               const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(sm, np);
  return PMP::compute_vertex_normal(v, sm, internal::parse_pmp_np<PolygonMesh>(np)
                                    // .vertex_point_map(vpm)
                                    );
}

template <typename PolygonMesh, typename VertexNormalMap>
auto compute_vertex_normals(const PolygonMesh& pmesh,
                                 VertexNormalMap vertex_normals,
                                 const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(sm, np);
  return PMP::compute_vertex_normals(pmesh, vertex_normals, internal::parse_pmp_np<PolygonMesh>(np)
                              // .vertex_point_map(vpm)
                              );
}

template <typename TriangleMesh>
auto area(const TriangleMesh& tm,
          const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::area(tm, internal::parse_pmp_np<TriangleMesh>(np)
                   // .vertex_point_map(vpm)
                  );
}

template <typename TriangleMesh>
auto area_f(const py::list& face_range,
            const TriangleMesh& tm,
            const py::dict& np = py::dict()) {
  using Gt = boost::graph_traits<TriangleMesh>;
  using Fd = typename Gt::face_descriptor;
  auto facevec = list2vec<Fd>(face_range);
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::area(facevec, tm, internal::parse_pmp_np<TriangleMesh>(np)
                   // .vertex_point_map(vpm)
                  );
}

template <typename TriangleMesh>
auto centroid(const TriangleMesh& tm,
              const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::centroid(tm, internal::parse_pmp_np<TriangleMesh>(np)
                      // .vertex_point_map(vpm)
                      );
}

template <typename TriangleMesh>
auto edge_length(typename boost::graph_traits<TriangleMesh>::edge_descriptor& e,
                 const TriangleMesh& tm,
                 const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::edge_length(e, tm, internal::parse_pmp_np<TriangleMesh>(np)
                          // .vertex_point_map(vpm)
                          );
}

template <typename TriangleMesh>
auto face_area(typename boost::graph_traits<TriangleMesh>::face_descriptor& f,
               const TriangleMesh& tm,
               const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::face_area(f, tm, internal::parse_pmp_np<TriangleMesh>(np)
                        // .vertex_point_map(vpm)
                        );
}

template <typename TriangleMesh>
auto face_aspect_ratio(typename boost::graph_traits<TriangleMesh>::face_descriptor& f,
                       const TriangleMesh& tm,
                       const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::face_aspect_ratio(f, tm, internal::parse_pmp_np<TriangleMesh>(np)
                                // .vertex_point_map(vpm)
                                );
}

template <typename TriangleMesh>
auto face_border_length(typename boost::graph_traits<TriangleMesh>::halfedge_descriptor& h,
                        const TriangleMesh& tm,
                        const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::face_border_length(h, tm, internal::parse_pmp_np<TriangleMesh>(np)
                                // .vertex_point_map(vpm)
                                );
}

template <typename TriangleMesh>
auto longest_border(const TriangleMesh& tm,
                    const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  auto pair = PMP::longest_border(tm, internal::parse_pmp_np<TriangleMesh>(np)
                            // .vertex_point_map(vpm)
                            );
  return py::make_tuple(pair.first, pair.second);
}

template <typename TriangleMesh>
auto match_faces(const TriangleMesh& tm1,
                 const TriangleMesh& tm2,
                 const py::dict& np1 = py::dict(),
                 const py::dict& np2 = py::dict()) {
  // auto vpm1 = get_vertex_point_map(tm1, np1);
  // auto vpm2 = get_vertex_point_map(tm2, np2);
  using Tm = TriangleMesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = typename Gt::face_descriptor;
  std::vector<std::pair<Fd, Fd>> common;
  std::vector<Fd> m1_only, m2_only;
  PMP::match_faces(tm1, tm2, std::back_inserter(common), std::back_inserter(m1_only), std::back_inserter(m2_only),
                   internal::parse_pmp_np<TriangleMesh>(np1)
                   // .vertex_point_map(vpm1)
                   ,
                   internal::parse_pmp_np<TriangleMesh>(np2)
                   // .vertex_point_map(vpm2)
                   );
  // convert common to list of tuples
  py::list common_list;
  for (const auto& item : common) {
    common_list.append(py::make_tuple(item.first, item.second));
  }
  return py::make_tuple(common_list, vec2list(m1_only), vec2list(m2_only));
}

template <typename TriangleMesh>
auto squared_edge_length(typename boost::graph_traits<TriangleMesh>::edge_descriptor& e,
                         const TriangleMesh& tm,
                         const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::squared_edge_length(e, tm, internal::parse_pmp_np<TriangleMesh>(np)
                                  // .vertex_point_map(vpm)
                                  );
}

template <typename TriangleMesh>
auto squared_face_area(typename boost::graph_traits<TriangleMesh>::face_descriptor& f,
                       const TriangleMesh& tm,
                       const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::squared_face_area(f, tm, internal::parse_pmp_np<TriangleMesh>(np)
                                // .vertex_point_map(vpm)
                                );
}

template <typename TriangleMesh>
auto volume(const TriangleMesh& tm,
            const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::volume(tm, internal::parse_pmp_np<TriangleMesh>(np)
                    // .vertex_point_map(vpm)
                    );
}

template <typename TriangleMesh>
auto does_bound_a_volume(TriangleMesh& tm,
                         const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  bool retv;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<TriangleMesh, bool>
      (tm, "INTERNAL_MAP0", np.contains("face_index_map") ? np["face_index_map"] : py::none());
    retv = PMP::does_bound_a_volume(tm, internal::parse_pmp_np<TriangleMesh>(np)
                                    .face_index_map(fim)
                                  // .vertex_point_map(vpm)
                                  );
  }
  else {
    retv = PMP::does_bound_a_volume(tm, internal::parse_pmp_np<TriangleMesh>(np)
                                  // .vertex_point_map(vpm)
                                  );
  }
  return retv;
}

template<typename PolygonMesh, typename VertexNormalMap, typename FaceNormalMap>
auto compute_normals(const PolygonMesh& pm,
                     VertexNormalMap vnormals,
                     FaceNormalMap fnormals,
                     const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  // auto vpm = get_vertex_point_map(pm, np);
  return PMP::compute_normals(pm, vnormals, fnormals, internal::parse_pmp_np<Pm>(np)
                       // .vertex_point_map(vpm)
                       );

}

template <typename PolygonMesh>
void interpolated_corrected_curvatures(PolygonMesh& pmesh,
                                       const py::dict& np = py::dict()) {
  auto vmcm = get_vertex_prop_map<PolygonMesh, FT>
    (pmesh, "INTERNAL_MAP0", np.contains("vertex_mean_curvature_map") ? np["vertex_mean_curvature_map"] : py::none());
  auto vgcm = get_vertex_prop_map<PolygonMesh, FT>
    (pmesh, "INTERNAL_MAP1", np.contains("vertex_Gaussian_curvature_map") ? np["vertex_Gaussian_curvature_map"] : py::none());
  auto vpcdm = get_vertex_prop_map<PolygonMesh, PMP::Principal_curvatures_and_directions<Kernel>>
    (pmesh, "INTERNAL_MAP2", np.contains("vertex_principal_curvatures_and_directions_map") ? np["vertex_principal_curvatures_and_directions_map"] : py::none());


  if (np.contains("vertex_normal_map")) {
    auto vnm = get_vertex_prop_map<PolygonMesh, Vector_3>
      (pmesh, "INTERNAL_MAP3", np.contains("vertex_normal_map") ? np["vertex_normal_map"] : py::none());
    // auto vpmap = get_vertex_point_map(pm, np); // does not work
    PMP::interpolated_corrected_curvatures(pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                                           .vertex_mean_curvature_map(vmcm)
                                           .vertex_Gaussian_curvature_map(vgcm)
                                           .vertex_principal_curvatures_and_directions_map(vpcdm)
                                           .vertex_normal_map(vnm)
                                           );
#if CGALPY_PMP_POLYGONAL_MESH == 1
    if (np.contains("vertex_normal_map")) {
      pmesh.remove_property_map(vnm);
    }
#endif
  } else {
    PMP::interpolated_corrected_curvatures(pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                                           .vertex_mean_curvature_map(vmcm)
                                           .vertex_Gaussian_curvature_map(vgcm)
                                           .vertex_principal_curvatures_and_directions_map(vpcdm)
                                           );
  }
  // delete the internal maps
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("vertex_mean_curvature_map")) {
    pmesh.remove_property_map(vmcm);
  }
  if (!np.contains("vertex_Gaussian_curvature_map")) {
    pmesh.remove_property_map(vgcm);
  }
  if (!np.contains("vertex_principal_curvatures_and_directions_map")) {
    pmesh.remove_property_map(vpcdm);
  }
#endif
}

template <typename PolygonMesh>
auto interpolated_corrected_curvatures_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                                       PolygonMesh& pm,
                                       const py::dict& np = py::dict()) {
  py::object vmc = py::none();
  py::object vgc = py::none();

  auto pcad = PMP::Principal_curvatures_and_directions<Kernel>();

  double vmc_d, vGc;
  if (np.contains("vertex_normal_map")) {
    auto vnm = get_vertex_prop_map<PolygonMesh, Vector_3>
      (pm, "INTERNAL_MAP1", np.contains("vertex_normal_map") ? np["vertex_normal_map"] : py::none());
    PMP::interpolated_corrected_curvatures(v, pm,
    internal::parse_pmp_np<PolygonMesh>(np)
                                           .vertex_mean_curvature(std::ref(vmc_d))
                                           .vertex_Gaussian_curvature(std::ref(vGc))
                                           .vertex_principal_curvatures_and_directions(std::ref(pcad))
                                           .vertex_normal_map(vnm)
                                           );
  #if CGALPY_PMP_POLYGONAL_MESH == 1
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
  vmc = py::cast(vmc_d);
  vgc = py::cast(vGc);
  return py::make_tuple(vmc, vgc, pcad);
}

template <typename PolygonMesh>
auto border_halfedges(const py::list& face_range,
                      const PolygonMesh& pm,
                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Hd = typename Gt::halfedge_descriptor;

  auto begin = stl_input_iterator<Fd>(face_range);
  auto end = stl_input_iterator<Fd>(face_range, false);

  std::vector<Hd> out;
  PMP::border_halfedges(boost::make_iterator_range(begin, end), pm, std::back_inserter(out),
                        internal::parse_pmp_np<PolygonMesh>(np));
  py::list result;
  for (const auto& hd : out) result.append(hd);
  return result;
}

template <typename PolygonMesh>
void split(PolygonMesh& pm,
           PolygonMesh& splitter,
           const py::dict& np_tm = py::dict(),
           const py::dict& np_s = py::dict()) {
  // this can also have 2 visitors
  bool visitor1 = np_tm.contains("visitor");
  bool visitor2 = np_s.contains("visitor");
  if (visitor1 && visitor2) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
  } else if (visitor1) {
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_tm["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s));
    }
    catch (const py::cast_error& e) {
    }
  } else if (visitor2) {
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
  } else {
    PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm),
               internal::parse_pmp_np<PolygonMesh>(np_s));
  }
}

template <typename PolygonMesh>
auto split_long_edges(const py::list& edge_range,
                      double max_length,
                      PolygonMesh& pmesh,
                      const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Ed = typename Gt::edge_descriptor;
  // auto vpm = get_vertex_point_map(pmesh, np);
  auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP0",
    np.contains("face_index_map") ? np["face_patch_index_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>(pmesh, "INTERNAL_MAP1",
    np.contains("face_patch_map") ? np["face_patch_map"] : py::none());
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP2",
    np.contains("edge_index_copy_map") ? np["edge_index_copy_map"] : py::none());
  // turn edge_range to a vector
  std::vector<Ed> edge_vec;
  for (const auto& ed : edge_range) edge_vec.push_back(py::cast<Ed>(ed));
  return PMP::split_long_edges(edge_vec,
                        max_length, pmesh, internal::parse_pmp_np<PolygonMesh>(np)
                        // .vertex_point_map(vpm)
                        .face_index_map(fim)
                        .face_patch_map(fpm)
                        .edge_is_constrained_map(eicm));
}

py::list ptvec2ptlist(const std::vector<Point_3>& ptvec) {
  py::list ptlist;
  for (auto pt : ptvec) {
    ptlist.append(pt);
  }
  return ptlist;
}

std::vector<Point_3> ptlist2ptvec(const py::list& ptlist) {
  std::vector<Point_3> ptvec;
  ptvec.reserve(py::len(ptlist));
  for (auto pt : ptlist) {
    ptvec.push_back(py::cast<Point_3>(pt));
  }
  return ptvec;
}

auto polylist2polyvec(const py::list& polylist) {
  std::vector<std::vector<size_t>> polyvec;
  polyvec.reserve(py::len(polylist));
  for (auto poly : polylist) {
    std::vector<size_t> poly_ids;
    // poyl_ids.reserve(py::len(poly));
    for (auto polyid : poly) {
      size_t id = py::cast<size_t>(polyid);
      poly_ids.push_back(id);
    }
    polyvec.push_back(poly_ids);
  }
  return polyvec;
}

auto polyvec2polylist(const std::vector<std::vector<size_t>> polyvec) {
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

auto is_polygon_soup_a_polygon_mesh(const py::list& polygons) {
  std::vector<std::vector<std::size_t> > polys = polylist2polyvec(polygons);
  return PMP::is_polygon_soup_a_polygon_mesh(polys);
}

auto merge_duplicate_points_in_polygon_soup(const py::list& points,
                                            const py::list& polygons,
                                            const py::dict& np = py::dict()) {
  auto pointvec = ptlist2ptvec(points);
  auto polyvec = polylist2polyvec(polygons);
  return PMP::merge_duplicate_points_in_polygon_soup(pointvec, polyvec,
                                             internal::parse_named_parameters(np));
}

auto merge_duplicate_polygons_in_polygon_soup(const py::list& points,
                                              const py::list& polygons,
                                              const py::dict& np = py::dict()) {
  auto pointvec = ptlist2ptvec(points);
  auto polyvec = polylist2polyvec(polygons);
  return PMP::merge_duplicate_polygons_in_polygon_soup(pointvec, polyvec,
                                             internal::parse_named_parameters(np));
}

template <typename PolygonMesh>
auto merge_duplicated_vertices_in_boundary_cycle(typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& h,
                                                 PolygonMesh& pm,
                                                 const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
  return PMP::merge_duplicated_vertices_in_boundary_cycle(h, pm, internal::parse_pmp_np<PolygonMesh>(np)
                                                        // .vertex_point_map(vpm)
                                                        );
}

template <typename PolygonMesh>
auto merge_duplicated_vertices_in_boundary_cycles(PolygonMesh& pm,
                                                  const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(pm, np);
  return PMP::merge_duplicated_vertices_in_boundary_cycles(pm, internal::parse_pmp_np<PolygonMesh>(np)
                                                        // .vertex_point_map(vpm)
                                                        );
}

template <typename PolygonMesh>
auto non_manifold_vertices(PolygonMesh& pm) {
  using Hd = typename boost::graph_traits<PolygonMesh>::halfedge_descriptor;
  std::vector<Hd> out;
  PMP::non_manifold_vertices(pm, std::back_inserter(out));
  return vec2list(out);
}

template <typename PolygonMesh>
auto polygon_mesh_to_polygon_soup(const PolygonMesh& pm,
                                  const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Vd = typename Gt::vertex_descriptor;
  using Fd = typename Gt::face_descriptor;
  std::vector<Point_3> pts;
  std::vector<std::vector<std::size_t>> polys;
  PMP::polygon_mesh_to_polygon_soup(pm, pts, polys,
                                    internal::parse_pmp_np<Pm>(np));
  return py::make_tuple(ptvec2ptlist(pts), polyvec2polylist(polys));
}

template <typename PolygonMesh>
py::list orient_triangle_soup_with_reference_triangle_mesh(const PolygonMesh& tm_ref,
                                                       const py::list& points,
                                                       const py::list& triangles,
                                                       const py::dict& np1 = py::dict(),
                                                       const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;
  // auto vpm = get_vertex_point_map(tm_ref, np1);
  std::vector<Point_3> pts = ptlist2ptvec(points);
  auto polys = polylist2polyvec(triangles);
  PMP::orient_triangle_soup_with_reference_triangle_mesh<TAG>(tm_ref, pts, polys,
                                                       internal::parse_pmp_np<PolygonMesh>(np1)
                                                       // .vertex_point_map(vpm)
                                                       ,
                                                       internal::parse_pmp_np<PolygonMesh>(np2));
  return polyvec2polylist(polys);
}

auto orient_triangle_soup_with_reference_triangle_soup(const py::list& ref_points,
                                                       const py::list& ref_faces,
                                                       const py::list& points,
                                                       const py::list& faces,
                                                       const py::dict& np1 = py::dict(),
                                                       const py::dict& np2 = py::dict()) {
  // const std::vector<Point_3> ref_points_v = ptlist2ptvec(ref_points);
  // const std::vector<std::vector<std::size_t>> ref_faces_v = polylist2polyvec(ref_faces);
  // const std::vector<Point_3> pts_v = ptlist2ptvec(points);
  // const std::vector<std::vector<std::size_t>> faces_v = polylist2polyvec(faces);
  // PMP::orient_triangle_soup_with_reference_triangle_soup(ref_points_v, ref_faces_v, pts_v, faces_v); // doesn't work for some reason
}

py::tuple duplicate_non_manifold_edges_in_polygon_soup(const py::list& points,
                                                       const py::list& polygons) {
  auto pointvec = ptlist2ptvec(points);
  auto polyvec = polylist2polyvec(polygons);
  bool duplicated = PMP::duplicate_non_manifold_edges_in_polygon_soup(pointvec, polyvec);
  return py::make_tuple(polyvec2polylist(polyvec), ptvec2ptlist(pointvec), duplicated);
}

template <typename TriangleMesh>
auto is_outward_oriented(TriangleMesh& tm,
                         const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  return PMP::is_outward_oriented(tm, internal::parse_pmp_np<TriangleMesh>(np)
                                  // .vertex_point_map(vpm)
                                  );
}

template <typename PolygonMesh>
auto duplicate_non_manifold_vertices(PolygonMesh& pm,
                                     const py::dict& np = py::dict()) {
  using Vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  std::vector<std::vector<Vd>> out;
  // auto vpm = get_vertex_point_map(pm, np);
  auto vicm = get_vertex_prop_map<PolygonMesh, Vd>
    (pm, "INTERNAL_MAP0", np.contains("vertex_is_constrained_map") ? np["vertex_is_constrained_map"] : py::none());
  auto nb = PMP::duplicate_non_manifold_vertices(pm,
                                       internal::parse_pmp_np<PolygonMesh>(np)
                                       // .vertex_point_map(vpm)
                                       .output_iterator(std::back_inserter(out)));
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("vertex_is_constrained_map")) {
    pm.remove_property_map(vicm);
  }
#endif
  py::list result;
  for (const auto& vec : out) {
    result.append(vec2list(vec));
  }
  return py::make_tuple(nb, result);
}

template <typename PolygonMesh>
py::tuple polygon_soup_to_polygon_mesh(const py::list& points,
                                       const py::list& polygons,
                                       const py::dict& np_ps = py::dict(),
                                       const py::dict& np_pm = py::dict()) {
  using vd = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using fd = typename boost::graph_traits<PolygonMesh>::face_descriptor;
  auto ptvec = ptlist2ptvec(points);
  auto polyvec = polylist2polyvec(polygons);
  py::object pv = py::none(), pf = py::none();
  PolygonMesh output;

  // auto vpm2 = get_vertex_point_map(output, np_pm);

  std::vector<std::pair<int, vd>> pvvec;
  std::vector<std::pair<int, fd>> pfvec;
  PMP::polygon_soup_to_polygon_mesh(ptvec, polyvec, output,
                                    internal::parse_pmp_np<PolygonMesh>(np_ps)
                                    .point_to_vertex_output_iterator(std::back_inserter(pvvec))
                                    .polygon_to_face_output_iterator(std::back_inserter(pfvec)),
                                    internal::parse_pmp_np<PolygonMesh>(np_pm)
                                    // .vertex_point_map(vpm2)
                                    );
  py::list pvlist, pflist;
  for (const auto& [p, v] : pvvec) {
    pvlist.append(py::make_tuple(p, v));
  }
  for (const auto& [p, f] : pfvec) {
    pflist.append(py::make_tuple(p, f));
  }
  return py::make_tuple(output, pvlist, pflist);
}

auto remove_isolated_points_in_polygon_soup(const py::list& points,
                                            const py::list& polygons) {
  auto pointvec = ptlist2ptvec(points);
  auto polyvec = polylist2polyvec(polygons);
  return PMP::remove_isolated_points_in_polygon_soup(pointvec, polyvec);
}

template <typename PolygonMesh, typename EBMap>
void detect_sharp_edges(PolygonMesh& pmesh,
                        double angle_in_deg,
                        EBMap ebmap,
                        const py::dict& np = py::dict()) {
  if (np.contains("vertex_face_degree_map")) {
    auto vfdm = get_vertex_prop_map<PolygonMesh, int>
      (pmesh, "INTERNAL_MAP0", np.contains("vertex_face_degree_map") ? np["vertex_face_degree_map"] : py::none());
    PMP::detect_sharp_edges(pmesh, angle_in_deg, ebmap, internal::parse_pmp_np<PolygonMesh>(np)
                            .vertex_feature_degree_map(vfdm)
                            );
#if CGALPY_PMP_POLYGONAL_MESH == 1
    pmesh.remove_property_map(vfdm);
#endif
  } else {
    PMP::detect_sharp_edges(pmesh, angle_in_deg, ebmap, internal::parse_pmp_np<PolygonMesh>(np));
  }
}

using Pm = Polygonal_mesh;
using Gt = boost::graph_traits<Polygonal_mesh>;
using Vd = typename Gt::vertex_descriptor;
using Hd = typename Gt::halfedge_descriptor;
using Fd = typename Gt::face_descriptor;

template <typename PolygonMesh, typename RegionMap>
auto region_growing_of_planes_on_faces(PolygonMesh& pmesh,
                                       RegionMap region_map,
                                       const py::dict& np = py::dict()) {
  std::size_t num_regions;

  if (np.contains("region_primitive_map")) {
    auto rpm = py::cast<boost::vector_property_map<Vector_3>>(np["region_primitive_map"]);
    num_regions = PMP::region_growing_of_planes_on_faces(pmesh, region_map,
                                                         internal::parse_pmp_np<PolygonMesh>(np)
                                                         .region_primitive_map(rpm));
  }
  else {
    num_regions = PMP::region_growing_of_planes_on_faces(pmesh, region_map,
                                                         internal::parse_pmp_np<PolygonMesh>(np));
  }
  return num_regions;
}

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
                                                 internal::parse_pmp_np<PolygonMesh>(np)
                                                 .edge_is_constrained_map(eicm));
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) {
    pmesh.remove_property_map(eicm);
  }
#endif
  return r;
}

template <typename PolygonMesh, typename FacePatchMap, typename VertexCornerMap, typename EdgeIsConstrainedMap>
auto remesh_almost_planar_patches(PolygonMesh tm_in,
                                  std::size_t nb_patches,
                                  std::size_t nb_corners,
                                  FacePatchMap face_patch_map,
                                  VertexCornerMap vertex_corner_map,
                                  EdgeIsConstrainedMap ecm,
                                  const py::dict& np_in = py::dict(),
                                  const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;

  Pm pm_out;

  auto fpm = get_face_prop_map<Pm, int>(pm_out, "INTERNAL_MAP1",
    np_out.contains("face_patch_map") ? np_out["face_patch_map"] : py::none());
  auto vcm = get_vertex_prop_map<Pm, int>(pm_out, "INTERNAL_MAP2",
    np_out.contains("vertex_corner_map") ? np_out["vertex_corner_map"] : py::none());

  bool retv;
  if (np_in.contains("patch_normal_map")) {
    auto rpm = py::cast<boost::vector_property_map<Vector_3>>(np_in["patch_normal_map"]);
    retv = PMP::remesh_almost_planar_patches(tm_in, pm_out, nb_patches, nb_corners, face_patch_map, vertex_corner_map, ecm,
                                             internal::parse_pmp_np<PolygonMesh>(np_in)
                                             .patch_normal_map(rpm),
                                             internal::parse_pmp_np<PolygonMesh>(np_out)
                                             .face_patch_map(fpm)
                                             .vertex_corner_map(vcm));
  }
  else {
    retv = PMP::remesh_almost_planar_patches(tm_in, pm_out, nb_patches, nb_corners, face_patch_map, vertex_corner_map, ecm,
                                             internal::parse_pmp_np<PolygonMesh>(np_in),
                                             internal::parse_pmp_np<PolygonMesh>(np_out)
                                             .face_patch_map(fpm)
                                             .vertex_corner_map(vcm));
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np_out.contains("face_patch_map")) {
    pm_out.remove_property_map(fpm);
  }
  if (!np_out.contains("vertex_corner_map")) {
    pm_out.remove_property_map(vcm);
  }
#endif
  if (!retv) throw std::runtime_error("Remeshing almost planar patches failed!");
  return pm_out;
}

template <typename PolygonMesh>
auto angle_and_area_smoothing(const py::list& faces,
                              PolygonMesh& pmesh,
                              const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  auto faces_vec = list2vec<Fd>(faces);
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP1",
    np.contains("vertex_is_constrained_map") ? np["vertex_is_constrained_map"] : py::none());
  PMP::angle_and_area_smoothing(faces_vec, pmesh,
                                internal::parse_pmp_np<PolygonMesh>(np)
                                .edge_is_constrained_map(eicm)
                                .vertex_is_constrained_map(vicm));
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) {
    pmesh.remove_property_map(eicm);
  }
  if (!np.contains("vertex_is_constrained_map")) {
    pmesh.remove_property_map(vicm);
  }
#endif
}

template <typename PolygonMesh, typename FaceBitMap>
auto compatible_orientations(PolygonMesh& pm,
                             FaceBitMap fbm,
                             const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto fpim = get_face_prop_map<Pm, std::size_t>(pm, "INTERNAL_MAP0",
    np.contains("face_patch_index_map") ? np["face_patch_index_map"] : py::none());

  auto retv = PMP::compatible_orientations(pm, fbm,
                                          internal::parse_pmp_np<PolygonMesh>(np)
                                          .face_partition_id_map(fpim));
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("face_patch_index_map")) {
    pm.remove_property_map(fpim);
  }
#endif
  return retv;
}

template <typename PolygonMesh>
auto surface_Delaunay_remeshing(PolygonMesh& tmesh,
                                const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(tmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
  auto fpm = get_face_prop_map<Pm, int>(tmesh, "INTERNAL_MAP1",
    np.contains("face_patch_map") ? np["face_patch_map"] : py::none());
  // auto vpm = get_vertex_point_map(tmesh, np);


  auto mfs = np.contains("mesh_facet_size") ? py::cast<FT>(np["mesh_facet_size"]) : 0;
  auto mfa = np.contains("mesh_facet_angle") ? py::cast<FT>(np["mesh_facet_angle"]) : 0;
  auto mfd = np.contains("mesh_facet_distance") ? py::cast<FT>(np["mesh_facet_distance"]) : 0;
  PolygonMesh retv;

  if (np.contains("mesh_edge_size")) {
    auto mes = py::cast<CGAL::Mesh_constant_domain_field_3<Kernel, int>>(np["mesh_edge_size"]);
    retv = PMP::surface_Delaunay_remeshing(tmesh,
                                           internal::parse_pmp_np<PolygonMesh>(np)
                                           .edge_is_constrained_map(eicm)
                                           .face_patch_map(fpm)
                                           .facet_size(mfs)
                                           .facet_angle(mfa)
                                           .facet_distance(mfd)
                                           .mesh_edge_size(mes)
                                           // .vertex_point_map(vpm)
                                           );
  }
  else {
    retv = PMP::surface_Delaunay_remeshing(tmesh,
                                           internal::parse_pmp_np<PolygonMesh>(np)
                                           .edge_is_constrained_map(eicm)
                                           .face_patch_map(fpm)
                                           .facet_size(mfs)
                                           .facet_angle(mfa)
                                           .facet_distance(mfd)
                                           // .vertex_point_map(vpm)
                                           );
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) {
    tmesh.remove_property_map(eicm);
  }
  if (!np.contains("face_patch_map")) {
    tmesh.remove_property_map(fpm);
  }
#endif
  return retv;
}

template<typename PolygonMesh, typename EdgeIsFeatureMap, typename PatchIdMap>
typename boost::graph_traits< PolygonMesh >::faces_size_type sharp_edges_segmentation
                              (PolygonMesh & pmesh,
                              FT angle_in_deg,
                              EdgeIsFeatureMap edge_is_feature_map,
                              PatchIdMap patch_id_map,
                              const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  using faces_size_type = typename boost::graph_traits< PolygonMesh >::faces_size_type;

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
                                                internal::parse_pmp_np<PolygonMesh>(np)
                                                .vertex_feature_degree_map(vfdm)
                                                .face_index_map(fim)
                                                .vertex_incident_patches_map(vipm));
  } else if (fimap) {
    auto fim = get_face_prop_map<Pm, std::size_t>(pmesh, "INTERNAL_MAP1",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    num_patches = PMP::sharp_edges_segmentation(pmesh, angle_in_deg, edge_is_feature_map, patch_id_map,
                                                internal::parse_pmp_np<PolygonMesh>(np)
                                                .vertex_feature_degree_map(vfdm)
                                                .face_index_map(fim));
  } else if (vimap) {
    auto vipm = get_vertex_prop_map<Pm, std::set<int>>(pmesh, "INTERNAL_MAP2",
      np.contains("vertex_index_map") ? np["vertex_index_map"] : py::none());
    num_patches = PMP::sharp_edges_segmentation(pmesh, angle_in_deg, edge_is_feature_map, patch_id_map,
                                                internal::parse_pmp_np<PolygonMesh>(np)
                                                .vertex_feature_degree_map(vfdm)
                                                .vertex_incident_patches_map(vipm));
  } else {
    num_patches = PMP::sharp_edges_segmentation(pmesh, angle_in_deg, edge_is_feature_map, patch_id_map,
                                                internal::parse_pmp_np<PolygonMesh>(np)
                                                .vertex_feature_degree_map(vfdm));
  }
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("vertex_face_degree_map")) {
    pmesh.remove_property_map(vfdm);
  }
#endif
  return num_patches;
}

template <typename PolygonMesh>
auto angle_and_area_smoothing_m(PolygonMesh& pmesh,
                              const py::dict& np = py::dict()) {
  using Pm = PolygonMesh;
  auto eicm = get_edge_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP0",
    np.contains("edge_is_constrained_map") ? np["edge_is_constrained_map"] : py::none());
  auto vicm = get_vertex_prop_map<Pm, bool>(pmesh, "INTERNAL_MAP1",
    np.contains("vertex_is_constrained_map") ? np["vertex_is_constrained_map"] : py::none());
  PMP::angle_and_area_smoothing(pmesh,
                                internal::parse_pmp_np<PolygonMesh>(np)
                                .edge_is_constrained_map(eicm)
                                .vertex_is_constrained_map(vicm));
#if CGALPY_PMP_POLYGONAL_MESH == 1
  if (!np.contains("edge_is_constrained_map")) {
    pmesh.remove_property_map(eicm);
  }
  if (!np.contains("vertex_is_constrained_map")) {
    pmesh.remove_property_map(vicm);
  }
#endif
}

// template <typename InputMesh, typename OutputMesh, typename BottomFunctor, typename TopFunctor>
// auto extrude_mesh(const InputMesh& imesh,
//                   OutputMesh& omesh,
//                   BottomFunctor bottom,
//                   TopFunctor top,
//                   const py::dict& np = py::dict()) {
//   using Im = InputMesh;
//   using Om = OutputMesh;
//   PMP::extrude_mesh(imesh, omesh, bottom, top,
//                     internal::parse_pmp_np<Om>(np),
//                     internal::parse_pmp_np<Om>(np));
// }

template <typename InputMesh, typename OutputMesh>
auto extrude_mesh_v(const InputMesh& imesh,
                  OutputMesh& omesh,
                  Vector_3 v,
                  const py::dict& np_in = py::dict(),
                  const py::dict& np_out = py::dict()) {
  PMP::extrude_mesh(imesh, omesh, v,
                    internal::parse_pmp_np<OutputMesh>(np_in),
                    internal::parse_pmp_np<OutputMesh>(np_out));
}

// only for polyhedron!
template <typename TriangleMesh>
auto fair(TriangleMesh& tmesh,
          const py::list& vertices,
          const py::dict& np = py::dict()) {
  using Tm = TriangleMesh;
  using Vh = typename TriangleMesh::Vertex_handle;
  std::vector<Vh> vvec = list2vec<Vd>(vertices);
  return PMP::fair(tmesh, vvec, internal::parse_pmp_np<Tm>(np));
}


template <typename PolygonMesh>
auto reverse_face_orientations(const py::list& face_range,
                               PolygonMesh& pm)
{
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  auto begin = stl_input_iterator<Fd>(face_range);
  auto end = stl_input_iterator<Fd>(face_range, false);

  std::vector<Fd> out;
  PMP::reverse_face_orientations(boost::make_iterator_range(begin, end), pm);
}

template <typename TriangleMesh, typename VolumeFaceIndexMap>
auto volume_connected_components(TriangleMesh& tm,
                                 VolumeFaceIndexMap volume_id_map,
                                 const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  std::vector<std::size_t> ccitvi, nl;
  std::vector<bool> icoo;
  std::vector<std::vector<std::size_t>> vi;
  std::vector<std::pair<std::size_t, std::size_t>> ivpot;
  std::size_t retv;
  if (np.contains("face_index_map")) {
    auto fim = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP0",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    if (np.contains("face_connected_component_map")) {
      auto fccm = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP1",
        np.contains("face_connected_component_map") ? np["face_connected_component_map"] : py::none());
        retv = PMP::volume_connected_components(tm, volume_id_map, internal::parse_pmp_np<TriangleMesh>(np)
                                        // .vertex_point_map(vpm)
                                        .face_index_map(fim)
                                        .face_connected_component_map(fccm)
                                        .volume_inclusions(std::ref(vi))
                                        .connected_component_id_to_volume_id(std::ref(ccitvi))
                                        .nesting_levels(std::ref(nl))
                                        .is_cc_outward_oriented(std::ref(icoo))
                                        .intersecting_volume_pairs_output_iterator(std::back_inserter(ivpot))
                                        );
    }
    else {
      retv = PMP::volume_connected_components(tm, volume_id_map, internal::parse_pmp_np<TriangleMesh>(np)
                                      // .vertex_point_map(vpm)
                                      .face_index_map(fim)
                                      .volume_inclusions(std::ref(vi))
                                      .connected_component_id_to_volume_id(std::ref(ccitvi))
                                      .nesting_levels(std::ref(nl))
                                      .is_cc_outward_oriented(std::ref(icoo))
                                      .intersecting_volume_pairs_output_iterator(std::back_inserter(ivpot))
                                      );
    }
  }
  else {
    if (np.contains("face_connected_component_map")) {
      auto fccm = get_face_prop_map<TriangleMesh, std::size_t>(tm, "INTERNAL_MAP1",
        np.contains("face_connected_component_map") ? np["face_connected_component_map"] : py::none());
      retv = PMP::volume_connected_components(tm, volume_id_map, internal::parse_pmp_np<TriangleMesh>(np)
                                      // .vertex_point_map(vpm)
                                      .face_connected_component_map(fccm)
                                      .volume_inclusions(std::ref(vi))
                                      .connected_component_id_to_volume_id(std::ref(ccitvi))
                                      .nesting_levels(std::ref(nl))
                                      .is_cc_outward_oriented(std::ref(icoo))
                                      .intersecting_volume_pairs_output_iterator(std::back_inserter(ivpot))
                                      );
    }
    else {
      retv = PMP::volume_connected_components(tm, volume_id_map, internal::parse_pmp_np<TriangleMesh>(np)
                                      // .vertex_point_map(vpm)
                                      .volume_inclusions(std::ref(vi))
                                      .connected_component_id_to_volume_id(std::ref(ccitvi))
                                      .nesting_levels(std::ref(nl))
                                      .is_cc_outward_oriented(std::ref(icoo))
                                      .intersecting_volume_pairs_output_iterator(std::back_inserter(ivpot))
                                      );
    }
  }
  py::list ccitvi_list, nl_list, ico_list;
  ccitvi_list = vec2list(ccitvi);
  nl_list = vec2list(nl);
  ico_list = vec2list(icoo);
  py::list vi_list;
  for (const auto& v : vi) {
    vi_list.append(vec2list(v));
  }
  py::list ivpot_list;
  for (const auto& [i, j] : ivpot) {
    ivpot_list.append(py::make_tuple(i, j));
  }
  return py::make_tuple(retv, ccitvi_list, nl_list, ico_list, vi_list, ivpot_list);
}

template <typename PolygonMesh>
void orient_to_bound_a_volume(PolygonMesh& tm,
                              const py::dict& np = py::dict()) {
  // auto vpm = get_vertex_point_map(tm, np);
  if(np.contains("face_index_map")) {
    auto fim = get_face_prop_map<PolygonMesh, std::size_t>(tm, "INTERNAL_MAP0",
      np.contains("face_index_map") ? np["face_index_map"] : py::none());
    PMP::orient_to_bound_a_volume(tm, internal::parse_pmp_np<PolygonMesh>(np)
                                  // .vertex_point_map(vpm)
                                  .face_index_map(fim));
  } else {
    PMP::orient_to_bound_a_volume(tm, internal::parse_pmp_np<PolygonMesh>(np)
                                  // .vertex_point_map(vpm)
                                  );
  }
}


// visitor stuff
void set_polygon_orientation_reversed(Default_orientation_visitor& v,
                           const std::function<void(std::size_t)>& f){
  v.set_polygon_orientation_reversed(f);
}

void set_vertex_id_in_polygon_replaced(Default_orientation_visitor& v,
                            const std::function<void(std::size_t, std::size_t, std::size_t)>& f) {
  v.set_vertex_id_in_polygon_replaced(f);
}

void set_duplicated_vertex(Default_orientation_visitor& v,
                           const std::function<void(std::size_t, std::size_t)>& f) {
  v.set_duplicated_vertex(f);
}

void set_non_manifold_edge(Default_orientation_visitor& v,
                           const std::function<void(std::size_t, std::size_t, std::size_t)>& f) {
  v.set_non_manifold_edge(f);
}

void set_non_manifold_vertex(Default_orientation_visitor& v,
                             const std::function<void(std::size_t, std::size_t)>& f) {
  v.set_non_manifold_vertex(f);
}

void set_link_connected_polygons(Default_orientation_visitor& v,
                                 const std::function<void(std::size_t, py::list)>& f) {
  v.set_link_connected_polygons(f);
}
// using Boolean_operation_type = COREFINEMENT::Boolean_operation_type;
using Boolean_operation_type = std::size_t;
using Cv = Corefine_visitor<Pm>;
void set_before_subface_creations_fn(Cv& v,
                                     const std::function<void(Fd, Pm&)>& f) {
  v.set_before_subface_creations(f);
}
void set_after_subface_creations_fn(Cv& v,
                                    const std::function<void(Pm&)>& f) {
  v.set_after_subface_creations(f);
}
void set_before_subface_created_fn(Cv& v,
                                   const std::function<void(Pm&)>& f) {
  v.set_before_subface_created(f);
}
void set_after_subface_created_fn(Cv& v,
                                  const std::function<void(Fd, Pm&)>& f) {
  v.set_after_subface_created(f);
}
void set_before_face_copy_fn(Cv& v,
                             const std::function<void(Fd, const Pm&, Pm&)>& f) {
  v.set_before_face_copy(f);
}
void set_after_face_copy_fn(Cv& v,
                            const std::function<void(Fd, const Pm&, Fd, Pm&)>& f) {
  v.set_after_face_copy(f);
}
void set_before_edge_split_fn(Cv& v,
                              const std::function<void(Hd, Pm&)>& f) {
  v.set_before_edge_split(f);
}
void set_edge_split_fn(Cv& v,
                       const std::function<void(Hd, Pm&)>& f) {
  v.set_edge_split(f);
}
void set_after_edge_split_fn(Cv& v,
                             const std::function<void()>& f) {
  v.set_after_edge_split(f);
}
void set_add_retriangulation_edge_fn(Cv& v,
                                     const std::function<void(Hd, Pm&)>& f) {
  v.set_add_retriangulation_edge(f);
}
void set_before_edge_copy_fn(Cv& v,
                             const std::function<void(Hd, const Pm&, Pm&)>& f) {
  v.set_before_edge_copy(f);
}
void set_after_edge_copy_fn(Cv& v,
                            const std::function<void(Hd, const Pm&, Hd, Pm&)>& f) {
  v.set_after_edge_copy(f);
}
void set_before_edge_duplicated_fn(Cv& v,
                                   const std::function<void(Hd, Pm&)>& f) {
  v.set_before_edge_duplicated(f);
}
void set_after_edge_duplicated_fn(Cv& v,
                                  const std::function<void(Hd, Hd, Pm&)>& f) {
  v.set_after_edge_duplicated(f);
}
void set_intersection_edge_copy_fn(Cv& v,
                                   const std::function<void(Hd, const Pm&, Hd, const Pm&, Hd, Pm&)>& f) {
  v.set_intersection_edge_copy(f);
}
void set_new_vertex_added_fn(Cv& v,
                             const std::function<void(std::size_t, Vd, const Pm&)>& f) {
  v.set_new_vertex_added(f);
}
void set_intersection_point_detected_fn(Cv& v,
                                        const std::function<void(std::size_t, int, Hd, Hd, const Pm&, const Pm&, bool, bool)>& f) {
  v.set_intersection_point_detected(f);
}
void set_before_vertex_copy_fn(Cv& v,
                               const std::function<void(Vd, const Pm&, Pm&)>& f) {
  v.set_before_vertex_copy(f);
}
void set_after_vertex_copy_fn(Cv& v,
                              const std::function<void(Vd, const Pm&, Vd, Pm&)>& f) {
  v.set_after_vertex_copy(f);
}
void set_start_filtering_intersections_fn(Cv& v,
                                          const std::function<void()>& f) {
  v.set_start_filtering_intersections(f);
}
void set_progress_filtering_intersections_fn(Cv& v,
                                             const std::function<void(double)>& f) {
  v.set_progress_filtering_intersections(f);
}
void set_end_filtering_intersections_fn(Cv& v,
                                        const std::function<void()>& f) {
  v.set_end_filtering_intersections(f);
}
void set_start_triangulating_faces_fn(Cv& v,
                                      const std::function<void(std::size_t)>& f) {
  v.set_start_triangulating_faces(f);
}
void set_triangulating_faces_step_fn(Cv& v,
                                     const std::function<void()>& f) {
  v.set_triangulating_faces_step(f);
}
void set_end_triangulating_faces_fn(Cv& v,
                                    const std::function<void()>& f) {
  v.set_end_triangulating_faces(f);
}
void set_start_handling_intersection_of_coplanar_faces_fn(Cv& v,
                                                          const std::function<void(std::size_t)>& f) {
  v.set_start_handling_intersection_of_coplanar_faces(f);
}
void set_intersection_of_coplanar_faces_step_fn(Cv& v,
                                                const std::function<void()>& f) {
  v.set_intersection_of_coplanar_faces_step(f);
}
void set_end_handling_intersection_of_coplanar_faces_fn(Cv& v,
                                                        const std::function<void()>& f) {
  v.set_end_handling_intersection_of_coplanar_faces(f);
}
void set_start_handling_edge_face_intersections_fn(Cv& v,
                                                   const std::function<void(std::size_t)>& f) {
  v.set_start_handling_edge_face_intersections(f);
}
void set_edge_face_intersections_step_fn(Cv& v,
                                         const std::function<void()>& f) {
  v.set_edge_face_intersections_step(f);
}
void set_end_handling_edge_face_intersections_fn(Cv& v,
                                                const std::function<void()>& f) {
  v.set_end_handling_edge_face_intersections(f);
}
void set_start_building_output_fn(Cv& v,
                                  const std::function<void()>& f) {
  v.set_start_building_output(f);
}
void set_end_building_output_fn(Cv& v,
                                const std::function<void()>& f) {
  v.set_end_building_output(f);
}
void set_filter_coplanar_edges_fn(Cv& v,
                                  const std::function<void()>& f) {
  v.set_filter_coplanar_edges(f);
}
void set_detect_patches_fn(Cv& v,
                           const std::function<void()>& f) {
  v.set_detect_patches(f);
}
void set_classify_patches_fn(Cv& v,
                             const std::function<void()>& f) {
  v.set_classify_patches(f);
}
void set_classify_intersection_free_patches_fn(Cv& v,
                                               const std::function<void(const Pm&)>& f) {
  v.set_classify_intersection_free_patches(f);
}
void set_out_of_place_operation_fn(Cv& v,
                                   const std::function<void(Boolean_operation_type)>& f) {
  v.set_out_of_place_operation(f);
}
void set_in_place_operation_fn(Cv& v,
                               const std::function<void(Boolean_operation_type)>& f) {
  v.set_in_place_operation(f);
}
void set_in_place_operations_fn(Cv& v,
                               const std::function<void(Boolean_operation_type, Boolean_operation_type)>& f) {
  v.set_in_place_operations(f);
}

// HFDefault_visitor
void set_start_planar_phase(HFDefault_visitor& v,
                            const std::function<void()>& f) {
  v.set_start_planar_phase(f);
}
void set_end_planar_phase(HFDefault_visitor& v,
                          const std::function<void(bool)>& f) {
  v.set_end_planar_phase(f);
}
void set_start_quadratic_phase(HFDefault_visitor& v,
                               const std::function<void(std::size_t)>& f) {
  v.set_start_quadratic_phase(f);
}
void set_quadratic_step(HFDefault_visitor& v,
                        const std::function<void()>& f) {
  v.set_quadratic_step(f);
}
void set_end_quadratic_phase(HFDefault_visitor& v,
                             const std::function<void(bool)>& f) {
  v.set_end_quadratic_phase(f);
}
void set_start_cubic_phase(HFDefault_visitor& v,
                           const std::function<void(std::size_t)>& f) {
  v.set_start_cubic_phase(f);
}
void set_cubic_step(HFDefault_visitor& v,
                    const std::function<void()>& f) {
  v.set_cubic_step(f);
}
void set_end_cubic_phase(HFDefault_visitor& v,
                         const std::function<void()>& f) {
  v.set_end_cubic_phase(f);
}
void set_start_refine_phase(HFDefault_visitor& v,
                            const std::function<void()>& f) {
  v.set_start_refine_phase(f);
}
void set_end_refine_phase(HFDefault_visitor& v,
                          const std::function<void()>& f) {
  v.set_end_refine_phase(f);
}
void set_start_fair_phase(HFDefault_visitor& v,
                          const std::function<void()>& f) {
  v.set_start_fair_phase(f);
}
void set_end_fair_phase(HFDefault_visitor& v,
                        const std::function<void()>& f) {
  v.set_end_fair_phase(f);
}

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

#if CGALPY_PMP_POLYGONAL_MESH == 1
  using edge_bool_map = Pm::Property_map<Ed, bool>;
  using RegionMap = Pm::Property_map<Fd, std::size_t>;
  using CornerIdMap = Pm::Property_map<Vd, std::size_t>;
  using FacePatchMap = Pm::Property_map<Fd, std::size_t>;
  using FaceSizeTypeMap = Pm::Property_map<Fd, faces_size_type>;
  using VertexCornerMap = Pm::Property_map<Vd, std::size_t>;
  using EdgeIsConstrainedMap = Pm::Property_map<Ed, bool>;
  using FaceBitMap = Pm::Property_map<Fd, bool>;
  using FaceComponentMap = Pm::Property_map<Fd, faces_size_type>;
  using FaceVectorMap = Pm::Property_map<Fd, Vector_3>;
  using VertexVectorMap = Pm::Property_map<Vd, Vector_3>;
#endif
#if CGALPY_PMP_POLYGONAL_MESH == 0
  using edge_bool_map = boost::property_map<Pm, CGAL::dynamic_edge_property_t<bool>>::type;
  using RegionMap = boost::property_map<Pm, CGAL::dynamic_face_property_t<std::size_t>>;
  using CornerIdMap = boost::property_map<Pm, CGAL::dynamic_vertex_property_t<std::size_t>>;
  using FacePatchMap = boost::property_map<Pm, CGAL::dynamic_face_property_t<std::size_t>>;
  using FaceSizeTypeMap = boost::property_map<Pm, CGAL::dynamic_face_property_t<faces_size_type>>;
  using VertexCornerMap = boost::property_map<Pm, CGAL::dynamic_vertex_property_t<std::size_t>>;
  using EdgeIsConstrainedMap = boost::property_map<Pm, CGAL::dynamic_edge_property_t<bool>>::type;
  using FaceBitMap = boost::property_map<Pm, CGAL::dynamic_face_property_t<bool>>;
  using FaceComponentMap = boost::property_map<Pm, CGAL::dynamic_face_property_t<faces_size_type>>;
  using FaceVectorMap = boost::property_map<Pm, CGAL::dynamic_face_property_t<Vector_3>>;
  using VertexVectorMap = boost::property_map<Pm, CGAL::dynamic_vertex_property_t<Vector_3>>;
#endif

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Np_class>(m, "Named_function_parameters")
    .def(py::init<>());

#if 0
  // The following fails because the 5th parameter is a reference to void,
  // which cannot be handled by nanobind.
  using Has_range = boost::has_range_const_iterator<Pm>::value;
  using Do_intersect_meshes = bool(*)(const Pm&, const Pm&,
                                      const Np_class&, const Np_class&,
                                      const std::enable_if_t<! Has_range>*);

  m.def("do_intersect", static_cast<Do_intersect1>(&PMP::do_intersect<Pm>));
#endif // 0

  // Connected Components
  m.def("connected_component", &pmp::connected_component<Pm>,
        py::arg("seed_face"), py::arg("pm"),
        py::arg("np") = py::dict());
  m.def("connected_components", &pmp::connected_components_map<Pm, boost::property_map<Pm, CGAL::dynamic_face_property_t<std::size_t>>::type>,
        py::arg("pm"), py::arg("fcm"), py::arg("np") = py::dict());
  m.def("connected_components", &pmp::connected_components_map<Pm, boost::property_map<Pm, CGAL::dynamic_face_property_t<std::uint32_t>>::type>,
        py::arg("pm"), py::arg("fcm"), py::arg("np") = py::dict());
  m.def("keep_connected_components", &pmp::keep_connected_components<Pm>,
        py::arg("pm"), py::arg("components_to_keep"), py::arg("np") = py::dict());
  m.def("keep_large_connected_components", &pmp::keep_large_connected_components<Pm, std::size_t>,
        py::arg("pm"), py::arg("min_size"), py::arg("np") = py::dict());
  m.def("keep_large_connected_components", &pmp::keep_large_connected_components<Pm, double>,
        py::arg("pm"), py::arg("min_size"), py::arg("np") = py::dict());
  m.def("keep_largest_connected_components", &pmp::keep_largest_connected_components<Pm>,
        py::arg("pm"), py::arg("nb_components_to_keep"), py::arg("np") = py::dict());
  m.def("detect_sharp_edges", &pmp::detect_sharp_edges<Pm, edge_bool_map>,
        py::arg("pm"), py::arg("angle_in_deg"), py::arg("edge_is_feature_map"),
        py::arg("np") = py::dict());
  m.def("remove_connected_components", &pmp::remove_connected_components<Pm>,
        py::arg("pm"), py::arg("components_to_remove"), py::arg("np") = py::dict());
  m.def("split_connected_components", &pmp::split_connected_components<Pm>,
        py::arg("pmesh"), py::arg("cc_meshes"), py::arg("np") = py::dict());


  // Hole Filling
  m.def("triangulate_and_refine_hole", &pmp::triangulate_and_refine_hole<Pm>,
        py::arg("pm"), py::arg("hole_boundary"),
        py::arg("np") = py::dict());
  m.def("triangulate_hole", &pmp::triangulate_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"),
        py::arg("np") = py::dict());
  m.def("triangulate_hole_polyline", &pmp::triangulate_hole_polyline<Pm>,
        py::arg("points"), py::arg("third_points") = py::list(),
        py::arg("np") = py::dict());
  m.def("triangulate_refine_and_fair_hole", &pmp::triangulate_refine_and_fair_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"),
        py::arg("np") = py::dict());

  // Meshing
  m.def("angle_and_area_smoothing", &pmp::angle_and_area_smoothing<Pm>,
        py::arg("faces"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("angle_and_area_smoothing", &pmp::angle_and_area_smoothing_m<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());
  // m.def("extrude_mesh", &pmp::extrude_mesh<Pm, Pm, 
  //       py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("extrude_mesh", &pmp::extrude_mesh_v<Pm, Pm>,
        py::arg("imesh"), py::arg("omesh"), py::arg("v"),
        py::arg("np_in") = py::dict(), py::arg("np_out") = py::dict());
  // fair only for polyhedron
  // m.def("isotropic_remeshing", &pmp::isotropic_remeshing<Pm>, // deprecated?
  //       py::arg("faces"), py::arg("target_edge_length"), py::arg("pmesh"),
  //       py::arg("np") = py::dict());
  m.def("isotropic_remeshing", &pmp::isotropic_remeshing_sf<Pm, pmp::Adaptive_sizing_field<Pm>>,
        py::arg("faces"), py::arg("sizing"), py::arg("pmesh"),
        py::arg("np") = py::dict());
  m.def("random_perturbation", &pmp::random_perturbation<Pm>,
        py::arg("tmesh"), py::arg("perturbation_max_size"),
        py::arg("np") = py::dict());
  m.def("random_perturbation", &pmp::random_perturbation_v<Pm>,
        py::arg("vertices"), py::arg("tmesh"), py::arg("perturbation_max_size"),
        py::arg("np") = py::dict());
  m.def("refine", &pmp::refine<Pm>,
        py::arg("tmesh"), py::arg("faces"), py::arg("np") = py::dict());
  m.def("remesh_planar_patches", &pmp::remesh_planar_patches<Pm>,
        py::arg("pm"), py::arg("np_in") = py::dict(), py::arg("np_out") = py::dict());
  m.def("smooth_shape", &pmp::smooth_shape<Pm>,
        py::arg("pm"), py::arg("time"), py::arg("np") = py::dict());
  m.def("split_long_edges", &pmp::split_long_edges<Pm>,
        py::arg("edge_range"), py::arg("max_length"), py::arg("pmesh"),
        py::arg("np") = py::dict());
  // m.def("tangential_relaxation", &pmp::tangential_relaxation<Pm>, // changed in master
  //       py::arg("pm"), py::arg("np") = py::dict());
  m.def("triangulate_faces", &pmp::triangulate_faces<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("triangulate_faces_r", &pmp::triangulate_faces_r<Pm>,
        py::arg("face_range"), py::arg("pm"),
        py::arg("np") = py::dict());
  m.def("triangulate_polygons", &pmp::triangulate_polygons,
      py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());

  // Corrected Curvature Computation
  m.def("interpolated_corrected_curvatures", &pmp::interpolated_corrected_curvatures<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("interpolated_corrected_curvatures", &pmp::interpolated_corrected_curvatures_v<Pm>,
        py::arg("v"), py::arg("pm"), py::arg("np") = py::dict());

  // Normal Computation
  m.def("compute_face_normal", &pmp::compute_face_normal<Pm>,
        py::arg("f"), py::arg("pmesh"),
        py::arg("np") = py::dict());
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only working for sm
  m.def("compute_face_normals", &pmp::compute_face_normals<Pm, FaceVectorMap>,
        py::arg("pmesh"), py::arg("face_normals"),
        py::arg("np") = py::dict());
  m.def("compute_normals", &pmp::compute_normals<Pm, VertexVectorMap, FaceVectorMap>,
        py::arg("vnormals"), py::arg("fnormals"), py::arg("pmesh"),
        py::arg("np") = py::dict());
#endif
  m.def("compute_vertex_normal", &pmp::compute_vertex_normal<Pm>,
        py::arg("v"), py::arg("pmesh"),
        py::arg("np") = py::dict());
#if CGALPY_PMP_POLYGONAL_MESH == 1 // only working for sm
  m.def("compute_vertex_normals", &pmp::compute_vertex_normals<Pm, VertexVectorMap>,
        py::arg("pmesh"), py::arg("vertex_normals"),
        py::arg("np") = py::dict());
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
  
  // Orientation Functions
  m.def("does_bound_a_volume", &pmp::does_bound_a_volume<Pm>, // TODO: is_cc_outward_oriented
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("duplicate_non_manifold_edges_in_polygon_soup", &pmp::duplicate_non_manifold_edges_in_polygon_soup,
        py::arg("points"), py::arg("polygons"));
  m.def("is_outward_oriented", &pmp::is_outward_oriented<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("merge_reversible_connected_components", &pmp::merge_reversible_connected_components<Pm>,
         py::arg("pm"), py::arg("np") = py::dict());
  m.def("orient", &pmp::orient<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("orient_polygon_soup", &pmp::orient_polygon_soup, // TODO: PMPPolygonSoupOrientationVisitor
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());
  m.def("orient_to_bound_a_volume", &pmp::orient_to_bound_a_volume<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("orient_triangle_soup_with_reference_triangle_mesh", &pmp::orient_triangle_soup_with_reference_triangle_mesh<Pm>, // TODO: point_map
        py::arg("tm_ref"), py::arg("points"), py::arg("triangles"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  // m.def("orient_triangle_soup_with_reference_triangle_soup", &pmp::orient_triangle_soup_with_reference_triangle_soup, // TODO: point_map
  //       py::arg("ref_points"), py::arg("ref_faces"), py::arg("points"), py::arg("faces"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("reverse_face_orientations", &pmp::reverse_face_orientations<Pm>,
        py::arg("face_range"), py::arg("pmesh"));
  m.def("reverse_face_orientations", &PMP::reverse_face_orientations<Pm>,
        py::arg("pmesh"));
#if CGALPY_PMP_POLYGONAL_MESH == 1
  m.def("volume_connected_components", &pmp::volume_connected_components<Pm, FaceSizeTypeMap>,
        py::arg("tm"), py::arg("volume_id_map"),
        py::arg("np") = py::dict());
#endif


  // Intersection Functions
  m.def("do_intersect", &pmp::do_intersect_polylines);
  m.def("do_intersect", &pmp::do_intersect_polyline_ranges);
  m.def("do_intersect", &pmp::do_intersect_meshes<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("do_intersect", &pmp::do_intersect_mesh_polyline<Pm>,
        py::arg("pm"), py::arg("lst"),
        py::arg("np") = py::dict());
  m.def("do_intersect", &pmp::do_intersect_mesh_polyline_range<Pm>,
        py::arg("pm"), py::arg("lst"),
        py::arg("np") = py::dict());
  m.def("does_self_intersect_faces", &pmp::does_self_intersect_faces<Pm>,
        py::arg("face_range"), py::arg("tmesh"),
        py::arg("np") = py::dict());
  m.def("does_self_intersect", &pmp::does_self_intersect<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("does_triangle_soup_self_intersect", &pmp::does_triangle_soup_self_intersect, // TODO: point_map
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict());
  m.def("intersecting_meshes", &pmp::intersecting_meshes<Pm>, py::arg("range"),
        py::arg("np") = py::dict(), py::arg("nps") = py::dict());
  m.def("self_intersections", &pmp::self_intersections<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("self_intersections", &pmp::self_intersections_faces<Pm>,
        py::arg("face_range"), py::arg("pm"),
        py::arg("np") = py::dict());
  m.def("triangle_soup_self_intersections", &pmp::triangle_soup_self_intersections, // TODO: point_map
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict());


  // Combinatorial Repair
  m.def("duplicate_non_manifold_vertices", &pmp::duplicate_non_manifold_vertices<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("is_non_manifold_vertex", &PMP::is_non_manifold_vertex<Pm>,
        py::arg("v"), py::arg("pm"));
  m.def("is_polygon_soup_a_polygon_mesh", &pmp::is_polygon_soup_a_polygon_mesh,
        py::arg("polygons"));
  m.def("merge_duplicate_points_in_polygon_soup", &pmp::merge_duplicate_points_in_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());
  m.def("merge_duplicate_polygons_in_polygon_soup", &pmp::merge_duplicate_polygons_in_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());
  m.def("merge_duplicated_vertices_in_boundary_cycle", &pmp::merge_duplicated_vertices_in_boundary_cycle<Pm>,
        py::arg("h"), py::arg("pm"), py::arg("np") = py::dict());
  m.def("merge_duplicated_vertices_in_boundary_cycles", &pmp::merge_duplicated_vertices_in_boundary_cycles<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("non_manifold_vertices", &pmp::non_manifold_vertices<Pm>,
        py::arg("pm"));
  m.def("polygon_mesh_to_polygon_soup", &pmp::polygon_mesh_to_polygon_soup<Pm>,
        py::arg("pm"), py::arg("np") = py::dict());
  m.def("polygon_soup_to_polygon_mesh", &pmp::polygon_soup_to_polygon_mesh<Pm>, // TODO: point_map, ptvm, ptfm
        py::arg("points"), py::arg("polygons"), py::arg("np_ps") = py::dict(), py::arg("np_pm") = py::dict());
  m.def("remove_isolated_points_in_polygon_soup", &pmp::remove_isolated_points_in_polygon_soup,
        py::arg("points"), py::arg("polygons"));
  m.def("repair_polygon_soup", &pmp::repair_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());
  m.def("stitch_borders", &pmp::stitch_borders_bc<Pm>,
        py::arg("boundary_cycle_representatives"), py::arg("pmesh"),
        py::arg("np") = py::dict());
  m.def("stitch_borders", &pmp::stitch_borders_he<Pm>,
        py::arg("pmesh"), py::arg("hedge_pairs_to_stitch"),
        py::arg("np") = py::dict());
  m.def("stitch_borders", &pmp::stitch_borders<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());
  m.def("stitch_boundary_cycle", &pmp::stitch_boundary_cycle<Pm>,
        py::arg("h"), py::arg("pmesh"),
        py::arg("np") = py::dict());
  m.def("stitch_boundary_cycles", &pmp::stitch_boundary_cycles<Pm>,
        py::arg("boundary_cycle_representatives"), py::arg("pmesh"),
        py::arg("np") = py::dict());


  // Distance Functions
  m.def("approximate_Hausdorff_distance", &pmp::approximate_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("approximate_max_distance_to_point_set", &pmp::approximate_max_distance_to_point_set<Pm>,
        py::arg("tm"), py::arg("points"), py::arg("precision"),
        py::arg("np") = py::dict());
  m.def("approximate_symmetric_Hausdorff_distance", &pmp::approximate_symmetric_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("bounded_error_Hausdorff_distance", &pmp::bounded_error_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("error_bound") = 0.0001,
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("is_Hausdorff_distance_larger", &pmp::is_Hausdorff_distance_larger<Pm>,
        py::arg("tm1"), py::arg("tm2"), py::arg("distance_bound"), py::arg("error_bound"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("max_distance_to_triangle_mesh", &pmp::max_distance_to_triangle_mesh<Pm>,
        py::arg("tm"), py::arg("point"),
        py::arg("np") = py::dict());
  m.def("sample_triangle_mesh", &pmp::sample_triangle_mesh<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("sample_triangle_soup", &pmp::sample_triangle_soup,
        py::arg("points"), py::arg("triangles"), py::arg("np") = py::dict());

  // Corefinement and Boolean Operations
  m.def("autorefine", &pmp::autorefine<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());
  m.def("autorefine_triangle_soup", &pmp::autorefine_triangle_soup,
        py::arg("soup_points"), py::arg("soup_triangles"), py::arg("np") = py::dict());
  m.def("clip", &pmp::clip_c<Pm>,
        py::arg("tm"), py::arg("plane"),
        py::arg("np") = py::dict());
  m.def("clip", &pmp::clip_p<Pm>,
        py::arg("tm"), py::arg("iso_cuboid"),
        py::arg("np") = py::dict());
  m.def("clip", &pmp::clip<Pm>,
        py::arg("tm"), py::arg("clipper"),
        py::arg("np_tm") = py::dict(), py::arg("np_c") = py::dict());
  m.def("corefine", &pmp::corefine<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("corefine_and_compute_boolean_operations", &pmp::corefine_and_compute_boolean_operations<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::tuple());
  m.def("corefine_and_compute_difference", &pmp::corefine_and_compute_difference<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(), py::arg("np_out") = py::dict());
  m.def("corefine_and_compute_intersection", &pmp::corefine_and_compute_intersection<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict());
  m.def("corefine_and_compute_union", &pmp::corefine_and_compute_union<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict());









#if CGALPY_PMP_POLYGONAL_MESH == 1
  m.def("connected_components", &pmp::connected_components_map<Pm, Pm::Property_map<Fd, std::size_t>>,
        py::arg("pm"), py::arg("fcm"), py::arg("np") = py::dict());
  m.def("connected_components", &pmp::connected_components_map<Pm, Pm::Property_map<Fd, std::uint32_t>>,
        py::arg("pm"), py::arg("fcm"), py::arg("np") = py::dict());
  m.def("sharp_edges_segmentation", &pmp::sharp_edges_segmentation<Pm, Pm::Property_map<Ed, bool>, Pm::Property_map<Fd, int>>,
        py::arg("pmesh"), py::arg("angle_in_deg"), py::arg("edge_is_feature_map"),
        py::arg("patch_id_map"), py::arg("np") = py::dict());

  // only for sm
  m.def("compatible_orientations", &pmp::compatible_orientations<Pm, FaceBitMap>,
        py::arg("pm"), py::arg("face_bit_map"),
        py::arg("np") = py::dict());
  m.def("remove_connected_components_map", &pmp::remove_connected_components_map<Pm, FaceComponentMap>,
        py::arg("pm"), py::arg("components_to_remove"), py::arg("fccmap"), py::arg("np") = py::dict());
  m.def("keep_connected_components", &pmp::keep_connected_components_map<Pm, FaceComponentMap>,
        py::arg("pm"), py::arg("components_to_keep"), py::arg("fcm"), py::arg("np") = py::dict());
#endif
#if CGALPY_PMP_POLYGONAL_MESH == 0
  // only for polyhedron
  m.def("fair", &pmp::fair<Pm>,
        py::arg("tmesh"), py::arg("vertices"),
        py::arg("np") = py::dict());
#endif


  // m.def("triangulate_faces", &pmp::triangulate_faces<Pm>,
  //       py::arg("face_range"), py::arg("pm"),
  //       py::arg("parameters") = py::dict());

  m.def("surface_intersection", &pmp::surface_intersection<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("repair_polygon_soup", &pmp::repair_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());

  m.def("extract_boundary_cycles", &pmp::extract_boundary_cycles<Pm>,
        py::arg("pm"));


  m.def("border_halfedges", &pmp::border_halfedges<Pm>,
        py::arg("face_range"), py::arg("pm"),
        py::arg("np") = py::dict());

  m.def("is_closed", &CGAL::is_closed<Pm>,
        py::arg("g"));

  // this is currently only supported for Surface_mesh, because Polyhedron would need Random_access_property_map
  // the boost::property_map types don't work here for some reason
  // eg. Random_access_property_map<vector<unsigned long, allocator<unsigned long>>>
#if CGALPY_PMP_POLYGONAL_MESH == 1
  m.def("region_growing_of_planes_on_faces", &pmp::region_growing_of_planes_on_faces<Pm, RegionMap>,
        py::arg("pmesh"), py::arg("region_map"), py::arg("np") = py::dict());

  m.def("detect_corners_of_regions", &pmp::detect_corners_of_regions<Pm, RegionMap, CornerIdMap>,
        py::arg("pmesh"), py::arg("region_map"), py::arg("nb_regions"), py::arg("corner_id_map"),
        py::arg("np") = py::dict());

  m.def("remesh_almost_planar_patches", &pmp::remesh_almost_planar_patches<Pm, FacePatchMap, VertexCornerMap, EdgeIsConstrainedMap>,
        py::arg("tm_in"), py::arg("nb_patches"), py::arg("nb_corners"),
        py::arg("face_patch_map"), py::arg("vertex_corner_map"), py::arg("ecm"),
        py::arg("np_in") = py::dict(), py::arg("np_out") = py::dict());
#endif

  m.def("set_non_manifold_edge", &pmp::set_non_manifold_edge);
  m.def("set_non_manifold_vertex", &pmp::set_non_manifold_vertex);
  m.def("set_duplicated_vertex", &pmp::set_duplicated_vertex);
  m.def("set_vertex_id_in_polygon_replaced", &pmp::set_vertex_id_in_polygon_replaced);
  m.def("set_polygon_orientation_reversed", &pmp::set_polygon_orientation_reversed);
  m.def("set_link_connected_polygons", &pmp::set_link_connected_polygons);

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



  // Corefine_visitor
  m.def("set_before_subface_creations", &pmp::set_before_subface_creations_fn);
  m.def("set_after_subface_creations", &pmp::set_after_subface_creations_fn);
  m.def("set_before_subface_created", &pmp::set_before_subface_created_fn);
  m.def("set_after_subface_created", &pmp::set_after_subface_created_fn);
  m.def("set_before_face_copy", &pmp::set_before_face_copy_fn);
  m.def("set_after_face_copy", &pmp::set_after_face_copy_fn);
  m.def("set_before_edge_split", &pmp::set_before_edge_split_fn);
  m.def("set_edge_split", &pmp::set_edge_split_fn);
  m.def("set_after_edge_split", &pmp::set_after_edge_split_fn);
  m.def("set_add_retriangulation_edge", &pmp::set_add_retriangulation_edge_fn);
  m.def("set_before_edge_copy", &pmp::set_before_edge_copy_fn);
  m.def("set_after_edge_copy", &pmp::set_after_edge_copy_fn);
  m.def("set_before_edge_duplicated", &pmp::set_before_edge_duplicated_fn);
  m.def("set_after_edge_duplicated", &pmp::set_after_edge_duplicated_fn);
  m.def("set_intersection_edge_copy", &pmp::set_intersection_edge_copy_fn);
  m.def("set_new_vertex_added", &pmp::set_new_vertex_added_fn);
  m.def("set_intersection_point_detected", &pmp::set_intersection_point_detected_fn);
  m.def("set_before_vertex_copy", &pmp::set_before_vertex_copy_fn);
  m.def("set_after_vertex_copy", &pmp::set_after_vertex_copy_fn);
  m.def("set_start_filtering_intersections", &pmp::set_start_filtering_intersections_fn);
  m.def("set_progress_filtering_intersections", &pmp::set_progress_filtering_intersections_fn);
  m.def("set_end_filtering_intersections", &pmp::set_end_filtering_intersections_fn);
  m.def("set_start_triangulating_faces", &pmp::set_start_triangulating_faces_fn);
  m.def("set_triangulating_faces_step", &pmp::set_triangulating_faces_step_fn);
  m.def("set_end_triangulating_faces", &pmp::set_end_triangulating_faces_fn);
  m.def("set_start_handling_intersection_of_coplanar_faces", &pmp::set_start_handling_intersection_of_coplanar_faces_fn);
  m.def("set_intersection_of_coplanar_faces_step", &pmp::set_intersection_of_coplanar_faces_step_fn);
  m.def("set_end_handling_intersection_of_coplanar_faces", &pmp::set_end_handling_intersection_of_coplanar_faces_fn);
  m.def("set_start_handling_edge_face_intersections", &pmp::set_start_handling_edge_face_intersections_fn);
  m.def("set_edge_face_intersections_step", &pmp::set_edge_face_intersections_step_fn);
  m.def("set_end_handling_edge_face_intersections", &pmp::set_end_handling_edge_face_intersections_fn);
  m.def("set_start_building_output", &pmp::set_start_building_output_fn);
  m.def("set_end_building_output", &pmp::set_end_building_output_fn);
  m.def("set_filter_coplanar_edges", &pmp::set_filter_coplanar_edges_fn);
  m.def("set_detect_patches", &pmp::set_detect_patches_fn);
  m.def("set_classify_patches", &pmp::set_classify_patches_fn);
  m.def("set_classify_intersection_free_patches", &pmp::set_classify_intersection_free_patches_fn);
  m.def("set_out_of_place_operation", &pmp::set_out_of_place_operation_fn);
  m.def("set_in_place_operation", &pmp::set_in_place_operation_fn);
  m.def("set_in_place_operations", &pmp::set_in_place_operations_fn);

  using Pcad = PMP::Principal_curvatures_and_directions<Kernel>;
  py::class_<Pcad>(m, "Principal_curvatures_and_directions")
    .def(py::init<>())
    .def("__init__", [](Pcad& instance, const py::list& l) {
      if (l.size() != 4) {
        throw std::invalid_argument("List must have 4 elements");
      }
      instance.min_curvature = py::cast<double>(l[0]);
      instance.max_curvature = py::cast<double>(l[1]);
      instance.min_direction = py::cast<Kernel::Vector_3>(l[2]);
      instance.max_direction = py::cast<Kernel::Vector_3>(l[3]);
    })
    .def_ro("min_curvature", &Pcad::min_curvature)
    .def_ro("max_curvature", &Pcad::max_curvature)
    .def_ro("min_direction", &Pcad::min_direction)
    .def_ro("max_direction", &Pcad::max_direction)
    ;

  using Asf = pmp::Adaptive_sizing_field<Pm>;
  py::class_<Asf>(m, "Adaptive_sizing_field")
    .def(py::init<double, py::tuple, py::list, Pm&>())
    .def("at", &Asf::at)
    .def("is_too_long", &Asf::is_too_long)
    .def("is_too_short", &Asf::is_too_short)
    .def("split_placement", &Asf::split_placement)
    .def("register_split_vertex", &Asf::register_split_vertex)
    ;

  // using Usf = PMP::Uniform_sizing_field<Pm>; // Work in progress
  // py::class_<Usf>(m, "Uniform_sizing_field")
  //   .def(py::init<double,
  //   .def("at", &Usf::at)
  //   .def("is_too_long", &Usf::is_too_long)
  //   .def("is_too_short", &Usf::is_too_short)
  //   .def("split_placement", &Usf::split_placement)
  //   .def("register_split_vertex", &Usf::register_split_vertex)
  //   ;

  using Av = pmp::Autorefinement_visitor;
  py::class_<Av>(m, "Autorefinement_visitor")
    .def(py::init<>())
    .def("set_number_of_output_triangles", &Av::set_number_of_output_triangles)
    .def("set_verbatim_triangle_copy", &Av::set_verbatim_triangle_copy)
    .def("set_new_subtriangle", &Av::set_new_subtriangle)
    ;

  // default visitor
  using Dv = pmp::Default_visitor<Pm>;
  py::class_<Dv>(m, "Default_visitor")
    .def(py::init<>())
    ;

  using Dov = pmp::Default_orientation_visitor;
  py::class_<Dov>(m, "Default_orientation_visitor")
    .def(py::init<>())
    ;

  // non-manifold
  using Nmv = pmp::Non_manifold_output_visitor<Pm>;
  py::class_<Nmv>(m, "Non_manifold_output_visitor")
    // constructor with 2 PolygonMesh arguments
    .def(py::init<Pm&, Pm&>())
    // visitor.extract_intersection(points, polygons);
    .def("extract_intersection", &Nmv::my_extract_intersection)
    ;

  // hole filling
  using Hfv = pmp::HFDefault_visitor;
  py::class_<Hfv>(m, "Hole_filling_default_visitor")
    .def(py::init<>())
    ;

  // corefine
  using Cv = pmp::Corefine_visitor<Pm>;
  py::class_<Cv>(m, "Corefine_visitor")
    .def(py::init<>())
    ;

}
