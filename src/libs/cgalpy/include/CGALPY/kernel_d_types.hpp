// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_KERNEL_D_D_TYPES_HPP
#define CGALPY_KERNEL_D_D_TYPES_HPP

#include <boost/static_assert.hpp>

#include "CGALPY/kernel_d_config.hpp"
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
#include "CGALPY/types.hpp"

#if CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPIC_D
typedef CGAL::Epicd_d<Dimension_tag>                            Kernel_d;
typedef Copy_const_reference                Kernel_return_value_policy;
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_EPEC_D
typedef CGAL::Epecd_d<Dimension_tag>                            Kernel_d;
typedef Return_by_value                     Kernel_return_value_policy;
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_DOUBLE
typedef CGAL::Cartesian_d<double>                               Kernel_d;
typedef Copy_const_reference                Kernel_return_value_policy;
#elif CGALPY_KERNEL_D == CGALPY_KERNEL_D_CARTESIAN_D_LAZY_GMPQ
typedef CGAL::Cartesian_d<CGAL::Lazy_exact_nt<CGAL::Gmpq>>      Kernel_d;
typedef Copy_const_reference                Kernel_return_value_policy;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL_D");
#endif

typedef Kernel_d::FT                                            FT_d;
typedef Kernel_d::Point_d                                       Point_d;

#endif
