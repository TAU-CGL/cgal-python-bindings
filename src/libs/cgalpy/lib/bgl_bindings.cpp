// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/boost/graph/properties.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace cgalpy {
namespace bgl {

template <typename DynamicPropertyMapTag>
void register_dynamic_property_map_tag(py::module_& m, const std::string& name) {
  using Dpmt = DynamicPropertyMapTag;
  if (! add_attr<Dpmt>(m, name.c_str())) py::class_<Dpmt>(m, name.c_str()).def(py::init<>());
}

//!
template<typename T>
void register_dynamic_property_map_tags(py::module_& m, const std::string& prop_name) {
  using Dvpt = CGAL::dynamic_vertex_property_t<T>;
  register_dynamic_property_map_tag<Dvpt>(m, "dynamic_vertex_" + prop_name + "_property_t");

  using Dhpt = CGAL::dynamic_halfedge_property_t<T>;
  register_dynamic_property_map_tag<Dhpt>(m, "dynamic_halfedge_" + prop_name + "_property_t");

  using Dfpt = CGAL::dynamic_face_property_t<T>;
  register_dynamic_property_map_tag<Dfpt>(m, "dynamic_face_" + prop_name + "_property_t");

  using Dept = CGAL::dynamic_edge_property_t<T>;
  register_dynamic_property_map_tag<Dept>(m, "dynamic_edge_" + prop_name + "_property_t");
}

}
} // namespace cgalpy

//!
void export_bgl(py::module_& m) {
  using vertex_point_t = CGAL::vertex_point_t;
  using vertex_index_t = CGAL::vertex_index_t;
  using vertex_external_index_t = CGAL::vertex_external_index_t;
  using halfedge_index_t = CGAL::halfedge_index_t;
  using halfedge_external_index_t = CGAL::halfedge_external_index_t;
  using edge_index_t = CGAL::edge_index_t;
  using edge_external_index_t = CGAL::edge_external_index_t;
  using face_index_t = CGAL::face_index_t;
  using face_external_index_t = CGAL::face_external_index_t;

  const py::handle info_vertex_point = py::type<vertex_point_t>();
  if (info_vertex_point.is_valid()) m.attr("vertex_point_t") = info_vertex_point;
  else py::enum_<vertex_point_t>(m, "vertex_point_t").value("vertex_point", CGAL::vertex_point).export_values();

  const py::handle info_vertex_index = py::type<vertex_index_t>();
  if (info_vertex_index.is_valid()) m.attr("vertex_index_t") = info_vertex_index;
  else py::enum_<vertex_index_t>(m, "vertex_index_t").value("vertex_index", CGAL::vertex_index).export_values();

  const py::handle info_vertex_external_index = py::type<vertex_external_index_t>();
  if (info_vertex_external_index.is_valid()) m.attr("vertex_external_index_t") = info_vertex_external_index;
  else py::enum_<vertex_external_index_t>(m, "vertex_external_index_t").
         value("vertex_external_index", CGAL::vertex_external_index).export_values();

  const py::handle info_halfedge_index = py::type<halfedge_index_t>();
  if (info_halfedge_index.is_valid()) m.attr("halfedge_index_t") = info_halfedge_index;
  else py::enum_<halfedge_index_t>(m, "halfedge_index_t").value("halfedge_index", CGAL::halfedge_index).export_values();

  const py::handle info_halfedge_external_index = py::type<halfedge_external_index_t>();
  if (info_halfedge_external_index.is_valid()) m.attr("halfedge_external_index_t") = info_halfedge_external_index;
  else py::enum_<halfedge_external_index_t>(m, "halfedge_external_index_t").
         value("halfedge_external_index", CGAL::halfedge_external_index).export_values();

  const py::handle info_edge_index = py::type<edge_index_t>();
  if (info_edge_index.is_valid()) m.attr("edge_index_t") = info_edge_index;
  else py::enum_<edge_index_t>(m, "edge_index_t").value("edge_index", CGAL::edge_index).export_values();

  const py::handle info_edge_external_index = py::type<edge_external_index_t>();
  if (info_edge_external_index.is_valid()) m.attr("edge_external_index_t") = info_edge_external_index;
  else py::enum_<edge_external_index_t>(m, "edge_external_index_t").
         value("edge_external_index", CGAL::edge_external_index).export_values();

  const py::handle info_face_index = py::type<face_index_t>();
  if (info_face_index.is_valid()) m.attr("face_index_t") = info_face_index;
  else py::enum_<face_index_t>(m, "face_index_t").value("face_index", CGAL::face_index).export_values();

  const py::handle info_face_external_index = py::type<face_external_index_t>();
  if (info_face_external_index.is_valid()) m.attr("face_external_index_t") = info_face_external_index;
  else py::enum_<face_external_index_t>(m, "face_external_index_t").
         value("face_external_index", CGAL::face_external_index).export_values();

  cgalpy::bgl::register_dynamic_property_map_tags<bool>(m, "bool");
  cgalpy::bgl::register_dynamic_property_map_tags<std::size_t>(m, "size_t");
  cgalpy::bgl::register_dynamic_property_map_tags<int>(m, "int");
  cgalpy::bgl::register_dynamic_property_map_tags<double>(m, "double");
  cgalpy::bgl::register_dynamic_property_map_tags<Point_3>(m, "point_3");
  cgalpy::bgl::register_dynamic_property_map_tags<Vector_3>(m, "vector_3");
  cgalpy::bgl::register_dynamic_property_map_tags<CGAL::IO::Color>(m, "color");
  cgalpy::bgl::register_dynamic_property_map_tags<py::tuple>(m, "tuple");
  cgalpy::bgl::register_dynamic_property_map_tags<py::set>(m, "set");

  if constexpr (! std::is_same<double, FT>::value) cgalpy::bgl::register_dynamic_property_map_tags<FT>(m, "FT");
}
