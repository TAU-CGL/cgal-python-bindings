#include "internal.hpp"

#ifdef CGALPY_POLYGON_MESH_PROCESSING_TYPES_HPP
// #include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/polygon_mesh_processing_config.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#endif

namespace py = nanobind;

namespace internal {

#ifdef CGALPY_POLYGON_MESH_PROCESSING_TYPES_HPP
typedef pmp::Polygonal_mesh Mesh;
typedef Kernel::Point_3 Point;
typedef boost::graph_traits<Mesh>::vertex_descriptor Vertex_descriptor;
template <typename Mesh>
Named_params handle_visitor(const py::handle& visitor, Named_params cgal_parameters) {
  // nanobind throws a runtime error even when casting to parent class
  try {
    return cgal_parameters.visitor(py::cast<pmp::Default_visitor<Mesh>>(visitor));
  }
  catch (const std::exception&) {
  }
  try {
    return cgal_parameters.visitor(py::cast<pmp::Corefine_visitor<Mesh>>(visitor));
  }
  catch (const std::exception&) {
  }
  try {
    return cgal_parameters.visitor(py::cast<pmp::Non_manifold_output_visitor<Mesh>>(visitor));
  }
  catch (const std::exception&) {
  }
  throw std::invalid_argument("Unknown visitor type");
}

template <typename Mesh>
Named_params handle_face_output_iterator(py::handle& face_output_iterator, Named_params cgal_parameters) {
  // try {
  //   return cgal_parameters.face_output_iterator(py::cast<pmp::Face_output_iterator<Mesh>>(face_output_iterator));
  // }
  // catch (const std::exception&) {
  // }
  // throw std::invalid_argument("Unknown face_output_iterator type");
  return cgal_parameters;
}
#endif // CGALPY_POLYGON_MESH_PROCESSING_BINDINGS_HPP

Named_params parse_named_parameters(const py::dict& params) {
  Named_params cgal_parameters = CGAL::parameters::all_default();
  // iterate throught all params and add them to the cgal_parameters
  for (const auto& item : params) {
    const std::string key = py::cast<std::string>(item.first);
    switch (Hash(key.c_str())) {
      case Hash("stream_precision"):
        cgal_parameters = cgal_parameters.stream_precision(py::cast<int>(item.second));
        break;
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
      case Hash("visitor"):
        // handle visitor takes a pointer
        cgal_parameters = handle_visitor<Mesh>(item.second, cgal_parameters);
        break;
      case Hash("face_output_iterator"):
        cgal_parameters = handle_face_output_iterator<Mesh>(item.second, cgal_parameters);
        break;
      case Hash("allow_move_functor"):
        // auto no_move = [](typename boost::graph_traits<Mesh>::vertex_descriptor, Point, Point)
        cgal_parameters = cgal_parameters.allow_move_functor(py::cast<std::function<bool(Vertex_descriptor, Point, Point)>>(item.second));
        break;
    #endif
      default:
        throw std::invalid_argument("Unknown parameter: " + key);
        break;
    }
  }
  return cgal_parameters;
}


}  // namespace internal

