// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSBASICTRAITS_HPP
#define CGALPY_EXPORT_AOSBASICTRAITS_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/add_class_object.hpp"

namespace py = nanobind;

//
template <typename T, typename C, typename Classes>
void export_Compare_y_at_x_left_2(C& c, Classes& classes, CGAL::Tag_true) {
  using Compare_y_at_x_left_2 = typename T::Compare_y_at_x_left_2;

  static const char compare_y_at_x_left_2[] = "Compare_y_at_x_left_2";
  if (add_class_object<Compare_y_at_x_left_2, compare_y_at_x_left_2>
    (c, classes.m_compare_y_at_x_left_2)) {
    classes.m_compare_y_at_x_left_2->
      def("__call__", &Compare_y_at_x_left_2::operator());
  }

  c.def("compare_y_at_x_left_2_object", &T::compare_y_at_x_left_2_object);
}

//
template <typename T, typename C, typename Classes>
void export_Compare_y_at_x_left_2(C& c, Classes& classes, CGAL::Tag_false) {}

//! Export the basic traits function objects
template <typename T, typename C, typename Concepts>
void export_AosBasicTraits(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  using Has_left_category = typename T::Has_left_category;
  using Point_2 = typename T::Point_2;
  using X_monotone_curve_2 = typename T::X_monotone_curve_2;
  using Compare_x_2 = typename T::Compare_x_2;
  using Compare_xy_2 = typename T::Compare_xy_2;
  using Equal_2 = typename T::Equal_2;
  using Construct_min_vertex_2 = typename T::Construct_min_vertex_2;
  using Construct_max_vertex_2 = typename T::Construct_max_vertex_2;
  using Is_vertical_2 = typename T::Is_vertical_2;
  using Compare_y_at_x_2 = typename T::Compare_y_at_x_2;
  using Compare_y_at_x_right_2 = typename T::Compare_y_at_x_right_2;

  auto& classes = concepts.m_basic_traits_classes;

  // Point_2
  static const char point_2[] = "Point_2";
  if (add_class_object<Point_2, point_2>(c, classes.m_point_2)) {
    classes.m_point_2->def(py::init<>());
    classes.m_point_2->def(py::init<const Point_2&>());
  }

  // X_monotone_curve_2
  static const char x_monotone_curve_2[] = "X_monotone_curve_2";
  if (add_class_object<X_monotone_curve_2, x_monotone_curve_2>
    (c, classes.m_x_monotone_curve_2)) {
    classes.m_x_monotone_curve_2->def(py::init<>());
    classes.m_x_monotone_curve_2->def(py::init<const X_monotone_curve_2&>());
  }

  // Compare_x_2
  static const char compare_x_2[] = "Compare_x_2";
  if (add_class_object<Compare_x_2, compare_x_2>(c, classes.m_compare_x_2)) {
    classes.m_compare_x_2->def("__call__", &Compare_x_2::operator());
  }

  // Compare_xy_2
  static const char compare_xy_2[] = "Compare_xy_2";
  if (add_class_object<Compare_xy_2, compare_x_2>(c, classes.m_compare_xy_2)) {
    classes.m_compare_xy_2->def("__call__", &Compare_xy_2::operator());
  }

  // Construct_min_vertex_2
  static const char construct_min_vertex_2[] = "Construct_min_vertex_2";
  if (add_class_object<Construct_min_vertex_2, construct_min_vertex_2>
    (c, classes.m_construct_min_vertex_2)) {
    classes.m_construct_min_vertex_2->
      def("__call__", &Construct_min_vertex_2::operator());
  }

  // Construct_max_vertex_2
  static const char construct_max_vertex_2[] = "Construct_max_vertex_2";
  if (add_class_object<Construct_max_vertex_2, construct_max_vertex_2>
    (c, classes.m_construct_max_vertex_2)) {
    classes.m_construct_max_vertex_2->
      def("__call__", &Construct_max_vertex_2::operator());
  }

  // Is_vertical_2
  static const char is_vertical_2[] = "Is_vertical_2";
  if (add_class_object<Is_vertical_2, is_vertical_2>
      (c, classes.m_is_vertical_2)) {
    classes.m_is_vertical_2->def("__call__", &Is_vertical_2::operator());
  }

  // Compare_y_at_x_2
  static const char compare_y_at_x_2[] = "Compare_y_at_x_2";
  if (add_class_object<Compare_y_at_x_2, compare_y_at_x_2>
    (c, classes.m_compare_y_at_x_2)) {
    classes.m_compare_y_at_x_2->def("__call__", &Compare_y_at_x_2::operator());
  }

  // Compare_y_at_x_right_2
  static const char compare_y_at_x_right_2[] = "Compare_y_at_x_right_2";
  if (add_class_object<Compare_y_at_x_right_2, compare_y_at_x_right_2>
    (c, classes.m_compare_y_at_x_right_2)) {
    classes.m_compare_y_at_x_right_2->
      def("__call__", &Compare_y_at_x_right_2::operator());
  }

  // Equal_2
  static const char equal_2[] = "Equal_2";
  if (add_class_object<Equal_2, equal_2>(c, classes.m_equal_2)) {
    classes.m_equal_2->def("__call__",
                           py::overload_cast<const Point_2&, const Point_2&>
                           (&Equal_2::operator(), py::const_));
    classes.m_equal_2->def("__call__",
                           py::overload_cast<const X_monotone_curve_2&,
                                             const X_monotone_curve_2&>
                           (&Equal_2::operator(), py::const_));
  }

  c.def(py::init<>())
    .def(py::init<T&>())
    .def("compare_x_2_object", &T::compare_x_2_object)
    .def("compare_xy_2_object", &T::compare_xy_2_object)
    .def("construct_min_vertex_2_object", &T::construct_min_vertex_2_object)
    .def("construct_max_vertex_2_object", &T::construct_max_vertex_2_object)
    .def("is_vertical_2_object", &T::is_vertical_2_object)
    .def("compare_y_at_x_2_object", &T::compare_y_at_x_2_object)
    .def("compare_y_at_x_right_2_object", &T::compare_y_at_x_right_2_object)
    .def("equal_2_object", &T::equal_2_object)
    ;

  export_Compare_y_at_x_left_2<T>(c, classes.m_compare_y_at_x_left_2_class,
                                  Has_left_category());

  exported = true;
}

#endif
