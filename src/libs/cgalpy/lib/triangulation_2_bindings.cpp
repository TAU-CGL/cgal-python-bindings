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

#include <CGAL/circulator.h>

#include "CGALPY/triangulation_2_types.hpp"
#include "CGALPY/types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/make_circulator.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace py = nanobind;

namespace tri2 {

bool equal(const Face& f1, const Face& f2) {
  return (f1.has_vertex(f2.vertex(0)) && f1.has_vertex(f2.vertex(1)) &&
          f1.has_vertex(f2.vertex(2)));
}

//! \brief obtaines a face handle from a face
Face_handle face_to_handle(Face& f) {
  auto n = f.neighbor(0);
  for (auto i = 0; i < 3; ++i)
    if (equal(*(n->neighbor(i)), f)) return n->neighbor(i);
  return Face_handle();
}

void insert_list(Triangulation_2& t, py::list& lst) {
  auto begin = stl_input_iterator<Point>(lst);
  auto end = stl_input_iterator<Point>(lst, false);
  t.insert(begin, end);
}

void flip(Triangulation_2& t, Face& f, int i) {
  auto fh = face_to_handle(f);
  t.flip(fh, i);
}

Triangle triangle(Triangulation_2& t, Face& f) {
  auto fh = face_to_handle(f);
  auto res = t.triangle(fh);
  return res;
}

Point circumcenter(Triangulation_2& t, Face& f) {
  auto fh = face_to_handle(f);
  auto res = t.circumcenter(fh);
  return res;
}

Vertex& insert_point(Triangulation_2& t, Point& p) { return *(t.insert(p)); }

void remove(Triangulation_2& t, Vertex& v) { t.remove(v.handle()); }

Face& infinite_face(Triangulation_2& t) { return *(t.infinite_face()); }

Vertex& infinite_vertex(Triangulation_2& t) { return *(t.infinite_vertex()); }

Vertex& finite_vertex(Triangulation_2& t) { return *(t.finite_vertex()); }

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

//
py::object incident_faces_circulator_0(const Triangulation_2& tri,
                                       const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return make_circulator(tri.incident_faces(vh));
}

//
py::object incident_faces_circulator_1(const Triangulation_2& tri,
                                       const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return make_circulator(tri.incident_faces(vh, fh));
}

py::object incident_edges_circulator_0(const Triangulation_2& tri,
                                       const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return make_circulator(tri.incident_edges(vh));
}

//
py::object incident_edges_circulator_1(const Triangulation_2& tri,
                                       const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return make_circulator(tri.incident_edges(vh, fh));
}

py::object incident_vertices_circulator_0(const Triangulation_2& tri,
                                          const Vertex& v) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  return make_circulator(tri.incident_vertices(vh));
}

//
py::object incident_vertices_circulator_1(const Triangulation_2& tri,
                                          const Vertex& v, const Face& f) {
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  return make_circulator(tri.incident_vertices(vh, fh));
}

// Iterators

//
py::object incident_faces_iterator_0(const Triangulation_2& tri,
                                     const Vertex& v) {
  using Tri = Triangulation_2;
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  using Fc = Tri::Face_circulator;
  using Fcc = CGAL::Container_from_circulator<Fc>;
  Fcc range(tri.incident_faces(vh));
  return make_iterator(range.begin(), range.end());
}

//
py::object incident_faces_iterator_1(const Triangulation_2& tri,
                                     const Vertex& v, const Face& f) {
  using Tri = Triangulation_2;
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  using Fc = Tri::Face_circulator;
  using Fcc = CGAL::Container_from_circulator<Fc>;
  Fcc range(tri.incident_faces(vh, fh));
  return make_iterator(range.begin(), range.end());
}

py::object incident_edges_iterator_0(const Triangulation_2& tri,
                                       const Vertex& v) {
  using Tri = Triangulation_2;
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  using Ec = Tri::Edge_circulator;
  using Ecc = CGAL::Container_from_circulator<Ec>;
  Ecc range(tri.incident_edges(vh));
  return make_iterator(range.begin(), range.end());
}

//
py::object incident_edges_iterator_1(const Triangulation_2& tri,
                                     const Vertex& v, const Face& f) {
  using Tri = Triangulation_2;
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  using Ec = Tri::Edge_circulator;
  using Ecc = CGAL::Container_from_circulator<Ec>;
  Ecc range(tri.incident_edges(vh, fh));
  return make_iterator(range.begin(), range.end());
}

py::object incident_vertices_iterator_0(const Triangulation_2& tri,
                                          const Vertex& v) {
  using Tri = Triangulation_2;
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  using Vc = Tri::Vertex_circulator;
  using Vcc = CGAL::Container_from_circulator<Vc>;
  Vcc range(tri.incident_vertices(vh));
  return make_iterator(range.begin(), range.end());
}

//
py::object incident_vertices_iterator_1(const Triangulation_2& tri,
                                        const Vertex& v, const Face& f) {
  using Tri = Triangulation_2;
  auto vh = Vertex_handle(const_cast<Vertex*>(&v));
  auto fh = Face_handle(const_cast<Face*>(&f));
  using Vc = Tri::Vertex_circulator;
  using Vcc = CGAL::Container_from_circulator<Vc>;
  Vcc range(tri.incident_vertices(vh, fh));
  return make_iterator(range.begin(), range.end());
}

//
#if ((CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED) ||        \
     (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY))
void insert_constraint(Triangulation_2& tri,
                       const Vertex& va, const Vertex& vb) {
  auto ha = Vertex_handle(const_cast<Vertex*>(&va));
  auto hb = Vertex_handle(const_cast<Vertex*>(&vb));
  tri.insert_constraint(ha, hb);
}
#endif

} // End of namespace tri2

