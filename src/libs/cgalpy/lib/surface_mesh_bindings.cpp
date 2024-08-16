// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/property_map/vector_property_map.hpp>
#define CGAL_USE_BASIC_VIEWER

#include <string>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/optional.h>

#include <CGAL/boost/graph/generators.h>
#include <CGAL/property_map.h>
#include <CGAL/Dynamic_property_map.h>
#include <CGAL/IO/polygon_soup_io.h>
#include <CGAL/boost/graph/selection.h>
#include <CGAL/boost/graph/Face_filtered_graph.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
#include <CGAL/IO/Color.h>
#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_surface_mesh.h>
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/Property_map.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/surface_mesh_types.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/export_boost_mesh_utils.hpp"
#include "CGALPY/export_mesh_iterators.hpp"
#include "CGALPY/get.hpp"
#include "CGALPY/helpers.hpp"
#include "CGALPY/internal.hpp"
#include "CGALPY/generator_functions.hpp"

namespace py = nanobind;

namespace sm {

template <typename Map_type>
void register_map(py::module_& m, const std::string& map_name) {
  py::class_<Map_type>(m, map_name.c_str())
    .def(py::init<>())
    .def_ro("map_", &Map_type::map_)
    ;
}

template <typename Dp, typename Mesh>
void register_map_get(py::module_& m, const std::string& prop_name) {
  py::class_<Dp> prop(m, prop_name.c_str());
  prop.def(py::init<>());
  m.def("get", &internal::get<Dp, Mesh>,
        py::arg("property_map"), py::arg("sm"));
}

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

template <typename Pm, typename P>
void edge_bool_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Ed = typename boost::graph_traits<Pm>::edge_descriptor;
  using dp = CGAL::dynamic_edge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Ed& e) { return py::bool_(get(p, e)); },
        py::arg("property_map"), py::arg("edge_descriptor"));
}

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

template <typename Pm, typename P>
void face_bool_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Fd = typename boost::graph_traits<Pm>::face_descriptor;
  using dp = CGAL::dynamic_face_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Fd& f) { return py::bool_(get(p, f)); },
        py::arg("property_map"), py::arg("face_descriptor"));
}

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

template <typename Pm, typename P>
void vertex_bool_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using dp = CGAL::dynamic_vertex_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Vd& v) { return py::bool_(get(p, v)); },
        py::arg("property_map"), py::arg("vertex_descriptor"));
}


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

template <typename Pm, typename P>
void halfedge_bool_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Hd = typename boost::graph_traits<Pm>::halfedge_descriptor;
  using dp = CGAL::dynamic_halfedge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Hd& h) { return py::bool_(get(p, h)); },
        py::arg("property_map"), py::arg("halfedge_descriptor"));
}

//
template <typename SurfaceMesh>
SurfaceMesh make_tetrahedron(const typename SurfaceMesh::Point& p1,
                             const typename SurfaceMesh::Point& p2,
                             const typename SurfaceMesh::Point& p3,
                             const typename SurfaceMesh::Point& p4) {
  using Sm = SurfaceMesh;

  Sm sm;
  CGAL::make_tetrahedron(p1, p2, p3, p4, sm);
  return sm;
}

// Read a surface mesh from a file.
template <typename SurfaceMesh>
SurfaceMesh read_polygon_mesh(const std::string& filename,
                              const py::dict& parameters = py::dict()) {
  using Sm = SurfaceMesh;

  Sm sm;
  if (! CGAL::IO::read_polygon_mesh(filename, sm,
                                    internal::parse_named_parameters(parameters)))
    throw std::runtime_error("Cannot read file!");
  return sm;
}
  // CGAL::expand_face_selection(seed, mesh, 5, selected, std::back_inserter(patch));

template <class Face, class FaceGraph, class IsFaceSelectedPMap>
auto expand_face_selection(const py::list& seed, const FaceGraph& mesh,
                           std::size_t max_distance, const IsFaceSelectedPMap& selected) {
  std::vector<typename boost::graph_traits<FaceGraph>::face_descriptor> patch;
  auto seed_vec = pmp::list2vec<Face>(seed);
  CGAL::expand_face_selection(seed_vec, mesh, max_distance, selected, std::back_inserter(patch));
  return pmp::vec2list(patch);
}

