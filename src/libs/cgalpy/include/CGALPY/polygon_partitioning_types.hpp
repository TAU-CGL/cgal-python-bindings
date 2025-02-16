// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYGON_PARTITIONING_TYPES_HPP
#define CGALPY_POLYGON_PARTITIONING_TYPES_HPP

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_vertical_decomposition_2.h>
#include <CGAL/Polygon_triangulation_decomposition_2.h>
#include <CGAL/Small_side_angle_bisector_decomposition_2.h>

#include "CGALPY/polygon_partitioning_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace pp2 {

using Polygon_2 = CGAL::Polygon_2<Kernel, Point_2_container>;
using Polygon_with_holes_2 =
  CGAL::Polygon_with_holes_2<Kernel, Point_2_container>;

using Point_2_container = CGAL::Polygon_vertical_decomposition_2<Kernel,;
  Polygon_vertical_decomposition_2;
using Polygon_triangulation_decomposition_2 =
  CGAL::Polygon_triangulation_decomposition_2<Kernel, Point_2_container>;
using Small_side_angle_bisector_decomposition_2 =
  CGAL::Small_side_angle_bisector_decomposition_2<Kernel, Point_2_container>;
}

#endif
