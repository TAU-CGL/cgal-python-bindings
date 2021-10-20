// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_BASIC_TRAITS_CLASSES_HPP
#define CGALPY_AOS_BASIC_TRAITS_CLASSES_HPP

template <typename T>
struct Aos_basic_traits_classes {
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
  // typedef typename T::Compare_y_at_x_left_2     Compare_y_at_x_left_2;

  bp::class_<Point_2>* m_point_2;
  bp::class_<X_monotone_curve_2>* m_x_monotone_curve_2;
  bp::class_<Compare_x_2>* m_compare_x_2;
  bp::class_<Compare_xy_2>* m_compare_xy_2;
  bp::class_<Equal_2>* m_equal_2;
  bp::class_<Construct_min_vertex_2>* m_construct_min_vertex_2;
  bp::class_<Construct_max_vertex_2>* m_construct_max_vertex_2;
  bp::class_<Is_vertical_2>* m_is_vertical_2;
  bp::class_<Compare_y_at_x_2>* m_compare_y_at_x_2;
  bp::class_<Compare_y_at_x_right_2>* m_compare_y_at_x_right_2;
  // bp::class_<Compare_y_at_x_left_2>* m_compare_y_at_x_left_2;
};

#endif