// Read Polygon soup from a file
template <typename SurfaceMesh>
auto read_polygon_soup(const std::string& fname,
                              const py::dict& np = py::dict()) {
  std::vector<Point_3> points;
  std::vector<std::vector<std::size_t> > polygons;

  if (! CGAL::IO::read_polygon_soup(fname, points, polygons))
    throw std::runtime_error("Cannot read file!");

  py::list pnt_lst, polygons_lst;
  for (auto p : points) {
    pnt_lst.append(p);
  }
  for (auto poly : polygons) {
    py::list new_poly;
    for (auto pt : poly) {
      new_poly.append(pt);
    }
    polygons_lst.append(new_poly);
  }

  return py::make_tuple(pnt_lst, polygons_lst);
}

//
template <typename SurfaceMesh>
bool write_polygon_mesh(std::string fname, const SurfaceMesh& pm,
                        const py::dict& parameters = py::dict()) {
  return CGAL::IO::write_polygon_mesh(fname, pm,
                                      internal::parse_named_parameters(parameters));
}

// Draw a surface mesh.
#ifdef CGALPY_HAS_VISUAL
template <typename SurfaceMesh>
void draw(const SurfaceMesh& sm) { CGAL::draw(sm); }
#endif

// Obtain the null face.
template <typename SurfaceMesh>
static typename SurfaceMesh::Face_index null_face()
{ return SurfaceMesh::null_face(); }

// Add a face from a list of vertices.
template <typename SurfaceMesh>
typename SurfaceMesh::Face_index add_face(SurfaceMesh& sm, py::list& lst) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;

  auto begin = stl_input_iterator<Vi>(lst);
  auto end = stl_input_iterator<Vi>(lst, false);
  return sm.add_face(CGAL::make_range(begin, end));
}


//
template <typename SurfaceMesh>
typename SurfaceMesh::Halfedge_index halfedge(const typename SurfaceMesh::Face_index& f,
                                              const SurfaceMesh& sm)
{ return CGAL::halfedge(f, sm); }

template <typename SurfaceMesh>
typename SurfaceMesh::Point my_point(const SurfaceMesh& sm,
                                     const typename SurfaceMesh::Vertex_index& v)
{ return sm.point(v); }

template <typename SurfaceMesh, typename Vi, typename Pnt>
auto points(const SurfaceMesh& sm) {
  return sm.points();
}

//
template <typename SurfaceMesh>
bool is_triangle(const typename SurfaceMesh::Halfedge_index& h, const SurfaceMesh& sm)
{ return CGAL::is_triangle(h, sm); }


//
template <typename SurfaceMesh>
auto has_valid_index_v(const SurfaceMesh& sm, typename SurfaceMesh::Vertex_index& vi)
{ return sm.is_valid(vi); }

//
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
auto add_map(SurfaceMesh& sm, const std::string& name = std::string(),
             const Value& default_value = Value()) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  using Pnt = typename Sm::Point;
  using Map = typename Sm::template Property_map<Vi, Pnt>;
  // Surface_mesh::Property_map<edge_descriptor, bool> is_constrained_map =
  //   mesh2.add_property_map<edge_descriptor, bool>("e:is_constrained", true).first;

  auto res = sm.template add_property_map<Key, Value>(name, default_value);
  return py::make_tuple(res.first, res.second);
}

