// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_HFDEFAULT_VISITOR_HPP
#define CGALPY_HFDEFAULT_VISITOR_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace HF = PMP::Hole_filling;

namespace cgalpy {
namespace pmp {

struct HFDefault_visitor : HF::Default_visitor {

  HFDefault_visitor() :
    spp_object(py::none()),
    epp_object(py::none()),
    sqp_object(py::none()),
    qs_object(py::none()),
    ecp_object(py::none()),
    scp_object(py::none()),
    cs_object(py::none()),
    ecup_object(py::none()),
    srp_object(py::none()),
    erp_object(py::none()),
    sfp_object(py::none()),
    efp_object(py::none())
  {}

  void start_planar_phase() const
  { if (! spp_object.is_none()) spp_object(); }

  void end_planar_phase(bool success) const
  { if (! epp_object.is_none()) epp_object(success); }

  void start_quadratic_phase(std::size_t n) const
  { if (! sqp_object.is_none()) sqp_object(n); }

  void quadratic_step() const
  { if (! qs_object.is_none()) qs_object(); }

  void end_quadratic_phase(bool success) const
  { if (! ecp_object.is_none()) ecp_object(success); }

  void start_cubic_phase(int n) const
  { if (! scp_object.is_none()) scp_object(n); }

  void cubic_step() const
  { if (! cs_object.is_none()) cs_object(); }

  void end_cubic_phase() const
  { if (! ecup_object.is_none()) ecup_object(); }

  void start_refine_phase() const
  { if (! srp_object.is_none()) srp_object(); }

  void end_refine_phase() const
  { if (! erp_object.is_none()) erp_object(); }

  void start_fair_phase() const
  { if (! sfp_object.is_none()) sfp_object(); }

  void end_fair_phase() const
  { if (! efp_object.is_none()) efp_object(); }

  void set_start_planar_phase(const py::object& fn) { spp_object = fn; }
  void set_end_planar_phase(const py::object& fn) { epp_object = fn; }
  void set_start_quadratic_phase(const py::object& fn) { sqp_object = fn; }
  void set_quadratic_step(const py::object& fn) { qs_object = fn; }
  void set_end_quadratic_phase(const py::object& fn) { ecp_object = fn; }
  void set_start_cubic_phase(const py::object& fn) { scp_object = fn; }
  void set_cubic_step(const py::object& fn) { cs_object = fn; }
  void set_end_cubic_phase(const py::object& fn) { ecup_object = fn; }
  void set_start_refine_phase(const py::object& fn) { srp_object = fn; }
  void set_end_refine_phase(const py::object& fn) { erp_object = fn; }
  void set_start_fair_phase(const py::object& fn) { sfp_object = fn; }
  void set_end_fair_phase(const py::object& fn) { efp_object = fn; }

  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    HFDefault_visitor* w = py::inst_ptr<HFDefault_visitor>(self);

    py::handle value_spp = py::find(w->spp_object);
    py::handle value_epp = py::find(w->epp_object);
    py::handle value_sqp = py::find(w->sqp_object);
    py::handle value_qs = py::find(w->qs_object);
    py::handle value_ecp = py::find(w->ecp_object);
    py::handle value_scp = py::find(w->scp_object);
    py::handle value_cs = py::find(w->cs_object);
    py::handle value_ecup = py::find(w->ecup_object);
    py::handle value_srp = py::find(w->srp_object);
    py::handle value_erp = py::find(w->erp_object);
    py::handle value_sfp = py::find(w->sfp_object);
    py::handle value_efp = py::find(w->efp_object);

    Py_VISIT(value_spp.ptr());
    Py_VISIT(value_epp.ptr());
    Py_VISIT(value_sqp.ptr());
    Py_VISIT(value_qs.ptr());
    Py_VISIT(value_ecp.ptr());
    Py_VISIT(value_scp.ptr());
    Py_VISIT(value_cs.ptr());
    Py_VISIT(value_ecup.ptr());
    Py_VISIT(value_srp.ptr());
    Py_VISIT(value_erp.ptr());
    Py_VISIT(value_sfp.ptr());
    Py_VISIT(value_efp.ptr());

#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  static int tp_clear(PyObject* self) {
    HFDefault_visitor* w = py::inst_ptr<HFDefault_visitor>(self);

    w->spp_object = {};
    w->epp_object = {};
    w->sqp_object = {};
    w->qs_object = {};
    w->ecp_object = {};
    w->scp_object = {};
    w->cs_object = {};
    w->ecup_object = {};
    w->srp_object = {};
    w->erp_object = {};
    w->sfp_object = {};
    w->efp_object = {};

    return 0;
  }

private:
  py::object spp_object;
  py::object epp_object;
  py::object sqp_object;
  py::object qs_object;
  py::object ecp_object;
  py::object scp_object;
  py::object cs_object;
  py::object ecup_object;
  py::object srp_object;
  py::object erp_object;
  py::object sfp_object;
  py::object efp_object;
};

}
} // namespace cgalpy // namespace pmp

#endif // CGALPY_HFDEFAULT_VISITOR_HPP

