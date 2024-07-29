// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <CGAL/Dynamic_property_map.h>
#define CGAL_USE_BASIC_VIEWER

#include <stdexcept>
#include <iterator>

#include <boost/graph/graph_traits.hpp>
#include <boost/iterator/function_output_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>

#include <CGAL/boost/graph/helpers.h>
#include <CGAL/iterator.h>
#include <CGAL/Named_function_parameters.h>
#include <CGAL/tags.h>
#include <CGAL/Polygon_mesh_processing/autorefinement.h>
#include <CGAL/Polygon_mesh_processing/clip.h>
#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/distance.h>
#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#include <CGAL/Polygon_mesh_processing/intersection.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup_extension.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/random_perturbation.h>
#include <CGAL/Polygon_mesh_processing/refine.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/repair_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/smooth_shape.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include "CGALPY/helpers.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/Adaptive_sizing_field.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#include "CGALPY/Non_manifold_output_visitor.hpp"
#include "CGALPY/Default_visitor.hpp"
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
  return get(CGAL::vertex_point, pm);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 0

#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
template <typename PolygonMesh, typename V>
auto get_vertex_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none()) {
  using K = typename boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using map_type = typename PolygonMesh::template Property_map<K, V>;
  return dict_key.is_none() ? pm.template add_property_map<K, V>(map_name, V()).first :
    py::cast<map_type>(dict_key);
}
#endif // CGALPY_PMP_POLYGONAL_MESH == 1
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron
  // boost::property_map<Mesh, CGAL::dynamic_vertex_property_t<Epic_kernel::FT>>::type
template <typename PolygonMesh, typename V>
auto get_vertex_prop_map(PolygonMesh& pm, const std::string& map_name, const py::object& dict_key = py::none()) {
  using dynamic_prop = typename CGAL::dynamic_vertex_property_t<V>;
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
  return PMP::do_intersect(pm1, pm2,
                           internal::parse_pmp_np<PolygonMesh>(np1),
                           internal::parse_pmp_np<PolygonMesh>(np2));
}

//
template <typename PolygonMesh>
bool do_intersect_mesh_polyline(const PolygonMesh& pm, const py::list& lst,
                                const py::dict& parameters = py::dict()) {
  auto begin = stl_input_iterator<Point_3>(lst);
  auto end = stl_input_iterator<Point_3>(lst, false);
  std::vector<Point_3> polyline(begin, end);
  return PMP::do_intersect(pm, polyline, internal::parse_pmp_np<PolygonMesh>(parameters));
}

//
template <typename PolygonMesh>
bool do_intersect_mesh_polyline_range(const PolygonMesh& pm,
                                      const py::list& lsts,
                                      const py::dict& parameters = py::dict()) {
  std::vector<std::vector<Point_3>> range;
  for (const auto& lh : lsts) {
    auto begin1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh));
    auto end1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh), false);
    range.emplace_back(begin1, end1);
  }
  return PMP::do_intersect(pm, range, internal::parse_pmp_np<PolygonMesh>(parameters));
}

//
template <typename PolygonMesh>
py::list intersecting_meshes(const py::list& lst) {
  using Pm = PolygonMesh;

  py::list result;
  auto begin = stl_input_iterator<Pm>(lst);
  auto end = stl_input_iterator<Pm>(lst, false);
  auto op = [&] (const std::pair<std::size_t, std::size_t>& res) mutable
            { result.append(res); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::intersecting_meshes(boost::make_iterator_range(begin, end), it);

  return result;
}

//
template <typename PolygonMesh>
py::list self_intersections(const PolygonMesh& pm,
                            const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  py::list result;
  auto op = [&] (const std::pair<Fd, Fd>& res) mutable
            { result.append(py::make_tuple(res.first, res.second)); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::self_intersections(pm, it, internal::parse_pmp_np<PolygonMesh>(parameters));

  return result;
}

//
template <typename PolygonMesh>
py::list self_intersections_faces(const py::list& face_range,
                                  const PolygonMesh& pm,
                                  const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  auto begin = stl_input_iterator<Fd>(face_range);
  auto end = stl_input_iterator<Fd>(face_range, false);

  py::list result;
  auto op = [&] (const std::pair<Fd, Fd>& res) mutable
            { result.append(res); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::self_intersections(boost::make_iterator_range(begin, end), pm, it,
                          internal::parse_pmp_np<PolygonMesh>(parameters));
  return result;
}

//
template <typename PolygonMesh>
bool does_self_intersect(const PolygonMesh& pm,
                         const py::dict& parameters = py::dict()) {
  return PMP::does_self_intersect(pm, internal::parse_pmp_np<PolygonMesh>(parameters));
}

//
template <typename PolygonMesh>
bool does_self_intersect_faces(const py::list& face_range,
                               const PolygonMesh& pm,
                               const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  auto begin = stl_input_iterator<Fd>(face_range);
  auto end = stl_input_iterator<Fd>(face_range, false);
  return PMP::does_self_intersect(boost::make_iterator_range(begin, end), pm, internal::parse_pmp_np<PolygonMesh>(parameters));
}

template <typename PolygonMesh>
void corefine(PolygonMesh& tm1, PolygonMesh& tm2,
              const py::dict& np1 = py::dict(),
              const py::dict& np2 = py::dict()) {
  // np1 can have a corefinement visitor
  bool visitor = np1.contains("visitor");
  if (visitor) {
    // try to cast to Non_manifold_output_visitor or Default_visitor
    try {
      auto visitor = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1).visitor(visitor),
                    internal::parse_pmp_np<PolygonMesh>(np2));
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Default_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1).visitor(visitor),
                    internal::parse_pmp_np<PolygonMesh>(np2));
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np1["visitor"]);
      PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1).visitor(visitor),
                    internal::parse_pmp_np<PolygonMesh>(np2));
    }
    catch (const py::cast_error&) {
    }
    throw std::runtime_error("Visitor type not recognized");
  }
  else {
    PMP::corefine(tm1, tm2, internal::parse_pmp_np<PolygonMesh>(np1),
                  internal::parse_pmp_np<PolygonMesh>(np2));
  }
}

