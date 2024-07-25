#ifndef PMP_NP_PARSER_HPP
#define PMP_NP_PARSER_HPP

#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#include <boost/range/iterator_range_core.hpp>
#include <functional>

#include "CGALPY/Property_map.hpp"
#include "CGALPY/kernel_types.hpp"

#include "CGALPY/internal.hpp"

namespace py = nanobind;


namespace internal {

namespace PMP = CGAL::Polygon_mesh_processing;

typedef Kernel::Point_3 Point;
typedef CGAL::Named_function_parameters<bool, CGAL::internal_np::all_default_t> Named_params;

template <typename K>
Named_params handle_vertex_principal_curvatures_and_directions(const py::handle& vpcad_handle, Named_params cgal_parameters) {
  auto vpcad = py::cast<PMP::Principal_curvatures_and_directions<K>>(vpcad_handle);
  return cgal_parameters.vertex_principal_curvatures_and_directions(std::ref(vpcad));
}

template<typename Mesh>
Named_params parse_pmp_np(const py::dict& params, Named_params cgal_parameters = CGAL::parameters::all_default()) {
  using Vd = typename boost::graph_traits<Mesh>::vertex_descriptor;
  using Fd = typename boost::graph_traits<Mesh>::face_descriptor;
  using Hd = typename boost::graph_traits<Mesh>::halfedge_descriptor;
  using Ed = typename boost::graph_traits<Mesh>::edge_descriptor;
  using Ft = Kernel::FT;
  // iterate throught all params and add them to the cgal_parameters
  for (const auto& item : params) {
    const std::string key = py::cast<std::string>(item.first);
    switch (Hash(key.c_str())) {
      // case Hash("visitor"):
      //   cgal_parameters = handle_visitor<Mesh>(item.second, cgal_parameters);
      //   break;
      case Hash("allow_move_functor"):
        cgal_parameters = cgal_parameters.allow_move_functor(py::cast<std::function<bool(Vd, Point, Point)>>(item.second));
        break;
      case Hash("vertex_principal_curvatures_and_directions"):
        cgal_parameters = handle_vertex_principal_curvatures_and_directions<Kernel>(item.second, cgal_parameters);
        break;
      case Hash("vertex_point_map"):
        cgal_parameters = cgal_parameters.vertex_point_map(py::cast<boost::property_map<Mesh, CGAL::dynamic_vertex_property_t<Point_3>>>(item.second));
        break;
      // case Hash("vertex_index_map"):
      //   cgal_parameters = cgal_parameters.vertex_index_map(py::cast<typename Mesh::template Property_map<Vd, std::size_t>>(item.second));
      //   break;
      // case Hash("face_index_map"):
      //   cgal_parameters = cgal_parameters.face_index_map(py::cast<typename Mesh::template Property_map<Fd, std::size_t>>(item.second));
      //   break;
      // case Hash("edge_is_constrained_map"):
      //   cgal_parameters = cgal_parameters.edge_is_constrained_map(py::cast<typename Mesh::template Property_map<Ed, bool>>(item.second));
      //   break;
      // case Hash("halfedge_index_map"):
      //   cgal_parameters = cgal_parameters.halfedge_index_map(py::cast<typename Mesh::template Property_map<Hd, std::size_t>>(item.second));
      //   break;
      // case Hash("edge_index_map"):
      //   cgal_parameters = cgal_parameters.edge_index_map(py::cast<typename Mesh::template Property_map<Ed, std::size_t>>(item.second));
      //   break;
      // case Hash("point_to_vertex_map"):
      //   cgal_parameters = cgal_parameters.point_to_vertex_map(py::cast<typename Mesh::template Property_map<Point, Vd>>(item.second));
      //   break;
      // case Hash("vertex_normal_map"):
      //   cgal_parameters = cgal_parameters.vertex_normal_map(py::cast<typename Mesh::template Property_map<Vd, Point>>(item.second));
      //   break;
      // case Hash("vertex_incident_patches_map"):
      //   cgal_parameters = cgal_parameters.vertex_incident_patches_map(py::cast<typename Mesh::template Property_map<Vd, int>>(item.second));
      //   break;
      // // case Hash("face_size_map"): // this can have a different type
      // //   cgal_parameters = cgal_parameters.face_size_map(py::cast<typename Mesh::template Property_map<Fd, std::size_t>>(item.second));
      // //   break;
      // case Hash("face_patch_map"):
      //   cgal_parameters = cgal_parameters.face_patch_map(py::cast<typename Mesh::template Property_map<Fd, std::size_t>>(item.second));
      //   break;
      // case Hash("vertex_mean_curvature_map"):
      //   cgal_parameters = cgal_parameters.vertex_mean_curvature_map(py::cast<typename Mesh::template Property_map<Vd, Ft>>(item.second));
      //   break;
      // // case Hash("vertex_principal_curvatures_and_directions_map"): // polyhedron can use that too
        
      


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

