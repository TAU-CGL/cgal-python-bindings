// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/python.hpp>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/polygon_set_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"
#include "CGALPY/add_attr.hpp"

namespace bp = boost::python;

namespace bso2 {

void polygons_with_holes(Polygon_set_2& ps, bp::list& lst) {
  auto v = std::vector<Polygon_with_holes_2>();
  ps.polygons_with_holes(std::back_inserter(v));
  for (auto pwh : v) lst.append(pwh);
}

void insert0(Polygon_set_2& ps, Polygon_2& pgn) { ps.insert(pgn); }

void insert1(Polygon_set_2& ps, Polygon_with_holes_2& pwh) { ps.insert(pwh); }

void insert_range0(Polygon_set_2& ps, bp::list& polygon_lst, bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);

  ps.insert(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void insert_range(Polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.insert(v.begin(), v.end());
}

void complement0(Polygon_set_2& ps0, Polygon_set_2& ps1) { ps0.complement(ps1); }

template <typename T>
void intersection(Polygon_set_2& ps, T& other) { ps.intersection(other); }

void intersection(Polygon_set_2& ps0, Polygon_set_2& ps1, Polygon_set_2& ps2)
{ ps0.intersection(ps1, ps2); }

void intersection_range0(Polygon_set_2& ps, bp::list& polygon_lst,
                         bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);
  ps.intersection(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void intersection_range(Polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.intersection(v.begin(), v.end());
}

template <typename T>
void join(Polygon_set_2& ps, T& other) { ps.join(other); }

void join(Polygon_set_2& ps0, Polygon_set_2& ps1, Polygon_set_2& ps2)
{ ps0.join(ps1, ps2); }

void join_range0(Polygon_set_2& ps, bp::list& polygon_lst,
                 bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);
  ps.join(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void join_range(Polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.join(v.begin(), v.end());
}

template <typename T>
void difference(Polygon_set_2& ps, T& other) { ps.difference(other); }

void difference(Polygon_set_2& ps0, Polygon_set_2& ps1, Polygon_set_2& ps2)
{ ps0.difference(ps1, ps2); }

template <typename T>
void symmetric_difference(Polygon_set_2& ps, T& other)
{ ps.symmetric_difference(other); }

void symmetric_difference(Polygon_set_2& ps0, Polygon_set_2& ps1,
                          Polygon_set_2& ps2)
{ ps0.symmetric_difference(ps1, ps2); }

void symmetric_difference_range0(Polygon_set_2& ps, bp::list& polygon_lst, bp::list& pwh_lst)
{
  auto begin0 = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);
  ps.symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void symmetric_difference_range(Polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.symmetric_difference(v.begin(), v.end());
}

template <typename T>
bool do_intersect(Polygon_set_2& ps, T& other)
{ return ps.do_intersect(other); }

bool do_intersect_range0(Polygon_set_2& ps, bp::list& polygon_lst,
                         bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = bp::stl_input_iterator<Polygon_2>();
  auto begin1 = bp::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = bp::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);
  return ps.do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void do_intersect_range(Polygon_set_2& ps, bp::list& lst) {
  auto begin = bp::stl_input_iterator<T>(lst);
  auto end = bp::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.do_intersect(v.begin(), v.end());
}

template<typename T>
CGAL::Oriented_side oriented_side(Polygon_set_2& ps, T& other)
{ return ps.oriented_side(other); }

}

void export_polygon_set_2() {
  typedef bso2::Polygon_set_2                               Ps2;

  bp::scope ps2_scope = bp::class_<Ps2>("Polygon_set_2")
    .def(bp::init<>())
    .def(bp::init<const Polygon_2&>())
    .def(bp::init<const Polygon_with_holes_2&>())
    .def(bp::init<const Ps2&>())
    .def("is_empty", &Ps2::is_empty)
    .def("is_plane", &Ps2::is_plane)
    .def("number_of_polygons_with_holes", &Ps2::number_of_polygons_with_holes)
    .def("polygons_with_holes", &bso2::polygons_with_holes)
    //.def("arrangement", &Ps2::arrangement)
    .def("clear", &Ps2::clear)
    .def("is_valid", &Ps2::is_valid)
    .def("insert", &bso2::insert0)
    .def("insert", &bso2::insert1)
    .def("insert", &bso2::insert_range0)
    .def("insert_polygons", &bso2::insert_range<Polygon_2>)
    .def("insert_polygons_with_holes", &bso2::insert_range<Polygon_with_holes_2>)
    .def<void (Ps2::*) ()>("complement", &Ps2::complement)
    .def("complement", &bso2::complement0)
    .def("intersection", &bso2::intersection<Ps2>)
    .def("intersection", &bso2::intersection<Polygon_2>)
    .def("intersection", &bso2::intersection<Polygon_with_holes_2>)
    .def<void (Ps2&, Ps2&, Ps2&)>("intersection", &bso2::intersection)
    .def("intersection", &bso2::intersection_range0)
    .def("intersection_polygons", &bso2::intersection_range<Polygon_2>)
    .def("intersection_polygons_with_holes", &bso2::intersection_range<Polygon_with_holes_2>)
    .def("join", &bso2::join<Ps2>)
    .def("join", &bso2::join<Polygon_2>)
    .def("join", &bso2::join<Polygon_with_holes_2>)
    .def<void (Ps2&, Ps2&, Ps2&)>("join", &bso2::join)
    .def("join", &bso2::join_range0)
    .def("join_polygons", &bso2::join_range<Polygon_2>)
    .def("join_polygons_with_holes", &bso2::join_range<Polygon_with_holes_2>)
    .def("difference", &bso2::difference<Ps2>)
    .def("difference", &bso2::difference<Polygon_2>)
    .def("difference", &bso2::difference<Polygon_with_holes_2>)
    .def<void (Ps2&, Ps2&, Ps2&)>("difference", &bso2::difference)
    .def("symmetric_difference", &bso2::symmetric_difference<Ps2>)
    .def("symmetric_difference", &bso2::symmetric_difference<Polygon_2>)
    .def("symmetric_difference", &bso2::symmetric_difference<Polygon_with_holes_2>)
    .def<void (Ps2&, Ps2&, Ps2&)>("symmetric_difference", &bso2::symmetric_difference)
    .def("symmetric_difference", &bso2::symmetric_difference_range0)
    .def("symmetric_difference_polygons", &bso2::symmetric_difference_range<Polygon_2>)
    .def("symmetric_difference_polygons_with_holes", &bso2::symmetric_difference_range<Polygon_with_holes_2>)
    .def("do_intersect", &bso2::do_intersect<Ps2>)
    .def("do_intersect", &bso2::do_intersect<Polygon_2>)
    .def("do_intersect", &bso2::do_intersect<Polygon_with_holes_2>)
    .def("do_intersect", &bso2::do_intersect_range0)
    .def("do_intersect_polygons", &bso2::do_intersect_range<Polygon_2>)
    .def("do_intersect_polygons_with_holes", &bso2::do_intersect_range<Polygon_with_holes_2>)
    .def("locate", &Ps2::locate)
    .def("oriented_side", &bso2::oriented_side<Point_2>)
    .def("oriented_side", &bso2::oriented_side<Ps2>)
    .def("oriented_side", &bso2::oriented_side<Polygon_2>)
    .def("oriented_side", &bso2::oriented_side<Polygon_with_holes_2>)
    ;

  // Types that have been registered already:
  add_attr<Ps2::Traits_2>("Traits_2", ps2_scope);
  add_attr<Ps2::Polygon_2>("Polygon_2", ps2_scope);
  add_attr<Ps2::Polygon_with_holes_2>("Polygon_with_holes_2", ps2_scope);
  add_attr<Ps2::Arrangement_2>("Arrangement_2", ps2_scope);
}
