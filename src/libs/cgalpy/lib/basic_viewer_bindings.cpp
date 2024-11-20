// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/basic_viewer_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

namespace bvr {
}

void export_basic_viewer(py::module_& m) {
  using Gso = bvr::Graphics_scene_options;

  if (! add_attr<Gso>(m, "Graphics_scene_options")) {
    py::class_<Gso> gso_c(m, "Graphics_scene_options");
    gso_c.def(py::init<>())
      ;
  }
}
