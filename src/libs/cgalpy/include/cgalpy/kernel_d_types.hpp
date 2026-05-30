// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_KERNEL_D_TYPES_HPP
#define CGALPY_KERNEL_D_TYPES_HPP

#include <boost/static_assert.hpp>

#include <CGAL/Dimension.h>
#include <CGAL/Linear_algebraCd.h>

#include "cgalpy/kernel_d_config.hpp"

#if CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPIC_D
#include <CGAL/Epick_d.h>
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D
#include <CGAL/Epeck_d.h>
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_DOUBLE
#include <CGAL/Cartesian_d.h>
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ
#include <CGAL/Cartesian_d.h>
#include <CGAL/Lazy_exact_nt.h>
#include <CGAL/Gmpq.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL_D");
#endif

#include "cgalpy/types.hpp"

// Dimention tag
#if CGALPY_KERNEL_D_DIMENSION_TAG == CGALPY_KERNEL_D_DIMENSION_TAG_STATIC
using Dimension_tag = CGAL::Dimension_tag<CGALPY_KERNEL_D_DIMENSION>;
#elif CGALPY_KERNEL_D_DIMENSION_TAG == CGALPY_KERNEL_D_DIMENSION_TAG_DYNAMIC
using Dimension_tag = CGAL::Dynamic_dimension_tag;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL_D_DIMENSION_TAG");
#endif

// dD Kernel
#if CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPIC_D
using Kernel_d = CGAL::Epick_d<Dimension_tag>;
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D
using Kernel_d = CGAL::Epeck_d<Dimension_tag>;
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_DOUBLE
using Kernel_d = CGAL::Cartesian_d<double>;
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ
using Kernel_d = CGAL::Cartesian_d<CGAL::Lazy_exact_nt<CGAL::Gmpq>>;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL_D");
#endif

using FT_d = Kernel_d::FT;
using Point_d = Kernel_d::Point_d;
using Segment_d = Kernel_d::Segment_d;
using Vector_d = Kernel_d::Vector_d;
using Linear_algebra_cartesian_d = CGAL::Linear_algebraCd<FT_d>;

// Determine whether the dD kernel is an an EPEC type.
// An EPEC type has a non trivial FT
constexpr bool is_epec_d_type() {
  return ((CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D) ||
          (CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ));
}

#endif
