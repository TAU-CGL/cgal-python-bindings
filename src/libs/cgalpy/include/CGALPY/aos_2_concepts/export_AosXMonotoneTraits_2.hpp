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

#include "CGALPY/aos_2_concepts/export_AosBasicTraits.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"

namespace py = nanobind;

template <typename T>
void intersect_2_call_operator(typename T::Intersect_2& i,
                               typename T::X_monotone_curve_2& xc1,
                               typename T::X_monotone_curve_2& xc2,
                               py::list& res) {
  typedef typename T::Point_2                   Point_2;
  typedef typename T::X_monotone_curve_2        X_monotone_curve_2;
  typedef typename T::Multiplicity              Multiplicity;

  typedef std::pair<Point_2, Multiplicity> pair;
  auto v = std::vector<boost::variant<pair, X_monotone_curve_2>>();
  i(xc1, xc2, std::back_inserter(v));
  for (auto o : v) {
    if (pair* pa = boost::get<pair>(&o)) {
      py::tuple tup = py::make_tuple(pa->first, pa->second);
      res.append(tup);
    }
    else if (X_monotone_curve_2* curve = boost::get<X_monotone_curve_2>(&o)){
      res.append(*curve);
    }
  }
}

template<typename T, typename C, typename Classes>
void export_Merge_2(C c, Classes& classes, CGAL::Tag_true) {
  typedef typename T::Merge_2   Merge_2;
  classes.m_merge_2 = new py::class_<Merge_2>(c, "Merge_2");
  classes.m_merge_2->def("__call__", &Merge_2::operator());
  c.def("merge_2_object", &T::merge_2_object);
}

template<typename T, typename C, typename Classes>
void export_Merge_2(C c, Classes& classes, CGAL::Tag_false) {}

template<typename T, typename C, typename Classes>
void export_Are_mergeable_2(C c, Classes& classes, CGAL::Tag_true) {
  typedef typename T::Are_mergeable_2   Are_mergeable_2;
  classes.m_are_mergeable_2 = new py::class_<Are_mergeable_2>(c, "Are_mergeable_2");
  classes.m_are_mergeable_2->def("__call__", &Are_mergeable_2::operator());
  c.def("are_mergeable_2_object", &T::are_mergeable_2_object);
}

template<typename T, typename C, typename Classes>
void export_Are_mergeable_2(C c, Classes& classes, CGAL::Tag_false) {}

template <typename T, typename C, typename Concepts>
void export_AosXMonotoneTraits_2(C c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  export_AosBasicTraits<T>(c, concepts);

  typedef typename T::Has_merge_category        Has_merge_category;
  typedef typename T::Intersect_2               Intersect_2;
  typedef typename T::Split_2                   Split_2;

  auto& classes = concepts.m_x_monotone_traits_classes;

  classes.m_intersect_2 = new py::class_<Intersect_2>(c, "Intersect_2");
  classes.m_intersect_2->def("__call__", &intersect_2_call_operator<T>);

  classes.m_split_2 = new py::class_<Split_2>(c, "Split_2");
  classes.m_split_2->def("__call__", &Split_2::operator());

  c.def("intersect_2_object", &T::intersect_2_object);
  c.def("split_2_object", &T::split_2_object);

  export_Are_mergeable_2<T>(c, classes, Has_merge_category());
  export_Merge_2<T>(c, classes, Has_merge_category());

  exported = true;
}

#endif
