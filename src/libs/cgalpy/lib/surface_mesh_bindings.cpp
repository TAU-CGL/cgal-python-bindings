// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com>
//            Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

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

#include "cgalpy/Bgl_docstrings.hpp"
#include "cgalpy/Sm_docstrings.hpp"

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

#include "cgalpy/add_insertion.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/bgl.hpp"
#include "cgalpy/bgl_global.hpp"
#include "cgalpy/export_bgl_iterators.hpp"
#include "cgalpy/export_boost_mesh_utils.hpp"
#include "cgalpy/export_mesh_selection_functions.hpp"
#include "cgalpy/export_mesh_helpers.hpp"
#include "cgalpy/export_mesh_partitioning_operations.hpp"
#include "cgalpy/export_mesh_partitioning_operations.hpp"
#include "cgalpy/export_property_map.hpp"
#include "cgalpy/generator_functions.hpp"
#include "cgalpy/make_iterator.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/surface_mesh_types.hpp"

namespace py = nanobind;
namespace bgl_doc = cgalpy::bgl::docstrings;
namespace sm_doc = cgalpy::sm::docstrings;

namespace cgalpy {
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
 template <typename MapType>
void export_dynamic_property_map(py::module_& m, const std::string& name) {
  using Mt = MapType;
  using Key = const typename Mt::key_type;
  using Value = typename Mt::value_type;

  if constexpr (std::is_same_v<Value, bool>) {
    // std::vector<bool>-backed maps return proxy references, which nanobind
    // cannot expose directly. Return/store plain bool values instead, and do
    // not expose the underlying map_ member for this specialization.
    if (! add_attr<Mt>(m, name.c_str())) {
      py::class_<Mt>(m, name.c_str(),
                     "Dynamic Surface_mesh property map.")
        .def(py::init<>(),
             "Constructs an empty dynamic Surface_mesh property map.")
        ;
    }
    m.def("get", [](const Mt& pm, const Key& key) -> bool {
        return static_cast<bool>(get(pm, key));
      },
      py::arg("property_map"), py::arg("key"),
      "Returns a boolean value from a dynamic Surface_mesh property map.");
    m.def("put", [](const Mt& pm, const Key& key, bool value) {
        return put(pm, key, value);
      },
      py::arg("property_map"), py::arg("key"), py::arg("value"),
      "Sets a boolean value in a dynamic Surface_mesh property map.");
  }
  else {
    if (! add_attr<Mt>(m, name.c_str())) {
      py::class_<Mt>(m, name.c_str(),
                     "Dynamic Surface_mesh property map.")
        .def(py::init<>(),
             "Constructs an empty dynamic Surface_mesh property map.")
        .def_ro("map_", &Mt::map_,
                "Underlying dynamic property map storage.")
        ;
    }
    m.def("get", [](const Mt& pm, const Key& key) { return get(pm, key); },
          py::arg("property_map"), py::arg("key"),
          "Returns a value from a dynamic Surface_mesh property map.");
    m.def("put", [](const Mt& pm, const Key& key, const Value& value) { return put(pm, key, value); },
          py::arg("property_map"), py::arg("key"), py::arg("value"),
          "Sets a value in a dynamic Surface_mesh property map.");
  }
}

//!
template <typename SurfaceMesh, typename T, py::rv_policy Policy = py::rv_policy::automatic>
void export_dynamic_vertex_map(py::module_& m, const std::string& name) {
  using Sm = SurfaceMesh;
  using Vd = typename boost::graph_traits<Sm>::vertex_descriptor;
  using Dvpt = CGAL::dynamic_vertex_property_t<T>;
  using Mt = typename boost::property_map<Sm, Dvpt>::type;
  constexpr auto ri(py::rv_policy::reference_internal);
  export_dynamic_property_map<Mt>(m, "Dynamic_" + name);
  m.def("get", [](Dvpt tag, Sm& g) { return CGAL::get(tag, g); }, ri,
        py::arg("tag"), py::arg("graph"),
        "Returns a dynamic vertex property map from a Surface_mesh.");

  // Observe that Dvpt is (an instance) exported by the Bgl module.
  // The get(t, g) function above accepts a tag as the first parameter.
  // A Python user must create bindings for the Bgl in order to obtain the wrapped tag.
  // As a shortcut, we also provide the alias below, which obliviates the Bgl bindings at least for this purpose.
  // Also, transfer the first character into lower case
  m.def(("get_dynamic_" + name).c_str(), [](Sm& g) { return CGAL::get(Dvpt(), g); }, ri,
        py::arg("graph"),
        "Returns a dynamic vertex property map from a Surface_mesh.");
}

//!
template <typename SurfaceMesh, typename T, py::rv_policy Policy = py::rv_policy::automatic>
void export_dynamic_halfedge_map(py::module_& m, const std::string& name) {
  using Sm = SurfaceMesh;
  using Hd = typename boost::graph_traits<Sm>::halfedge_descriptor;
  using Dhpt = CGAL::dynamic_halfedge_property_t<T>;
  using Mt = typename boost::property_map<Sm, Dhpt>::type;
  constexpr auto ri(py::rv_policy::reference_internal);
  export_dynamic_property_map<Mt>(m, "Dynamic_" + name);
  m.def("get", [](Dhpt tag, Sm& g) { return CGAL::get(tag, g); }, ri,
        py::arg("property_map"), py::arg("graph"),
        "Returns a dynamic halfedge property map from a Surface_mesh.");

  // Observe that Dvpt is (an instance) exported by the Bgl module.
  // The get(t, g) function above accepts a tag as the first parameter.
  // A Python user must create bindings for the Bgl in order to obtain the wrapped tag.
  // As a shortcut, we also provide the alias below, which obliviates the Bgl bindings at least for this purpose.
  // Also, transfer the first character into lower case
  m.def(("get_dynamic_" + name).c_str(), [](Sm& g) { return CGAL::get(Dhpt(), g); }, ri,
        py::arg("graph"),
        "Returns a dynamic halfedge property map from a Surface_mesh.");
}

//!
template <typename SurfaceMesh, typename T, py::rv_policy Policy = py::rv_policy::automatic>
void export_dynamic_face_map(py::module_& m, const std::string& name) {
  using Sm = SurfaceMesh;
  using Fd = typename boost::graph_traits<Sm>::face_descriptor;
  using Dfpt = CGAL::dynamic_face_property_t<T>;
  using Mt = typename boost::property_map<Sm, Dfpt>::type;
  constexpr auto ri(py::rv_policy::reference_internal);
  export_dynamic_property_map<Mt>(m, "Dynamic_" + name);
  m.def("get", [](Dfpt tag, Sm& g) { return CGAL::get(tag, g); }, ri,
        py::arg("property_map"), py::arg("graph"),
        "Returns a dynamic face property map from a Surface_mesh.");

  // Observe that Dvpt is (an instance) exported by the Bgl module.
  // The get(t, g) function above accepts a tag as the first parameter.
  // A Python user must create bindings for the Bgl in order to obtain the wrapped tag.
  // As a shortcut, we also provide the alias below, which obliviates the Bgl bindings at least for this purpose.
  // Also, transfer the first character into lower case
  m.def(("get_dynamic_" + name).c_str(), [](Sm& g) { return CGAL::get(Dfpt(), g); }, ri,
        py::arg("graph"),
        "Returns a dynamic face property map from a Surface_mesh.");
}

//!
template <typename SurfaceMesh, typename T, py::rv_policy Policy = py::rv_policy::automatic>
void export_dynamic_edge_map(py::module_& m, const std::string& name) {
  using Sm = SurfaceMesh;
  using Ed = typename boost::graph_traits<Sm>::edge_descriptor;
  using Dept = CGAL::dynamic_edge_property_t<T>;
  using Mt = typename boost::property_map<Sm, Dept>::type;
  constexpr auto ri(py::rv_policy::reference_internal);
  export_dynamic_property_map<Mt>(m, "Dynamic_" + name);
  m.def("get", [](Dept tag, Sm& g) { return CGAL::get(tag, g); }, ri,
        py::arg("property_map"), py::arg("graph"),
        "Returns a dynamic edge property map from a Surface_mesh.");

  // Observe that Dvpt is (an instance) exported by the Bgl module.
  // The get(t, g) function above accepts a tag as the first parameter.
  // A Python user must create bindings for the Bgl in order to obtain the wrapped tag.
  // As a shortcut, we also provide the alias below, which obliviates the Bgl bindings at least for this purpose.
  // Also, transfer the first character into lower case
  m.def(("get_dynamic_" + name).c_str(), [](Sm& g) { return CGAL::get(Dept(), g); }, ri,
        py::arg("graph"),
        "Returns a dynamic edge property map from a Surface_mesh.");
}

/*! Export dynamic property maps.
 *
 */
template <typename SurfaceMesh, typename V, py::rv_policy Policy = py::rv_policy::automatic>
void export_dynamic_property_maps(py::module_& m, const std::string& name) {
  using Sm = SurfaceMesh;
  export_dynamic_vertex_map<Sm, V, Policy>(m, ("vertex_" + name + "_map").c_str());
  export_dynamic_halfedge_map<Sm, V, Policy>(m, ("halfedge_" + name + "_map").c_str());
  export_dynamic_face_map<Sm, V, Policy>(m, ("face_" + name + "_map").c_str());
  export_dynamic_edge_map<Sm, V, Policy>(m, ("edge_" + name + "_map").c_str());
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
  // Surface_mesh::Property_map<edge_descriptor, bool> is_constrained_map =
  //   mesh2.add_property_map<edge_descriptor, bool>("e:is_constrained", true).first;
  auto res = sm.template add_property_map<Key, Value>(name, default_value);
  return res;
}

template <typename T>
auto make_random_access_property_map(const std::vector<T>& vec)
{ return CGAL::make_random_access_property_map(vec); }

//!
template<typename C, typename Sm, typename T>
void export_property_maps(C& c, const std::string& type_name) {
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  internal::export_property_map<Sm, Vi, T>(c, ("Vertex_" + type_name + "_map").c_str());
  internal::export_property_map<Sm, Ei, T>(c, ("Edge_" + type_name + "_map").c_str());
  internal::export_property_map<Sm, Hi, T>(c, ("Halfedge_" + type_name + "_map").c_str());
  internal::export_property_map<Sm, Fi, T>(c, ("Face_" + type_name + "_map").c_str());
}

//!
template <typename C, typename SurfaceMesh, typename Key, typename Value>
void add_generic_map(C& c, const std::string& map_name, const Value& default_value = Value()) {
  using Sm = SurfaceMesh;

  c.def(("add_" + map_name).c_str(), sm::add_map<Sm, Key, Value>,
        py::arg("name") = std::string(), py::arg("default_value") = default_value,
        sm_doc::Surface_mesh_add_property_map)
    .def(map_name.c_str(), &Sm::template property_map<Key, Value>,
         py::arg("name") = std::string(),
         sm_doc::Surface_mesh_property_map)
    .def("remove_property_map", &Sm::template remove_property_map<Key, Value>,
         py::arg("p"),
         sm_doc::Surface_mesh_remove_property_map)
    ;
}

//!
template <typename C, typename SurfaceMesh, typename ValueType>
void add_generic_maps(C& c, const std::string& map_name) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  add_generic_map<C, Sm, Vi, ValueType>(c, ("property_map_vertex_" + map_name));
  add_generic_map<C, Sm, Ei, ValueType>(c, ("property_map_edge_" + map_name));
  add_generic_map<C, Sm, Hi, ValueType>(c, ("property_map_halfedge_" + map_name));
  add_generic_map<C, Sm, Fi, ValueType>(c, ("property_map_face_" + map_name));
}

