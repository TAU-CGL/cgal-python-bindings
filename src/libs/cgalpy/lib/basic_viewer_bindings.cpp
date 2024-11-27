// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/IO/Color.h>

#include "CGALPY/basic_viewer_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

namespace bvr {

void export_basic_viewer(py::module_& m) {
  using Gso = bvr::Graphics_scene_options;
  if (! add_attr<Gso>(m, "Graphics_scene_options_base")) {
    py::class_<Gso>(m, "Graphics_scene_options_base")
      .def(py::init<>())
      ;
  }

  using Gsoe = bvr::Graphics_scene_options_extended;
  if (! add_attr<Gsoe>(m, "Graphics_scene_options")) {
    py::class_<Gsoe, Gso>(m, "Graphics_scene_options")
      .def(py::init<>())
      .def("colored_face", &Gsoe::apply_colored_face, py::keep_alive<1, 2>())
      .def("face_color", &Gsoe::apply_face_color, py::keep_alive<1, 2>())
      ;
  }
}
