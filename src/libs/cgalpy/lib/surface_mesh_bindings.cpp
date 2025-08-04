// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <string>

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/vector_property_map.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/pair.h>

#include <CGAL/boost/graph/generators.h>
#include <CGAL/property_map.h>
#include <CGAL/Dynamic_property_map.h>
#include <CGAL/boost/graph/selection.h>
#include <CGAL/boost/graph/Face_filtered_graph.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/IO/polygon_soup_io.h>

//! \todo move to polygon_mesh_processing_bindings.cpp because it depends on Eigen
#ifdef CGALPY_POLYGON_MESH_PROCESSING_BINDINGS
#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#endif

#include <CGAL/IO/Color.h>
#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_surface_mesh.h>
#endif

#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/bgl.hpp"
#include "CGALPY/bgl_global.hpp"
#include "CGALPY/export_boost_mesh_utils.hpp"
#include "CGALPY/export_mesh_iterators.hpp"
#include "CGALPY/export_mesh_selection_functions.hpp"
#include "CGALPY/export_mesh_helpers.hpp"
#include "CGALPY/export_mesh_partitioning_operations.hpp"
#include "CGALPY/export_mesh_partitioning_operations.hpp"
#include "CGALPY/export_property_map.hpp"
#include "CGALPY/generator_functions.hpp"
#include "CGALPY/get.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/surface_mesh_types.hpp"

namespace py = nanobind;

namespace sm {

//! Add a face from a list of vertices.
template <typename SurfaceMesh>
typename SurfaceMesh::Face_index
add_face(SurfaceMesh& sm, const std::vector<typename SurfaceMesh::Vertex_index>& lst) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  return sm.add_face(lst);
}

//!
template <typename SurfaceMesh>
auto has_valid_index_v(const SurfaceMesh& sm, typename SurfaceMesh::Vertex_index& vi)
{ return sm.is_valid(vi); }

//!
template <typename SurfaceMesh>
auto has_valid_index_e(const SurfaceMesh& sm, typename SurfaceMesh::Edge_index& ei)
{ return sm.is_valid(ei); }

//
template <typename SurfaceMesh>
auto has_valid_index_h(const SurfaceMesh& sm, typename SurfaceMesh::Halfedge_index& hi)
{ return sm.is_valid(hi); }

//
template <typename SurfaceMesh>
auto has_valid_index_f(const SurfaceMesh& sm, typename SurfaceMesh::Face_index& fi)
{ return sm.is_valid(fi); }

//!
template <typename Map_type>
void register_map(py::module_& m, const std::string& map_name) {
  py::class_<Map_type>(m, map_name.c_str())
    .def(py::init<>())
    .def_ro("map_", &Map_type::map_)
    ;
}

//!
template <typename Dp, typename Mesh>
void register_map_get(py::module_& m, const std::string& prop_name) {
  py::class_<Dp> prop(m, prop_name.c_str());
  prop.def(py::init<>());
  m.def("get", &internal::get<Dp, Mesh>, py::arg("property_map"), py::arg("sm"));
}

//!
template <typename Pm, typename P>
void edge_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Ed = typename boost::graph_traits<Pm>::edge_descriptor;
  using dp = CGAL::dynamic_edge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Ed& e) { return get(p, e); },
        py::arg("property_map"), py::arg("edge_descriptor"));
}

//!
template <typename Pm, typename P>
void edge_bool_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Ed = typename boost::graph_traits<Pm>::edge_descriptor;
  using dp = CGAL::dynamic_edge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get",
        [](const map_type& p, const Ed& e) { return py::bool_(get(p, e)); },
        py::arg("property_map"), py::arg("edge_descriptor"));
}

//!
template <typename Pm, typename P>
void face_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Fd = typename boost::graph_traits<Pm>::face_descriptor;
  using dp = CGAL::dynamic_face_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Fd& f) { return get(p, f); },
        py::arg("property_map"), py::arg("face_descriptor"));
}

//!
template <typename Pm, typename P>
void face_bool_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Fd = typename boost::graph_traits<Pm>::face_descriptor;
  using dp = CGAL::dynamic_face_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get",
        [](const map_type& p, const Fd& f) { return py::bool_(get(p, f)); },
        py::arg("property_map"), py::arg("face_descriptor"));
}

//!
template <typename Pm, typename P>
void vertex_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using dp = CGAL::dynamic_vertex_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Vd& v) { return get(p, v); },
        py::arg("property_map"), py::arg("vertex_descriptor"));
}

//!
template <typename Pm, typename P>
void vertex_bool_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using dp = CGAL::dynamic_vertex_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get",
        [](const map_type& p, const Vd& v) { return py::bool_(get(p, v)); },
        py::arg("property_map"), py::arg("vertex_descriptor"));
}

//!
template <typename Pm, typename P>
void halfedge_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Hd = typename boost::graph_traits<Pm>::halfedge_descriptor;
  using dp = CGAL::dynamic_halfedge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Hd& h) { return get(p, h); },
        py::arg("property_map"), py::arg("halfedge_descriptor"));
}

