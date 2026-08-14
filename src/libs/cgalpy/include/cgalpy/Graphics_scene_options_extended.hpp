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
// #include <memory>

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
  Draw_vertex_fnc* m_draw_vertex_fnc;

  py::object m_draw_edge_object;
  Draw_edge_fnc* m_draw_edge_fnc;

  py::object m_draw_face_object;
  Draw_face_fnc* m_draw_face_fnc;

  py::object m_colored_vertex_object;
  Colored_vertex_fnc* m_colored_vertex_fnc;

  py::object m_colored_edge_object;
  Colored_edge_fnc* m_colored_edge_fnc;

  py::object m_edge_color_object;
  Edge_color_fnc* m_edge_color_fnc;

  py::object m_colored_face_object;
  Colored_face_fnc* m_colored_face_fnc;

  py::object m_face_wireframe_object;
  Face_wireframe_fnc* m_face_wireframe_fnc;

  py::object m_vertex_color_object;
  Vertex_color_fnc* m_vertex_color_fnc;

  py::object m_face_color_object;
  Face_color_fnc* m_face_color_fnc;

public:
  /*! constructs default. */
  template<typename ... Args>
  Graphics_scene_options_extended(Args ... args) :
    Base(std::forward<Args>(args)...),
    m_draw_vertex_object(py::none()),
    m_draw_vertex_fnc(nullptr),
    m_draw_edge_object(py::none()),
    m_draw_edge_fnc(nullptr),
    m_draw_face_object(py::none()),
    m_draw_face_fnc(nullptr),
    m_colored_vertex_object(py::none()),
    m_colored_vertex_fnc(nullptr),
    m_colored_edge_object(py::none()),
    m_colored_edge_fnc(nullptr),
    m_edge_color_object(py::none()),
    m_edge_color_fnc(nullptr),
    m_colored_face_object(py::none()),
    m_colored_face_fnc(nullptr),
    m_face_wireframe_object(py::none()),
    m_face_wireframe_fnc(nullptr),
    m_vertex_color_object(py::none()),
    m_vertex_color_fnc(nullptr),
    m_face_color_object(py::none()),
    m_face_color_fnc(nullptr) {
    // Handle draw_vertex
    auto draw_vertex = [&](const Ds& ds, Vd vd) -> bool {
      return execute_draw_vertex(ds, *vd);
    };
    m_draw_vertex_fnc = new Draw_vertex_fnc(draw_vertex);

    // Handle draw_edge
#if ((CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_2_DATA_STRUCTURE) || \
     (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE))
    auto draw_edge = [&](const Ds& ds, Ed ed) -> bool {
      return execute_draw_edge(ds, *ed);
    };
#else
    auto draw_edge = [&](const Ds& ds, Ed ed) -> bool {
      return execute_draw_edge(ds, *CGAL::halfedge(ed, ds));
    };
#endif
    m_draw_edge_fnc = new Draw_edge_fnc(draw_edge);

    // Handle draw_face
    auto draw_face = [&](const Ds& ds, Fd fd) -> bool {
      return execute_draw_face(ds, *fd);
    };
    m_draw_face_fnc = new Draw_face_fnc(draw_face);

    // Handle colored_vertex
    auto colored_vertex = [&](const Ds& ds, Vd vd) -> bool {
      return execute_colored_vertex(ds, *vd);
    };
    m_colored_vertex_fnc = new Colored_vertex_fnc(colored_vertex);

    // Handle colored_edge
#if ((CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_2_DATA_STRUCTURE) || \
     (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE))
    auto colored_edge = [&](const Ds& ds, Ed ed) -> bool { return execute_colored_edge(ds, *ed); };
#else
    auto colored_edge = [&](const Ds& ds, Ed ed) -> bool { return execute_colored_edge(ds, *CGAL::halfedge(ed, ds)); };
#endif
    m_colored_edge_fnc = new Colored_edge_fnc(colored_edge);

    // Handle edge_color
#if ((CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_2_DATA_STRUCTURE) || \
     (CGALPY_BVR_DATA_STRUCTURE == CGALPY_BVR_AOS_WITH_HISTORY_2_DATA_STRUCTURE))
    auto edge_color = [&](const Ds& ds, Ed ed) -> Color { return execute_edge_color(ds, *ed); };
