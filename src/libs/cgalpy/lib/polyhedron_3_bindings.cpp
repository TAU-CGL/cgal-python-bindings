// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <cstddef>
#include <stdexcept>

#define CGAL_USE_BASIC_VIEWER

#include <boost/math/constants/constants.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/make_iterator.h>

#include <CGAL/IO/polygon_soup_io.h>

//! \todo move to polygon_mesh_processing_bindings.cpp because it depends on Eigen
#ifdef CGALPY_POLYGON_MESH_PROCESSING_BINDINGS
#include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h> // needed for a type
#endif
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

#include <CGAL/boost/graph/properties.h>
#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_polyhedron.h>
#endif

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Kernel/export_point_3.hpp"
#include "CGALPY/polyhedron_3_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/make_circulator.hpp"
#include "CGALPY/export_boost_mesh_utils.hpp"
#include "CGALPY/export_mesh_iterators.hpp"
#include "CGALPY/get.hpp"
#include "CGALPY/parse_named_parameters.hpp"
#include "CGALPY/export_mesh_selection_functions.hpp"
#include "CGALPY/export_mesh_helpers.hpp"
#include "CGALPY/generator_functions.hpp"
#include "CGALPY/export_mesh_partitioning_operations.hpp"

namespace py = nanobind;

namespace pol3 {

template <typename Map_type>
void register_map(py::module_& m, const std::string& map_name) {
  py::class_<Map_type>(m, map_name.c_str())
    .def(py::init<>())
    .def("clear", &Map_type::clear)
    .def("default_value", &Map_type::default_value)
    .def("put", [](Map_type& p, const typename Map_type::key_type& k,
                   const typename Map_type::value_type& v) { put(p, k, v); },
         py::arg("key"), py::arg("value"))
    .def("__setitem__", [](Map_type& p, const typename Map_type::key_type& k,
                          const typename Map_type::value_type& v) { put(p, k, v); },
         py::arg("key"), py::arg("value"))
    .def("get", [](const Map_type& p, const typename Map_type::key_type& k) {
        return get(p, k);
      })
    .def("__getitem__", [](const Map_type& p, const typename Map_type::key_type& k) {
        return get(p, k);
      })
    .def_ro("map_", &Map_type::map_)
    .def_ro("default_value_", &Map_type::default_value_)
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
void halfedge_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
  using Hd = typename boost::graph_traits<Pm>::halfedge_descriptor;
  using dp = CGAL::dynamic_halfedge_property_t<P>;
  using map_type = typename boost::property_map<Pm, dp>::type;
  register_map<map_type>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const map_type& p, const Hd& h) { return get(p, h); },
        py::arg("property_map"), py::arg("halfedge_descriptor"));
}

// Access functions
const Vertex& vertex(const Halfedge& e) { return (*(e.vertex())); }
const Halfedge& opposite(const Halfedge& e) { return (*(e.opposite())); }
const Halfedge& next(const Halfedge& e) { return (*(e.next())); }
const Halfedge& prev(const Halfedge& e) { return (*(e.prev())); }
const Halfedge& halfedge(const Face& f) { return (*(f.halfedge())); }

// Read a surface mesh from a file.
template <typename Polyhedron_3>
Polyhedron_3 read_polygon_mesh(const std::string& filename,
                               const py::dict& np = py::dict()) {
  using Prn = Polyhedron_3;
  Prn pol;
  if (! CGAL::IO::read_polygon_mesh(filename, pol,
                                    internal::parse_named_parameters(np)))
    throw std::runtime_error("Cannot read file!");
  return pol;
}

// Write a surface mesh to a file.
template <typename Polyhedron_3>
bool write_polygon_mesh(std::string fname, const Polyhedron_3& pm,
                        const py::dict& np = py::dict()) {
  return CGAL::IO::write_polygon_mesh(fname, pm,
                                      internal::parse_named_parameters(np));
}

