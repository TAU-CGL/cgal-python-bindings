#ifndef PMP_NP_PARSER_HPP
#define PMP_NP_PARSER_HPP

#include <boost/range/iterator_range_core.hpp>

#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/polygon_mesh_processing_config.hpp"
#include "CGALPY/Corefine_visitor.hpp"
#include "CGALPY/Default_visitor.hpp"
#include "CGALPY/Non_manifold_output_visitor.hpp"

#include "internal.hpp"

namespace py = nanobind;

namespace internal {

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

// template <typename Mesh>
// Named_params handle_face_output_iterator(const py::handle& face_output_iterator, Named_params cgal_parameters) {
//   // second arg is a py::list
//   using Pm = Mesh;
//   using Gt = boost::graph_traits<Pm>;
//   using Fd = typename Gt::face_descriptor;
//   py::list face_output_iterator_list = py::cast<py::list>(face_output_iterator);
//
//   // create a back inserter
//   stl_output_iterator face_output_iterator_obj(face_output_iterator_list);
//   return cgal_parameters.face_output_iterator(face_output_iterator_obj);
// }

Named_params parse_pmp_np(const py::dict& params, Named_params cgal_parameters = CGAL::parameters::all_default()) {
  // iterate throught all params and add them to the cgal_parameters
  for (const auto& item : params) {
    const std::string key = py::cast<std::string>(item.first);
    switch (Hash(key.c_str())) {
      case Hash("visitor"):
        // handle visitor takes a pointer
        cgal_parameters = handle_visitor<Mesh>(item.second, cgal_parameters);
        break;
      // case Hash("face_output_iterator"):
      //   cgal_parameters = handle_face_output_iterator<Mesh>(item.second, cgal_parameters);
      //   break;
      case Hash("allow_move_functor"):
        // auto no_move = [](typename boost::graph_traits<Mesh>::vertex_descriptor, Point, Point)
        cgal_parameters = cgal_parameters.allow_move_functor(py::cast<std::function<bool(Vertex_descriptor, Point, Point)>>(item.second));
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

