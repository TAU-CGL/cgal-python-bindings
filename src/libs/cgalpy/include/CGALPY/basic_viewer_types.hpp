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

namespace bvr {

using My_ds_params = Ds_params<CGALPY_BVR_DATA_STRUCTURE>;
using Graphics_scene_options =
  CGAL::Graphics_scene_options<My_ds_params::Ds,
                               My_ds_params::VertexDescriptor,
                               My_ds_params::EdgeDescriptor,
                               My_ds_params::FaceDescriptor,
                               My_ds_params::VolumeDescriptor>;
}

#endif
