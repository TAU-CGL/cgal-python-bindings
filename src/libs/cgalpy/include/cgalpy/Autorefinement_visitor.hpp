// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_AUTOREFINEMENT_VISITOR_HPP
#define CGALPY_AUTOREFINEMENT_VISITOR_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/autorefinement.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;

namespace cgalpy {
namespace pmp {

//
struct Autorefinement_visitor : PMP::Autorefinement::Default_visitor {
  Autorefinement_visitor() :
    noot_object(py::none()),
    vtc_object(py::none()),
    ns_object(py::none())
  {}

  inline void number_of_output_triangles(std::size_t nbt) {
    if (! noot_object.is_none()) noot_object(nbt);
  }

  inline void verbatim_triangle_copy(std::size_t tgt_id, std::size_t src_id) {
    if (! vtc_object.is_none()) vtc_object(tgt_id, src_id);
  }

  inline void new_subtriangle(std::size_t tgt_id, std::size_t src_id) {
    if (! ns_object.is_none()) ns_object(tgt_id, src_id);
  }

  void set_number_of_output_triangles(const py::object& fn) { noot_object = fn; }
  void set_verbatim_triangle_copy(const py::object& fn) { vtc_object = fn; }
  void set_new_subtriangle(const py::object& fn) { ns_object = fn; }

  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    Autorefinement_visitor* w = py::inst_ptr<Autorefinement_visitor>(self);

    py::handle value_noot = py::find(w->noot_object);
    py::handle value_vtc = py::find(w->vtc_object);
    py::handle value_ns = py::find(w->ns_object);

    Py_VISIT(value_noot.ptr());
    Py_VISIT(value_vtc.ptr());
    Py_VISIT(value_ns.ptr());

#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  static int tp_clear(PyObject* self) {
    Autorefinement_visitor* w = py::inst_ptr<Autorefinement_visitor>(self);

    w->noot_object = {};
    w->vtc_object = {};
    w->ns_object = {};

    return 0;
  }

private:
  py::object noot_object;
  py::object vtc_object;
  py::object ns_object;
};

}
} // namespace cgalpy // namespace pmp

#endif // CGALPY_AUTOREFINEMENT_VISITOR_HPP
