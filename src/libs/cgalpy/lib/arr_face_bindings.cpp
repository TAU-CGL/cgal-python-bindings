// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Envelope_3/Envelope_base.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/make_circulator.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

namespace aos2 {

// Bind iterators & circulators
py::object outer_ccb(const Face& f) { return make_circulator(f.outer_ccb()); }

py::object outer_ccbs(const Face& f)
{ return make_iterator(f.outer_ccbs_begin(), f.outer_ccbs_end()); }

py::object inner_ccbs(const Face& f) {
  Arrangement_on_surface_2::Inner_ccb_const_iterator begin = f.inner_ccbs_begin();
  Arrangement_on_surface_2::Inner_ccb_const_iterator end = f.inner_ccbs_end();
  return make_iterator(begin, end);
}

//
size_t number_of_inner_ccbs(const Face& f) { return f.number_of_inner_ccbs(); }
size_t number_of_outer_ccbs(const Face& f) { return f.number_of_outer_ccbs(); }

#ifdef CGALPY_ENVELOPE_3_BINDINGS
py::object surfaces(const Face& f)
{ return make_iterator(f.surfaces_begin(), f.surfaces_end()); }
#endif

}

//
void export_face(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Face = Aos::Face;
  using V = Aos::Vertex;

  // Face base
  if (! add_attr<CGAL::Arr_face_base>(c, "Arr_face_base")) {
    py::class_<CGAL::Arr_face_base> face_base_c(c, "Arr_face_base");
    face_base_c.def("assign", &CGAL::Arr_face_base::assign)
      .def("is_unbounded", &CGAL::Arr_face_base::is_unbounded)
      .def("is_fictitious", &CGAL::Arr_face_base::is_fictitious)
      ;

    // Isolated vertices
    using Ivci = Aos::Isolated_vertex_const_iterator;
    add_iterator<Ivci, Ivci, const V&>("Isolated_vertex_iterator", face_base_c);
    face_base_c.def("isolated_vertices",
                    [](const Face& face) {
                      return make_iterator(face.isolated_vertices_begin(),
                                           face.isolated_vertices_end());
                    },
                    py::keep_alive<0, 1>());
  }

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  using Env_data = Face::Face_data;
  using Dd = CGAL::Dac_decision;
#endif

  // Face
  if (add_attr<Face>(c, "Face")) return;
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

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  // Nanobind does not support multiple inheritance; therfore, we bind
  // Envelope_pm_face members, using explicit lamda functions
    .def("is_env_set", [](Face& f)->bool { return f.is_env_set(); })
    .def("set_is_env_set", [](Face& f, bool b) { f.set_is_env_set(b); })
    .def("is_decision_set", [](Face& f)->bool { return f.is_decision_set(); })
    .def("decision", [](Face& f)->Dd { return f.decision(); })
    .def("set_decision", [](Face& f, CGAL::Comparison_result cr) { f.set_decision(cr); })
    .def("set_decision", [](Face& f, Dd dd) { f.set_decision(dd); })
    .def("number_of_surfaces", [](Face& f) { return f.number_of_surfaces(); })
    .def("surfaces", &aos2::surfaces, py::keep_alive<0, 1>())
    .def("surface", [](Face& f)->const Env_data& { return f.surface(); })
    .def("number_of_surfaces",
         [](Face& f)->int { return f.number_of_surfaces(); })
    .def("has_no_env_data", [](Face& f)->bool { return f.has_no_env_data(); })
    .def("env_data_front",
         [](Face& f)->const Env_data&  { return f.env_data_front(); })
    .def("set_env_data",
         [](Face& f, const Env_data& data) { f.set_env_data(data); })
    //.def("set_env_data", [](Face& f) { f.set_env_data(); })
    .def("set_no_env_data", [](Face& f) { f.set_no_env_data(); })
    .def("add_env_data",
         [](Face& f, const Env_data& data) { f.add_env_data(data); })
    //.def("add_data", [](Face& f) { f.add_data(); })
    .def("clear_env_data", [](Face& f) { f.clear_env_data(); })
    //.def("is_equal_data", [](Face& f) { f.is_equal_data(); })
    //.def("has_equal_data", [](Face& f) { f.has_equal_data(); })
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

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  using Si = Face::Data_const_iterator;
  add_iterator<Si, Si>("Surface_iterator", face_c);
#endif

  add_attr<V>(face_c, "Vertex");
}
