// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

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

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace PARMS = CGAL::parameters;

namespace pmp {

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
  return PMP::do_intersect(pm1, pm2);
}

//
template <typename PolygonMesh>
bool do_intersect_mesh_polyline(const PolygonMesh& pm, const py::list& lst) {
  auto begin = stl_input_iterator<Point_3>(lst);
  auto end = stl_input_iterator<Point_3>(lst, false);
  std::vector<Point_3> polyline(begin, end);
  return PMP::do_intersect(pm, polyline);
}

//
template <typename PolygonMesh>
bool do_intersect_mesh_polyline_range(const PolygonMesh& pm,
                                      const py::list& lsts) {
  std::vector<std::vector<Point_3>> range;
  for (const auto& lh : lsts) {
    auto begin1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh));
    auto end1 = stl_input_iterator<Point_3>(py::cast<py::list>(lh), false);
    range.emplace_back(begin1, end1);
  }
  return PMP::do_intersect(pm, range);
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
            { result.append(res); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  PMP::self_intersections(pm, it);
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
  PMP::self_intersections(boost::make_iterator_range(begin, end), pm, it);
  return result;
}

//
template <typename PolygonMesh>
bool does_self_intersect(const PolygonMesh& pm,
                         const py::dict& parameters = py::dict()) {
  return PMP::does_self_intersect(pm);
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
  return PMP::does_self_intersect(boost::make_iterator_range(begin, end), pm);
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
  PMP::connected_component(seed_face, pm, it);
  return lst;
}

//
template <typename PolygonMesh>
py::list connected_components(const PolygonMesh& pm,
                              const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;

  auto fccmap = CGAL::get(CGAL::dynamic_face_property_t<std::size_t>(), pm);
  auto num = PMP::connected_components(pm, fccmap);
  py::dict dct;
  for (auto f : CGAL::faces(pm)) dct[py::cast(f)] = py::cast(get(fccmap, f));
  py::list lst;
  lst.append(num);
  lst.append(std::move(dct));
  return lst;
}

//
template <typename PolygonMesh>
PolygonMesh corefine_and_compute_union(PolygonMesh& pm1, PolygonMesh& pm2,
                                       Corefine_visitor<PolygonMesh>& visitor) {
  using Pm = PolygonMesh;

  Pm out;

  bool valid =
    PMP::corefine_and_compute_union(pm1, pm2, out, PARMS::visitor(visitor));
  if (! valid) throw std::runtime_error("Cannot compute union!");
  return out;
}

//
template <typename PolygonMesh>
py::list triangulate_hole_polyline(const py::list& lst1, const py::list& lst2) {
  auto begin1 = stl_input_iterator<Point_3>(lst1);
  auto end1 = stl_input_iterator<Point_3>(lst1, false);
  std::vector<Point_3> polyline1(begin1, end1);
  auto begin2 = stl_input_iterator<Point_3>(lst2);
  auto end2 = stl_input_iterator<Point_3>(lst2, false);
  std::vector<Point_3> polyline2(begin2, end2);

  typedef CGAL::Triple<int, int, int> Triangle_int;
  std::vector<Triangle_int> out;
  PMP::triangulate_hole_polyline(polyline1, polyline2, std::back_inserter(out));
  // convert to a list of integers
  py::list result;
  for (const auto& t : out) {
    result.append(py::make_tuple(t.first, t.second, t.third));
  }
  return result;
}

template <typename PolygonMesh>
PolygonMesh triangulate_faces(const PolygonMesh& pm,
                              const py::dict& parameters) {
  using Pm = PolygonMesh;

  // make a copy of the input mesh
  Pm out(pm);

  // triangulate the faces
  // PMP::triangulate_faces(out);
  if (!PMP::triangulate_faces(out)) throw std::runtime_error("Could not triangulate faces!");
  return out;
}