//!
template <typename Pm, typename P>
void halfedge_bool_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Hd = typename boost::graph_traits<Pm>::halfedge_descriptor;
  using dp = CGAL::dynamic_halfedge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get",
        [](const map_type& p, const Hd& h) { return py::bool_(get(p, h)); },
        py::arg("property_map"), py::arg("halfedge_descriptor"));
}

//! Read Polygon soup from a file
template <typename SurfaceMesh>
auto read_polygon_soup(const std::string& fname, const py::dict& np = py::dict()) {
  std::vector<Point_3> points;
  std::vector<std::vector<std::size_t> > polygons;

  if (! CGAL::IO::read_polygon_soup(fname, points, polygons))
    throw std::runtime_error("Cannot read file!");

  return std::make_tuple(points, polygons);
}

// Draw a surface mesh.
#ifdef CGALPY_HAS_VISUAL
template <typename SurfaceMesh>
void draw(const SurfaceMesh& sm, const char* title) { CGAL::draw(sm, title); }
#endif

template <typename SurfaceMesh>
typename SurfaceMesh::Point my_point(const SurfaceMesh& sm, const typename SurfaceMesh::Vertex_index& v)
{ return sm.point(v); }

//!
template <typename SurfaceMesh, typename Vi, typename Pnt>
auto points(const SurfaceMesh& sm) { return sm.points(); }

//!
template <typename SurfaceMesh>
bool is_triangle(const typename SurfaceMesh::Halfedge_index& h, const SurfaceMesh& sm)
{ return CGAL::is_triangle(h, sm); }

/// \name Iterators
/// @{

//
template <typename SurfaceMesh>
py::object vertices(const SurfaceMesh& sm)
{ return make_iterator(sm.vertices_begin(), sm.vertices_end()); }

//
template <typename SurfaceMesh>
py::object halfedges(const SurfaceMesh& sm)
{ return make_iterator(sm.halfedges_begin(), sm.halfedges_end()); }

//
template <typename SurfaceMesh>
py::object edges(const SurfaceMesh& sm)
{ return make_iterator(sm.edges_begin(), sm.edges_end()); }

//
template <typename SurfaceMesh>
py::object faces(const SurfaceMesh& sm)
{ return make_iterator(sm.faces_begin(), sm.faces_end()); }

/// @}

// property maps
template <typename SurfaceMesh, typename Key, typename Value>
auto add_map(SurfaceMesh& sm, const std::string& name = std::string(), const Value& default_value = Value()) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  using Pnt = typename Sm::Point;
  using Map = typename Sm::template Property_map<Vi, Pnt>;
  // Surface_mesh::Property_map<edge_descriptor, bool> is_constrained_map =
  //   mesh2.add_property_map<edge_descriptor, bool>("e:is_constrained", true).first;

  auto res = sm.template add_property_map<Key, Value>(name, default_value);
  return res;
}

template <typename T>
auto make_random_access_property_map(const std::vector<T>& vec)
{ return CGAL::make_random_access_property_map(vec); }

//!
template<typename C, typename Sm, typename V>
C export_property_maps(C& c, const std::string& type_name) {
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  internal::export_property_map<Sm, Vi, V>(c, ("Vertex_" + type_name + "_map").c_str());
  internal::export_property_map<Sm, Ei, V>(c, ("Edge_" + type_name + "_map").c_str());
  internal::export_property_map<Sm, Hi, V>(c, ("Halfedge_" + type_name + "_map").c_str());
  internal::export_property_map<Sm, Fi, V>(c, ("Face_" + type_name + "_map").c_str());
  return c;
}

//!
template <typename C, typename Mesh, typename Key, typename ValueType>
void add_generic_map(C& c, const std::string& map_name, const ValueType& default_value = ValueType()) {
  c.def(("add_" + map_name).c_str(), sm::add_map<Mesh, Key, ValueType>,
        py::arg("name") = std::string(), py::arg("default_value") = default_value,
        "adds a property map named `name` with value type `T` and default `t`\n"
        "for index type `I`. Returns the property map together with a Boolean\n"
        "that is `true` if a new map was created. In case it already exists\n"
        "the existing map together with `false` is returned.")
    .def(map_name.c_str(), &Mesh::template property_map<Key, ValueType>,
         py::arg("name") = std::string(),
         "returns an optional property map named `name` with key type `I` and value type `T`.")
    .def("remove_property_map", &Mesh::template remove_property_map<Key, ValueType>,
         py::arg("p"),
         "removes property map `name`. The memory allocated for that property map is freed.")
    ;
}

//!
template <typename C, typename SurfaceMesh, typename ValueType>
C add_generic_maps(C& c, const std::string& map_name) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  add_generic_map<C, Sm, Vi, ValueType>(c, ("property_map_vertex_" + map_name));
  add_generic_map<C, Sm, Ei, ValueType>(c, ("property_map_edge_" + map_name));
  add_generic_map<C, Sm, Hi, ValueType>(c, ("property_map_halfedge_" + map_name));
  add_generic_map<C, Sm, Fi, ValueType>(c, ("property_map_face_" + map_name));
  return c;
}

