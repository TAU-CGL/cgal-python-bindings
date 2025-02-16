// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYGON_SET_2_TYPES_HPP
#define CGALPY_POLYGON_SET_2_TYPES_HPP

#include <CGAL/Polygon_set_2.h>

#include "CGALPY/polygon_set_2_config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/general_polygon_set_2_types.hpp"

namespace bso2 {

using Polygon_set_2 = CGAL::Polygon_set_2<Kernel, Point_2_container, Dcel>;

}

#endif
