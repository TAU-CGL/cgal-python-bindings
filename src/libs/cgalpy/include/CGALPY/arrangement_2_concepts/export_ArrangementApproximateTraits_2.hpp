// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ARRANGEMENTAPPROXIMATETRAITS_2_HPP
#define CGALPY_EXPORT_ARRANGEMENTAPPROXIMATETRAITS_2_HPP

#include "export_ArrangementBasicTraits.hpp"

template <typename T, typename RVP, typename C>
void export_ArrangementApproximateTraits_2 (C c) {
  static bool exported = false;
  if (exported) return;

  export_ArrangementBasicTraits<T, RVP>(c);
  bp::class_<typename T::Approximate_2>("Approximate_2", bp::no_init)
    .def("__call__", &T::Approximate_2::operator())
    ;

  c.def("Approximate_2", &T::approximate_2_object);

  exported = true;
}

#endif //CGALPY_EXPORT_ARRANGEMENTAPPROXIMATETRAITS_2_HPP