//!
template <typename SurfaceMesh, typename C>
C add_maps(C& c) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  using Pnt = typename Sm::Point;

  c
    // TODO: add a class for this:
    .def("property_map_vertex_set_int", [](Sm& sm, const std::string& name, const py::set& default_value = py::set()) {
          std::set<int> s;
          for (auto v : default_value) {
            try {
              s.insert(py::cast<int>(v));
            }
            catch (const py::cast_error&) {
              throw std::runtime_error("Cannot cast to int");
            }
          }
          return sm::add_map<Sm, Vi, std::set<int>>(sm, name, s);
        },
        py::arg("name") = std::string(), py::arg("default_value") = py::set())
    // this is here because it confused clang
    .def("properties_vertex", [](const Sm& sm) { return sm.template properties<Vi>(); },
        "returns a vector with all strings that describe properties with the key type `Vertex_index`.")
    .def("properties_edge", [](const Sm& sm) { return sm.template properties<Ei>(); },
        "returns a vector with all strings that describe properties with the key type `Edge_index`.")
    .def("properties_halfedge", [](const Sm& sm) { return sm.template properties<Hi>(); },
        "returns a vector with all strings that describe properties with the key type `Halfedge_index`.")
    .def("properties_face", [](const Sm& sm) { return sm.template properties<Fi>(); },
        "returns a vector with all strings that describe properties with the key type `Face_index`.")
    ;

//! \todo move to polygon_mesh_processing_bindings.cpp because it depends on Eigen
#ifdef CGALPY_POLYGON_MESH_PROCESSING_BINDINGS
  using Pcad = CGAL::Polygon_mesh_processing::Principal_curvatures_and_directions<Kernel>;
  c.def("add_property_map_vertex_Principal_curvatures_and_directions", &sm::add_map<Sm, Vi, Pcad>,
        py::arg("name"), py::arg("default_value"),
        "adds a property map named `name` with value type `Principal_curvatures_and_directions` and default `default_value`\n"
        "for index type `Vertex_index`. Returns the property map together with a Boolean\n"
        "that is `true` if a new map was created. In case it already exists\n"
        "the existing map together with `false` is returned.")
    .def("property_map_vertex_Principal_curvatures_and_directions", &Sm::template property_map<Vi, Pcad>,
         py::arg("name") = std::string(),
         "returns an optional property map named `name` with key type `Vertex_index` and value type `Principal_curvatures_and_directions`.")
    ;
#endif

  add_generic_maps<C, Sm, std::string>(c, "string");
  add_generic_maps<C, Sm, CGAL::IO::Color>(c, "color");
  add_generic_maps<C, Sm, typename Sm::Point>(c, "point");
  add_generic_maps<C, Sm, bool>(c, "bool");
  add_generic_maps<C, Sm, std::size_t>(c, "size_t");
  add_generic_maps<C, Sm, Vector_3>(c, "vector");
  add_generic_maps<C, Sm, int>(c, "int");
  add_generic_maps<C, Sm, FT>(c, "FT");
  add_generic_maps<C, Sm, py::tuple>(c, "tuple");
  add_generic_maps<C, Sm, py::set>(c, "set");
  add_generic_maps<C, Sm, py::list>(c, "list");
  // add_generic_maps<C, Sm, std::uint32_t>(c, "uint32_t"); //no

  #if __cplusplus >= 202002l
  if constexpr (!std::is_same<double, FT>::value) {
    add_generic_maps<C, Sm, double>(c, "float"); // shadows FT
  }
  #endif

  c.def("remove_all_property_maps", &Sm::remove_all_property_maps,
        "removes all property maps for all index types added by a call to `add_property_map()`.\n"
        "The memory allocated for those property maps is freed.");
  c.def("remove_property_maps_vertex", &Sm::template remove_property_maps<Vi>,
        "removes all property maps for the index type `Vertex_index` added by a call to `add_property_map()`.\n"
        "The memory allocated for those property maps is freed.");
  c.def("remove_property_maps_edge", &Sm::template remove_property_maps<Ei>,
        "removes all property maps for the index type `Edge_index` added by a call to `add_property_map()`.\n"
        "The memory allocated for those property maps is freed.");
  c.def("remove_property_maps_halfedge", &Sm::template remove_property_maps<Hi>,
        "removes all property maps for the index type `Halfedge_index` added by a call to `add_property_map()`.\n"
        "The memory allocated for those property maps is freed.");
  c.def("remove_property_maps_face", &Sm::template remove_property_maps<Fi>,
        "removes all property maps for the index type `Face_index` added by a call to `add_property_map()`.\n"
        "The memory allocated for those property maps is freed.");

  return c;
}

} // namespace sm

//
template <typename SurfaceMesh, typename C>
void add_sm_index(C& c) {
  using Sm = SurfaceMesh;
  using size_type = typename Sm::size_type;
  using Sm_i = typename C::Type;

  c.def(py::init<>())
    .def(py::init<size_type>())
    .def("id", &Sm_i::id)
    .def("idx", &Sm_i::idx)
    .def("is_valid", &Sm_i::is_valid)
    .def("reset", &Sm_i::reset)
    ;
}

