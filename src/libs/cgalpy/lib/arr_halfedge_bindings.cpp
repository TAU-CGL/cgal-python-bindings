// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace bp = boost::python;

Vertex& source(Halfedge& e) { return (*(e.source())); }
Vertex& target(Halfedge& e) { return (*(e.target())); }
Halfedge& next(Halfedge& e) { return (*(e.next())); }
Halfedge& prev(Halfedge& e) { return (*(e.prev())); }
Halfedge& twin(Halfedge& e) { return (*(e.twin())); }
Face& face(Halfedge& e) { return (*(e.face())); }
X_monotone_curve_2& curve(Halfedge& e) { return (e.curve()); }

Iterator_from_circulator<Ccb_halfedge_circulator>* ccb(Halfedge& e)
{ return new Iterator_from_circulator<Ccb_halfedge_circulator>(e.ccb()); }

void export_halfedge() {
  bp::class_<Halfedge>("Halfedge")
    .def(bp::init<>())
    .def("direction", &Halfedge::direction)
    .def("source", &source, bp::return_value_policy<bp::reference_existing_object>())
    .def("target", &target, bp::return_value_policy<bp::reference_existing_object>())
    .def("twin", &twin, bp::return_value_policy<bp::reference_existing_object>())
    .def("face", &face, bp::return_value_policy<bp::reference_existing_object>())
    .def("next", &next, bp::return_value_policy<bp::reference_existing_object>())
    .def("prev", &prev, bp::return_value_policy<bp::reference_existing_object>())
    .def("curve", &curve, bp::return_value_policy<bp::reference_existing_object>())
    .def("ccb", &ccb, bp::return_value_policy<bp::manage_new_object>())
#if CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL
    .def("set_data", &Halfedge::set_data)
    .def<Halfedge::Data& (Halfedge::*)()>("data", &Halfedge::data, bp::return_value_policy<bp::copy_non_const_reference>())
#endif
    ;
}
