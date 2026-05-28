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
#include "CGALPY/stl_forward_iterator.hpp"

#include "cgalpy/Bso2_docstrings.hpp"

namespace py = nanobind;
namespace bso2_doc = cgalpy::bso2::docstrings;

namespace cgalpy {
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
  auto begin0 = stl_forward_iterator<T0>(pgn_lst);
  auto end0 = stl_forward_iterator<T0>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<T1>(pwh_lst);
  auto end1 = stl_forward_iterator<T1>(pwh_lst, false);
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
  auto begin0 = stl_forward_iterator<T0>(pgn_lst);
  auto end0 = stl_forward_iterator<T0>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<T1>(pwh_lst);
  auto end1 = stl_forward_iterator<T1>(pwh_lst, false);
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
  auto begin0 = stl_forward_iterator<T0>(pgn_lst);
  auto end0 = stl_forward_iterator<T0>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<T1>(pwh_lst);
  auto end1 = stl_forward_iterator<T1>(pwh_lst, false);
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
  auto begin0 = stl_forward_iterator<T0>(pgn_lst);
  auto end0 = stl_forward_iterator<T0>(pgn_lst, false);
  auto begin1 = stl_forward_iterator<T1>(pwh_lst);
  auto end1 = stl_forward_iterator<T1>(pwh_lst, false);
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
} // namespace cgalpy

