// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSCONSTRUCTXMONOTONECURVETRAITS_2_HPP
#define CGALPY_EXPORT_AOSCONSTRUCTXMONOTONECURVETRAITS_2_HPP

#include <nanobind/nanobind.h>

#include "cgalpy/aos_2_concepts/export_AosBasicTraits_2.hpp"
#include "cgalpy/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"

#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_construct_x_monotone_doc = cgalpy::aos2::docstrings;

template <typename T, typename C, typename Concepts>
void export_AosConstructXMonotoneCurveTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits_2<T>(c, concepts);

  using Pnt = typename T::Point_2;
  using Xcv = typename T::X_monotone_curve_2;
  using Ctr_xcv = typename T::Construct_x_monotone_curve_2;

  auto& classes = concepts.m_aos_construct_x_monotone_curve_traits_2_classes;

  using Ctr_xcv_fnc = Xcv(Ctr_xcv::*)(const Pnt&, const Pnt&) const;
  classes.m_construct_x_monotone_curve_2 =
    new py::class_<Ctr_xcv>(
      c, "Construct_x_monotone_curve_2",
      aos2_construct_x_monotone_doc::AosConstructXMonotoneCurveTraits_2_Construct_x_monotone_curve_2);
  classes.m_construct_x_monotone_curve_2->
    def("__call__", static_cast<Ctr_xcv_fnc>(&Ctr_xcv::operator()),
        py::arg("p1"), py::arg("p2"),
        aos2_construct_x_monotone_doc::AosTraits_ConstructXMonotoneCurve_2_operator_call);

  c.def("construct_x_monotone_curve_2_object", &T::construct_x_monotone_curve_2_object,
        aos2_construct_x_monotone_doc::AosConstructXMonotoneCurveTraits_2_construct_x_monotone_curve_2_object);

  exported = true;
}

#endif
