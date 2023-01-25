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
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

namespace aos2 {

//
Iterator_from_circulator<Ccb_halfedge_circulator>* outer_ccb(Face& f)
{ return new Iterator_from_circulator<Ccb_halfedge_circulator>(f.outer_ccb()); }

//
Iterator_of_circulators<Inner_ccb_iterator>* inner_ccbs(Face& f) {
  return new Iterator_of_circulators<Inner_ccb_iterator>(f.inner_ccbs_begin(),
                                                         f.inner_ccbs_end());
}

//
Iterator_of_circulators<Inner_ccb_iterator>* outer_ccbs(Face& f) {
  return new Iterator_of_circulators<Inner_ccb_iterator>(f.outer_ccbs_begin(),
                                                         f.outer_ccbs_end());
}

//
size_t number_of_inner_ccbs(const Face& f) { return f.number_of_inner_ccbs(); }
size_t number_of_outer_ccbs(const Face& f) { return f.number_of_outer_ccbs(); }

}

//
void export_face(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Face = Aos::Face;
  using Inner_ccb_iterator = Aos::Inner_ccb_iterator;

  // Face base
  py::class_<CGAL::Arr_face_base> face_base_c(c, "Arr_face_base");
  face_base_c.def("assign", &Face::assign)
    .def("is_unbounded", &CGAL::Arr_face_base::is_unbounded)
    .def("is_fictitious", &CGAL::Arr_face_base::is_fictitious)
    ;

  // Isolated vertices
  using Ivci = Aos::Isolated_vertex_const_iterator;
  using V = Aos::Vertex;
  add_iterator<Ivci, Ivci, const V&>("Isolated_vertex_iterator", face_base_c);
  face_base_c.def("isolated_vertices",
                  [](const Face& face) {
                    return make_iterator(face.isolated_vertices_begin(),
                                         face.isolated_vertices_end());
                  },
                  py::keep_alive<0, 1>());

  // Face
  py::class_<Face, CGAL::Arr_face_base> face_c(c, "Face");
  face_c.def(py::init<>())
    .def("number_of_inner_ccbs", &aos2::number_of_inner_ccbs)
    .def("inner_ccbs", &aos2::inner_ccbs)
    .def("number_of_outer_ccbs", &aos2::number_of_outer_ccbs)
    .def("outer_ccbs", &aos2::outer_ccbs)
    .def("number_of_isolated_vertices", &Face::number_of_isolated_vertices)
    .def("holes", &aos2::inner_ccbs)
    .def("outer_ccb", &aos2::outer_ccb)
    .def("has_outer_ccb", &Face::has_outer_ccb)
    .def("number_of_holes", &Face::number_of_holes)

#ifdef CGALPY_AOS2_FACE_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Face. Therefore, we cannot directly refere to any of them, e.g.,
    // `Face::set_data`. Instead, we introduce lambda functions that call the
    // appropriate member functions.
    .def("set_data", [](Face& f, py::object obj) { f.set_data(obj); })
    .def("data", [](const Face& f)->py::object { return f.data(); })
#endif
    ;

  bind_iterator<Iterator_from_circulator<Aos::Ccb_halfedge_circulator>>
    (face_c, "Ccb_halfedge_iterator");
  bind_iterator_of_circulators<Iterator_of_circulators<Inner_ccb_iterator>>
    (face_c, "Inner_ccbs_iterator");

  add_attr<V>(face_c, "Vertex");
}
