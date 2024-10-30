// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_KERNEL_CONFIG_HPP
#define CGALPY_KERNEL_CONFIG_HPP

#define CGAL_HEADER_ONLY 1

// Prevent compilation issues with some kernels
// #define CGAL_DO_NOT_USE_BOOST_MP 1
// Suppressing GMPXX is not recommended, but if you need to, uncomment the next
// #undef CGAL_USE_GMPXX

#define CGALPY_KERNEL_EPIC                                  0
#define CGALPY_KERNEL_EPEC                                  1
#define CGALPY_KERNEL_EPEC_WITH_SQRT                        2
#define CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE      3
#define CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ   4
#define CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL               5

#ifndef CGALPY_KERNEL
#define CGALPY_KERNEL 0
#endif

// Determine whether the dD kernel is an an EPEC type.
// An EPEC type has a non trivial FT
constexpr bool is_exact_ft() {
  return ((CGALPY_KERNEL == CGALPY_KERNEL_EPEC) ||                      \
          (CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT) ||            \
          (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) || \
          (CGALPY_KERNEL == CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL));
}

#endif //CGALPY_KERNEL_CONFIG_HPP
