// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYGON_2_TYPES_HPP
#define CGALPY_POLYGON_2_TYPES_HPP

#include "cgalpy/polygon_2_config.hpp"
#include "cgalpy/kernel_types.hpp"

#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>

namespace cgalpy {
namespace pol2 {

using Polygon_2 = CGAL::Polygon_2<Kernel, Point_2_container>;
using Polygon_with_holes_2 =
  CGAL::Polygon_with_holes_2<Kernel, Point_2_container>;
using General_polygon_with_holes_2 =
  CGAL::General_polygon_with_holes_2<Polygon_2>;
}
} // namespace cgalpy

#endif
