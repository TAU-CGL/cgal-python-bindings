// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

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

#if ((CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_2_DATA_STRUCTURE) || \
     (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE))
#include "cgalpy/Aos2_docstrings.hpp"
#elif (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_POLYHEDRON_3_DATA_STRUCTURE)
#include "cgalpy/Pol3_docstrings.hpp"
#else
#error "Unsupported CGALPY basic viewer data structure"
#endif

namespace py = nanobind;
namespace bvr_doc = cgalpy::bvr::docstrings;

#if ((CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_2_DATA_STRUCTURE) || \
     (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE))
namespace bvr_ds_doc = cgalpy::aos2::docstrings;
#elif (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_POLYHEDRON_3_DATA_STRUCTURE)
namespace bvr_ds_doc = cgalpy::pol3::docstrings;
#endif

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
           bvr_doc::Graphics_scene_options_Graphics_scene_options)
      .def("disable_vertices", &Gso::disable_vertices,
           bvr_doc::GraphicsSceneOptions_disable_vertices)
      .def("enable_vertices", &Gso::enable_vertices,
           bvr_doc::GraphicsSceneOptions_enable_vertices)
      .def("are_vertices_enabled", &Gso::are_vertices_enabled,
           bvr_doc::GraphicsSceneOptions_are_vertices_enabled)
      .def("ignore_all_vertices", &Gso::ignore_all_vertices, py::arg("b"),
           bvr_doc::GraphicsSceneOptions_ignore_all_vertices)
      .def("disable_edges", &Gso::disable_edges,
           bvr_doc::GraphicsSceneOptions_disable_edges)
      .def("enable_edges", &Gso::enable_edges,
           bvr_doc::GraphicsSceneOptions_enable_edges)
      .def("are_edges_enabled", &Gso::are_edges_enabled,
           bvr_doc::GraphicsSceneOptions_are_edges_enabled)
      .def("ignore_all_edges", &Gso::ignore_all_edges, py::arg("b"),
           bvr_doc::GraphicsSceneOptions_ignore_all_edges)
      .def("disable_faces", &Gso::disable_faces,
           bvr_doc::GraphicsSceneOptions_disable_faces)
      .def("enable_faces", &Gso::enable_faces,
           bvr_doc::GraphicsSceneOptions_enable_faces)
      .def("are_faces_enabled", &Gso::are_faces_enabled,
           bvr_doc::GraphicsSceneOptions_are_faces_enabled)
      .def("ignore_all_faces", &Gso::ignore_all_faces, py::arg("b"),
           bvr_doc::GraphicsSceneOptions_ignore_all_faces)
      ;
  }

  using Gsoe = cgalpy::bvr::Graphics_scene_options_extended;
  if (! add_attr<Gsoe>(m, "Graphics_scene_options")) {
    py::class_<Gsoe, Gso>(
        m, "Graphics_scene_options",
        "Python extension of CGAL::Graphics_scene_options that retains Python "
        "callback objects and installs C++ wrappers for the corresponding "
        "drawing callbacks.",
        py::type_slots(cgalpy::bvr::gsoe_slots))
      .def(py::init<>(),
           bvr_doc::Graphics_scene_options_Graphics_scene_options)
      .def("draw_vertex", &Gsoe::apply_draw_vertex,
           py::arg("draw_vertex"),
           bvr_doc::GraphicsSceneOptions_draw_vertex)
      .def("draw_edge", &Gsoe::apply_draw_edge,
           py::arg("draw_edge"),
           bvr_doc::GraphicsSceneOptions_draw_edge)
      .def("draw_face", &Gsoe::apply_draw_face,
           py::arg("draw_face"),
           bvr_doc::GraphicsSceneOptions_draw_face)
      .def("colored_vertex", &Gsoe::apply_colored_vertex,
           py::arg("colored_vertex"),
           bvr_doc::GraphicsSceneOptions_colored_vertex)
      .def("colored_edge", &Gsoe::apply_colored_edge,
           py::arg("colored_edge"),
           bvr_doc::GraphicsSceneOptions_colored_edge)
      .def("colored_face", &Gsoe::apply_colored_face,
           py::arg("colored_face"),
           bvr_doc::GraphicsSceneOptions_colored_face)
      .def("face_wireframe", &Gsoe::apply_face_wireframe,
           py::arg("face_wireframe"),
           bvr_doc::GraphicsSceneOptions_face_wireframe)
      .def("vertex_color", &Gsoe::apply_vertex_color,
           py::arg("vertex_color"),
           bvr_doc::GraphicsSceneOptions_vertex_color)
      .def("edge_color", &Gsoe::apply_edge_color,
           py::arg("edge_color"),
           bvr_doc::GraphicsSceneOptions_edge_color)
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
           bvr_doc::Graphics_scene_Graphics_scene)
      .def("empty", &Gs::empty,
           bvr_doc::Graphics_scene_empty)
      .def("clear", &Gs::clear,
           bvr_doc::Graphics_scene_clear)
      .def("is_two_dimensional", &Gs::is_two_dimensional,
           bvr_doc::Graphics_scene_is_two_dimensional)
      .def("number_of_elements", &Gs::number_of_elements, py::arg("index"),
           bvr_doc::Graphics_scene_number_of_elements)
      .def("get_size_of_index", &Gs::get_size_of_index, py::arg("index"),
           bvr_doc::Graphics_scene_get_size_of_index)
      .def("buffer", [](const Gs& gs, int index) {
          return gs.get_array_of_index(index);
        }, py::arg("index"),
        bvr_doc::Graphics_scene_get_array_of_index)
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
        bvr_ds_doc::add_to_graphics_scene_1)
    .def("add_to_graphics_scene",
         [](const cgalpy::bvr::Ds& ds, Gs& gs, const Gso& gso)
         { CGAL::add_to_graphics_scene(ds, gs, gso); },
         py::arg("ds"), py::arg("gs"), py::arg("gso"),
         bvr_ds_doc::add_to_graphics_scene)
    .def("draw_graphics_scene", &CGAL::draw_graphics_scene,
         py::arg("graphics_scene"),
         py::arg("title") = "CGAL Basic Viewer (Qt)",
         bvr_doc::draw_graphics_scene);
}
