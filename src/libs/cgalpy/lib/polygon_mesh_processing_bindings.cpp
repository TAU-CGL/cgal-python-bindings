// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <CGAL/Named_function_parameters.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/iterator.h>
#include <CGAL/tags.h>
#include <iterator>
#define CGAL_USE_BASIC_VIEWER

#include <stdexcept>

#include <boost/iterator/function_output_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>
#include <CGAL/Polygon_mesh_processing/intersection.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
#include <CGAL/Polygon_mesh_processing/remesh.h>
#include <CGAL/Polygon_mesh_processing/distance.h>
#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup_extension.h>
#include <CGAL/Polygon_mesh_processing/autorefinement.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"

#include "CGALPY/Adaptive_sizing_field.hpp"

#include "CGALPY/Corefine_visitor.hpp"
#include "CGALPY/Non_manifold_output_visitor.hpp"
#include "CGALPY/Default_visitor.hpp"

#include "CGALPY/pmp_np_parser.hpp"
#include "CGALPY/internal.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace PARMS = CGAL::parameters;

namespace pmp {

// helper
template <typename T>
py::list vec2list(T vec) {
  py::list retv;
  for (const auto& item : vec) {
    retv.append(item);
  }
  return retv;
}

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
                           internal::parse_pmp_np(np1),
                           internal::parse_pmp_np(np2));
}

//
template <typename PolygonMesh>
bool do_intersect_mesh_polyline(const PolygonMesh& pm, const py::list& lst,
                                const py::dict& parameters = py::dict()) {
  auto begin = stl_input_iterator<Point_3>(lst);
  auto end = stl_input_iterator<Point_3>(lst, false);
  std::vector<Point_3> polyline(begin, end);
  return PMP::do_intersect(pm, polyline, internal::parse_pmp_np(parameters));
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
  return PMP::do_intersect(pm, range, internal::parse_pmp_np(parameters));
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
  PMP::self_intersections(pm, it, internal::parse_pmp_np(parameters));

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
                          internal::parse_pmp_np(parameters));
  return result;
}

//
template <typename PolygonMesh>
bool does_self_intersect(const PolygonMesh& pm,
                         const py::dict& parameters = py::dict()) {
  return PMP::does_self_intersect(pm, internal::parse_pmp_np(parameters));
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
  return PMP::does_self_intersect(boost::make_iterator_range(begin, end), pm, internal::parse_pmp_np(parameters));
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
  PMP::connected_component(seed_face, pm, it, internal::parse_pmp_np(parameters));
  return lst;
}

