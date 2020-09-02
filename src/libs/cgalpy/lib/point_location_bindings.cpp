// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#ifdef CGALPY_POINT_LOCATION_BINDINGS
#include "CGALPY/common.hpp"

#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>

typedef typename CGAL::Arr_naive_point_location<Arrangement_2>           Naive_pl;
typedef typename CGAL::Arr_walk_along_line_point_location<Arrangement_2> Wal_pl;
typedef typename CGAL::Arr_landmarks_point_location<Arrangement_2>       Landmarks_pl;
typedef typename CGAL::Arr_trapezoid_ric_point_location<Arrangement_2>   Trapezoid_pl;
//typedef typename CGAL::Arr_point_location_result<Arrangement_2>          Pl_result;
//typedef typename std::pair<Point_2, Pl_result::Type>                     Pl_query_result;

typedef typename CGAL::Arr_point_location_result<Arrangement_2>::Type
Pl_result;

//void locate(Arrangement_2& arr, boost::python::list& lst)
//{
//  auto v = std::vector< Point_2 >(boost::python::stl_input_iterator< Point_2 >(lst),
//    boost::python::stl_input_iterator< Point_2 >());
//}

template <typename type>
bool is_type(Pl_result& pl_result)
{
  type* get;
  bool res = (get = boost::get<type>(&pl_result));
  return res;
}

template <typename type>
bool get_type(Pl_result& pl_result, typename type::value_type& t)
{
  type* get;
  bool res = (get = boost::get<type>(&pl_result));
  if (res) t = *(*get);
  return res;
}
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_LINEAR_TRAITS || CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_SEGMENT_TRAITS \
|| CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_NON_CACHING_SEGMENT_TRAITS
void Landmarks_pl_attach(Landmarks_pl& pl, Arrangement_2& arr)
{
  pl.attach(arr);
}
#endif

void export_point_location()
{
  using namespace boost::python;

  //supported only by some of the traits
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_LINEAR_TRAITS || CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_SEGMENT_TRAITS \
|| CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_NON_CACHING_SEGMENT_TRAITS
  class_<Landmarks_pl>("Arr_landmarks_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>()[with_custodian_and_ward<1, 2>()])
    .def("attach", &Landmarks_pl_attach, with_custodian_and_ward<1, 2>())
    .def("detach", &Landmarks_pl::detach)
    .def("locate", &Landmarks_pl::locate)
    ;
#endif
  class_<Trapezoid_pl, boost::noncopyable>("Arr_trapezoid_ric_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>()[with_custodian_and_ward<1, 2>()])
    .def("attach", &Trapezoid_pl::attach, with_custodian_and_ward<1,2>())
    .def("detach", &Trapezoid_pl::detach)
    .def("depth", &Trapezoid_pl::depth)
    .def("longest_query_path_length", &Trapezoid_pl::longest_query_path_length)
    .def("with_guarantees", &Trapezoid_pl::with_guarantees)
    .def<Arrangement_2* (Trapezoid_pl::*)()>("arrangement", &Trapezoid_pl::arrangement, return_value_policy<reference_existing_object>())
    .def("locate", &Trapezoid_pl::locate)
    .def("ray_shoot_up", &Trapezoid_pl::ray_shoot_up)
    .def("ray_shoot_down", &Trapezoid_pl::ray_shoot_down)
    ;

  class_<Wal_pl>("Arr_walk_along_line_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>()[with_custodian_and_ward<1, 2>()])
    .def("attach", &Wal_pl::attach, with_custodian_and_ward<1, 2>())
    .def("detach", &Wal_pl::detach)
    .def("locate", &Wal_pl::locate)
    .def("ray_shoot_up", &Wal_pl::ray_shoot_up)
    .def("ray_shoot_down", &Wal_pl::ray_shoot_down)
    ;

  class_<Naive_pl>("Arr_naive_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>()[with_custodian_and_ward<1, 2>()])
    .def("attach", &Naive_pl::attach, with_custodian_and_ward<1, 2>())
    .def("detach", &Naive_pl::detach)
    .def("locate", &Naive_pl::locate)
    ;

  class_<Pl_result>("Arr_point_location_result")
    .def("empty", &Pl_result::empty)
    .def("is_face", &is_type<Face_const_handle>)
    .def("is_halfedge", &is_type<Halfedge_const_handle>)
    .def("is_vertex", &is_type<Vertex_const_handle>)
    .def("get_face", &get_type<Face_const_handle>)
    .def("get_halfedge", &get_type<Halfedge_const_handle>)
    .def("get_vertex", &get_type<Vertex_const_handle>)
    ;
}
#endif
