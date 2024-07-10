// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_polyhedron.h>
#endif

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/Kernel/export_point_3.hpp"
#include "CGALPY/polyhedron_3_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/make_circulator.hpp"

namespace py = nanobind;

namespace pol3 {

// Access functions
const Vertex& vertex(const Halfedge& e) { return (*(e.vertex())); }
const Halfedge& opposite(const Halfedge& e) { return (*(e.opposite())); }
const Halfedge& next(const Halfedge& e) { return (*(e.next())); }
const Halfedge& prev(const Halfedge& e) { return (*(e.prev())); }
const Halfedge& halfedge(const Face& f) { return (*(f.halfedge())); }

// Read a surface mesh from a file.
template <typename Polyhedron_3>
Polyhedron_3 read_polygon_mesh(const std::string& filename) {
  using Prn = Polyhedron_3;

  Prn pol;
  if (! CGAL::IO::read_polygon_mesh(filename, pol))
    throw std::runtime_error("Cannot read file!");
  return pol;
}

// Draw a polyhedron.
#ifdef CGALPY_HAS_VISUAL
void draw(const Polyhedron_3& prn) { CGAL::draw(prn); }
#endif

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

/// \name Iterators
/// @{

//
py::object my_vertices(const Polyhedron_3& prn)
{ return make_iterator(prn.vertices_begin(), prn.vertices_end()); }

//
py::object my_halfedges(const Polyhedron_3& prn)
{ return make_iterator(prn.halfedges_begin(), prn.halfedges_end()); }

//
py::object my_edges(const Polyhedron_3& prn)
{ return make_iterator(prn.edges_begin(), prn.edges_end()); }

//
py::object my_faces(const Polyhedron_3& prn)
{ return make_iterator(prn.facets_begin(), prn.facets_end()); }

//
//
py::object halfedges_around_facet(const Face& f)
{ return make_circulator(f.facet_begin()); }

/// @}


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
    ;
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

// Export Polyhedron_3.
void export_polyhedron_3(py::module_& m) {
  using Prn = pol3::Polyhedron_3;
  using Pnt = Prn::Point_3;
  using Vertex = Prn::Vertex;
  using Halfedge = Prn::Halfedge;
  using Face = Prn::Face;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Pnt>(m, "Point_3")) {
    py::class_<Pnt> pnt_c(m, "Point_3");
    export_point_3<Kernel>(pnt_c);
  }

  export_vertex(m);
  export_halfedge(m);
  export_face(m);

  if (! add_attr<Prn>(m, "Polyhedron_3")) {
    py::class_<Prn> prn_c(m, "Polyhedron_3");
    prn_c.def(py::init<>())
      .def(py::init<const Prn&>())
      .def("make_tetrahedron", &pol3::make_tetrahedron1, ri)
      .def("make_tetrahedron", &pol3::make_tetrahedron2, ri)
      .def("is_tetrahedron", &pol3::is_tetrahedron)
      .def("clear", &Prn::clear)
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
  m.def("draw", &pol3::draw);
#endif
  m.def("read_polygon_mesh", &pol3::read_polygon_mesh<Prn>);
}
