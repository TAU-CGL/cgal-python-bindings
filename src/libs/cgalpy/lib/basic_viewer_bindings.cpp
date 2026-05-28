// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <nanobind/nanobind.h>

#include <CGAL/draw_arrangement_2.h>
#include <CGAL/draw_polyhedron.h>
#include <CGAL/Graphics_scene.h>
#include <CGAL/Graphics_scene_options.h>
#include <CGAL/IO/Color.h>

#include "CGALPY/basic_viewer_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

namespace cgalpy {
namespace bvr {

//!
static ::PyType_Slot gsoe_slots[] = {
  {Py_tp_traverse, (void*) Graphics_scene_options_extended::tp_traverse},
  {Py_tp_clear, (void*) Graphics_scene_options_extended::tp_clear},
  {0, nullptr}
};

}
} // namespace cgalpy

/*! The implementation of the bindings of the CGAL::Graphics_scene_options class
 * template presents a specific challenge. The interface of this class template
 * consists of several callback functions optionally set by the user. Let `gso`
 * be an object the type of which is an instance of the
 * CGAL::Graphics_scene_options class template. The statement, for instance,
 * `gso.colored_face = colored_face` sets the callback to a user provided
 * function called `colored_face`, which determines whether to draw faces.  This
 * function must have a specific signature, namely `bool colored_function(const
 * Arrangement&, Face_descriptor fd)`.  Naturally, when implementing the
 * bindings we would like to let the Python user provide a python function that
 * determines whether to draw faces. Therefore, we create a function in C++ that
 * simply wraps the call to the user (developer) provided Python function. We
 * need to store both functions, namely, the Python function and the wrapper and
 * associate them with the `gso` object.  One solution is to maintain a global
 * mapping from CGAL::Graphics_scene_options objects to pairs of functions as
 * described above. Maintaining static data by shared libraries is prone to
 * errors.  Instead, we introduce a class template
 * Graphics_scene_options_extended that derives from
 * CGAL::Graphics_scene_options and maintains the two functions above. The
 * Python user should only use Graphics_scene_options_extended; thus we bind
 * this class to the attribute name "Graphics_scene_options", while the CGAL
 * class template Graphics_scene_options is bound to the attribute name
 * "Graphics_scene_options_base". The latter is probably of little, or no use,
 * for a Python user.
 */
void export_basic_viewer(py::module_& m) {
  using Gso = cgalpy::bvr::Graphics_scene_options;
  if (! add_attr<Gso>(m, "Graphics_scene_options_base")) {
    py::class_<Gso>(m, "Graphics_scene_options_base")
      .def(py::init<>())
      .def("ignore_all_faces", &Gso::ignore_all_faces)
      ;
  }

  using Gsoe = cgalpy::bvr::Graphics_scene_options_extended;
  if (! add_attr<Gsoe>(m, "Graphics_scene_options")) {
    py::class_<Gsoe, Gso>(m, "Graphics_scene_options", py::type_slots(cgalpy::bvr::gsoe_slots))
      .def(py::init<>())
      .def("colored_face", &Gsoe::apply_colored_face)
      .def("face_color", &Gsoe::apply_face_color)
      ;
  }

  using Gs = CGAL::Graphics_scene;
  if (! add_attr<Gs>(m, "Graphics_scene")) {
    py::class_<Gs>(m, "Graphics_scene")
      .def(py::init<>())
      ;
  }

  m.def("add_to_graphics_scene", [](const cgalpy::bvr::Ds& ds, Gs& gs) { CGAL::add_to_graphics_scene(ds, gs); })
    .def("add_to_graphics_scene",
         [](const cgalpy::bvr::Ds& ds, Gs& gs, const Gso& gso) { CGAL::add_to_graphics_scene(ds, gs, gso); })
    .def("draw_graphics_scene", &CGAL::draw_graphics_scene);
}