void export_triangulation_2(py::module_& m) {
  using Tri = tri2::Triangulation_2;

  if (add_attr<Tri>(m, "Triangulation_2")) return;

  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Tri> tri_c(m, "Triangulation_2");
  tri_c.def(py::init<>())
    .def(py::init<Tri&>())
    .def("dimension", &Tri::dimension)
    .def("number_of_vertices", &Tri::number_of_vertices)
    .def("number_of_faces", &Tri::number_of_faces)
    .def("infinite_face", &tri2::infinite_face, ri)
    .def("infinite_vertex", &tri2::infinite_vertex, ri)
    // .def("finite_vertex", &tri2::finite_vertex, ri)
    .def("clear", &Tri::clear)
    .def("insert", &tri2::insert_list)
    .def("insert", &tri2::insert_point, ri)
    .def("triangle", &tri2::triangle)
    .def("circumcenter", &tri2::circumcenter)
    .def("flip", &tri2::flip)
    .def("remove", &tri2::remove)
    .def("mirror_edge", &Tri::mirror_edge)
    .def("segment", static_cast<tri2::Segment(Tri::*)(const tri2::Edge&) const>(&Tri::segment))
    .def("is_infinite", static_cast<bool (Tri::*)(const tri2::Edge&) const>(&Tri::is_infinite))
    .def("ccw", static_cast<int(*)(int)>(&Tri::ccw))
    .def("cw", static_cast<int(*)(int)>(&Tri::cw))
#if ((CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED) ||        \
     (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY))
    .def("insert_constraint", &tri2::insert_constraint)
#endif
    ;

  m.def("ccw", static_cast<int(*)(int)>(&Tri::ccw))
    .def("cw", static_cast<int(*)(int)>(&Tri::cw))
    ;

  using Vertex = Tri::Vertex;
  using Face = Tri::Face;
  using Edge = Tri::Edge;
  using Pnt = Tri::Point;

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

  // Circulators
  using Vc = Tri::Vertex_circulator;
  using Ec = Tri::Edge_circulator;
  using Fc = Tri::Face_circulator;

  add_circulator<Vc>("Vertex_circulator", tri_c);
  add_circulator<Ec, Edge>("Edge_circulator", tri_c);
  add_circulator<Fc>("Face_circulator", tri_c);

  tri_c.def("incident_faces", &tri2::incident_faces_circulator_0)
    .def("incident_faces", &tri2::incident_faces_circulator_1)
    .def("incident_edges", &tri2::incident_edges_circulator_0)
    .def("incident_edges", &tri2::incident_edges_circulator_1)
    .def("incident_vertices", &tri2::incident_vertices_circulator_0)
    .def("incident_vertices", &tri2::incident_vertices_circulator_1)
    ;

  // Iterators
  using Vcc = CGAL::Container_from_circulator<Vc>;
  using Vcci = Vcc::const_iterator;
  using Ecc = CGAL::Container_from_circulator<Ec>;
  using Ecci = Ecc::const_iterator;
  using Fcc = CGAL::Container_from_circulator<Fc>;
  using Fcci = Fcc::const_iterator;

  add_iterator<Vcci, Vcci>("Vertex_iterator", tri_c);
  add_iterator<Ecci, Ecci, Edge>("Edge_iterator", tri_c);
  add_iterator<Fcci, Fcci>("Face_iterator", tri_c);

  tri_c.def("incident_faces_range", &tri2::incident_faces_iterator_0)
    .def("incident_faces_range", &tri2::incident_faces_iterator_1)
    .def("incident_edges_range", &tri2::incident_edges_iterator_0)
    .def("incident_edges_range", &tri2::incident_edges_iterator_1)
    .def("incident_vertices_range", &tri2::incident_vertices_iterator_0)
    .def("incident_vertices_range", &tri2::incident_vertices_iterator_1)
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

  py::class_<Vertex>(tri_c, "Vertex")
    .def(py::init<>())
    .def("point", [](const Vertex& v)->const Pnt& { return v.point(); }, ri)
#if ((CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED) ||        \
     (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY))
    .def("info", [](const Vertex& v)->py::object { return v.info(); })
    .def("set_info", [](Vertex& v, py::object obj) { v.info() = obj; })
#endif
    ;

  // py::class_<tri2::Edge>(tri_c, "Edge")
  //   .def_readwrite("first", &tri2::Edge::first)
  //   .def_readwrite("second", &tri2::Edge::second)
  //   ;

  py::class_<Face>(tri_c, "Face")
    .def("is_valid", &Face::is_valid)
    .def("neighbor", [](const Face& f, int i)->const Face& { return *(f.neighbor(i)); }, ri)
    .def("vertex", [](const Face& f, int i)->const Vertex& { return *(f.vertex(i)); }, ri)
#if ((CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED) ||        \
     (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY))
    .def("info", [](const Face& f)->py::object { return f.info(); })
    .def("set_info", [](Face& f, py::object obj) { f.info() = obj; })
    .def("is_constrained", [](const Face& f, int i)->bool { return f.is_constrained(i); })
#endif
    ;

  // We wrap the handles, because, e.g., the edges iterator value is a handle
  // to a face.
  py::class_<tri2::Vertex_handle>(tri_c, "Vertex_handle")
    .def(py::init<>())
    .def("value", &tri2::value<tri2::Vertex_handle>, ri)
    ;

  py::class_<tri2::Face_handle>(tri_c, "Face_handle")
    .def(py::init<>())
    .def("value", &tri2::value<tri2::Face_handle>, ri)
    ;
}
