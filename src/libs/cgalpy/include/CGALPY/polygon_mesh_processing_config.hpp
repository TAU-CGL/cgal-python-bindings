// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYGON_MESH_PROCESSING_CONFIG_HPP
#define CGALPY_POLYGON_MESH_PROCESSING_CONFIG_HPP

#include "CGALPY/polyhedron_3_types.hpp"
#include "CGALPY/surface_mesh_types.hpp"

#define CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH          0
#define CGALPY_PMP_SURFACE_MESH_3_POLYGONAL_MESH        1

namespace pmp {

template <int i> struct Poly_mesh {};
template <> struct Poly_mesh<CGALPY_PMP_POLYHEDRON_3_POLYGONAL_MESH>
{ typedef pol3::Polyhedron_3 type; };
template <> struct Poly_mesh<CGALPY_PMP_SURFACE_MESH_3_POLYGONAL_MESH>
{ typedef sm::Surface_mesh_3 type; };

}

#endif