//
template <typename PolygonMesh>
py::list connected_components(const PolygonMesh& pm,
                              const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;

  auto fccmap = CGAL::get(CGAL::dynamic_face_property_t<std::size_t>(), pm);
  auto num = PMP::connected_components(pm, fccmap,
                                       internal::parse_pmp_np(parameters));
  py::dict dct;
  for (auto f : CGAL::faces(pm)) dct[py::cast(f)] = py::cast(get(fccmap, f));
  py::list lst;
  lst.append(num);
  lst.append(std::move(dct));
  return lst;
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

//
template <typename PolygonMesh>
PolygonMesh corefine_and_compute_union(PolygonMesh& pm1, PolygonMesh& pm2,
                                      const py::dict& np1 = py::dict(),
                                      const py::dict& np2 = py::dict(),
                                      const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;

  Pm out;

  bool valid =
    PMP::corefine_and_compute_union(pm1, pm2, out,
                                    internal::parse_pmp_np(np1),
                                    internal::parse_pmp_np(np2),
                                    internal::parse_pmp_np(np_out));
  if (! valid) throw std::runtime_error("Cannot compute union!");
  return out;
}

//
template <typename PolygonMesh>
auto autorefine(PolygonMesh& tm,
                const py::dict& np = py::dict()) {
  PMP::autorefine(tm, internal::parse_pmp_np(np));
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
                                  internal::parse_pmp_np(parameters));
  // convert to a list of integers
  py::list result;
  for (const auto& t : out) {
    result.append(py::make_tuple(t.first, t.second, t.third));
  }
  return result;
}
// corefine_and_compute_intersection(      TriangleMesh& tm1,
//                                         TriangleMesh& tm2,
//                                         TriangleMesh& tm_out,
//                                   const NamedParameters1& np1 = parameters::default_values(),
//                                   const NamedParameters2& np2 = parameters::default_values(),
//                                   const NamedParametersOut& np_out = parameters::default_values())
template <typename PolygonMesh>
PolygonMesh corefine_and_compute_intersection(PolygonMesh& pm1, PolygonMesh& pm2,
                                             const py::dict& np1 = py::dict(),
                                             const py::dict& np2 = py::dict(),
                                             const py::dict& np_out = py::dict()) {
  using Pm = PolygonMesh;

  Pm out;

  bool valid =
    PMP::corefine_and_compute_intersection(pm1, pm2, out,
                                           internal::parse_pmp_np(np1),
                                           internal::parse_pmp_np(np2),
                                           internal::parse_pmp_np(np_out));
  if (! valid) throw std::runtime_error("Intersection was successfully computed but is non-manifold");
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
  internal::Named_params np = CGAL::parameters::default_values();

  // this is dirty, but it works, i'm open to other solutions that work and are more elegant
  // the use of auto avoids some probles with types
  bool faces_flag = parameters.contains("face_output_iterator") && py::cast<bool>(parameters["face_output_iterator"]);
  bool vertices_flag = parameters.contains("vertex_output_iterator") && py::cast<bool>(parameters["vertex_output_iterator"]);
  if (faces_flag && vertices_flag) {
    std::vector<Face_identifier> fids;
    std::vector<Vertex_identifier> vids;
    auto it1 = std::back_inserter(fids);
    auto it2 = std::back_inserter(vids);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge, internal::parse_pmp_np(parameters).face_output_iterator(it1).vertex_output_iterator(it2));
    for (const auto& fid : fids) facets.append(fid);
    for (const auto& vid : vids) vertices.append(vid);
    return py::make_tuple(std::get<0>(res), facets, vertices);
  } else if (faces_flag) {
    std::vector<Face_identifier> fids;
    auto it = std::back_inserter(fids);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge, internal::parse_pmp_np(parameters).face_output_iterator(it));
    for (const auto& fid : fids) facets.append(fid);
    return py::make_tuple(std::get<0>(res), facets, vertices);
  } else if (vertices_flag) {
    std::vector<Vertex_identifier> vids;
    auto it = std::back_inserter(vids);
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge, internal::parse_pmp_np(parameters).vertex_output_iterator(it));
    for (const auto& vid : vids) vertices.append(vid);
    return py::make_tuple(std::get<0>(res), facets, vertices);
  }
  else {
    auto res = PMP::triangulate_refine_and_fair_hole(pmesh, border_halfedge, internal::parse_pmp_np(parameters));
    return py::make_tuple(std::get<0>(res), facets, vertices);
  }
}

//
template <typename PolygonMesh>
auto triangulate_faces(PolygonMesh& pm,
                              const py::dict& parameters) {
  using Pm = PolygonMesh;

  // make a copy of the input mesh
  return PMP::triangulate_faces(pm, internal::parse_pmp_np(parameters));
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
                           internal::parse_pmp_np(parameters));
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
                           internal::parse_pmp_np(parameters));

}

template <typename PolygonMesh>
auto tangential_relaxation(PolygonMesh& tm,
                                  const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;

  PMP::tangential_relaxation(tm, internal::parse_pmp_np(parameters));
}

template <typename PolygonMesh>
double approximate_Hausdorff_distance(const PolygonMesh& tm1, const PolygonMesh& tm2,
                                      const py::dict& np1 = py::dict(),
                                      const py::dict& np2 = py::dict()) {
  using TAG = CGAL::Sequential_tag;
  return PMP::approximate_Hausdorff_distance<TAG>(tm1, tm2,
                                             internal::parse_pmp_np(np1),
                                             internal::parse_pmp_np(np2));
}

template <typename PolygonMesh>
auto surface_intersection(const PolygonMesh& tm1,
                          const PolygonMesh& tm2,
                          const py::dict& np1 = py::dict(),
                          const py::dict& np2 = py::dict()) {
  std::vector< std::vector<Point_3> > polylines;
  PMP::surface_intersection(tm1, tm2, std::back_inserter(polylines),
                            internal::parse_pmp_np(np1),
                            internal::parse_pmp_np(np2));
  py::list retv;
  for (const auto& item : polylines) {
    const auto linelist = vec2list(item);
    retv.append(linelist);
  }
  return retv;
}

