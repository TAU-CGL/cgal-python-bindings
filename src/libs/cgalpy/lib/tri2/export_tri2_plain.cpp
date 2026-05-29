// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <iostream>

#include <nanobind/nanobind.h>
// Do not include pair.h; if you must, you will need to fix the bindings of Edge (which is a pair...)
// #include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>

#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_triangulation_2.h>
#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
#include "CGALPY/basic_viewer_types.hpp"
#endif
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/export_circulator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_dereference_forward_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/triangulation_2_types.hpp"
#include "cgalpy/Tri2_docstrings.hpp"
#include "CGALPY/types.hpp"

void export_tri2_vertex(py::class_<cgalpy::tri2::Triangulation_2, CGAL::Triangulation_cw_ccw_2>&);
void export_tri2_face(py::class_<cgalpy::tri2::Triangulation_2, CGAL::Triangulation_cw_ccw_2>&);

namespace py = nanobind;
namespace tri2_doc = cgalpy::tri2::docstrings;

namespace cgalpy {
namespace tri2 {

//!
void tri2_init(tri2::Triangulation_2* tri, py::list& lst) {
  auto begin = stl_forward_iterator<tri2::Point>(lst);
  auto end = stl_forward_iterator<tri2::Point>(lst, false);
  new (tri) tri2::Triangulation_2(begin, end);  // placement new
}

//!
bool equal(const Face& f1, const Face& f2)
{ return (f1.has_vertex(f2.vertex(0)) && f1.has_vertex(f2.vertex(1)) && f1.has_vertex(f2.vertex(2))); }

//! \brief obtaines a face handle from a face
Face_handle face_to_handle(Face& f) {
  auto n = f.neighbor(0);
  for (auto i = 0; i < 3; ++i) if (equal(*(n->neighbor(i)), f)) return n->neighbor(i);
  return Face_handle();
}

//!
Point circumcenter(Triangulation_2& t, Face& f) {
  auto fh = face_to_handle(f);
  auto res = t.circumcenter(fh);
  return res;
}

//!
size_type degree(const Triangulation_2& tri, Vertex& v)
{ return tri.degree(Vertex_handle(&v)); }

//!
Vertex& finite_vertex(const Triangulation_2& t) { return *(t.finite_vertex()); }

//!
void flip(Triangulation_2& t, Face& f, int i) {
  auto fh = face_to_handle(f);
  t.flip(fh, i);
}

//!
auto includes_edge(Triangulation_2& tri, Vertex& va, Vertex& vb) {
  Vertex_handle vbb;
  Face_handle fr;
  int i;
  auto res = tri.includes_edge(Vertex_handle(&va), Vertex_handle(&vb), vbb, fr, i);
  if (! res) return py::make_tuple(false);
  return py::make_tuple(true, *vbb, *fr, i);
}

//!
auto inexact_locate1(py::handle self, const Point& query) {
  constexpr auto ri(py::rv_policy::reference_internal);
  auto& tri = py::cast<Triangulation_2&>(self);
  auto fh = tri.inexact_locate(query);
  return (fh == Face_handle()) ? py::none() : py::cast(*fh, ri, self);
}

//!
auto inexact_locate2(py::handle self, const Point& query, Face& start) {
  constexpr auto ri(py::rv_policy::reference_internal);
  auto& tri = py::cast<Triangulation_2&>(self);
  auto fh = tri.inexact_locate(query, Face_handle(&start));
  return (fh == Face_handle()) ? py::none() : py::cast(*fh, ri, self);
}

//!
Face& infinite_face(const Triangulation_2& tri) { return *(tri.infinite_face()); }

//!
Vertex& infinite_vertex(const Triangulation_2& tri) { return *(tri.infinite_vertex()); }

//!
Vertex& insert_first(Triangulation_2& tri, const Point& p) { return *(tri.insert_first(p)); }

//!
Vertex& insert_in_edge(Triangulation_2& tri, const Point& p, Face& f, int i)
{ return *(tri.insert_in_edge(p, Face_handle(&f), i)); }

//!
Vertex& insert_in_face(Triangulation_2& tri, const Point& p, Face& f)
{ return *(tri.insert_in_face(p, Face_handle(&f))); }

//!
Vertex& insert_outside_affine_hull(Triangulation_2& tri, const Point& p)
{ return *(tri.insert_outside_affine_hull(p)); }

//!
Vertex& insert_outside_convex_hull(Triangulation_2& tri, const Point& p, Face& f)
{ return *(tri.insert_outside_convex_hull(p, Face_handle(&f))); }

//!
Vertex& insert_second(Triangulation_2& tri, const Point& p) { return *(tri.insert_second(p)); }

//!
bool is_edge(const Triangulation_2& tri, Vertex& v1, Vertex& v2)
{ return tri.is_edge(Vertex_handle(&v1), Vertex_handle(&v2)); }

//!
auto is_edge_get_edge(py::handle self, Vertex& v1, Vertex& v2) {
  constexpr auto ri(py::rv_policy::reference_internal);
  auto& tri = py::cast<Triangulation_2&>(self);
  Face_handle fh;
  int i;
  auto res = tri.is_edge(Vertex_handle(&v1), Vertex_handle(&v2), fh, i);
  if (! res) return py::make_tuple(false);
  return py::make_tuple(true, py::cast(*fh, ri, self), i);
}

//!
bool is_face(const Triangulation_2& tri, Vertex& v1, Vertex& v2, Vertex& v3)
  { return tri.is_face(Vertex_handle(&v1), Vertex_handle(&v2), Vertex_handle(&v3)); }

//!
auto is_face_get_face(py::handle self, Vertex& v1, Vertex& v2, Vertex& v3) {
  constexpr auto ri(py::rv_policy::reference_internal);
  auto& tri = py::cast<Triangulation_2&>(self);
  Face_handle fh;
  auto res = tri.is_face(Vertex_handle(&v1), Vertex_handle(&v2), Vertex_handle(&v3), fh);
  if (! res) return py::make_tuple(false);
  return py::make_tuple(true, py::cast(*fh, ri, self));
}

//!
bool is_infinite1(const Triangulation_2& tri, Face& f) { return tri.is_infinite(Face_handle(&f)); }

//!
bool is_infinite2(const Triangulation_2& tri, Vertex& v) { return tri.is_infinite(Vertex_handle(&v)); }

//!
bool is_infinite3(const Triangulation_2& tri, Face& f, int i) { return tri.is_infinite(Face_handle(&f), i); }

//!
auto locate1(py::handle self, const Point& query) {
  constexpr auto ri(py::rv_policy::reference_internal);
  auto& tri = py::cast<Triangulation_2&>(self);
  auto fh = tri.locate(query);
  return (fh == Face_handle()) ? py::none() : py::cast(*fh, ri, self);
}

//!
auto locate2(py::handle self, const Point& query, Face& hint) {
   constexpr auto ri(py::rv_policy::reference_internal);
  auto& tri = py::cast<Triangulation_2&>(self);
  auto fh =  tri.locate(query, Face_handle(&hint));
  return (fh == Face_handle()) ? py::none() : py::cast(*fh, ri, self);
}

//!
py::object locate_dispatch(py::handle self, Face_handle fh, Locate_type lt, int li) {
  constexpr auto ri(py::rv_policy::reference_internal);
  switch (lt) {
   case Triangulation_2::VERTEX:
   case Triangulation_2::EDGE:
   case Triangulation_2::FACE: return py::make_tuple(py::cast(lt), py::cast(*fh, ri, self), py::int_(li));
   case Triangulation_2::OUTSIDE_CONVEX_HULL: return py::make_tuple(py::cast(lt), py::cast(*fh, ri, self));
   case Triangulation_2::OUTSIDE_AFFINE_HULL: return py::make_tuple(py::cast(lt));
  }
  return py::make_tuple(py::cast(lt));
}

//!
py::object locate_get_incident1(py::handle self, const Point& query) {
  auto& tri = py::cast<Triangulation_2&>(self);
  Locate_type lt;
  int li;
  auto fh = tri.locate(query, lt, li);
  return locate_dispatch(self, fh, lt, li);
}

//!
py::object locate_get_incident2(py::handle self, const Point& query, Face& hint) {
  auto& tri = py::cast<Triangulation_2&>(self);
  Locate_type lt;
  int li;
  auto fh = tri.locate(query, lt, li, Face_handle(&hint));
  return locate_dispatch(self, fh, lt, li);
}

//!
int mirror_index(const Triangulation_2& tri, Face& f, int i) { return tri.mirror_index(Face_handle(&f), i); }

//!
Vertex& mirror_vertex(const Triangulation_2& tri, Face& f, int i) { return *(tri.mirror_vertex(Face_handle(&f), i)); }

//!
Vertex& move(Triangulation_2& tri, Vertex& v, const Point& p) { return *(tri.move(Vertex_handle(&v), p)); }

//!
Vertex& move_if_no_collision(Triangulation_2& tri, Vertex& v, const Point& p)
{ return *(tri.move_if_no_collision(Vertex_handle(&v), p)); }

//!
CGAL::Oriented_side oriented_side(const Triangulation_2& tri, Face& f, const Point& p)
{ return tri.oriented_side(Face_handle(&f), p); }

//!
const Point& point1(const Triangulation_2& tri, Vertex& v) { return tri.point(Vertex_handle(&v)); }

//!
const Point& point2(const Triangulation_2& tri, Face& f, int i) { return tri.point(Face_handle(&f), i); }

//!
Vertex& push_back(Triangulation_2& tri, const Point& p) { return *(tri.push_back(p)); }

//!
void remove(Triangulation_2& tri, Vertex& v) { tri.remove(v.handle()); }

//!
void remove_degree_31(Triangulation_2& tri, Vertex& v) { tri.remove_degree_3(Vertex_handle(&v)); }

//!
void remove_degree_32(Triangulation_2& tri, Vertex& v, Face& f)
{ tri.remove_degree_3(Vertex_handle(&v), Face_handle(&f)); }

//!
void remove_first(Triangulation_2& tri, Vertex& v) { tri.remove_first(Vertex_handle(&v)); }

//!
void remove_second(Triangulation_2& tri, Vertex& v) { tri.remove_second(Vertex_handle(&v)); }

//!
Segment segment(const Triangulation_2& tri, Face& f, int i) { return tri.segment(Face_handle(&f), i); }

//!
void set_infinite_vertex(Triangulation_2& tri, Vertex& v) { tri.set_infinite_vertex(Vertex_handle(&v)); }

//!
CGAL::Oriented_side side_of_oriented_circle(const Triangulation_2& tri, Face& f, const Point& p, bool perturb)
{ return tri.side_of_oriented_circle(Face_handle(&f), p, perturb); }

//!
Vertex& star_hole1(Triangulation_2& tri, const Point& p, py::list& edges) {
  auto edges_begin = stl_forward_iterator<Edge>(edges);
  auto edges_end = stl_forward_iterator<Edge>(edges, false);
  return *(tri.star_hole(p, edges_begin, edges_end));
}

//!
Vertex& star_hole2(Triangulation_2& tri, const Point& p, py::list& edges, py::list& faces) {
  auto edges_begin = stl_forward_iterator<Edge>(edges);
  auto edges_end = stl_forward_iterator<Edge>(edges, false);
  auto faces_begin = stl_dereference_forward_iterator<Face_handle>(faces);
  auto faces_end = stl_dereference_forward_iterator<Face_handle>(faces, false);
  return *(tri.star_hole(p, edges_begin, edges_end, faces_begin, faces_end));
}

//!
Triangle triangle(Triangulation_2& t, Face& f) {
  auto fh = face_to_handle(f);
  auto res = t.triangle(fh);
  return res;
}

template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

// Iterators

//
py::object all_vertices(const Triangulation_2& tri)
{ return make_iterator(tri.all_vertices_begin(), tri.all_vertices_end()); }

py::object all_edges(const Triangulation_2& tri)
{ return make_iterator(tri.all_edges_begin(), tri.all_edges_end()); }

//
py::object finite_vertices(const Triangulation_2& tri)
{ return make_iterator(tri.finite_vertices_begin(), tri.finite_vertices_end()); }

//
py::object all_faces(const Triangulation_2& tri)
{ return make_iterator(tri.all_faces_begin(), tri.all_faces_end()); }

//
py::object finite_faces(const Triangulation_2& tri)
{ return make_iterator(tri.finite_faces_begin(), tri.finite_faces_end()); }

//
py::object finite_edges(const Triangulation_2& tri)
{ return make_iterator(tri.finite_edges_begin(), tri.finite_edges_end()); }

//
py::object points(const Triangulation_2& tri)
{ return make_iterator(tri.points_begin(), tri.points_end()); }

// Circulators

//! Wrap the function that obtains the real circulator
auto incident_faces_circulator_0(const Triangulation_2& tri, const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return tri.incident_faces(vh);
}

//! Wrap the function that obtains the real circulator
auto incident_faces_circulator_1(const Triangulation_2& tri, const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return tri.incident_faces(vh, fh);
}

//! Wrap the function that obtains the real circulator
auto incident_edges_circulator_0(const Triangulation_2& tri, const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return tri.incident_edges(vh);
}

//! Wrap the function that obtains the real circulator
auto incident_edges_circulator_1(const Triangulation_2& tri, const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return tri.incident_edges(vh, fh);
}

//! Wrap the function that obtains the real circulator
auto incident_vertices_circulator_0(const Triangulation_2& tri, const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return tri.incident_vertices(vh);
}

//! Wrap the function that obtains the real circulator
auto incident_vertices_circulator_1(const Triangulation_2& tri, const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return tri.incident_vertices(vh, fh);
}

// Iterators

//
py::object incident_faces_iterator_0(const Triangulation_2& tri,
                                     const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return make_iterator(tri.incident_faces(vh), tri.incident_faces(vh));
}

//
py::object incident_faces_iterator_1(const Triangulation_2& tri,
                                     const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return make_iterator(tri.incident_faces(vh, fh), tri.incident_faces(vh, fh));
}

py::object incident_edges_iterator_0(const Triangulation_2& tri,
                                       const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return make_iterator(tri.incident_edges(vh), tri.incident_edges(vh));
}

//
py::object incident_edges_iterator_1(const Triangulation_2& tri,
                                     const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return make_iterator(tri.incident_edges(vh, fh), tri.incident_edges(vh, fh));
}

py::object incident_vertices_iterator_0(const Triangulation_2& tri,
                                          const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return make_iterator(tri.incident_vertices(vh), tri.incident_vertices(vh));
}

//
py::object incident_vertices_iterator_1(const Triangulation_2& tri,
                                        const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return make_iterator(tri.incident_vertices(vh, fh), tri.incident_vertices(vh, fh));
}

}
} // namespace cgalpy // End of namespace tri2

