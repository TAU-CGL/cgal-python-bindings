// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <CGAL/envelope_2.h>

#ifndef CGALPY_ENVELOPE_2_TYPES_HPP
#define CGALPY_ENVELOPE_2_TYPES_HPP

#include <CGAL/Envelope_diagram_1.h>

#include "cgalpy/arrangement_on_surface_2_types.hpp"

namespace cgalpy {
namespace env2 {

using Gt = aos2::Geometry_traits_2;
using Envelope_diagram_1 = CGAL::Envelope_diagram_1<Gt>;
using Geometry_traits_2 = Envelope_diagram_1::Traits_2;
using X_monotone_curve_2 = Envelope_diagram_1::X_monotone_curve_2;
using Curve_2 = Geometry_traits_2::Curve_2;
using Vertex = Envelope_diagram_1::Vertex;
using Edge = Envelope_diagram_1::Edge;

}
} // namespace cgalpy

#endif
