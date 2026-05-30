// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSLANDMARKTRAITS_2_HPP
#define CGALPY_EXPORT_AOSLANDMARKTRAITS_2_HPP

#include "cgalpy/aos_2_concepts/export_AosApproximateTraits_2.hpp"
#include "cgalpy/aos_2_concepts/export_AosConstructXMonotoneCurveTraits_2.hpp"
#include "cgalpy/aos_2_concepts/Aos_landmark_traits_classes.hpp"

template <typename T, typename C, typename Concepts>
void export_AosLandmarkTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosApproximateTraits_2<T>(c, concepts);
  export_AosConstructXMonotoneCurveTraits_2<T>(c, concepts);

  exported = true;
}

#endif
