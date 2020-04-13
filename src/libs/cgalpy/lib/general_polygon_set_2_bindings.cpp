// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#ifdef CGALPY_MINKOWSKI_SUM_2_BINDINGS
#include "CGALPY/common.hpp"

#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>

typedef typename CGAL::Gps_circle_segment_traits_2<Kernel> CSTraits;
typedef typename CSTraits::Polygon_2                       General_polygon_2;
typedef typename CSTraits::Polygon_with_holes_2
  General_polygon_with_holes_2;
typedef typename CGAL::General_polygon_set_2<CSTraits>     General_polygon_set_2;
typedef General_polygon_2::X_monotone_curve_2
  CS_traits_X_monotone_curve_2;
typedef General_polygon_2::Curve_iterator                  Curve_iterator;
typedef CSTraits::Point_2                                  CSPoint_2;

void polygons_with_holes(General_polygon_set_2& ps, boost::python::list& lst)
{
  auto v = std::vector<General_polygon_with_holes_2>();
  ps.polygons_with_holes(std::back_inserter(v));
  for (auto pwh : v)
  {
    lst.append(pwh);
  }
}

void insert0(General_polygon_set_2& ps, General_polygon_2& pgn)
{
  ps.insert(pgn);
}

void insert1(General_polygon_set_2& ps, General_polygon_with_holes_2& pwh)
{
  ps.insert(pwh);
}

