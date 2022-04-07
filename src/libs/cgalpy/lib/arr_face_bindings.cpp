// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace py = nanobind;

namespace aos2 {

Iterator_from_circulator<Ccb_halfedge_circulator>* outer_ccb(Face& f)
{ return new Iterator_from_circulator<Ccb_halfedge_circulator>(f.outer_ccb()); }

Iterator_of_circulators<Inner_ccb_iterator>* inner_ccbs(Face& f)
{
  return new Iterator_of_circulators<Inner_ccb_iterator>(f.inner_ccbs_begin(), f.inner_ccbs_end());
}

Isolated_vertex_iterator isolated_vertices_begin(Face& f)
{ return f.isolated_vertices_begin(); }

Isolated_vertex_iterator isolated_vertices_end(Face& f)
{ return f.isolated_vertices_end(); }

}

void export_face() {
  typedef aos2::Arrangement_2   Arr2;
  typedef Arr2::Face            Face;

  py::class_<Face>("Face")
    .def(py::init<>())
    .def("assign", &Face::assign)
    .def("is_unbounded", &Face::is_unbounded)
    .def("is_fictitious", &Face::is_fictitious)
    .def("has_outer_ccb", &Face::has_outer_ccb)
    .def("number_of_inner_ccbs", &Face::number_of_inner_ccbs)
    .def("number_of_outer_ccbs", &Face::number_of_outer_ccbs)
    .def("splice_isolated_vertices", &Face::splice_isolated_vertices)
    .def("splice_inner_ccbs", &Face::splice_inner_ccbs)
    .def("outer_ccb", &aos2::outer_ccb, Manage_new_object())
    .def("inner_ccbs", &aos2::inner_ccbs, Manage_new_object())
    .def("holes", &aos2::inner_ccbs, Manage_new_object())
    .def("number_of_isolated_vertices", &Face::number_of_isolated_vertices)
    .def("isolated_vertices", py::range<py::return_internal_reference<>>(&aos2::isolated_vertices_begin, &aos2::isolated_vertices_end))
#ifdef CGALPY_AOS2_FACE_EXTENDED
    .def("set_data", &Face::set_data)
    .def<Face::Data& (Face::*)()>("data", &Face::data, Copy_non_const_reference())
#endif
    ;

  bind_iterator<Iterator_from_circulator<Arr2::Ccb_halfedge_circulator>>("Ccb_halfedge_iterator");
  bind_iterator_of_circulators<Iterator_of_circulators<Arr2::Inner_ccb_iterator>>("Inner_ccbs_iterator");
}
