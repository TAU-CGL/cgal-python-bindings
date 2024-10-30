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

#include "CGALPY/kernel_type.hpp"

#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Direction_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Aff_transformation_2.h>

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
typedef Kernel::Plane_3                                Plane_3;
typedef Kernel::Weighted_point_3                       Weighted_point_3;
typedef Kernel::Iso_cuboid_3                           Iso_cuboid_3;
typedef Kernel::Line_3                                 Line_3;
typedef Kernel::Ray_3                                  Ray_3;
typedef Kernel::Segment_3                              Segment_3;
typedef Kernel::Tetrahedron_3                          Tetrahedron_3;
typedef Kernel::Triangle_3                             Triangle_3;
typedef Kernel::Sphere_3                               Sphere_3;
typedef Kernel::Vector_3                               Vector_3;

typedef std::vector<Point_2>                           Point_2_container;
typedef std::vector<Point_3>                           PointRange;
typedef std::vector<std::vector<std::size_t>>          PolygonRange;

typedef CGAL::Aff_transformation_2<Kernel>             Aff_transformation_2;
typedef CGAL::Aff_transformation_3<Kernel>             Aff_transformation_3;
typedef CGAL::Bbox_2                                   Bbox_2;
typedef CGAL::Bbox_3                                   Bbox_3;
typedef CGAL::Rotation                                 Rotation;
typedef CGAL::Scaling                                  Scaling;
typedef CGAL::Translation                              Translation;

// consteval for C++20, msvc needs additional option for this
#if __cplusplus >= 202002L
consteval
#else
constexpr
#endif // __cpp_consteval >= 202002L
const char* kernel_doc() {
#if CGALPY_KERNEL == CGALPY_KERNEL_EPIC
  return "Exact_predicates_inexact_constructions_kernel\n"
         "A kernel that has the following properties:\n"
         "\t• It uses Cartesian representation.\n"
         "\t• It supports constructions of points from double Cartesian coordinates.\n"
         "\t• It provides exact geometric predicates, but geometric constructions are not exact in general."
         ;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC
  return "Exact_predicates_exact_constructions_kernel\n"
         "A kernel that has the following properties:"
         "\t• It uses Cartesian representation.\n"
         "\t• It supports constructions of points from double Cartesian coordinates.\n"
         "\t• It provides both exact geometric predicates and exact geometric constructions."
         ;
#elif CGALPY_KERNEL == CGALPY_KERNEL_EPEC_WITH_SQRT
  return "Exact_predicates_exact_constructions_kernel_with_sqrt\n"
         "A kernel that has the following properties:\n"
         "\t• It uses Cartesian representation.\n"
         "\t• It supports constructions of points from double Cartesian coordinates.\n"
         "\t• It provides both exact geometric predicates and exact geometric constructions.\n"
         "\t• Its FT nested type is model of the FieldWithSqrt concept."
         ;
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_DOUBLE
  return "Filtered_simple_cartesian_double"
#elif CGALPY_KERNEL == CGALPY_KERNEL_FILTERED_SIMPLE_CARTESIAN_LAZY_GMPQ
  return "Filtered_simple_cartesian_lazy_gmpq";
#elif CGALPY_KERNEL == CGALPY_KERNEL_CARTESIAN_CORE_RATIONAL
  return "Cartesian_core_rational";
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif
}

#endif //CGALPY_KERNEL_TYPES_HPP
