// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSLANDMARKTRAITS_2_HPP
#define CGALPY_EXPORT_AOSLANDMARKTRAITS_2_HPP

#include "CGALPY/aos_2_concepts/export_AosApproximateTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosConstructXMonotoneCurveTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_landmark_traits_classes.hpp"

template <typename T, typename RVP, typename C, typename Concepts>
void export_AosLandmarkTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosApproximateTraits_2<T, RVP>(c, concepts);
  export_AosConstructXMonotoneTraits_2<T, RVP>(c, concepts);

  exported = true;
}

#endif
