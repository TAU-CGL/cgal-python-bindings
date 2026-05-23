// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSTRAITS_2_HPP
#define CGALPY_EXPORT_AOSTRAITS_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/aos_2_concepts/export_AosXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"

#include "CGALPY/add_attr.hpp"
#include "CGALPY/aos_2_concepts/make_x_monotone_2_call_operator.hpp"

#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_traits_doc = cgalpy::docstrings::Arrangement_on_surface_2;

template <typename T, typename C, typename Concepts>
void export_AosTraits_2(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  using Curve_2 = typename T::Curve_2;
  using Make_x_monotone_2 = typename T::Make_x_monotone_2;

  export_AosXMonotoneTraits_2<T>(c, concepts);

  auto& classes = concepts.m_aos_traits_2_classes;

  if (! add_attr<Curve_2>(c, "Curve_2")) {
    classes.m_curve_2 = new py::class_<Curve_2>(
      c, "Curve_2", aos2_traits_doc::AosTraits_2_Curve_2);
  }

  if (! add_attr<Make_x_monotone_2>(c, "Make_x_monotone_2")) {
    classes.m_make_x_monotone_2 =
      new py::class_<Make_x_monotone_2>(
        c, "Make_x_monotone_2",
        aos2_traits_doc::AosTraits_2_Make_x_monotone_2);
    classes.m_make_x_monotone_2->def("__call__",
                                     &make_x_monotone_2_call_operator<T>);
  }

  c.def("make_x_monotone_2_object", &T::make_x_monotone_2_object,
        aos2_traits_doc::AosTraits_2_make_x_monotone_2_object);

  exported = true;
}

#endif
