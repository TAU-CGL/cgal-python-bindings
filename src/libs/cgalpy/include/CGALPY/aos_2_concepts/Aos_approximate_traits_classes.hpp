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
  typedef typename T::Approximate_2             Approximate_2;

  py::class_<Approximate_2>* m_approximate_2;
};

#endif
