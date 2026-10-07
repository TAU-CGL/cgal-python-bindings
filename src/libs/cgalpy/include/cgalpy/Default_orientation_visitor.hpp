// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP
#define CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP

#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;

namespace cgalpy {
namespace pmp {

//
struct Default_orientation_visitor : PMP::Default_orientation_visitor {

  Default_orientation_visitor() :
    nme_object(py::none()),
    nmv_object(py::none()),
    dv_object(py::none()),
    viipr_object(py::none()),
    por_object(py::none()),
    lcp_object(py::none())
  {}

  void non_manifold_edge(std::size_t id1, std::size_t id2,
                         std::size_t nb_poly) {
    if (! nme_object.is_none()) nme_object(id1, id2, nb_poly);
  }

  void non_manifold_vertex(std::size_t id, std::size_t nb_cycles) {
    if (! nmv_object.is_none()) nmv_object(id, nb_cycles);
  }

  void duplicated_vertex(std::size_t v1, std::size_t v2) {
    if (! dv_object.is_none()) dv_object(v1, v2);
  }

  void vertex_id_in_polygon_replaced(std::size_t p_id, std::size_t i1,
                                     std::size_t i2) {
    if (! viipr_object.is_none()) viipr_object(p_id, i1, i2);
  }

  void polygon_orientation_reversed(std::size_t p_id) {
    if (! por_object.is_none()) por_object(p_id);
  }

  void link_connected_polygons(std::size_t id,
                               const std::vector<std::size_t>& v) {
    if (! lcp_object.is_none()) lcp_object(id, v);
  }

  void set_non_manifold_edge(const py::object& fn) { nme_object = fn; }
  void set_non_manifold_vertex(const py::object& fn) { nmv_object = fn; }
  void set_duplicated_vertex(const py::object& fn) { dv_object = fn; }
  void set_vertex_id_in_polygon_replaced(const py::object& fn)
  { viipr_object = fn; }
  void set_polygon_orientation_reversed(const py::object& fn)
  { por_object = fn; }
  void set_link_connected_polygons(const py::object& fn) { lcp_object = fn; }

  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    Default_orientation_visitor* w = py::inst_ptr<Default_orientation_visitor>(self);

    py::handle value_nme = py::find(w->nme_object);
    py::handle value_nmv = py::find(w->nmv_object);
    py::handle value_dv = py::find(w->dv_object);
    py::handle value_viipr = py::find(w->viipr_object);
    py::handle value_por = py::find(w->por_object);
    py::handle value_lcp = py::find(w->lcp_object);

    Py_VISIT(value_nme.ptr());
    Py_VISIT(value_nmv.ptr());
    Py_VISIT(value_dv.ptr());
    Py_VISIT(value_viipr.ptr());
    Py_VISIT(value_por.ptr());
    Py_VISIT(value_lcp.ptr());

#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  static int tp_clear(PyObject* self) {
    Default_orientation_visitor* w = py::inst_ptr<Default_orientation_visitor>(self);

    w->nme_object = {};
    w->nmv_object = {};
    w->dv_object = {};
    w->viipr_object = {};
    w->por_object = {};
    w->lcp_object = {};

    return 0;
  }

private:
  py::object nme_object;
  py::object nmv_object;
  py::object dv_object;
  py::object viipr_object;
  py::object por_object;
  py::object lcp_object;
};

}
} // namespace cgalpy // namespace pmp

#endif // CGALPY_DEFAULT_ORIENTATION_VISITOR_HPP
