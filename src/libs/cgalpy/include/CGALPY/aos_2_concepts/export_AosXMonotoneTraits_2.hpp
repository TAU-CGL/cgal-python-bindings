// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOSXMONOTONETRAITS_2_HPP
#define CGALPY_EXPORT_AOSXMONOTONETRAITS_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/aos_2_concepts/export_AosBasicTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/intersect_2_call_operator.hpp"

#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_x_monotone_doc = cgalpy::aos2::docstrings;

//
template<typename T, typename C, typename Classes>
void export_Merge_2(C& c, Classes& classes, CGAL::Tag_true) {
  using Merge_2 = typename T::Merge_2;

  classes.m_merge_2 = new py::class_<Merge_2>(
    c, "Merge_2", aos2_x_monotone_doc::AosXMonotoneTraits_2_Merge_2);
  classes.m_merge_2->def("__call__", &Merge_2::operator());
  c.def("merge_2_object", &T::merge_2_object,
        aos2_x_monotone_doc::AosXMonotoneTraits_2_merge_2_object);
}

//
template<typename T, typename C, typename Classes>
void export_Merge_2(C& c, Classes& classes, CGAL::Tag_false) {}

//
template<typename T, typename C, typename Classes>
void export_Are_mergeable_2(C& c, Classes& classes, CGAL::Tag_true) {
  using Are_mergeable_2 = typename T::Are_mergeable_2;

  classes.m_are_mergeable_2 =
    new py::class_<Are_mergeable_2>(
      c, "Are_mergeable_2",
      aos2_x_monotone_doc::AosXMonotoneTraits_2_Are_mergeable_2);
  classes.m_are_mergeable_2->def("__call__", &Are_mergeable_2::operator());
  c.def("are_mergeable_2_object", &T::are_mergeable_2_object,
        aos2_x_monotone_doc::AosXMonotoneTraits_2_are_mergeable_2_object);
}

//
template<typename T, typename C, typename Classes>
void export_Are_mergeable_2(C& c, Classes& classes, CGAL::Tag_false) {}

//
template <typename T, typename C, typename Concepts>
void export_AosXMonotoneTraits_2(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits_2<T>(c, concepts);

  using Has_merge_category = typename T::Has_merge_category;
  using Intersect_2 = typename T::Intersect_2;
  using Split_2 = typename T::Split_2;

  auto& classes = concepts.m_aos_x_monotone_traits_2_classes;

  classes.m_intersect_2 = new py::class_<Intersect_2>(
    c, "Intersect_2", aos2_x_monotone_doc::AosXMonotoneTraits_2_Intersect_2);
  classes.m_intersect_2->def("__call__", &intersect_2_call_operator<T>);

  classes.m_split_2 = new py::class_<Split_2>(
    c, "Split_2", aos2_x_monotone_doc::AosXMonotoneTraits_2_Split_2);
  classes.m_split_2->def("__call__", &Split_2::operator());

  c.def("intersect_2_object", &T::intersect_2_object,
        aos2_x_monotone_doc::AosXMonotoneTraits_2_intersect_2_object);
  c.def("split_2_object", &T::split_2_object,
        aos2_x_monotone_doc::AosXMonotoneTraits_2_split_2_object);

  export_Are_mergeable_2<T>(c, classes.m_merge_2_class, Has_merge_category());
  export_Merge_2<T>(c, classes.m_merge_2_class, Has_merge_category());

  exported = true;
}

#endif
