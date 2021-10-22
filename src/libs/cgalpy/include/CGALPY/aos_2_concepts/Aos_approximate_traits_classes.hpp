// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_AOS_APPROXIMATE_TRAITS_CLASSES_HPP
#define CGALPY_AOS_APPROXIMATE_TRAITS_CLASSES_HPP

template <typename T>
struct Aos_approximate_traits_classes {
  typedef typename T::Approximate_2             Approximate_2;

  bp::class_<Approximate_2>* m_approximate_2;
};

#endif
