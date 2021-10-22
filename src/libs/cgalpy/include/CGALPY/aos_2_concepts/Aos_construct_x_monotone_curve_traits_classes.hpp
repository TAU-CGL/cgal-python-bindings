// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_CONSTRUCT_X_MONOTONE_CURVE_TRAITS_CLASSES_HPP
#define CGALPY_AOS_CONSTRUCT_X_MONOTONE_CURVE_TRAITS_CLASSES_HPP

template <typename T>
struct Aos_construct_x_monotone_curve_traits_classes {
  typedef typename T::Point_2                       Point_2;
  typedef typename T::X_monotone_curve_2            X_monotone_curve_2;
  typedef typename T::Construct_x_monotone_curve_2  Construct_x_monotone_curve_2;

  // Constructor
  Aos_construct_x_monotone_curve_traits_classes() :
    m_construct_x_monotone_curve_2(nullptr)
  {}

  // Destructor
  ~Aos_construct_x_monotone_curve_traits_classes() {
    if (m_construct_x_monotone_curve_2) delete m_construct_x_monotone_curve_2;
  }

  // Class objects
  bp::class_<Construct_x_monotone_curve_2>* m_construct_x_monotone_curve_2;
};

#endif
