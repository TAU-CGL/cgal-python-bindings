// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSBASICTRAITS_HPP
#define CGALPY_EXPORT_AOSBASICTRAITS_HPP

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"

//
template <typename T, typename C>
void export_Compare_at_x_left_2(C c, CGAL::Tag_true) {
  bp::class_<typename T::Compare_y_at_x_left_2>("Compare_y_at_x_left_2", bp::no_init)
    .def("__call__", &T::Compare_y_at_x_left_2::operator())
    ;

  c.def("compare_y_at_x_left_2_object", &T::compare_y_at_x_left_2_object);
}

//
template <typename T, typename C>
void export_Compare_at_x_left_2(C c, CGAL::Tag_false) {}

// Introduce a new class object
template <typename Type, const char* Name, typename Class_, bp::no_init_t E = bp::no_init>
void add_class_object(Class_& co, bp::scope& my_scope) {
  bp::handle<> tco(bp::objects::registered_class_object(bp::type_id<Type>()));
  if (tco.get() != 0) my_scope.attr(Name) = tco;
  else co = new bp::class_<Type>(Name, E);
}

//! Export the basic traits function objects
template <typename T, typename RVP, typename C, typename Concepts>
void export_AosBasicTraits(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  typedef typename T::Has_left_category         Has_left_category;
  typedef typename T::Point_2                   Point_2;
  typedef typename T::X_monotone_curve_2        X_monotone_curve_2;
  typedef typename T::Compare_x_2               Compare_x_2;
  typedef typename T::Compare_xy_2              Compare_xy_2;
  typedef typename T::Equal_2                   Equal_2;
  typedef typename T::Construct_min_vertex_2    Construct_min_vertex_2;
  typedef typename T::Construct_max_vertex_2    Construct_max_vertex_2;
  typedef typename T::Is_vertical_2             Is_vertical_2;
  typedef typename T::Compare_y_at_x_2          Compare_y_at_x_2;
  typedef typename T::Compare_y_at_x_right_2    Compare_y_at_x_right_2;

  bp::scope traits_scope(c);
  auto& classes = concepts.m_basic_traits_classes;

  // Ponit_2
  bp::handle<> point_co(bp::objects::registered_class_object(bp::type_id<Point_2>()));
  if (point_co.get() != 0) traits_scope.attr("Point_2") = point_co;
  else {
    classes.m_point_2 = new bp::class_<Point_2>("Point_2");
    classes.m_point_2->def(bp::init<>());
    classes.m_point_2->def(bp::init<Point_2&>());
  }

  // X_monotone_curve_2
  bp::handle<> xcv_co(bp::objects::registered_class_object(bp::type_id<X_monotone_curve_2>()));
  if (xcv_co.get() != 0) traits_scope.attr("X_monotone_curve_2") = xcv_co;
  else {
    classes.m_x_monotone_curve_2 = new bp::class_<X_monotone_curve_2>("X_monotone_curve_2");
    classes.m_x_monotone_curve_2->def(bp::init<>());
    classes.m_x_monotone_curve_2->def(bp::init<X_monotone_curve_2&>());
  }

  // Compare_x_2
  static const char compare_x_2[] = "Compare_x_2";
  add_class_object<Compare_x_2, compare_x_2>(classes.m_compare_x_2, traits_scope);
  if (classes.m_compare_x_2) classes.m_compare_x_2->def("__call__", &Compare_x_2::operator());

  // Compare_xy_2
  static const char compare_xy_2[] = "Compare_xy_2";
  add_class_object<Compare_xy_2, compare_x_2>(classes.m_compare_xy_2, traits_scope);
  if (classes.m_compare_xy_2)
    classes.m_compare_xy_2->def("__call__", &Compare_xy_2::operator());

  // Construct_min_vertex_2
  static const char construct_min_vertex_2[] = "Construct_min_vertex_2";
  add_class_object<Construct_min_vertex_2, construct_min_vertex_2>
    (classes.m_construct_min_vertex_2, traits_scope);
  if (classes.m_construct_min_vertex_2)
    classes.m_construct_min_vertex_2->def("__call__", &Construct_min_vertex_2::operator(), RVP());

  // Construct_max_vertex_2
  static const char construct_max_vertex_2[] = "Construct_max_vertex_2";
  add_class_object<Construct_max_vertex_2, construct_max_vertex_2>
    (classes.m_construct_max_vertex_2, traits_scope);
  classes.m_construct_max_vertex_2->def("__call__", &Construct_max_vertex_2::operator(), RVP());

  // Is_vertical_2
  static const char is_vertical_2[] = "Is_vertical_2";
  add_class_object<Is_vertical_2, is_vertical_2>(classes.m_is_vertical_2, traits_scope);
  if (classes.m_is_vertical_2)
    classes.m_is_vertical_2->def("__call__", &Is_vertical_2::operator());

  // Compare_y_at_x_2
  static const char compare_y_at_x_2[] = "Compare_y_at_x_2";
  add_class_object<Compare_y_at_x_2, compare_y_at_x_2>
    (classes.m_compare_y_at_x_2, traits_scope);
  if (classes.m_compare_y_at_x_2)
    classes.m_compare_y_at_x_2->def("__call__", &Compare_y_at_x_2::operator());

  // Compare_y_at_x_right_2
  static const char compare_y_at_x_right_2[] = "Compare_y_at_x_right_2";
  add_class_object<Compare_y_at_x_right_2, compare_y_at_x_right_2>
    (classes.m_compare_y_at_x_right_2, traits_scope);
  if (classes.m_compare_y_at_x_right_2)
    classes.m_compare_y_at_x_right_2->def("__call__", &Compare_y_at_x_right_2::operator());

  // Equal_2
  static const char equal_2[] = "Equal_2";
  add_class_object<Equal_2, equal_2>(classes.m_equal_2, traits_scope);
  if (classes.m_equal_2)  {
    using Equal1 = bool(Equal_2::*)(const Point_2&, const Point_2&) const;
    using Equal2 = bool(Equal_2::*)(const X_monotone_curve_2&, const X_monotone_curve_2&) const;
    classes.m_equal_2->def("__call__", static_cast<Equal1>(&Equal_2::operator()));
    classes.m_equal_2->def("__call__", static_cast<Equal2>(&Equal_2::operator()));
  }

  c.def(bp::init<>())
    .def(bp::init<T&>())
    .def("compare_x_2_object", &T::compare_x_2_object)
    .def("compare_xy_2_object", &T::compare_xy_2_object)
    .def("construct_min_vertex_2_object", &T::construct_min_vertex_2_object)
    .def("construct_max_vertex_2_object", &T::construct_max_vertex_2_object)
    .def("is_vertical_2_object", &T::is_vertical_2_object)
    .def("compare_y_at_x_2_object", &T::compare_y_at_x_2_object)
    .def("compare_y_at_x_right_2_object", &T::compare_y_at_x_right_2_object)
    .def("equal_2_object", &T::equal_2_object)
    ;

  export_Compare_at_x_left_2<T>(c, Has_left_category());

  exported = true;
}

#endif
