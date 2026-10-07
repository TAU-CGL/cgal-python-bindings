// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_TRIANGULATE_FACES_VISITOR_HPP
#define CGALPY_TRIANGULATE_FACES_VISITOR_HPP

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;

namespace cgalpy {
namespace pmp {

//! Visitor wrapper for CGAL::Polygon_mesh_processing::triangulate_faces().
template <typename PolygonMesh>
struct Triangulate_faces_visitor :
  public PMP::Triangulate_faces::Default_visitor<PolygonMesh>
{
  using Base = PMP::Triangulate_faces::Default_visitor<PolygonMesh>;
  using Gt = boost::graph_traits<PolygonMesh>;
  using Fd = typename Gt::face_descriptor;
  using Vd = typename Gt::vertex_descriptor;

  Triangulate_faces_visitor() :
    before_subface_creations_object(py::none()),
    after_subface_creations_object(py::none()),
    after_subface_created_object(py::none()),
    accept_face_object(py::none())
  {}

  void before_subface_creations(Fd f)
  {
    if (! before_subface_creations_object.is_none())
      before_subface_creations_object(f);
  }

  void after_subface_creations()
  {
    if (! after_subface_creations_object.is_none())
      after_subface_creations_object();
  }

  void after_subface_created(Fd f)
  {
    if (! after_subface_created_object.is_none())
      after_subface_created_object(f);
  }

  bool accept_face(Fd f, Vd v0, Vd v1, Vd v2) const
  {
    if (! accept_face_object.is_none())
      return py::cast<bool>(accept_face_object(f, v0, v1, v2));
    return Base::accept_face(f, v0, v1, v2);
  }

  void set_before_subface_creations(const py::object& fn)
  { before_subface_creations_object = fn; }

  void set_after_subface_creations(const py::object& fn)
  { after_subface_creations_object = fn; }

  void set_after_subface_created(const py::object& fn)
  { after_subface_created_object = fn; }

  void set_accept_face(const py::object& fn)
  { accept_face_object = fn; }

  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    Triangulate_faces_visitor* w =
      py::inst_ptr<Triangulate_faces_visitor>(self);

    py::handle value_before = py::find(w->before_subface_creations_object);
    py::handle value_after_all = py::find(w->after_subface_creations_object);
    py::handle value_after_each = py::find(w->after_subface_created_object);
    py::handle value_accept = py::find(w->accept_face_object);

    Py_VISIT(value_before.ptr());
    Py_VISIT(value_after_all.ptr());
    Py_VISIT(value_after_each.ptr());
    Py_VISIT(value_accept.ptr());

#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  static int tp_clear(PyObject* self) {
    Triangulate_faces_visitor* w =
      py::inst_ptr<Triangulate_faces_visitor>(self);

    w->before_subface_creations_object = {};
    w->after_subface_creations_object = {};
    w->after_subface_created_object = {};
    w->accept_face_object = {};

    return 0;
  }

private:
  py::object before_subface_creations_object;
  py::object after_subface_creations_object;
  py::object after_subface_created_object;
  py::object accept_face_object;
};

}
} // namespace cgalpy // namespace pmp

#endif // CGALPY_TRIANGULATE_FACES_VISITOR_HPP
