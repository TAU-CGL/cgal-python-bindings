// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/python.hpp>

#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>

#include "CGALPY/general_polygon_set_2_types.hpp"

namespace bp = boost::python;

namespace bso2 {

void polygons_with_holes(General_polygon_set_2& ps, bp::list& lst) {
  typedef Traits::Polygon_with_holes_2  Polygon_with_holes_2;
  auto v = std::vector<Polygon_with_holes_2>();
  ps.polygons_with_holes(std::back_inserter(v));
  for (auto pwh : v) lst.append(pwh);
}

void insert0(General_polygon_set_2& ps, Traits::Polygon_2& pgn)
{ ps.insert(pgn); }

void insert1(General_polygon_set_2& ps, Traits::Polygon_with_holes_2& pwh)
{ ps.insert(pwh); }

void insert_range0(General_polygon_set_2& ps, bp::list& polygon_lst,
                   bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Traits::Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Traits::Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>();
  auto v0 = std::vector<Traits::Polygon_2>(begin0, end0);
  auto v1 = std::vector<Traits::Polygon_with_holes_2>(begin1, end1);

  ps.insert(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void insert_range(General_polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.insert(v.begin(), v.end());
}

void complement0(General_polygon_set_2& ps0, General_polygon_set_2& ps1)
{ ps0.complement(ps1); }

template <typename T>
void intersection(General_polygon_set_2& ps, T& other)
{ ps.intersection(other); }

void intersection(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
                  General_polygon_set_2& ps2)
{ ps0.intersection(ps1, ps2); }

void intersection_range0(General_polygon_set_2& ps, bp::list& polygon_lst,
                         bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Traits::Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Traits::Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>();
  auto v0 = std::vector<Traits::Polygon_2>(begin0, end0);
  auto v1 = std::vector<Traits::Polygon_with_holes_2>(begin1, end1);
  ps.intersection(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void intersection_range(General_polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.intersection(v.begin(), v.end());
}

template <typename T>
void join(General_polygon_set_2& ps, T& other) { ps.join(other); }

void join(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
          General_polygon_set_2& ps2)
{ ps0.join(ps1, ps2); }

void join_range0(General_polygon_set_2& ps, bp::list& polygon_lst,
                 bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Traits::Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Traits::Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>();
  auto v0 = std::vector<Traits::Polygon_2>(begin0, end0);
  auto v1 = std::vector<Traits::Polygon_with_holes_2>(begin1, end1);
  ps.join(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void join_range(General_polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.join(v.begin(), v.end());
}

template <typename T>
void difference(General_polygon_set_2& ps, T& other) { ps.difference(other); }

void difference(General_polygon_set_2& ps0, General_polygon_set_2& ps1,
                General_polygon_set_2& ps2)
{ ps0.difference(ps1, ps2); }

template <typename T>
void symmetric_difference(General_polygon_set_2& ps, T& other)
{ ps.symmetric_difference(other); }

void symmetric_difference(General_polygon_set_2& ps0,
                          General_polygon_set_2& ps1,
                          General_polygon_set_2& ps2)
{ ps0.symmetric_difference(ps1, ps2); }

void symmetric_difference_range0(General_polygon_set_2& ps,
                                 bp::list& polygon_lst, bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Traits::Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Traits::Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>();
  auto v0 = std::vector<Traits::Polygon_2>(begin0, end0);
  auto v1 = std::vector<Traits::Polygon_with_holes_2>(begin1, end1);
  ps.symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void symmetric_difference_range(General_polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.symmetric_difference(v.begin(), v.end());
}

template <typename T>
bool do_intersect(General_polygon_set_2& ps, T& other)
{ return ps.do_intersect(other); }

bool do_intersect_range0(General_polygon_set_2& ps,
                         bp::list& polygon_lst, bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Traits::Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Traits::Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Traits::Polygon_with_holes_2>();
  auto v0 = std::vector<Traits::Polygon_2>(begin0, end0);
  auto v1 = std::vector<Traits::Polygon_with_holes_2>(begin1, end1);
  return ps.do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void do_intersect_range(General_polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.do_intersect(v.begin(), v.end());
}

template<typename T>
CGAL::Oriented_side oriented_side(General_polygon_set_2& ps, T& other)
{ return ps.oriented_side(other); }

}

void export_general_polygon_set_2() {
  typedef bso2::General_polygon_set_2   Gps2;
  typedef bso2::Traits                  Traits;

  bp::scope ps2_scope = bp::class_<Gps2>("General_polygon_set_2")
    .def(bp::init<>())
    .def(bp::init<const Traits::Polygon_2&>())
    .def(bp::init<const Traits::Polygon_with_holes_2&>())
    .def(bp::init<const Gps2&>())
    .def("is_empty", &Gps2::is_empty)
    .def("is_plane", &Gps2::is_plane)
    .def("number_of_polygons_with_holes", &Gps2::number_of_polygons_with_holes)
    .def("polygons_with_holes", &bso2::polygons_with_holes)
    //.def("arrangement", &Gps2::arrangement)
    .def("clear", &Gps2::clear)
    .def("is_valid", &Gps2::is_valid)
    .def("insert", &bso2::insert0)
    .def("insert", &bso2::insert1)
    .def("insert", &bso2::insert_range0)
    .def("insert_polygons", &bso2::insert_range<Traits::Polygon_2>)
    .def("insert_polygons_with_holes", &bso2::insert_range<Traits::Polygon_with_holes_2>)
    .def<void (Gps2::*) ()>("complement", &Gps2::complement)
    .def("complement", &bso2::complement0)
    .def("intersection", &bso2::intersection<Gps2>)
    .def("intersection", &bso2::intersection<Traits::Polygon_2>)
    .def("intersection", &bso2::intersection<Traits::Polygon_with_holes_2>)
    .def<void (Gps2&, Gps2&, Gps2&)>("intersection", &bso2::intersection)
    .def("intersection", &bso2::intersection_range0)
    .def("intersection_polygons", &bso2::intersection_range<Traits::Polygon_2>)
    .def("intersection_polygons_with_holes", &bso2::intersection_range<Traits::Polygon_with_holes_2>)
    .def("join", &bso2::join<Gps2>)
    .def("join", &bso2::join<Traits::Polygon_2>)
    .def("join", &bso2::join<Traits::Polygon_with_holes_2>)
    .def<void (Gps2&, Gps2&, Gps2&)>("join", &bso2::join)
    .def("join", &bso2::join_range0)
    .def("join_polygons", &bso2::join_range<Traits::Polygon_2>)
    .def("join_polygons_with_holes", &bso2::join_range<Traits::Polygon_with_holes_2>)
    .def("difference", &bso2::difference<Gps2>)
    .def("difference", &bso2::difference<Traits::Polygon_2>)
    .def("difference", &bso2::difference<Traits::Polygon_with_holes_2>)
    .def<void (Gps2&, Gps2&, Gps2&)>("difference", &bso2::difference)
    .def("symmetric_difference", &bso2::symmetric_difference<Gps2>)
    .def("symmetric_difference", &bso2::symmetric_difference<Traits::Polygon_2>)
    .def("symmetric_difference", &bso2::symmetric_difference<Traits::Polygon_with_holes_2>)
    .def<void (Gps2&, Gps2&, Gps2&)>("symmetric_difference", &bso2::symmetric_difference)
    .def("symmetric_difference", &bso2::symmetric_difference_range0)
    .def("symmetric_difference_polygons", &bso2::symmetric_difference_range<Traits::Polygon_2>)
    .def("symmetric_difference_polygons_with_holes", &bso2::symmetric_difference_range<Traits::Polygon_with_holes_2>)
    .def("do_intersect", &bso2::do_intersect<Gps2>)
    .def("do_intersect", &bso2::do_intersect<Traits::Polygon_2>)
    .def("do_intersect", &bso2::do_intersect<Traits::Polygon_with_holes_2>)
    .def("do_intersect", &bso2::do_intersect_range0)
    .def("do_intersect_polygons", &bso2::do_intersect_range<Traits::Polygon_2>)
    .def("do_intersect_polygons_with_holes", &bso2::do_intersect_range<Traits::Polygon_with_holes_2>)
    .def("locate", &Gps2::locate)
    .def("oriented_side", &bso2::oriented_side<Traits::Point_2>)
    .def("oriented_side", &bso2::oriented_side<Gps2>)
    .def("oriented_side", &bso2::oriented_side<Traits::Polygon_2>)
    .def("oriented_side", &bso2::oriented_side<Traits::Polygon_with_holes_2>)
    ;
}
