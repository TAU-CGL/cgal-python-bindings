// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_X_MONOTONE_TRAITS_CLASSES_HPP
#define CGALPY_AOS_X_MONOTONE_TRAITS_CLASSES_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T, typename Tag = CGAL::Tag_false>
struct Merge_2_class { /* empty */ };

template <typename T>
struct Merge_2_class<T, CGAL::Tag_true> {
  using Merge_2 = typename T::Merge_2;
  using Are_mergeable_2 = typename T::Are_mergeable_2;

  // Constructor
  Merge_2_class() :
    m_merge_2(nullptr),
    m_are_mergeable_2(nullptr)
  {}

  // Destructor
  ~Merge_2_class() {
    if (m_merge_2) delete m_merge_2;
    if (m_are_mergeable_2) delete m_are_mergeable_2;
  }

  py::class_<Merge_2>* m_merge_2;
  py::class_<Are_mergeable_2>* m_are_mergeable_2;
};

template <typename T>
struct Aos_x_monotone_traits_classes {
  using Intersect_2 = typename T::Intersect_2;
  using Split_2 = typename T::Split_2;

  // Constructor
  Aos_x_monotone_traits_classes() :
    m_intersect_2(nullptr),
    m_split_2(nullptr)
  {}

  // Destructor
  ~Aos_x_monotone_traits_classes() {
    if (m_intersect_2) delete m_intersect_2;
    if (m_split_2) delete m_split_2;
  }

  py::class_<Intersect_2>* m_intersect_2;
  py::class_<Split_2>* m_split_2;

  using Has_merge_category = typename T::Has_merge_category;

  Merge_2_class<T, Has_merge_category> m_merge_2_class;
};

#endif
