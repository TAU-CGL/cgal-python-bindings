// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/arrangement_on_surface_2_types.hpp"

#ifndef CGALPY_BASIC_VIEWER_CONFIG_HPP
#define CGALPY_BASIC_VIEWER_CONFIG_HPP

#define CGALPY_BVR_ARR_2_DATA_STRUCTURE                 0
#define CGALPY_BVR_ARR_WITH_HISTORY_2_DATA_STRUCTURE    1
#define CGALPY_BVR_POLYHEDRON_3_DATA_STRUCTURE          2

namespace bvr {

template <int id> struct Ds_params {};

template <> struct Ds_params<CGALPY_BVR_ARR_2_DATA_STRUCTURE> {
  using Ds = aos2::Arrangement_2;
  using Vertex_descriptor = aos2::Vertex_const_handle;
  using Edge_descriptor = aos2::Halfedge_const_handle;
  using Face_descriptor = aos2::Face_const_handle;
  using Volume_descriptor = void;
};

}

#endif
