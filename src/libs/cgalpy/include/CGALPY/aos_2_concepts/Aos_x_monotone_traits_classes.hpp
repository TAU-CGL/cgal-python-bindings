// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_X_MONOTONE_TRAITS_CLASSES_HPP
#define CGALPY_AOS_X_MONOTONE_TRAITS_CLASSES_HPP

template <typename T>
struct Aos_x_monotone_traits_classes {
  typedef typename T::Intersect_2               Intersect_2;
  typedef typename T::Split_2                   Split_2;
  typedef typename T::Are_mergeable_2           Are_mergeable_2;
  typedef typename T::Merge_2                   Merge_2;

  bp::class_<Intersect_2>* m_intersect_2;
  bp::class_<Split_2>* m_split_2;
  bp::class_<Are_mergeable_2>* m_are_mergeable_2;
  bp::class_<Merge_2>* m_merge_2;
};

#endif
