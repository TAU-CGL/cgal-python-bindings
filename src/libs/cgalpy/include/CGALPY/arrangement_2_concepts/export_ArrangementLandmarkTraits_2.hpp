// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ARRANGEMENTLANDMARKTRAITS_2_HPP
#define CGALPY_EXPORT_ARRANGEMENTLANDMARKTRAITS_2_HPP

#include "export_ArrangementApproximateTraits_2.hpp"
#include "export_ArrangementConstructXMonotoneCurveTraits_2.hpp"

template <typename T, typename RVP, typename C>
void export_ArrangementLandmarkTraits_2 (C c) {
  static bool exported = false;
  if (exported) return;

  export_ArrangementApproximateTraits_2<T, RVP>(c);
  export_ArrangementConstructXMonotoneTraits_2<T, RVP>(c);

  exported = true;
}

#endif //CGALPY_EXPORT_ARRANGEMENTLANDMARKTRAITS_2_HPP