//
template <typename PolygonMesh>
PolygonMesh isotropic_remeshing(const py::list& face_range,
                                double target_edge_length,
                                const PolygonMesh& pmesh,
                                const py::dict& parameters = py::dict()) {
  using Pm = PolygonMesh;
  using Gt = boost::graph_traits<Pm>;
  using Fd = typename Gt::face_descriptor;

  // copy the input mesh
  Pm out(pmesh);

  PMP::isotropic_remeshing(boost::make_iterator_range(stl_input_iterator<Fd>(face_range),
                                                      stl_input_iterator<Fd>(face_range, false)),
                           target_edge_length, out);
  return out;

}

// template <typename PolygonMesh, typename Face_normal_map, typename NamedParameters = parameters::Default_named_parameters>
// void compute_face_normals(const PolygonMesh& pmesh,
//                           Face_normal_map face_normals,
//                           const NamedParameters& np = parameters::default_values())
// {
//   typedef typename GetGeomTraits<PolygonMesh,NamedParameters>::type Kernel;
//
//   for(typename boost::graph_traits<PolygonMesh>::face_descriptor f : faces(pmesh))
//   {
//     typename Kernel::Vector_3 vec = compute_face_normal(f, pmesh, np);
//     put(face_normals, f, vec);
// #ifdef CGAL_PMP_COMPUTE_NORMAL_DEBUG_PP
//     std::cout << "normal at face " << f << " is " << get(face_normals, f) << std::endl;
// #endif
//   }
// }
// PMP::compute_normals(mesh, boost::make_assoc_property_map(vnormals),
//                            boost::make_assoc_property_map(fnormals));

// template <typename PolygonMesh>
// py::dict compute_face_normals(const PolygonMesh& pm) {
//   using Pm = PolygonMesh;
//   using Gt = boost::graph_traits<Pm>;
//   using Fd = typename Gt::face_descriptor;
//   using Vector = typename Kernel::Vector_3;
//
//   std::unordered_map<Fd, Vector> fnormals;
//   // Instead of std::map you may use std::unordered_map, boost::unordered_map
//   // or CGAL::Unique_hash_map
//   // CGAL::Unique_hash_map<face_descriptor,Vector> fnormals;
//   // boost::unordered_map<vertex_descriptor,Vector> vnormals;
//   auto pmap = boost::make_assoc_property_map(fnormals);
//   PMP::compute_face_normals(pm, pmap);
//   py::dict result;
//   for (const auto& [f, v] : fnormals) {
//     result[f] = v;
//   }
//   return result;
// }

} // namespace pmp

// Export Polygon_mesh_processing
void export_polygon_mesh_processing(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;
  using Polyline = std::vector<Kernel::Point_3>;
  using Np = CGAL::parameters::Default_named_parameters;

  using Np_t = bool;
  using Np_tag = CGAL::internal_np::all_default_t;
  using Np_base = CGAL::internal_np::No_property;
  using Np_class = CGAL::Named_function_parameters<Np_t, Np_tag, Np_base>;
  using Concurrency_tag = CGAL::Sequential_tag;

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
  m.def("do_intersect_mesh_polyline", &pmp::do_intersect_mesh_polyline<Pm>);
  m.def("do_intersect_mesh_polyline_range", &pmp::do_intersect_mesh_polyline_range<Pm>);

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

  m.def("corefine_and_compute_union", &pmp::corefine_and_compute_union<Pm>);

  m.def("triangulate_hole_polyline", &pmp::triangulate_hole_polyline<Pm>,
        py::arg("lst1"), py::arg("lst2") = py::list());

  // m.def("triangulate_faces", &pmp::triangulate_faces<Pm>,
  //       py::arg("face_range"), py::arg("pm"),
  //       py::arg("parameters") = py::dict());

  m.def("triangulate_faces", &pmp::triangulate_faces<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());

  // m.def("compute_face_normals", &pmp::compute_face_normals<Pm>,
  //       py::arg("pm"));

// Pmp.isotropic_remeshing(tm2.faces(), 0.05, tm2)
  m.def("isotropic_remeshing", &pmp::isotropic_remeshing<Pm>,
        py::arg("faces"), py::arg("target_edge_length"), py::arg("pmesh"),
        py::arg("parameters") = py::dict());


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
