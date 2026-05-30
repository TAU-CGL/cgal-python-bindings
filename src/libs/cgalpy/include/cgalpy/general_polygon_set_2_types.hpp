// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_GENERAL_POLYGON_SET_2_TYPES_HPP
#define CGALPY_GENERAL_POLYGON_SET_2_TYPES_HPP

#include <CGAL/General_polygon_set_2.h>

#include "cgalpy/general_polygon_set_2_config.hpp"
#include "cgalpy/arrangement_on_surface_2_types.hpp"

namespace cgalpy {
namespace bso2 {

using Dcel = aos2::Dcel;
using Geometry_traits_2 = aos2::Geometry_traits_2;
using Topology_traits = aos2::Topology_traits;

using Precondition_validation_policy =
  CGAL::Boolean_set_operation_2_internal::PreconditionValidationPolicy;
using Gps_on_surface_base_2 =
  CGAL::Gps_on_surface_base_2<Geometry_traits_2, Topology_traits,
                              Precondition_validation_policy>;

using General_polygon_set_on_surface_2 =
  CGAL::General_polygon_set_on_surface_2<Geometry_traits_2, Topology_traits>;
using General_polygon_set_2 =
  CGAL::General_polygon_set_2<Geometry_traits_2, Dcel>;
using General_polygon_2 = General_polygon_set_2::Polygon_2;
using General_polygon_with_holes_2 = General_polygon_set_2::Polygon_with_holes_2;
using Arrangement_2 = General_polygon_set_2::Arrangement_2;

}
} // namespace cgalpy

#endif