template <typename PolygonMesh>
auto stitch_borders(PolygonMesh& pmesh,
                    const py::dict& np = py::dict()) {
  return PMP::stitch_borders(pmesh, internal::parse_pmp_np(np));
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

  std::array<bool, 4> res =
    PMP::corefine_and_compute_boolean_operations(
      pm1, pm2,
      {&out_union, &out_intersection, &tm1_minus_tm2, &tm2_minus_tm1},
      internal::parse_pmp_np(np1),
      internal::parse_pmp_np(np2),
      std::make_tuple(internal::parse_pmp_np(np_out_union),
                      internal::parse_pmp_np(np_out_intersection),
                      internal::parse_pmp_np(np_out_tm1_minus_tm2),
                      internal::parse_pmp_np(np_out_tm2_minus_tm1)));

  return py::make_tuple(res[0] ? py::cast(out_union) : py::none(),
                        res[1] ? py::cast(out_intersection) : py::none(),
                        res[2] ? py::cast(tm1_minus_tm2) : py::none(),
                        res[3] ? py::cast(tm2_minus_tm1) : py::none());

}

template <typename PolygonMesh>
Vector_3 compute_face_normal(const typename boost::graph_traits<PolygonMesh>::face_descriptor& f,
                             const PolygonMesh& sm,
                             const py::dict& np = py::dict()) {
  return PMP::compute_face_normal(f, sm, internal::parse_pmp_np(np));
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
  return PMP::compute_vertex_normal(v, sm, internal::parse_pmp_np(np));
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
void interpolated_corrected_curvatures(const PolygonMesh& pm,
                                       const py::dict& np = py::dict()) {
  PMP::interpolated_corrected_curvatures(pm, internal::parse_pmp_np(np));
}

template <typename PolygonMesh>
auto interpolated_corrected_curvatures_v(typename boost::graph_traits<PolygonMesh>::vertex_descriptor v,
                                       const PolygonMesh& pm,
                                       const py::dict& np = py::dict()) {
  // check if vertex_mean_curvature is defined and true
  bool vertex_mean_curvature = np.contains("vertex_mean_curvature") &&
    py::cast<bool>(np["vertex_mean_curvature"]);
  bool vertex_Gaussian_curvature = np.contains("vertex_Gaussian_curvature") &&
    py::cast<bool>(np["vertex_Gaussian_curvature"]);
  py::object vmc = py::none();
  py::object vgc = py::none();
  if (vertex_mean_curvature && vertex_Gaussian_curvature) {
    double vmc_d, vGc;
    PMP::interpolated_corrected_curvatures(v, pm,
    internal::parse_pmp_np(np).vertex_mean_curvature(std::ref(vmc_d)).vertex_Gaussian_curvature(std::ref(vGc)));
    vmc = py::cast(vmc_d);
    vgc = py::cast(vGc);
  }
  else if (vertex_mean_curvature) {
    double vmc_d;
    PMP::interpolated_corrected_curvatures(v, pm,
    internal::parse_pmp_np(np).vertex_mean_curvature(std::ref(vmc_d)));
    vmc = py::cast(vmc_d);
  }
  else if (vertex_Gaussian_curvature) {
    double vGc;
    PMP::interpolated_corrected_curvatures(v, pm,
    internal::parse_pmp_np(np).vertex_Gaussian_curvature(std::ref(vGc)));
    vgc = py::cast(vGc);
  }
  else {
    PMP::interpolated_corrected_curvatures(v, pm, internal::parse_pmp_np(np));
  }
  return py::make_tuple(vmc, vgc);

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
                        internal::parse_pmp_np(parameters));
  py::list result;
  for (const auto& hd : out) result.append(hd);
  return result;
}

// template<typename PolygonMesh
//        , typename EdgeRange
//        , typename SizingValue
//        , typename NamedParameters = parameters::Default_named_parameters
//        , typename = typename std::enable_if_t<std::is_convertible_v<SizingValue, double>>>
// void split_long_edges(const EdgeRange& edges
//                     , const SizingValue max_length
//                     , PolygonMesh& pmesh
//                     , const NamedParameters& np = parameters::default_values())

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
                        max_length, pmesh, internal::parse_pmp_np(parameters));
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

// void orient_triangle_soup_with_reference_triangle_mesh(const TriangleMesh& tm_ref,
//                                                        const PointRange& points,
//                                                        TriangleRange& triangles,
//                                                        const NamedParameters1& np1 = parameters::default_values(),
//                                                        const NamedParameters2& np2 = parameters::default_values())

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
                                                       internal::parse_pmp_np(np1), internal::parse_pmp_np(np2));
  return polyvec2polylist(polys);
}