//
void export_boolean_set_operations_2(py::module_& m) {
  using Gt = cgalpy::bso2::Geometry_traits_2;
  using Pnt = Gt::Point_2;
  using Pgn = cgalpy::bso2::General_polygon_2;
  using Pwh = cgalpy::bso2::General_polygon_with_holes_2;

  m.def("complement", cgalpy::bso2::complement0,
        py::arg("polygon"), bso2_doc::complement_2);
  m.def("complement", cgalpy::bso2::complement1,
        py::arg("polygon_with_holes"), bso2_doc::complement_5);
  m.def("do_intersect", &cgalpy::bso2::do_intersect<Pgn, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::do_intersect_4);
  m.def("do_intersect", &cgalpy::bso2::do_intersect<Pgn, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::do_intersect_5);
  m.def("do_intersect", &cgalpy::bso2::do_intersect<Pwh, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::do_intersect_6);
  m.def("do_intersect", &cgalpy::bso2::do_intersect<Pwh, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::do_intersect_7);
  m.def("do_intersect", &cgalpy::bso2::do_intersect_range<Pgn, Pwh>,
        py::arg("polygons"), py::arg("polygons_with_holes"),
        bso2_doc::do_intersect_9);
  m.def("intersection", &cgalpy::bso2::intersection_linear<Pgn, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::intersection_8);
  m.def("intersection", &cgalpy::bso2::intersection_linear<Pgn, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::intersection_9);
  m.def("intersection", &cgalpy::bso2::intersection_linear<Pwh, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::intersection_10);
  m.def("intersection", &cgalpy::bso2::intersection_linear<Pwh, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::intersection_11);
  m.def("intersection", &cgalpy::bso2::intersection_linear<Pgn, Pgn, Gt>,
        py::arg("pgn1"), py::arg("pgn2"), py::arg("traits"),
        bso2_doc::intersection_20);
  m.def("intersection", &cgalpy::bso2::intersection_linear<Pgn, Pwh, Gt>,
        py::arg("pgn1"), py::arg("pgn2"), py::arg("traits"),
        bso2_doc::intersection_21);
  m.def("intersection", &cgalpy::bso2::intersection_linear<Pwh, Pgn, Gt>,
        py::arg("pgn1"), py::arg("pgn2"), py::arg("traits"),
        bso2_doc::intersection_22);
  m.def("intersection", &cgalpy::bso2::intersection_linear<Pwh, Pwh, Gt>,
        py::arg("pgn1"), py::arg("pgn2"), py::arg("traits"),
        bso2_doc::intersection_23);
  m.def("intersection", &cgalpy::bso2::intersection_range<Pgn, Pwh>,
        py::arg("polygons"), py::arg("polygons_with_holes"),
        bso2_doc::intersection_14);
  m.def("difference", &cgalpy::bso2::difference_linear<Pgn, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::difference_8);
  m.def("difference", &cgalpy::bso2::difference_linear<Pgn, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::difference_9);
  m.def("difference", &cgalpy::bso2::difference_linear<Pwh, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::difference_10);
  m.def("difference", &cgalpy::bso2::difference_linear<Pwh, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"), bso2_doc::difference_11);
  m.def("join", &cgalpy::bso2::join_linear<Pgn, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"), py::arg("result"),
        bso2_doc::join_8);
  m.def("join", &cgalpy::bso2::join_linear<Pwh, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"), py::arg("result"),
        bso2_doc::join_10);
  m.def("join", &cgalpy::bso2::join_linear<Pgn, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"), py::arg("result"),
        bso2_doc::join_9);
  m.def("join", &cgalpy::bso2::join_linear<Pwh, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"), py::arg("result"),
        bso2_doc::join_11);
  m.def("join", &cgalpy::bso2::join_range<Pgn, Pwh>,
        py::arg("polygons"), py::arg("polygons_with_holes"),
        bso2_doc::join_14);
  m.def("symmetric_difference", &cgalpy::bso2::symmetric_difference_linear<Pgn, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"),
        bso2_doc::symmetric_difference_8);
  m.def("symmetric_difference", &cgalpy::bso2::symmetric_difference_linear<Pgn, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"),
        bso2_doc::symmetric_difference_9);
  m.def("symmetric_difference", &cgalpy::bso2::symmetric_difference_linear<Pwh, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"),
        bso2_doc::symmetric_difference_10);
  m.def("symmetric_difference", &cgalpy::bso2::symmetric_difference_linear<Pwh, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"),
        bso2_doc::symmetric_difference_11);
  m.def("symmetric_difference", &cgalpy::bso2::symmetric_difference_range<Pgn, Pwh>,
        py::arg("polygons"), py::arg("polygons_with_holes"),
        bso2_doc::symmetric_difference_14);
  m.def("oriented_side", &cgalpy::bso2::oriented_side<Pnt, Pgn>,
        py::arg("point"), py::arg("polygon"),
        bso2_doc::oriented_side_22);
  m.def("oriented_side", &cgalpy::bso2::oriented_side<Pnt, Pwh>,
        py::arg("point"), py::arg("polygon_with_holes"),
        bso2_doc::oriented_side_23);
  m.def("oriented_side", &cgalpy::bso2::oriented_side<Pgn, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"),
        bso2_doc::oriented_side_8);
  m.def("oriented_side", &cgalpy::bso2::oriented_side<Pgn, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"),
        bso2_doc::oriented_side_9);
  m.def("oriented_side", &cgalpy::bso2::oriented_side<Pwh, Pgn>,
        py::arg("pgn1"), py::arg("pgn2"),
        bso2_doc::oriented_side_10);
  m.def("oriented_side", &cgalpy::bso2::oriented_side<Pwh, Pwh>,
        py::arg("pgn1"), py::arg("pgn2"),
        bso2_doc::oriented_side_11);

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  m.def("connect_holes", &cgalpy::bso2::connect_holes,
        py::arg("polygon_with_holes"), bso2_doc::connect_holes);
#else

  if (! add_attr<Pgn>(m, "General_polygon_2")) {
    auto cs_pgn_c = py::class_<Pgn>(
      m, "General_polygon_2", bso2_doc::GeneralPolygon_2_class);
    export_general_polygon_2<Pgn>(cs_pgn_c,
      bso2_doc::GeneralPolygon_2_GeneralPolygon_2,
      bso2_doc::GeneralPolygon_2_GeneralPolygon_2_1,
      bso2_doc::GeneralPolygon_2_init,
      bso2_doc::GeneralPolygon_2_curves_begin_1);
  }

  if (! add_attr<Pwh>(m, "General_polygon_with_holes_2")) {
    auto cs_pwh_c = py::class_<Pwh>(
      m, "General_polygon_with_holes_2",
      bso2_doc::GpsTraitsGeneralPolygonWithHoles_2_class);
    export_general_polygon_with_holes_2<Pwh>(cs_pwh_c);
  }

#endif
}
