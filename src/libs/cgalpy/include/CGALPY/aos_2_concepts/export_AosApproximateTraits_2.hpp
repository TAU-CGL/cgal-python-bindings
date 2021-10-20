// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSAPPROXIMATETRAITS_2_HPP
#define CGALPY_EXPORT_AOSAPPROXIMATETRAITS_2_HPP

#include "CGALPY/aos_2_concepts/Aos_aosapproximate_traits_classes.hpp"

template <typename T, typename RVP, typename C, typename Concepts>
void export_AosApproximateTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits<T, RVP>(c, concepts);

  typedef typename T::Approximate_2             Approximate_2;

  bp::class_<Approximate_2>("Approximate_2", bp::no_init)
    .def("__call__", &T::Approximate_2::operator())
    ;

  c.def("Approximate_2", &T::approximate_2_object);

  exported = true;
}

#endif