//!
template <typename SurfaceMesh, typename C>
void add_maps(C& c) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  using Pnt = typename Sm::Point;

  c.def("properties_vertex", [](const Sm& sm) { return sm.template properties<Vi>(); },
        sm_doc::Surface_mesh_properties)
    .def("properties_edge", [](const Sm& sm) { return sm.template properties<Ei>(); },
        sm_doc::Surface_mesh_properties)
    .def("properties_halfedge", [](const Sm& sm) { return sm.template properties<Hi>(); },
        sm_doc::Surface_mesh_properties)
    .def("properties_face", [](const Sm& sm) { return sm.template properties<Fi>(); },
        sm_doc::Surface_mesh_properties)

    // TODO: add a class for this:
    .def("property_map_vertex_set_int",
         [](Sm& sm, const std::string& name, const py::set& default_value = py::set()) {
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
         py::arg("name") = std::string(), py::arg("default_value") = py::set(),
         sm_doc::Surface_mesh_add_property_map)
    ;

//! \todo move to polygon_mesh_processing_bindings.cpp because it depends on Eigen
#ifdef CGALPY_POLYGON_MESH_PROCESSING_BINDINGS
  using Pcad = CGAL::Polygon_mesh_processing::Principal_curvatures_and_directions<Kernel>;
  c.def("add_property_map_vertex_Principal_curvatures_and_directions", &sm::add_map<Sm, Vi, Pcad>,
        py::arg("name"), py::arg("default_value"),
        sm_doc::Surface_mesh_add_property_map)
    .def("property_map_vertex_Principal_curvatures_and_directions", &Sm::template property_map<Vi, Pcad>,
         py::arg("name") = std::string(),
         sm_doc::Surface_mesh_property_map)
    ;
#endif

  add_generic_maps<C, Sm, bool>(c, "bool");
  add_generic_maps<C, Sm, int>(c, "int");
  add_generic_maps<C, Sm, std::size_t>(c, "size_t");
  add_generic_maps<C, Sm, double>(c, "float");
  add_generic_maps<C, Sm, std::string>(c, "string");
  add_generic_maps<C, Sm, CGAL::IO::Color>(c, "color");
  add_generic_maps<C, Sm, typename Sm::Point>(c, "point");
  add_generic_maps<C, Sm, py::list>(c, "list");
  add_generic_maps<C, Sm, py::set>(c, "set");
  add_generic_maps<C, Sm, py::tuple>(c, "tuple");
  add_generic_maps<C, Sm, Vector_3>(c, "vector");

#if __cplusplus >= 202002l
  if constexpr (! std::is_same<double, FT>::value) add_generic_maps<C, Sm, FT>(c, "FT");
#endif

  c.def("remove_all_property_maps", &Sm::remove_all_property_maps,
        sm_doc::Surface_mesh_remove_all_property_maps);
  c.def("remove_property_maps_vertex", &Sm::template remove_property_maps<Vi>,
        sm_doc::Surface_mesh_remove_property_maps);
  c.def("remove_property_maps_edge", &Sm::template remove_property_maps<Ei>,
        sm_doc::Surface_mesh_remove_property_maps);
  c.def("remove_property_maps_halfedge", &Sm::template remove_property_maps<Hi>,
        sm_doc::Surface_mesh_remove_property_maps);
  c.def("remove_property_maps_face", &Sm::template remove_property_maps<Fi>,
        sm_doc::Surface_mesh_remove_property_maps);
}

}
} // namespace cgalpy // namespace sm

