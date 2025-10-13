// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_BASIC_VIEWER_TYPES_HPP
#define CGALPY_BASIC_VIEWER_TYPES_HPP

#include <CGAL/Graphics_scene_options.h>

#include "CGALPY/basic_viewer_config.hpp"
#include "CGALPY/Graphics_scene_options_extended.hpp"

namespace bvr {

using My_ds_params = Ds_params<CGALPY_BVR_DATA_STRUCTURE>;
using Ds = My_ds_params::Ds;
using Vertex_descriptor = My_ds_params::Vertex_descriptor;
using Edge_descriptor = My_ds_params::Edge_descriptor;
using Face_descriptor = My_ds_params::Face_descriptor;
using Volume_descriptor = My_ds_params::Volume_descriptor;

using Graphics_scene_options =
  CGAL::Graphics_scene_options<Ds, Vertex_descriptor, Edge_descriptor, Face_descriptor, Volume_descriptor>;
using Graphics_scene_options_extended =
  Graphics_scene_options_extended<Ds, Vertex_descriptor, Edge_descriptor, Face_descriptor, Volume_descriptor>;

}

#endif
