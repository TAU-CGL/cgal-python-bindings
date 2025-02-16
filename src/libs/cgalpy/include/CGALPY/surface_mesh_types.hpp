// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_SURFACE_MESH_TYPES_HPP
#define CGALPY_SURFACE_MESH_TYPES_HPP

#include <CGAL/Surface_mesh.h>

#include "CGALPY/kernel_types.hpp"

namespace sm {

  using Surface_mesh_3 = CGAL::Surface_mesh<Kernel::Point_3>;
  using Surface_mesh_2 = CGAL::Surface_mesh<Kernel::Point_2>;

}

#endif