//
template <typename PolygonMesh>
py::list
connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor
                      seed_face,
                    const PolygonMesh& pm,
                    const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  py::list lst;
  auto op = [&] (Fd face_descriptor) mutable
            { lst.append(py::cast(face_descriptor)); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::connected_component(seed_face, pm, it, internal::parse_pmp_np<PolygonMesh>(parameters));
  return lst;
}

//
template <typename PolygonMesh>
py::list connected_components(const PolygonMesh& pm,
                              const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;

  auto fccmap = CGAL::get(CGAL::dynamic_face_property_t<std::size_t>(), pm);
  auto num = PMP::connected_components(pm, fccmap,
                                       internal::parse_pmp_np<PolygonMesh>(parameters));
  py::dict dct;
  for (auto f : CGAL::faces(pm)) dct[py::cast(f)] = py::cast(get(fccmap, f));
  py::list lst;
  lst.append(num);
  lst.append(std::move(dct));
  return lst;
}

//
// using dfppm = typename boost::property_map<Prn, CGAL::dynamic_face_property_t<std::size_t>>::type;
template <typename PolygonMesh>
auto connected_components_map(const PolygonMesh& pm,
                              typename boost::property_map<PolygonMesh, CGAL::dynamic_face_property_t<std::size_t>>::type& fccmap,
                              const py::dict& parameters = py::dict()) {
  return PMP::connected_components(pm, fccmap, internal::parse_pmp_np<PolygonMesh>(parameters));
}

