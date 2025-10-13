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

using BigInt = CORE::BigInt;

using FT = Kernel::FT;
using RT = Kernel::RT;

using Point_2 = Kernel::Point_2;
using Segment_2 = Kernel::Segment_2;
using Line_2 = Kernel::Line_2;
using Ray_2 = Kernel::Ray_2;
using Direction_2 = Kernel::Direction_2;
using Vector_2 = Kernel::Vector_2;
using Circle_2 = Kernel::Circle_2;
using Triangle_2 = Kernel::Triangle_2;
using Iso_rectangle_2 = Kernel::Iso_rectangle_2;

using Direction_3 = Kernel::Direction_3;
using Point_3 = Kernel::Point_3;
using Plane_3 = Kernel::Plane_3;
using Weighted_point_3 = Kernel::Weighted_point_3;
using Iso_cuboid_3 = Kernel::Iso_cuboid_3;
using Line_3 = Kernel::Line_3;
using Ray_3 = Kernel::Ray_3;
using Segment_3 = Kernel::Segment_3;
using Tetrahedron_3 = Kernel::Tetrahedron_3;
using Triangle_3 = Kernel::Triangle_3;
using Sphere_3 = Kernel::Sphere_3;
using Vector_3 = Kernel::Vector_3;

using Point_2_container = std::vector<Point_2>;
using PointRange = std::vector<Point_3>;
using PolygonRange = std::vector<std::vector<std::size_t>>;

using Aff_transformation_2 = CGAL::Aff_transformation_2<Kernel>;
using Aff_transformation_3 = CGAL::Aff_transformation_3<Kernel>;
using Bbox_2 = CGAL::Bbox_2;
using Bbox_3 = CGAL::Bbox_3;
using Rotation = CGAL::Rotation;
using Scaling = CGAL::Scaling;
using Translation = CGAL::Translation;

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
#elif CGALPY_KERNEL == CGALPY_KERNEL_EXACT_CIRCULAR_KERNEL_2
  return "Exact_circular_kernel_2";
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_KERNEL");
#endif
}

#endif