// Export Surface_mesh.
template <typename SurfaceMesh>
void export_surface_mesh_impl(py::module_& m, const char* name) {
  using Sm = SurfaceMesh;
  using Pnt = typename Sm::Point;
  using size_type = typename Sm::size_type;
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  using Gt = boost::graph_traits<Sm>;
  using Fd = typename Gt::face_descriptor;
  using Hd = typename Gt::halfedge_descriptor;
  using Vd = typename Gt::vertex_descriptor;

  constexpr auto ri(py::rv_policy::reference_internal);

  // Vertex_index
  using Sm_vi = typename CGAL::SM_Index<Vi>;
  if (! add_attr<Sm_vi>(m, "SM_Index_vertex_index")) {
    py::class_<Sm_vi> sm_vi_c(m, "SM_vertex_index");
    add_sm_index<Sm>(sm_vi_c);
  }

  if (! add_attr<Vi>(m, "Vertex_index")) {
    py::class_<Vi, Sm_vi>(m, "Vertex_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Vi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Vi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Edge_index
  if (! add_attr<Ei>(m, "Edge_index")) {
    py::class_<Ei>(m, "Edge_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("halfedge", &Ei::halfedge)
      .def("idx", &Ei::idx)
      .def("reset", &Ei::reset)
      .def("is_valid", &Ei::is_valid)
      .def("__str__", [](const Ei& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Ei& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Halfedge_index
  using Sm_hi = typename CGAL::SM_Index<Hi>;
  if (! add_attr<Sm_hi>(m, "SM_halfedge_index")) {
    py::class_<Sm_hi> sm_hi_c(m, "SM_halfedge_index");
    add_sm_index<Sm>(sm_hi_c);
  }

  if (! add_attr<Hi>(m, "Halfedge_index")) {
    py::class_<Hi, Sm_hi>(m, "Halfedge_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Hi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Hi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Face_index
  using Sm_fi = typename CGAL::SM_Index<Fi>;
  if (! add_attr<Sm_fi>(m, "SM_face_index")) {
    py::class_<Sm_fi> sm_fi_c(m, "SM_face_index");
    add_sm_index<Sm>(sm_fi_c);
  }

  if (! add_attr<Fi>(m, "Face_index")) {
    py::class_<Fi, Sm_fi>(m, "Face_index")
      .def(py::init<>())
      .def(py::init<size_type>())
      .def("__str__", [](const Fi& smi){ return std::to_string(smi.idx()); })
      .def("__repr__", [](const Fi& smi){ return std::to_string(smi.idx()); })
      ;
  }

  // Face descriptor
  if (! add_attr<Fd>(m, "Face_descriptor")) {
    py::class_<Fd>(m, "Face_descriptor")
      .def(py::init<>())
      .def(py::init<Fi>())
      .def("idx", &Fd::idx)
      .def("is_valid", &Fd::is_valid)
      .def("__str__", [](const Fd& fd){ return std::to_string(fd.idx()); })
      .def("__repr__", [](const Fd& fd){ return std::to_string(fd.idx()); })
      ;
  }

  // Halfedge descriptor
  if (! add_attr<Hd>(m, "Halfedge_descriptor")) {
    py::class_<Hd>(m, "Halfedge_descriptor")
      .def(py::init<>())
      .def(py::init<Hi>())
      .def("idx", &Hd::idx)
      .def("is_valid", &Hd::is_valid)
      .def("__str__", [](const Hd& hd){ return std::to_string(hd.idx()); })
      .def("__repr__", [](const Hd& hd){ return std::to_string(hd.idx()); })
      ;
  }

  // Vertex_descriptor
  if (! add_attr<Vd>(m, "Vertex_descriptor")) {
    py::class_<Vd>(m, "Vertex_descriptor")
      .def(py::init<>())
      .def(py::init<Vi>())
      .def("idx", &Vd::idx)
      .def("is_valid", &Vd::is_valid)
      .def("__str__", [](const Vd& vd){ return std::to_string(vd.idx()); })
      .def("__repr__", [](const Vd& vd){ return std::to_string(vd.idx()); })
      ;
  }

  // Surface mesh
  if (! add_attr<Sm>(m, name)) {

    py::class_<Sm> sm_c(m, name);
    sm::add_maps<Sm, py::class_<Sm>>(sm_c);
    sm_c.def(py::init<>())
      .def(py::init<const Sm&>())
      // .def("assign", &Sm::assign, ri)
      .def("add_vertex", py::overload_cast<>(&Sm::add_vertex))
      .def("add_vertex", py::overload_cast<const Pnt&>(&Sm::add_vertex))
      .def("add_edge", py::overload_cast<>(&Sm::add_edge))
      .def("add_edge", py::overload_cast<Vi, Vi>(&Sm::add_edge))
      .def("add_face", static_cast<Fi(Sm::*)()>(&Sm::add_face))
      .def("add_face", static_cast<Fi(Sm::*)(Vi, Vi, Vi)>(&Sm::add_face))
      .def("add_face", static_cast<Fi(Sm::*)(Vi, Vi, Vi, Vi)>(&Sm::add_face))
      .def("add_face", &sm::add_face<Sm>)

      .def("has_valid_index", &sm::has_valid_index_v<Sm>)
      .def("has_valid_index", &sm::has_valid_index_e<Sm>)
      .def("has_valid_index", &sm::has_valid_index_h<Sm>)
      .def("has_valid_index", &sm::has_valid_index_f<Sm>)

      .def("remove_vertex", &Sm::remove_vertex)
      .def("remove_edge", &Sm::remove_edge)
      .def("remove_face", &Sm::remove_face)
      .def("num_vertices", &Sm::num_vertices)
      .def("number_of_vertices", &Sm::number_of_vertices)
      .def("num_halfedges", &Sm::num_halfedges)
      .def("number_of_halfedges", &Sm::number_of_halfedges)
      .def("num_edges", &Sm::num_edges)
      .def("number_of_edges", &Sm::number_of_edges)
      .def("num_faces", &Sm::num_faces)
      .def("number_of_faces", &Sm::number_of_faces)
      .def("is_empty", &Sm::is_empty)
      .def("clear_without_removing_property_maps", &Sm::clear_without_removing_property_maps)
      .def("clear", &Sm::clear)
      .def("halfedge", [](const Sm& sm, Vi v) { return sm.halfedge(v); },
           py::arg("v"))
      .def("halfedge", [](const Sm& sm, Fi f) { return sm.halfedge(f); },
           py::arg("f"))
      .def("halfedge", [](const Sm& sm, Ei e) { return sm.halfedge(e); },
           py::arg("e"))
      .def("halfedge",
           [](const Sm& sm, Ei e, unsigned int i) { return sm.halfedge(e, i); },
           py::arg("e"), py::arg("i"))
      .def("halfedge",
           [](const Sm& sm, Vi source, Vi target)
           { return sm.halfedge(source, target); },
           py::arg("source"), py::arg("target"))
      .def("degree", [](const Sm& sm, Vi v) { return sm.degree(v); },
           py::arg("v"))
      .def("degree", [](const Sm& sm, Fi f) { return sm.degree(f); },
           py::arg("f"))
      .def("is_border", [](const Sm& sm, Hi h) { return sm.is_border(h); },
           py::arg("h"))
      .def("is_border", [](const Sm& sm, Ei e) { return sm.is_border(e); },
           py::arg("e"))
      .def("is_border",
           [](const Sm& sm, Vi v, bool check_all_incident_halfedges = true)
           { return sm.is_border(v, check_all_incident_halfedges); },
           py::arg("v"), py::arg("check_all_incident_halfedges") = true)
      .def("is_removed", [](const Sm& sm, Vi v) { return sm.is_removed(v); },
           py::arg("v"))
      .def("is_removed", [](const Sm& sm, Ei e) { return sm.is_removed(e); },
           py::arg("e"))
      .def("is_removed", [](const Sm& sm, Fi f) { return sm.is_removed(f); },
           py::arg("f"))
      .def("is_removed", [](const Sm& sm, Hi h) { return sm.is_removed(h); },
           py::arg("h"))
      .def("source", &Sm::source)
      .def("target", &Sm::target)
      // void reserve(size_type nvertices, size_type nedges, size_type nfaces )
      // void resize(size_type nvertices, size_type nedges, size_type nfaces )
      // join(const Surface_mesh& other)
      .def("edge", &Sm::edge)
      .def("face", &Sm::face)
      .def("join", &Sm::join)
      .def("next", &Sm::next)
      .def("prev", &Sm::prev)
      .def("resize", &Sm::resize)
      .def("set_target", &Sm::set_target)
      .def("has_garbage", &Sm::has_garbage)
      .def("collect_garbage", [](Sm& sm) { sm.collect_garbage(); })
      .def("is_isolated", &Sm::is_isolated)
      // .def("halfedges_end", &Sm::halfedges_end) // not needed
      .def("set_next_only", &Sm::set_next_only)
      .def("set_prev_only", &Sm::set_prev_only)
      .def("shrink_to_fit", &Sm::shrink_to_fit)
      .def("set_recycle_garbage", &Sm::set_recycle_garbage)
      .def("does_recycle_garbage", &Sm::does_recycle_garbage)

      .def("number_of_removed_edges", &Sm::number_of_removed_edges)
      .def("number_of_removed_faces", &Sm::number_of_removed_faces)
      .def("number_of_removed_vertices", &Sm::number_of_removed_vertices)
      .def("number_of_removed_halfedges", &Sm::number_of_removed_halfedges)

      .def("next_around_source", &Sm::next_around_source)
      .def("prev_around_source", &Sm::prev_around_source)
      .def("next_around_target", &Sm::next_around_target)
      .def("prev_around_target", &Sm::prev_around_target)

      // .def("property_stats", &Sm::property_stats)

      .def("point", &sm::my_point<Sm>, ri)
      .def("points", &sm::points<Sm, Vi, Pnt>)

      // .def("__iadd__",
      //      py::self += py::self,
      //      "Inserts other into sm.")
      .def("__iadd__",
           [](Sm& sm, const Sm& other) { sm.join(other); return sm; },
           "Inserts other into sm.",
           py::is_operator())

      .def("is_valid", py::overload_cast<bool>(&Sm::is_valid, py::const_))
#if CGAL_VERSION_NR >= 1050600000
      .def("is_valid", py::overload_cast<Vi, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Ei, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Hi, bool>(&Sm::is_valid, py::const_))
      .def("is_valid", py::overload_cast<Fi, bool>(&Sm::is_valid, py::const_))
#endif
      ;

    add_attr<Vi>(sm_c, "Vertex_index");
    add_attr<Ei>(sm_c, "Edge_index");
    add_attr<Hi>(sm_c, "Halfedge_index");
    add_attr<Fi>(sm_c, "Face_index");
    // add_attr<Halfedge>(sm_c, "Halfedge");

    using Vci = typename Sm::Vertex_iterator;
    using Hci = typename Sm::Halfedge_iterator;
    using Eci = typename Sm::Edge_iterator;
    using Fci = typename Sm::Face_iterator;

    add_iterator<Vci, Vci>("Vertex_iterator", sm_c);
    add_iterator<Hci, Hci>("Halfedge_iterator", sm_c);
    add_iterator<Eci, Eci>("Edge_iterator", sm_c);
    add_iterator<Fci, Fci>("Face_iterator", sm_c);

    sm_c.def("vertices", &sm::vertices<Sm>, py::keep_alive<0, 1>())
      .def("halfedges", &sm::halfedges<Sm>, py::keep_alive<0, 1>())
      .def("edges", &sm::edges<Sm>, py::keep_alive<0, 1>())
      .def("faces", &sm::faces<Sm>, py::keep_alive<0, 1>())
      ;

    add_insertion(sm_c, "__str__");
    add_insertion(sm_c, "__repr__");
  }

#ifdef CGALPY_HAS_VISUAL
  m.def("draw", &sm::draw<Sm>);
#endif
}

// Export Surface_mesh<Pnt>
void export_surface_mesh(py::module_& m) {
  // 2D Surface mesh
  // using Sm_2 = sm::Surface_mesh_2;
  // export_surface_mesh_impl<Sm_2>(m, "Surface_mesh_2");

  // 3D Surface mesh
  using Kernel_ = Kernel;
  using Sm_3 = sm::Surface_mesh_3;
  using Vi = typename Sm_3::Vertex_index;
  using Fi = typename Sm_3::Face_index;
  using Hi = typename Sm_3::Halfedge_index;
  using Ei = typename Sm_3::Edge_index;
  using Pnt = Kernel_::Point_3;
  using Vector_3 = Kernel_::Vector_3;
  constexpr auto ri(py::rv_policy::reference_internal);

  export_surface_mesh_impl<Sm_3>(m, "Surface_mesh_3");

  // sm::vertex_map<Sm_3, Pnt>(m, "vertex_point_boost_map", "Vertex_point_boost_map"); //this is the boost::property_map

  using vbmap = typename Sm_3::template Property_map<Vi, bool>;
  internal::export_property_map_bool<Sm_3, Vi>(m, "Vertex_bool_map");
  using fbmap = typename Sm_3::template Property_map<Fi, bool>;
  internal::export_property_map_bool<Sm_3, Fi>(m, "Face_bool_map");
  using hbmap = typename Sm_3::template Property_map<Hi, bool>;
  internal::export_property_map_bool<Sm_3, Hi>(m, "Halfedge_bool_map");
  using ebmap_type = typename Sm_3::template Property_map<Ei, bool>;
  internal::export_property_map_bool<Sm_3, Ei>(m, "Edge_bool_map");

//! \todo move to polygon_mesh_processing_bindings.cpp because it depends on Eigen
#ifdef CGALPY_POLYGON_MESH_PROCESSING_BINDINGS
  using Pcad = CGAL::Polygon_mesh_processing::Principal_curvatures_and_directions<Kernel>;
  // internal::export_property_map<Sm_3, Vi, Pcad>(m, "Vertex_Principal_curvatures_and_directions_map");
#endif

  // Export all property maps ownedhandled by Surface_mesh
  sm::export_property_maps<py::module_, Sm_3, int>(m, "int");
  sm::export_property_maps<py::module_, Sm_3, FT>(m, "FT");
  sm::export_property_maps<py::module_, Sm_3, std::string>(m, "string");
  sm::export_property_maps<py::module_, Sm_3, std::size_t>(m, "size_t");
  sm::export_property_maps<py::module_, Sm_3, CGAL::IO::Color>(m, "Color");
  sm::export_property_maps<py::module_, Sm_3, std::uint32_t>(m, "uint32_t"); // why is this needed?
  sm::export_property_maps<py::module_, Sm_3, py::tuple>(m, "tuple");
  sm::export_property_maps<py::module_, Sm_3, py::list>(m, "list");
  sm::export_property_maps<py::module_, Sm_3, py::set>(m, "set");
  sm::export_property_maps<py::module_, Sm_3, Vector_3>(m, "vector_3");
  sm::export_property_maps<py::module_, Sm_3, Plane_3>(m, "Plane_3");
  sm::export_property_maps<py::module_, Sm_3, Point_3>(m, "Point_3");

  if constexpr (! std::is_same<double, FT>::value) {
    sm::export_property_maps<py::module_, Sm_3, double>(m, "float"); // shadows FT
  }

  // implemented:
  // vertex_point_map
  // edge_is_constrained_map
  // face_index_map
  // point_map
  // vertex_is_constrained_map
  // face_patch_map
  // vertex_corner_map
  // VPMap
  // GeomTraits::Plane_3

  // ???
  // region_primitive_map:
  // a property map filled by this function and that will contain for each region the plane (or only its orthognonal
  // vector) estimated that approximates it.
  // Type: a class model of WritablePropertyMap with the value type of RegionMap as key and GeomTraits::Plane_3 or
  // GeomTraits::Vector_3 as value type, GeomTraits being the type of the parameter geom_traits
  // Default: None

  py::class_<boost::vector_property_map<Vector_3>>(m, "Vector_vector_3_map")
    .def(py::init<>())
    ;

  // void set_selected_faces(const FacePatchIDRange& selected_face_patch_ids,
  //                         FacePatchIDMap face_patch_id_map

  using Ffg3 = CGAL::Face_filtered_graph<Sm_3>;

  py::class_<Ffg3>(m, "Face_filtered_graph")
    .def(py::init<const Sm_3&, std::size_t, const Sm_3::Property_map<Fi, std::size_t>&>())
    .def("graph", [](const Ffg3& ffg) { return ffg.graph(); })
    .def("reset_indices", [](Ffg3& ffg) { return ffg.reset_indices(); })
    .def("number_of_faces",
         [](const Ffg3& ffg) { return ffg.number_of_faces(); })
    .def("invert_selection", [](Ffg3& ffg) { return ffg.invert_selection(); })
    // commented for stubs
    // .def("get_face_index_map",
    //    [](const Ffg3& ffg) { return ffg.get_face_index_map(); })
    .def("number_of_vertices",
         [](const Ffg3& ffg) { return ffg.number_of_vertices(); })
    .def("number_of_halfedges",
         [](const Ffg3& ffg) { return ffg.number_of_halfedges(); })
    // .def("get_vertex_index_map",
    //   [](const Ffg3& ffg) { return ffg.get_vertex_index_map(); })
    // .def("get_halfedge_index_map",
    //   [](const Ffg3& ffg) { return ffg.get_halfedge_index_map(); })
    .def("initialize_face_indices",
         [](Ffg3& ffg) { return ffg.initialize_face_indices(); })
    .def("initialize_vertex_indices",
         [](Ffg3& ffg) { return ffg.initialize_vertex_indices(); })
    .def("initialize_halfedge_indices",
         [](Ffg3& ffg) { return ffg.initialize_halfedge_indices(); })
    .def("set_selected_faces",
         [](Ffg3& ffg, const std::vector<std::size_t>& vec,
            const Sm_3::Property_map<Fi, std::size_t>& fccmap) {
        return ffg.set_selected_faces(vec, fccmap);
      })
    ;

  // Face filtered Graph
  m.def("faces", &boost_utils::my_faces<Ffg3>);

  // Still Need Sorting
  m.def("clear", &CGAL::clear<Sm_3>);
  m.def("is_closed", &CGAL::is_closed<Sm_3>);

  //! \todo export CGAL::vertex_point and CGAL::get() instead.
  m.def("get_vertex_point", [](const Sm_3& pm, const Vi& vd)
  { return get(CGAL::vertex_point, pm, vd); });

  // m.def("expand_face_selection",
  //       &sm::expand_face_selection<Fi, Sm_3, Sm_3::Property_map<Fi, int>>);
  m.def("read_polygon_soup", &sm::read_polygon_soup<Sm_3>,
        py::arg("fname"), py::arg("np") = py::dict());
  m.def("is_triangle", &sm::is_triangle<Sm_3>);
  m.def("is_triangle_mesh", &CGAL::is_triangle_mesh<Sm_3>);

  // CGAL and the Boost Graph Library

  // Global functions
  m.def("add_edge", &bgl::my_add_edge<Sm_3>);
  m.def("add_face", &bgl::my_add_face<Sm_3>);
  m.def("add_vertex", &bgl::my_add_vertex<Sm_3>);
  // Not documented
  // m.def("collect_garbage", &bgl::my_collect_garbage<Sm_3>);
  m.def("num_edges", &bgl::my_num_edges<Sm_3>);
  m.def("num_faces", &bgl::my_num_faces<Sm_3>);
  m.def("num_halfedges", &bgl::my_num_halfedges<Sm_3>);
  m.def("num_vertices", &bgl::my_num_vertices<Sm_3>);
  m.def("remove_all_elements", &bgl::my_remove_all_elements<Sm_3>);
  m.def("reserve", &bgl::my_reserve<Sm_3>);

  // Other
  m.def("adjacent_vertices", &bgl::adjacent_vertices<Sm_3>);
  m.def("add_vertex", &bgl::add_vertex_p<Sm_3>);
  m.def("degree", &bgl::degree_v<Sm_3>);
  m.def("degree", &bgl::degree_f<Sm_3>);
  m.def("edge", &bgl::edge<Sm_3>);
  m.def("edge", &bgl::edge_vv<Sm_3>);
  m.def("face", &bgl::face<Sm_3>);
  m.def("halfedge", &bgl::halfedge_e<Sm_3>);
  m.def("halfedge", &bgl::halfedge_f<Sm_3>);
  m.def("halfedge", &bgl::halfedge_v<Sm_3>);
  m.def("halfedge", &bgl::halfedge_vv<Sm_3>);
  m.def("in_degree", &bgl::in_degree<Sm_3>);
  m.def("is_border", &bgl::my_is_border_h<Sm_3>);
  m.def("is_border", &bgl::my_is_border_e<Sm_3>);
  m.def("is_border", &bgl::my_is_border_v<Sm_3>);
  m.def("is_border", &bgl::my_is_border_edge<Sm_3>);
  m.def("is_valid_vertex_descriptor",
        &bgl::my_is_valid_vertex_descriptor<Sm_3>,
        py::arg("v"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_halfedge_descriptor",
        &bgl::my_is_valid_halfedge_descriptor<Sm_3>,
        py::arg("h"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_edge_descriptor",
        &bgl::my_is_valid_edge_descriptor<Sm_3>,
        py::arg("e"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_face_descriptor",
        &bgl::my_is_valid_face_descriptor<Sm_3>,
        py::arg("f"), py::arg("g"), py::arg("verbose") = false);
  m.def("next", &bgl::next<Sm_3>);
  m.def("null_face", &bgl::null_face<Sm_3>);
  m.def("null_halfedge", &bgl::null_halfedge<Sm_3>);
  m.def("null_vertex", &bgl::null_vertex<Sm_3>);
  m.def("opposite", &bgl::opposite<Sm_3>);
  m.def("out_degree", &bgl::out_degree<Sm_3>);
  m.def("prev", &bgl::prev<Sm_3>);
  m.def("source", &bgl::source_e<Sm_3>);
  m.def("source", &bgl::source_h<Sm_3>);
  m.def("target", &bgl::target_e<Sm_3>);
  m.def("target", &bgl::target_h<Sm_3>);
  m.def("remove_edge", &bgl::remove_edge_e<Sm_3>);
  // Fails to compile, but also not documented
  // m.def("remove_edge", &bgl::remove_edge_vv<Sm_3>);
  m.def("remove_face", &bgl::remove_face<Sm_3>);
  m.def("remove_vertex", &bgl::remove_vertex<Sm_3>);
  m.def("set_face", &bgl::set_face<Sm_3>);
  m.def("set_halfedge", &bgl::set_halfedge_vh<Sm_3>);
  m.def("set_halfedge", &bgl::set_halfedge_fh<Sm_3>);
  m.def("set_next", &bgl::set_next<Sm_3>);
  m.def("set_target", &bgl::set_target<Sm_3>);

  m.def("vertices", &boost_utils::my_vertices<Sm_3>);
  m.def("edges", &boost_utils::my_edges<Sm_3>);
  m.def("in_edges", &boost_utils::my_in_edges<Sm_3>);
  m.def("out_edges", &boost_utils::my_out_edges<Sm_3>);
  m.def("halfedges", &boost_utils::my_halfedges<Sm_3>);
  m.def("faces", &boost_utils::my_faces<Sm_3>);

  // Generator Functions
  m.def("make_tetrahedron", &bgl::my_make_tetrahedron<Sm_3>);

  // Euler operations
  boost_utils::define_euler_operations<py::module_, Sm_3, ebmap_type>(m);

  // Iterators and Circulators
  boost_utils::define_boost_iterators<py::module_, Sm_3>(m);

  // Selection Functions
  using ebmap_type = typename Sm_3::template Property_map<Ei, bool>;
  using fbmap_type = typename Sm_3::template Property_map<Fi, bool>;
  using vbmap_type = typename Sm_3::template Property_map<Vi, bool>;
  // boost_utils::define_boost_selection_functions<py::module_, Sm_3, ebmap_type, fbmap_type, vbmap_type>(m);

  // Helper Functions
  boost_utils::define_boost_helpers<py::module_, Sm_3, Sm_3>(m);

  // Generator Functions
  boost_utils::define_generate_functions<py::module_, Sm_3, Kernel>(m);

  // Partitioning Operations
  using EdgeDoubleMap = typename Sm_3::template Property_map<Ei, double>;
  using VertexVectorDoubleMap =
    typename Sm_3::template Property_map<Vi, std::vector<double>>;
  using VertexSizeTMap = typename Sm_3::template Property_map<Vi, std::size_t>;
  boost_utils::define_boost_partitioning_operations<py::module_, Sm_3, EdgeDoubleMap, VertexVectorDoubleMap, VertexSizeTMap>(m);

}
