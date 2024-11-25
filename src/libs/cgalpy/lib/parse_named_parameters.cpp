// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <stdexcept>

#include <CGAL/Mesh_facet_topology.h>

#include "CGALPY/helpers.hpp"
#include "CGALPY/kernel_type.hpp"
#include "CGALPY/parse_named_parameters.hpp"

namespace py = nanobind;

namespace internal {

Named_params parse_named_parameters(const py::dict& params, Named_params np) {
  using Ft = typename Kernel::FT;
  // iterate throught all params and add them to np
  for (const auto& item : params) {
    const std::string key = py::cast<std::string>(item.first);
    try {
      switch (Hash(key.c_str())) {
       case Hash("stream_precision"):
        np = np.stream_precision(py::cast<int>(item.second));
        break;
       case Hash("number_of_points_per_area_unit"):
        np = np.number_of_points_per_area_unit(py::cast<double>(item.second));
        break;
       case Hash("number_of_iterations"):
        np = np.number_of_iterations(py::cast<unsigned int>(item.second));
        break;
       case Hash("do_self_intersection_tests"):
        np = np.do_self_intersection_tests(py::cast<bool>(item.second));
        break;
       case Hash("do_orientation_tests"):
        np = np.do_orientation_tests(py::cast<bool>(item.second));
        break;
       case Hash("relax_constraints"):
        np = np.relax_constraints(py::cast<bool>(item.second));
        break;
       case Hash("dry_run"):
        np = np.dry_run(py::cast<bool>(item.second));
        break;
       case Hash("density_control_factor"):
        np = np.density_control_factor(py::cast<double>(item.second));
        break;
       case Hash("do_not_use_cubic_algorithm"):
        np = np.do_not_use_cubic_algorithm(py::cast<bool>(item.second));
        break;
       case Hash("threshold_distance"):
        np = np.threshold_distance(py::cast<double>(item.second));
        break;
       case Hash("use_2d_constrained_delaunay_triangulation"):
        np = np.use_2d_constrained_delaunay_triangulation(py::cast<bool>(item.second));
        break;
       case Hash("use_delaunay_triangulation"):
        np = np.use_delaunay_triangulation(py::cast<bool>(item.second));
        break;
       case Hash("fairing_continuity"):
        np = np.fairing_continuity(py::cast<unsigned int>(item.second));
        break;
       case Hash("use_angle_smoothing"):
        np = np.use_angle_smoothing(py::cast<bool>(item.second));
        break;
       case Hash("use_area_smoothing"):
        np = np.use_area_smoothing(py::cast<bool>(item.second));
        break;
       case Hash("use_safety_constraints"):
        np = np.use_safety_constraints(py::cast<bool>(item.second));
        break;
       case Hash("use_Delaunay_flips"):
        np = np.use_Delaunay_flips(py::cast<bool>(item.second));
        break;
       case Hash("do_project"):
        np = np.do_project(py::cast<bool>(item.second));
        break;
       case Hash("do_flip"):
        np = np.do_flip(py::cast<bool>(item.second));
        break;
       case Hash("do_collapse"):
        np = np.do_collapse(py::cast<bool>(item.second));
        break;
       case Hash("do_not_triangulate_faces"):
        np = np.do_not_triangulate_faces(py::cast<bool>(item.second));
        break;
       case Hash("do_sample_edges"):
        np = np.do_sample_edges(py::cast<bool>(item.second));
        break;
       case Hash("do_sample_vertices"):
        np = np.do_sample_vertices(py::cast<bool>(item.second));
        break;
       case Hash("do_sample_faces"):
        np = np.do_sample_faces(py::cast<bool>(item.second));
        break;
       case Hash("do_overlap_test_of_bounded_sides"):
        np = np.do_overlap_test_of_bounded_sides(py::cast<bool>(item.second));
        break;
       case Hash("do_split"):
        np = np.do_split(py::cast<bool>(item.second));
        break;
       case Hash("do_not_modify"):
        np = np.do_not_modify(py::cast<bool>(item.second));
        break;
       case Hash("do_scale"):
        np = np.do_scale(py::cast<bool>(item.second));
        break;
       case Hash("do_lock_mesh"):
        np = np.do_lock_mesh(py::cast<bool>(item.second));
        break;
       case Hash("do_simplify_border"):
        np = np.do_simplify_border(py::cast<bool>(item.second));
        break;
       case Hash("do_enforce_manifoldness"):
        np = np.do_enforce_manifoldness(py::cast<bool>(item.second));
        break;
       case Hash("do_freeze"):
        np = np.do_freeze(py::cast<bool>(item.second));
        break;
       case Hash("do_reset_c3t3"):
        np = np.do_reset_c3t3(py::cast<bool>(item.second));
        break;
       case Hash("use_random_uniform_sampling"):
        np = np.use_random_uniform_sampling(py::cast<bool>(item.second));
        break;
       case Hash("use_grid_sampling"):
        np = np.use_grid_sampling(py::cast<bool>(item.second));
        break;
       case Hash("use_monte_carlo_sampling"):
        np = np.use_monte_carlo_sampling(py::cast<bool>(item.second));
        break;
       case Hash("use_compact_clipper"):
        np = np.use_compact_clipper(py::cast<bool>(item.second));
        break;
       case Hash("use_relaxed_order"):
        np = np.use_relaxed_order(py::cast<bool>(item.second));
        break;
       case Hash("use_smoothing"):
        np = np.use_smoothing(py::cast<bool>(item.second));
        break;
       case Hash("use_bool_op_to_clip_surface"):
        np = np.use_bool_op_to_clip_surface(py::cast<bool>(item.second));
        break;
       case Hash("use_convex_hull"):
        np = np.use_convex_hull(py::cast<bool>(item.second));
        break;
       case Hash("size"):
        np = np.size(py::cast<size_t>(item.second));
        break;
       case Hash("protect_constraints"):
        np = np.protect_constraints(py::cast<bool>(item.second));
        break;
       case Hash("number_of_points_on_edges"):
        np = np.number_of_points_on_edges(py::cast<unsigned int>(item.second));
        break;
       case Hash("number_of_points_on_faces"):
        np = np.number_of_points_on_faces(py::cast<unsigned int>(item.second));
        break;
       case Hash("number_of_points_per_distance_unit"):
        np = np.number_of_points_per_distance_unit(py::cast<double>(item.second));
        break;
       case Hash("number_of_points_per_edge"):
        np = np.number_of_points_per_edge(py::cast<unsigned int>(item.second));
        break;
       case Hash("number_of_points_per_face"):
        np = np.number_of_points_per_face(py::cast<unsigned int>(item.second));
        break;
       case Hash("grid_spacing"):
        np = np.grid_spacing(py::cast<double>(item.second));
        break;
       case Hash("random_seed"):
        np = np.random_seed(py::cast<unsigned int>(item.second));
        break;
       case Hash("maximum_number_of_faces"):
        np = np.maximum_number_of_faces(py::cast<size_t>(item.second));
        break;
       case Hash("outward_orientation"):
        np = np.outward_orientation(py::cast<bool>(item.second));
        break;
       case Hash("maximum_number"):
        np = np.maximum_number(py::cast<unsigned int>(item.second));
        break;
       case Hash("erase_all_duplicates"):
        np = np.erase_all_duplicates(py::cast<bool>(item.second));
        break;
       case Hash("require_same_orientation"):
        np = np.require_same_orientation(py::cast<bool>(item.second));
        break;
       case Hash("apply_per_connected_component"):
        np = np.apply_per_connected_component(py::cast<bool>(item.second));
        break;
       case Hash("cap_threshold"):
        np = np.cap_threshold(py::cast<double>(item.second));
        break;
       case Hash("needle_threshold"):
        np = np.needle_threshold(py::cast<double>(item.second));
        break;
       case Hash("collapse_length_threshold"):
        np = np.collapse_length_threshold(py::cast<double>(item.second));
        break;
       case Hash("flip_triangle_height_threshold"):
        np = np.flip_triangle_height_threshold(py::cast<double>(item.second));
        break;
       case Hash("throw_on_self_intersection"):
        np = np.throw_on_self_intersection(py::cast<bool>(item.second));
        break;
       case Hash("clip_volume"):
        np = np.clip_volume(py::cast<bool>(item.second));
        break;
       case Hash("allow_self_intersections"):
        np = np.allow_self_intersections(py::cast<bool>(item.second));
        break;
       case Hash("first_index"):
        np = np.first_index(py::cast<std::size_t>(item.second));
        break;
       case Hash("snapping_tolerance"):
        np = np.snapping_tolerance(py::cast<double>(item.second));
        break;
       case Hash("verbose"):
        np = np.verbose(py::cast<bool>(item.second));
        break;
       case Hash("repair_polygon_soup"):
        np = np.repair_polygon_soup(py::cast<bool>(item.second));
        break;
       case Hash("cosine_of_maximum_angle"):
        np = np.cosine_of_maximum_angle(py::cast<Ft>(item.second));
        break;
       case Hash("match_faces"):
        np = np.match_faces(py::cast<bool>(item.second));
        break;
       case Hash("use_one_sided_hausdorff"):
        np = np.use_one_sided_hausdorff(py::cast<bool>(item.second));
        break;
       case Hash("bbox_scaling"):
        np = np.bbox_scaling(py::cast<double>(item.second));
        break;
       case Hash("maximum_distance"):
        np = np.maximum_distance(py::cast<Ft>(item.second));
        break;
       case Hash("maximum_angle"):
        np = np.maximum_angle(py::cast<Ft>(item.second));
        break;
       case Hash("postprocess_regions"):
        np = np.postprocess_regions(py::cast<bool>(item.second));
        break;
       case Hash("minimum_region_size"): // different types for this
        np = np.minimum_region_size(py::cast<std::size_t>(item.second));
        break;
       case Hash("ball_radius"):
        np = np.ball_radius(py::cast<Ft>(item.second));
        break;
       case Hash("collapse_constraints"):
        np = np.collapse_constraints(py::cast<bool>(item.second));
        break;
       case Hash("number_of_relaxation_steps"):
        np = np.number_of_relaxation_steps(py::cast<unsigned int>(item.second));
        break;
       case Hash("features_angle_bound"):
        np = np.features_angle_bound(py::cast<Ft>(item.second));
        break;
       case Hash("k_neighbors"):
        np = np.k_neighbors(py::cast<std::size_t>(item.second));
        break;
       case Hash("angle_tolerance"):
        np = np.angle_tolerance(py::cast<Ft>(item.second));
        break;
       case Hash("maximum_offset"):
        np = np.maximum_offset(py::cast<Ft>(item.second));
        break;
       case Hash("regularize_parallelism"):
        np = np.regularize_parallelism(py::cast<bool>(item.second));
        break;
       case Hash("regularize_orthogonality"):
        np = np.regularize_orthogonality(py::cast<bool>(item.second));
        break;
       case Hash("regularize_coplanarity"):
        np = np.regularize_coplanarity(py::cast<bool>(item.second));
        break;
       case Hash("regularize_axis_symmetry"):
        np = np.regularize_axis_symmetry(py::cast<bool>(item.second));
        break;
       case Hash("symmetry_direction"):
        np = np.symmetry_direction(py::cast<Kernel::Vector_3>(item.second));
        break;
      // case Hash("mesh_edge_size"): // can be a different type
      //   np = np.mesh_edge_size(py::cast<Ft>(item.second));
      //   break;
      // case Hash("mesh_facet_size"): // can be a different type
      //   np = np.mesh_facet_size(py::cast<Ft>(item.second));
      //   break;
      // case Hash("mesh_facet_angle"): // can be a different type
      //   np = np.mesh_facet_angle(py::cast<Ft>(item.second));
      //   break;
      // case Hash("mesh_facet_distance"): // can be a different type
      //   np = np.mesh_facet_distance(py::cast<Ft>(item.second));
      //   break;
       case Hash("polyline_constraints"):
        np = np.polyline_constraints(pmp::list2vec<Kernel::Point_3>(py::cast<py::list>(item.second)));
        break;
       case Hash("mesh_facet_topology"):
        np = np.mesh_facet_topology(py::cast<CGAL::Mesh_facet_topology>(item.second));
        break;
       case Hash("sharpness_angle"):
        np = np.sharpness_angle(py::cast<double>(item.second));
        break;
       case Hash("neighbor_radius"):
        np = np.neighbor_radius(py::cast<double>(item.second));
        break;
       case Hash("attraction_factor"):
        np = np.attraction_factor(py::cast<double>(item.second));
        break;
       case Hash("edge_sensitivity"):
        np = np.edge_sensitivity(py::cast<double>(item.second));
        break;
       case Hash("min_points_per_cell"):
        np = np.min_points_per_cell(py::cast<unsigned int>(item.second));
        break;
       case Hash("maximum_variation"):
        np = np.maximum_variation(py::cast<double>(item.second));
        break;
       case Hash("degree_fitting"):
        np = np.degree_fitting(py::cast<unsigned int>(item.second));
        break;
       case Hash("degree_monge"):
        np = np.degree_monge(py::cast<unsigned int>(item.second));
        break;
       case Hash("number_of_samples"):
        np = np.number_of_samples(py::cast<unsigned int>(item.second));
        break;
       case Hash("maximum_normal_deviation"):
        np = np.maximum_normal_deviation(py::cast<double>(item.second));
        break;
       case Hash("accuracy"):
        np = np.accuracy(py::cast<double>(item.second));
        break;
       case Hash("overlap"):
        np = np.overlap(py::cast<double>(item.second));
        break;
       case Hash("maximum_running_time"):
        np = np.maximum_running_time(py::cast<double>(item.second));
        break;
       case Hash("threshold_percent"):
        np = np.threshold_percent(py::cast<double>(item.second));
        break;
       case Hash("select_percentage"):
        np = np.select_percentage(py::cast<double>(item.second));
        break;
       case Hash("require_uniform_sampling"):
        np = np.require_uniform_sampling(py::cast<bool>(item.second));
        break;
      }
    }
    catch (const std::exception& e) {
      throw std::invalid_argument("Error parsing parameter: " + key);
    }
  }
  return np;
}

}  // namespace internal
