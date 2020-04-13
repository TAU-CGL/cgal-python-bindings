// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/common.hpp"
#include "CGALPY/python_iterator_templates.hpp"

Vertex& source(Halfedge& e) { return (*(e.source())); }
Vertex& target(Halfedge& e) { return (*(e.target())); }
Halfedge& next(Halfedge& e) { return (*(e.next())); }
Halfedge& prev(Halfedge& e) { return (*(e.prev())); }
Halfedge& twin(Halfedge& e) { return (*(e.twin())); }
Face& face(Halfedge& e) { return (*(e.face())); }
X_monotone_curve_2& curve(Halfedge& e) { return (e.curve()); }

Iterator_from_circulator<Ccb_halfedge_circulator>* ccb(Halfedge& e)
{
  return new Iterator_from_circulator<Ccb_halfedge_circulator>(e.ccb());
}

void export_halfedge()
{
  using namespace boost::python;
  class_<Halfedge>("Halfedge")
    .def(init<>())
    .def("direction", &Halfedge::direction)
    .def("source", &source, return_value_policy<reference_existing_object>())
    .def("target", &target, return_value_policy<reference_existing_object>())
    .def("twin", &twin, return_value_policy<reference_existing_object>())
    .def("face", &face, return_value_policy<reference_existing_object>())
    .def("next", &next, return_value_policy<reference_existing_object>())
    .def("prev", &prev, return_value_policy<reference_existing_object>())
    .def("curve", &curve, return_value_policy<reference_existing_object>())
    .def("ccb", &ccb, return_value_policy<manage_new_object>())
#if CGALPY_DCEL == CGALPY_EXTENDED_DCEL
    .def("set_data", &Halfedge::set_data)
    .def<Halfedge::Data& (Halfedge::*)()>("data", &Halfedge::data, return_value_policy<copy_non_const_reference>())
#endif
    ;
}
