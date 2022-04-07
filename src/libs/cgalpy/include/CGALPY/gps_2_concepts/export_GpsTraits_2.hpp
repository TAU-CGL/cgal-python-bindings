// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_GPSTRAITS_2_HPP
#define CGALPY_EXPORT_GPSTRAITS_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"
#include "CGALPY/add_class_object.hpp"
#include "CGALPY/export_general_polygon_2.hpp"
#include "CGALPY/export_general_polygon_with_holes_2.hpp"

namespace py = nanobind;

namespace bso2 {

template <typename T>
//! This is completely messed up. The constructor should return a Polygon_2.
// If it is just for initializing, then it's redundant!
void export_ctr_gp_2_op(typename T::Construct_polygon_2 m,
                        py::list& curves, typename T::Polygon_2& pgn) {
  auto begin = py::stl_input_iterator<typename T::X_monotone_curve_2>(curves);
  auto end = py::stl_input_iterator<typename T::X_monotone_curve_2>();
  return m(begin, end, pgn);
}

template <typename T>
typename T::Polygon_with_holes_2
export_ctr_gpwh_2_op(typename T::Construct_general_polygon_with_holes_2 m,
                     const typename T::Polygon_2& boundary, py::list& holes) {
  auto begin = py::stl_input_iterator<typename T::Polygon_2>(holes);
  auto end = py::stl_input_iterator<typename T::Polygon_2>();
  return m(boundary, begin, end);
}

}

template <typename T, typename C, typename Concepts>
void export_GpsTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  typedef typename T::Polygon_2                         Polygon_2;
  typedef typename T::Polygon_with_holes_2              Polygon_with_holes_2;
  typedef typename T::Construct_polygon_2               Construct_polygon_2;
  typedef typename T::Construct_general_polygon_with_holes_2
    Construct_general_polygon_with_holes_2;
  typedef typename T::Construct_outer_boundary          Construct_outer_boundary;
  typedef typename T::Construct_holes                   Construct_holes;
  typedef typename T::Is_unbounded                      Is_unbounded;

  py::scope traits_scope(c);
  auto& classes = concepts.m_traits_classes;

  // Polygon_2
  static const char polygon_2[] = "Polygon_2";
  export_general_polygon_2<Polygon_2, polygon_2>(traits_scope,
                                                 classes.m_polygon_2);

  // Polygon_with_holes_2
  static const char polygon_with_holes_2[] = "Polygon_with_holes_2";
  export_general_polygon_with_holes_2<Polygon_with_holes_2,
                                      polygon_with_holes_2>
    (traits_scope, classes.m_polygon_with_holes_2);

  // Construct_polygon_2
  classes.m_construct_polygon_2 =
    new py::class_<Construct_polygon_2>("Construct_polygon_2", py::no_init);
  classes.m_construct_polygon_2->def("__call__", &bso2::export_ctr_gp_2_op<T>);

  // Construct_general_polygon_with_holes_2
  classes.m_construct_general_polygon_with_holes_2 =
    new py::class_<Construct_general_polygon_with_holes_2>
    ("Construct_general_polygon_with_holes_2", py::no_init);
  using Ctr_gpwh1 =
    Polygon_with_holes_2(Construct_general_polygon_with_holes_2::*)
    (const Polygon_2&) const;
  classes.m_construct_general_polygon_with_holes_2->
    def("__call__", static_cast<Ctr_gpwh1>
        (&Construct_general_polygon_with_holes_2::operator()));
  classes.m_construct_general_polygon_with_holes_2->
    def("__call__", &bso2::export_ctr_gpwh_2_op<T>);

  // Construct_outer_boundary
  classes.m_construct_outer_boundary =
    new py::class_<Construct_outer_boundary>
    ("Construct_outer_boundary", py::no_init);
  classes.m_construct_outer_boundary->
    def("__call__", &Construct_outer_boundary::operator());

  // Construct_holes
  classes.m_construct_holes =
    new py::class_<Construct_holes>("Construct_holes", py::no_init);
  classes.m_construct_holes->def("__call__", &Construct_holes::operator());

  // Is unbounded
  classes.m_is_unbounded =
    new py::class_<Is_unbounded>("Is_unbounded", py::no_init);
  classes.m_is_unbounded->def("__call__", &Is_unbounded::operator());

  // // The following is a typo. Should be instead
  // // c.def("is_unbounded_object", &T::is_unbounded_object);
  // c.def("is_unbounded_object", &T::construct_is_unbounded_object);

  exported = true;
}

#endif
