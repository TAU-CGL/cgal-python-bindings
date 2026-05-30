// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_CONSTRUCT_X_MONOTONE_CURVE_TRAITS_CLASSES_HPP
#define CGALPY_AOS_CONSTRUCT_X_MONOTONE_CURVE_TRAITS_CLASSES_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T>
struct Aos_construct_x_monotone_curve_traits_classes {
  using Point_2 = typename T::Point_2;
  using X_monotone_curve_2 = typename T::X_monotone_curve_2;
  using Construct_x_monotone_curve_2 = typename T::Construct_x_monotone_curve_2;

  // Constructor
  Aos_construct_x_monotone_curve_traits_classes() :
    m_construct_x_monotone_curve_2(nullptr)
  {}

  // Destructor
  ~Aos_construct_x_monotone_curve_traits_classes()
  { if (m_construct_x_monotone_curve_2) delete m_construct_x_monotone_curve_2; }

  // Class objects
  py::class_<Construct_x_monotone_curve_2>* m_construct_x_monotone_curve_2;
};

#endif