//! Bindings for Triangulation_2
void export_tri2_plain(py::module_& m) {
  constexpr auto ri(py::rv_policy::reference_internal);

  using Tri = cgalpy::tri2::Triangulation_2;
  if (add_attr<Tri>(m, "Triangulation_2")) return;

  using Tricc = CGAL::Triangulation_cw_ccw_2;
  py::class_<Tri, Tricc> tri_c(m, "Triangulation_2",
                                  tri2_doc::Triangulation_2_class);

  using Traits = cgalpy::tri2::Traits;
  using Vertex = Tri::Vertex;
  using Face = Tri::Face;
  using Edge = Tri::Edge;
  using Pnt = Tri::Point;

  tri_c.def(py::init<>(),
            "Constructs an empty 2D triangulation.")
    .def(py::init<Tri&>(),
         py::arg("other"),
         "Copy-constructs a 2D triangulation.")
    .def(py::init<const cgalpy::tri2::Traits&>(),
         py::arg("traits"),
         "Constructs a 2D triangulation with geometric traits.")
    .def("__init__", &cgalpy::tri2::tri2_init,
         py::arg("points"),
         "Constructs a 2D triangulation from input points.")
    .def("circumcenter", &cgalpy::tri2::circumcenter, py::arg("f"),
         tri2_doc::Triangulation_2_circumcenter)
    .def("clear", &Tri::clear,
         tri2_doc::Triangulation_2_clear)
    .def("degree", &cgalpy::tri2::degree, py::arg("v"),
         tri2_doc::Triangulation_2_degree)
    .def("dimension", &Tri::dimension,
         tri2_doc::Triangulation_2_dimension)
    .def("finite_vertex", &cgalpy::tri2::finite_vertex, ri,
         tri2_doc::Triangulation_2_finite_vertex)
    .def("flip", &cgalpy::tri2::flip, py::arg("f"), py::arg("i"),
         tri2_doc::Triangulation_2_flip)
    .def("geom_traits", &Tri::geom_traits, ri,
         tri2_doc::Triangulation_2_geom_traits)
    .def("includes_edge", &cgalpy::tri2::includes_edge, py::arg("va"), py::arg("vb"),
         tri2_doc::Triangulation_2_includes_edge)
    .def("inexact_locate", &cgalpy::tri2::inexact_locate1, py::arg("query"),
         tri2_doc::Triangulation_2_inexact_locate)
    .def("inexact_locate", &cgalpy::tri2::inexact_locate2, py::arg("query"), py::arg("start"),
         tri2_doc::Triangulation_2_inexact_locate)
    .def("infinite_face", &cgalpy::tri2::infinite_face, ri,
         tri2_doc::Triangulation_2_infinite_face)
    .def("infinite_vertex", &cgalpy::tri2::infinite_vertex, ri,
         tri2_doc::Triangulation_2_infinite_vertex)
    .def("insert", &cgalpy::tri2::insert_point1<Tri>, ri, py::arg("p"),
         tri2_doc::Triangulation_2_insert)
    .def("insert", &cgalpy::tri2::insert_point2<Tri>, ri, py::arg("p"), py::arg("start"),
         tri2_doc::Triangulation_2_insert)
    .def("insert", &cgalpy::tri2::insert_point3<Tri>, ri, py::arg("p"), py::arg("lt"), py::arg("loc"), py::arg("li"),
         tri2_doc::Triangulation_2_insert_1)
    .def("insert", &cgalpy::tri2::insert_points<Tri>, ri, py::arg("points"),
         tri2_doc::Triangulation_2_insert_2)
    .def("insert_first", &cgalpy::tri2::insert_first, ri, py::arg("p"),
         tri2_doc::Triangulation_2_insert_first)
    .def("insert_in_edge", &cgalpy::tri2::insert_in_edge, ri, py::arg("p"), py::arg("f"), py::arg("i"),
         tri2_doc::Triangulation_2_insert_in_edge)
    .def("insert_in_face", &cgalpy::tri2::insert_in_face, ri, py::arg("p"), py::arg("f"),
         tri2_doc::Triangulation_2_insert_in_face)
    .def("insert_outside_affine_hull", &cgalpy::tri2::insert_outside_affine_hull, ri, py::arg("p"),
         tri2_doc::Triangulation_2_insert_outside_affine_hull)
    .def("insert_outside_convex_hull", &cgalpy::tri2::insert_outside_convex_hull, ri, py::arg("p"), py::arg("f"),
         tri2_doc::Triangulation_2_insert_outside_convex_hull)
    .def("insert_second", &cgalpy::tri2::insert_second, ri, py::arg("p"),
         tri2_doc::Triangulation_2_insert_second)
    .def("is_edge", &cgalpy::tri2::is_edge, py::arg("va"), py::arg("vb"),
         tri2_doc::Triangulation_2_is_edge)
    .def("is_edge_get_edge", &cgalpy::tri2::is_edge_get_edge, py::arg("va"), py::arg("vb"),
         tri2_doc::Triangulation_2_is_edge_1)
    .def("is_face", &cgalpy::tri2::is_face, py::arg("v1"), py::arg("v2"), py::arg("v3"),
         tri2_doc::Triangulation_2_is_face)
    .def("is_face_get_face", &cgalpy::tri2::is_face_get_face, py::arg("v1"), py::arg("v2"), py::arg("v3"),
         tri2_doc::Triangulation_2_is_face_1)
    .def("is_infinite", static_cast<bool (Tri::*)(const cgalpy::tri2::Edge&) const>(&Tri::is_infinite), py::arg("e"),
         tri2_doc::Triangulation_2_is_infinite_3)
    .def("is_infinite", &cgalpy::tri2::is_infinite1, py::arg("f"),
         tri2_doc::Triangulation_2_is_infinite_1)
    .def("is_infinite", &cgalpy::tri2::is_infinite2, py::arg("v"),
         tri2_doc::Triangulation_2_is_infinite)
    .def("is_infinite", &cgalpy::tri2::is_infinite3, py::arg("f"), py::arg("i"),
         tri2_doc::Triangulation_2_is_infinite_2)
    .def("is_valid", &Tri::is_valid, py::arg("verbose") = false, py::arg("level") = 0,
         tri2_doc::Triangulation_2_is_valid)
    .def("locate", &cgalpy::tri2::locate1, py::arg("query"),
         tri2_doc::Triangulation_2_locate)
    .def("locate", &cgalpy::tri2::locate2, py::arg("query"), py::arg("start"),
         tri2_doc::Triangulation_2_locate)
    .def("locate_get_incident", &cgalpy::tri2::locate_get_incident1, py::arg("query"),
         tri2_doc::Triangulation_2_locate_1)
    .def("locate_get_incident", &cgalpy::tri2::locate_get_incident2, py::arg("query"), py::arg("start"),
         tri2_doc::Triangulation_2_locate_1)
    .def("mirror_edge", &Tri::mirror_edge, py::arg("e"),
         tri2_doc::Triangulation_2_mirror_edge)
    .def("mirror_index", &cgalpy::tri2::mirror_index, py::arg("f"), py::arg("i"),
         tri2_doc::Triangulation_2_mirror_index)
    .def("mirror_vertex", &cgalpy::tri2::mirror_vertex, ri, py::arg("f"), py::arg("i"),
         tri2_doc::Triangulation_2_mirror_vertex)
    .def("move", &cgalpy::tri2::move, ri, py::arg("v"), py::arg("p"),
         tri2_doc::Triangulation_2_move)
    .def("move_if_no_collision", &cgalpy::tri2::move_if_no_collision, ri, py::arg("v"), py::arg("p"),
         tri2_doc::Triangulation_2_move_if_no_collision)
    .def("number_of_vertices", &Tri::number_of_vertices,
         tri2_doc::Triangulation_2_number_of_vertices)
    .def("number_of_faces", &Tri::number_of_faces,
         tri2_doc::Triangulation_2_number_of_faces)
    .def("oriented_side",
         py::overload_cast<const Pnt&, const Pnt&, const Pnt&, const Pnt&>(&Tri::oriented_side, py::const_),
         py::arg("p0"), py::arg("p1"), py::arg("p2"), py::arg("p"),
         "Returns the oriented side of a point with respect to an oriented triangle.")
    .def("oriented_side", &cgalpy::tri2::oriented_side, py::arg("f"), py::arg("p"),
         tri2_doc::Triangulation_2_oriented_side)
    .def("point", &cgalpy::tri2::point1, py::arg("v"),
         "Obtain the point of a vertex\n"
         "Parameters:\n"
         "  v (Vertex) the input vertex\n"
         "Return:\n"
         "  Point_2\n")
    .def("point", &cgalpy::tri2::point2, py::arg("f"), py::arg("i"),
         "Obtain the point of vertex i of face f\n"
         "Parameters:\n"
         "  f (Face) the input face\n"
         "  i (int)\n"
         "Return:\n"
         "  Point_2\n")
    .def("push_back", &cgalpy::tri2::push_back, ri, py::arg("p"),
         tri2_doc::Triangulation_2_push_back)
    .def("remove", &cgalpy::tri2::remove, py::arg("v"),
         tri2_doc::Triangulation_2_remove)
    .def("remove_degree_3", &cgalpy::tri2::remove_degree_31, py::arg("v"),
         "Remove a vertex of degree three\n"
         "Parameters:\n"
         "  v (Vertex): the vertex to remove\n")
    .def("remove_degree_3", &cgalpy::tri2::remove_degree_32, py::arg("v"), py::arg("f"),
         "Remove a vertex of degree three\n"
         "Parameters:\n"
         "  v (Vertex): the vertex to remove\n"
         "  f (Face)\n")
    .def("remove_first", &cgalpy::tri2::remove_first, py::arg("v"),
         tri2_doc::Triangulation_2_remove_first)
    .def("remove_second", &cgalpy::tri2::remove_second, py::arg("v"),
         tri2_doc::Triangulation_2_remove_second)
    .def("segment", py::overload_cast<const Edge&>(&Tri::segment, py::const_), py::arg("e"),
         tri2_doc::Triangulation_2_segment_1)
    .def("segment", &cgalpy::tri2::segment, py::arg("f"), py::arg("i"),
         tri2_doc::Triangulation_2_segment)
    .def("set_infinite_vertex", &cgalpy::tri2::set_infinite_vertex, py::arg("v"),
         tri2_doc::Triangulation_2_set_infinite_vertex)
    .def("side_of_oriented_circle",
         py::overload_cast<const Pnt&, const Pnt&, const Pnt&, const Pnt&, bool>
         (&Tri::side_of_oriented_circle, py::const_),
         py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("p"), py::arg("perturb"),
         "Determine on which side of the circumcircle of the a face defined by three points lies a forth point\n"
         "Parameters:\n"
         "  p1 (point_2)\n"
         "  p2 (point_2)\n"
         "  p2 (point_2)\n"
         "  p (point_2) the input point\n"
         "  perturb (Boolean)\n"
         "Return:\n"
         "  Oriented_side\n")
    .def("side_of_oriented_circle", &cgalpy::tri2::side_of_oriented_circle,
         py::arg("f"), py::arg("p"), py::arg("perturb") = false,
         tri2_doc::Triangulation_2_side_of_oriented_circle)
    .def("star_hole", &cgalpy::tri2::star_hole1, ri, py::arg("p"), py::arg("edges"),
         tri2_doc::Triangulation_2_star_hole)
    .def("star_hole", &cgalpy::tri2::star_hole2, ri, py::arg("p"), py::arg("edges"), py::arg("faces"),
         tri2_doc::Triangulation_2_star_hole_1)
    .def("swap", &Tri::swap, py::arg("tr"),
         tri2_doc::Triangulation_2_swap)
    .def("tds", py::overload_cast<>(&Tri::tds), ri,
         tri2_doc::Triangulation_2_tds_1)
    .def("triangle", &cgalpy::tri2::triangle, py::arg("f"),
         tri2_doc::Triangulation_2_triangle)

    // operator=
    // operator<<
    // operator>>
    ;

  // line_walk

  // Iterators
  using Avi = Tri::All_vertices_iterator;
  using Aei = Tri::All_edges_iterator;
  using Afi = Tri::All_faces_iterator;
  using Fvi = Tri::Finite_vertices_iterator;
  using Fei = Tri::Finite_edges_iterator;
  using Ffi = Tri::Finite_faces_iterator;
  using Pi = Tri::Point_iterator;

  add_iterator<Avi, Avi, const Vertex&>("All_vertices_iterator", tri_c);
  add_iterator<Aei, Aei, const Edge&>("All_edges_iterator", tri_c);
  add_iterator<Afi, Afi, const Face&>("All_faces_iterator", tri_c);
  add_iterator<Fvi, Fvi, const Vertex&>("Finite_vertices_iterator", tri_c);
  add_iterator<Ffi, Ffi, const Face&>("Finite_faces_iterator", tri_c);
  add_iterator<Fei, Fei, const Edge&>("Finite_edges_iterator", tri_c);
  add_iterator<Pi, Pi, const Pnt&>("Point_iterator", tri_c);

  tri_c.def("all_vertices", &cgalpy::tri2::all_vertices, py::keep_alive<0, 1>(),
            "Returns all vertices of the triangulation.")
    .def("all_edges", &cgalpy::tri2::all_edges, py::keep_alive<0, 1>(),
         tri2_doc::Triangulation_2_all_edges)
    .def("all_faces", &cgalpy::tri2::all_faces, py::keep_alive<0, 1>(),
         "Returns all faces of the triangulation.")
    .def("finite_vertices", &cgalpy::tri2::finite_vertices, py::keep_alive<0, 1>(),
         "Returns finite vertices of the triangulation.")
    .def("finite_edges", &cgalpy::tri2::finite_edges, py::keep_alive<0, 1>(),
         tri2_doc::Triangulation_2_finite_edges)
    .def("finite_faces", &cgalpy::tri2::finite_faces, py::keep_alive<0, 1>(),
         "Returns finite faces of the triangulation.")
    .def("points", &cgalpy::tri2::points, py::keep_alive<0, 1>(),
         tri2_doc::Triangulation_2_points)
    ;

  // Iterators & Circulators
  using Vc = Tri::Vertex_circulator;
  using Ec = Tri::Edge_circulator;
  using Fc = Tri::Face_circulator;

  // Iterators
  add_iterator_from_circulator<Vc>("Vertex_iterator", tri_c);
  add_iterator_from_circulator<Ec, Edge>("Edge_iterator", tri_c);
  add_iterator_from_circulator<Fc>("Face_iterator", tri_c);

  tri_c.def("incident_faces", &cgalpy::tri2::incident_faces_iterator_0,
          py::arg("v"),
          tri2_doc::Triangulation_2_incident_faces)
    .def("incident_faces", &cgalpy::tri2::incident_faces_iterator_1,
         py::arg("v"), py::arg("f"),
         tri2_doc::Triangulation_2_incident_faces_1)
    .def("incident_edges", &cgalpy::tri2::incident_edges_iterator_0,
         py::arg("v"),
         tri2_doc::Triangulation_2_incident_edges)
    .def("incident_edges", &cgalpy::tri2::incident_edges_iterator_1,
         py::arg("v"), py::arg("f"),
         tri2_doc::Triangulation_2_incident_edges_1)
    .def("incident_vertices", &cgalpy::tri2::incident_vertices_iterator_0,
         py::arg("v"),
         tri2_doc::Triangulation_2_incident_vertices)
    .def("incident_vertices", &cgalpy::tri2::incident_vertices_iterator_1,
         py::arg("v"), py::arg("f"),
         tri2_doc::Triangulation_2_incident_vertices_1)
    ;

  // Circulators
  export_circulator<Vc>(tri_c, "Vertex_circulator");
  export_circulator<Ec, Edge>(tri_c, "Edge_circulator");
  export_circulator<Fc>(tri_c, "Face_circulator");

  tri_c.def("incident_faces_circulator", &cgalpy::tri2::incident_faces_circulator_0,
            py::arg("v"),
            "Returns a circulator over faces incident to a vertex.")
    .def("incident_faces_circulator", &cgalpy::tri2::incident_faces_circulator_1,
         py::arg("v"), py::arg("f"),
         "Returns a circulator over faces incident to a vertex, starting at a face.")
    .def("incident_edges_circulator", &cgalpy::tri2::incident_edges_circulator_0,
         py::arg("v"),
         "Returns a circulator over edges incident to a vertex.")
    .def("incident_edges_circulator", &cgalpy::tri2::incident_edges_circulator_1,
         py::arg("v"), py::arg("f"),
         "Returns a circulator over edges incident to a vertex, starting at a face.")
    .def("incident_vertices_circulator", &cgalpy::tri2::incident_vertices_circulator_0,
         py::arg("v"),
         "Returns a circulator over vertices incident to a vertex.")
    .def("incident_vertices_circulator", &cgalpy::tri2::incident_vertices_circulator_1,
         py::arg("v"), py::arg("f"),
         "Returns a circulator over vertices incident to a vertex, starting at a face.")
    ;

  // Enumerations
  py::enum_<cgalpy::tri2::Locate_type>(tri_c, "Locate_type")
    .value("VERTEX", Tri::VERTEX)
    .value("EDGE", Tri::EDGE)
    .value("FACE", Tri::FACE)
    .value("OUTSIDE_CONVEX_HULL", Tri::OUTSIDE_CONVEX_HULL)
    .value("OUTSIDE_AFFINE_HULL", Tri::OUTSIDE_AFFINE_HULL)
    .export_values()
    ;

  // Types that have been registered already:
#if ((CGALPY_TRI2 == CGALPY_TRI2_PERIODIC_REGULAR) ||       \
     (CGALPY_TRI2 == CGALPY_TRI2_PERIODIC_DELAUNAY))
  // \todo: generate bindings for periodic traits
  ;
