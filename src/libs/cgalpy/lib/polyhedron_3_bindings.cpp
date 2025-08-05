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

#include <CGAL/boost/graph/properties.h>
#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_polyhedron.h>
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/bgl_global.hpp"
#include "CGALPY/export_circulator.hpp"
#include "CGALPY/generator_functions.hpp"
#include "CGALPY/get.hpp"
#include "CGALPY/Internal_face_plane_3_map.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Kernel/export_point_3.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/parse_named_parameters.hpp"
#include "CGALPY/polyhedron_3_types.hpp"

extern void export_polyhedron_traits_with_normals_3(py::module_& m);
extern void export_polyhedron_halfedge_ds(py::module_& m);
extern void export_polyhedron_incremental_builder_3(py::module_& m);
extern void export_polyhedron_builder(py::module_& m);
extern void export_pol3_vertex(py::class_<pol3::Polyhedron_3>& prn_c);
extern void export_pol3_halfedge(py::class_<pol3::Polyhedron_3>& prn_c);
extern void export_pol3_face(py::class_<pol3::Polyhedron_3>& prn_c);

namespace py = nanobind;

namespace pol3 {

//!
template <typename MapType>
void register_map(py::module_& m, const std::string& map_name) {
  using Mt = MapType;
  if (add_attr<Mt>(m, map_name.c_str())) return;

  py::class_<Mt>(m, map_name.c_str())
    .def(py::init<>())
    .def("clear", &Mt::clear)
    .def("default_value", &Mt::default_value)
    .def("put",
         [](Mt& p, const typename Mt::key_type& k,
            const typename Mt::value_type& v)
         { put(p, k, v); },
         py::arg("key"), py::arg("value"))
    .def("__setitem__",
         [](Mt& p, const typename Mt::key_type& k,
            const typename Mt::value_type& v) { put(p, k, v); },
         py::arg("key"), py::arg("value"))
    .def("get",
         [](const Mt& p, const typename Mt::key_type& k) { return get(p, k); })
    .def("__getitem__",
         [](const Mt& p, const typename Mt::key_type& k) { return get(p, k); })
    .def_ro("map_", &Mt::map_)
    .def_ro("default_value_", &Mt::default_value_)
    ;
}

//!
template <typename Dp, typename Mesh>
void register_map_get(py::module_& m, const std::string& prop_name) {
  if (! add_attr<Dp>(m, prop_name.c_str())) {
    py::class_<Dp>(m, prop_name.c_str())
      .def(py::init<>())
      ;
  }
  m.def("get", &internal::get<Dp, Mesh>,
        py::arg("property_map"), py::arg("sm"));
}

//!
template <typename Pm, typename P>
void edge_map(py::module_& m, const std::string& map_name,
              const std::string& prop_name) {
  using Halfedge = typename Pm::Halfedge;
  using Ed = typename boost::graph_traits<Pm>::edge_descriptor;
  using dp = CGAL::dynamic_edge_property_t<P>;
  using Mt = typename boost::property_map<Pm, dp>::const_type;
  register_map<Mt>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const Mt& p, Halfedge& e) { return get(p, Ed(&e)); },
        py::arg("property_map"), py::arg("edge"));
}

//!
template <typename PolygonMesh, typename P>
void face_map(py::module_& m,
              const std::string& map_name, const std::string& prop_name) {
  using Pm = PolygonMesh;
  using Face = typename Pm::Face;
  using Fd = typename boost::graph_traits<Pm>::face_descriptor;
  using dp = CGAL::dynamic_face_property_t<P>;
  using Mt = typename boost::property_map<Pm, dp>::const_type;
  register_map<Mt>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const Mt& p, Face& f) { return get(p, Fd(&f)); },
        py::arg("property_map"), py::arg("face"));
}