//
Halfedge& make_tetrahedron1(Polyhedron_3& prn,
                            const Point_3& p1, const Point_3& p2,
                            const Point_3& p3, const Point_3& p4)
{ return *(prn.make_tetrahedron(p1, p2, p3, p4)); }

//
Halfedge& make_tetrahedron2(Polyhedron_3& prn)
{ return *(prn.make_tetrahedron()); }

//
bool is_tetrahedron(const Polyhedron_3& prn, const Halfedge& h)
{ return prn.is_tetrahedron(Halfedge_const_handle(&h)); }

auto make_triangle_empty(Polyhedron_3& prn)
{ return prn.make_triangle(); }

auto make_triangle(Polyhedron_3& prn, const typename Polyhedron_3::Point& p1,
                   const typename Polyhedron_3::Point& p2,
                   const typename Polyhedron_3::Point& p3) {
  return prn.make_triangle(p1, p2, p3);
}

/// \name Iterators
/// @{

//
auto my_vertices(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Vertex_const_iterator>(),
                               "Vertex_iterator",
                               prn.vertices_begin(), prn.vertices_end());
}

//
auto my_halfedges(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Halfedge_const_iterator>(),
                               "Halfedge_iterator",
                               prn.halfedges_begin(), prn.halfedges_end());
}

//
auto my_edges(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Edge_const_iterator>(),
                               "Edge_iterator",
                               prn.edges_begin(), prn.edges_end());
}

//
auto my_faces(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Face_const_iterator>(),
                               "Face_iterator",
                               prn.facets_begin(), prn.facets_end());
}

//
auto my_planes(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Plane_const_iterator>(),
                               "Plane_iterator",
                               prn.planes_begin(), prn.planes_end());
}

//
auto halfedges_around_facet(const Face& f)
{ return make_circulator(f.facet_begin()); }

/// @}

// Obtain the null face.
template <typename Pm>
auto null_face()
{ return boost::graph_traits<Pm>::null_face(); }

// pol3::vertex_map<Prn, std::size_t>(m, "vertex_size_t_map", "dynamic_property_vertex_size_t");
template <typename C, typename Pm, typename V>
C register_maps(C& m, const std::string& prop_name) {
  vertex_map<Pm, V>(m, ("vertex_" + prop_name + "_map").c_str(),
                    ("dynamic_property_vertex_" + prop_name).c_str());
  halfedge_map<Pm, V>(m, ("halfedge_" + prop_name + "_map").c_str(),
                      ("dynamic_property_halfedge_" + prop_name).c_str());
  face_map<Pm, V>(m, ("face_" + prop_name + "_map").c_str(),
                  ("dynamic_property_face_" + prop_name).c_str());
  edge_map<Pm, V>(m, ("edge_" + prop_name + "_map").c_str(),
                  ("dynamic_property_edge_" + prop_name).c_str());
  return m;
}

} // namespace pol3

// Export Vertex
template <typename C>
void export_vertex(C& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Vertex = Prn::Vertex;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Vertex>(prn_c, "Vertex")) return;

  py::class_<Vertex> vertex_c(prn_c, "Vertex");
  vertex_c.def(py::init<>())
    .def("point", [](const Vertex& v){ return v.point(); }, ri)
#ifdef CGALPY_POL3_VERTEX_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Vertex. Therefore, we cannot directly refere to any of them, e.g.,
    // `Vertex::set_data`. Instead, we introduce lambda functions that call
    // the appropriate member functions.
    .def("set_data", [](Vertex& v, py::object obj) { v.set_data(obj); })
    .def("data", [](const Vertex& v)->py::object { return v.data(); })
#endif
    ;
}

// Export Vertex
template <typename C>
void export_vertex_handle(C& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Vertex_handle = Prn::Vertex_handle;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Vertex_handle>(prn_c, "Vertex_handle")) return;

  py::class_<Vertex_handle> vertex_handle_c(prn_c, "Vertex_handle");
  vertex_handle_c.def(py::init<>());
}

