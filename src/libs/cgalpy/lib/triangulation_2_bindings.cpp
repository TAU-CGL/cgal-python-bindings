// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/config.hpp"
#ifdef CGALPY_TRIANGULATION_2_BINDINGS
#include "CGALPY/common.hpp"

#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_2.h>
#include "CGALPY/python_iterator_templates.hpp"

namespace bp = boost::python;

typedef CGAL::Triangulation_2<Kernel>                           Triangulation_2;
typedef CGAL::Delaunay_triangulation_2<Kernel>                  Delaunay_triangulation_2;
typedef Triangulation_2::Vertex                                 TVertex;
typedef Triangulation_2::Vertex_circulator                      TVertex_circulator;
typedef Triangulation_2::All_vertices_iterator                  All_vertices_iterator;
typedef Triangulation_2::Finite_vertices_iterator               Finite_vertices_iterator;
typedef Triangulation_2::Edge                                   TEdge;
typedef Triangulation_2::Edge_circulator                        TEdge_circulator;
typedef Triangulation_2::All_edges_iterator                     All_edges_iterator;
typedef Triangulation_2::Finite_edges_iterator                  Finite_edges_iterator;
typedef Triangulation_2::Face                                   TFace;
typedef Triangulation_2::Face_circulator                        TFace_circulator;
typedef Triangulation_2::Face_handle                            TFace_handle;
typedef Triangulation_2::All_faces_iterator                     All_faces_iterator;
typedef Triangulation_2::Finite_faces_iterator                  Finite_faces_iterator;
typedef Triangulation_2::Point_iterator                         Point_iterator;

bool equal(TFace& f1, TFace& f2)
{
  return (f1.has_vertex(f2.vertex(0)) && f1.has_vertex(f2.vertex(1)) && f1.has_vertex(f2.vertex(2)));
}

//workaround to get a face handle from a face
TFace_handle face_to_handle(TFace& f)
{
  TFace_handle res;
  auto n = f.neighbor(0);
  for (auto i = 0; i < 3; ++i)
  {
    if (equal(*(n->neighbor(i)), f))
    {
      res = n->neighbor(i);
      continue;
    }
  }
  return res;
}

template <typename T>
CopyIterator<All_edges_iterator>* all_edges_iterator(T& t)
{
  return new CopyIterator<All_edges_iterator>(t.all_edges_begin(), t.all_edges_end());
}

template <typename T>
CopyIterator<Finite_edges_iterator>* finite_edges_iterator(T& t)
{
  return new CopyIterator<Finite_edges_iterator>(t.finite_edges_begin(), t.finite_edges_end());
}


template<typename T>
Copy_iterator_from_circulator<TEdge_circulator>* edges_around_vertex_iterator0(T& t ,TVertex& v)
{
  return new Copy_iterator_from_circulator<TEdge_circulator>(t.incident_edges(v.handle()));
}

template<typename T>
Copy_iterator_from_circulator<TEdge_circulator>* edges_around_vertex_iterator1(T& t, TVertex& v, TFace& f)
{
  auto fh = face_to_handle(f);
  return new Copy_iterator_from_circulator<TEdge_circulator>(t.incident_edges(v.handle(), fh));
}

template<typename T>
Iterator_from_circulator<TFace_circulator>* faces_around_vertex_iterator0(T& t, TVertex& v)
{
  return new Iterator_from_circulator<TFace_circulator>(t.incident_faces(v.handle()));
}

template<typename T>
Iterator_from_circulator<TFace_circulator>* faces_around_vertex_iterator1(T& t, TVertex& v, TFace& f)
{
  auto fh = face_to_handle(f);
  return new Iterator_from_circulator<TFace_circulator>(t.incident_faces(v.handle(), fh));
}

template<typename T>
Iterator_from_circulator<TVertex_circulator>* vertices_around_vertex_iterator0(T& t, TVertex& v)
{
  return new Iterator_from_circulator<TVertex_circulator>(t.incident_vertices(v.handle()));
}

template<typename T>
Iterator_from_circulator<TVertex_circulator>* vertices_around_vertex_iterator1(T& t, TVertex& v, TFace& f)
{
  auto fh = face_to_handle(f);
  return new Iterator_from_circulator<TVertex_circulator>(t.incident_vertices(v.handle(), fh));
}

template <typename T>
void insert_list(T& t, bp::list& lst)
{
  auto v = std::vector< Point_2 >(bp::stl_input_iterator< Point_2 >(lst),
    bp::stl_input_iterator< Point_2 >());
  t.insert(v.begin(), v.end());
}