//!
template <typename PolygonMesh, typename P>
void vertex_map(py::module_& m,
                const std::string& map_name, const std::string& prop_name) {
  using Pm = PolygonMesh;
  using Vertex = typename Pm::Vertex;
  using Vd = typename boost::graph_traits<Pm>::vertex_descriptor;
  using dp = CGAL::dynamic_vertex_property_t<P>;
  using Mt = typename boost::property_map<Pm, dp>::const_type;
  register_map<Mt>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const Mt& pm, Vertex& v) { return get(pm, Vd(&v)); },
        py::arg("property_map"), py::arg("vertex"));
}

//!
template <typename PolygonMesh, typename P>
void halfedge_map(py::module_& m,
                  const std::string& map_name, const std::string& prop_name) {
  using Pm = PolygonMesh;
  using Halfedge = typename Pm::Halfedge;
  using Hd = typename boost::graph_traits<Pm>::halfedge_descriptor;
  using dp = CGAL::dynamic_halfedge_property_t<P>;
  using Mt = typename boost::property_map<Pm, dp>::const_type;
  register_map<Mt>(m, map_name);
  register_map_get<dp, Pm>(m, prop_name);
  m.def("get", [](const Mt& p, Halfedge& h) { return get(p, Hd(&h)); },
        py::arg("property_map"), py::arg("halfedge"));
}

// Global access functions

//!
template <typename PolygonMesh>
auto my_edges(const PolygonMesh& p) {
  constexpr auto ri(py::rv_policy::reference_internal);
  using Pm = PolygonMesh;
  using Ei = typename boost::graph_traits<Pm>::edge_iterator;
  auto range = CGAL::edges(p);
  return py::make_iterator<ri>(py::type<Ei>(), "Face_iterator", range.first, range.second);
}

//!
template <typename PolygonMesh>
auto my_faces(const PolygonMesh& p) {
  constexpr auto ri(py::rv_policy::reference_internal);
  using Pm = PolygonMesh;
  using Fi = typename boost::graph_traits<Pm>::face_iterator;
  auto range = CGAL::faces(p);
  return py::make_iterator<ri>(py::type<Fi>(), "Face_iterator", range.first, range.second);
}

//!
template <typename PolygonMesh>
auto my_halfedges(const PolygonMesh& p) {
  constexpr auto ri(py::rv_policy::reference_internal);
  using Pm = PolygonMesh;
  using Hi = typename boost::graph_traits<Pm>::halfedge_iterator;
  auto range = CGAL::halfedges(p);
  return py::make_iterator<ri>(py::type<Hi>(), "Face_iterator", range.first, range.second);
}

//!
template <typename PolygonMesh>
auto my_vertices(const PolygonMesh& p) {
  constexpr auto ri(py::rv_policy::reference_internal);
  using Pm = PolygonMesh;
  using Vi = typename boost::graph_traits<Pm>::vertex_iterator;
  auto range = CGAL::vertices(p);
  return py::make_iterator<ri>(py::type<Vi>(), "Face_iterator", range.first, range.second);
}

//!
boost::graph_traits<Polyhedron_3>::degree_size_type
degree_f(Face& f, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Fd = typename boost::graph_traits<Prn>::face_descriptor;
  return CGAL::degree(Fd(&f), p);
}

// Handle dependant

//!
boost::graph_traits<Polyhedron_3>::degree_size_type
degree_v(Vertex& v, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Vd = typename boost::graph_traits<Prn>::vertex_descriptor;
  return CGAL::degree(Vd(&v), p);
}

//!
const Face& face_h(Halfedge& h, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Hd = typename boost::graph_traits<Prn>::halfedge_descriptor;
  return *(CGAL::face(Hd(&h), p));
}

//!
const Halfedge& halfedge_v(Vertex& v, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Vd = typename boost::graph_traits<Prn>::vertex_descriptor;
  return *(CGAL::halfedge(Vd(&v), p));
}

//!
const Halfedge& halfedge_f(Face& f, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Fd = typename boost::graph_traits<Prn>::face_descriptor;
  return *(CGAL::halfedge(Fd(&f), p));
}