// Export Vertex
template <typename C>
void export_halfedge(C& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Halfedge = Prn::Halfedge;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Halfedge>(prn_c, "Halfedge")) return;

  py::class_<Halfedge> halfedge_c(prn_c, "Halfedge");
  halfedge_c.def(py::init<>())
    .def("vertex", &pol3::vertex, ri)
    .def("opposite", &pol3::opposite, ri)
    .def("next", &pol3::next, ri)
    .def("prev", &pol3::prev, ri)
    .def("is_border", [](const Halfedge& e){ return e.is_border(); })
#ifdef CGALPY_POL3_HALFEDGE_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Halfedge. Therefore, we cannot directly refere to any of them, e.g.,
    // `Halfedge::set_data`. Instead, we introduce lambda functions that call
    // the appropriate member functions.
    .def("set_data", [](Halfedge& h, py::object obj) { h.set_data(obj); })
    .def("data", [](const Halfedge& h)->py::object { return h.data(); })
#endif
    ;
}

// Export Vertex
template <typename C>
void export_face(C& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Face = Prn::Face;
  using Plane_3 = Prn::Plane_3;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Face>(prn_c, "Face")) return;

  py::class_<Face> face_c(prn_c, "Face");
  face_c.def(py::init<>())
    .def("plane", [](const Face& f){ return f.plane(); }, ri)
    .def("set_plane", [](Face& f, const Plane_3& plane){ f.plane() = plane; })
    .def("halfedge", &pol3::halfedge, ri)
    .def("facet_degree", [](const Face& f){ return f.facet_degree(); })
    .def("is_triangle", [](const Face& f){ return f.is_triangle(); })
    .def("is_quad", [](const Face& f){ return f.is_quad(); })
#ifdef CGALPY_POL3_FACE_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Face. Therefore, we cannot directly refere to any of them, e.g.,
    // `Face::set_data`. Instead, we introduce lambda functions that call
    // the appropriate member functions.
    .def("set_data", [](Face& f, py::object obj) { f.set_data(obj); })
    .def("data", [](const Face& f)->py::object { return f.data(); })
#endif
    ;

  using Hafcc = pol3::Halfedge_around_facet_const_circulator;
  add_circulator<Hafcc>("Halfedge_around_facet_circulator", face_c);
  face_c.def("halfedges", &pol3::halfedges_around_facet, py::keep_alive<0, 1>());

  // Until 'consteval' is supported (C++20), we cannot assume that
  // pol3::face_with_id() is evaluated at compiletime
#if defined(CGALPY_POL3_FACE_WITH_ID)
  if constexpr(pol3::face_with_id())
    face_c.def("id", [](const Face& f){ return f.id(); });
#endif
}

// export Face_handle
template <typename C>
void export_face_handle(C& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Face_handle = Prn::Face_handle;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Face_handle>(prn_c, "Face_handle")) return;

  py::class_<Face_handle> face_handle_c(prn_c, "Face_handle");
  face_handle_c.def(py::init<>());
}

