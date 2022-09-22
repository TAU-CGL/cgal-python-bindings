// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_KERNEL_TYPES_HPP
#define CGALPY_KERNEL_TYPES_HPP

#include <boost/static_assert.hpp>

#include "CGALPY/types.hpp"
#include "CGALPY/kernel_config.hpp"

#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Simple_cartesian.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Lazy_exact_nt.h>
#include <CGAL/Gmpq.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif

#include <CGAL/CORE_BigInt.h>
#include <CGAL/Sqrt_extension.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Direction_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Aff_transformation_2.h>

#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC
typedef CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt
                                                                Kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE
typedef double                                                  NT;
typedef CGAL::Filtered_kernel<CGAL::Simple_cartesian<NT>>       Kernel;
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ
typedef CGAL::Lazy_exact_nt<CGAL::Gmpq>                         NT;
typedef CGAL::Filtered_kernel<CGAL::Simple_cartesian<NT>>       Kernel;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif

typedef CORE::BigInt                                   BigInt;

typedef Kernel::FT                                     FT;
typedef Kernel::RT                                     RT;
typedef Kernel::Point_2                                Point_2;
typedef Kernel::Segment_2                              Segment_2;
typedef Kernel::Line_2                                 Line_2;
typedef Kernel::Ray_2                                  Ray_2;
typedef Kernel::Direction_2                            Direction_2;
typedef Kernel::Vector_2                               Vector_2;
typedef Kernel::Circle_2                               Circle_2;
typedef Kernel::Triangle_2                             Triangle_2;
typedef Kernel::Iso_rectangle_2                        Iso_rectangle_2;
typedef Kernel::Point_3                                Point_3;
typedef Kernel::Weighted_point_3                       Weighted_point_3;

typedef std::list<Point_2>                             Point_2_container;

typedef CGAL::Aff_transformation_2<Kernel>             Aff_transformation_2;
typedef CGAL::Aff_transformation_3<Kernel>             Aff_transformation_3;
typedef CGAL::Bbox_2                                   Bbox_2;
typedef CGAL::Rotation                                 Rotation;
typedef CGAL::Scaling                                  Scaling;
typedef CGAL::Translation                              Translation;

typedef CGAL::Object                                   Object;

#endif //CGALPY_KERNEL_TYPES_HPP
