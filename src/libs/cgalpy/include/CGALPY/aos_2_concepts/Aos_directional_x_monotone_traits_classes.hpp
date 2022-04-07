// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_DIRECTIONAL_X_MONOTONE_TRAITS_CLASSES_HPP
#define CGALPY_AOS_DIRECTIONAL_X_MONOTONE_TRAITS_CLASSES_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T>
struct Aos_directional_x_monotone_traits_classes {
  typedef typename T::Compare_endpoints_xy_2    Compare_endpoints_xy_2;
  typedef typename T::Construct_opposite_2      Construct_opposite_2;

  // Constructor
  Aos_directional_x_monotone_traits_classes() :
    m_compare_endpoints_xy_2(nullptr),
    m_construct_opposite_2(nullptr)
  {}

  // Destructor
  ~Aos_directional_x_monotone_traits_classes() {
    if (m_compare_endpoints_xy_2) delete m_compare_endpoints_xy_2;
    if (m_construct_opposite_2) delete m_construct_opposite_2;
  }

  // Class objects
  py::class_<Compare_endpoints_xy_2>* m_compare_endpoints_xy_2;
  py::class_<Construct_opposite_2>* m_construct_opposite_2;
};

#endif
