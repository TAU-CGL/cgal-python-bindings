#ifndef PMP_NP_PARSER_HPP
#define PMP_NP_PARSER_HPP

#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#include <boost/range/iterator_range_core.hpp>
#include <functional>

#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/polygon_mesh_processing_config.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#include "CGALPY/Default_visitor.hpp"
#include "CGALPY/Default_orientation_visitor.hpp"
#include "CGALPY/Non_manifold_output_visitor.hpp"
#include "CGALPY/kernel_types.hpp"

#include "CGALPY/internal.hpp"

namespace py = nanobind;


namespace internal {

namespace PMP = CGAL::Polygon_mesh_processing;

typedef Kernel::Point_3 Point;
typedef CGAL::Named_function_parameters<bool, CGAL::internal_np::all_default_t> Named_params;

template <typename Mesh>
Named_params handle_visitor(const py::handle& visitor, Named_params cgal_parameters) {
  using Vertex_descriptor = typename boost::graph_traits<Mesh>::vertex_descriptor;
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
  try {
    return cgal_parameters.visitor(py::cast<pmp::Dummy_default_orienatation_visitor>(visitor));
  }
  catch (const std::exception&) {
  }
  throw std::invalid_argument("Unknown visitor type");
}

template <typename K>
Named_params handle_vertex_principal_curvatures_and_directions(const py::handle& vpcad_handle, Named_params cgal_parameters) {
  auto vpcad = py::cast<PMP::Principal_curvatures_and_directions<K>>(vpcad_handle);
  return cgal_parameters.vertex_principal_curvatures_and_directions(std::ref(vpcad));
}

template<typename Mesh>
Named_params parse_pmp_np(const py::dict& params, Named_params cgal_parameters = CGAL::parameters::all_default()) {
  using Vertex_descriptor = typename boost::graph_traits<Mesh>::vertex_descriptor;
  // iterate throught all params and add them to the cgal_parameters
  for (const auto& item : params) {
    const std::string key = py::cast<std::string>(item.first);
    switch (Hash(key.c_str())) {
      case Hash("visitor"):
        // handle visitor takes a pointer
        cgal_parameters = handle_visitor<Mesh>(item.second, cgal_parameters);
        break;
      case Hash("allow_move_functor"):
        cgal_parameters = cgal_parameters.allow_move_functor(py::cast<std::function<bool(Vertex_descriptor, Point, Point)>>(item.second));
        break;
      case Hash("vertex_principal_curvatures_and_directions"):
        cgal_parameters = handle_vertex_principal_curvatures_and_directions<Kernel>(item.second, cgal_parameters);
        break;
      // default:
      //   throw std::invalid_argument("Unknown parameter: " + key);
      //   break;
    }
    // pass the params to a generic function
    cgal_parameters = parse_named_parameters(params, cgal_parameters);
  }
  return cgal_parameters;
}

} // namespace internal

#endif // PMP_NP_PARSER_HPP