#else
  // In the case of non-priodic triangulation the kernel serves as the traits.
  if (! add_attr<cgalpy::tri2::Geom_traits>(tri_c, "Geom_traits"))
    std::cerr << "'cgalpy::tri2::Geom_traits' not registered!\n";
#endif

  if (! add_attr<cgalpy::tri2::Point>(tri_c, "Point"))
    std::cerr << "'cgalpy::tri2::Point' not registered!\n";
  if (! add_attr<cgalpy::tri2::Segment>(tri_c, "Segment"))
    std::cerr << "'cgalpy::tri2::Segment' not registered!\n";
  if (! add_attr<cgalpy::tri2::Triangle>(tri_c, "Triangle"))
    std::cerr << "'cgalpy::tri2::Triangle' not registered!\n";

  export_tri2_vertex(tri_c);
  export_tri2_face(tri_c);

  py::class_<Edge>(tri_c, "Edge",
                   "Edge represented by a face and an edge index.")
    .def("face", [](Edge& e)->cgalpy::tri2::Face& { return *(e.first); } , ri,
         "Returns the face of the edge representation.")
    .def_rw("index", &Edge::second,
            "Index of the edge in its face.")
    ;

#ifdef CGALPY_HAS_VISUAL
  m.def("draw",
        [](const Tri& tri, const char* title)
        { CGAL::draw(tri, title); },
        py::arg("tri"), py::arg("title"),
        "Draws the triangulation.");

#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
  m.def("draw",
        [](const Tri& tri, const cgalpy::bvr::Graphics_scene_options& gso, const char* title)
        { CGAL::draw(tri, gso, title); },
        py::arg("tri"), py::arg("graphics_scene_options"), py::arg("title"),
        "Draws the triangulation with graphics-scene options.");
#endif
#endif
}
