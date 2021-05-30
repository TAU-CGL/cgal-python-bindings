// Copyright (c) 2020 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_HASH_POINT
#define CGALPY_HASH_POINT

#include <boost/functional/hash_fwd.hpp>

#include <CGAL/basic.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Algebraic_structure_traits.h>

#if CGALPY_KERNEL == CGALPY_KERNEL_EPEC
template <typename Point>
size_t hash_rational_point(Point& p)
{
  auto simplify = CGAL::Algebraic_structure_traits<CGAL::Gmpq>::Simplify();
  size_t seed = 0;
  for (auto c = p.cartesian_begin(); c != p.cartesian_end(); ++c)
  {
    auto q = (*c).exact();
    // simplify(q);
    // boost::hash_combine(seed, q.numerator().to_double());
    // boost::hash_combine(seed, q.denominator().to_double());
  }
  return seed;
}
#endif

#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
template <typename Point>
size_t hash_rational_point(Point& p)
{
  return boost::hash_range(p.cartesian_begin(), p.cartesian_end());
}
#endif



#endif
