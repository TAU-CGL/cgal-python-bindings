// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_APPROXIMATE_TRAITS_CLASSES_HPP
#define CGALPY_AOS_APPROXIMATE_TRAITS_CLASSES_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T>
struct Aos_approximate_traits_classes {
  using Approximate_2 = typename T::Approximate_2;

  // Constructor
  Aos_approximate_traits_classes() : m_approximate_2(nullptr) {}

  // Destructor
  ~Aos_approximate_traits_classes()
  { if (m_approximate_2) delete m_approximate_2; }

  // Class objects
  py::class_<Approximate_2>* m_approximate_2;
};

#endif
