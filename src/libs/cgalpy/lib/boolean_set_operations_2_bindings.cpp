// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>


#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <vector>

#include <boost/python.hpp>

#include <CGAL/connect_holes.h>
#include <CGAL/Boolean_set_operations_2.h>

#include "CGALPY/boolean_set_operations_2_config.hpp"
#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/polygon_set_2_types.hpp"

namespace bp = boost::python;

void complement0(Polygon_2& pgn, Polygon_with_holes_2& res)
{ CGAL::complement(pgn, res); }

void complement1(Polygon_with_holes_2& pgn, bp::list& lst) {
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::complement(pgn, std::back_inserter(v));
  for (auto p : v) { lst.append(p); }
}

template <typename T0, typename T1>
bool do_intersect(T0& p0, T1& p1)
{ return CGAL::do_intersect(p0, p1); }

template<typename T0, typename T1>
bool do_intersect_range(bp::list& polygon_lst, bp::list& pwh_lst) {
  auto begin0 = bp::stl_input_iterator<T0>(polygon_lst);
  auto end0 = bp::stl_input_iterator<T0>();
  auto begin1 = bp::stl_input_iterator<T1>(pwh_lst);
  auto end1 = bp::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  return do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T0, typename T1>
void intersection_linear(T0& p0, T1& p1, bp::list& lst) {
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::intersection(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

template<typename T0, typename T1>
void intersection_range(bp::list& polygon_lst, bp::list& pwh_lst, bp::list& lst) {
  auto begin0 = bp::stl_input_iterator<T0>(polygon_lst);
  auto end0 = bp::stl_input_iterator<T0>();
  auto begin1 = bp::stl_input_iterator<T1>(pwh_lst);
  auto end1 = bp::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::intersection(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(res));
  for (auto p : res) lst.append(p);
}

template <typename T0, typename T1>
void difference_linear(T0& p0, T1& p1, bp::list& lst) {
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::difference(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

template <typename T0, typename T1>
bool join_linear(T0& p0, T1& p1, Polygon_with_holes_2& pwh)
{ return CGAL::join(p0, p1, pwh); }

template<typename T0, typename T1>
void join_range(bp::list& polygon_lst, bp::list& pwh_lst, bp::list& lst) {
  auto begin0 = bp::stl_input_iterator<T0>(polygon_lst);
  auto end0 = bp::stl_input_iterator<T0>();
  auto begin1 = bp::stl_input_iterator<T1>(pwh_lst);
  auto end1 = bp::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::join(v0.begin(), v0.end(), v1.begin(), v1.end(),
             std::back_inserter(res));
  for (auto p : res) lst.append(p);
}

template <typename T0, typename T1>
void symmetric_difference_linear(T0& p0, T1& p1, bp::list& lst) {
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::symmetric_difference(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

template<typename T0, typename T1>
void symmetric_difference_range(bp::list& polygon_lst, bp::list& pwh_lst,
                                bp::list& lst) {
  auto begin0 = bp::stl_input_iterator<T0>(polygon_lst);
  auto end0 = bp::stl_input_iterator<T0>();
  auto begin1 = bp::stl_input_iterator<T1>(pwh_lst);
  auto end1 = bp::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end(),
                             std::back_inserter(res));
  for (auto p : res) lst.append(p);
}

template <typename T0, typename T1>
CGAL::Oriented_side oriented_side(T0& p0, T1& p1)
{ return CGAL::oriented_side(p0, p1); }

void connect_holes(Polygon_with_holes_2& pwh, bp::list& lst) {
  auto v = std::vector<Point_2>();
  CGAL::connect_holes(pwh, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

void export_boolean_set_operations_2() {
  bp::def("complement", complement0);
  bp::def("complement", complement1);
  bp::def("do_intersect", &do_intersect<Polygon_2, Polygon_2>);
  bp::def("do_intersect", &do_intersect<Polygon_2, Polygon_with_holes_2>);
  bp::def("do_intersect", &do_intersect<Polygon_with_holes_2, Polygon_2>);
  bp::def("do_intersect", &do_intersect<Polygon_with_holes_2, Polygon_with_holes_2>);
  bp::def("do_intersect", &do_intersect_range<Polygon_2, Polygon_with_holes_2>);
  bp::def("intersection", &intersection_linear<Polygon_2, Polygon_2>);
  bp::def("intersection", &intersection_linear<Polygon_2, Polygon_with_holes_2>);
  bp::def("intersection", &intersection_linear<Polygon_with_holes_2, Polygon_2>);
  bp::def("intersection", &intersection_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  bp::def("intersection", &intersection_range<Polygon_2, Polygon_with_holes_2>);
  bp::def("difference", &difference_linear<Polygon_2, Polygon_2>);
  bp::def("difference", &difference_linear<Polygon_2, Polygon_with_holes_2>);
  bp::def("difference", &difference_linear<Polygon_with_holes_2, Polygon_2>);
  bp::def("difference", &difference_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  bp::def("join", &join_linear<Polygon_2, Polygon_2>);
  bp::def("join", &join_linear<Polygon_with_holes_2, Polygon_2>);
  bp::def("join", &join_linear<Polygon_2, Polygon_with_holes_2>);
  bp::def("join", &join_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  bp::def("join", &join_range<Polygon_2, Polygon_with_holes_2>);
  bp::def("symmetric_difference", &symmetric_difference_linear<Polygon_2, Polygon_2>);
  bp::def("symmetric_difference", &symmetric_difference_linear<Polygon_2, Polygon_with_holes_2>);
  bp::def("symmetric_difference", &symmetric_difference_linear<Polygon_with_holes_2, Polygon_2>);
  bp::def("symmetric_difference", &symmetric_difference_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  bp::def("symmetric_difference", &symmetric_difference_range<Polygon_2, Polygon_with_holes_2>);
  bp::def("oriented_side", &oriented_side<Polygon_2, Polygon_2>);
  bp::def("oriented_side", &oriented_side<Polygon_2, Polygon_with_holes_2>);
  bp::def("oriented_side", &oriented_side<Polygon_with_holes_2, Polygon_2>);
  bp::def("oriented_side", &oriented_side<Polygon_with_holes_2, Polygon_with_holes_2>);
  bp::def("connect_holes", &connect_holes);
}
