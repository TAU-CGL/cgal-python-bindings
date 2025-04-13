// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_GRAPHICS_SCENE_OPTIONS_EXTENDED_HPP
#define CGALPY_GRAPHICS_SCENE_OPTIONS_EXTENDED_HPP

#include <functional>
// #include <memory>

#include <CGAL/IO/Color.h>
#include <CGAL/Graphics_scene_options.h>

template <typename DS,
          typename VertexDescriptor,
          typename EdgeDescriptor,
          typename FaceDescriptor,
          typename VolumeDescriptor = void>
class Graphics_scene_options_extended :
  public CGAL::Graphics_scene_options<DS, VertexDescriptor, EdgeDescriptor,
                                      FaceDescriptor, VolumeDescriptor> {
private:
  using Ds = DS;
  using Vd = VertexDescriptor;
  using Ed = EdgeDescriptor;
  using Fd = FaceDescriptor;
  using Base = CGAL::Graphics_scene_options<DS, Vd, Ed, Fd, VolumeDescriptor>;
  using Color = CGAL::IO::Color;

public:
  using Colored_face_fnc = std::function<bool(const Ds&, Fd)>;
  using Face_color_fnc = std::function<Color(const Ds&, Fd)>;

private:
  py::object m_colored_face_object;
  Colored_face_fnc* m_colored_face_fnc;

  py::object m_face_color_object;
  Face_color_fnc* m_face_color_fnc;

public:
  /*! constructs default. */
  template<typename ... Args>
  Graphics_scene_options_extended(Args ... args) :
    Base(std::forward<Args>(args)...),
    m_colored_face_object(py::none()),
    m_colored_face_fnc(nullptr),
    m_face_color_object(py::none()),
    m_face_color_fnc(nullptr)
  {
    // Handle colored_face
    auto colored_face =
     [&](const Ds& arr, Fd fd) -> bool
     { return execute_colored_face(arr, *fd); };
    m_colored_face_fnc = new Colored_face_fnc(colored_face);

    // Handle face_color
    auto face_color =
      [&](const Ds& arr, Fd fd) -> Color
      { return execute_face_color(arr, *fd); };
     m_face_color_fnc = new Face_color_fnc(face_color);
  }

  /*! destructs */
  ~Graphics_scene_options_extended() {
    if (m_colored_face_fnc) {
      delete m_colored_face_fnc;
      m_colored_face_fnc = nullptr;
    }

    if (m_face_color_fnc) {
      delete m_face_color_fnc;
      m_face_color_fnc = nullptr;
    }
  }

  //!
  void apply_colored_face(const py::object& colored_face_object) {
    m_colored_face_object = colored_face_object;
    this->colored_face = *m_colored_face_fnc;
  }

  //!
  void apply_face_color(const py::object& face_color_object) {
    m_face_color_object = face_color_object;
    this->face_color = *m_face_color_fnc;
  }

  //!
  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    // Get the C++ object associated with 'self' (this always succeeds)
    Graphics_scene_options_extended* w =
      py::inst_ptr<Graphics_scene_options_extended>(self);

    // If w->value has an associated Python object, return it.
    // If not, value.ptr() will equal NULL, which is also fine.
    py::handle value_colored_face_object = py::find(w->m_colored_face_object);
    py::handle value_face_color_object = py::find(w->m_face_color_object);

    // Inform the Python GC about the instance
    Py_VISIT(value_colored_face_object.ptr());
    Py_VISIT(value_face_color_object.ptr());

    // On Python 3.9+, we must traverse the implicit dependency
    // of an object on its associated type object.
#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  //!
  static int tp_clear(PyObject* self) {
    // Get the C++ object associated with 'self' (this always succeeds)
    Graphics_scene_options_extended* w =
      py::inst_ptr<Graphics_scene_options_extended>(self);

    // Break reference cycles!
    w->m_colored_face_object = {};
    w->m_face_color_object = {};

    return 0;
  }

private:
  //!
  bool execute_colored_face(const Ds& arr, const typename Ds::Face& face)
  { return py::cast<bool>(m_colored_face_object(&arr, &face)); }

  //!
  Color execute_face_color(const Ds& arr, const typename Ds::Face& face)
  { return py::cast<Color>(m_face_color_object(&arr, &face)); }
};

#endif
