// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSAPPROXIMATETRAITS_2_HPP
#define CGALPY_EXPORT_AOSAPPROXIMATETRAITS_2_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"

template <typename T, typename C, typename Concepts>
void export_AosApproximateTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits<T>(c, concepts);

  typedef typename T::Approximate_2             Approximate_2;

  auto& classes = concepts.m_approximate_traits_classes;
  classes.m_approximate_2 = new py::class_<Approximate_2>(c, "Approximate_2");
  classes.m_approximate_2->def("__call__", &T::Approximate_2::operator());

  c.def("approximate_2_object", &T::approximate_2_object);

  exported = true;
}

#endif
