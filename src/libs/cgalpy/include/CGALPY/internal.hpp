#ifndef CGALPY_PARSE_NAMED_PARAMS_HPP
#define CGALPY_PARSE_NAMED_PARAMS_HPP

#include <CGAL/Named_function_parameters.h>
#include <CGAL/exceptions.h>
#include <csignal>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include "kernel_types.hpp"
#ifdef CGALPY_POLYGON_MESH_PROCESSING_TYPES_HPP
#include "CGALPY/polygon_mesh_processing_config.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#endif




namespace py = nanobind;

namespace internal {

typedef CGAL::Named_function_parameters<bool, CGAL::internal_np::all_default_t> Named_params;

// string hash function from: https://gist.github.com/EvanMcBroom/2a9bed888c2755153a9616aa7ae1f79a
template <typename _T>
unsigned int constexpr Hash(_T const* input) {
    return *input ? static_cast<unsigned int>(*input) + 33 * Hash(input + 1) : 5381;
}

#ifdef CGALPY_POLYGON_MESH_PROCESSING_TYPES_HPP
template <typename Mesh>
Named_params handle_visitor(const py::handle& visitor, Named_params cgal_parameters) {
  // nanobind throws a runtime error even when casting to parent class
  try {
    pmp::Default_visitor<Mesh> v = py::cast<pmp::Default_visitor<Mesh>>(visitor);
    return cgal_parameters.visitor(v);
  }
  catch (const std::exception&) {
  }
  try {
    pmp::Corefine_visitor<Mesh> v = py::cast<pmp::Corefine_visitor<Mesh>>(visitor);
    return cgal_parameters.visitor(v);
  }
  catch (const std::exception&) {
  }
  return cgal_parameters;
}
#endif


Named_params parse_named_parameters(const py::dict& params) {
  Named_params cgal_parameters = CGAL::parameters::all_default();
  // iterate throught all params and add them to the cgal_parameters
  for (const auto& item : params) {
    const std::string key = py::cast<std::string>(item.first);
    switch (Hash(key.c_str())) {
      case Hash("number_of_points_per_area_unit"):
        cgal_parameters = cgal_parameters.number_of_points_per_area_unit(py::cast<int>(item.second));
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
    #ifdef CGALPY_POLYGON_MESH_PROCESSING_TYPES_HPP
      case Hash("allow_move_functor"):
        // auto no_move = [](typename boost::graph_traits<Mesh>::vertex_descriptor, Point, Point)
        typedef Kernel::Point_3 Point;
        typedef pmp::Polygonal_mesh Mesh;
        typedef boost::graph_traits<Mesh>::vertex_descriptor Vertex_descriptor;
        cgal_parameters = cgal_parameters.allow_move_functor(py::cast<std::function<bool(Vertex_descriptor, Point, Point)>>(item.second));
        break;
    #endif
    #ifdef CGALPY_POLYGON_MESH_PROCESSING_TYPES_HPP
      case Hash("visitor"):
        cgal_parameters = handle_visitor<Mesh>(item.second, cgal_parameters);
        break;
    #endif
      default:
        throw std::invalid_argument("Unknown parameter: " + key);
        break;
    }
  }
  return cgal_parameters;
}

} // namespace parse_params

#endif //CGALPY_PARSE_PARAMS_HPP
