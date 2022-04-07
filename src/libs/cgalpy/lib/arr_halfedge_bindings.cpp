// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include <nanobind/nanobind.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace py = nanobind;

namespace aos2 {

Vertex& source(Halfedge& e) { return (*(e.source())); }
Vertex& target(Halfedge& e) { return (*(e.target())); }
Halfedge& next(Halfedge& e) { return (*(e.next())); }
Halfedge& prev(Halfedge& e) { return (*(e.prev())); }
Halfedge& twin(Halfedge& e) { return (*(e.twin())); }
Face& face(Halfedge& e) { return (*(e.face())); }
X_monotone_curve_2& curve(Halfedge& e) { return (e.curve()); }

Iterator_from_circulator<Ccb_halfedge_circulator>* ccb(Halfedge& e)
{ return new Iterator_from_circulator<Ccb_halfedge_circulator>(e.ccb()); }

}

void export_halfedge() {
  typedef aos2::Arrangement_2   Arr2;
  typedef Arr2::Halfedge        Halfedge;

  py::class_<Halfedge>("Halfedge")
    .def(py::init<>())
    .def("direction", &Halfedge::direction)
    .def("source", &aos2::source, Reference_existing_object())
    .def("target", &aos2::target, Reference_existing_object())
    .def("twin", &aos2::twin, Reference_existing_object())
    .def("face", &aos2::face, Reference_existing_object())
    .def("next", &aos2::next, Reference_existing_object())
    .def("prev", &aos2::prev, Reference_existing_object())
    .def("curve", &aos2::curve, Reference_existing_object())
    .def("ccb", &aos2::ccb, Manage_new_object())
#ifdef CGALPY_AOS2_HALFEDGE_EXTENDED
    .def("set_data", &Halfedge::set_data)
    .def<Halfedge::Data& (Halfedge::*)()>("data", &Halfedge::data, Copy_non_const_reference())
#endif
    ;
}
