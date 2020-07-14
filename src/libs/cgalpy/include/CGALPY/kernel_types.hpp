// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_KERNEL_TYPES_HPP
#define CGALPY_KERNEL_TYPES_HPP

#include <CGALPY/kernel_config.hpp>
#include <boost/python.hpp>
#include <boost/static_assert.hpp>
namespace bp = boost::python;

#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
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
typedef typename CGAL::Exact_predicates_inexact_constructions_kernel    Kernel;
typedef typename bp::return_value_policy<bp::copy_const_reference>      Kernel_return_value_policy;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC
typedef typename CGAL::Exact_predicates_exact_constructions_kernel      Kernel;
typedef typename bp::return_value_policy<bp::return_by_value>           Kernel_return_value_policy;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif

typedef typename CORE::BigInt                                           BigInt;
typedef typename CGAL::Gmpz                                             Gmpz;
typedef typename CGAL::Gmpq                                             Gmpq;
typedef typename Kernel::FT                                             FT;

#if CGALPY_KERNEL == CGALPY_KERNEL_EPEC
typedef typename CGAL::Exact_predicates_exact_constructions_kernel      Kernel;
typedef typename bp::return_value_policy<bp::return_by_value>           Kernel_return_value_policy;
#endif

//typedef typename CGAL::Sqrt_extension <FT, FT>                          CoordNT;
typedef typename Kernel::RT                                             RT;
//typedef typename CGAL::Arr_circle_segment_traits_2<Kernel>::CoordNT     CoordNT;
typedef typename CGAL::Object                                           Object;
typedef typename Kernel::Point_2                                        Point_2;
typedef typename Kernel::Segment_2                                      Segment_2;
typedef typename Kernel::Line_2                                         Line_2;
typedef typename Kernel::Ray_2                                          Ray_2;
typedef typename CGAL::Bbox_2                                           Bbox_2;
typedef typename Kernel::Direction_2                                    Direction_2;
typedef typename Kernel::Vector_2                                       Vector_2;
typedef typename Kernel::Circle_2                                       Circle_2;
typedef typename Kernel::Triangle_2                                     Triangle_2;
typedef typename std::list<Point_2>                                     Point_2_container;
typedef typename Kernel::Iso_rectangle_2                                Iso_rectangle_2;
typedef typename CGAL::Aff_transformation_2<Kernel>                     Aff_transformation_2;

typedef typename Kernel::Point_3                                        Point_3;
typedef typename Kernel::Weighted_point_3                               Weighted_point_3;
typedef typename CGAL::Aff_transformation_3<Kernel>                     Aff_transformation_3;

typedef typename CGAL::Rotation                                         Rotation;
typedef typename CGAL::Scaling                                          Scaling;
typedef typename CGAL::Translation                                      Translation;

#endif //CGALPY_KERNEL_TYPES_HPP
