// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

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
#include <utility>

#include <CGAL/IO/Color.h>
#include <CGAL/Graphics_scene_options.h>

template <typename DS,
          typename VertexDescriptor,
          typename EdgeDescriptor,
          typename FaceDescriptor,
          typename VolumeDescriptor = void>
class Graphics_scene_options_extended :
  public CGAL::Graphics_scene_options<DS, VertexDescriptor, EdgeDescriptor, FaceDescriptor, VolumeDescriptor> {
private:
  using Ds = DS;
  using Vd = VertexDescriptor;
  using Ed = EdgeDescriptor;
  using Fd = FaceDescriptor;
  using Base = CGAL::Graphics_scene_options<DS, Vd, Ed, Fd, VolumeDescriptor>;
  using Color = CGAL::IO::Color;

public:
  using Draw_vertex_fnc = std::function<bool(const Ds&, Vd)>;
  using Draw_edge_fnc = std::function<bool(const Ds&, Ed)>;
  using Draw_face_fnc = std::function<bool(const Ds&, Fd)>;

  using Colored_vertex_fnc = std::function<bool(const Ds&, Vd)>;
  using Colored_edge_fnc = std::function<bool(const Ds&, Ed)>;
  using Colored_face_fnc = std::function<bool(const Ds&, Fd)>;

  using Face_wireframe_fnc = std::function<bool(const Ds&, Fd)>;

  using Vertex_color_fnc = std::function<Color(const Ds&, Vd)>;
  using Edge_color_fnc = std::function<Color(const Ds&, Ed)>;
  using Face_color_fnc = std::function<Color(const Ds&, Fd)>;

private:
  py::object m_draw_vertex_object;

  py::object m_draw_edge_object;

  py::object m_draw_face_object;

  py::object m_colored_vertex_object;

  py::object m_colored_edge_object;

  py::object m_edge_color_object;

  py::object m_colored_face_object;

  py::object m_face_wireframe_object;

  py::object m_vertex_color_object;

  py::object m_face_color_object;

public:
  /*! constructs default. */
  template<typename ... Args>
  Graphics_scene_options_extended(Args ... args) :
    Base(std::forward<Args>(args)...),
    m_draw_vertex_object(py::none()),
    m_draw_edge_object(py::none()),
    m_draw_face_object(py::none()),
    m_colored_vertex_object(py::none()),
    m_colored_edge_object(py::none()),
    m_edge_color_object(py::none()),
    m_colored_face_object(py::none()),
    m_face_wireframe_object(py::none()),
    m_vertex_color_object(py::none()),
    m_face_color_object(py::none())
  {}

  Graphics_scene_options_extended(
    const Graphics_scene_options_extended&) = delete;

  Graphics_scene_options_extended&
  operator=(const Graphics_scene_options_extended&) = delete;

  Graphics_scene_options_extended(
    Graphics_scene_options_extended&&) = delete;

  Graphics_scene_options_extended&
  operator=(Graphics_scene_options_extended&&) = delete;

  //!
  void apply_draw_vertex(const py::object& draw_vertex_object) {
    m_draw_vertex_object = draw_vertex_object;
    this->draw_vertex = [this](const Ds& ds, Vd vd) -> bool {
      return execute_draw_vertex(ds, *vd);
    };
  }

  //!
  void apply_draw_edge(const py::object& draw_edge_object) {
    m_draw_edge_object = draw_edge_object;
#if ((CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_2_DATA_STRUCTURE) || \
     (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE))
    this->draw_edge = [this](const Ds& ds, Ed ed) -> bool {
      return execute_draw_edge(ds, *ed);
    };
#else
    this->draw_edge = [this](const Ds& ds, Ed ed) -> bool {
      return execute_draw_edge(ds, *CGAL::halfedge(ed, ds));
    };
#endif
  }

  //!
  void apply_draw_face(const py::object& draw_face_object) {
    m_draw_face_object = draw_face_object;
    this->draw_face = [this](const Ds& ds, Fd fd) -> bool {
      return execute_draw_face(ds, *fd);
    };
  }

  //!
  void apply_colored_vertex(const py::object& colored_vertex_object) {
    m_colored_vertex_object = colored_vertex_object;
    this->colored_vertex = [this](const Ds& ds, Vd vd) -> bool {
      return execute_colored_vertex(ds, *vd);
    };
  }

  //!
  void apply_face_wireframe(const py::object& face_wireframe_object) {
    m_face_wireframe_object = face_wireframe_object;
    this->face_wireframe = [this](const Ds& ds, Fd fd) -> bool {
      return execute_face_wireframe(ds, *fd);
    };
  }

  //!
  void apply_vertex_color(const py::object& vertex_color_object) {
    m_vertex_color_object = vertex_color_object;
    this->vertex_color = [this](const Ds& ds, Vd vd) -> Color {
      return execute_vertex_color(ds, *vd);
    };
  }

  //!
  void apply_colored_edge(const py::object& colored_edge_object) {
    m_colored_edge_object = colored_edge_object;
#if ((CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_2_DATA_STRUCTURE) || \
     (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE))
    this->colored_edge = [this](const Ds& ds, Ed ed) -> bool {
      return execute_colored_edge(ds, *ed);
    };