//
template <typename PolygonMesh>
void merge_reversible_connected_components(PolygonMesh& pm,
                              const py::dict& parameters = py::dict()) {
  PMP::merge_reversible_connected_components(pm, internal::parse_pmp_np<PolygonMesh>(parameters));
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

  Pm out_union, out_intersection, tm1_minus_tm2, tm2_minus_tm1;
  py::dict np_out_union, np_out_intersection, np_out_tm1_minus_tm2, np_out_tm2_minus_tm1;

  if (np_out.size() > 0)
    np_out_union = py::cast<py::dict>(np_out[0]);
  if (np_out.size() > 1)
    np_out_intersection = py::cast<py::dict>(np_out[1]);
  if (np_out.size() > 2)
    np_out_tm1_minus_tm2 = py::cast<py::dict>(np_out[2]);
  if (np_out.size() > 3)
    np_out_tm2_minus_tm1 = py::cast<py::dict>(np_out[3]);

  std::array<bool, 4> res;
  bool visitor1 = np_out_union.contains("visitor");
  bool visitor2 = np_out_intersection.contains("visitor");

  if (visitor1 && visitor2) {
    try {
      auto v1 = py::cast<pmp::Default_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Default_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Default_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Default_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Default_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Default_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out_union["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
  }
  else if (visitor1) {
    try {
      auto v1 = py::cast<pmp::Default_visitor<Pm>>(np_out_union["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np_out_union["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out_union["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
        internal::parse_pmp_np<PolygonMesh>(np2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
  }
  else if (visitor2) {
    try {
      auto v2 = py::cast<pmp::Default_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out_intersection["visitor"]);
      res = PMP::corefine_and_compute_boolean_operations(
        pm1, pm2,
        {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
        internal::parse_pmp_np<PolygonMesh>(np1),
        internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2),
        std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                        internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                        internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
    }
    catch (const py::cast_error& e) {
    }
  }
  else {
    res = PMP::corefine_and_compute_boolean_operations(
      pm1, pm2,
      {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
      internal::parse_pmp_np<PolygonMesh>(np1),
      internal::parse_pmp_np<PolygonMesh>(np2),
      std::make_tuple(internal::parse_pmp_np<PolygonMesh>(np_out_union),
                      internal::parse_pmp_np<PolygonMesh>(np_out_intersection),
                      internal::parse_pmp_np<PolygonMesh>(np_out_tm1_minus_tm2),
                      internal::parse_pmp_np<PolygonMesh>(np_out_tm2_minus_tm1)));
  }

  return py::make_tuple(res[0] ? py::cast(out_union) : py::none(),
                        res[1] ? py::cast(out_intersection) : py::none(),
                        res[2] ? py::cast(tm1_minus_tm2) : py::none(),
                        res[3] ? py::cast(tm2_minus_tm1) : py::none());

}

//
template <typename PolygonMesh>
PolygonMesh corefine_and_compute_union(PolygonMesh& pm1, PolygonMesh& pm2,
                                      const py::dict& np1 = py::dict(),
                                      const py::dict& np2 = py::dict(),
                                      const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using Fd = typename boost::graph_traits<Pm>::face_descriptor;
  using Ed = typename boost::graph_traits<Pm>::edge_descriptor;

  Pm out;
  bool visitor = np_out.contains("visitor");
  bool valid;
#if CGALPY_PMP_POLYGONAL_MESH == 1 //surface_mesh
  using vpmap = typename PolygonMesh::template Property_map<Vd, Point_3>;
  using fimap = typename PolygonMesh::template Property_map<Fd, std::size_t>;
  using constrained_map = typename PolygonMesh::template Property_map<Ed, bool>;
  auto vpm1 = np1.contains("vertex_point_map") ? py::cast<vpmap>(np1["vertex_point_map"]) : pm1.points();
  auto vpm2 = np2.contains("vertex_point_map") ? py::cast<vpmap>(np2["vertex_point_map"]) : pm2.points();
  auto vpm3 = np_out.contains("vertex_point_map") ? py::cast<vpmap>(np_out["vertex_point_map"]) : out.points();
  auto propmap1 = np1.contains("edge_is_constrained_map") ?
    py::cast<constrained_map>(np1["edge_is_constrained_map"]) :
    pm1.template add_property_map<Ed, bool>("INTERNAL_MAP1", false).first;
  auto propmap2 = np2.contains("edge_is_constrained_map") ?
    py::cast<constrained_map>(np2["edge_is_constrained_map"]) :
    pm2.template add_property_map<Ed, bool>("INTERNAL_MAP2", false).first;
  auto propmap3 = np_out.contains("edge_is_constrained_map") ?
    py::cast<constrained_map>(np_out["edge_is_constrained_map"]) :
    out.template add_property_map<Ed, bool>("INTERNAL_MAP3", false).first;
  auto fimap1 = np1.contains("face_index_map") ?
    py::cast<fimap>(np1["face_index_map"]) :
    pm1.template add_property_map<Fd, std::size_t>("INTERNAL_MAP4", 0).first;
  auto fimap2 = np2.contains("face_index_map") ?
    py::cast<fimap>(np2["face_index_map"]) :
    pm2.template add_property_map<Fd, std::size_t>("INTERNAL_MAP5", 0).first;


  if (visitor) {
    // try to cast to Non_manifold_output_visitor or Default_visitor
    try {
      auto visitor = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                              internal::parse_pmp_np<PolygonMesh>(np1)
                                              .vertex_point_map(vpm1)
                                              .edge_is_constrained_map(propmap1)
                                              .face_index_map(fimap1)
                                              .visitor(visitor),
                                              internal::parse_pmp_np<PolygonMesh>(np2)
                                              .vertex_point_map(vpm2)
                                              .edge_is_constrained_map(propmap2)
                                              .face_index_map(fimap2)
                                              .visitor(visitor),
                                              internal::parse_pmp_np<PolygonMesh>(np_out)
                                              .vertex_point_map(vpm3)
                                              .edge_is_constrained_map(propmap3)
                                              );
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Default_visitor<Pm>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                              internal::parse_pmp_np<PolygonMesh>(np1)
                                              .vertex_point_map(vpm1)
                                              .edge_is_constrained_map(propmap1)
                                              .face_index_map(fimap1)
                                              .visitor(visitor),
                                              internal::parse_pmp_np<PolygonMesh>(np2)
                                              .vertex_point_map(vpm2)
                                              .edge_is_constrained_map(propmap2)
                                              .face_index_map(fimap2)
                                              .visitor(visitor),
                                              internal::parse_pmp_np<PolygonMesh>(np_out)
                                              .vertex_point_map(vpm3)
                                              .edge_is_constrained_map(propmap3)
                                              );
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                              // internal::parse_pmp_np<PolygonMesh>(np1),
                                              // internal::parse_pmp_np<PolygonMesh>(np2),
                                              // internal::parse_pmp_np<PolygonMesh>(np_out).visitor(visitor));
                                              internal::parse_pmp_np<PolygonMesh>(np1)
                                              .vertex_point_map(vpm1)
                                              .edge_is_constrained_map(propmap1)
                                              .face_index_map(fimap1)
                                              .visitor(visitor),
                                              internal::parse_pmp_np<PolygonMesh>(np2)
                                              .vertex_point_map(vpm2)
                                              .edge_is_constrained_map(propmap2)
                                              .face_index_map(fimap2)
                                              .visitor(visitor),
                                              internal::parse_pmp_np<PolygonMesh>(np_out)
                                              .vertex_point_map(vpm3)
                                              .edge_is_constrained_map(propmap3)
                                              );
    }
    catch (const py::cast_error&) {
    }
    throw std::runtime_error("Visitor type not recognized");
  }
  else {
    valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                            internal::parse_pmp_np<PolygonMesh>(np1)
                                            .vertex_point_map(vpm1)
                                            .edge_is_constrained_map(propmap1)
                                            .face_index_map(fimap1)
                                            ,
                                            internal::parse_pmp_np<PolygonMesh>(np2)
                                            .vertex_point_map(vpm2)
                                            .edge_is_constrained_map(propmap2)
                                            .face_index_map(fimap2)
                                            ,
                                            internal::parse_pmp_np<PolygonMesh>(np_out)
                                            .vertex_point_map(vpm3)
                                            .edge_is_constrained_map(propmap3)
                                            );
  }
#endif
#if CGALPY_PMP_POLYGONAL_MESH == 0 //polyhedron, temporary
  valid = PMP::corefine_and_compute_union(pm1, pm2, out,
                                          internal::parse_pmp_np<PolygonMesh>(np1),
                                          internal::parse_pmp_np<PolygonMesh>(np2),
                                          internal::parse_pmp_np<PolygonMesh>(np_out));
#endif //CGALPY_PMP_POLYGONAL_MESH
  if (! valid) throw std::runtime_error("Cannot compute union!");
  return out;
}

//
template <typename PolygonMesh>
PolygonMesh corefine_and_compute_difference(PolygonMesh& pm1, PolygonMesh& pm2,
                                          const py::dict& np1 = py::dict(),
                                          const py::dict& np2 = py::dict(),
                                          const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;
  bool visitor = np_out.contains("visitor");
  Pm out;
  bool valid;
  if (visitor) {
    // try to cast to Non_manifold_output_visitor or Default_visitor
    try {
      auto visitor = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                           internal::parse_pmp_np<PolygonMesh>(np1),
                                           internal::parse_pmp_np<PolygonMesh>(np2),
                                           internal::parse_pmp_np<PolygonMesh>(np_out).visitor(visitor));
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Default_visitor<Pm>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                           internal::parse_pmp_np<PolygonMesh>(np1),
                                           internal::parse_pmp_np<PolygonMesh>(np2),
                                           internal::parse_pmp_np<PolygonMesh>(np_out).visitor(visitor));
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                           internal::parse_pmp_np<PolygonMesh>(np1),
                                           internal::parse_pmp_np<PolygonMesh>(np2),
                                           internal::parse_pmp_np<PolygonMesh>(np_out).visitor(visitor));
    }
    catch (const py::cast_error&) {
    }
    throw std::runtime_error("Visitor type not recognized");
  }
  else {
    valid = PMP::corefine_and_compute_difference(pm1, pm2, out,
                                         internal::parse_pmp_np<PolygonMesh>(np1),
                                         internal::parse_pmp_np<PolygonMesh>(np2),
                                         internal::parse_pmp_np<PolygonMesh>(np_out));
  }
  if (! valid) throw std::runtime_error("Cannot compute difference!");
  return out;
}

//
template <typename PolygonMesh>
auto autorefine(PolygonMesh& tm,
                const py::dict& np = py::dict()) {
  PMP::autorefine(tm, internal::parse_pmp_np<PolygonMesh>(np));
}
//
template <typename PolygonMesh>
py::list triangulate_hole_polyline(const py::list& lst1, const py::list& lst2,
                                   const py::dict& parameters = py::dict()) {
  auto begin1 = stl_input_iterator<Point_3>(lst1);
  auto end1 = stl_input_iterator<Point_3>(lst1, false);
  std::vector<Point_3> polyline1(begin1, end1);
  auto begin2 = stl_input_iterator<Point_3>(lst2);
  auto end2 = stl_input_iterator<Point_3>(lst2, false);
  std::vector<Point_3> polyline2(begin2, end2);

  typedef CGAL::Triple<int, int, int> Triangle_int;
  std::vector<Triangle_int> out;
  PMP::triangulate_hole_polyline(polyline1, polyline2, std::back_inserter(out),
                                  internal::parse_pmp_np<PolygonMesh>(parameters));
  // convert to a list of integers
  py::list result;
  for (const auto& t : out) {
    result.append(py::make_tuple(t.first, t.second, t.third));
  }
  return result;
}

template <typename PolygonMesh>
PolygonMesh corefine_and_compute_intersection(PolygonMesh& pm1, PolygonMesh& pm2,
                                             const py::dict& np1 = py::dict(),
                                             const py::dict& np2 = py::dict(),
                                             const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;
  Pm out;
  bool visitor = np_out.contains("visitor");
  bool valid;
  if (visitor) {
    // try to cast to Non_manifold_output_visitor or Default_visitor
    try {
      auto visitor = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                              internal::parse_pmp_np<PolygonMesh>(np1),
                                              internal::parse_pmp_np<PolygonMesh>(np2),
                                              internal::parse_pmp_np<PolygonMesh>(np_out).visitor(visitor));
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Default_visitor<Pm>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                              internal::parse_pmp_np<PolygonMesh>(np1),
                                              internal::parse_pmp_np<PolygonMesh>(np2),
                                              internal::parse_pmp_np<PolygonMesh>(np_out).visitor(visitor));
    } catch (const py::cast_error&) {
    }
    try {
      auto visitor = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_out["visitor"]);
      valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                              internal::parse_pmp_np<PolygonMesh>(np1),
                                              internal::parse_pmp_np<PolygonMesh>(np2),
                                              internal::parse_pmp_np<PolygonMesh>(np_out).visitor(visitor));
    }
    catch (const py::cast_error&) {
    }
    throw std::runtime_error("Visitor type not recognized");
  }
  else {
    valid = PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                            internal::parse_pmp_np<PolygonMesh>(np1),
                                            internal::parse_pmp_np<PolygonMesh>(np2),
                                            internal::parse_pmp_np<PolygonMesh>(np_out));
  }
  if (! valid) throw std::runtime_error("Cannot compute intersection!");
  return out;
}

