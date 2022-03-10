// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSDIRECTIONALXMONOTONETRAITS_2_HPP
#define CGALPY_EXPORT_AOSDIRECTIONALXMONOTONETRAITS_2_HPP

#include "CGALPY/aos_2_concepts/export_AosXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"

template <typename T, typename RVP, typename C, typename Concepts>
void export_AosDirectionalXMonotoneTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosXMonotoneTraits_2<T, RVP>(c, concepts);

  typedef typename T::Compare_endpoints_xy_2    Compare_endpoints_xy_2;
  typedef typename T::Construct_opposite_2      Construct_opposite_2;

  auto& classes = concepts.m_directional_x_monotone_traits_classes;

  classes.m_compare_endpoints_xy_2 =
    new bp::class_<Compare_endpoints_xy_2>("Compare_endpoints_xy_2", bp::no_init);
  classes.m_compare_endpoints_xy_2->def("__call__", &T::Compare_endpoints_xy_2::operator());

  classes.m_construct_opposite_2 =
    new bp::class_<Construct_opposite_2>("Construct_opposite_2", bp::no_init);
  classes.m_construct_opposite_2->def("__call__", &T::Construct_opposite_2::operator());

  c.def("compare_endpoints_xy_2_object", &T::compare_endpoints_xy_2_object)
    .def("construct_opposite_2_object", &T::construct_opposite_2_object)
    ;

  exported = true;
}

#endif
