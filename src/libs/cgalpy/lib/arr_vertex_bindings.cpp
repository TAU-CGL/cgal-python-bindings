// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace bp = boost::python;

Iterator_from_circulator<Halfedge_around_vertex_circulator>*
halfedge_around_vertex_iterator(Vertex& v) {
  return new Iterator_from_circulator<Halfedge_around_vertex_circulator>(v.incident_halfedges());
}

void export_vertex() {
  bp::class_<Vertex>("Vertex")
    .def(bp::init<>())
    .def<TPoint_2& (Vertex::*)()>("point", &Vertex::point, bp::return_internal_reference<>())
    .def("is_isolated", &Vertex::is_isolated)
    .def("degree", &Vertex::degree)
    .def("incident_halfedges", &halfedge_around_vertex_iterator, bp::return_value_policy<bp::manage_new_object>())
#if CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL
    .def<Vertex::Data& (Vertex::*)()>("data", &Vertex::data, bp::return_value_policy<bp::copy_non_const_reference>())
    .def("set_data", &Vertex::set_data)
#endif
    ;
  bind_iterator<Iterator_from_circulator<Halfedge_around_vertex_circulator>>("Halfedge_around_vertex_iterator");
}