//
template <typename PolygonMesh>
auto triangulate_refine_and_fair_hole(PolygonMesh& pmesh,
    typename boost::graph_traits<PolygonMesh>::halfedge_descriptor& border_halfedge,
                                          const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Graph_traits = boost::graph_traits<Pm>;
  using halfedge_descriptor = typename Graph_traits::halfedge_descriptor;
  using Fd = typename Graph_traits::face_descriptor;

  ///// change this to a more general type
  using Vertex_identifier = typename Graph_traits::vertex_descriptor;
  using Face_identifier = typename Graph_traits::face_descriptor;
  /////

  py::list facets, vertices;
  internal::Named_params np = CGAL::parameters::verbose(false);

  // this is dirty, but it works, i'm open to other solutions that work and are more elegant
  // the use of auto avoids some probles with types
  bool faces_flag = parameters.contains("face_output_iterator") && py::cast<bool>(parameters["face_output_iterator"]);
  bool vertices_flag = parameters.contains("vertex_output_iterator") && py::cast<bool>(parameters["vertex_output_iterator"]);
  if (faces_flag && vertices_flag) {
    std::vector<Face_identifier> fids;
    std::vector<Vertex_identifier> vids;
    auto it1 = std::back_inserter(fids);
    auto it2 = std::back_inserter(vids);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge, internal::parse_pmp_np<PolygonMesh>(parameters).face_output_iterator(it1).vertex_output_iterator(it2));
    for (const auto& fid : fids) facets.append(fid);
    for (const auto& vid : vids) vertices.append(vid);
    return py::make_tuple(std::get<0>(res), facets, vertices);
  } else if (faces_flag) {
    std::vector<Face_identifier> fids;
    auto it = std::back_inserter(fids);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge, internal::parse_pmp_np<PolygonMesh>(parameters).face_output_iterator(it));
    for (const auto& fid : fids) facets.append(fid);
    return py::make_tuple(std::get<0>(res), facets, vertices);
  } else if (vertices_flag) {
    std::vector<Vertex_identifier> vids;
    auto it = std::back_inserter(vids);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge, internal::parse_pmp_np<PolygonMesh>(parameters).vertex_output_iterator(it));
    for (const auto& vid : vids) vertices.append(vid);
    return py::make_tuple(std::get<0>(res), facets, vertices);
  }
  else {
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge, internal::parse_pmp_np<PolygonMesh>(parameters));
    return py::make_tuple(std::get<0>(res), facets, vertices);
  }
}