#else
    auto edge_color = [&](const Ds& ds, Ed ed) -> Color { return execute_edge_color(ds, *CGAL::halfedge(ed, ds)); };
#endif
    m_edge_color_fnc = new Edge_color_fnc(edge_color);

    // Handle colored_face
    auto colored_face = [&](const Ds& ds, Fd fd) -> bool { return execute_colored_face(ds, *fd); };
    m_colored_face_fnc = new Colored_face_fnc(colored_face);

    // Handle face_wireframe
    auto face_wireframe = [&](const Ds& ds, Fd fd) -> bool {
      return execute_face_wireframe(ds, *fd);
    };
    m_face_wireframe_fnc = new Face_wireframe_fnc(face_wireframe);

    // Handle vertex_color
    auto vertex_color = [&](const Ds& ds, Vd vd) -> Color {
      return execute_vertex_color(ds, *vd);
    };
    m_vertex_color_fnc = new Vertex_color_fnc(vertex_color);

    // Handle face_color
    auto face_color = [&](const Ds& ds, Fd fd) -> Color {
      return execute_face_color(ds, *fd);
    };
    m_face_color_fnc = new Face_color_fnc(face_color);
  }

  /*! destructs */
  ~Graphics_scene_options_extended() {
    if (m_draw_vertex_fnc) {
      delete m_draw_vertex_fnc;
      m_draw_vertex_fnc = nullptr;
    }

    if (m_draw_edge_fnc) {
      delete m_draw_edge_fnc;
      m_draw_edge_fnc = nullptr;
    }

    if (m_draw_face_fnc) {
      delete m_draw_face_fnc;
      m_draw_face_fnc = nullptr;
    }

    if (m_colored_vertex_fnc) {
      delete m_colored_vertex_fnc;
      m_colored_vertex_fnc = nullptr;
    }

    if (m_colored_edge_fnc) {
      delete m_colored_edge_fnc;
      m_colored_edge_fnc = nullptr;
    }

    if (m_edge_color_fnc) {
      delete m_edge_color_fnc;
      m_edge_color_fnc = nullptr;
    }

    if (m_colored_face_fnc) {
      delete m_colored_face_fnc;
      m_colored_face_fnc = nullptr;
    }

    if (m_face_wireframe_fnc) {
      delete m_face_wireframe_fnc;
      m_face_wireframe_fnc = nullptr;
    }

    if (m_vertex_color_fnc) {
      delete m_vertex_color_fnc;
      m_vertex_color_fnc = nullptr;
    }

    if (m_face_color_fnc) {
      delete m_face_color_fnc;
      m_face_color_fnc = nullptr;
    }
  }

  //!
  void apply_draw_vertex(const py::object& draw_vertex_object) {
    m_draw_vertex_object = draw_vertex_object;
    this->draw_vertex = *m_draw_vertex_fnc;
  }

  //!
  void apply_draw_edge(const py::object& draw_edge_object) {
    m_draw_edge_object = draw_edge_object;
    this->draw_edge = *m_draw_edge_fnc;
  }

  //!
  void apply_draw_face(const py::object& draw_face_object) {
    m_draw_face_object = draw_face_object;
    this->draw_face = *m_draw_face_fnc;
  }

  //!
  void apply_colored_vertex(const py::object& colored_vertex_object) {
    m_colored_vertex_object = colored_vertex_object;
    this->colored_vertex = *m_colored_vertex_fnc;
  }

  //!
  void apply_face_wireframe(const py::object& face_wireframe_object) {
    m_face_wireframe_object = face_wireframe_object;
    this->face_wireframe = *m_face_wireframe_fnc;
  }

  //!
  void apply_vertex_color(const py::object& vertex_color_object) {
    m_vertex_color_object = vertex_color_object;
    this->vertex_color = *m_vertex_color_fnc;
  }

  //!
  void apply_colored_edge(const py::object& colored_edge_object) {
    m_colored_edge_object = colored_edge_object;
    this->colored_edge = *m_colored_edge_fnc;
  }

  //!
  void apply_edge_color(const py::object& edge_color_object) {
    m_edge_color_object = edge_color_object;
    this->edge_color = *m_edge_color_fnc;
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
