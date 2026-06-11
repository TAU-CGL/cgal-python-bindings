// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <CGAL/draw_arrangement_2.h>
#include <CGAL/draw_polyhedron.h>
#include <CGAL/Graphics_scene.h>
#include <CGAL/Graphics_scene_options.h>
#include <CGAL/IO/Color.h>

#include "cgalpy/basic_viewer_types.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/Bvr_docstrings.hpp"

namespace py = nanobind;
namespace bvr_doc = cgalpy::bvr::docstrings;

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
    py::class_<Gso>(
        m, "Graphics_scene_options_base",
        bvr_doc::Graphics_scene_options_class)
      .def(py::init<>(),
           "Construct default graphics scene options.")
      .def("ignore_all_faces", &Gso::ignore_all_faces, py::arg("b"),
           bvr_doc::GraphicsSceneOptions_ignore_all_faces)
      ;
  }

  using Gsoe = cgalpy::bvr::Graphics_scene_options_extended;
  if (! add_attr<Gsoe>(m, "Graphics_scene_options")) {
    py::class_<Gsoe, Gso>(
        m, "Graphics_scene_options",
        bvr_doc::GraphicsSceneOptions_class,
        py::type_slots(cgalpy::bvr::gsoe_slots))
      .def(py::init<>(),
           "Construct default Python-extended graphics scene options.")
      .def("colored_face", &Gsoe::apply_colored_face,
           py::arg("colored_face"),
           bvr_doc::GraphicsSceneOptions_is_face_colored)
      .def("face_color", &Gsoe::apply_face_color,
           py::arg("face_color"),
           bvr_doc::GraphicsSceneOptions_face_color)
      ;
  }

  using Gs = CGAL::Graphics_scene;
  if (! add_attr<Gs>(m, "Graphics_scene")) {
    py::class_<Gs>(m, "Graphics_scene",
                   bvr_doc::Graphics_scene_class)
      .def(py::init<>(),
           "Construct an empty graphics scene.")
      .def("empty", &Gs::empty,
           "Return whether the graphics scene has no buffered geometry.")
      .def("clear", &Gs::clear,
           "Clear all buffered geometry and reset the bounding box.")
      .def("is_two_dimensional", &Gs::is_two_dimensional,
           "Return whether the buffered scene is two-dimensional.")
      .def("number_of_elements", &Gs::number_of_elements, py::arg("index"),
           "Return the number of 3-float elements stored in the selected graphics buffer.")
      .def("get_size_of_index", &Gs::get_size_of_index, py::arg("index"),
           "Return the selected graphics buffer size in bytes.")
      .def("buffer", [](const Gs& gs, int index) {
          return gs.get_array_of_index(index);
        }, py::arg("index"),
        "Return a copy of the selected graphics buffer as a list of floats.")
      ;
  }

  m.attr("POS_POINTS") = py::int_(static_cast<int>(Gs::POS_POINTS));
  m.attr("POS_SEGMENTS") = py::int_(static_cast<int>(Gs::POS_SEGMENTS));
  m.attr("POS_RAYS") = py::int_(static_cast<int>(Gs::POS_RAYS));
  m.attr("POS_LINES") = py::int_(static_cast<int>(Gs::POS_LINES));
  m.attr("POS_FACES") = py::int_(static_cast<int>(Gs::POS_FACES));
  m.attr("COLOR_POINTS") = py::int_(static_cast<int>(Gs::COLOR_POINTS));
  m.attr("COLOR_SEGMENTS") = py::int_(static_cast<int>(Gs::COLOR_SEGMENTS));
  m.attr("COLOR_RAYS") = py::int_(static_cast<int>(Gs::COLOR_RAYS));
  m.attr("COLOR_LINES") = py::int_(static_cast<int>(Gs::COLOR_LINES));
  m.attr("COLOR_FACES") = py::int_(static_cast<int>(Gs::COLOR_FACES));
  m.attr("SMOOTH_NORMAL_FACES") = py::int_(static_cast<int>(Gs::SMOOTH_NORMAL_FACES));
  m.attr("FLAT_NORMAL_FACES") = py::int_(static_cast<int>(Gs::FLAT_NORMAL_FACES));
  m.attr("LAST_INDEX") = py::int_(static_cast<int>(Gs::LAST_INDEX));

  m.def("add_to_graphics_scene",
        [](const cgalpy::bvr::Ds& ds, Gs& gs)
        { CGAL::add_to_graphics_scene(ds, gs); },
        py::arg("ds"), py::arg("gs"),
        "Add a data structure to a graphics scene using default graphics scene options.")
    .def("add_to_graphics_scene",
         [](const cgalpy::bvr::Ds& ds, Gs& gs, const Gso& gso)
         { CGAL::add_to_graphics_scene(ds, gs, gso); },
         py::arg("ds"), py::arg("gs"), py::arg("gso"),
         "Add a data structure to a graphics scene using the given graphics scene options.")
    .def("draw_graphics_scene", &CGAL::draw_graphics_scene,
         py::arg("graphics_scene"),
         py::arg("title") = "CGAL Basic Viewer (Qt)",
         bvr_doc::draw_graphics_scene);
}