void insert_range0(General_polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<General_polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<General_polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>();
  auto v0 = std::vector<General_polygon_2>(begin0, end0);
  auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);

  ps.insert(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void insert_range(General_polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.insert(v.begin(), v.end());
}

void complement0(General_polygon_set_2& ps0, General_polygon_set_2& ps1)
{
  ps0.complement(ps1);
}

template <typename T>
void intersection(General_polygon_set_2& ps, T& other)
{
  ps.intersection(other);
}

void intersection(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
                  General_polygon_set_2& ps2)
{
  ps0.intersection(ps1, ps2);
}

void intersection_range0(General_polygon_set_2& ps,
                         boost::python::list& polygon_lst,
                         boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<General_polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<General_polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>();
  auto v0 = std::vector<General_polygon_2>(begin0, end0);
  auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);
  ps.intersection(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void intersection_range(General_polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.intersection(v.begin(), v.end());
}

template <typename T>
void join(General_polygon_set_2& ps, T& other)
{
  ps.join(other);
}

void join(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
          General_polygon_set_2& ps2)
{
  ps0.join(ps1, ps2);
}

void join_range0(General_polygon_set_2& ps, boost::python::list& polygon_lst,
                 boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<General_polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<General_polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>();
  auto v0 = std::vector<General_polygon_2>(begin0, end0);
  auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);
  ps.join(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void join_range(General_polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.join(v.begin(), v.end());
}

template <typename T>
void difference(General_polygon_set_2& ps, T& other)
{
  ps.difference(other);
}

void difference(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
                General_polygon_set_2& ps2)
{
  ps0.difference(ps1, ps2);
}

template <typename T>
void symmetric_difference(General_polygon_set_2& ps, T& other)
{
  ps.symmetric_difference(other);
}

void symmetric_difference(General_polygon_set_2& ps0,
                          General_polygon_set_2& ps1,
                          General_polygon_set_2& ps2)
{
  ps0.symmetric_difference(ps1, ps2);
}

void symmetric_difference_range0(General_polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<General_polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<General_polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>();
  auto v0 = std::vector<General_polygon_2>(begin0, end0);
  auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);
  ps.symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void symmetric_difference_range(General_polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.symmetric_difference(v.begin(), v.end());
}

template <typename T>
bool do_intersect(General_polygon_set_2& ps, T& other)
{
  return ps.do_intersect(other);
}

bool do_intersect_range0(General_polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<General_polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<General_polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<General_polygon_with_holes_2>();
  auto v0 = std::vector<General_polygon_2>(begin0, end0);
  auto v1 = std::vector<General_polygon_with_holes_2>(begin1, end1);
  return ps.do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void do_intersect_range(General_polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.do_intersect(v.begin(), v.end());
}

template<typename T>
CGAL::Oriented_side oriented_side(General_polygon_set_2& ps, T& other)
{
  return ps.oriented_side(other);
}

void export_general_polygon_set_2()
{
  using namespace boost::python;
  class_<General_polygon_set_2>("General_polygon_set_2")
    .def(init<>())
    .def(init<const General_polygon_2&>())
    .def(init<const General_polygon_with_holes_2&>())
    .def(init<const General_polygon_set_2&>())
    .def("is_empty", &General_polygon_set_2::is_empty)
    .def("is_plane", &General_polygon_set_2::is_plane)
    .def("number_of_polygons_with_holes", &General_polygon_set_2::number_of_polygons_with_holes)
    .def("polygons_with_holes", &polygons_with_holes)
    //.def("arrangement", &General_polygon_set_2::arrangement)
    .def("clear", &General_polygon_set_2::clear)
    .def("is_valid", &General_polygon_set_2::is_valid)
    .def("insert", &insert0)
    .def("insert", &insert1)
    .def("insert", &insert_range0)
    .def("insert_polygons", &insert_range<General_polygon_2>)
    .def("insert_polygons_with_holes", &insert_range<General_polygon_with_holes_2>)
    .def<void (General_polygon_set_2::*) ()>("complement", &General_polygon_set_2::complement)
    .def("complement", &complement0)
    .def("intersection", &intersection<General_polygon_set_2>)
    .def("intersection", &intersection<General_polygon_2>)
    .def("intersection", &intersection<General_polygon_with_holes_2>)
    .def<void (General_polygon_set_2&, General_polygon_set_2&, General_polygon_set_2&)>("intersection", &intersection)
    .def("intersection", &intersection_range0)
    .def("intersection_polygons", &intersection_range<General_polygon_2>)
    .def("intersection_polygons_with_holes", &intersection_range<General_polygon_with_holes_2>)
    .def("join", &join<General_polygon_set_2>)
    .def("join", &join<General_polygon_2>)
    .def("join", &join<General_polygon_with_holes_2>)
    .def<void (General_polygon_set_2&, General_polygon_set_2&, General_polygon_set_2&)>("join", &join)
    .def("join", &join_range0)
    .def("join_polygons", &join_range<General_polygon_2>)
    .def("join_polygons_with_holes", &join_range<General_polygon_with_holes_2>)
    .def("difference", &difference<General_polygon_set_2>)
    .def("difference", &difference<General_polygon_2>)
    .def("difference", &difference<General_polygon_with_holes_2>)
    .def<void (General_polygon_set_2&, General_polygon_set_2&, General_polygon_set_2&)>("difference", &difference)
    .def("symmetric_difference", &symmetric_difference<General_polygon_set_2>)
    .def("symmetric_difference", &symmetric_difference<General_polygon_2>)
    .def("symmetric_difference", &symmetric_difference<General_polygon_with_holes_2>)
    .def<void (General_polygon_set_2&, General_polygon_set_2&, General_polygon_set_2&)>("symmetric_difference", &symmetric_difference)
    .def("symmetric_difference", &symmetric_difference_range0)
    .def("symmetric_difference_polygons", &symmetric_difference_range<General_polygon_2>)
    .def("symmetric_difference_polygons_with_holes", &symmetric_difference_range<General_polygon_with_holes_2>)
    .def("do_intersect", &do_intersect<General_polygon_set_2>)
    .def("do_intersect", &do_intersect<General_polygon_2>)
    .def("do_intersect", &do_intersect<General_polygon_with_holes_2>)
    .def("do_intersect", &do_intersect_range0)
    .def("do_intersect_polygons", &do_intersect_range<General_polygon_2>)
    .def("do_intersect_polygons_with_holes", &do_intersect_range<General_polygon_with_holes_2>)
    .def("locate", &General_polygon_set_2::locate)
    .def("oriented_side", &oriented_side<CSPoint_2>)
    .def("oriented_side", &oriented_side<General_polygon_set_2>)
    .def("oriented_side", &oriented_side<General_polygon_2>)
    .def("oriented_side", &oriented_side<General_polygon_with_holes_2>)
    ;
}
#endif