//
template <typename SurfaceMesh, typename C>
void add_sm_index(C& c) {
  using Sm = SurfaceMesh;
  using size_type = typename Sm::size_type;
  using Sm_i = typename C::Type;

  c.def(py::init<>(),
        "Constructs an invalid Surface_mesh index.")
    .def(py::init<size_type>(),
         py::arg("idx"),
         "Constructs a Surface_mesh index from an integer index.")
    .def("id", &Sm_i::id,
         "Returns the internal index identifier.")
    .def("idx", &Sm_i::idx,
         "Returns the internal index value.")
    .def("is_valid", &Sm_i::is_valid,
         "Returns whether the index is valid.")
    .def("reset", &Sm_i::reset,
         "Resets the index to the invalid state.")
    ;
}

// Export Surface_mesh.
template <typename SurfaceMesh>
void export_surface_mesh_impl(py::module_& m, const char* name) {
  using Sm = SurfaceMesh;
  using Pnt = typename Sm::Point;
  using Vec = Kernel::Vector_3;
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
    py::class_<Vi, Sm_vi>(m, "Vertex_index",
                           sm_doc::Surface_mesh_Vertex_index_class)
      .def(py::init<>(),
           sm_doc::Surface_mesh_Vertex_index_Vertex_index)
      .def(py::init<size_type>(),
           py::arg("idx"),
           "Constructs a vertex index from an integer index.")
      .def("__str__", [](const Vi& vi) { return std::to_string(vi.idx()); })
      .def("__repr__", [](const Vi& vi) { return std::to_string(vi.idx()); })
      .def("__eq__", [](const Vi& vi, const Vi& other) { return vi == other; })
      .def("__ne__", [](const Vi& vi, const Vi& other) { return vi != other; })
      .def("__lt__", [](const Vi& vi, const Vi& other) { return vi < other; })
      .def("__hash__", [](const Vi& vi) { return py::hash(py::int_(vi.idx())); })
      ;
  }

  // Edge_index
  if (! add_attr<Ei>(m, "Edge_index")) {
    py::class_<Ei>(m, "Edge_index",
                   sm_doc::Surface_mesh_Edge_index_class)
      .def(py::init<>(),
           sm_doc::Surface_mesh_Edge_index_Edge_index)
      .def(py::init<size_type>(),
           py::arg("idx"),
           "Constructs an edge index from an integer index.")
      .def("halfedge", &Ei::halfedge,
           "Returns the associated halfedge index.")
      .def("idx", &Ei::idx,
           "Returns the internal index value.")
      .def("reset", &Ei::reset,
           "Resets the index to the invalid state.")
      .def("is_valid", &Ei::is_valid,
           "Returns whether the index is valid.")
      .def("__str__", [](const Ei& ei) { return std::to_string(ei.idx()); })
      .def("__repr__", [](const Ei& ei) { return std::to_string(ei.idx()); })
      .def("__eq__", [](const Ei& ei, const Ei& other) { return ei == other; })
      .def("__ne__", [](const Ei& ei, const Ei& other) { return ei != other; })
      .def("__lt__", [](const Ei& ei, const Ei& other) { return ei < other; })
      .def("__hash__", [](const Ei& ei) { return py::hash(py::int_(ei.idx())); })
      ;
  }

  // Halfedge_index
  using Sm_hi = typename CGAL::SM_Index<Hi>;
  if (! add_attr<Sm_hi>(m, "SM_halfedge_index")) {
    py::class_<Sm_hi> sm_hi_c(m, "SM_halfedge_index");
    add_sm_index<Sm>(sm_hi_c);
  }

  if (! add_attr<Hi>(m, "Halfedge_index")) {
    py::class_<Hi, Sm_hi>(m, "Halfedge_index",
                           sm_doc::Surface_mesh_Halfedge_index_class)
      .def(py::init<>(),
           sm_doc::Surface_mesh_Halfedge_index_Halfedge_index)
      .def(py::init<size_type>(),
           py::arg("idx"),
           "Constructs a halfedge index from an integer index.")
      .def("__str__", [](const Hi& hi) { return std::to_string(hi.idx()); })
      .def("__repr__", [](const Hi& hi) { return std::to_string(hi.idx()); })
      .def("__eq__", [](const Hi& hi, const Hi& other) { return hi == other; })
      .def("__ne__", [](const Hi& hi, const Hi& other) { return hi != other; })
      .def("__lt__", [](const Hi& hi, const Hi& other) { return hi < other; })
      .def("__hash__", [](const Hi& hi) { return py::hash(py::int_(hi.idx())); })
      ;
  }

  // Face_index
  using Sm_fi = typename CGAL::SM_Index<Fi>;
  if (! add_attr<Sm_fi>(m, "SM_face_index")) {
    py::class_<Sm_fi> sm_fi_c(m, "SM_face_index");
    add_sm_index<Sm>(sm_fi_c);
  }

  if (! add_attr<Fi>(m, "Face_index")) {
    py::class_<Fi, Sm_fi>(m, "Face_index",
                           sm_doc::Surface_mesh_Face_index_class)
      .def(py::init<>(),
           sm_doc::Surface_mesh_Face_index_Face_index)
      .def(py::init<size_type>(),
           py::arg("idx"),
           "Constructs a face index from an integer index.")
      .def("__str__", [](const Fi& fi) { return std::to_string(fi.idx()); })
      .def("__repr__", [](const Fi& fi) { return std::to_string(fi.idx()); })
      .def("__eq__", [](const Fi& fi, const Fi& other) { return fi == other; })
      .def("__ne__", [](const Fi& fi, const Fi& other) { return fi != other; })
      .def("__lt__", [](const Fi& fi, const Fi& other) { return fi < other; })
      .def("__hash__", [](const Fi& fi) { return py::hash(py::int_(fi.idx())); })
      ;
  }

  // Face descriptor
  if (! add_attr<Fd>(m, "Face_descriptor")) {
    py::class_<Fd>(m, "Face_descriptor",
                   "Descriptor of a Surface_mesh face.")
      .def(py::init<>(),
           "Constructs an invalid face descriptor.")
      .def(py::init<Fi>(),
           py::arg("face_index"),
           "Constructs a face descriptor from a face index.")
      .def("idx", &Fd::idx,
           "Returns the internal descriptor index.")
      .def("is_valid", &Fd::is_valid,
           "Returns whether the descriptor is valid.")
      .def("__str__", [](const Fd& fd){ return std::to_string(fd.idx()); })
      .def("__repr__", [](const Fd& fd){ return std::to_string(fd.idx()); })
      ;
  }

  // Halfedge descriptor
  if (! add_attr<Hd>(m, "Halfedge_descriptor")) {
    py::class_<Hd>(m, "Halfedge_descriptor",
                   "Descriptor of a Surface_mesh halfedge.")
      .def(py::init<>(),
           "Constructs an invalid halfedge descriptor.")
      .def(py::init<Hi>(),
           py::arg("halfedge_index"),
           "Constructs a halfedge descriptor from a halfedge index.")
      .def("idx", &Hd::idx,
           "Returns the internal descriptor index.")
      .def("is_valid", &Hd::is_valid,
           "Returns whether the descriptor is valid.")
      .def("__str__", [](const Hd& hd){ return std::to_string(hd.idx()); })
      .def("__repr__", [](const Hd& hd){ return std::to_string(hd.idx()); })
      ;
  }

  // Vertex_descriptor
  if (! add_attr<Vd>(m, "Vertex_descriptor")) {
    py::class_<Vd>(m, "Vertex_descriptor",
                   "Descriptor of a Surface_mesh vertex.")
      .def(py::init<>(),
           "Constructs an invalid vertex descriptor.")
      .def(py::init<Vi>(),
           py::arg("vertex_index"),
           "Constructs a vertex descriptor from a vertex index.")
      .def("idx", &Vd::idx,
           "Returns the internal descriptor index.")
      .def("is_valid", &Vd::is_valid,
           "Returns whether the descriptor is valid.")
      .def("__str__", [](const Vd& vd){ return std::to_string(vd.idx()); })
      .def("__repr__", [](const Vd& vd){ return std::to_string(vd.idx()); })
      ;
  }

  // Surface mesh
  if (! add_attr<Sm>(m, name)) {

    py::class_<Sm> sm_c(m, name, sm_doc::Surface_mesh_class);

    cgalpy::sm::add_maps<Sm, py::class_<Sm>>(sm_c);

    // Dynamic bool property maps are backed by std::vector<bool>, whose proxy
    // reference/iterator types cannot be exposed safely through nanobind.
    // Static bool property maps are exported below via export_property_map_bool().
    // cgalpy::sm::export_dynamic_property_maps<Sm, bool>(m, "bool");
    cgalpy::sm::export_dynamic_property_maps<Sm, int>(m, "int");
    cgalpy::sm::export_dynamic_property_maps<Sm, double>(m, "float");
    cgalpy::sm::export_dynamic_property_maps<Sm, std::size_t>(m, "size_t");
    cgalpy::sm::export_dynamic_property_maps<Sm, Pnt, py::rv_policy::reference_internal>(m, "point");
    cgalpy::sm::export_dynamic_property_maps<Sm, Vec, py::rv_policy::reference_internal>(m, "vector_3");
    cgalpy::sm::export_dynamic_property_maps<Sm, CGAL::IO::Color, py::rv_policy::reference_internal>(m, "color");
    cgalpy::sm::export_dynamic_property_maps<Sm, py::tuple>(m, "tuple");
    cgalpy::sm::export_dynamic_property_maps<Sm, py::set>(m, "set");

    if constexpr (! std::is_same<double, FT>::value)
      cgalpy::sm::export_dynamic_property_maps<Sm, FT, py::rv_policy::reference_internal>(m, "FT");


    sm_c.def(py::init<>(), sm_doc::Surface_mesh_Surface_mesh)
      .def(py::init<const Sm&>(),
           py::arg("rhs"),
           sm_doc::Surface_mesh_Surface_mesh_1)
      // .def("assign", &Sm::assign, ri)
      .def("add_vertex", py::overload_cast<>(&Sm::add_vertex),
           sm_doc::Surface_mesh_add_vertex)
      .def("add_vertex", py::overload_cast<const Pnt&>(&Sm::add_vertex),
           py::arg("p"),
           sm_doc::Surface_mesh_add_vertex_1)
      .def("add_edge", py::overload_cast<>(&Sm::add_edge),
           sm_doc::Surface_mesh_add_edge)
      .def("add_edge", py::overload_cast<Vi, Vi>(&Sm::add_edge),
           py::arg("v0"), py::arg("v1"),
           sm_doc::Surface_mesh_add_edge_1)
      .def("add_face", static_cast<Fi(Sm::*)()>(&Sm::add_face),
           sm_doc::Surface_mesh_add_face)
      .def("add_face", static_cast<Fi(Sm::*)(Vi, Vi, Vi)>(&Sm::add_face),
           py::arg("v0"), py::arg("v1"), py::arg("v2"),
           sm_doc::Surface_mesh_add_face_2)
      .def("add_face", static_cast<Fi(Sm::*)(Vi, Vi, Vi, Vi)>(&Sm::add_face),
           py::arg("v0"), py::arg("v1"), py::arg("v2"), py::arg("v3"),
           sm_doc::Surface_mesh_add_face_3)
      .def("add_face", &cgalpy::sm::add_face<Sm>,
           py::arg("vertices"),
           sm_doc::Surface_mesh_add_face_1)

      .def("has_valid_index", &cgalpy::sm::has_valid_index_v<Sm>, py::arg("v"),
           "Returns whether a vertex index is valid for this mesh.")
      .def("has_valid_index", &cgalpy::sm::has_valid_index_e<Sm>, py::arg("e"),
           "Returns whether an edge index is valid for this mesh.")
      .def("has_valid_index", &cgalpy::sm::has_valid_index_h<Sm>, py::arg("h"),
           "Returns whether a halfedge index is valid for this mesh.")
      .def("has_valid_index", &cgalpy::sm::has_valid_index_f<Sm>, py::arg("f"),
           "Returns whether a face index is valid for this mesh.")

      .def("remove_vertex", &Sm::remove_vertex,
           py::arg("v"),
           sm_doc::Surface_mesh_remove_vertex)
      .def("remove_edge", &Sm::remove_edge,
           py::arg("e"),
           sm_doc::Surface_mesh_remove_edge)
      .def("remove_face", &Sm::remove_face,
           py::arg("f"),
           sm_doc::Surface_mesh_remove_face)
      .def("num_vertices", &Sm::num_vertices,
           sm_doc::Surface_mesh_number_of_vertices)
      .def("number_of_vertices", &Sm::number_of_vertices,
           sm_doc::Surface_mesh_number_of_vertices)
      .def("num_halfedges", &Sm::num_halfedges,
           sm_doc::Surface_mesh_number_of_halfedges)
      .def("number_of_halfedges", &Sm::number_of_halfedges,
           sm_doc::Surface_mesh_number_of_halfedges)
      .def("num_edges", &Sm::num_edges,
           sm_doc::Surface_mesh_number_of_edges)
      .def("number_of_edges", &Sm::number_of_edges,
           sm_doc::Surface_mesh_number_of_edges)
      .def("num_faces", &Sm::num_faces,
           sm_doc::Surface_mesh_number_of_faces)
      .def("number_of_faces", &Sm::number_of_faces,
           sm_doc::Surface_mesh_number_of_faces)
      .def("is_empty", &Sm::is_empty,
           sm_doc::Surface_mesh_is_empty)
      .def("clear_without_removing_property_maps",
           &Sm::clear_without_removing_property_maps,
           sm_doc::Surface_mesh_clear_without_removing_property_maps)
      .def("clear", &Sm::clear,
           sm_doc::Surface_mesh_clear)
      .def("halfedge", [](const Sm& sm, Vi v) { return sm.halfedge(v); },
           py::arg("v"),
           sm_doc::Surface_mesh_halfedge)
      .def("halfedge", [](const Sm& sm, Fi f) { return sm.halfedge(f); },
           py::arg("f"),
           sm_doc::Surface_mesh_halfedge_1)
      .def("halfedge", [](const Sm& sm, Ei e) { return sm.halfedge(e); },
           py::arg("e"),
           sm_doc::Surface_mesh_halfedge_3)
      .def("halfedge", [](const Sm& sm, Ei e, unsigned int i) { return sm.halfedge(e, i); },
           py::arg("e"), py::arg("i"),
           sm_doc::Surface_mesh_halfedge_4)
      .def("halfedge", [](const Sm& sm, Vi source, Vi target) { return sm.halfedge(source, target); },
           py::arg("source"), py::arg("target"),
           sm_doc::Surface_mesh_halfedge_2)
      .def("degree", [](const Sm& sm, Vi v) { return sm.degree(v); },
           py::arg("v"),
           sm_doc::Surface_mesh_degree)
      .def("degree", [](const Sm& sm, Fi f) { return sm.degree(f); },
           py::arg("f"),
           sm_doc::Surface_mesh_degree_1)
      .def("is_border", [](const Sm& sm, Hi h) { return sm.is_border(h); },
           py::arg("h"),
           sm_doc::Surface_mesh_is_border_1)
      .def("is_border", [](const Sm& sm, Ei e) { return sm.is_border(e); },
           py::arg("e"),
           sm_doc::Surface_mesh_is_border_2)
      .def("is_border",
           [](const Sm& sm, Vi v, bool check_all_incident_halfedges = true)
           { return sm.is_border(v, check_all_incident_halfedges); },
           py::arg("v"), py::arg("check_all_incident_halfedges") = true,
           sm_doc::Surface_mesh_is_border)
      .def("is_removed", [](const Sm& sm, Vi v) { return sm.is_removed(v); },
           py::arg("v"),
           sm_doc::Surface_mesh_is_removed)
      .def("is_removed", [](const Sm& sm, Ei e) { return sm.is_removed(e); },
           py::arg("e"),
           sm_doc::Surface_mesh_is_removed_2)
      .def("is_removed", [](const Sm& sm, Fi f) { return sm.is_removed(f); },
           py::arg("f"),
           sm_doc::Surface_mesh_is_removed_3)
      .def("is_removed", [](const Sm& sm, Hi h) { return sm.is_removed(h); },
           py::arg("h"),
           sm_doc::Surface_mesh_is_removed_1)
      .def("source", &Sm::source,
           py::arg("h"),
           sm_doc::Surface_mesh_source)
      .def("target", &Sm::target,
           py::arg("h"),
           sm_doc::Surface_mesh_target)
      // void reserve(size_type nvertices, size_type nedges, size_type nfaces )
      // void resize(size_type nvertices, size_type nedges, size_type nfaces )
      // join(const Surface_mesh& other)
      .def("edge", &Sm::edge,
           py::arg("h"),
           sm_doc::Surface_mesh_edge)
      .def("face", &Sm::face,
           py::arg("h"),
           sm_doc::Surface_mesh_face)
      .def("join", &Sm::join,
           py::arg("other"),
           sm_doc::Surface_mesh_join)
      .def("next", &Sm::next,
           py::arg("h"),
           sm_doc::Surface_mesh_next)
      .def("prev", &Sm::prev,
           py::arg("h"),
           sm_doc::Surface_mesh_prev)
      .def("resize", &Sm::resize,
           py::arg("nvertices"), py::arg("nedges"), py::arg("nfaces"),
           "Resizes the mesh storage.")
      .def("set_target", &Sm::set_target,
           py::arg("h"), py::arg("v"),
           sm_doc::Surface_mesh_set_target)
      .def("has_garbage", &Sm::has_garbage,
           sm_doc::Surface_mesh_has_garbage)
      .def("collect_garbage", [](Sm& sm) { sm.collect_garbage(); },
           sm_doc::Surface_mesh_collect_garbage)
      .def("is_isolated", &Sm::is_isolated,
           py::arg("v"),
           sm_doc::Surface_mesh_is_isolated)
      .def("set_next_only", &Sm::set_next_only,
           py::arg("h"), py::arg("nh"),
           "Sets the next halfedge without updating the opposite previous link.")
      .def("set_prev_only", &Sm::set_prev_only,
           py::arg("h"), py::arg("ph"),
           "Sets the previous halfedge without updating the opposite next link.")
      .def("shrink_to_fit", &Sm::shrink_to_fit,
           "Requests reduction of unused mesh storage.")
      .def("set_recycle_garbage", &Sm::set_recycle_garbage,
           py::arg("do_recycle"),
           sm_doc::Surface_mesh_set_recycle_garbage)
      .def("does_recycle_garbage", &Sm::does_recycle_garbage,
           sm_doc::Surface_mesh_does_recycle_garbage)

      .def("number_of_removed_edges", &Sm::number_of_removed_edges,
           sm_doc::Surface_mesh_number_of_removed_edges)
      .def("number_of_removed_faces", &Sm::number_of_removed_faces,
           sm_doc::Surface_mesh_number_of_removed_faces)
      .def("number_of_removed_vertices", &Sm::number_of_removed_vertices,
           sm_doc::Surface_mesh_number_of_removed_vertices)
      .def("number_of_removed_halfedges", &Sm::number_of_removed_halfedges,
           sm_doc::Surface_mesh_number_of_removed_halfedges)

      .def("next_around_source", &Sm::next_around_source,
           py::arg("h"),
           sm_doc::Surface_mesh_next_around_source)
      .def("prev_around_source", &Sm::prev_around_source,
           py::arg("h"),
           sm_doc::Surface_mesh_prev_around_source)
      .def("next_around_target", &Sm::next_around_target,
           py::arg("h"),
           sm_doc::Surface_mesh_next_around_target)
      .def("prev_around_target", &Sm::prev_around_target,
           py::arg("h"),
           sm_doc::Surface_mesh_prev_around_target)

      // .def("property_stats", &Sm::property_stats)

      .def("point", &cgalpy::sm::my_point<Sm>, ri,
           py::arg("v"),
           sm_doc::Surface_mesh_point)
      .def("points", &cgalpy::sm::points<Sm, Vi, Pnt>,
           sm_doc::Surface_mesh_points)

      // .def("__iadd__",
      //      py::self += py::self,
      //      "Inserts other into sm.")
      .def("__iadd__",
           [](Sm& sm, const Sm& other) { sm.join(other); return sm; },
           "Inserts other into sm.",
           py::is_operator())

      .def("is_valid", py::overload_cast<bool>(&Sm::is_valid, py::const_),
           py::arg("verbose") = false,
           sm_doc::Surface_mesh_is_valid)
#if CGAL_VERSION_NR >= 1050600000
      .def("is_valid", py::overload_cast<Vi, bool>(&Sm::is_valid, py::const_),
           py::arg("v"), py::arg("verbose") = false,
           sm_doc::Surface_mesh_is_valid_1)
      .def("is_valid", py::overload_cast<Ei, bool>(&Sm::is_valid, py::const_),
           py::arg("e"), py::arg("verbose") = false,
           sm_doc::Surface_mesh_is_valid_3)
      .def("is_valid", py::overload_cast<Hi, bool>(&Sm::is_valid, py::const_),
           py::arg("h"), py::arg("verbose") = false,
           sm_doc::Surface_mesh_is_valid_2)
      .def("is_valid", py::overload_cast<Fi, bool>(&Sm::is_valid, py::const_),
           py::arg("f"), py::arg("verbose") = false,
           sm_doc::Surface_mesh_is_valid_4)
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

    sm_c.def("vertices", &cgalpy::sm::vertices<Sm>,
             py::keep_alive<0, 1>(),
             sm_doc::Surface_mesh_vertices)
      .def("halfedges", &cgalpy::sm::halfedges<Sm>,
           py::keep_alive<0, 1>(),
           sm_doc::Surface_mesh_halfedges)
      .def("edges", &cgalpy::sm::edges<Sm>,
           py::keep_alive<0, 1>(),
           sm_doc::Surface_mesh_edges)
      .def("faces", &cgalpy::sm::faces<Sm>,
           py::keep_alive<0, 1>(),
           sm_doc::Surface_mesh_faces)
      ;

    add_insertion(sm_c, "__str__");
    add_insertion(sm_c, "__repr__");
  }

