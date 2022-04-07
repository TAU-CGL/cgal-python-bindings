// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSCONSTRUCTXMONOTONECURVETRAITS_2_HPP
#define CGALPY_EXPORT_AOSCONSTRUCTXMONOTONECURVETRAITS_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/aos_2_concepts/export_AosBasicTraits.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"

namespace py = nanobind;

template <typename T, typename RVP, typename C, typename Concepts>
void export_AosConstructXMonotoneCurveTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits<T, RVP>(c, concepts);

  typedef typename T::Point_2                       Point_2;
  typedef typename T::X_monotone_curve_2            X_monotone_curve_2;
  typedef typename T::Construct_x_monotone_curve_2  Construct_x_monotone_curve_2;

  auto& classes = concepts.m_construct_x_monotone_curve_traits_classes;

  using Ctr_xcv = X_monotone_curve_2 (Construct_x_monotone_curve_2::*)(const Point_2&, const Point_2&) const;
  classes.m_construct_x_monotone_curve_2 =
    new py::class_<Construct_x_monotone_curve_2>("Construct_x_monotone_curve_2", py::no_init);
  classes.m_construct_x_monotone_curve_2->
    def("__call__", static_cast<Ctr_xcv>(&Construct_x_monotone_curve_2::operator()));

  c.def("construct_x_monotone_curve_2_object", &T::construct_x_monotone_curve_2_object);

  exported = true;
}

#endif