//
template <typename PolygonMesh>
auto triangulate_faces(PolygonMesh& pm,
                              const py::dict& parameters) {
  using Pm = PolygonMesh;

  // make a copy of the input mesh
  return PMP::triangulate_faces(pm, internal::parse_pmp_np<PolygonMesh>(parameters));
}

//
template <typename PolygonMesh>
auto isotropic_remeshing(const py::list& face_range,
                                double target_edge_length,
                                PolygonMesh& pmesh,
                                const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  PMP::isotropic_remeshing(boost::make_iterator_range(stl_input_iterator<Fd>(face_range),
                                                      stl_input_iterator<Fd>(face_range, false)),
                           target_edge_length, pmesh,
                           internal::parse_pmp_np<PolygonMesh>(parameters));
}


//
template <typename PolygonMesh, typename SizingFunction>
auto isotropic_remeshing_sf(const py::list& face_range,
                                SizingFunction& sizing,
                                PolygonMesh& pmesh,
                                const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  PMP::isotropic_remeshing(boost::make_iterator_range(stl_input_iterator<Fd>(face_range),
                                                      stl_input_iterator<Fd>(face_range, false)),
                           sizing, pmesh,
                           internal::parse_pmp_np<PolygonMesh>(parameters));

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

template <typename PolygonMesh>
auto tangential_relaxation(PolygonMesh& tm,
                                  const py::dict& parameters = py::dict()) {
  PMP::tangential_relaxation(tm, internal::parse_pmp_np<PolygonMesh>(parameters));
}

template <typename PolygonMesh>
double approximate_Hausdorff_distance(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                      const py::dict& np1 = py::dict(),
                                      const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;
  return PMP::approximate_Hausdorff_distance<TAG>(tm1, tm2,
                                             internal::parse_pmp_np<PolygonMesh>(np1),
                                             internal::parse_pmp_np<PolygonMesh>(np2));
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
          const py::dict& np1 = py::dict(),
          const py::dict& np2 = py::dict()) {
  using Pm = PolygonMesh;
  bool visitor1 = np1.contains("visitor");
  bool visitor2 = np2.contains("visitor");
  bool res;
  if (visitor1 && visitor2) {
    try {
      auto v1 = py::cast<pmp::Default_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Default_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Default_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Default_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Default_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Default_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
  } else if (visitor1) {
    try {
      auto v1 = py::cast<pmp::Default_visitor<Pm>>(np1["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<Pm>>(np1["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np1["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1).visitor(v1),
                      internal::parse_pmp_np<PolygonMesh>(np2));
    }
    catch (const py::cast_error& e) {
    }
  } else if (visitor2) {
    try {
      auto v2 = py::cast<pmp::Default_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Corefine_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<Pm>>(np2["visitor"]);
      res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1),
                      internal::parse_pmp_np<PolygonMesh>(np2).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
  } else {
    res = PMP::clip(tm, clipper, internal::parse_pmp_np<PolygonMesh>(np1),
                    internal::parse_pmp_np<PolygonMesh>(np2));
  }
  return res;
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
auto stitch_borders(PolygonMesh& pmesh,
                    const py::dict& np = py::dict()) {
  return PMP::stitch_borders(pmesh, internal::parse_pmp_np<PolygonMesh>(np));
}

template <typename PolygonMesh>
auto random_perturbation(PolygonMesh& tmesh,
                         double perturbation,
                         const py::dict& np = py::dict()) {
  return PMP::random_perturbation(tmesh, perturbation, internal::parse_pmp_np<PolygonMesh>(np));
}

template <typename PolygonMesh>
auto sample_triangle_mesh(const PolygonMesh& tm,
                          const py::dict& np = py::dict()) {
  PointRange pts;
  PMP::sample_triangle_mesh(tm, std::back_inserter(pts), internal::parse_pmp_np<PolygonMesh>(np));
  return vec2list(pts);
}

template <typename PolygonMesh>
Vector_3 compute_face_normal(const typename boost::graph_traits<PolygonMesh>::face_descriptor& f,
                             const PolygonMesh& sm,
                             const py::dict& np = py::dict()) {
  return PMP::compute_face_normal(f, sm, internal::parse_pmp_np<PolygonMesh>(np));
}

template <typename PolygonMesh>
py::tuple compute_face_normals(const PolygonMesh& sm) {
  using Pm = PolygonMesh;

  py::list faces_list;
  py::list fnormals_list;
  for (auto f : faces(sm)) {
    faces_list.append(f);
    auto n = PMP::compute_face_normal(f, sm);
    fnormals_list.append(n);
  }

  return py::make_tuple(faces_list, fnormals_list);
}

template <typename PolygonMesh>
Vector_3 compute_vertex_normal(const typename boost::graph_traits<PolygonMesh>::vertex_descriptor& v,
                               const PolygonMesh& sm,
                               const py::dict& np = py::dict()) {
  return PMP::compute_vertex_normal(v, sm, internal::parse_pmp_np<PolygonMesh>(np));
}

template <typename PolygonMesh>
py::tuple compute_vertex_normals(const PolygonMesh& sm) {
  using Pm = PolygonMesh;

  py::list vertices_list;
  py::list vnormals_list;
  for (auto v : vertices(sm)) {
    vertices_list.append(v);
    auto n = PMP::compute_vertex_normal(v, sm);
    vnormals_list.append(n);
  }

  return py::make_tuple(vertices_list, vnormals_list);
}

template<typename PolygonMesh>
py::tuple compute_normals(const PolygonMesh& pm) {
  auto fn = compute_face_normals(pm);
  auto vn = compute_vertex_normals(pm);
  return py::make_tuple(fn, vn);
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

  auto ball_radius = np.contains("ball_radius") ? py::cast<FT>(np["ball_radius"]) : -1;
  auto geom_traits = np.contains("geom_traits") ? py::cast<Kernel>(np["geom_traits"]) : Kernel();


  double vmc_d, vGc;
  if (np.contains("vertex_normal_map")) {
    auto vnm = get_vertex_prop_map<PolygonMesh, Vector_3>
      (pm, "INTERNAL_MAP1", np.contains("vertex_normal_map") ? np["vertex_normal_map"] : py::none());
    PMP::interpolated_corrected_curvatures(v, pm,
    CGAL::parameters::geom_traits(geom_traits)
                                           .vertex_mean_curvature(std::ref(vmc_d))
                                           .vertex_Gaussian_curvature(std::ref(vGc))
                                           .vertex_principal_curvatures_and_directions(std::ref(pcad))
                                           .ball_radius(ball_radius)
                                           .vertex_normal_map(vnm)
                                           );
  } else {
    PMP::interpolated_corrected_curvatures(v, pm,
    CGAL::parameters::geom_traits(geom_traits)
                                           .vertex_mean_curvature(std::ref(vmc_d))
                                           .vertex_Gaussian_curvature(std::ref(vGc))
                                           .vertex_principal_curvatures_and_directions(std::ref(pcad))
                                           .ball_radius(ball_radius)
                                           );
  }
  vmc = py::cast(vmc_d);
  vgc = py::cast(vGc);
  return py::make_tuple(vmc, vgc, pcad);
}

template <typename PolygonMesh>
auto border_halfedges(const py::list& face_range,
                      const PolygonMesh& pm,
                      const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;
  using Hd = typename Gt::halfedge_descriptor;

  auto begin = stl_input_iterator<Fd>(face_range);
  auto end = stl_input_iterator<Fd>(face_range, false);

  std::vector<Hd> out;
  PMP::border_halfedges(boost::make_iterator_range(begin, end), pm, std::back_inserter(out),
                        internal::parse_pmp_np<PolygonMesh>(parameters));
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
      auto v1 = py::cast<pmp::Default_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Default_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Default_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Non_manifold_output_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Default_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_s["visitor"]);
      PMP::split(pm, splitter, internal::parse_pmp_np<PolygonMesh>(np_tm).visitor(v1),
                 internal::parse_pmp_np<PolygonMesh>(np_s).visitor(v2));
    }
    catch (const py::cast_error& e) {
    }
    try {
      auto v1 = py::cast<pmp::Corefine_visitor<PolygonMesh>>(np_tm["visitor"]);
      auto v2 = py::cast<pmp::Default_visitor<PolygonMesh>>(np_s["visitor"]);
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
      auto v2 = py::cast<pmp::Default_visitor<PolygonMesh>>(np_s["visitor"]);
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
      auto v1 = py::cast<pmp::Default_visitor<PolygonMesh>>(np_tm["visitor"]);
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
      auto v2 = py::cast<pmp::Default_visitor<PolygonMesh>>(np_s["visitor"]);
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
                      const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Ed = typename Gt::edge_descriptor;

  // turn edge_range to a vector
  std::vector<Ed> edge_vec;
  for (const auto& ed : edge_range) edge_vec.push_back(py::cast<Ed>(ed));
  PMP::split_long_edges(edge_vec,
                        max_length, pmesh, internal::parse_pmp_np<PolygonMesh>(parameters));
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

template <typename PolygonMesh>
py::list orient_triangle_soup_with_reference_triangle_mesh(const PolygonMesh& tm_ref,
                                                       const py::list& points,
                                                       const py::list& triangles,
                                                       const py::dict& np1 = py::dict(),
                                                       const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;
  std::vector<Point_3> pts = ptlist2ptvec(points);
  auto polys = polylist2polyvec(triangles);
  PMP::orient_triangle_soup_with_reference_triangle_mesh<TAG>(tm_ref, pts, polys,
                                                       internal::parse_pmp_np<PolygonMesh>(np1), internal::parse_pmp_np<PolygonMesh>(np2));
  return polyvec2polylist(polys);
}

py::tuple duplicate_non_manifold_edges_in_polygon_soup(const py::list& points,
                                                       const py::list& polygons) {
  auto pointvec = ptlist2ptvec(points);
  auto polyvec = polylist2polyvec(polygons);
  bool duplicated = PMP::duplicate_non_manifold_edges_in_polygon_soup(pointvec, polyvec);
  return py::make_tuple(polyvec2polylist(polyvec), ptvec2ptlist(pointvec), duplicated);
}


template <typename PolygonMesh>
py::tuple orient_polygon_soup(const py::list& points,
                              const py::list& polygons,
                              const py::dict& np = py::dict()) {
  PointRange pts = ptlist2ptvec(points);
  PolygonRange polys = polylist2polyvec(polygons);
  bool success;
  if (np.contains("visitor")) {
    auto vv = np["visitor"];
    try {
      // return cgal_parameters.visitor(py::cast<pmp::Dummy_default_orienatation_visitor>(visitor));
      success = PMP::orient_polygon_soup(pts, polys,
                               internal::parse_pmp_np<PolygonMesh>(np).visitor(py::cast<pmp::Default_orientation_visitor>(vv)));
      if (!success) throw std::runtime_error("Polygon soup orientation failed!");
      return py::make_tuple(ptvec2ptlist(pts), polyvec2polylist(polys));
    }
    catch (const py::cast_error&) {
    }
  }
  success = PMP::orient_polygon_soup(pts, polys,
                           internal::parse_pmp_np<PolygonMesh>(np));

  if (!success) throw std::runtime_error("Polygon soup orientation failed!");
  return py::make_tuple(ptvec2ptlist(pts), polyvec2polylist(polys));
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
  bool pvflag = (np_ps.contains("point_to_vertex_output_iterator") &&
    py::cast<bool>(np_ps["point_to_vertex_output_iterator"]));
  bool pfflag = (np_ps.contains("polygon_to_face_output_iterator") &&
    py::cast<bool>(np_ps["polygon_to_face_output_iterator"]));
  PolygonMesh output;

  if (pvflag && pfflag) {
    std::vector<std::pair<int, vd>> pvvec;
    std::vector<std::pair<int, fd>> pfvec;
    PMP::polygon_soup_to_polygon_mesh(ptvec, polyvec, output,
                                      internal::parse_pmp_np<PolygonMesh>(np_ps)
                                      .point_to_vertex_output_iterator(std::back_inserter(pvvec))
                                      .polygon_to_face_output_iterator(std::back_inserter(pfvec)),
                                      internal::parse_pmp_np<PolygonMesh>(np_pm));
    py::list pvlist, pflist;
    for (const auto& [p, v] : pvvec) {
      pvlist.append(py::make_tuple(p, v));
    }
    for (const auto& [p, f] : pfvec) {
      pflist.append(py::make_tuple(p, f));
    }
    return py::make_tuple(output, pvlist, pflist);
  } else if (pvflag) {
    std::vector<std::pair<int, vd>> pvvec;
    PMP::polygon_soup_to_polygon_mesh(ptvec, polyvec, output,
                                      internal::parse_pmp_np<PolygonMesh>(np_ps)
                                      .point_to_vertex_output_iterator(std::back_inserter(pvvec)),
                                      internal::parse_pmp_np<PolygonMesh>(np_pm));
    py::list pvlist;
    for (const auto& [p, v] : pvvec) {
      pvlist.append(py::make_tuple(p, v));
    }
    return py::make_tuple(output, pvlist, pf);
  } else if (pfflag) {
    std::vector<std::pair<int, fd>> pfvec;
    PMP::polygon_soup_to_polygon_mesh(ptvec, polyvec, output,
                                      internal::parse_pmp_np<PolygonMesh>(np_ps)
                                      .polygon_to_face_output_iterator(std::back_inserter(pfvec)),
                                      internal::parse_pmp_np<PolygonMesh>(np_pm));
    py::list pflist;
    for (const auto& [p, f] : pfvec) {
      pflist.append(py::make_tuple(p, f));
    }
    return py::make_tuple(output, pv, pflist);
  } else {
    PMP::polygon_soup_to_polygon_mesh(ptvec, polyvec, output,
                                      internal::parse_pmp_np<PolygonMesh>(np_ps),
                                      internal::parse_pmp_np<PolygonMesh>(np_pm));
    return py::make_tuple(output, pv, pf);
  }
}

// visitor stuff
template <typename PolygonMesh>
void orient_to_bound_a_volume(PolygonMesh& tm,
                              const py::dict& np = py::dict()) {
  PMP::orient_to_bound_a_volume(tm, internal::parse_pmp_np<PolygonMesh>(np));
}

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
using Pm = Polygonal_mesh;
using Gt = boost::graph_traits<Polygonal_mesh>;
using Vd = typename Gt::vertex_descriptor;
using Hd = typename Gt::halfedge_descriptor;
using Fd = typename Gt::face_descriptor;
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

  using Np_t = bool;
  using Np_tag = CGAL::internal_np::all_default_t;
  using Np_base = CGAL::internal_np::No_property;
  using Np_class = CGAL::Named_function_parameters<Np_t, Np_tag, Np_base>;
  using Concurrency_tag = CGAL::Sequential_tag;

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

  m.def("connected_component", &pmp::connected_component<Pm>,
        py::arg("seed_face"), py::arg("pm"),
        py::arg("parameters") = py::dict());
  m.def("connected_components", &pmp::connected_components<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());
  m.def("connected_components", &pmp::connected_components_map<Pm>,
        py::arg("pm"), py::arg("fcm"), py::arg("parameters") = py::dict());

  m.def ("merge_reversible_connected_components", &pmp::merge_reversible_connected_components<Pm>,
         py::arg("pm"), py::arg("parameters") = py::dict());

  m.def("do_intersect_polylines", &pmp::do_intersect_polylines);
  m.def("do_intersect_polyline_ranges", &pmp::do_intersect_polyline_ranges);
  m.def("do_intersect_meshes", &pmp::do_intersect_meshes<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());
  m.def("do_intersect_mesh_polyline", &pmp::do_intersect_mesh_polyline<Pm>,
        py::arg("pm"), py::arg("lst"),
        py::arg("parameters") = py::dict());
  m.def("do_intersect_mesh_polyline_range", &pmp::do_intersect_mesh_polyline_range<Pm>,
        py::arg("pm"), py::arg("lst"),
        py::arg("parameters") = py::dict());

  m.def("intersecting_meshes", &pmp::intersecting_meshes<Pm>);

  m.def("self_intersections", &pmp::self_intersections<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());
  m.def("self_intersections_faces", &pmp::self_intersections_faces<Pm>,
        py::arg("face_range"), py::arg("pm"),
        py::arg("parameters") = py::dict());

  m.def("does_self_intersect", &pmp::does_self_intersect<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());

  m.def("does_self_intersect_faces", &pmp::does_self_intersect_faces<Pm>,
        py::arg("pm"), py::arg("face_range"),
        py::arg("parameters") = py::dict());

  m.def("corefine", &pmp::corefine<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("corefine_and_compute_union", &pmp::corefine_and_compute_union<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict());

  m.def("autorefine", &pmp::autorefine<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());

  m.def("triangulate_hole_polyline", &pmp::triangulate_hole_polyline<Pm>,
        py::arg("lst1"), py::arg("lst2") = py::list(),
        py::arg("parameters") = py::dict());

  m.def("corefine_and_compute_intersection", &pmp::corefine_and_compute_intersection<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::dict());

  m.def("triangulate_refine_and_fair_hole", &pmp::triangulate_refine_and_fair_hole<Pm>,
        py::arg("pmesh"), py::arg("border_halfedge"),
        py::arg("parameters") = py::dict());

  // m.def("triangulate_faces", &pmp::triangulate_faces<Pm>,
  //       py::arg("face_range"), py::arg("pm"),
  //       py::arg("parameters") = py::dict());

  m.def("triangulate_faces", &pmp::triangulate_faces<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());

  // m.def("compute_face_normals", &pmp::compute_face_normals<Pm>,
  //       py::arg("pm"));

  m.def("isotropic_remeshing", &pmp::isotropic_remeshing<Pm>,
        py::arg("faces"), py::arg("target_edge_length"), py::arg("pmesh"),
        py::arg("parameters") = py::dict());

  m.def("isotropic_remeshing", &pmp::isotropic_remeshing_sf<Pm, pmp::Adaptive_sizing_field<Pm>>,
        py::arg("faces"), py::arg("sizing"), py::arg("pmesh"),
        py::arg("parameters") = py::dict());

  m.def("tangential_relaxation", &pmp::tangential_relaxation<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());

  m.def("smooth_shape", &pmp::smooth_shape<Pm>,
        py::arg("pm"), py::arg("time"), py::arg("parameters") = py::dict());

  m.def("approximate_Hausdorff_distance", &pmp::approximate_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("surface_intersection", &pmp::surface_intersection<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("repair_polygon_soup", &pmp::repair_polygon_soup,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());

  m.def("triangulate_polygons", &pmp::triangulate_polygons,
      py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());

  m.def("autorefine_triangle_soup", &pmp::autorefine_triangle_soup,
      py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());

  m.def("stitch_borders", &pmp::stitch_borders<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());

  m.def("random_perturbation", &pmp::random_perturbation<Pm>,
        py::arg("pm"), py::arg("perturbation_max_size"), py::arg("np") = py::dict());

  m.def("refine", &pmp::refine<Pm>,
        py::arg("tmesh"), py::arg("faces"), py::arg("np") = py::dict());

  m.def("sample_triangle_mesh", &pmp::sample_triangle_mesh<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());


  m.def("extract_boundary_cycles", &pmp::extract_boundary_cycles<Pm>,
        py::arg("pm"));

  m.def("corefine_and_compute_boolean_operations", &pmp::corefine_and_compute_boolean_operations<Pm>,
        py::arg("pm1"), py::arg("pm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict(),
        py::arg("np_out") = py::tuple());

  m.def("compute_face_normal", &pmp::compute_face_normal<Pm>);
  m.def("compute_face_normals", &pmp::compute_face_normals<Pm>);
  m.def("compute_vertex_normal", &pmp::compute_vertex_normal<Pm>);
  m.def("compute_vertex_normals", &pmp::compute_vertex_normals<Pm>);
  m.def("compute_normals", &pmp::compute_normals<Pm>);
  m.def("interpolated_corrected_curvatures", &pmp::interpolated_corrected_curvatures<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());
  m.def("interpolated_corrected_curvatures", &pmp::interpolated_corrected_curvatures_v<Pm>,
        py::arg("v"), py::arg("pm"), py::arg("parameters") = py::dict());

  m.def("border_halfedges", &pmp::border_halfedges<Pm>,
        py::arg("face_range"), py::arg("pm"),
        py::arg("parameters") = py::dict());

  m.def("split_long_edges", &pmp::split_long_edges<Pm>,
        py::arg("edge_range"), py::arg("max_length"), py::arg("pmesh"),
        py::arg("parameters") = py::dict());

  m.def("is_polygon_soup_a_polygon_mesh", &pmp::is_polygon_soup_a_polygon_mesh,
        py::arg("polygons"));

  m.def("orient_triangle_soup_with_reference_triangle_mesh", &pmp::orient_triangle_soup_with_reference_triangle_mesh<Pm>,
        py::arg("tm_ref"), py::arg("points"), py::arg("triangles"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("duplicate_non_manifold_edges_in_polygon_soup", &pmp::duplicate_non_manifold_edges_in_polygon_soup,
        py::arg("points"), py::arg("polygons"));

  m.def("orient_polygon_soup", &pmp::orient_polygon_soup<Pm>,
        py::arg("points"), py::arg("polygons"), py::arg("np") = py::dict());

  m.def("polygon_soup_to_polygon_mesh", &pmp::polygon_soup_to_polygon_mesh<Pm>,
        py::arg("points"), py::arg("polygons"), py::arg("np_ps") = py::dict(), py::arg("np_pm") = py::dict());

  m.def("is_closed", &CGAL::is_closed<Pm>,
        py::arg("g"));

  m.def("orient_to_bound_a_volume", &pmp::orient_to_bound_a_volume<Pm>,
        py::arg("tm"), py::arg("np") = py::dict());

  m.def("reverse_face_orientations", &PMP::reverse_face_orientations<Pm>,
        py::arg("pmesh"));

  // m.def("reverse_face_orientations")

  m.def("set_non_manifold_edge", &pmp::set_non_manifold_edge);
  m.def("set_non_manifold_vertex", &pmp::set_non_manifold_vertex);
  m.def("set_duplicated_vertex", &pmp::set_duplicated_vertex);
  m.def("set_vertex_id_in_polygon_replaced", &pmp::set_vertex_id_in_polygon_replaced);
  m.def("set_polygon_orientation_reversed", &pmp::set_polygon_orientation_reversed);
  m.def("set_link_connected_polygons", &pmp::set_link_connected_polygons);

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

  // corefine
  using Cv = pmp::Corefine_visitor<Pm>;
  py::class_<Cv>(m, "Corefine_visitor")
    .def(py::init<>())
    ;

}
