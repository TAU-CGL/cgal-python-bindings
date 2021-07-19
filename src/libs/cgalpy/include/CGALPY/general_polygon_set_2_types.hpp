// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_GENERAL_POLYGON_SET_2_TYPES_HPP
#define CGALPY_GENERAL_POLYGON_SET_2_TYPES_HPP

#include <CGAL/General_polygon_set_2.h>

#include "CGALPY/general_polygon_set_2_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace bso2 {

typedef typename CGAL::Gps_circle_segment_traits_2<Kernel> CSTraits;
typedef typename CSTraits::Polygon_2                       General_polygon_2;
typedef typename CSTraits::Polygon_with_holes_2
  General_polygon_with_holes_2;
typedef typename CGAL::General_polygon_set_2<CSTraits>     General_polygon_set_2;
typedef General_polygon_2::X_monotone_curve_2
  CS_traits_X_monotone_curve_2;
typedef General_polygon_2::Curve_iterator                  Curve_iterator;
typedef CSTraits::Point_2                                  CSPoint_2;

}

#endif
