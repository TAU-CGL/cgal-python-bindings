// Copyright (c) 2020 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_HASH_RATIONAL_POINT_HPP
#define CGALPY_HASH_RATIONAL_POINT_HPP

#include <boost/functional/hash_fwd.hpp>
#include <boost/functional/hash/hash.hpp>

template <bool b>
struct Hash_rational_point {};

template <>
struct Hash_rational_point<true> {
  template <typename Point>
  static size_t hash_rational_point(Point& p) {
    size_t seed = 0;
    for (auto c = p.cartesian_begin(); c != p.cartesian_end(); ++c) {
      auto q = (*c).exact();
      auto simplify =
        typename CGAL::Algebraic_structure_traits<decltype(q)>::Simplify();
      CGAL::Rational_traits<decltype(q)> traits;
      simplify(q);
      boost::hash_combine(seed, CGAL::to_double(traits.numerator(q)));
      boost::hash_combine(seed, CGAL::to_double(traits.denominator(q)));
    }
    return seed;
  }
};

template <>
struct Hash_rational_point<false> {
  template <typename Point>
  static size_t hash_rational_point(Point& p)
  { return boost::hash_range(p.cartesian_begin(), p.cartesian_end()); }
};

template<bool b, typename Point>
size_t hash_rational_point(Point& p) {
  return Hash_rational_point<b>::hash_rational_point(p);
};

// #if (CGALPY_KERNEL == CGALPY_KERNEL_EPEC) || \
//   (CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT) || \
//   (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ) || \
//   (CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) || \
//   (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ)
// template <typename Point>
// size_t hash_rational_point(Point& p) {
//   size_t seed = 0;
//   for (auto c = p.cartesian_begin(); c != p.cartesian_end(); ++c) {
//     auto q = (*c).exact();
//     auto simplify =
//       typename CGAL::Algebraic_structure_traits<decltype(q)>::Simplify();
//     CGAL::Rational_traits<decltype(q)> traits;
//     simplify(q);
//     boost::hash_combine(seed, CGAL::to_double(traits.numerator(q)));
//     boost::hash_combine(seed, CGAL::to_double(traits.denominator(q)));
//   }
//   return seed;
// }
// #endif

// #if (CGALPY_KERNEL == CGALPY_KERNEL_EPIC) || \
//   (CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE) || \
//   (CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPIC_D) || \
//   (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_DOUBLE)
// template <typename Point>
// size_t hash_rational_point(Point& p)
// { return boost::hash_range(p.cartesian_begin(), p.cartesian_end()); }
// #endif

#endif
