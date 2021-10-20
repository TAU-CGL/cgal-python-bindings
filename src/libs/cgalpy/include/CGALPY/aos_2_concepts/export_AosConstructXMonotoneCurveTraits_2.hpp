// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSCONSTRUCTXMONOTONECURVETRAITS_2_HPP
#define CGALPY_EXPORT_AOSCONSTRUCTXMONOTONECURVETRAITS_2_HPP

#include "CGALPY/aos_2_concepts/export_AosBasicTraits.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"

template <typename T, typename RVP, typename C, typename Concepts>
void export_AosConstructXMonotoneTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits<T, RVP>(c, concepts);

  typedef typename T::Point_2                       Point_2;
  typedef typename T::X_monotone_curve_2            X_monotone_curve_2;
  typedef typename T::Construct_x_monotone_curve_2  Construct_x_monotone_curve_2;

  using Ctr_xcv = X_monotone_curve_2 (Construct_x_monotone_curve_2::*)(const Point_2&, const Point_2&) const;
  bp::class_<Construct_x_monotone_curve_2>("Construct_x_monotone_curve_2", bp::no_init)
    .def("__call__", static_cast<Ctr_xcv>(&Construct_x_monotone_curve_2::operator()))
    ;

  c.def("construct_x_monotone_curve_2_object", &T::construct_x_monotone_curve_2_object)
  ;

  exported = true;
}

#endif
