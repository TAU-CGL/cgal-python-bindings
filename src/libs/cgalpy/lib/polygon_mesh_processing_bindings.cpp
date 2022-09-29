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

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/connected_components.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/Corefine_visitor.hpp"

namespace py = nanobind;
namespace PMP = CGAL::Polygon_mesh_processing;
namespace PARMS = CGAL::parameters;

namespace pmp {

//
template <typename PolygonMesh>
py::list
connected_component(typename boost::graph_traits<PolygonMesh>::face_descriptor
                      seed_face,
                    const PolygonMesh& pm, py::dict parameters) {
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
                              py::dict parameters = py::dict()) {
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

}

// Export Polygon_mesh_processing
void export_polygon_mesh_processing(py::module_& m) {
  using Pm = pmp::Polygonal_mesh;

  m.def("connected_component", &pmp::connected_component<Pm>,
        py::arg("seed_face"), py::arg("pm"),
        py::arg("parameters") = py::dict());
  m.def("connected_components", &pmp::connected_components<Pm>,
        py::arg("pm"), py::arg("parameters") = py::dict());

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

  m.def("corefine_and_compute_union", &pmp::corefine_and_compute_union<Pm>);
}
