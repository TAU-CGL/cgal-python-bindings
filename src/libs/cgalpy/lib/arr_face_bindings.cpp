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

//
Iterator_from_circulator<Ccb_halfedge_circulator>* outer_ccb(Face& f)
{ return new Iterator_from_circulator<Ccb_halfedge_circulator>(f.outer_ccb()); }

//
Iterator_of_circulators<Inner_ccb_iterator>* inner_ccbs(Face& f)
{
  return new Iterator_of_circulators<Inner_ccb_iterator>(f.inner_ccbs_begin(),
                                                         f.inner_ccbs_end());
}

//
Isolated_vertex_iterator isolated_vertices_begin(Face& f)
{ return f.isolated_vertices_begin(); }

//
Isolated_vertex_iterator isolated_vertices_end(Face& f)
{ return f.isolated_vertices_end(); }

}

//
void export_face(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Arr2 = aos2::Arrangement_2;
  using Face = Arr2::Face;
  using Inner_ccb_iterator = Arr2::Inner_ccb_iterator;

  py::class_<Face>(c, "Face")
    .def(py::init<>())
    .def("assign", &Face::assign)
    .def("is_unbounded", &Face::is_unbounded)
    .def("is_fictitious", &Face::is_fictitious)
    .def("has_outer_ccb", &Face::has_outer_ccb)
    .def("number_of_inner_ccbs", &Face::number_of_inner_ccbs)
    .def("number_of_outer_ccbs", &Face::number_of_outer_ccbs)
    .def("splice_isolated_vertices", &Face::splice_isolated_vertices)
    .def("splice_inner_ccbs", &Face::splice_inner_ccbs)
    .def("outer_ccb", &aos2::outer_ccb)
    .def("inner_ccbs", &aos2::inner_ccbs)
    .def("holes", &aos2::inner_ccbs)
    .def("number_of_isolated_vertices", &Face::number_of_isolated_vertices)
    // .def("isolated_vertices", py::range<py::return_internal_reference<>>(&aos2::isolated_vertices_begin, &aos2::isolated_vertices_end)) NB
#ifdef CGALPY_AOS2_FACE_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Face. Therefore, we cannot directly refere to any of them, e.g.,
    // `Face::set_data`. Instead, we introduce lambda functions that call the
    // appropriate member functions.
    .def("set_data", [](Face* face, py::object obj) { face->set_data(obj); })
    .def("data", [](const Face* face)->py::object { return face->data(); })
#endif
    ;

  bind_iterator<Iterator_from_circulator<Arr2::Ccb_halfedge_circulator>>
    (c, "Ccb_halfedge_iterator");
  bind_iterator_of_circulators<Iterator_of_circulators<Inner_ccb_iterator>>
    (c, "Inner_ccbs_iterator");
}
