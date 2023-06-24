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

#include "CGALPY/aos_2_concepts/export_AosBasicTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"

namespace py = nanobind;

template <typename T, typename C, typename Concepts>
void export_AosApproximateTraits_2(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits_2<T>(c, concepts);

  using Pt = typename T::Point_2;
  using Approximate_2 = typename T::Approximate_2;
  using Ant = typename T::Approximate_number_type;
  using Ak = typename T::Approximate_kernel;
  using Ap = typename T::Approximate_point_2;

  auto& classes = concepts.m_approximate_traits_classes;

  classes.m_approximate_2 = new py::class_<Approximate_2>(c, "Approximate_2");

  using ovld1 = Ant(Approximate_2::*)(const Pt&, int i) const;
  using ovld2 = Ap(Approximate_2::*)(const Pt&) const;
  classes.m_approximate_2->def("__call__",
                               static_cast<ovld1>(&T::Approximate_2::operator()));
  classes.m_approximate_2->def("__call__",
                               static_cast<ovld2>(&T::Approximate_2::operator()));

  c.def("approximate_2_object", &T::approximate_2_object);

  exported = true;
}

#endif
