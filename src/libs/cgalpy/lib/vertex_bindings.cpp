// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/common.hpp"
#include "CGALPY/python_iterator_templates.hpp"

Iterator_from_circulator<Halfedge_around_vertex_circulator>* halfedge_around_vertex_iterator(Vertex& v)
{
  return new Iterator_from_circulator<Halfedge_around_vertex_circulator>(v.incident_halfedges());
}

void export_vertex()
{
  using namespace boost::python;
  class_<Vertex>("Vertex")
    .def(init<>())
    .def<TPoint_2& (Vertex::*)()>("point", &Vertex::point, return_internal_reference<>())
    .def("is_isolated", &Vertex::is_isolated)
    .def("degree", &Vertex::degree)
    .def("incident_halfedges", &halfedge_around_vertex_iterator, return_value_policy<manage_new_object>())
#if CGALPY_DCEL == CGALPY_EXTENDED_DCEL
    .def<Vertex::Data& (Vertex::*)()>("data", &Vertex::data, return_value_policy<copy_non_const_reference>())
    .def("set_data", &Vertex::set_data)
#endif
    ;
  bind_iterator<Iterator_from_circulator<Halfedge_around_vertex_circulator>>("Halfedge_around_vertex_iterator");
}