template <typename T>
auto make_random_access_property_map(const py::list& lst) {
  std::vector<T> vec;
  for (auto v : lst) {
    try {
      vec.push_back(py::cast<T>(v));
    }
    catch (const py::cast_error&) {
      throw std::runtime_error("Cannot cast to map value type");
    }
  }
  return CGAL::make_random_access_property_map(vec);
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

template <typename C, typename Mesh, typename Key, typename ValueType>
C add_generic_map(C& c, const std::string& map_name, const ValueType& default_value = ValueType()) {
  return c.def(("add_" + map_name).c_str(), sm::add_map<Mesh, Key, ValueType>,
               py::arg("name") = std::string(), py::arg("default_value") = default_value,
               "adds a property map named `name` with value type `T` and default `t`\n"
               "for index type `I`. Returns the property map together with a Boolean\n"
               "that is `true` if a new map was created. In case it already exists\n"
               "the existing map together with `false` is returned.")
          .def(map_name.c_str(), &Mesh::template property_map<Key, ValueType>,
               py::arg("name") = std::string(),
               "returns an optional property map named `name` with key type `I` and value type `T`.")
  ;
}

template <typename SurfaceMesh, typename C>
C add_maps(C& c) {
  using Sm = SurfaceMesh;
  using Vi = typename Sm::Vertex_index;
  using Ei = typename Sm::Edge_index;
  using Hi = typename Sm::Halfedge_index;
  using Fi = typename Sm::Face_index;
  using Pnt = typename Sm::Point;
  using Pcad = CGAL::Polygon_mesh_processing::Principal_curvatures_and_directions<Kernel>;
  c.def("property_map_vertex_set_int", [](Sm& sm, const std::string& name, const py::set& default_value = py::set()) {
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
    .def("add_property_map_vertex_Principal_curvatures_and_directions", &sm::add_map<Sm, Vi, Pcad>,
       py::arg("name"), py::arg("default_value"),
        "adds a property map named `name` with value type `Principal_curvatures_and_directions` and default `default_value`\n"
        "for index type `Vertex_index`. Returns the property map together with a Boolean\n"
        "that is `true` if a new map was created. In case it already exists\n"
        "the existing map together with `false` is returned.")
    .def("property_map_vertex_Principal_curvatures_and_directions", &Sm::template property_map<Vi, Pcad>,
        py::arg("name") = std::string(),
          "returns an optional property map named `name` with key type `Vertex_index` and value type `Principal_curvatures_and_directions`.")
    ;
  add_generic_map<C, Sm, Vi, std::string>(c, "property_map_vertex_string");
  add_generic_map<C, Sm, Vi, CGAL::IO::Color>(c, "add_property_map_vertex_color");
  add_generic_map<C, Sm, Vi, typename Sm::Point>(c, "property_map_vertex_point");
  add_generic_map<C, Sm, Vi, bool>(c, "property_map_vertex_bool");
  add_generic_map<C, Sm, Vi, std::size_t>(c, "property_map_vertex_size_t");
  add_generic_map<C, Sm, Vi, Vector_3>(c, "property_map_vertex_vector");
  add_generic_map<C, Sm, Vi, int>(c, "property_map_vertex_int");
  add_generic_map<C, Sm, Vi, FT>(c, "property_map_vertex_FT");

  add_generic_map<C, Sm, Ei, std::string>(c, "property_map_edge_string");
  add_generic_map<C, Sm, Ei, bool>(c, "property_map_edge_bool");
  add_generic_map<C, Sm, Ei, CGAL::IO::Color>(c, "property_map_edge_color");

  // Face property maps
  add_generic_map<C, Sm, Fi, std::string>(c, "property_map_face_string");
  add_generic_map<C, Sm, Fi, double>(c, "property_map_face_double");
  add_generic_map<C, Sm, Fi, Vector_3>(c, "property_map_face_vector");
  add_generic_map<C, Sm, Fi, std::size_t>(c, "property_map_face_size_t");
  add_generic_map<C, Sm, Fi, int>(c, "property_map_face_int");
  add_generic_map<C, Sm, Fi, bool>(c, "property_map_face_bool");
  add_generic_map<C, Sm, Fi, std::uint32_t>(c, "property_map_face_uint32_t");
  add_generic_map<C, Sm, Fi, CGAL::IO::Color>(c, "property_map_face_color");

  // Halfedge property maps
  add_generic_map<C, Sm, Hi, std::string>(c, "property_map_halfedge_string");
  add_generic_map<C, Sm, Hi, std::size_t>(c, "property_map_halfedge_size_t");
  add_generic_map<C, Sm, Hi, py::tuple>(c, "property_map_halfedge_tuple");
  add_generic_map<C, Sm, Hi, bool>(c, "property_map_halfedge_bool");
  add_generic_map<C, Sm, Hi, CGAL::IO::Color>(c, "property_map_halfedge_color");
  
  return c;
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

  // 

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

    using Pcad = CGAL::Polygon_mesh_processing::Principal_curvatures_and_directions<Kernel>;
    py::class_<Sm> sm_c(m, name);
    add_maps<Sm, py::class_<Sm>>(sm_c);
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
      .def("halfedge", [](const Sm& sm, Ei e, unsigned int i) { return sm.halfedge(e, i); },
           py::arg("e"), py::arg("i"))
      .def("halfedge", [](const Sm& sm, Vi source, Vi target) { return sm.halfedge(source, target); },
           py::arg("source"), py::arg("target"))
      .def("degree", [](const Sm& sm, Vi v) { return sm.degree(v); },
           py::arg("v"))
      .def("degree", [](const Sm& sm, Fi f) { return sm.degree(f); },
           py::arg("f"))
      .def("is_border", [](const Sm& sm, Hi h) { return sm.is_border(h); },
           py::arg("h"))
      .def("is_border", [](const Sm& sm, Ei e) { return sm.is_border(e); },
           py::arg("e"))
      .def("is_border", [](const Sm& sm, Vi v, bool check_all_incident_halfedges = true) { return sm.is_border(v, check_all_incident_halfedges); },
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
  using Sm_3 = sm::Surface_mesh_3;
  using Vi = typename Sm_3::Vertex_index;
  using Fi = typename Sm_3::Face_index;
  using Hi = typename Sm_3::Halfedge_index;
  using Ei = typename Sm_3::Edge_index;
  using Pnt = Point_3;
  using Pcad = CGAL::Polygon_mesh_processing::Principal_curvatures_and_directions<Kernel>;
  constexpr auto ri(py::rv_policy::reference_internal);

  export_surface_mesh_impl<Sm_3>(m, "Surface_mesh_3");

  define_generate_functions<py::module_, Sm_3, Kernel>(m);

  internal::export_property_map<Sm_3, Vi, Pnt>(m, "Vertex_point_map"); //this is the Pm::Property_map
  // sm::vertex_map<Sm_3, Pnt>(m, "vertex_point_boost_map", "Vertex_point_boost_map"); //this is the boost::property_map
  using vbmap = typename Sm_3::template Property_map<Vi, bool>;
  internal::export_property_map_bool<Sm_3, Vi>(m, "Vertex_bool_map");
  // sm::vertex_bool_map<Sm_3, bool>(m, "vertex_bool_boost_map", "Vertex_bool_boost_map");
  internal::export_property_map<Sm_3, Vi, CGAL::IO::Color>(m, "Vertex_color_map");
  internal::export_property_map<Sm_3, Vi, std::size_t>(m, "Vertex_size_t_map");
  // sm::vertex_map<Sm_3, std::size_t>(m, "vertex_size_t_boost_map", "Vertex_size_t_boost_map");
  internal::export_property_map<Sm_3, Vi, Vector_3>(m, "Vertex_vector_map");
  // sm::vertex_map<Sm_3, Vector_3>(m, "vertex_vector_boost_map", "Vertex_vector_boost_map");
  internal::export_property_map<Sm_3, Vi, int>(m, "Vertex_int_map");
  // sm::vertex_map<Sm_3, int>(m, "vertex_int_boost_map", "Vertex_int_boost_map");
  internal::export_property_map<Sm_3, Vi, FT>(m, "Vertex_FT_map");
  // sm::vertex_map<Sm_3, FT>(m, "vertex_FT_boost_map", "Vertex_FT_boost_map");
  internal::export_property_map<Sm_3, Vi, Pcad>(m, "Vertex_Principal_curvatures_and_directions_map");
  // sm::vertex_map<Sm_3, Pcad>(m, "vertex_Principal_curvatures_and_directions_boost_map", "Vertex_Principal_curvatures_and_directions_boost_map");
  internal::export_property_map<Sm_3, Vi, py::set>(m, "Vertex_set_map");
  // sm::vertex_map<Sm_3, py::set>(m, "vertex_set_boost_map", "Vertex_set_boost_map");
  internal::export_property_map<Sm_3, Vi, std::string>(m, "Vertex_string_map");

  using ebmap_type = typename Sm_3::template Property_map<Ei, bool>; // different for bools because it would return std::_Bit_reference
  internal::export_property_map_bool<Sm_3, Ei>(m, "Edge_bool_map");
  // sm::edge_bool_map<Sm_3, bool>(m, "edge_bool_boost_map", "Edge_bool_boost_map");
  // sm::register_map<ebmap_type>(m, "Edge_bool_map");
  internal::export_property_map<Sm_3, Ei, std::string>(m, "Edge_string_map");
  internal::export_property_map<Sm_3, Ei, CGAL::IO::Color>(m, "Edge_color_map");


  internal::export_property_map<Sm_3, Fi, std::string>(m, "Face_string_map");
  internal::export_property_map<Sm_3, Fi, double>(m, "Face_double_map");
  // sm::face_map<Sm_3, double>(m, "face_double_boost_map", "Face_double_boost_map");
  internal::export_property_map<Sm_3, Fi, Vector_3>(m, "Face_vector_map");
  // sm::face_map<Sm_3, Vector_3>(m, "face_vector_boost_map", "Face_vector_boost_map");
  internal::export_property_map<Sm_3, Fi, std::size_t>(m, "Face_size_t_map");
  // sm::face_map<Sm_3, std::size_t>(m, "face_size_t_boost_map", "Face_size_t_boost_map");
  internal::export_property_map<Sm_3, Fi, int>(m, "Face_int_map");
  // sm::face_map<Sm_3, int>(m, "face_int_boost_map", "Face_int_boost_map");
  using fbmap = typename Sm_3::template Property_map<Fi, bool>;
  internal::export_property_map_bool<Sm_3, Fi>(m, "Face_bool_map");
  // sm::face_bool_map<Sm_3, bool>(m, "face_bool_boost_map", "Face_bool_boost_map");
  internal::export_property_map<Sm_3, Fi, CGAL::IO::Color>(m, "Face_color_map");
  internal::export_property_map<Sm_3, Fi, std::uint32_t>(m, "Face_uint32_t_map");
  // sm::face_map<Sm_3, std::uint32_t>(m, "face_uint32_t_boost_map", "Face_uint32_t_boost_map");
  

  internal::export_property_map<Sm_3, Hi, CGAL::IO::Color>(m, "Halfedge_color_map");
  internal::export_property_map<Sm_3, Hi, std::string>(m, "Halfedge_string_map");
  internal::export_property_map<Sm_3, Hi, std::size_t>(m, "Halfedge_size_t_map");
  // sm::halfedge_map<Sm_3, std::size_t>(m, "halfedge_size_t_boost_map", "Halfedge_size_t_boost_map");
  internal::export_property_map<Sm_3, Hi, py::tuple>(m, "Halfedge_tuple_map");
  // sm::halfedge_map<Sm_3, py::tuple>(m, "halfedge_tuple_boost_map", "Halfedge_tuple_boost_map");
  using hbmap = typename Sm_3::template Property_map<Hi, bool>;
  internal::export_property_map_bool<Sm_3, Hi>(m, "Halfedge_bool_map");
  // sm::halfedge_bool_map<Sm_3, bool>(m, "halfedge_bool_boost_map", "Halfedge_bool_boost_map");

  // internal::export_property_map<Sm_3, Pnt, Pnt>(m, "Point_point_map"); // this is a dict not a map??

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
  // a property map filled by this function and that will contain for each region the plane (or only its orthognonal vector) estimated that approximates it.
  // Type: a class model of WritablePropertyMap with the value type of RegionMap as key and GeomTraits::Plane_3 or GeomTraits::Vector_3 as value type, GeomTraits being the type of the parameter geom_traits
  // Default: None

  
  py::class_<boost::vector_property_map<Vector_3>>(m, "Vector_vector_3_map")
    .def(py::init<>())
    ;

  // void set_selected_faces(const FacePatchIDRange& selected_face_patch_ids,
  //                         FacePatchIDMap face_patch_id_map
  py::class_<CGAL::Face_filtered_graph<Sm_3>>(m, "Face_filtered_graph")
    .def(py::init<const Sm_3&, std::size_t, const Sm_3::Property_map<Fi, std::size_t>&>())
    .def("graph", [](const CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.graph(); })
    .def("reset_indices", [](CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.reset_indices(); })
    .def("number_of_faces", [](const CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.number_of_faces(); })
    .def("invert_selection", [](CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.invert_selection(); })
    // .def("get_face_index_map", [](const CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.get_face_index_map(); }) // commented for stubs
    .def("number_of_vertices", [](const CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.number_of_vertices(); })
    .def("number_of_halfedges", [](const CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.number_of_halfedges(); })
    // .def("get_vertex_index_map", [](const CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.get_vertex_index_map(); })
    // .def("get_halfedge_index_map", [](const CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.get_halfedge_index_map(); })
    .def("initialize_face_indices", [](CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.initialize_face_indices(); })
    .def("initialize_vertex_indices", [](CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.initialize_vertex_indices(); })
    .def("initialize_halfedge_indices", [](CGAL::Face_filtered_graph<Sm_3>& ffg) { return ffg.initialize_halfedge_indices(); })
    .def("set_selected_faces", [](CGAL::Face_filtered_graph<Sm_3>& ffg, const py::list& lst, const Sm_3::Property_map<Fi, std::size_t>& fccmap) {
        auto vec = pmp::list2vec<std::size_t>(lst);
        return ffg.set_selected_faces(vec, fccmap);
      })
    ;

  m.def("clear", &CGAL::clear<Sm_3>);
  m.def("is_closed", &CGAL::is_closed<Sm_3>);

  m.def("Halfedge", &sm::halfedge<Sm_3>);

  m.def("null_face", &sm::null_face<Sm_3>);
  m.def("read_polygon_mesh", &sm::read_polygon_mesh<Sm_3>,
        py::arg("fname"), py::arg("parameters") = py::dict());
  m.def("expand_face_selection", &sm::expand_face_selection<Fi, Sm_3, Sm_3::Property_map<Fi, int>>);
  m.def("read_polygon_soup", &sm::read_polygon_soup<Sm_3>,
        py::arg("fname"), py::arg("np") = py::dict());
  m.def("make_tetrahedron", &sm::make_tetrahedron<Sm_3>);
  m.def("write_polygon_mesh", &sm::write_polygon_mesh<Sm_3>,
        py::arg("fname"), py::arg("pm"), py::arg("parameters") = py::dict());
  m.def("halfedge", &sm::halfedge<Sm_3>);
  m.def("is_triangle", &sm::is_triangle<Sm_3>);
  m.def("is_triangle_mesh", &CGAL::is_triangle_mesh<Sm_3>);

  m.def("null_vertex", &Sm_3::null_vertex);
  m.def("null_halfedge", &Sm_3::null_halfedge);

  // boost
  m.def("num_vertices", &boost_utils::num_vertices<Sm_3>);
  m.def("num_edges", &boost_utils::num_edges<Sm_3>);
  m.def("degree", &boost_utils::degree_v<Sm_3>);
  m.def("degree", &boost_utils::degree_f<Sm_3>);
  m.def("out_degree", &boost_utils::out_degree<Sm_3>);
  m.def("in_degree", &boost_utils::in_degree<Sm_3>);
  m.def("source", &boost_utils::source_e<Sm_3>);
  m.def("source", &boost_utils::source_h<Sm_3>);
  m.def("target", &boost_utils::target_e<Sm_3>);
  m.def("target", &boost_utils::target_h<Sm_3>);
  m.def("vertices", &boost_utils::my_vertices<Sm_3>);
  m.def("edges", &boost_utils::my_edges<Sm_3>);
  m.def("in_edges", &boost_utils::my_in_edges<Sm_3>);
  m.def("out_edges", &boost_utils::my_out_edges<Sm_3>);
  m.def("adjacent_vertices", &boost_utils::my_adjacent_vertices<Sm_3>);
  m.def("edge", &boost_utils::edge<Sm_3>);
  m.def("next", &boost_utils::next<Sm_3>);
  m.def("prev", &boost_utils::prev<Sm_3>);
  m.def("opposite", &boost_utils::opposite<Sm_3>);
  m.def("edge", &boost_utils::edge_h<Sm_3>);
  m.def("halfedge", &boost_utils::halfedge_e<Sm_3>);
  m.def("halfedge", &boost_utils::halfedge_v<Sm_3>);
  m.def("halfedge", &boost_utils::halfedge_vv<Sm_3>);
  m.def("halfedges", &boost_utils::my_halfedges<Sm_3>);
  m.def("num_halfedges", &boost_utils::num_halfedges<Sm_3>);
  m.def("set_next", &boost_utils::set_next<Sm_3>);
  m.def("set_target", &boost_utils::set_target<Sm_3>);
  m.def("set_halfedge", &boost_utils::set_halfedge_vh<Sm_3>);
  m.def("collect_garbage", &boost_utils::my_collect_garbage<Sm_3>);
  m.def("add_edge", &boost_utils::add_edge<Sm_3>);
  // m.def("halfedge", &boost_utils::halfedge_f<Sm_3>); // this one is already registered
  m.def("face", &boost_utils::face_h<Sm_3>);
  m.def("set_face", &boost_utils::set_face<Sm_3>);
  m.def("set_halfedge", &boost_utils::set_halfedge_fh<Sm_3>);
  m.def("num_faces", &boost_utils::num_faces<Sm_3>);
  m.def("faces", &boost_utils::my_faces<Sm_3>);
  m.def("faces", &boost_utils::my_faces<CGAL::Face_filtered_graph<Sm_3>>);
  m.def("add_vertex", &boost_utils::add_vertex<Sm_3>);
  m.def("add_vertex", &boost_utils::add_vertex_p<Sm_3>);
  m.def("reserve", &boost_utils::reserve<Sm_3>);
  m.def("remove_vertex", &boost_utils::remove_vertex<Sm_3>);
  // m.def("remove_edge", &boost_utils::remove_edge_vv<Sm_3>); // vv only for sm
  m.def("remove_edge", &boost_utils::remove_edge_e<Sm_3>);
  m.def("remove_face", &boost_utils::remove_face<Sm_3>);
  m.def("remove_all_elements", &boost_utils::remove_all_elements<Sm_3>);
  m.def("add_face", &boost_utils::add_face<Sm_3>);
  // m.def("normalize_border", &boost_utils::normalize_border<Sm_3>); ???
  m.def("is_valid_vertex_descriptor", &boost_utils::my_is_valid_vertex_descriptor<Sm_3>,
        py::arg("v"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_halfedge_descriptor", &boost_utils::my_is_valid_halfedge_descriptor<Sm_3>,
        py::arg("h"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_edge_descriptor", &boost_utils::my_is_valid_edge_descriptor<Sm_3>,
        py::arg("e"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_face_descriptor", &boost_utils::my_is_valid_face_descriptor<Sm_3>,
        py::arg("f"), py::arg("g"), py::arg("verbose") = false);

  // helpers from helpers.h

  m.def("is_border", &boost_utils::my_is_border_h<Sm_3>,
        py::arg("hd"), py::arg("g"));
  m.def("is_border_edge", &boost_utils::my_is_border_edge<Sm_3>,
        py::arg("vd"), py::arg("g"));
  m.def("is_border", &boost_utils::my_is_border_e<Sm_3>,
        py::arg("ed"), py::arg("g"));
  m.def("is_border", &boost_utils::my_is_border_v<Sm_3>,
        py::arg("vd"), py::arg("g"));


  // iterators
  m.def("halfedges_around_face", &boost_utils::my_halfedges_around_face<Sm_3>);
  m.def("halfedges_around_target", &boost_utils::my_halfedges_around_target<Sm_3>);

}
