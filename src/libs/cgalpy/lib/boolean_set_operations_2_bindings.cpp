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
#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace bso2 {

//
General_polygon_with_holes_2 complement0(General_polygon_2& pgn) {
  General_polygon_with_holes_2 res;
  CGAL::complement(pgn, res);
  return res;
}

//
py::list complement1(General_polygon_with_holes_2& pgn) {
  using Gpwh = General_polygon_with_holes_2;
  py::list res;
  auto op = [&] (const Gpwh& gpwh) mutable { res.append(gpwh); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::complement(pgn, it);
  return res;
}

//
template <typename T0, typename T1>
bool do_intersect(T0& p0, T1& p1) { return CGAL::do_intersect(p0, p1); }

//
template<typename T0, typename T1>
bool do_intersect_range(py::list& pgn_lst, py::list& pwh_lst) {
  auto begin0 = stl_input_iterator<T0>(pgn_lst);
  auto end0 = stl_input_iterator<T0>(pgn_lst, false);
  auto begin1 = stl_input_iterator<T1>(pwh_lst);
  auto end1 = stl_input_iterator<T1>(pwh_lst, false);
  return do_intersect(begin0, end0, begin1, end1);
}

//
template <typename T0, typename T1>
py::list intersection_linear(T0& p0, T1& p1) {
  py::list lst;
  CGAL::intersection(p0, p1, append_iterator(lst));
  return lst;
}

  template <typename T0, typename T1, typename Traits>
  py::list intersection_linear(T0& p0, T1& p1, const Traits& traits) {
  py::list lst;
  CGAL::intersection(p0, p1, append_iterator(lst), traits);
  return lst;
}

//
template<typename T0, typename T1>
py::list intersection_range(py::list& pgn_lst, py::list& pwh_lst) {
  py::list res;
  auto op = [&] (const T1& gpwh) mutable { res.append(gpwh); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin0 = stl_input_iterator<T0>(pgn_lst);
  auto end0 = stl_input_iterator<T0>(pgn_lst, false);
  auto begin1 = stl_input_iterator<T1>(pwh_lst);
  auto end1 = stl_input_iterator<T1>(pwh_lst, false);
  CGAL::intersection(begin0, end0, begin1, end1, it);
  return res;
}

//
template <typename T0, typename T1>
py::list difference_linear(T0& p0, T1& p1) {
  using Gpwh = General_polygon_with_holes_2;
  py::list res;
  auto op = [&] (const Gpwh& gpwh) mutable { res.append(gpwh); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::difference(p0, p1, it);
  return res;
}

// xxx
template <typename T0, typename T1>
bool join_linear(T0& p0, T1& p1, General_polygon_with_holes_2& pwh)
{ return CGAL::join(p0, p1, pwh); }

//
template<typename T0, typename T1>
py::list join_range(py::list& pgn_lst, py::list& pwh_lst) {
  py::list res;
  auto op = [&] (const T1& gpwh) mutable { res.append(gpwh); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin0 = stl_input_iterator<T0>(pgn_lst);
  auto end0 = stl_input_iterator<T0>(pgn_lst, false);
  auto begin1 = stl_input_iterator<T1>(pwh_lst);
  auto end1 = stl_input_iterator<T1>(pwh_lst, false);
  CGAL::join(begin0, end0, begin1, end1, it);
  return res;
}

//
template <typename T0, typename T1>
py::list symmetric_difference_linear(T0& p0, T1& p1) {
  using Gpwh = General_polygon_with_holes_2;
  py::list res;
  auto op = [&] (const Gpwh& gpwh) mutable { res.append(gpwh); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::symmetric_difference(p0, p1, it);
  return res;
}

//
template<typename T0, typename T1>
py::list symmetric_difference_range(py::list& pgn_lst, py::list& pwh_lst) {
  using Gpwh = General_polygon_with_holes_2;
  py::list res;
  auto op = [&] (const Gpwh& gpwh) mutable { res.append(gpwh); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin0 = stl_input_iterator<T0>(pgn_lst);
  auto end0 = stl_input_iterator<T0>(pgn_lst, false);
  auto begin1 = stl_input_iterator<T1>(pwh_lst);
  auto end1 = stl_input_iterator<T1>(pwh_lst, false);
  CGAL::symmetric_difference(begin0, end0, begin1, end1, it);
  return res;
}

//
template <typename T0, typename T1>
CGAL::Oriented_side oriented_side(T0& p0, T1& p1)
{ return CGAL::oriented_side(p0, p1); }

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
//
py::list connect_holes(General_polygon_with_holes_2& pwh) {
  using Pnt = Geometry_traits_2::Point_2;
  py::list res;
  auto op = [&] (const Pnt& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::connect_holes(pwh, it);
  return res;
}
#endif

}

//
void export_boolean_set_operations_2(py::module_& m) {
  using Gt = bso2::Geometry_traits_2;
  using Pnt = Gt::Point_2;
  using Pgn = bso2::General_polygon_2;
  using Pwh = bso2::General_polygon_with_holes_2;

  m.def("complement", bso2::complement0);
  m.def("complement", bso2::complement1);
  m.def("do_intersect", &bso2::do_intersect<Pgn, Pgn>);
  m.def("do_intersect", &bso2::do_intersect<Pgn, Pwh>);
  m.def("do_intersect", &bso2::do_intersect<Pwh, Pgn>);
  m.def("do_intersect", &bso2::do_intersect<Pwh, Pwh>);
  m.def("do_intersect", &bso2::do_intersect_range<Pgn, Pwh>);
  m.def("intersection", &bso2::intersection_linear<Pgn, Pgn>);
  m.def("intersection", &bso2::intersection_linear<Pgn, Pwh>);
  m.def("intersection", &bso2::intersection_linear<Pwh, Pgn>);
  m.def("intersection", &bso2::intersection_linear<Pwh, Pwh>);
  m.def("intersection", &bso2::intersection_linear<Pgn, Pgn, Gt>);
  m.def("intersection", &bso2::intersection_linear<Pgn, Pwh, Gt>);
  m.def("intersection", &bso2::intersection_linear<Pwh, Pgn, Gt>);
  m.def("intersection", &bso2::intersection_linear<Pwh, Pwh, Gt>);
  m.def("intersection", &bso2::intersection_range<Pgn, Pwh>);
  m.def("difference", &bso2::difference_linear<Pgn, Pgn>);
  m.def("difference", &bso2::difference_linear<Pgn, Pwh>);
  m.def("difference", &bso2::difference_linear<Pwh, Pgn>);
  m.def("difference", &bso2::difference_linear<Pwh, Pwh>);
  m.def("join", &bso2::join_linear<Pgn, Pgn>);
  m.def("join", &bso2::join_linear<Pwh, Pgn>);
  m.def("join", &bso2::join_linear<Pgn, Pwh>);
  m.def("join", &bso2::join_linear<Pwh, Pwh>);
  m.def("join", &bso2::join_range<Pgn, Pwh>);
  m.def("symmetric_difference", &bso2::symmetric_difference_linear<Pgn, Pgn>);
  m.def("symmetric_difference", &bso2::symmetric_difference_linear<Pgn, Pwh>);
  m.def("symmetric_difference", &bso2::symmetric_difference_linear<Pwh, Pgn>);
  m.def("symmetric_difference", &bso2::symmetric_difference_linear<Pwh, Pwh>);
  m.def("symmetric_difference", &bso2::symmetric_difference_range<Pgn, Pwh>);
  m.def("oriented_side", &bso2::oriented_side<Pnt, Pgn>);
  m.def("oriented_side", &bso2::oriented_side<Pnt, Pwh>);
  m.def("oriented_side", &bso2::oriented_side<Pgn, Pgn>);
  m.def("oriented_side", &bso2::oriented_side<Pgn, Pwh>);
  m.def("oriented_side", &bso2::oriented_side<Pwh, Pgn>);
  m.def("oriented_side", &bso2::oriented_side<Pwh, Pwh>);

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  m.def("connect_holes", &bso2::connect_holes);
#else

  if (! add_attr<Pgn>(m, "General_polygon_2")) {
    auto cs_pgn_c = py::class_<Pgn>(m, "General_polygon_2");
    export_general_polygon_2<Pgn>(cs_pgn_c);
  }

  if (! add_attr<Pwh>(m, "General_polygon_with_holes_2")) {
    auto cs_pwh_c = py::class_<Pwh>(m, "General_polygon_with_holes_2");
    export_general_polygon_with_holes_2<Pwh>(cs_pwh_c);
  }

#endif
}