//!
const Halfedge& opposite_h(Halfedge& h, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Hd = typename boost::graph_traits<Prn>::halfedge_descriptor;
  return *(CGAL::opposite(Hd(&h), p));
}

//!
const Halfedge& prev_h(Halfedge& h, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Hd = typename boost::graph_traits<Prn>::halfedge_descriptor;
  return *(CGAL::prev(Hd(&h), p));
}

//!
const Halfedge& next_h(Halfedge& h, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Hd = typename boost::graph_traits<Prn>::halfedge_descriptor;
  return *(CGAL::next(Hd(&h), p));
}

//!
const Vertex& source_h(Halfedge& h, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Hd = typename boost::graph_traits<Prn>::halfedge_descriptor;
  return *(CGAL::source(Hd(&h), p));
}

//!
const Vertex& target_h(Halfedge& h, const Polyhedron_3& p) {
  using Prn = Polyhedron_3;
  using Hd = typename boost::graph_traits<Prn>::halfedge_descriptor;
  return *(CGAL::target(Hd(&h), p));
}

//!
Halfedge& make_tetrahedron1(Polyhedron_3& prn,
                            const Point_3& p1, const Point_3& p2,
                            const Point_3& p3, const Point_3& p4)
{ return *(prn.make_tetrahedron(p1, p2, p3, p4)); }

//!
Halfedge& make_tetrahedron2(Polyhedron_3& prn)
{ return *(prn.make_tetrahedron()); }

//!
bool is_tetrahedron(const Polyhedron_3& prn, const Halfedge& h)
{ return prn.is_tetrahedron(Halfedge_const_handle(&h)); }

//!
auto make_triangle_empty(Polyhedron_3& prn)
{ return prn.make_triangle(); }

//!
auto make_triangle(Polyhedron_3& prn, const Point_3& p1,
                   const Point_3& p2, const Point_3& p3)
{ return prn.make_triangle(p1, p2, p3); }

/// \name Internal Iterators
/// @{

//
auto polyhedron_vertices(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Vertex_const_iterator>(),
                               "Vertex_iterator",
                               prn.vertices_begin(), prn.vertices_end());
}

//!
auto polyhedron_halfedges(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Halfedge_const_iterator>(),
                               "Halfedge_iterator",
                               prn.halfedges_begin(), prn.halfedges_end());
}

//!
auto polyhedron_edges(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Edge_const_iterator>(),
                               "Edge_iterator",
                               prn.edges_begin(), prn.edges_end());
}

//!
auto polyhedron_faces(const Polyhedron_3& prn) {
  constexpr auto ri(py::rv_policy::reference_internal);
  return py::make_iterator<ri>(py::type<Polyhedron_3::Face_const_iterator>(),
                               "Face_iterator",
                               prn.facets_begin(), prn.facets_end());
}

//!
auto polyhedron_points(const Polyhedron_3& prn) {
  using Pnt_cit = Polyhedron_3::Point_const_iterator;
  return make_iterator<Pnt_cit, Pnt_cit>(prn.points_begin(), prn.points_end());
}

//!
auto polyhedron_planes(const Polyhedron_3& prn) {
  return make_iterator(prn.planes_begin(), prn.planes_end());
}

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

/*! \todo rename to something like handle_get and handle_put
 */
template <typename PropertyMap>
typename PropertyMap::value_type
face_get(const PropertyMap& pm,
         typename PropertyMap::key_type::value_type & f) {
  using Facet_handle = typename PropertyMap::key_type;
  const auto& v = get(pm, Facet_handle(&f));
  return get(pm, Facet_handle(&f));
}

/*!
 */
template <typename PropertyMap>
void face_put(const PropertyMap& pm,
              typename PropertyMap::key_type::value_type & f,
              const typename PropertyMap::value_type& val) {
  using Facet_handle = typename PropertyMap::key_type;
  put(pm, Facet_handle(&f), val);
}

} // namespace pol3

/*! export Internal_face_plane_3_map
 */
