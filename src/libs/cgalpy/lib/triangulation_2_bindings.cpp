// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

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

template <typename T>
CopyIterator<All_edges_iterator>* all_edges_iterator(T& t) {
  return new CopyIterator<All_edges_iterator>(t.all_edges_begin(), t.all_edges_end());
}

template <typename T>
CopyIterator<Finite_edges_iterator>* finite_edges_iterator(T& t) {
  return new CopyIterator<Finite_edges_iterator>(t.finite_edges_begin(), t.finite_edges_end());
}

template<typename T>
Copy_iterator_from_circulator<Edge_circulator>*
edges_around_vertex_iterator0(T& t, Vertex& v) {
  return new Copy_iterator_from_circulator<Edge_circulator>(t.incident_edges(v.handle()));
}

template<typename T>
Copy_iterator_from_circulator<Edge_circulator>*
edges_around_vertex_iterator1(T& t, Vertex& v, Face& f) {
  auto fh = face_to_handle(f);
  return new Copy_iterator_from_circulator<Edge_circulator>(t.incident_edges(v.handle(), fh));
}

template<typename T>
Iterator_from_circulator<Face_circulator>*
faces_around_vertex_iterator0(T& t, Vertex& v) {
  return new Iterator_from_circulator<Face_circulator>(t.incident_faces(v.handle()));
}

template<typename T>
Iterator_from_circulator<Face_circulator>*
faces_around_vertex_iterator1(T& t, Vertex& v, Face& f) {
  auto fh = face_to_handle(f);
  return new Iterator_from_circulator<Face_circulator>(t.incident_faces(v.handle(), fh));
}

template<typename T>
Iterator_from_circulator<Vertex_circulator>*
vertices_around_vertex_iterator0(T& t, Vertex& v) {
  return new Iterator_from_circulator<Vertex_circulator>(t.incident_vertices(v.handle()));
}

template<typename T>
Iterator_from_circulator<Vertex_circulator>*
vertices_around_vertex_iterator1(T& t, Vertex& v, Face& f) {
  auto fh = face_to_handle(f);
  return new Iterator_from_circulator<Vertex_circulator>(t.incident_vertices(v.handle(), fh));
}

template <typename T>
void insert_list(T& t, bp::list& lst) {
  auto v = std::vector<Point_2>(bp::stl_input_iterator<Point_2>(lst),
                                bp::stl_input_iterator<Point_2>());
  t.insert(v.begin(), v.end());
}

template <typename T>
void flip(T& t, Face& f, int i) {
  auto fh = face_to_handle(f);
  t.flip(fh, i);
}

template <typename T>
typename T::Triangle triangle(T& t, Face& f) {
  auto fh = face_to_handle(f);
  auto res = t.triangle(fh);
  return res;
}

template <typename T>
Point_2 circumcenter(T& t, Face& f) {
  auto fh = face_to_handle(f);
  auto res = t.circumcenter(fh);
  return res;
}

template<typename T>
Vertex& insert_point(T& t, Point_2& p) { return *(t.insert(p)); }

template<typename T>
void remove(T& t, Vertex& v) { t.remove(v.handle()); }

template<typename T>
Face& infinite_face(T& t) { return *(t.infinite_face()); }

template<typename T>
Vertex& infinite_vertex(T& t) { return *(t.infinite_vertex()); }

template<typename T>
Vertex& finite_vertex(T& t) { return *(t.finite_vertex()); }

template <typename T, typename C>
void export_triangulation(C c) {
  c.def(bp::init<>())
    .def(bp::init<T&>())
    .def("dimension", &T::dimension)
    .def("number_of_vertices", &T::number_of_vertices)
    .def("number_of_faces", &T::number_of_faces)
    .def("infinite_face", &infinite_face<T>, bp::return_internal_reference<>())
    .def("infinite_vertex", &infinite_vertex<T>, bp::return_internal_reference<>())
    .def("finite_vertex", &finite_vertex<T>, bp::return_internal_reference<>())
    .def("clear", &T::clear)
    .def("insert", &insert_list<T>)
    .def("insert", &insert_point<T>, bp::return_internal_reference<>())
    .def("triangle", &triangle<T>)
    .def("circumcenter", &circumcenter<T>)
    .def("flip", &flip<T>)
    .def("remove", &remove<T>)
    .def("all_vertices", bp::range<bp::return_internal_reference<>>(&T::all_vertices_begin, &T::all_vertices_end))
    .def("finite_vertices", bp::range<bp::return_internal_reference<>>(&T::finite_vertices_begin, &T::finite_vertices_end))
    .def("all_edges", &all_edges_iterator<T>, bp::return_value_policy<bp::manage_new_object>())
    .def("finite_edges", &finite_edges_iterator<T>, bp::return_value_policy<bp::manage_new_object>())
    .def("all_faces", bp::range<bp::return_internal_reference<>>(&T::all_faces_begin, &T::all_faces_end))
    .def("finite_faces", bp::range<bp::return_internal_reference<>>(&T::finite_faces_begin, &T::finite_faces_end))
    .def("points", bp::range<bp::return_internal_reference<>>(&T::points_begin, &T::points_end))
    //circulators
    .def("incident_vertices", &vertices_around_vertex_iterator0<T>, bp::return_value_policy<bp::manage_new_object>())
    .def("incident_vertices", &vertices_around_vertex_iterator1<T>, bp::return_value_policy<bp::manage_new_object>())
    .def("incident_edges", &edges_around_vertex_iterator0<T>, bp::return_value_policy<bp::manage_new_object>())
    .def("incident_edges", &edges_around_vertex_iterator1<T>, bp::return_value_policy<bp::manage_new_object>())
    .def("incident_faces", &faces_around_vertex_iterator0<T>, bp::return_value_policy<bp::manage_new_object>())
    .def("incident_faces", &faces_around_vertex_iterator1<T>, bp::return_value_policy<bp::manage_new_object>())
    .def("mirror_edge", &T::mirror_edge)
    .def("segment", static_cast<Segment(T::*)(const Edge&) const>(&T::segment))
    .def("is_infinite", static_cast<bool (T::*)(const Edge&) const>(&T::is_infinite))
    .def("ccw", &T::ccw)
    .def("cw", &T::cw)
    //.def()
    ;
}

} // End of namespace tri2

void export_triangulation_2() {
  using namespace boost::python;
  auto c0 = bp::class_<tri2::Triangulation_2>("Triangulation_2");
  tri2::export_triangulation<tri2::Triangulation_2>(c0);

  bp::class_<tri2::Vertex>("Vertex")
    .def<tri2::Point& (tri2::Vertex::*) ()>("point", &tri2::Vertex::point, return_internal_reference<>())
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
