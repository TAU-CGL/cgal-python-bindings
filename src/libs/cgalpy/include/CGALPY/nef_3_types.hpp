// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_NEF_3_TYPES_HPP
#define CGALPY_NEF_3_TYPES_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Nef_3/SNC_indexed_items.h>

#include "CGALPY/nef_3_config.hpp"

namespace cgalpy {
namespace nef3 {

using Nef_polyhedron_3 =
  CGAL::Nef_polyhedron_3<Kernel, CGAL::SNC_indexed_items>;
using Polygonal_mesh = Poly_mesh<CGALPY_NEF3_POLYGONAL_MESH>::type;

}
} // namespace cgalpy

#endif
