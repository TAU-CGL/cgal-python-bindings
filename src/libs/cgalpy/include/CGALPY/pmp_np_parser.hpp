// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#ifndef PMP_NP_PARSER_HPP
#define PMP_NP_PARSER_HPP

#include <CGAL/Named_function_parameters.h>
#include <functional>

#include <boost/range/iterator_range_core.hpp>

#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/parse_named_parameters.hpp"

namespace py = nanobind;


namespace internal {

namespace PMP = CGAL::Polygon_mesh_processing;

using Point = Kernel::Point_3;

//!
template <typename K>
Named_params handle_vertex_principal_curvatures_and_directions(const py::handle& vpcad_handle, Named_params cgal_parameters) {
  auto vpcad = py::cast<PMP::Principal_curvatures_and_directions<K>>(vpcad_handle);
  return cgal_parameters.vertex_principal_curvatures_and_directions(std::ref(vpcad));
}

//!
template <typename Mesh>
auto parse_pmp_np(const py::dict& params,
                  Named_params cgal_parameters =
                    CGAL::parameters::verbose(false)) {
  using Vd = typename boost::graph_traits<Mesh>::vertex_descriptor;
  using Fd = typename boost::graph_traits<Mesh>::face_descriptor;
  using Hd = typename boost::graph_traits<Mesh>::halfedge_descriptor;
  using Ed = typename boost::graph_traits<Mesh>::edge_descriptor;
  using Ft = Kernel::FT;
  for (const auto& item : params) {
    const std::string key = py::cast<std::string>(item.first);
    switch (internal::Hash(key.c_str())) {
     case internal::Hash("geom_traits"):
      cgal_parameters = cgal_parameters.geom_traits(item.second);
      break;

     case internal::Hash("allow_move_functor"):
      cgal_parameters = cgal_parameters.allow_move_functor(py::cast<std::function<bool(Vd, Point, Point)>>(item.second));
      break;

     case internal::Hash("vertex_principal_curvatures_and_directions"):
      cgal_parameters = handle_vertex_principal_curvatures_and_directions<Kernel>(item.second, cgal_parameters);
      break;
      // case internal::Hash("vertex_point_map"):
      //   cgal_parameters = cgal_parameters.vertex_point_map(py::cast<typename Mesh::template Property_map<Vd, Point>>(item.second));
      //   break;
      // case internal::Hash("vertex_is_constrained_map"):
      //   cgal_parameters.vertex_is_constrained_map(py::cast<typename Mesh::template Property_map<Vd, bool>>(item.second));
      //   break;
      // case internal::Hash("vertex_index_map"):
      //   cgal_parameters = cgal_parameters.vertex_index_map(py::cast<typename Mesh::template Property_map<Vd, std::size_t>>(item.second));
      //   break;
      // case internal::Hash("face_index_map"):
      //   cgal_parameters = cgal_parameters.face_index_map(py::cast<typename Mesh::template Property_map<Fd, std::size_t>>(item.second));
      //   break;
      // case internal::Hash("edge_is_constrained_map"):
      //   cgal_parameters = cgal_parameters.edge_is_constrained_map(py::cast<typename Mesh::template Property_map<Ed, bool>>(item.second));
      //   break;
      // case internal::Hash("halfedge_index_map"):
      //   cgal_parameters = cgal_parameters.halfedge_index_map(py::cast<typename Mesh::template Property_map<Hd, std::size_t>>(item.second));
      //   break;
      // case internal::Hash("edge_index_map"):
      //   cgal_parameters = cgal_parameters.edge_index_map(py::cast<typename Mesh::template Property_map<Ed, std::size_t>>(item.second));
      //   break;
      // case internal::Hash("point_to_vertex_map"):
      //   cgal_parameters = cgal_parameters.point_to_vertex_map(py::cast<typename Mesh::template Property_map<Point, Vd>>(item.second));
      //   break;
      // case internal::Hash("vertex_normal_map"):
      //   cgal_parameters = cgal_parameters.vertex_normal_map(py::cast<typename Mesh::template Property_map<Vd, Point>>(item.second));
      //   break;
      // case internal::Hash("vertex_incident_patches_map"):
      //   cgal_parameters = cgal_parameters.vertex_incident_patches_map(py::cast<typename Mesh::template Property_map<Vd, int>>(item.second));
      //   break;
      // // case internal::Hash("face_size_map"): // this can have a different type
      // //   cgal_parameters = cgal_parameters.face_size_map(py::cast<typename Mesh::template Property_map<Fd, std::size_t>>(item.second));
      // //   break;
      // case internal::Hash("face_patch_map"):
      //   cgal_parameters = cgal_parameters.face_patch_map(py::cast<typename Mesh::template Property_map<Fd, std::size_t>>(item.second));
      //   break;
      // case internal::Hash("vertex_mean_curvature_map"):
      //   cgal_parameters = cgal_parameters.vertex_mean_curvature_map(py::cast<typename Mesh::template Property_map<Vd, Ft>>(item.second));
      //   break;
      // // case internal::Hash("vertex_principal_curvatures_and_directions_map"): // polyhedron can use that too




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

#endif
