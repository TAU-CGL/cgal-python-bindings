// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//

#include <CGAL/Mesh_facet_topology.h>

#include "CGALPY/helpers.hpp"
#include "CGALPY/internal.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace internal {

Named_params parse_named_parameters(const py::dict& params, Named_params cgal_parameters) {
  using Ft = double;
  // iterate throught all params and add them to the cgal_parameters
  for (const auto& item : params) {
    const std::string key = py::cast<std::string>(item.first);
    switch (Hash(key.c_str())) {
      case Hash("stream_precision"):
        cgal_parameters = cgal_parameters.stream_precision(py::cast<int>(item.second));
        break;
      case Hash("number_of_points_per_area_unit"):
        cgal_parameters = cgal_parameters.number_of_points_per_area_unit(py::cast<double>(item.second));
        break;
      case Hash("number_of_iterations"):
        cgal_parameters = cgal_parameters.number_of_iterations(py::cast<unsigned int>(item.second));
        break;
      case Hash("do_self_intersection_tests"):
        cgal_parameters = cgal_parameters.do_self_intersection_tests(py::cast<bool>(item.second));
        break;
      case Hash("do_orientation_tests"):
        cgal_parameters = cgal_parameters.do_orientation_tests(py::cast<bool>(item.second));
        break;
      case Hash("relax_constraints"):
        cgal_parameters = cgal_parameters.relax_constraints(py::cast<bool>(item.second));
        break;
      case Hash("dry_run"):
        cgal_parameters = cgal_parameters.dry_run(py::cast<bool>(item.second));
        break;
      case Hash("density_control_factor"):
        cgal_parameters = cgal_parameters.density_control_factor(py::cast<double>(item.second));
        break;
      case Hash("do_not_use_cubic_algorithm"):
        cgal_parameters = cgal_parameters.do_not_use_cubic_algorithm(py::cast<bool>(item.second));
        break;
      case Hash("threshold_distance"):
        cgal_parameters = cgal_parameters.threshold_distance(py::cast<double>(item.second));
        break;
      case Hash("use_2d_constrained_delaunay_triangulation"):
        cgal_parameters = cgal_parameters.use_2d_constrained_delaunay_triangulation(py::cast<bool>(item.second));
        break;
      case Hash("use_delaunay_triangulation"):
        cgal_parameters = cgal_parameters.use_delaunay_triangulation(py::cast<bool>(item.second));
        break;
      case Hash("fairing_continuity"):
        cgal_parameters = cgal_parameters.fairing_continuity(py::cast<unsigned int>(item.second));
        break;
      case Hash("use_angle_smoothing"):
        cgal_parameters = cgal_parameters.use_angle_smoothing(py::cast<bool>(item.second));
        break;
      case Hash("use_area_smoothing"):
        cgal_parameters = cgal_parameters.use_area_smoothing(py::cast<bool>(item.second));
        break;
      case Hash("use_safety_constraints"):
        cgal_parameters = cgal_parameters.use_safety_constraints(py::cast<bool>(item.second));
        break;
      case Hash("use_Delaunay_flips"):
        cgal_parameters = cgal_parameters.use_Delaunay_flips(py::cast<bool>(item.second));
        break;
      case Hash("do_project"):
        cgal_parameters = cgal_parameters.do_project(py::cast<bool>(item.second));
        break;
      case Hash("do_flip"):
        cgal_parameters = cgal_parameters.do_flip(py::cast<bool>(item.second));
        break;
      case Hash("do_collapse"):
        cgal_parameters = cgal_parameters.do_collapse(py::cast<bool>(item.second));
        break;
      case Hash("do_not_triangulate_faces"):
        cgal_parameters = cgal_parameters.do_not_triangulate_faces(py::cast<bool>(item.second));
        break;
      case Hash("do_sample_edges"):
        cgal_parameters = cgal_parameters.do_sample_edges(py::cast<bool>(item.second));
        break;
      case Hash("do_sample_vertices"):
        cgal_parameters = cgal_parameters.do_sample_vertices(py::cast<bool>(item.second));
        break;
      case Hash("do_sample_faces"):
        cgal_parameters = cgal_parameters.do_sample_faces(py::cast<bool>(item.second));
        break;
      case Hash("do_overlap_test_of_bounded_sides"):
        cgal_parameters = cgal_parameters.do_overlap_test_of_bounded_sides(py::cast<bool>(item.second));
        break;
      case Hash("do_split"):
        cgal_parameters = cgal_parameters.do_split(py::cast<bool>(item.second));
        break;
      case Hash("do_not_modify"):
        cgal_parameters = cgal_parameters.do_not_modify(py::cast<bool>(item.second));
        break;
      case Hash("do_scale"):
        cgal_parameters = cgal_parameters.do_scale(py::cast<bool>(item.second));
        break;
      case Hash("do_lock_mesh"):
        cgal_parameters = cgal_parameters.do_lock_mesh(py::cast<bool>(item.second));
        break;
      case Hash("do_simplify_border"):
        cgal_parameters = cgal_parameters.do_simplify_border(py::cast<bool>(item.second));
        break;
      case Hash("do_enforce_manifoldness"):
        cgal_parameters = cgal_parameters.do_enforce_manifoldness(py::cast<bool>(item.second));
        break;
      case Hash("do_freeze"):
        cgal_parameters = cgal_parameters.do_freeze(py::cast<bool>(item.second));
        break;
      case Hash("do_reset_c3t3"):
        cgal_parameters = cgal_parameters.do_reset_c3t3(py::cast<bool>(item.second));
        break;
      case Hash("use_random_uniform_sampling"):
        cgal_parameters = cgal_parameters.use_random_uniform_sampling(py::cast<bool>(item.second));
        break;
      case Hash("use_grid_sampling"):
        cgal_parameters = cgal_parameters.use_grid_sampling(py::cast<bool>(item.second));
        break;
      case Hash("use_monte_carlo_sampling"):
        cgal_parameters = cgal_parameters.use_monte_carlo_sampling(py::cast<bool>(item.second));
        break;
      case Hash("use_compact_clipper"):
        cgal_parameters = cgal_parameters.use_compact_clipper(py::cast<bool>(item.second));
        break;
      case Hash("use_relaxed_order"):
        cgal_parameters = cgal_parameters.use_relaxed_order(py::cast<bool>(item.second));
        break;
      case Hash("use_smoothing"):
        cgal_parameters = cgal_parameters.use_smoothing(py::cast<bool>(item.second));
        break;
      case Hash("use_bool_op_to_clip_surface"):
        cgal_parameters = cgal_parameters.use_bool_op_to_clip_surface(py::cast<bool>(item.second));
        break;
      case Hash("use_convex_hull"):
        cgal_parameters = cgal_parameters.use_convex_hull(py::cast<bool>(item.second));
        break;
      case Hash("size"):
        cgal_parameters = cgal_parameters.size(py::cast<size_t>(item.second));
        break;
      case Hash("protect_constraints"):
        cgal_parameters = cgal_parameters.protect_constraints(py::cast<bool>(item.second));
        break;
      case Hash("number_of_points_on_edges"):
        cgal_parameters = cgal_parameters.number_of_points_on_edges(py::cast<unsigned int>(item.second));
        break;
      case Hash("number_of_points_on_faces"):
        cgal_parameters = cgal_parameters.number_of_points_on_faces(py::cast<unsigned int>(item.second));
        break;
      case Hash("number_of_points_per_distance_unit"):
        cgal_parameters = cgal_parameters.number_of_points_per_distance_unit(py::cast<double>(item.second));
        break;
      case Hash("number_of_points_per_edge"):
        cgal_parameters = cgal_parameters.number_of_points_per_edge(py::cast<unsigned int>(item.second));
        break;
      case Hash("number_of_points_per_face"):
        cgal_parameters = cgal_parameters.number_of_points_per_face(py::cast<unsigned int>(item.second));
        break;
      case Hash("grid_spacing"):
        cgal_parameters = cgal_parameters.grid_spacing(py::cast<double>(item.second));
        break;
      case Hash("random_seed"):
        cgal_parameters = cgal_parameters.random_seed(py::cast<unsigned int>(item.second));
        break;
      case Hash("maximum_number_of_faces"):
        cgal_parameters = cgal_parameters.maximum_number_of_faces(py::cast<size_t>(item.second));
        break;
      case Hash("outward_orientation"):
        cgal_parameters = cgal_parameters.outward_orientation(py::cast<bool>(item.second));
        break;
      case Hash("maximum_number"):
        cgal_parameters = cgal_parameters.maximum_number(py::cast<unsigned int>(item.second));
        break;
      case Hash("erase_all_duplicates"):
        cgal_parameters = cgal_parameters.erase_all_duplicates(py::cast<bool>(item.second));
        break;
      case Hash("require_same_orientation"):
        cgal_parameters = cgal_parameters.require_same_orientation(py::cast<bool>(item.second));
        break;
      case Hash("apply_per_connected_component"):
        cgal_parameters = cgal_parameters.apply_per_connected_component(py::cast<bool>(item.second));
        break;
      case Hash("cap_threshold"):
        cgal_parameters = cgal_parameters.cap_threshold(py::cast<double>(item.second));
        break;
      case Hash("needle_threshold"):
        cgal_parameters = cgal_parameters.needle_threshold(py::cast<double>(item.second));
        break;
      case Hash("collapse_length_threshold"):
        cgal_parameters = cgal_parameters.collapse_length_threshold(py::cast<double>(item.second));
        break;
      case Hash("flip_triangle_height_threshold"):
        cgal_parameters = cgal_parameters.flip_triangle_height_threshold(py::cast<double>(item.second));
        break;
      case Hash("throw_on_self_intersection"):
        cgal_parameters = cgal_parameters.throw_on_self_intersection(py::cast<bool>(item.second));
        break;
      case Hash("clip_volume"):
        cgal_parameters = cgal_parameters.clip_volume(py::cast<bool>(item.second));
        break;
      case Hash("allow_self_intersections"):
        cgal_parameters = cgal_parameters.allow_self_intersections(py::cast<bool>(item.second));
        break;
      case Hash("first_index"):
        cgal_parameters = cgal_parameters.first_index(py::cast<std::size_t>(item.second));
        break;
      case Hash("snapping_tolerance"):
        cgal_parameters = cgal_parameters.snapping_tolerance(py::cast<double>(item.second));
        break;
      case Hash("verbose"):
        cgal_parameters = cgal_parameters.verbose(py::cast<bool>(item.second));
        break;
      case Hash("repair_polygon_soup"):
        cgal_parameters = cgal_parameters.repair_polygon_soup(py::cast<bool>(item.second));
        break;
      case Hash("cosine_of_maximum_angle"):
        cgal_parameters = cgal_parameters.cosine_of_maximum_angle(py::cast<Ft>(item.second));
        break;
      case Hash("match_faces"):
        cgal_parameters = cgal_parameters.match_faces(py::cast<bool>(item.second));
        break;
      case Hash("use_one_sided_hausdorff"):
        cgal_parameters = cgal_parameters.use_one_sided_hausdorff(py::cast<bool>(item.second));
        break;
      case Hash("bbox_scaling"):
        cgal_parameters = cgal_parameters.bbox_scaling(py::cast<double>(item.second));
        break;
      case Hash("maximum_distance"):
        cgal_parameters = cgal_parameters.maximum_distance(py::cast<Ft>(item.second));
        break;
      case Hash("maximum_angle"):
        cgal_parameters = cgal_parameters.maximum_angle(py::cast<Ft>(item.second));
        break;
      case Hash("postprocess_regions"):
        cgal_parameters = cgal_parameters.postprocess_regions(py::cast<bool>(item.second));
        break;
      case Hash("minimum_region_size"):
        cgal_parameters = cgal_parameters.minimum_region_size(py::cast<std::size_t>(item.second));
        break;
      case Hash("ball_radius"):
        cgal_parameters = cgal_parameters.ball_radius(py::cast<Ft>(item.second));
        break;
      case Hash("collapse_constraints"):
        cgal_parameters = cgal_parameters.collapse_constraints(py::cast<bool>(item.second));
        break;
      case Hash("number_of_relaxation_steps"):
        cgal_parameters = cgal_parameters.number_of_relaxation_steps(py::cast<unsigned int>(item.second));
        break;
      case Hash("features_angle_bound"):
        cgal_parameters = cgal_parameters.features_angle_bound(py::cast<Ft>(item.second));
        break;
      case Hash("k_neighbors"):
        cgal_parameters = cgal_parameters.k_neighbors(py::cast<std::size_t>(item.second));
        break;
      case Hash("angle_tolerance"):
        cgal_parameters = cgal_parameters.angle_tolerance(py::cast<Ft>(item.second));
        break;
      case Hash("maximum_offset"):
        cgal_parameters = cgal_parameters.maximum_offset(py::cast<Ft>(item.second));
        break;
      case Hash("regularize_parallelism"):
        cgal_parameters = cgal_parameters.regularize_parallelism(py::cast<bool>(item.second));
        break;
      case Hash("regularize_orthogonality"):
        cgal_parameters = cgal_parameters.regularize_orthogonality(py::cast<bool>(item.second));
        break;
      case Hash("regularize_coplanarity"):
        cgal_parameters = cgal_parameters.regularize_coplanarity(py::cast<bool>(item.second));
        break;
      case Hash("regularize_axis_symmetry"):
        cgal_parameters = cgal_parameters.regularize_axis_symmetry(py::cast<bool>(item.second));
        break;
      case Hash("symmetry_direction"):
        cgal_parameters = cgal_parameters.symmetry_direction(py::cast<Vector_3>(item.second));
        break;
      // case Hash("mesh_edge_size"): // can be a different type
      //   cgal_parameters = cgal_parameters.mesh_edge_size(py::cast<Ft>(item.second));
      //   break;
      // case Hash("mesh_facet_size"): // can be a different type
      //   cgal_parameters = cgal_parameters.mesh_facet_size(py::cast<Ft>(item.second));
      //   break;
      // case Hash("mesh_facet_angle"): // can be a different type
      //   cgal_parameters = cgal_parameters.mesh_facet_angle(py::cast<Ft>(item.second));
      //   break;
      // case Hash("mesh_facet_distance"): // can be a different type
      //   cgal_parameters = cgal_parameters.mesh_facet_distance(py::cast<Ft>(item.second));
      //   break;
      case Hash("polyline_constraints"):
        cgal_parameters = cgal_parameters.polyline_constraints(pmp::list2vec<Point_3>(py::cast<py::list>(item.second)));
        break;
      case Hash("mesh_facet_topology"):
        cgal_parameters = cgal_parameters.mesh_facet_topology(py::cast<CGAL::Mesh_facet_topology>(item.second));
        break;

        
      // default:
      //   throw std::invalid_argument("Unknown parameter: " + key);
      //   break;
    }
  }
  return cgal_parameters;
}

}  // namespace internal
