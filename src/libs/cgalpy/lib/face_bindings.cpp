// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/common.hpp"
#include "CGALPY/python_iterator_templates.hpp"

Iterator_from_circulator<Ccb_halfedge_circulator>* outer_ccb(Face& f)
{
  return new Iterator_from_circulator<Ccb_halfedge_circulator>(f.outer_ccb());
}
Iterator_of_circulators<Inner_ccb_iterator>* inner_ccbs(Face& f)
{
  return new Iterator_of_circulators<Inner_ccb_iterator>(f.inner_ccbs_begin(), f.inner_ccbs_end());
}

Isolated_vertex_iterator isolated_vertices_begin(Face& f)
{
  return f.isolated_vertices_begin();
}
Isolated_vertex_iterator isolated_vertices_end(Face& f)
{
  return f.isolated_vertices_end();
}
void export_face()
{
  using namespace boost::python;
  class_<Face>("Face")
    .def(init<>())
    .def("assign", &Face::assign)
    .def("is_unbounded", &Face::is_unbounded)
    .def("is_fititious", &Face::is_fictitious)
    .def("has_outer_ccb", &Face::has_outer_ccb)
    .def("number_of_inner_ccbs", &Face::number_of_inner_ccbs)
    .def("number_of_outer_ccbs", &Face::number_of_outer_ccbs)
    .def("splice_isolated_vertices", &Face::splice_isolated_vertices)
    .def("splice_inner_ccbs", &Face::splice_inner_ccbs)
    .def("outer_ccb", &outer_ccb, return_value_policy<manage_new_object>())
    .def("inner_ccbs", &inner_ccbs, return_value_policy<manage_new_object>())
    .def("number_of_isolated_vertices", &Face::number_of_isolated_vertices)
    .def("isolated_vertices", range<return_internal_reference<>>(&isolated_vertices_begin, &isolated_vertices_end))
#if CGALPY_DCEL == CGALPY_EXTENDED_DCEL || CGALPY_DCEL == CGALPY_FACE_EXTENDED_DCEL
    .def("set_data", &Face::set_data)
    .def<Face::Data& (Face::*)()>("data", &Face::data, return_value_policy<copy_non_const_reference>())
#endif
    ;
  bind_iterator<Iterator_from_circulator<Ccb_halfedge_circulator>>("Ccb_halfedge_iterator");
  bind_iterator_of_circulators<Iterator_of_circulators<Inner_ccb_iterator>>("Inner_ccbs_iterator");
}
