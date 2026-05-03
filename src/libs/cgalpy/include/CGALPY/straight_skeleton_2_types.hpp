// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_STRAIGHT_SKELETON_2_TYPES_HPP
#define CGALPY_STRAIGHT_SKELETON_2_TYPES_HPP

#include <CGAL/Straight_skeleton_2.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/straight_skeleton_2_config.hpp"

namespace sn {

using Straight_skeleton_2 = CGAL::Straight_skeleton_2<Kernel>;
using Vertex = Straight_skeleton_2::Vertex;
using Halfedge = Straight_skeleton_2::Halfedge;
using Face = Straight_skeleton_2::Face;

}

#endif
