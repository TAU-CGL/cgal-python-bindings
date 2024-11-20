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

void apply_colored_face(Graphics_scene_options& gso, py::object) {
  gso.colored_face =
    [](const Ds&, Face_descriptor) -> bool
    { return true; };
}

void apply_face_color(Graphics_scene_options& gso, py::object) {
  std::size_t id(0);
  gso.face_color =
    [&id](const Ds& arr, Face_descriptor) ->
    CGAL::IO::Color {
      double h = 360 * id++ / arr.number_of_faces();
      CGAL::IO::Color c;
      c.set_hsv(h, 50, 50);
      return c;
    };
}

}

void export_basic_viewer(py::module_& m) {
  using Gso = bvr::Graphics_scene_options;

  if (! add_attr<Gso>(m, "Graphics_scene_options")) {
    py::class_<Gso> gso_c(m, "Graphics_scene_options");
    gso_c.def(py::init<>())
      .def("colored_face", &bvr::apply_colored_face)
      .def("face_color", &bvr::apply_face_color)
      ;
  }
}
