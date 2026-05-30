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

#include <nanobind/nanobind.h>

#include "cgalpy/aos_2_concepts/export_AosXMonotoneTraits_2.hpp"
#include "cgalpy/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"

#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_directional_doc = cgalpy::aos2::docstrings;

template <typename T, typename C, typename Concepts>
void export_AosDirectionalXMonotoneTraits_2(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosXMonotoneTraits_2<T>(c, concepts);

  using Compare_endpoints_xy_2 = typename T::Compare_endpoints_xy_2;
  using Construct_opposite_2 = typename T::Construct_opposite_2;

  auto& classes = concepts.m_aos_directional_x_monotone_traits_2_classes;

  classes.m_compare_endpoints_xy_2 =
    new py::class_<Compare_endpoints_xy_2>(
      c, "Compare_endpoints_xy_2",
      aos2_directional_doc::AosTraits_CompareX_2_class);
  classes.m_compare_endpoints_xy_2->def("__call__",
                                        &T::Compare_endpoints_xy_2::operator(),
                                        py::arg("xcv"),
                                        "Compare the endpoints of an x-monotone curve.");

  classes.m_construct_opposite_2 =
    new py::class_<Construct_opposite_2>(
      c, "Construct_opposite_2",
      aos2_directional_doc::AosTraits_ConstructXMonotoneCurve_2_class);
  classes.m_construct_opposite_2->def("__call__",
                                      &T::Construct_opposite_2::operator(),
                                      py::arg("xcv"),
                                      "Construct the oppositely directed x-monotone curve.");

  c.def("compare_endpoints_xy_2_object", &T::compare_endpoints_xy_2_object,
        "Return the compare-endpoints functor.")
    .def("construct_opposite_2_object", &T::construct_opposite_2_object,
         "Return the construct-opposite functor.")
    ;

  exported = true;
}

#endif