// export boost halfedge
template<typename C>
void export_boost_halfedge(C& prn_c) {
  using PolygonMesh = pol3::Polyhedron_3;
  using Halfedge = boost::graph_traits<PolygonMesh>::halfedge_descriptor;

  if (add_attr<Halfedge>(prn_c, "Halfedge")) return;

  py::class_<Halfedge> halfedge_c(prn_c, "halfedge_descriptor");
  halfedge_c.def(py::init<>())
    .def("facet", [](const Halfedge& e){ return e->facet(); })
    .def("facet_begin", [](const Halfedge& e){ return e->facet_begin(); })
    .def("is_border", [](const Halfedge& e){ return e->is_border(); })
    .def("face", [](const Halfedge& e){ return e->face(); })
    .def("next", [](const Halfedge& e){ return e->next(); })
    .def("prev", [](const Halfedge& e){ return e->prev(); })
    .def("vertex", [](const Halfedge& e){ return e->vertex(); })
    .def("is_quad", [](const Halfedge& e){ return e->is_quad(); })
    .def("opposite", [](const Halfedge& e){ return e->opposite(); })
    .def("is_bivalent", [](const Halfedge& e){ return e->is_bivalent(); })
    .def("is_triangle", [](const Halfedge& e){ return e->is_triangle(); })
    .def("facet_degree", [](const Halfedge& e){ return e->facet_degree(); })
    .def("is_trivalent", [](const Halfedge& e){ return e->is_trivalent(); })
    .def("vertex_begin", [](const Halfedge& e){ return e->vertex_begin(); })
    .def("vertex_degree", [](const Halfedge& e){ return e->vertex_degree(); })
    .def("is_border_edge", [](const Halfedge& e){ return e->is_border_edge(); })
    .def("next_on_vertex", [](const Halfedge& e){ return e->next_on_vertex(); })
    .def("prev_on_vertex", [](const Halfedge& e){ return e->prev_on_vertex(); })
    // .def("prev_link", [](const Halfedge& e){ return e->prev_link(); })
    // .def("next_link", [](const Halfedge& e){ return e->next_link(); })
    ;
}

// export boost vertex
template<typename C>
void export_boost_vertex(C& prn_c) {
  using PolygonMesh = pol3::Polyhedron_3;
  using Vertex = boost::graph_traits<PolygonMesh>::vertex_descriptor;
  using Halfedge = boost::graph_traits<PolygonMesh>::halfedge_descriptor;

  if (add_attr<Vertex>(prn_c, "Vertex")) return;

  py::class_<Vertex> vertex_c(prn_c, "vertex_descriptor");
  vertex_c.def(py::init<>())
    .def("point", [](const Vertex& v){ return v->point(); })
    .def("degree", [](const Vertex& v){ return v->degree(); })
    .def("halfedge", [](const Vertex& v){ return v->halfedge(); })
    .def("is_bivalent", [](const Vertex& v){ return v->is_bivalent(); })
    .def("is_trivalent", [](const Vertex& v){ return v->is_trivalent(); })
    .def("set_halfedge", [](Vertex& v, const Halfedge& h){ v->halfedge() = h; })
    .def("vertex_begin", [](const Vertex& v){ return v->vertex_begin(); })
    // .def_ro("prev_link", [](const Vertex& v){ return v->prev_link(); })
    // .def_ro("next_link", [](const Vertex& v){ return v->next_link(); })
    .def("vertex_degree", [](const Vertex& v){ return v->vertex_degree(); })
    ;
}

// export boost face
template<typename C>
void export_boost_face(C& prn_c) {
  using PolygonMesh = pol3::Polyhedron_3;
  using Face = boost::graph_traits<PolygonMesh>::face_descriptor;
  using Halfedge = boost::graph_traits<PolygonMesh>::halfedge_descriptor;

  if (add_attr<Face>(prn_c, "Face")) return;

  py::class_<Face> face_c(prn_c, "face_descriptor");
  face_c.def(py::init<>())
    .def("facet_begin", [](const Face& f){ return f->facet_begin(); })
    .def("size", [](const Face& f){ return f->size(); })
    .def("plane", [](const Face& f){ return f->plane(); })
    .def("is_quad", [](const Face& f){ return f->is_quad(); })
    .def("halfedge", [](const Face& f){ return f->halfedge(); })
    .def("is_triangle", [](const Face& f){ return f->is_triangle(); })
    .def("facet_degree", [](const Face& f){ return f->facet_degree(); })
    // .def_ro("prev_link", [](const Face& f){ return f->prev_link(); })
    // .def_ro("next_link", [](const Face& f){ return f->next_link(); })
    .def("set_halfedge", [](Face& f, const Halfedge& h){ f->halfedge() = h; })
    ;
}

