// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ENVELOPE_3TYPES_HPP
#define CGALPY_ENVELOPE_3TYPES_HPP

#include <CGAL/envelope_3.h>
#include <CGAL/Env_surface_data_traits_3.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/envelope_3_config.hpp"

namespace env3 {

using Bgt = Base_tr<CGALPY_ENV3_GEOMETRY_TRAITS, Kernel>::type;
using Cnv = CGAL::_Default_convert_func<py::object, py::object>;
using Gt = Tr<surface_data(), Bgt, py::object, py::object, Cnv>::type;
using Dcel = CGAL::Envelope_3::Envelope_pm_dcel<Gt, Gt::Xy_monotone_surface_3>;
using Tt = CGAL::Default_planar_topology<Gt, Dcel>::Traits;
using Envelope_diagram_on_surface_2 =
  CGAL::Envelope_diagram_on_surface_2<Gt, Tt>;
using Envelope_diagram_2 = CGAL::Envelope_diagram_2<Gt, Dcel>;

using Geometry_traits_3 = Envelope_diagram_on_surface_2::Traits_3;
using Topology_traits = Envelope_diagram_on_surface_2::TopTraits;
using Surface_3 = Geometry_traits_3::Surface_3;

}

#endif
