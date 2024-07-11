#ifndef CGALPY_PARSE_NAMED_PARAMS_HPP
#define CGALPY_PARSE_NAMED_PARAMS_HPP

#include <CGAL/Named_function_parameters.h>
#include <CGAL/exceptions.h>
#include <csignal>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>


namespace py = nanobind;

namespace internal {

CGAL::Named_function_parameters<bool, CGAL::internal_np::all_default_t> parse_named_parameters(const py::dict& params) {
  CGAL::Named_function_parameters<bool, CGAL::internal_np::all_default_t> cgal_parameters = CGAL::parameters::all_default();
  // iterate throught all params and add them to the cgal_parameters
  for (const auto& item : params) {
    std::string key = py::cast<std::string>(item.first);
    if (key == "number_of_points_per_area_unit") {
      int value = py::cast<int>(item.second);
      cgal_parameters = cgal_parameters.number_of_points_per_area_unit(value);
    }
    else if (key == "number_of_iterations") {
      unsigned int value = py::cast<unsigned int>(item.second);
      cgal_parameters = cgal_parameters.number_of_iterations(value);
    }
    else if (key == "do_self_intersection_tests") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_self_intersection_tests(value);
    }
    else if (key == "do_orientation_tests") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_orientation_tests(value);
    }
    else if (key == "relax_constraints") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.relax_constraints(value);
    }
    else if (key == "dry_run") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.dry_run(value);
    }
    else if (key == "density_control_factor") {
      double value = py::cast<double>(item.second);
      cgal_parameters = cgal_parameters.density_control_factor(value);
    }
    else if (key == "do_not_use_cubic_algorithm") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_not_use_cubic_algorithm(value);
    }
    else if (key == "threshold_distance") {
      double value = py::cast<double>(item.second);
      cgal_parameters = cgal_parameters.threshold_distance(value);
    }
    else if (key == "use_2d_constrained_delaunay_triangulation") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_2d_constrained_delaunay_triangulation(value);
    }
    else if (key == "use_delaunay_triangulation") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_delaunay_triangulation(value);
    }
    else if (key == "fairing_continuity") {
      unsigned int value = py::cast<unsigned int>(item.second);
      cgal_parameters = cgal_parameters.fairing_continuity(value);
    }
    else if (key == "use_angle_smoothing") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_angle_smoothing(value);
    }
    else if (key == "use_area_smoothing") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_area_smoothing(value);
    }
    else if (key == "use_safety_constraints") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_safety_constraints(value);
    }
    else if (key == "use_Delaunay_flips") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_Delaunay_flips(value);
    }
    else if (key == "do_project") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_project(value);
    }
    else if (key == "do_flip") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_flip(value);
    }
    else if (key == "do_collapse") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_collapse(value);
    }
    else if (key == "do_not_triangulate_faces") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_not_triangulate_faces(value);
    }
    else if (key == "do_sample_edges") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_sample_edges(value);
    }
    else if (key == "do_sample_vertices") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_sample_vertices(value);
    }
    else if (key == "do_sample_faces") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_sample_faces(value);
    }
    else if (key == "do_overlap_test_of_bounded_sides") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_overlap_test_of_bounded_sides(value);
    }
    else if (key == "do_split") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_split(value);
    }
    else if (key == "do_not_modify") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_not_modify(value);
    }
    else if (key == "do_scale") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_scale(value);
    }
    else if (key == "do_lock_mesh") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_lock_mesh(value);
    }
    else if (key == "do_simplify_border") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_simplify_border(value);
    }
    else if (key == "do_enforce_manifoldness") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_enforce_manifoldness(value);
    }
    else if (key == "do_freeze") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_freeze(value);
    }
    else if (key == "do_reset_c3t3") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.do_reset_c3t3(value);
    }
    else if (key == "use_random_uniform_sampling") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_random_uniform_sampling(value);
    }
    else if (key == "use_grid_sampling") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_grid_sampling(value);
    }
    else if (key == "use_monte_carlo_sampling") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_monte_carlo_sampling(value);
    }
    else if (key == "use_compact_clipper") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_compact_clipper(value);
    }
    else if (key == "use_relaxed_order") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_relaxed_order(value);
    }
    else if (key == "use_smoothing") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_smoothing(value);
    }
    else if (key == "use_bool_op_to_clip_surface") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_bool_op_to_clip_surface(value);
    }
    else if (key == "use_convex_hull") {
      bool value = py::cast<bool>(item.second);
      cgal_parameters = cgal_parameters.use_convex_hull(value);
    }
    else if (key == "size") {
      size_t value = py::cast<size_t>(item.second);
      cgal_parameters = cgal_parameters.size(value);
    }

    else {
      std::cerr << "Unknown parameter: " << key << "\n";
    }
  }
  return cgal_parameters;
}

} // namespace parse_params

#endif //CGALPY_PARSE_PARAMS_HPP
