// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ARRANGEMENTCONSTRUCTXMONOTONECURVETRAITS_2_HPP
#define CGALPY_EXPORT_ARRANGEMENTCONSTRUCTXMONOTONECURVETRAITS_2_HPP

#include "export_ArrangementBasicTraits.hpp"

template <typename T, typename RVP, typename C>
void export_ArrangementConstructXMonotoneTraits_2(C c) {
  static bool exported = false;
  if (exported) return;

  export_ArrangementBasicTraits<T, RVP>(c);
  bp::class_<typename T::Construct_x_monotone_curve_2>
    ("Construct_x_monotone_curve_2", bp::no_init)
    .def<typename T::X_monotone_curve_2 (T::Construct_x_monotone_curve_2::*)
      (const T::Point_2&, const T::Point_2&) const>
      ("__call__", &T::Construct_x_monotone_curve_2::operator())
      ;

  c.def("construct_x_monotone_curve_2_object", &T::construct_x_monotone_curve_2_object)
  ;

  exported = true;
}

#endif //CGALPY_EXPORT_ARRANGEMENTCONSTRUCTXMONOTONECURVETRAITS_2_HPP