#ifdef CGALPY_HAS_VISUAL
  m.def("faces", &boost_utils::my_faces<Sm>,
        py::arg("graph"), py::keep_alive<0, 1>(), bgl_doc::FaceListGraph_faces);

#endif
}

// Export Surface_mesh<Pnt>
void export_surface_mesh(py::module_& m) {
  // 2D Surface mesh
  // using Sm_2 = cgalpy::sm::Surface_mesh_2;
  // export_surface_mesh_impl<Sm_2>(m, "Surface_mesh_2");

  // 3D Surface mesh
  using Kernel_ = Kernel;
  using Sm_3 = cgalpy::sm::Surface_mesh_3;
  using Vi = typename Sm_3::Vertex_index;
  using Fi = typename Sm_3::Face_index;
  using Hi = typename Sm_3::Halfedge_index;
  using Ei = typename Sm_3::Edge_index;
  using Pnt = Kernel_::Point_3;
  using Vector_3 = Kernel_::Vector_3;
  constexpr auto ri(py::rv_policy::reference_internal);

  export_surface_mesh_impl<Sm_3>(m, "Surface_mesh_3");

  // cgalpy::sm::vertex_map<Sm_3, Pnt>(m, "vertex_point_boost_map", "Vertex_point_boost_map"); //this is the boost::property_map

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
  internal::export_property_map<Sm_3, Vi, Pcad>(m, "Vertex_Principal_curvatures_and_directions_map");
#endif

  // Export all property maps andled by Surface_mesh
  cgalpy::sm::export_property_maps<py::module_, Sm_3, int>(m, "int");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, FT>(m, "FT");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, std::string>(m, "string");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, std::size_t>(m, "size_t");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, CGAL::IO::Color>(m, "Color");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, py::tuple>(m, "tuple");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, py::list>(m, "list");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, py::set>(m, "set");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, Vector_3>(m, "vector_3");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, Plane_3>(m, "Plane_3");
  cgalpy::sm::export_property_maps<py::module_, Sm_3, Point_3>(m, "Point_3");

  if constexpr (! std::is_same<double, FT>::value) cgalpy::sm::export_property_maps<py::module_, Sm_3, double>(m, "float");

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

  py::class_<boost::vector_property_map<Vector_3>>(m, "Vector_vector_3_map",
                                                   "Writable property map from indices to 3D vectors.")
    .def(py::init<>(),
         "Constructs an empty vector property map.")
    ;

  // void set_selected_faces(const FacePatchIDRange& selected_face_patch_ids,
  //                         FacePatchIDMap face_patch_id_map

  using Ffg3 = CGAL::Face_filtered_graph<Sm_3>;

  py::class_<Ffg3>(m, "Face_filtered_graph",
                    "Face-filtered view of a Surface_mesh.")
    .def(py::init<const Sm_3&, std::size_t, const Sm_3::Property_map<Fi, std::size_t>&>(),
         py::arg("graph"), py::arg("selected_face_patch_id"), py::arg("face_patch_id_map"),
         "Constructs a face-filtered graph view.")
    .def("graph", [](const Ffg3& ffg) { return ffg.graph(); },
         "Returns the underlying graph.")
    .def("reset_indices", [](Ffg3& ffg) { return ffg.reset_indices(); },
         "Resets cached indices of the filtered graph.")
    .def("number_of_faces", [](const Ffg3& ffg) { return ffg.number_of_faces(); },
         bgl_doc::Face_filtered_graph_number_of_faces)
    .def("invert_selection", [](Ffg3& ffg) { return ffg.invert_selection(); },
         "Inverts the face selection.")
    // commented for stubs
    // .def("get_face_index_map",
    //    [](const Ffg3& ffg) { return ffg.get_face_index_map(); })
    .def("number_of_vertices",
         [](const Ffg3& ffg) { return ffg.number_of_vertices(); },
         bgl_doc::Face_filtered_graph_number_of_vertices)
    .def("number_of_halfedges",
         [](const Ffg3& ffg) { return ffg.number_of_halfedges(); },
         bgl_doc::Face_filtered_graph_number_of_halfedges)
    // .def("get_vertex_index_map",
    //   [](const Ffg3& ffg) { return ffg.get_vertex_index_map(); })
    // .def("get_halfedge_index_map",
    //   [](const Ffg3& ffg) { return ffg.get_halfedge_index_map(); })
    .def("initialize_face_indices", [](Ffg3& ffg) { return ffg.initialize_face_indices(); },
         "Initializes face indices for the filtered graph.")
    .def("initialize_vertex_indices", [](Ffg3& ffg) { return ffg.initialize_vertex_indices(); },
         "Initializes vertex indices for the filtered graph.")
    .def("initialize_halfedge_indices", [](Ffg3& ffg) { return ffg.initialize_halfedge_indices(); },
         "Initializes halfedge indices for the filtered graph.")
    .def("set_selected_faces",
         [](Ffg3& ffg, const std::vector<std::size_t>& vec, const Sm_3::Property_map<Fi, std::size_t>& fccmap) {
           return ffg.set_selected_faces(vec, fccmap);
         },
         py::arg("selected_face_patch_ids"), py::arg("face_patch_id_map"),
         bgl_doc::Face_filtered_graph_set_selected_faces)
    ;

  // Face filtered Graph
  m.def("clear", &CGAL::clear<Sm_3>, py::arg("graph"), bgl_doc::clear);


  // Still Need Sorting
  m.def("is_closed", &CGAL::is_closed<Sm_3>,
        py::arg("graph"), bgl_doc::is_closed);

  m.def("get_vertex_point",
        [](const Sm_3& pm, const Vi& vd) { return get(CGAL::vertex_point, pm, vd); },
        py::arg("graph"), py::arg("vertex"),
        "Returns the point associated with a vertex descriptor.");


  //! \todo export CGAL::vertex_point and CGAL::get() instead.

  // m.def("expand_face_selection",
  //       &cgalpy::sm::expand_face_selection<Fi, Sm_3, Sm_3::Property_map<Fi, int>>);
  m.def("is_triangle", &cgalpy::sm::is_triangle<Sm_3>,
        py::arg("halfedge"), py::arg("graph"), bgl_doc::is_triangle);

  m.def("is_triangle_mesh", &CGAL::is_triangle_mesh<Sm_3>,
        py::arg("graph"), bgl_doc::is_triangle_mesh);

  m.def("add_edge", &cgalpy::bgl::my_add_edge<Sm_3>,
        py::arg("graph"), bgl_doc::MutableHalfedgeGraph_add_edge);


  // CGAL and the Boost Graph Library

  // Global functions
  m.def("add_face", &cgalpy::bgl::my_add_face<Sm_3>,
        py::arg("graph"), bgl_doc::MutableFaceGraph_add_face);

  m.def("add_vertex", &cgalpy::bgl::my_add_vertex<Sm_3>,
        py::arg("graph"), bgl_doc::MutableHalfedgeGraph_add_vertex);

  m.def("num_edges", &cgalpy::bgl::my_num_edges<Sm_3>,
        py::arg("graph"), bgl_doc::EdgeListGraph_num_edges);

  // Not documented
  // m.def("collect_garbage", &cgalpy::bgl::my_collect_garbage<Sm_3>);
  m.def("num_faces", &cgalpy::bgl::my_num_faces<Sm_3>,
        py::arg("graph"), bgl_doc::FaceListGraph_num_faces);

  m.def("num_halfedges", &cgalpy::bgl::my_num_halfedges<Sm_3>,
        py::arg("graph"), bgl_doc::HalfedgeListGraph_num_halfedges);

  m.def("num_vertices", &cgalpy::bgl::my_num_vertices<Sm_3>,
        py::arg("graph"), bgl_doc::VertexListGraph_num_vertices);

  m.def("remove_all_elements", &cgalpy::bgl::my_remove_all_elements<Sm_3>,
        py::arg("graph"),
        "Removes all vertices, edges, halfedges, and faces from the graph.");

  m.def("reserve", &cgalpy::bgl::my_reserve<Sm_3>,
        py::arg("graph"), py::arg("num_vertices"), py::arg("num_edges"),
        py::arg("num_faces"), bgl_doc::MutableFaceGraph_reserve);

  //! Obtain the propery maps
#ifdef CGALPY_BGL_BINDINGS
  m.def("get", [](CGAL::vertex_point_t tag, Sm_3& g) { return CGAL::get(tag, g); }, ri,
        py::arg("tag"), py::arg("graph"),
        "Returns the vertex point property map.");
  m.def("get", [](CGAL::vertex_index_t tag, Sm_3& g) { return CGAL::get(tag, g); }, ri,
        py::arg("tag"), py::arg("graph"),
        "Returns the vertex index property map.");
  m.def("get", [](CGAL::halfedge_index_t tag, Sm_3& g) { return CGAL::get(tag, g); }, ri,
        py::arg("tag"), py::arg("graph"),
        "Returns the halfedge index property map.");
  m.def("get", [](CGAL::face_index_t tag, Sm_3& g) { return CGAL::get(tag, g); }, ri,
        py::arg("tag"), py::arg("graph"),
        "Returns the face index property map.");
  m.def("adjacent_vertices", &cgalpy::bgl::adjacent_vertices<Sm_3>,
        py::arg("vertex"), py::arg("graph"), bgl_doc::adjacent_vertices);

#endif

  // Other
  m.def("add_vertex", &cgalpy::bgl::add_vertex_p<Sm_3>,
        py::arg("point"), py::arg("graph"), bgl_doc::MutableHalfedgeGraph_add_vertex);

  m.def("degree", &cgalpy::bgl::degree_v<Sm_3>,
        py::arg("vertex"), py::arg("graph"), bgl_doc::degree);

  m.def("degree", &cgalpy::bgl::degree_f<Sm_3>,
        py::arg("face"), py::arg("graph"), bgl_doc::FaceGraph_degree);

  m.def("edge", &cgalpy::bgl::edge<Sm_3>,
        py::arg("halfedge"), py::arg("graph"), bgl_doc::HalfedgeGraph_edge);

  m.def("edge", &cgalpy::bgl::edge_vv<Sm_3>,
        py::arg("source"), py::arg("target"), py::arg("graph"),
        "Returns the edge between two vertices, if it exists.");

  m.def("face", &cgalpy::bgl::face<Sm_3>,
        py::arg("halfedge"), py::arg("graph"), bgl_doc::FaceGraph_face);

  m.def("halfedge", &cgalpy::bgl::halfedge_e<Sm_3>,
        py::arg("edge"), py::arg("graph"), bgl_doc::HalfedgeGraph_halfedge);

  m.def("halfedge", &cgalpy::bgl::halfedge_f<Sm_3>,
        py::arg("face"), py::arg("graph"), bgl_doc::FaceGraph_halfedge);

  m.def("halfedge", &cgalpy::bgl::halfedge_v<Sm_3>,
        py::arg("vertex"), py::arg("graph"), bgl_doc::HalfedgeGraph_halfedge_1);

  m.def("halfedge", &cgalpy::bgl::halfedge_vv<Sm_3>,
        py::arg("source"), py::arg("target"), py::arg("graph"),
        bgl_doc::HalfedgeGraph_halfedge_2);

  m.def("in_degree", &cgalpy::bgl::in_degree<Sm_3>,
        py::arg("vertex"), py::arg("graph"), bgl_doc::in_degree);

  m.def("is_border", &cgalpy::bgl::my_is_border_h<Sm_3>,
        py::arg("halfedge"), py::arg("graph"),
        "Tests whether a halfedge is on the border.");

  m.def("is_border", &cgalpy::bgl::my_is_border_e<Sm_3>,
        py::arg("edge"), py::arg("graph"),
        "Tests whether an edge is on the border.");

  m.def("is_border", &cgalpy::bgl::my_is_border_v<Sm_3>,
        py::arg("vertex"), py::arg("graph"),
        "Tests whether a vertex is incident to the border.");

  m.def("is_border", &cgalpy::bgl::my_is_border_edge<Sm_3>,
        py::arg("halfedge"), py::arg("graph"),
        "Tests whether the edge corresponding to a halfedge is on the border.");

  m.def("is_valid_vertex_descriptor", &cgalpy::bgl::my_is_valid_vertex_descriptor<Sm_3>,
        py::arg("v"), py::arg("g"), py::arg("verbose") = false,
        "Returns whether a vertex descriptor is valid for the graph.");
  m.def("is_valid_halfedge_descriptor", &cgalpy::bgl::my_is_valid_halfedge_descriptor<Sm_3>,
        py::arg("h"), py::arg("g"), py::arg("verbose") = false,
        bgl_doc::is_valid_halfedge_descriptor);
  m.def("is_valid_edge_descriptor", &cgalpy::bgl::my_is_valid_edge_descriptor<Sm_3>,
        py::arg("e"), py::arg("g"), py::arg("verbose") = false,
        bgl_doc::is_valid_edge_descriptor);
  m.def("next", &cgalpy::bgl::next<Sm_3>,
        py::arg("halfedge"), py::arg("graph"), bgl_doc::HalfedgeGraph_next);

  m.def("null_face", &cgalpy::bgl::null_face<Sm_3>,
        bgl_doc::FaceGraph_null_face);

  m.def("null_halfedge", &cgalpy::bgl::null_halfedge<Sm_3>,
        bgl_doc::HalfedgeGraph_null_halfedge);

  m.def("null_vertex", &cgalpy::bgl::null_vertex<Sm_3>,
        "Returns a null vertex descriptor.");

  m.def("opposite", &cgalpy::bgl::opposite<Sm_3>,
        py::arg("halfedge"), py::arg("graph"), bgl_doc::HalfedgeGraph_opposite);

  m.def("out_degree", &cgalpy::bgl::out_degree<Sm_3>,
        py::arg("vertex"), py::arg("graph"), bgl_doc::out_degree);

  m.def("prev", &cgalpy::bgl::prev<Sm_3>,
        py::arg("halfedge"), py::arg("graph"), bgl_doc::HalfedgeGraph_prev);

  m.def("source", &cgalpy::bgl::source_e<Sm_3>,
        py::arg("edge"), py::arg("graph"), bgl_doc::EdgeListGraph_source);

  m.def("source", &cgalpy::bgl::source_h<Sm_3>,
        py::arg("halfedge"), py::arg("graph"), bgl_doc::HalfedgeGraph_source);

  m.def("target", &cgalpy::bgl::target_e<Sm_3>,
        py::arg("edge"), py::arg("graph"), bgl_doc::EdgeListGraph_target);

  m.def("target", &cgalpy::bgl::target_h<Sm_3>,
        py::arg("halfedge"), py::arg("graph"), bgl_doc::HalfedgeGraph_target);

  m.def("remove_edge", &cgalpy::bgl::remove_edge_e<Sm_3>,
        py::arg("edge"), py::arg("graph"), bgl_doc::MutableHalfedgeGraph_remove_edge);

  m.def("remove_face", &cgalpy::bgl::remove_face<Sm_3>,
        py::arg("face"), py::arg("graph"), bgl_doc::MutableFaceGraph_remove_face);

  // Fails to compile, but also not documented
  // m.def("remove_edge", &cgalpy::bgl::remove_edge_vv<Sm_3>);
  m.def("remove_vertex", &cgalpy::bgl::remove_vertex<Sm_3>,
        py::arg("vertex"), py::arg("graph"),
        bgl_doc::MutableHalfedgeGraph_remove_vertex);

  m.def("set_face", &cgalpy::bgl::set_face<Sm_3>,
        py::arg("halfedge"), py::arg("face"), py::arg("graph"),
        bgl_doc::MutableFaceGraph_set_face);

  m.def("set_halfedge", &cgalpy::bgl::set_halfedge_vh<Sm_3>,
        py::arg("vertex"), py::arg("halfedge"), py::arg("graph"),
        bgl_doc::MutableHalfedgeGraph_set_halfedge);

  m.def("set_halfedge", &cgalpy::bgl::set_halfedge_fh<Sm_3>,
        py::arg("face"), py::arg("halfedge"), py::arg("graph"),
        bgl_doc::MutableFaceGraph_set_halfedge);

  m.def("set_next", &cgalpy::bgl::set_next<Sm_3>,
        py::arg("halfedge1"), py::arg("halfedge2"), py::arg("graph"),
        bgl_doc::MutableHalfedgeGraph_set_next);

  m.def("set_target", &cgalpy::bgl::set_target<Sm_3>,
        py::arg("halfedge"), py::arg("vertex"), py::arg("graph"),
        bgl_doc::MutableHalfedgeGraph_set_target);

  m.def("vertices", &boost_utils::my_vertices<Sm_3>,
        py::arg("graph"), py::keep_alive<0, 1>(), bgl_doc::VertexListGraph_vertices);


  m.def("edges", &boost_utils::my_edges<Sm_3>,
        py::arg("graph"), py::keep_alive<0, 1>(), bgl_doc::EdgeListGraph_edges);

  m.def("in_edges", &boost_utils::my_in_edges<Sm_3>,
        py::arg("vertex"), py::arg("graph"), bgl_doc::in_edges);

  m.def("out_edges", &boost_utils::my_out_edges<Sm_3>,
        py::arg("vertex"), py::arg("graph"), bgl_doc::out_edges);

  m.def("halfedges", &boost_utils::my_halfedges<Sm_3>,
        py::arg("graph"), py::keep_alive<0, 1>(),
        bgl_doc::HalfedgeListGraph_halfedges);

  m.def("faces", &boost_utils::my_faces<Sm_3>,
        py::arg("graph"), py::keep_alive<0, 1>(), bgl_doc::FaceListGraph_faces);

  m.def("make_tetrahedron", &cgalpy::bgl::my_make_tetrahedron<Sm_3>,
        py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("p4"),
        bgl_doc::make_tetrahedron);


  // Generator Functions

  // Euler operations
  boost_utils::define_euler_operations<py::module_, Sm_3, ebmap_type>(m);

  // Iterators and Circulators
  export_bgl_iterators<Sm_3>(m);

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
