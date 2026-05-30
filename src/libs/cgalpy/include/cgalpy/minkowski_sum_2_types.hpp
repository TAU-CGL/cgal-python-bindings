// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_MINKOWSKI_SUM_2_TYPES_HPP
#define CGALPY_MINKOWSKI_SUM_2_TYPES_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Gps_traits_2.h>

#include "cgalpy/kernel_types.hpp"

namespace cgalpy {
namespace ms2 {

using Polygon_2 = CGAL::Polygon_2<Kernel, Point_2_container>;
using Polygon_with_holes_2 =
  CGAL::Polygon_with_holes_2<Kernel, Point_2_container>;

using Circle_segment_polygon_2 =
  CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_2;
using Circle_segment_polygon_with_holes_2 =
  CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_with_holes_2;

using Polygon_nop_decomposition_2 =
  CGAL::Polygon_nop_decomposition_2<Kernel, Point_2_container>;

using Nt_traits = CGAL::CORE_algebraic_number_traits;
using Rational = Nt_traits::Rational;
using Rat_kernel = CGAL::Cartesian<Rational>;
using Rat_FT = Rat_kernel::FT;
using Rat_point = Rat_kernel::Point_2;
using Rat_segment = Rat_kernel::Segment_2;
using Rat_circle = Rat_kernel::Circle_2;
using Rat_polygon_2 = CGAL::Polygon_2<Rat_kernel>;
using Algebraic = Nt_traits::Algebraic;
using Alg_kernel = CGAL::Cartesian<Algebraic>;
using Conic_traits = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
using Conic_polygon_2 = CGAL::Gps_traits_2<Conic_traits>::Polygon_2;
using Conic_polygon_with_holes_2 =
  CGAL::Gps_traits_2<Conic_traits>::Polygon_with_holes_2;
}
} // namespace cgalpy

#endif
