// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

template<typename T0, typename T1>
bool is_type(Object& o)
{
  return o.is<T0>() || o.is<T1>();
}

//asssigns the value to t
//returns true iff assignment was successful
template<typename T0, typename T1>
bool get_type(Object& o, typename T0::value_type& t)
{
  T0 get0;
  bool res = CGAL::assign<T0>(get0, o);
  if (res)
  {
    t = *(get0);
    return res;
  }
  else
  {
    T1 get1;
    bool res = CGAL::assign<T1>(get1, o);
    if (res)
    {
      t = *(get1);
    }
  }
  return res;
}

void export_object()
{
  using namespace boost::python;
  class_<Object>("Object", no_init)
    .def("empty", &Object::empty)
    .def("is_vertex", &is_type<Arrangement_2::Vertex_handle, Vertex_const_handle>)
    .def("get_vertex", &get_type<Arrangement_2::Vertex_handle, Vertex_const_handle>)
    .def("is_halfedge", &is_type<Arrangement_2::Halfedge_handle, Halfedge_const_handle>)
    .def("get_halfedge", &get_type<Arrangement_2::Halfedge_handle, Halfedge_const_handle>)
    .def("is_face", &is_type<Arrangement_2::Face_handle, Face_const_handle>)
    .def("get_face", &get_type<Arrangement_2::Face_handle, Face_const_handle>)
    ;
}