// export boost edge
template<typename C>
void export_boost_edge(C& prn_c) {
  using PolygonMesh = pol3::Polyhedron_3;
  using Edge = boost::graph_traits<PolygonMesh>::edge_descriptor;

  if (add_attr<Edge>(prn_c, "Edge")) return;

  py::class_<Edge> edge_c(prn_c, "edge_descriptor");
  edge_c.def(py::init<>())
    // this is not working
    // .def("id", [](const Edge& e){ return e->id(); })
    // .def("next", [](const Edge& e){ return e->next(); })
    // .def("prev", [](const Edge& e){ return e->prev(); })
    // .def("halfedge", [](const Edge& e){ return e->halfedge(); })
    // .def("opposite", [](const Edge& e){ return e->opposite(); })
    // .def("next_opposite", [](const Edge& e){ return e->next_opposite(); })
    // .def("opposite_next", [](const Edge& e){ return e->opposite_next(); })
    // .def("opposite_prev", [](const Edge& e){ return e->opposite_prev(); })
    // .def("prev_opposite", [](const Edge& e){ return e->prev_opposite(); })
    ;
}

// Export Polyhedron_3.
void export_polyhedron_3(py::module_& m) {
  using Prn = pol3::Polyhedron_3;
  using Pnt = Prn::Point_3;
  using Vertex = Prn::Vertex;
  using Halfedge = Prn::Halfedge;
  using Face = Prn::Face;
  using Gt = boost::graph_traits<Prn>;
  using vertex_descriptor = Gt::vertex_descriptor;

  constexpr auto ri(py::rv_policy::reference_internal);

  export_vertex(m);
  export_vertex_handle(m);
  export_halfedge(m);
  export_face(m);
  export_face_handle(m);
  export_boost_halfedge(m);
  export_boost_vertex(m);
  export_boost_face(m);
  export_boost_edge(m);

  // define_generate_functions<py::module_, Prn, Kernel>(m); // doesn't work for polyhedron

  if (! add_attr<Prn>(m, "Polyhedron_3")) {
    py::class_<Prn> prn_c(m, "Polyhedron_3");
    prn_c.def(py::init<>())
      .def(py::init<const Prn&>())
      .def("add_facet_to_border", &Prn::add_facet_to_border)
      .def("add_vertex_and_facet_to_border", &Prn::add_vertex_and_facet_to_border)
      .def("bytes", &Prn::bytes)
      .def("bytes_reserved", &Prn::bytes_reserved)
      .def("capacity_of_facets", &Prn::capacity_of_facets)
      .def("capacity_of_halfedges", &Prn::capacity_of_halfedges)
      .def("capacity_of_vertices", &Prn::capacity_of_vertices)
      .def("clear", &Prn::clear)
      .def("create_center_vertex", &Prn::create_center_vertex)
      .def("erase_all", &Prn::erase_all)
      .def("erase_center_vertex", &Prn::erase_center_vertex)
      .def("erase_connected_component", &Prn::erase_connected_component)
      .def("erase_facet", &Prn::erase_facet)
      .def("fill_hole", &Prn::fill_hole)
      .def("flip_edge", &Prn::flip_edge)
      .def("inside_out", &Prn::inside_out)
      .def("is_closed", &Prn::is_closed)
      .def("is_empty", &Prn::is_empty)
      .def("is_tetrahedron", &pol3::is_tetrahedron)
      // .def("is_triangle", &Prn::is_triangle) // this is probably not meant to be used
      .def("is_valid", &Prn::is_valid)
      .def("join_facet", &Prn::join_facet)
      .def("join_loop", &Prn::join_loop)
      .def("join_vertex", &Prn::join_vertex)
      .def("keep_largest_connected_components", &Prn::keep_largest_connected_components)
      .def("make_hole", &Prn::make_hole)
      .def("make_tetrahedron", &pol3::make_tetrahedron1, ri)
      .def("make_tetrahedron", &pol3::make_tetrahedron2, ri)
      .def("normalize_border", &Prn::normalize_border)
      .def("normalized_border_is_valid", &Prn::normalized_border_is_valid)
      .def("size_of_border_edges", &Prn::size_of_border_edges)
      .def("size_of_border_halfedges", &Prn::size_of_border_halfedges)
      .def("size_of_facets", &Prn::size_of_facets)
      .def("size_of_halfedges", &Prn::size_of_halfedges)
      .def("size_of_vertices", &Prn::size_of_vertices)
      .def("split_facet", &Prn::split_facet)
      .def("split_loop", &Prn::split_loop)
      .def("split_vertex", &Prn::split_vertex)
      .def("make_triangle", &pol3::make_triangle_empty)
      .def("make_triangle", &pol3::make_triangle)

      // .def("is_pure_quad", &Prn::is_pure_quad)
      // .def("is_pure_bivalent", &Prn::is_pure_bivalent)
      // .def("is_pure_triangle", &Prn::is_pure_triangle)
      ;

    using Vci = Prn::Vertex_const_iterator;
    using Hci = Prn::Halfedge_const_iterator;
    using Eci = Prn::Edge_const_iterator;
    using Fci = Prn::Face_const_iterator;

    add_iterator<Vci, Vci>("Vertex_iterator", prn_c);
    add_iterator<Hci, Hci>("Halfedge_iterator", prn_c);
    add_iterator<Eci, Eci>("Edge_iterator", prn_c);
    add_iterator<Fci, Fci>("Face_iterator", prn_c);

    prn_c.def("vertices", &pol3::my_vertices, py::keep_alive<0, 1>())
      .def("halfedges", &pol3::my_halfedges, py::keep_alive<0, 1>())
      .def("edges", &pol3::my_edges, py::keep_alive<0, 1>())
      .def("faces", &pol3::my_faces, py::keep_alive<0, 1>())
      .def("planes", &pol3::my_planes, py::keep_alive<0, 1>())
      ;

    add_insertion(prn_c, "__str__");
    add_insertion(prn_c, "__repr__");
    add_extraction(prn_c);

    add_attr<Pnt>(prn_c, "Point_3");
    add_attr<Vertex>(prn_c, "Vertex");
    add_attr<Halfedge>(prn_c, "Halfedge");
    add_attr<Face>(prn_c, "Face");
  }

#ifdef CGALPY_HAS_VISUAL
  using Draw_arr = void(*)(const Prn&, const char*);
  m.def("draw", [](const Prn& prn, const char* title)
  { CGAL::draw(prn, title); });
#endif

  m.def("read_polygon_mesh", &pol3::read_polygon_mesh<Prn>,
        py::arg("filename"), py::arg("np") = py::dict());
  m.def("write_polygon_mesh", &pol3::write_polygon_mesh<Prn>,
        py::arg("filename"), py::arg("pm"), py::arg("np") = py::dict());

  pol3::vertex_map<Prn, CGAL::vertex_incident_patches_t<int>>(m, "vertex_incident_patches_map", "Vertex_incident_patches_map");

  pol3::register_maps<py::module_, Prn, Pnt>(m, "point");
  pol3::register_maps<py::module_, Prn, bool>(m, "bool");
  pol3::register_maps<py::module_, Prn, std::size_t>(m, "size_t");
  pol3::register_maps<py::module_, Prn, FT>(m, "FT");
  pol3::register_maps<py::module_, Prn, Vector_3>(m, "vector_3");
  pol3::register_maps<py::module_, Prn, int>(m, "int");
  pol3::register_maps<py::module_, Prn, CGAL::IO::Color>(m, "color");
  pol3::register_maps<py::module_, Prn, py::tuple>(m, "tuple");
  pol3::register_maps<py::module_, Prn, py::set>(m, "set");
  // pol3::register_maps<py::module_, Prn, std::uint32_t>(m, "uint32_t"); //no

  if constexpr (!std::is_same<double, FT>::value) {
    pol3::register_maps<py::module_, Prn, double>(m, "double"); // shadows FT
  }

//! \todo move to polygon_mesh_processing_bindings.cpp because it depends on Eigen
#ifdef CGALPY_POLYGON_MESH_PROCESSING_BINDINGS
  namespace PMP = CGAL::Polygon_mesh_processing;
  using pcad = PMP::Principal_curvatures_and_directions<Kernel>;
  pol3::vertex_map<Prn, pcad>
    (m, "vertex_principal_curvatures_and_directions_map",
     "dynamic_property_vertex_PC");
#endif

  m.def("get_edge_is_feature_map",
        [](const Prn& sm) { return get(CGAL::edge_is_feature, sm); });

  py::class_<CGAL::Polyhedron_is_feature_edge_pmap>(m, "Polyhedron_is_feature_edge_pmap")
    .def(py::init<>())
    ;

  using poly_items = pol3::Polyhedron_items;
  using poly_items_derived = CGAL::I_Polyhedron_derived_items_3<poly_items>;

  // the type
  using vertex_point_map_type = CGAL::internal::Point_accessor<CGAL::internal::In_place_list_iterator<CGAL::HalfedgeDS_in_place_list_vertex<CGAL::I_Polyhedron_vertex<CGAL::HalfedgeDS_vertex_base<CGAL::HalfedgeDS_list_types<Kernel, poly_items_derived, std::allocator<int> >, std::integral_constant<bool, true>, Point_3 > > >, std::allocator<CGAL::HalfedgeDS_in_place_list_vertex<CGAL::I_Polyhedron_vertex<CGAL::HalfedgeDS_vertex_base<CGAL::HalfedgeDS_list_types<Kernel, poly_items_derived, std::allocator<int> >, std::integral_constant<bool, true>, Point_3 > > > > >, Point_3, Point_3 const&, true>;
  py::class_<vertex_point_map_type>(m, "vertex_point_map")
    .def(py::init<>())
    ;

  m.def("get_vertex_point_map",
        [](const Prn& pm) { return get(CGAL::vertex_point, pm); });
  m.def("get_vertex_point",
        [](const Prn& pm, const vertex_descriptor& vd)
        { return get(CGAL::vertex_point, pm, vd); });

  m.def("is_triangle_mesh", &CGAL::is_triangle_mesh<Prn>);
  m.def("clear", &CGAL::clear<Prn>);
  m.def("is_closed", &CGAL::is_closed<Prn>);
  m.def("null_face", &pol3::null_face<Prn>);

  m.def("num_vertices", &boost_utils::num_vertices<Prn>);
  m.def("num_edges", &boost_utils::num_edges<Prn>);
  m.def("degree", &boost_utils::degree_v<Prn>);
  m.def("degree", &boost_utils::degree_f<Prn>);
  m.def("out_degree", &boost_utils::out_degree<Prn>);
  m.def("in_degree", &boost_utils::in_degree<Prn>);
  m.def("source", &boost_utils::source_e<Prn>);
  m.def("source", &boost_utils::source_h<Prn>);
  m.def("target", &boost_utils::target_e<Prn>);
  m.def("target", &boost_utils::target_h<Prn>);
  m.def("vertices", &boost_utils::my_vertices<Prn>);
  m.def("edges", &boost_utils::my_edges<Prn>);
  m.def("in_edges", &boost_utils::my_in_edges<Prn>);
  m.def("out_edges", &boost_utils::my_out_edges<Prn>);
  m.def("adjacent_vertices", &boost_utils::my_adjacent_vertices<Prn>);
  m.def("edge", &boost_utils::edge<Prn>);
  m.def("next", &boost_utils::next<Prn>);
  m.def("prev", &boost_utils::prev<Prn>);
  m.def("opposite", &boost_utils::opposite<Prn>);
  m.def("edge", &boost_utils::edge_h<Prn>);
  m.def("halfedge", &boost_utils::halfedge_e<Prn>);
  m.def("halfedge", &boost_utils::halfedge_v<Prn>);
  m.def("halfedge", &boost_utils::halfedge_vv<Prn>);
  m.def("halfedges", &boost_utils::my_halfedges<Prn>);
  m.def("num_halfedges", &boost_utils::num_halfedges<Prn>);
  m.def("set_next", &boost_utils::set_next<Prn>);
  m.def("set_target", &boost_utils::set_target<Prn>);
  m.def("set_halfedge", &boost_utils::set_halfedge_vh<Prn>);
  m.def("collect_garbage", &boost_utils::my_collect_garbage<Prn>);
  m.def("add_edge", &boost_utils::add_edge<Prn>);
  m.def("halfedge", &boost_utils::halfedge_f<Prn>);
  m.def("face", &boost_utils::face_h<Prn>);
  m.def("set_face", &boost_utils::set_face<Prn>);
  m.def("set_halfedge", &boost_utils::set_halfedge_fh<Prn>);
  m.def("num_faces", &boost_utils::num_faces<Prn>);
  m.def("faces", &boost_utils::my_faces<Prn>);
  m.def("add_vertex", &boost_utils::add_vertex<Prn>);
  m.def("add_vertex", &boost_utils::add_vertex_p<Prn>);
  m.def("reserve", &boost_utils::reserve<Prn>);
  m.def("remove_vertex", &boost_utils::remove_vertex<Prn>);
  // m.def("remove_edge", &boost_utils::remove_edge_vv<Prn>); // vv only for sm
  m.def("remove_edge", &boost_utils::remove_edge_e<Prn>);
  m.def("remove_face", &boost_utils::remove_face<Prn>);
  m.def("remove_all_elements", &boost_utils::remove_all_elements<Prn>);
  m.def("add_face", &boost_utils::add_face<Prn>);
  // m.def("normalize_border", &boost_utils::normalize_border<Prn>); ???
  m.def("is_valid_vertex_descriptor",
        &boost_utils::my_is_valid_vertex_descriptor<Prn>,
        py::arg("v"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_halfedge_descriptor",
        &boost_utils::my_is_valid_halfedge_descriptor<Prn>,
        py::arg("h"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_edge_descriptor",
        &boost_utils::my_is_valid_edge_descriptor<Prn>,
        py::arg("e"), py::arg("g"), py::arg("verbose") = false);
  m.def("is_valid_face_descriptor",
        &boost_utils::my_is_valid_face_descriptor<Prn>,
        py::arg("f"), py::arg("g"), py::arg("verbose") = false);

  using ebmap_type =
    boost::property_map<Prn, CGAL::dynamic_edge_property_t<bool>>::type;
  using fbmap_type =
    boost::property_map<Prn, CGAL::dynamic_face_property_t<bool>>::type;
  using vbmap_type =
    boost::property_map<Prn, CGAL::dynamic_vertex_property_t<bool>>::type;
  // Euler operations
  boost_utils::define_euler_operations<py::module_, Prn, ebmap_type>(m);

  // Iterators and Circulators
  boost_utils::define_boost_iterators<py::module_, Prn>(m);

  // // Selection Functions
  // boost_utils::define_boost_selection_functions<py::module_, Prn, ebmap_type, fbmap_type, vbmap_type>(m);

  // Helper Functions
  boost_utils::define_boost_helpers<py::module_, Prn, Prn>(m);

  // Generator Functions
  boost_utils::define_generate_functions<py::module_, Prn, Kernel>(m);

  // Partitioning Operations
  using EdgeDoubleMap =
    boost::property_map<Prn, CGAL::dynamic_edge_property_t<double>>::type;
  using VertexVectorDoubleMap =
    boost::property_map<Prn, CGAL::dynamic_vertex_property_t<std::vector<double>>>::type;
  using VertexSizeTMap =
    boost::property_map<Prn, CGAL::dynamic_vertex_property_t<std::size_t>>::type;
  boost_utils::define_boost_partitioning_operations<py::module_, Prn, EdgeDoubleMap, VertexVectorDoubleMap, VertexSizeTMap>(m);
}
