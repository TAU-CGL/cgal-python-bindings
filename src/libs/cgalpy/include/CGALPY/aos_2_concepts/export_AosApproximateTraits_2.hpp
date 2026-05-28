// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSAPPROXIMATETRAITS_2_HPP
#define CGALPY_EXPORT_AOSAPPROXIMATETRAITS_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/aos_2_concepts/export_AosBasicTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"

#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_approximate_doc = cgalpy::aos2::docstrings;

// Not all traits classes support the type T::Approximate_point_2 (and the
// operator:
//      T::Approximate_point_2 T::Approximate_2:operator(const T::Point_2&)
// We use SFINAE-constrained overloads to dispatch the appropriate function.

//! Fall through---do nothing
template <typename T, typename C, typename Concepts>
void export_approximate_point(C& c, Concepts& concepts, ...) {}

// Bind the approximate point related objects
template <typename T, typename C, typename Concepts,
          typename = typename T::Approximate_point_2>
void export_approximate_point(C& c, Concepts& concepts, bool) {
  using Pt = typename T::Point_2;
  using Approximate_2 = typename T::Approximate_2;
  using Ant = typename T::Approximate_number_type;
  using Ap = typename T::Approximate_point_2;
  using ovld2 = Ap(Approximate_2::*)(const Pt&) const;

  // Bind the approximate point
  if (! add_attr<Ap>(c, "Approximate_point_2")) {
    throw std::runtime_error("Approximate_point_2 is not bound!");
  }

  // Bind the operator
  auto& classes = concepts.m_aos_approximate_traits_2_classes;
  classes.m_approximate_2->def("__call__",
                               static_cast<ovld2>(&Approximate_2::operator()),
                               py::arg("point"),
                               aos2_approximate_doc::AosTraits_ApproximatePoint_2_operator_call);
}

// Bind the Approximate_2 traits functor
template <typename T, typename C, typename Concepts>
void export_AosApproximateTraits_2(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits_2<T>(c, concepts);

  using Pt = typename T::Point_2;
  using Approximate_2 = typename T::Approximate_2;
  using Ant = typename T::Approximate_number_type;

  auto& classes = concepts.m_aos_approximate_traits_2_classes;

  if (! add_attr<Approximate_2>(c, "Approximate_2")) {
    classes.m_approximate_2 = new py::class_<Approximate_2>(
      c, "Approximate_2",
      aos2_approximate_doc::AosApproximateTraits_2_Approximate_2);
    using ovld1 = Ant(Approximate_2::*)(const Pt&, int i) const;
    classes.m_approximate_2->def("__call__",
                                 static_cast<ovld1>(&Approximate_2::operator()),
                                 py::arg("point"), py::arg("i"),
                                 aos2_approximate_doc::AosTraits_Approximate_2_operator_call);
    export_approximate_point<T>(c, concepts, true);
  }

  c.def("approximate_2_object", &T::approximate_2_object,
        aos2_approximate_doc::AosApproximateTraits_2_approximate_2_object);

  exported = true;
}

#endif