#else
    this->colored_edge = [this](const Ds& ds, Ed ed) -> bool {
      return execute_colored_edge(ds, *CGAL::halfedge(ed, ds));
    };
#endif
  }

  //!
  void apply_edge_color(const py::object& edge_color_object) {
    m_edge_color_object = edge_color_object;
#if ((CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_2_DATA_STRUCTURE) || \
     (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE))
    this->edge_color = [this](const Ds& ds, Ed ed) -> Color {
      return execute_edge_color(ds, *ed);
    };
#else
    this->edge_color = [this](const Ds& ds, Ed ed) -> Color {
      return execute_edge_color(ds, *CGAL::halfedge(ed, ds));
    };
#endif
  }

  //!
  void apply_colored_face(const py::object& colored_face_object) {
    m_colored_face_object = colored_face_object;
    this->colored_face = [this](const Ds& ds, Fd fd) -> bool {
      return execute_colored_face(ds, *fd);
    };
  }

  //!
  void apply_face_color(const py::object& face_color_object) {
    m_face_color_object = face_color_object;
    this->face_color = [this](const Ds& ds, Fd fd) -> Color {
      return execute_face_color(ds, *fd);
    };
  }

  //!
  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    Graphics_scene_options_extended* w =
      py::inst_ptr<Graphics_scene_options_extended>(self);

    py::handle draw_vertex_object = py::find(w->m_draw_vertex_object);
    py::handle draw_edge_object = py::find(w->m_draw_edge_object);
    py::handle draw_face_object = py::find(w->m_draw_face_object);
    py::handle colored_vertex_object = py::find(w->m_colored_vertex_object);
    py::handle colored_edge_object = py::find(w->m_colored_edge_object);
    py::handle colored_face_object = py::find(w->m_colored_face_object);
    py::handle face_wireframe_object = py::find(w->m_face_wireframe_object);
    py::handle vertex_color_object = py::find(w->m_vertex_color_object);
    py::handle edge_color_object = py::find(w->m_edge_color_object);
    py::handle face_color_object = py::find(w->m_face_color_object);

    Py_VISIT(draw_vertex_object.ptr());
    Py_VISIT(draw_edge_object.ptr());
    Py_VISIT(draw_face_object.ptr());
    Py_VISIT(colored_vertex_object.ptr());
    Py_VISIT(colored_edge_object.ptr());
    Py_VISIT(colored_face_object.ptr());
    Py_VISIT(face_wireframe_object.ptr());
    Py_VISIT(vertex_color_object.ptr());
    Py_VISIT(edge_color_object.ptr());
    Py_VISIT(face_color_object.ptr());

#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  //!
  static int tp_clear(PyObject* self) {
    Graphics_scene_options_extended* w =
      py::inst_ptr<Graphics_scene_options_extended>(self);

    w->m_draw_vertex_object = {};
    w->m_draw_edge_object = {};
    w->m_draw_face_object = {};
    w->m_colored_vertex_object = {};
    w->m_colored_edge_object = {};
    w->m_colored_face_object = {};
    w->m_face_wireframe_object = {};
    w->m_vertex_color_object = {};
    w->m_edge_color_object = {};
    w->m_face_color_object = {};

    return 0;
  }

private:
  //!
  bool execute_draw_vertex(const Ds& ds, const typename Ds::Vertex& vertex)
  { return py::cast<bool>(m_draw_vertex_object(&ds, &vertex)); }

  //!
  bool execute_draw_edge(const Ds& ds, const typename Ds::Halfedge& edge)
  { return py::cast<bool>(m_draw_edge_object(&ds, &edge)); }

  //!
  bool execute_draw_face(const Ds& ds, const typename Ds::Face& face)
  { return py::cast<bool>(m_draw_face_object(&ds, &face)); }

  //!
  bool execute_colored_vertex(const Ds& ds, const typename Ds::Vertex& vertex)
  { return py::cast<bool>(m_colored_vertex_object(&ds, &vertex)); }

  //!
  bool execute_face_wireframe(const Ds& ds, const typename Ds::Face& face)
  { return py::cast<bool>(m_face_wireframe_object(&ds, &face)); }

  //!
  Color execute_vertex_color(const Ds& ds, const typename Ds::Vertex& vertex)
  { return py::cast<Color>(m_vertex_color_object(&ds, &vertex)); }

  //!
  bool execute_colored_edge(const Ds& ds, const typename Ds::Halfedge& edge)
  { return py::cast<bool>(m_colored_edge_object(&ds, &edge)); }

  //!
  Color execute_edge_color(const Ds& ds, const typename Ds::Halfedge& edge)
  { return py::cast<Color>(m_edge_color_object(&ds, &edge)); }

  //!
  bool execute_colored_face(const Ds& ds, const typename Ds::Face& face)
  { return py::cast<bool>(m_colored_face_object(&ds, &face)); }

  //!
  Color execute_face_color(const Ds& ds, const typename Ds::Face& face)
  { return py::cast<Color>(m_face_color_object(&ds, &face)); }
};

#endif
