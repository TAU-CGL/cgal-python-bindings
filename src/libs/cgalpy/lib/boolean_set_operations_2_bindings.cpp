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

// template<typename T0, typename T1>
// bool do_intersect_range(py::list& polygon_lst, py::list& pwh_lst) {
//   auto begin0 = py::stl_input_iterator<T0>(polygon_lst);
//   auto end0 = py::stl_input_iterator<T0>();
//   auto begin1 = py::stl_input_iterator<T1>(pwh_lst);
//   auto end1 = py::stl_input_iterator<T1>();
//   auto v0 = std::vector<T0>(begin0, end0);
//   auto v1 = std::vector<T1>(begin1, end1);
//   return do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
// }

template <typename T0, typename T1>
py::list intersection_linear(T0& p0, T1& p1) {
  py::list lst;
  CGAL::intersection(p0, p1, append_iterator(lst));
  return lst;
}

// template<typename T0, typename T1>
// void intersection_range(py::list& polygon_lst, py::list& pwh_lst, py::list& lst)
// {
//   auto begin0 = py::stl_input_iterator<T0>(polygon_lst);
//   auto end0 = py::stl_input_iterator<T0>();
//   auto begin1 = py::stl_input_iterator<T1>(pwh_lst);
//   auto end1 = py::stl_input_iterator<T1>();
//   auto v0 = std::vector<T0>(begin0, end0);
//   auto v1 = std::vector<T1>(begin1, end1);
//   auto res = std::vector<T1>();
//   CGAL::intersection(v0.begin(), v0.end(), v1.begin(), v1.end(),
//                      std::back_inserter(res));
//   for (auto p : res) lst.append(p);
// }

template <typename T0, typename T1>
void difference_linear(T0& p0, T1& p1, py::list& lst) {
  auto v = std::vector<General_polygon_with_holes_2>();
  CGAL::difference(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

template <typename T0, typename T1>
bool join_linear(T0& p0, T1& p1, General_polygon_with_holes_2& pwh)
{ return CGAL::join(p0, p1, pwh); }

// template<typename T0, typename T1>
// void join_range(py::list& polygon_lst, py::list& pwh_lst, py::list& lst) {
//   auto begin0 = py::stl_input_iterator<T0>(polygon_lst);
//   auto end0 = py::stl_input_iterator<T0>();
//   auto begin1 = py::stl_input_iterator<T1>(pwh_lst);
//   auto end1 = py::stl_input_iterator<T1>();
//   auto v0 = std::vector<T0>(begin0, end0);
//   auto v1 = std::vector<T1>(begin1, end1);
//   auto res = std::vector<T1>();
//   CGAL::join(v0.begin(), v0.end(), v1.begin(), v1.end(),
//              std::back_inserter(res));
//   for (auto p : res) lst.append(p);
// }

template <typename T0, typename T1>
void symmetric_difference_linear(T0& p0, T1& p1, py::list& lst) {
  auto v = std::vector<General_polygon_with_holes_2>();
  CGAL::symmetric_difference(p0, p1, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

// template<typename T0, typename T1>
// void symmetric_difference_range(py::list& polygon_lst, py::list& pwh_lst,
//                                 py::list& lst) {
//   auto begin0 = py::stl_input_iterator<T0>(polygon_lst);
//   auto end0 = py::stl_input_iterator<T0>();
//   auto begin1 = py::stl_input_iterator<T1>(pwh_lst);
//   auto end1 = py::stl_input_iterator<T1>();
//   auto v0 = std::vector<T0>(begin0, end0);
//   auto v1 = std::vector<T1>(begin1, end1);
//   auto res = std::vector<T1>();
//   CGAL::symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end(),
//                              std::back_inserter(res));
//   for (auto p : res) lst.append(p);
// }

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

void export_boolean_set_operations_2(py::module_& m) {
  using Pgn = bso2::General_polygon_2;
  using Pwh = bso2::General_polygon_with_holes_2;

  m.def("complement", bso2::complement0);
  m.def("complement", bso2::complement1);
  m.def("do_intersect", &bso2::do_intersect<Pgn, Pgn>);
  m.def("do_intersect", &bso2::do_intersect<Pgn, Pwh>);
  m.def("do_intersect", &bso2::do_intersect<Pwh, Pgn>);
  m.def("do_intersect", &bso2::do_intersect<Pwh, Pwh>);
  // m.def("do_intersect", &bso2::do_intersect_range<Pgn, Pwh>); NB
  m.def("intersection", &bso2::intersection_linear<Pgn, Pgn>);
  m.def("intersection", &bso2::intersection_linear<Pgn, Pwh>);
  m.def("intersection", &bso2::intersection_linear<Pwh, Pgn>);
  m.def("intersection", &bso2::intersection_linear<Pwh, Pwh>);
  // m.def("intersection", &bso2::intersection_range<Pgn, Pwh>); NB
  m.def("difference", &bso2::difference_linear<Pgn, Pgn>);
  m.def("difference", &bso2::difference_linear<Pgn, Pwh>);
  m.def("difference", &bso2::difference_linear<Pwh, Pgn>);
  m.def("difference", &bso2::difference_linear<Pwh, Pwh>);
  m.def("join", &bso2::join_linear<Pgn, Pgn>);
  m.def("join", &bso2::join_linear<Pwh, Pgn>);
  m.def("join", &bso2::join_linear<Pgn, Pwh>);
  m.def("join", &bso2::join_linear<Pwh, Pwh>);
  // m.def("join", &bso2::join_range<Pgn, Pwh>); NB
  m.def("symmetric_difference", &bso2::symmetric_difference_linear<Pgn, Pgn>);
  m.def("symmetric_difference", &bso2::symmetric_difference_linear<Pgn, Pwh>);
  m.def("symmetric_difference", &bso2::symmetric_difference_linear<Pwh, Pgn>);
  m.def("symmetric_difference", &bso2::symmetric_difference_linear<Pwh, Pwh>);
  // m.def("symmetric_difference", &bso2::symmetric_difference_range<Pgn, Pwh>); NB
  m.def("oriented_side", &bso2::oriented_side<Pgn, Pgn>);
  m.def("oriented_side", &bso2::oriented_side<Pgn, Pwh>);
  m.def("oriented_side", &bso2::oriented_side<Pwh, Pgn>);
  m.def("oriented_side", &bso2::oriented_side<Pwh, Pwh>);

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  m.def("connect_holes", &bso2::connect_holes);
#else

  if (add_attr<Pgn>("General_polygon_2", m)) return;
  auto cs_pgn_c = py::class_<Pgn>(m, "General_polygon_2");
  export_general_polygon_2<Pgn>(cs_pgn_c);

  if (add_attr<Pwh>("General_polygon_with_holes_2", m)) return;
  auto cs_pwh_c = py::class_<Pwh>(m, "General_polygon_with_holes_2");
  export_general_polygon_with_holes_2<Pwh>(cs_pwh_c);

#endif
}
