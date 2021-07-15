// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace bp = boost::python;

namespace tri2 {

bool equal(Face& f1, Face& f2) {
  return (f1.has_vertex(f2.vertex(0)) && f1.has_vertex(f2.vertex(1)) &&
          f1.has_vertex(f2.vertex(2)));
}

//workaround to get a face handle from a face
Face_handle face_to_handle(Face& f) {
  Face_handle res;
  auto n = f.neighbor(0);
  for (auto i = 0; i < 3; ++i) {
    if (equal(*(n->neighbor(i)), f)) {
      res = n->neighbor(i);
      continue;
    }
  }
  return res;
}

CopyIterator<All_edges_iterator>* all_edges_iterator(Triangulation_2& t) {
  return new CopyIterator<All_edges_iterator>(t.all_edges_begin(), t.all_edges_end());
}

CopyIterator<Finite_edges_iterator>* finite_edges_iterator(Triangulation_2& t) {
  return new CopyIterator<Finite_edges_iterator>(t.finite_edges_begin(), t.finite_edges_end());
}

Copy_iterator_from_circulator<Edge_circulator>*
edges_around_vertex_iterator0(Triangulation_2& t, Vertex& v) {
  return new Copy_iterator_from_circulator<Edge_circulator>(t.incident_edges(v.handle()));
}

Copy_iterator_from_circulator<Edge_circulator>*
edges_around_vertex_iterator1(Triangulation_2& t, Vertex& v, Face& f) {
  auto fh = face_to_handle(f);
  return new Copy_iterator_from_circulator<Edge_circulator>(t.incident_edges(v.handle(), fh));
}

Iterator_from_circulator<Face_circulator>*
faces_around_vertex_iterator0(Triangulation_2& t, Vertex& v) {
  return new Iterator_from_circulator<Face_circulator>(t.incident_faces(v.handle()));
}

Iterator_from_circulator<Face_circulator>*
faces_around_vertex_iterator1(Triangulation_2& t, Vertex& v, Face& f) {
  auto fh = face_to_handle(f);
  return new Iterator_from_circulator<Face_circulator>(t.incident_faces(v.handle(), fh));
}

Iterator_from_circulator<Vertex_circulator>*
vertices_around_vertex_iterator0(Triangulation_2& t, Vertex& v) {
  return new Iterator_from_circulator<Vertex_circulator>(t.incident_vertices(v.handle()));
}

Iterator_from_circulator<Vertex_circulator>*
vertices_around_vertex_iterator1(Triangulation_2& t, Vertex& v, Face& f) {
  auto fh = face_to_handle(f);
  return new Iterator_from_circulator<Vertex_circulator>(t.incident_vertices(v.handle(), fh));
}

void insert_list(Triangulation_2& t, bp::list& lst) {
  auto v = std::vector<Point>(bp::stl_input_iterator<Point>(lst),
                              bp::stl_input_iterator<Point>());
  t.insert(v.begin(), v.end());
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

} // End of namespace tri2

void export_triangulation_2() {
  typedef tri2::Triangulation_2                 Tri2;

  bp::scope tri2_scope =
    bp::class_<Tri2>("Triangulation_2")
    .def(bp::init<>())
    .def(bp::init<Tri2&>())
    .def("dimension", &Tri2::dimension)
    .def("number_of_vertices", &Tri2::number_of_vertices)
    .def("number_of_faces", &Tri2::number_of_faces)
    .def("infinite_face", &tri2::infinite_face, bp::return_internal_reference<>())
    .def("infinite_vertex", &tri2::infinite_vertex, bp::return_internal_reference<>())
    .def("finite_vertex", &tri2::finite_vertex, bp::return_internal_reference<>())
    .def("clear", &Tri2::clear)
    .def("insert", &tri2::insert_list)
    .def("insert", &tri2::insert_point, bp::return_internal_reference<>())
    .def("triangle", &tri2::triangle)
    .def("circumcenter", &tri2::circumcenter)
    .def("flip", &tri2::flip)
    .def("remove", &tri2::remove)
    .def("all_vertices", bp::range<bp::return_internal_reference<>>(&Tri2::all_vertices_begin, &Tri2::all_vertices_end))
    .def("finite_vertices", bp::range<bp::return_internal_reference<>>(&Tri2::finite_vertices_begin, &Tri2::finite_vertices_end))
    .def("all_edges", &tri2::all_edges_iterator, Manage_new_object())
    .def("finite_edges", &tri2::finite_edges_iterator, Manage_new_object())
    .def("all_faces", bp::range<bp::return_internal_reference<>>(&Tri2::all_faces_begin, &Tri2::all_faces_end))
    .def("finite_faces", bp::range<bp::return_internal_reference<>>(&Tri2::finite_faces_begin, &Tri2::finite_faces_end))
    .def("points", bp::range<bp::return_internal_reference<>>(&Tri2::points_begin, &Tri2::points_end))
    //circulators
    .def("incident_vertices", &tri2::vertices_around_vertex_iterator0, Manage_new_object())
    .def("incident_vertices", &tri2::vertices_around_vertex_iterator1, Manage_new_object())
    .def("incident_edges", &tri2::edges_around_vertex_iterator0, Manage_new_object())
    .def("incident_edges", &tri2::edges_around_vertex_iterator1, Manage_new_object())
    .def("incident_faces", &tri2::faces_around_vertex_iterator0, Manage_new_object())
    .def("incident_faces", &tri2::faces_around_vertex_iterator1, Manage_new_object())
    .def("mirror_edge", &Tri2::mirror_edge)
    .def("segment", static_cast<tri2::Segment(Tri2::*)(const tri2::Edge&) const>(&Tri2::segment))
    .def("is_infinite", static_cast<bool (Tri2::*)(const tri2::Edge&) const>(&Tri2::is_infinite))
    .def("ccw", &Tri2::ccw)
    .def("cw", &Tri2::cw)
    ;

  // Distinguish between kernel traits and non-kernel traits
  // Triangulation_data_structure

  bp::enum_<tri2::Locate_type>("Locate_type")
    .value("VERTEX", Tri2::VERTEX)
    .value("EDGE", Tri2::EDGE)
    .value("FACE", Tri2::FACE)
    .value("OUTSIDE_CONVEX_HULL", Tri2::OUTSIDE_CONVEX_HULL)
    .value("OUTSIDE_AFFINE_HULL", Tri2::OUTSIDE_AFFINE_HULL)
    .export_values()
    ;

  // Types that have been registered already:
  add_attr<tri2::Point>("Point", tri2_scope);
  add_attr<tri2::Segment>("Segment", tri2_scope);
  add_attr<tri2::Triangle>("Triangle", tri2_scope);

  bp::class_<tri2::Vertex>("Vertex")
    .def<tri2::Point&(tri2::Vertex::*)()>("point", &tri2::Vertex::point, bp::return_internal_reference<>())
    ;

  bp::class_<tri2::Edge>("Edge")
    .def_readwrite("first", &tri2::Edge::first)
    .def_readwrite("second", &tri2::Edge::second)
    ;

  bp::class_<tri2::Face>("Face")
    .def("is_valid", &tri2::Face::is_valid)
    ;

  bind_copy_iterator<CopyIterator<tri2::All_edges_iterator>>("Triangulation_all_edges_iterator");
  bind_copy_iterator<CopyIterator<tri2::Finite_edges_iterator>>("Triangulation_finite_edges_iterator");
  bind_copy_iterator<Copy_iterator_from_circulator<tri2::Edge_circulator>>("Triangulation_edges_iterator");

  bind_iterator<Iterator_from_circulator<tri2::Face_circulator>>("Triangulation_faces_iterator");
  bind_iterator<Iterator_from_circulator<tri2::Vertex_circulator>>("Triangulation_vertices_iterator");
}
