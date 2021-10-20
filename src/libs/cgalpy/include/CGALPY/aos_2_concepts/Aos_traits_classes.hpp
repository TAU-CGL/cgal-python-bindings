// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_TRAITS_CLASSES_HPP
#define CGALPY_AOS_TRAITS_CLASSES_HPP

template <typename T>
struct Aos_traits_classes {
  typedef typename T::Curve_2                   Curve_2;
  typedef typename T::Make_x_monotone_2         Make_x_monotone_2;

  bp::class_<Curve_2>* m_curve_2;
  bp::class_<Make_x_monotone_2>* m_make_x_monotone_2;
};

#endif
