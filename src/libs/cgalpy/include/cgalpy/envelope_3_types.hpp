// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ENVELOPE_3_TYPES_HPP
#define CGALPY_ENVELOPE_3_TYPES_HPP

#include <CGAL/envelope_3.h>
#include <CGAL/Env_surface_data_traits_3.h>

#include "cgalpy/kernel_types.hpp"
#include "cgalpy/envelope_3_config.hpp"
#include "cgalpy/arrangement_on_surface_2_types.hpp"

namespace cgalpy {
namespace env3 {

using Egt = aos2::Egt;
using Base_egt = aos2::Base_egt;

using Gt = aos2::Geometry_traits_2;
using Dcel = aos2::Dcel;
using Tt = aos2::Topology_traits;

using Envelope_diagram_on_surface_2 =
  CGAL::Envelope_diagram_on_surface_2<Gt, Tt>;
using Envelope_diagram_2 = CGAL::Envelope_diagram_2<Gt, Dcel>;

using Geometry_traits_3 = Envelope_diagram_on_surface_2::Traits_3;
using Topology_traits = Envelope_diagram_on_surface_2::TopTraits;
using Surface_3 = Geometry_traits_3::Surface_3;
using Xy_monotone_surface_3 = Geometry_traits_3::Xy_monotone_surface_3;

}
} // namespace cgalpy

#endif
