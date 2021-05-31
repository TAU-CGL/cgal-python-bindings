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
#include <CGAL/Algebraic_structure_traits.h>
#include <CGAL/Rational_traits.h>

#if CGALPY_KERNEL == CGALPY_KERNEL_EPEC
template <typename Point>
size_t hash_rational_point(Point& p)
{
  size_t seed = 0;
  for (auto c = p.cartesian_begin(); c != p.cartesian_end(); ++c)
  {
    auto q = (*c).exact();
    auto simplify = typename CGAL::Algebraic_structure_traits<decltype(q)>::Simplify();
    CGAL::Rational_traits<decltype(q)> traits;
    simplify(q);
    boost::hash_combine(seed, CGAL::to_double(traits.numerator(q)));
    boost::hash_combine(seed, CGAL::to_double(traits.denominator(q)));
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
