// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_TRAITS_CLASSES_HPP
#define CGALPY_AOS_TRAITS_CLASSES_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T>
struct Aos_traits_classes {
  using Curve_2 = typename T::Curve_2;
  using Make_x_monotone_2 = typename T::Make_x_monotone_2;

  // Constructor
  Aos_traits_classes() :
    m_curve_2(nullptr),
    m_make_x_monotone_2(nullptr)
  {}

  // Destructor
  ~Aos_traits_classes() {
    if (m_curve_2) delete m_curve_2;
    if (m_make_x_monotone_2) delete m_make_x_monotone_2;
  }

  py::class_<Curve_2>* m_curve_2;
  py::class_<Make_x_monotone_2>* m_make_x_monotone_2;
};

#endif
