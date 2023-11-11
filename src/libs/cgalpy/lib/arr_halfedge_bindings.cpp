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
#include "CGALPY/make_circulator.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

namespace aos2 {

//
Vertex& source(Halfedge& e) { return (*(e.source())); }
Vertex& target(Halfedge& e) { return (*(e.target())); }
Halfedge& next(Halfedge& e) { return (*(e.next())); }
Halfedge& prev(Halfedge& e) { return (*(e.prev())); }
Halfedge& twin(Halfedge& e) { return (*(e.twin())); }
Face& face(Halfedge& e) { return (*(e.face())); }
X_monotone_curve_2& curve(Halfedge& e) { return (e.curve()); }

py::object ccb(const Halfedge& h)
{ return make_circulator(h.ccb()); }

#ifdef CGALPY_ENVELOPE_3_BINDINGS
py::object surfaces(const Halfedge& h)
{ return make_iterator(h.surfaces_begin(), h.surfaces_end()); }
#endif

}

//
void export_halfedge(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using He = Aos::Halfedge;
  constexpr auto ri(py::rv_policy::reference_internal);

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  using Env_data = He::Halfedge_data;
  using Dd = CGAL::Dac_decision;
#endif

  py::class_<He> halfedge_c(c, "Halfedge");
  halfedge_c.def(py::init<>())
    .def("direction", [](const He& h)->bool{ return h.direction(); })
    .def("is_fictitious", &He::is_fictitious)
    .def("source", &aos2::source, ri)
    .def("target", &aos2::target, ri)
    .def("twin", &aos2::twin, ri)
    .def("face", &aos2::face, ri)
    .def("next", &aos2::next, ri)
    .def("prev", &aos2::prev, ri)
    .def("curve", &aos2::curve, ri)
    .def("ccb", &aos2::ccb, py::keep_alive<0, 1>())

#ifdef CGALPY_AOS2_HALFEDGE_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Face. Therefore, we cannot directly refere to any of them, e.g.,
    // `Face::set_data`. Instead, we introduce lambda functions that call the
    // appropriate member functions.
    .def("set_data", [](He& h, py::object obj) { h.set_data(obj); })
    .def("data", [](const He& h)->py::object { return h.data(); })
#endif

#ifdef CGALPY_ENVELOPE_3_BINDINGS
    .def("number_of_surfaces", [](He& h) { return h.number_of_surfaces(); })
    .def("surfaces", &aos2::surfaces, py::keep_alive<0, 1>())
#endif
    ;

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  using Si = He::Data_const_iterator;
  add_iterator<Si, Si>("Surface_iterator", halfedge_c);
#endif

  using Chcc = Aos::Ccb_halfedge_const_circulator;
  add_circulator<Chcc>("Halfedge_around_vertex_circulator", halfedge_c);
}