void export_internal_face_plane_3_map(py::module_& m) {
  using Prn = pol3::Polyhedron_3;
  using Ifpm = pol3::Internal_face_plane_3_map<Prn>;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Ifpm>(m, "Internal_face_plane_3_map")) return;

  py::class_<Ifpm>(m, "Internal_face_plane_3_map")
    .def(py::init<>())
    .def("get", &pol3::face_get<Ifpm>, ri)
    .def("put", &pol3::face_put<Ifpm>, ri)
    ;
}

// Export Polyhedron_3.
void export_polyhedron_3(py::module_& m) {
  using Prn = pol3::Polyhedron_3;
  using Pnt = Prn::Point_3;
  using Vertex = Prn::Vertex;
  using Halfedge = Prn::Halfedge;
  using Face = Prn::Face;
  using Pln = Face::Plane_3;
  using Gt = boost::graph_traits<Prn>;
  using Vd = Gt::vertex_descriptor;
  using Ed = Gt::edge_descriptor;
  using Hd = Gt::halfedge_descriptor;
  using Fd = Gt::face_descriptor;

  constexpr auto ri(py::rv_policy::reference_internal);
  constexpr auto ref(py::rv_policy::reference);

#if CGALPY_POL3_GEOMETRY_TRAITS == CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS
  export_polyhedron_traits_with_normals_3(m);
#endif
  export_polyhedron_halfedge_ds(m);
  export_polyhedron_incremental_builder_3(m);
  export_polyhedron_builder(m);
  export_internal_face_plane_3_map(m);

  // define_generate_functions<py::module_, Prn, Kernel>(m); // doesn't work for polyhedron

  if (! add_attr<Prn>(m, "Polyhedron_3")) {
    py::class_<Prn> prn_c(m, "Polyhedron_3");
    prn_c.def(py::init<>())
      .def(py::init<const Prn&>())
      .def(py::init<const pol3::Traits&>())
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
      .def("is_triangle", &Prn::is_triangle)
      .def("is_valid", &Prn::is_valid)
      .def("join_facet", &Prn::join_facet)
      .def("join_loop", &Prn::join_loop)
      .def("join_vertex", &Prn::join_vertex)
      .def("keep_largest_connected_components",
           &Prn::keep_largest_connected_components)
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
      .def("delegate", &Prn::delegate)
      .def("is_pure_quad", py::overload_cast<>(&Prn::is_pure_quad, py::const_))
      .def("is_pure_bivalent",
           py::overload_cast<>(&Prn::is_pure_bivalent, py::const_))
      .def("is_pure_trivalent",
           py::overload_cast<>(&Prn::is_pure_trivalent, py::const_))
      .def("is_pure_triangle",
           py::overload_cast<>(&Prn::is_pure_triangle, py::const_))
      ;

    using Vci = Prn::Vertex_const_iterator;
    using Hci = Prn::Halfedge_const_iterator;
    using Eci = Prn::Edge_const_iterator;
    using Fci = Prn::Face_const_iterator;
    using Pnt_ci = Prn::Point_const_iterator;
    using Pln_ci = Prn::Plane_const_iterator;

    add_iterator<Vci, Vci>("Vertex_iterator", prn_c);
    add_iterator<Hci, Hci>("Halfedge_iterator", prn_c);
    add_iterator<Eci, Eci>("Edge_iterator", prn_c);
    add_iterator<Fci, Fci>("Face_iterator", prn_c);
    add_iterator<Pnt_ci, Pnt_ci>("Point_iterator", prn_c);
    add_iterator<Pln_ci, Pln_ci>("Plane_iterator", prn_c);

    prn_c.def("vertices", &pol3::polyhedron_vertices, py::keep_alive<0, 1>())
      .def("halfedges", &pol3::polyhedron_halfedges, py::keep_alive<0, 1>())
      .def("edges", &pol3::polyhedron_edges, py::keep_alive<0, 1>())
      .def("faces", &pol3::polyhedron_faces, py::keep_alive<0, 1>())
      .def("points", &pol3::polyhedron_points, py::keep_alive<0, 1>())
      .def("planes", &pol3::polyhedron_planes, py::keep_alive<0, 1>())
      ;

    export_pol3_vertex(prn_c);
    export_pol3_halfedge(prn_c);
    export_pol3_face(prn_c);

    add_insertion(prn_c, "__str__");
    add_insertion(prn_c, "__repr__");
    add_extraction(prn_c);
  }

  add_attr<Pnt>(m, "Point_3");

#ifdef CGALPY_HAS_VISUAL
  using Draw_arr = void(*)(const Prn&, const char*);
  m.def("draw", [](const Prn& prn, const char* title)
  { CGAL::draw(prn, title); });
#endif

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

  using Pifem = CGAL::Polyhedron_is_feature_edge_pmap;
  if (! add_attr<Pifem>(m, "Polyhedron_is_feature_edge_pmap")) {
    py::class_<Pifem>(m, "Polyhedron_is_feature_edge_pmap")
      .def(py::init<>())
      ;
  }

  //! \todo the following 3 functions should be replaced by something more
  // general that applies to all maps, similar to the register_map().

  //! The vertex->point property map
  using Vpt = CGAL::vertex_point_t;
  using Vpm = boost::property_map<Prn, Vpt>::const_type;
  if (! add_attr<Vpm>(m, "Vertex_point_map")) {
    py::class_<Vpm>(m, "Vertex_point_map")
      .def(py::init<>())
      ;
  }

  //! The get and put functions that operate on the vertex->point property map
  m.def("get", [](const Vpm& pm, Vertex& v) { return get(pm, Vd(&v)); }, ref,
        py::arg("property_map"), py::arg("vertex"));

  // Free functions
  m.def("clear", &CGAL::clear<Prn>);
  m.def("is_triangle_mesh", &CGAL::is_triangle_mesh<Prn>);
  m.def("is_closed", &CGAL::is_closed<Prn>);

  //! \todo export CGAL::vertex_point and CGAL::get() instead.
  m.def("get_vertex_point",
        [](const Prn& pm, Vertex& v)
        { return get(CGAL::vertex_point, pm, Vd(&v)); });

  // CGAL and the Boost Graph Library

  // Iterators
  m.def("edges", &pol3::my_edges<Prn>, py::keep_alive<0, 1>());
  m.def("faces", &pol3::my_faces<Prn>, py::keep_alive<0, 1>());
  m.def("halfedges", &pol3::my_halfedges<Prn>, py::keep_alive<0, 1>());
  m.def("vertices", &pol3::my_vertices<Prn>, py::keep_alive<0, 1>());

  // Functions that do not involve handlers
  m.def("add_edge", &bgl::my_add_edge<Prn>);
  m.def("add_face", &bgl::my_add_face<Prn>);
  m.def("add_vertex", &bgl::my_add_vertex<Prn>);
  m.def("num_edges", &bgl::my_num_edges<Prn>);
  m.def("num_faces", &bgl::my_num_faces<Prn>);
  m.def("num_halfedges", &bgl::my_num_halfedges<Prn>);
  m.def("num_vertices", &bgl::my_num_vertices<Prn>);
  m.def("remove_all_elements", &bgl::my_remove_all_elements<Prn>);
  m.def("reserve", &bgl::my_reserve<Prn>);

  //! Obtain the propery maps
  m.def("get", &bgl::get<CGAL::vertex_point_t, Prn>, ri);

  // Other
  // m.def("add_vertex", &pol3::add_vertex_p);
  // m.def("adjacent_vertices", &pol3::adjacent_vertices);
  m.def("degree", &pol3::degree_f);
  m.def("degree", &pol3::degree_v);
  // m.def("edge", &pol3::edge);
  m.def("face", &pol3::face_h, ref);
  m.def("halfedge", &pol3::halfedge_v, ref);
  m.def("halfedge", &pol3::halfedge_f, ref);
  // m.def("halfedge", &pol3::halfedge_vv);
  // m.def("in_degree", &pol3::in_degree);
  // m.def("is_valid_vertex_descriptor",
  //       &bgl::my_is_valid_vertex_descriptor<Prn>,
  //       py::arg("v"), py::arg("g"), py::arg("verbose") = false);
  // m.def("is_valid_halfedge_descriptor",
  //       &bgl::my_is_valid_halfedge_descriptor<Prn>,
  //       py::arg("h"), py::arg("g"), py::arg("verbose") = false);
  // m.def("is_valid_edge_descriptor",
  //       &bgl::my_is_valid_edge_descriptor<Prn>,
  //       py::arg("e"), py::arg("g"), py::arg("verbose") = false);
  // m.def("is_valid_face_descriptor",
  //       &bgl::my_is_valid_face_descriptor<Prn>,
  //       py::arg("f"), py::arg("g"), py::arg("verbose") = false);
  m.def("next", &pol3::next_h, ref);
  // m.def("null_face", &bgl::null_face<Prn>);
  // m.def("null_halfedge", &bgl::null_halfedge<Prn>);
  // m.def("null_vertex", &bgl::null_vertex<Prn>);
  m.def("opposite", &pol3::opposite_h, ref);
  // m.def("out_degree", &bgl::out_degree<Prn>);
  m.def("prev", &pol3::prev_h, ref);
  // m.def("remove_edge", &bgl::remove_edge_vv<Prn>);
  // m.def("remove_edge", &bgl::remove_edge_e<Prn>);
  // m.def("remove_face", &bgl::remove_face<Prn>);
  // m.def("remove_vertex", &bgl::remove_vertex<Prn>);
  m.def("source", &pol3::source_h, ref);
  m.def("target", &pol3::target_h, ref);
  // m.def("set_face", &bgl::set_face<Prn>);
  // m.def("set_halfedge", &bgl::set_halfedge_vh<Prn>);
  // m.def("set_halfedge", &bgl::set_halfedge_fh<Prn>);
  // m.def("set_next", &bgl::set_next<Prn>);
  // m.def("set_target", &bgl::set_target<Prn>);

  // m.def("in_edges", &bgl::my_in_edges<Prn>);
  // m.def("out_edges", &bgl::my_out_edges<Prn>);

  // Generators
  m.def("make_tetrahedron", &bgl::my_make_tetrahedron<Prn>);

  // using Edge_bool_tag = CGAL::dynamic_edge_property_t<bool>;
  // using ebmap_type = boost::property_map<Prn, Edge_bool_tag>::type;

  // using Face_bool_tag = CGAL::dynamic_face_property_t<bool>;
  // using fbmap_type = boost::property_map<Prn, Face_bool_tag>::type;

  // using Vertex_bool_tag = CGAL::dynamic_vertex_property_t<bool>;
  // using vbmap_type = boost::property_map<Prn, Vertex_bool_tag>::type;

  // Euler operations
  // bgl::define_euler_operations<py::module_, Prn, ebmap_type>(m);

  // // Selection Functions
  // bgl::define_boost_selection_functions<py::module_, Prn, ebmap_type, fbmap_type, vbmap_type>(m);

  // Helper Functions
  // bgl::define_boost_helpers<py::module_, Prn, Prn>(m);

  // Generator Functions
  // bgl::define_generate_functions<py::module_, Prn, Kernel>(m);

  // Partitioning Operations
  // using EdgeDoubleMap =
  //   boost::property_map<Prn, CGAL::dynamic_edge_property_t<double>>::type;
  // using VertexVectorDoubleMap =
  //   boost::property_map<Prn, CGAL::dynamic_vertex_property_t<std::vector<double>>>::type;
  // using VertexSizeTMap =
  //   boost::property_map<Prn, CGAL::dynamic_vertex_property_t<std::size_t>>::type;
  // bgl::define_boost_partitioning_operations<py::module_, Prn, EdgeDoubleMap, VertexVectorDoubleMap, VertexSizeTMap>(m);

}
