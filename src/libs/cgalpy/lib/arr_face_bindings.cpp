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
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/make_circulator.hpp"

namespace py = nanobind;

namespace aos2 {

// Bind iterators & circulators
py::object outer_ccb(const Face& f) { return make_circulator(f.outer_ccb()); }

py::object outer_ccbs(const Face& f)
{ return make_iterator(f.outer_ccbs_begin(), f.outer_ccbs_end()); }

py::object inner_ccbs(const Face& f)
{ return make_iterator(f.inner_ccbs_begin(), f.inner_ccbs_end()); }

//
size_t number_of_inner_ccbs(const Face& f) { return f.number_of_inner_ccbs(); }
size_t number_of_outer_ccbs(const Face& f) { return f.number_of_outer_ccbs(); }

}

//
void export_face(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Face = Aos::Face;

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
    .def("number_of_outer_ccbs", &aos2::number_of_outer_ccbs)
    .def("number_of_isolated_vertices", &Face::number_of_isolated_vertices)
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

  using Chcc = Aos::Ccb_halfedge_const_circulator;
  using Icci = Aos::Inner_ccb_const_iterator;
  using Occi = Aos::Outer_ccb_const_iterator;
  add_circulator<Chcc>("Ccb_halfedge_circulator", face_c);
  add_iterator_of_circulator<Icci, Icci, Chcc>("Inner_ccb_iterator", face_c);
  add_iterator_of_circulator<Occi, Occi, Chcc>("Outer_ccb_iterator", face_c);

  face_c.def("outer_ccb", &aos2::outer_ccb, py::keep_alive<0, 1>());
  face_c.def("outer_ccbs", &aos2::outer_ccbs, py::keep_alive<0, 1>());
  face_c.def("inner_ccbs", &aos2::inner_ccbs, py::keep_alive<0, 1>());
  face_c.def("holes", &aos2::inner_ccbs, py::keep_alive<0, 1>());

  add_attr<V>(face_c, "Vertex");
}