template <typename T>
void flip(T& t, TFace& f, int i)
{
  auto fh = face_to_handle(f);
  t.flip(fh, i);
}

template <typename T>
typename T::Triangle triangle(T& t, TFace& f)
{
  auto fh = face_to_handle(f);
  auto res = t.triangle(fh);
  return res;
}

template <typename T>
Point_2 circumcenter(T& t, TFace& f)
{
auto fh = face_to_handle(f);
auto res = t.circumcenter(fh);
return res;
}

template<typename T>
TVertex& insert_point(T& t, Point_2& p)
{
  return *(t.insert(p));
}

template<typename T>
void remove(T& t, TVertex& v)
{
  t.remove(v.handle());
}

template <typename T, typename C>
void export_triangulation(C c)
{
  using namespace boost::python;
  //class_<Triangulation_2>("Triangulation");

  c
    .def(init<>())
    .def(init<T&>())
    .def("dimension", &T::dimension)
    .def("number_of_vertices", &T::number_of_vertices)
    .def("number_of_faces", &T::number_of_faces)
    //infinite face, vertex, finite vertex
    .def("clear", &T::clear)
    .def("insert", &insert_list<T>)
    .def("insert", &insert_point<T>, return_internal_reference<>())
    .def("triangle", &triangle<T>)
    .def("circumcenter", &circumcenter<T>)
    .def("flip", &flip<T>)
    .def("remove", &remove<T>)
    .def("all_vertices", range<return_internal_reference<>>(&T::all_vertices_begin, &T::all_vertices_end))
    .def("finite_vertices", range<return_internal_reference<>>(&T::finite_vertices_begin, &T::finite_vertices_end))
    .def("all_edges", &all_edges_iterator<T>, return_value_policy<manage_new_object>())
    .def("finite_edges", &finite_edges_iterator<T>, return_value_policy<manage_new_object>())
    .def("all_faces", range<return_internal_reference<>>(&T::all_faces_begin, &T::all_faces_end))
    .def("finite_faces", range<return_internal_reference<>>(&T::finite_faces_begin, &T::finite_faces_end))
    .def("points", range<return_internal_reference<>>(&T::points_begin, &T::points_end))
    //circulators
    .def("incident_vertices", &vertices_around_vertex_iterator0<T>, return_value_policy<manage_new_object>())
    .def("incident_vertices", &vertices_around_vertex_iterator1<T>, return_value_policy<manage_new_object>())
    .def("incident_edges", &edges_around_vertex_iterator0<T>, return_value_policy<manage_new_object>())
    .def("incident_edges", &edges_around_vertex_iterator1<T>, return_value_policy<manage_new_object>())
    .def("incident_faces", &faces_around_vertex_iterator0<T>, return_value_policy<manage_new_object>())
    .def("incident_faces", &faces_around_vertex_iterator1<T>, return_value_policy<manage_new_object>())
    .def("mirror_edge", &T::mirror_edge)
    .def("segment", static_cast<Segment_2(T::*)(const TEdge&) const>(&T::segment))
    .def("is_infinite", static_cast<bool (T::*)(const TEdge&) const>(&T::is_infinite))
    .def("ccw", &T::ccw)
    .def("cw", &T::cw)
    //.def()
    ;
}

void export_triangulations()
{
  using namespace boost::python;
  auto c0 = class_<Triangulation_2>("Triangulation_2");
  auto c1 = class_<Delaunay_triangulation_2, bases<Triangulation_2>>("Delaunay_triangulation_2");

  export_triangulation<Triangulation_2>(c0);
  export_triangulation<Delaunay_triangulation_2>(c1);

  class_<TVertex>("Triangulation_vertex")
    .def<Point_2& (TVertex::*) ()>("point", &TVertex::point, return_internal_reference<>())
    ;

  class_<TEdge>("Triangulation_edge")
    .def_readwrite("first", &TEdge::first)
    .def_readwrite("second", &TEdge::second)
    ;
  class_<TFace>("Triangulation_face")
    .def("is_valid", &TFace::is_valid)
    ;


  bind_copy_iterator<CopyIterator<All_edges_iterator>>("Triangulation_all_edges_iterator");
  bind_copy_iterator<CopyIterator<Finite_edges_iterator>>("Triangulation_finite_edges_iterator");
  bind_copy_iterator<Copy_iterator_from_circulator<TEdge_circulator>>("Triangulation_edges_iterator");

  bind_iterator<Iterator_from_circulator<TFace_circulator>>("Triangulation_faces_iterator");
  bind_iterator<Iterator_from_circulator<TVertex_circulator>>("Triangulation_vertices_iterator");
}
#endif
