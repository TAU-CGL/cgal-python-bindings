// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <iostream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/export_circulator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_dereference_input_iterator.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/triangulation_2_types.hpp"
#include "CGALPY/types.hpp"

void export_tri2_vertex(py::class_<tri2::Triangulation_2, CGAL::Triangulation_cw_ccw_2>&);
void export_tri2_face(py::class_<tri2::Triangulation_2, CGAL::Triangulation_cw_ccw_2>&);

namespace py = nanobind;

namespace tri2 {

//!
void tri2_init(tri2::Triangulation_2* tri, py::list& lst) {
  auto begin = stl_input_iterator<tri2::Point>(lst);
  auto end = stl_input_iterator<tri2::Point>(lst, false);
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
Face& inexact_locate1(const Triangulation_2& tri, const Point& query) {
  auto fh = tri.inexact_locate(query);
  return *fh;
}

//!
Face& inexact_locate2(const Triangulation_2& tri, const Point& query, Face& start) {
  auto fh = tri.inexact_locate(query, Face_handle(&start));
  return *fh;
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
auto is_edge_get_edge(const Triangulation_2& tri, Vertex& v1, Vertex& v2) {
  Face_handle fh;
  int i;
  auto res = tri.is_edge(Vertex_handle(&v1), Vertex_handle(&v2), fh, i);
  if (! res) return py::make_tuple(false);
  return py::make_tuple(true, *fh, i);
}

//!
bool is_face(const Triangulation_2& tri, Vertex& v1, Vertex& v2, Vertex& v3)
  { return tri.is_face(Vertex_handle(&v1), Vertex_handle(&v2), Vertex_handle(&v3)); }

//!
auto is_face_get_face(const Triangulation_2& tri, Vertex& v1, Vertex& v2, Vertex& v3) {
  Face_handle fh;
  auto res = tri.is_face(Vertex_handle(&v1), Vertex_handle(&v2), Vertex_handle(&v3), fh);
  if (! res) return py::make_tuple(false);
  return py::make_tuple(true, *fh);
}

//!
bool is_infinite1(const Triangulation_2& tri, Face& f) { return tri.is_infinite(Face_handle(&f)); }

//!
bool is_infinite2(const Triangulation_2& tri, Vertex& v) { return tri.is_infinite(Vertex_handle(&v)); }

//!
bool is_infinite3(const Triangulation_2& tri, Face& f, int i) { return tri.is_infinite(Face_handle(&f), i); }

//!
bool is_infinite4(const Triangulation_2& tri, const Edge& e) { return tri.is_infinite(e); }

//!
Face& locate1(const Triangulation_2& tri, const Point& query) { return *(tri.locate(query)); }

//!
Face& locate2(const Triangulation_2& tri, const Point& query, Face& hint)
{ return *(tri.locate(query), Face_handle(&hint)); }

//!
auto locate_get_incident1(const Triangulation_2& tri, const Point& query) {
  Locate_type lt;
  int li;
  auto fh = tri.locate(query, lt, li);
  return py::make_tuple(lt, *fh, li);
}

//!
auto locate_get_incident2(const Triangulation_2& tri, const Point& query, Face& hint) {
  Locate_type lt;
  int li;
  auto fh = tri.locate(query, lt, li, Face_handle(&hint));
  return py::make_tuple(lt, *fh, li);
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
  auto edges_begin = stl_input_iterator<Edge>(edges);
  auto edges_end = stl_input_iterator<Edge>(edges, false);
  return *(tri.star_hole(p, edges_begin, edges_end));
}

//!
Vertex& star_hole2(Triangulation_2& tri, const Point& p, py::list& edges, py::list& faces) {
  auto edges_begin = stl_input_iterator<Edge>(edges);
  auto edges_end = stl_input_iterator<Edge>(edges, false);
  auto faces_begin = stl_dereference_input_iterator<Face_handle>(faces);
  auto faces_end = stl_dereference_input_iterator<Face_handle>(faces, false);
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

//
#if ((CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED) ||        \
     (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY))
void insert_constraint(Triangulation_2& tri, const Vertex& va, const Vertex& vb) {
  auto ha = Vertex_handle(const_cast<Vertex*>(&va));
  auto hb = Vertex_handle(const_cast<Vertex*>(&vb));
  tri.insert_constraint(ha, hb);
}
#endif

} // End of namespace tri2

void export_triangulation_2(py::module_& m) {
  constexpr auto ri(py::rv_policy::reference_internal);

  using Tricc = CGAL::Triangulation_cw_ccw_2;

  if (add_attr<Tricc>(m, "Triangulation_cw_ccw_2")) return;

  py::class_<Tricc>(m, "Triangulation_cw_ccw_2")
    .def(py::init<Tricc&>())
    .def_prop_ro_static("ccw", [](py::handle /*unused*/, int i) { return Tricc::ccw(i); })
    .def_prop_ro_static("cw", [](py::handle /*unused*/, int i) { return Tricc::cw(i); })
    ;

  using Tri = tri2::Triangulation_2;
  using Traits = tri2::Traits;
  using Vertex = Tri::Vertex;
  using Face = Tri::Face;
  using Edge = Tri::Edge;
  using Pnt = Tri::Point;

  if (add_attr<Tri>(m, "Triangulation_2")) return;

  py::class_<Tri, Tricc> tri_c(m, "Triangulation_2");
  tri_c.def(py::init<>())
    .def(py::init<Tri&>())
    .def(py::init<const tri2::Traits&>())
    .def("__init__", &tri2::tri2_init)
    .def("circumcenter", &tri2::circumcenter, py::arg("f"))
    .def("clear", &Tri::clear)
    .def("dimension", &Tri::dimension)
    .def("finite_vertex", &tri2::finite_vertex, ri)
    .def("flip", &tri2::flip, py::arg("f"), py::arg("i"))
    .def("geom_traits", &Tri::geom_traits, ri, "Obtain the geometric traits object")
    .def("includes_edge", &tri2::includes_edge)
    .def("inexact_locate", &tri2::inexact_locate1, ri)
    .def("inexact_locate", &tri2::inexact_locate2, ri)
    .def("infinite_face", &tri2::infinite_face, ri)
    .def("infinite_vertex", &tri2::infinite_vertex, ri)
    .def("insert", &tri2::insert_point1<Tri>, ri, py::arg("p"),
         "Parameters:\n"
         "  p (Point_2): The point\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri2::insert_point2<Tri>, ri, py::arg("p"), py::arg("start"),
         "Parameters:\n"
         "  p (Point_2): The point\n"
         "  start (Face): Start the search at this face\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri2::insert_point3<Tri>, ri, py::arg("p"), py::arg("lt"), py::arg("loc"), py::arg("li"),
         "Insert a point into the triangulation using the values returned from a previous location query\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  lt (Locate_type): together with loc and li the return values of a previous location query\n"
         "  loc (Face)\n"
         "  li (int)\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri2::insert_points<Tri>, ri, py::arg("points"),
         "Insert a list of points\n"
         "Parameters:\n"
         "  points (list) the list of points\n"
         "Return:\n"
         "  The number of inserted points\n")
    .def("insert_first", &tri2::insert_first, ri)
    .def("insert_in_edge", &tri2::insert_in_edge, ri)
    .def("insert_in_face", &tri2::insert_in_face, ri)
    .def("insert_outside_affine_hull", &tri2::insert_outside_affine_hull, ri)
    .def("insert_outside_convex_hull", &tri2::insert_outside_convex_hull, ri)
    .def("insert_second", &tri2::insert_second, ri)
    .def("is_edge", &tri2::is_edge)
    .def("is_edge_get_edge", &tri2::is_edge_get_edge)
    .def("is_face", &tri2::is_face)
    .def("is_face_get_face", &tri2::is_face_get_face)
    .def("is_infinite", static_cast<bool (Tri::*)(const tri2::Edge&) const>(&Tri::is_infinite))
    .def("is_infinite", &tri2::is_infinite1)
    .def("is_infinite", &tri2::is_infinite2)
    .def("is_infinite", &tri2::is_infinite3)
    .def("is_infinite", &tri2::is_infinite4)
    .def("is_valid", &Tri::is_valid, py::arg("verbose") = false, py::arg("level") = 0)
    .def("locate", &tri2::locate1, ri)
    .def("locate", &tri2::locate2, ri)
    .def("locate_get_incident", &tri2::locate_get_incident1, ri)
    .def("locate_get_incident", &tri2::locate_get_incident2, ri)
    .def("mirror_edge", &Tri::mirror_edge)
    .def("mirror_index", &tri2::mirror_index)
    .def("mirror_vertex", &tri2::mirror_vertex, ri)
    .def("move", &tri2::move, ri)
    .def("move_if_no_collision", &tri2::move_if_no_collision, ri)
    .def("number_of_vertices", &Tri::number_of_vertices)
    .def("number_of_faces", &Tri::number_of_faces)
    .def("oriented_side",
         py::overload_cast<const Pnt&, const Pnt&, const Pnt&, const Pnt&>(&Tri::oriented_side, py::const_),
         py::arg("p0"), py::arg("p1"), py::arg("p2"), py::arg("p"))
    .def("oriented_side", &tri2::oriented_side)
    .def("push_back", &tri2::push_back, ri)
    .def("remove", &tri2::remove)
    .def("remove_degree_3", &tri2::remove_degree_31)
    .def("remove_degree_3", &tri2::remove_degree_32)
    .def("remove_first", &tri2::remove_first)
    .def("remove_second", &tri2::remove_second)
    .def("segment", py::overload_cast<const Edge&>(&Tri::segment, py::const_))
    .def("segment", &tri2::segment)
    .def("set_infinite_vertex", &tri2::set_infinite_vertex)
    .def("side_of_oriented_circle",
         py::overload_cast<const Pnt&, const Pnt&, const Pnt&, const Pnt&, bool>
         (&Tri::side_of_oriented_circle, py::const_))
    .def("side_of_oriented_circle", &tri2::side_of_oriented_circle,
         py::arg("f"), py::arg("p"), py::arg("perturb") = false)
    .def("star_hole", &tri2::star_hole1, ri)
    .def("star_hole", &tri2::star_hole2, ri)
    .def("swap", &Tri::swap)
    // .def("tds", &Tri::tds, ri)
    .def("triangle", &tri2::triangle)

    // operator=
    // operator<<
    // operator>>

#if ((CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED) ||        \
     (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY))
    .def("insert_constraint", &tri2::insert_constraint)
#endif
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

  tri_c.def("all_vertices", &tri2::all_vertices, py::keep_alive<0, 1>())
    .def("all_edges", &tri2::all_edges, py::keep_alive<0, 1>())
    .def("all_faces", &tri2::all_faces, py::keep_alive<0, 1>())
    .def("finite_vertices", &tri2::finite_vertices, py::keep_alive<0, 1>())
    .def("finite_edges", &tri2::finite_edges, py::keep_alive<0, 1>())
    .def("finite_faces", &tri2::finite_faces, py::keep_alive<0, 1>())
    .def("points", &tri2::points, py::keep_alive<0, 1>())
    ;

  // Iterators & Circulators
  using Vc = Tri::Vertex_circulator;
  using Ec = Tri::Edge_circulator;
  using Fc = Tri::Face_circulator;

  // Iterators
  add_iterator_from_circulator<Vc>("Vertex_iterator", tri_c);
  add_iterator_from_circulator<Ec, Edge>("Edge_iterator", tri_c);
  add_iterator_from_circulator<Fc>("Face_iterator", tri_c);

  tri_c.def("incident_faces", &tri2::incident_faces_iterator_0)
    .def("incident_faces", &tri2::incident_faces_iterator_1)
    .def("incident_edges", &tri2::incident_edges_iterator_0)
    .def("incident_edges", &tri2::incident_edges_iterator_1)
    .def("incident_vertices", &tri2::incident_vertices_iterator_0)
    .def("incident_vertices", &tri2::incident_vertices_iterator_1)
    ;

  // Circulators
  export_circulator<Vc>(tri_c, "Vertex_circulator");
  export_circulator<Ec, Edge>(tri_c, "Edge_circulator");
  export_circulator<Fc>(tri_c, "Face_circulator");

  tri_c.def("incident_faces_circulator", &tri2::incident_faces_circulator_0)
    .def("incident_faces_circulator", &tri2::incident_faces_circulator_1)
    .def("incident_edges_circulator", &tri2::incident_edges_circulator_0)
    .def("incident_edges_circulator", &tri2::incident_edges_circulator_1)
    .def("incident_vertices_circulator", &tri2::incident_vertices_circulator_0)
    .def("incident_vertices_circulator", &tri2::incident_vertices_circulator_1)
    ;

  // Enumerations
  py::enum_<tri2::Locate_type>(tri_c, "Locate_type")
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
  if (! add_attr<tri2::Geom_traits>(tri_c, "Geom_traits"))
    std::cerr << "'tri2::Geom_traits' not registered!\n";
#endif

  if (! add_attr<tri2::Point>(tri_c, "Point"))
    std::cerr << "'tri2::Point' not registered!\n";
  if (! add_attr<tri2::Segment>(tri_c, "Segment"))
    std::cerr << "'tri2::Segment' not registered!\n";
  if (! add_attr<tri2::Triangle>(tri_c, "Triangle"))
    std::cerr << "'tri2::Triangle' not registered!\n";

  export_tri2_vertex(tri_c);
  export_tri2_face(tri_c);

  // py::class_<tri2::Edge>(tri_c, "Edge")
  //   .def_readwrite("first", &tri2::Edge::first)
  //   .def_readwrite("second", &tri2::Edge::second)
  //   ;
}