py::tuple duplicate_non_manifold_edges_in_polygon_soup(const py::list& points,
                                                       const py::list& polygons) {
  auto pointvec = ptlist2ptvec(points);
  auto polyvec = polylist2polyvec(polygons);
  bool duplicated = PMP::duplicate_non_manifold_edges_in_polygon_soup(pointvec, polyvec);
  return py::make_tuple(polyvec2polylist(polyvec), ptvec2ptlist(pointvec), duplicated);
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
#endif

  m.def("connected_component", &pmp::connected_component<Pm>,
        py::arg("seed_face"), py::arg("pm"),
        py::arg("parameters") = py::dict());
  m.def("connected_components", &pmp::connected_components<Pm>,
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

  m.def("approximate_Hausdorff_distance", &pmp::approximate_Hausdorff_distance<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("surface_intersection", &pmp::surface_intersection<Pm>,
        py::arg("tm1"), py::arg("tm2"),
        py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("stitch_borders", &pmp::stitch_borders<Pm>,
        py::arg("pmesh"), py::arg("np") = py::dict());


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

  // default visitor
  using Dv = pmp::Default_visitor<Pm>;
  py::class_<Dv>(m, "Default_visitor")
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
    .def("set_before_subface_creations", &Cv::set_before_subface_creations)
    .def("set_after_subface_creations", &Cv::set_after_subface_creations)
    .def("set_before_subface_created", &Cv::set_before_subface_created)
    .def("set_after_subface_created", &Cv::set_after_subface_created)
    .def("set_before_face_copy", &Cv::set_before_face_copy)
    .def("set_after_face_copy", &Cv::set_after_face_copy)
    .def("set_before_edge_split", &Cv::set_before_edge_split)
    .def("set_edge_split", &Cv::set_edge_split)
    .def("set_after_edge_split", &Cv::set_after_edge_split)
    .def("set_add_retriangulation_edge", &Cv::set_add_retriangulation_edge)
    .def("set_before_edge_copy", &Cv::set_before_edge_copy)
    .def("set_after_edge_copy", &Cv::set_after_edge_copy)
    .def("set_before_edge_duplicated", &Cv::set_before_edge_duplicated)
    .def("set_after_edge_duplicated", &Cv::set_after_edge_duplicated)
    .def("set_intersection_edge_copy", &Cv::set_intersection_edge_copy)
    .def("set_new_vertex_added", &Cv::set_new_vertex_added)
    .def("set_intersection_point_detected", &Cv::set_intersection_point_detected)
    .def("set_before_vertex_copy", &Cv::set_before_vertex_copy)
    .def("set_after_vertex_copy", &Cv::set_after_vertex_copy)
    .def("set_start_filtering_intersections", &Cv::set_start_filtering_intersections)
    .def("set_progress_filtering_intersections", &Cv::set_progress_filtering_intersections)
    .def("set_end_filtering_intersections", &Cv::set_end_filtering_intersections)
    .def("set_start_triangulating_faces", &Cv::set_start_triangulating_faces)
    .def("set_triangulating_faces_step", &Cv::set_triangulating_faces_step)
    .def("set_end_triangulating_faces", &Cv::set_end_triangulating_faces)
    .def("set_start_handling_intersection_of_coplanar_faces", &Cv::set_start_handling_intersection_of_coplanar_faces)
    .def("set_intersection_of_coplanar_faces_step", &Cv::set_intersection_of_coplanar_faces_step)
    .def("set_end_handling_intersection_of_coplanar_faces", &Cv::set_end_handling_intersection_of_coplanar_faces)
    .def("set_start_handling_edge_face_intersections", &Cv::set_start_handling_edge_face_intersections)
    .def("set_edge_face_intersections_step", &Cv::set_edge_face_intersections_step)
    .def("set_end_handling_edge_face_intersections", &Cv::set_end_handling_edge_face_intersections)
    .def("set_start_building_output", &Cv::set_start_building_output)
    .def("set_end_building_output", &Cv::set_end_building_output)
    .def("set_filter_coplanar_edges", &Cv::set_filter_coplanar_edges)
    .def("set_detect_patches", &Cv::set_detect_patches)
    .def("set_classify_patches", &Cv::set_classify_patches)
    .def("set_classify_intersection_free_patches", &Cv::set_classify_intersection_free_patches)
    .def("set_out_of_place_operation", &Cv::set_out_of_place_operation)
    .def("set_in_place_operation", &Cv::set_in_place_operation)
    .def("set_in_place_operations", &Cv::set_in_place_operations)
    ;

}
