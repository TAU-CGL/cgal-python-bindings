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
#include "CGALPY/boolean_set_operations_2_types.hpp"
#include "CGALPY/export_general_polygon_2.hpp"
#include "CGALPY/export_general_polygon_with_holes_2.hpp"

namespace bp = boost::python;

namespace bso2 {

void complement0(General_polygon_2& pgn, General_polygon_with_holes_2& res)
{ CGAL::complement(pgn, res); }

void complement1(General_polygon_with_holes_2& pgn, bp::list& lst) {
  auto v = std::vector<General_polygon_with_holes_2>();
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
  auto v = std::vector<General_polygon_with_holes_2>();
  CGAL::intersection(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

template<typename T0, typename T1>
void intersection_range(bp::list& polygon_lst, bp::list& pwh_lst, bp::list& lst)
{
  auto begin0 = bp::stl_input_iterator<T0>(polygon_lst);
  auto end0 = bp::stl_input_iterator<T0>();
  auto begin1 = bp::stl_input_iterator<T1>(pwh_lst);
  auto end1 = bp::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::intersection(v0.begin(), v0.end(), v1.begin(), v1.end(),
                     std::back_inserter(res));
  for (auto p : res) lst.append(p);
}

template <typename T0, typename T1>
void difference_linear(T0& p0, T1& p1, bp::list& lst) {
  auto v = std::vector<General_polygon_with_holes_2>();
  CGAL::difference(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

template <typename T0, typename T1>
bool join_linear(T0& p0, T1& p1, General_polygon_with_holes_2& pwh)
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
  auto v = std::vector<General_polygon_with_holes_2>();
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

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
void connect_holes(General_polygon_with_holes_2& pwh, bp::list& lst) {
  auto v = std::vector<Geometry_traits_2::Point_2>();
  CGAL::connect_holes(pwh, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}
#endif

}

void export_boolean_set_operations_2() {
  typedef bso2::General_polygon_2               Pgn;
  typedef bso2::General_polygon_with_holes_2    Pwh;

  bp::scope bso2_scope = bp::scope();

  bp::def("complement", bso2::complement0);
  bp::def("complement", bso2::complement1);
  bp::def("do_intersect", &bso2::do_intersect<Pgn, Pgn>);
  bp::def("do_intersect", &bso2::do_intersect<Pgn, Pwh>);
  bp::def("do_intersect", &bso2::do_intersect<Pwh, Pgn>);
  bp::def("do_intersect", &bso2::do_intersect<Pwh, Pwh>);
  bp::def("do_intersect", &bso2::do_intersect_range<Pgn, Pwh>);
  bp::def("intersection", &bso2::intersection_linear<Pgn, Pgn>);
  bp::def("intersection", &bso2::intersection_linear<Pgn, Pwh>);
  bp::def("intersection", &bso2::intersection_linear<Pwh, Pgn>);
  bp::def("intersection", &bso2::intersection_linear<Pwh, Pwh>);
  bp::def("intersection", &bso2::intersection_range<Pgn, Pwh>);
  bp::def("difference", &bso2::difference_linear<Pgn, Pgn>);
  bp::def("difference", &bso2::difference_linear<Pgn, Pwh>);
  bp::def("difference", &bso2::difference_linear<Pwh, Pgn>);
  bp::def("difference", &bso2::difference_linear<Pwh, Pwh>);
  bp::def("join", &bso2::join_linear<Pgn, Pgn>);
  bp::def("join", &bso2::join_linear<Pwh, Pgn>);
  bp::def("join", &bso2::join_linear<Pgn, Pwh>);
  bp::def("join", &bso2::join_linear<Pwh, Pwh>);
  bp::def("join", &bso2::join_range<Pgn, Pwh>);
  bp::def("symmetric_difference", &bso2::symmetric_difference_linear<Pgn, Pgn>);
  bp::def("symmetric_difference", &bso2::symmetric_difference_linear<Pgn, Pwh>);
  bp::def("symmetric_difference", &bso2::symmetric_difference_linear<Pwh, Pgn>);
  bp::def("symmetric_difference", &bso2::symmetric_difference_linear<Pwh, Pwh>);
  bp::def("symmetric_difference", &bso2::symmetric_difference_range<Pgn, Pwh>);
  bp::def("oriented_side", &bso2::oriented_side<Pgn, Pgn>);
  bp::def("oriented_side", &bso2::oriented_side<Pgn, Pwh>);
  bp::def("oriented_side", &bso2::oriented_side<Pwh, Pgn>);
  bp::def("oriented_side", &bso2::oriented_side<Pwh, Pwh>);

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  bp::def("connect_holes", &bso2::connect_holes);
#else
  static const char pgn[] = "General_polygon_2";
  bp::class_<Pgn>* co_pgn(nullptr);
  export_general_polygon_2<Pgn, pgn>(bso2_scope, co_pgn);

  static const char pwh[] = "General_polygon_with_holes_2";
  bp::class_<Pwh>* co_pwh(nullptr);
  export_general_polygon_with_holes_2<Pwh, pwh>(bso2_scope, co_pwh);
#endif
}
