// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <vector>

#include <nanobind/nanobind.h>

#include <CGAL/connect_holes.h>
#include <CGAL/Boolean_set_operations_2.h>

#include "CGALPY/boolean_set_operations_2_config.hpp"
#include "CGALPY/boolean_set_operations_2_types.hpp"
#include "CGALPY/export_general_polygon_2.hpp"
#include "CGALPY/export_general_polygon_with_holes_2.hpp"
#include "CGALPY/append_iterator.hpp"

namespace py = nanobind;

namespace bso2 {

void complement0(General_polygon_2& pgn, General_polygon_with_holes_2& res)
{ CGAL::complement(pgn, res); }

void complement1(General_polygon_with_holes_2& pgn, py::list& lst) {
  auto v = std::vector<General_polygon_with_holes_2>();
  CGAL::complement(pgn, std::back_inserter(v));
  for (auto p : v) { lst.append(p); }
}

template <typename T0, typename T1>
bool do_intersect(T0& p0, T1& p1)
{ return CGAL::do_intersect(p0, p1); }

template<typename T0, typename T1>
bool do_intersect_range(py::list& polygon_lst, py::list& pwh_lst) {
  auto begin0 = py::stl_input_iterator<T0>(polygon_lst);
  auto end0 = py::stl_input_iterator<T0>();
  auto begin1 = py::stl_input_iterator<T1>(pwh_lst);
  auto end1 = py::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  return do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T0, typename T1>
py::list intersection_linear(T0& p0, T1& p1) {
  py::list lst;
  CGAL::intersection(p0, p1, append_iterator(lst));
  return lst;
}

template<typename T0, typename T1>
void intersection_range(py::list& polygon_lst, py::list& pwh_lst, py::list& lst)
{
  auto begin0 = py::stl_input_iterator<T0>(polygon_lst);
  auto end0 = py::stl_input_iterator<T0>();
  auto begin1 = py::stl_input_iterator<T1>(pwh_lst);
  auto end1 = py::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::intersection(v0.begin(), v0.end(), v1.begin(), v1.end(),
                     std::back_inserter(res));
  for (auto p : res) lst.append(p);
}

template <typename T0, typename T1>
void difference_linear(T0& p0, T1& p1, py::list& lst) {
  auto v = std::vector<General_polygon_with_holes_2>();
  CGAL::difference(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

template <typename T0, typename T1>
bool join_linear(T0& p0, T1& p1, General_polygon_with_holes_2& pwh)
{ return CGAL::join(p0, p1, pwh); }

template<typename T0, typename T1>
void join_range(py::list& polygon_lst, py::list& pwh_lst, py::list& lst) {
  auto begin0 = py::stl_input_iterator<T0>(polygon_lst);
  auto end0 = py::stl_input_iterator<T0>();
  auto begin1 = py::stl_input_iterator<T1>(pwh_lst);
  auto end1 = py::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::join(v0.begin(), v0.end(), v1.begin(), v1.end(),
             std::back_inserter(res));
  for (auto p : res) lst.append(p);
}

template <typename T0, typename T1>
void symmetric_difference_linear(T0& p0, T1& p1, py::list& lst) {
  auto v = std::vector<General_polygon_with_holes_2>();
  CGAL::symmetric_difference(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

template<typename T0, typename T1>
void symmetric_difference_range(py::list& polygon_lst, py::list& pwh_lst,
                                py::list& lst) {
  auto begin0 = py::stl_input_iterator<T0>(polygon_lst);
  auto end0 = py::stl_input_iterator<T0>();
  auto begin1 = py::stl_input_iterator<T1>(pwh_lst);
  auto end1 = py::stl_input_iterator<T1>();
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
void connect_holes(General_polygon_with_holes_2& pwh, py::list& lst) {
  auto v = std::vector<Geometry_traits_2::Point_2>();
  CGAL::connect_holes(pwh, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}
#endif

}

void export_boolean_set_operations_2() {
  typedef bso2::General_polygon_2               Pgn;
  typedef bso2::General_polygon_with_holes_2    Pwh;

  py::scope bso2_scope = py::scope();

  py::def("complement", bso2::complement0);
  py::def("complement", bso2::complement1);
  py::def("do_intersect", &bso2::do_intersect<Pgn, Pgn>);
  py::def("do_intersect", &bso2::do_intersect<Pgn, Pwh>);
  py::def("do_intersect", &bso2::do_intersect<Pwh, Pgn>);
  py::def("do_intersect", &bso2::do_intersect<Pwh, Pwh>);
  py::def("do_intersect", &bso2::do_intersect_range<Pgn, Pwh>);
  py::def("intersection", &bso2::intersection_linear<Pgn, Pgn>);
  py::def("intersection", &bso2::intersection_linear<Pgn, Pwh>);
  py::def("intersection", &bso2::intersection_linear<Pwh, Pgn>);
  py::def("intersection", &bso2::intersection_linear<Pwh, Pwh>);
  py::def("intersection", &bso2::intersection_range<Pgn, Pwh>);
  py::def("difference", &bso2::difference_linear<Pgn, Pgn>);
  py::def("difference", &bso2::difference_linear<Pgn, Pwh>);
  py::def("difference", &bso2::difference_linear<Pwh, Pgn>);
  py::def("difference", &bso2::difference_linear<Pwh, Pwh>);
  py::def("join", &bso2::join_linear<Pgn, Pgn>);
  py::def("join", &bso2::join_linear<Pwh, Pgn>);
  py::def("join", &bso2::join_linear<Pgn, Pwh>);
  py::def("join", &bso2::join_linear<Pwh, Pwh>);
  py::def("join", &bso2::join_range<Pgn, Pwh>);
  py::def("symmetric_difference", &bso2::symmetric_difference_linear<Pgn, Pgn>);
  py::def("symmetric_difference", &bso2::symmetric_difference_linear<Pgn, Pwh>);
  py::def("symmetric_difference", &bso2::symmetric_difference_linear<Pwh, Pgn>);
  py::def("symmetric_difference", &bso2::symmetric_difference_linear<Pwh, Pwh>);
  py::def("symmetric_difference", &bso2::symmetric_difference_range<Pgn, Pwh>);
  py::def("oriented_side", &bso2::oriented_side<Pgn, Pgn>);
  py::def("oriented_side", &bso2::oriented_side<Pgn, Pwh>);
  py::def("oriented_side", &bso2::oriented_side<Pwh, Pgn>);
  py::def("oriented_side", &bso2::oriented_side<Pwh, Pwh>);

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  py::def("connect_holes", &bso2::connect_holes);
#else
  static const char pgn[] = "General_polygon_2";
  py::class_<Pgn>* co_pgn(nullptr);
  export_general_polygon_2<Pgn, pgn>(bso2_scope, co_pgn);

  static const char pwh[] = "General_polygon_with_holes_2";
  py::class_<Pwh>* co_pwh(nullptr);
  export_general_polygon_with_holes_2<Pwh, pwh>(bso2_scope, co_pwh);
#endif
}
