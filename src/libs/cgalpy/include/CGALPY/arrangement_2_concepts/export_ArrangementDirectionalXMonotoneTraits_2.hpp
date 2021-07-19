// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ARRANGEMENTDIRECTIONALXMONOTONETRAITS_2_HPP
#define CGALPY_EXPORT_ARRANGEMENTDIRECTIONALXMONOTONETRAITS_2_HPP

#include "export_ArrangementXMonotoneTraits_2.hpp"

template <typename T, typename RVP, typename C>
void export_ArrangementDirectionalXMonotoneTraits_2(C c) {
  static bool exported = false;
  if (exported) return;

  export_ArrangementXMonotoneTraits_2<T, RVP>(c);
  bp::class_<typename T::Compare_endpoints_xy_2>("Compare_endpoints_xy_2",
                                                 bp::no_init)
    .def("__call__", &T::Compare_endpoints_xy_2::operator());

  bp::class_<typename T::Construct_opposite_2>("Construct_opposite_2",
                                                 bp::no_init)
    .def("__call__", &T::Construct_opposite_2::operator());

  c.def("compare_endpoints_xy_2_object", &T::compare_endpoints_xy_2_object)
    .def("construct_opposite_2_object", &T::construct_opposite_2_object)
    ;

  exported = true;
}

#endif //CGALPY_EXPORT_ARRANGEMENTDIRECTIONALXMONOTONETRAITS_2_HPP
