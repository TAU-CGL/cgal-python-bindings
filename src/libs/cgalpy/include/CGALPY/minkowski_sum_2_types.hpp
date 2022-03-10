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

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Gps_circle_segment_traits_2.h>

#include "CGALPY/kernel_types.hpp"

namespace ms2 {

typedef CGAL::Polygon_2<Kernel, Point_2_container>             Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel, Point_2_container>  Polygon_with_holes_2;

typedef CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_2
  Circle_segment_polygon_2;
typedef CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_with_holes_2
  Circle_segment_polygon_with_holes_2;

typedef CGAL::Polygon_nop_decomposition_2<Kernel, Point_2_container>
  Polygon_nop_decomposition_2;

}

#endif
