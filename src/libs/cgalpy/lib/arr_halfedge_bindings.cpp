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
Vertex& source(Halfedge& e) { return (*(e.source())); }
Vertex& target(Halfedge& e) { return (*(e.target())); }
Halfedge& next(Halfedge& e) { return (*(e.next())); }
Halfedge& prev(Halfedge& e) { return (*(e.prev())); }
Halfedge& twin(Halfedge& e) { return (*(e.twin())); }
Face& face(Halfedge& e) { return (*(e.face())); }
X_monotone_curve_2& curve(Halfedge& e) { return (e.curve()); }

//
Iterator_from_circulator<Ccb_halfedge_circulator>* ccb(Halfedge& e)
{ return new Iterator_from_circulator<Ccb_halfedge_circulator>(e.ccb()); }

}

//
void export_halfedge(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Halfedge = Aos::Halfedge;
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Halfedge>(c, "Halfedge")
    .def(py::init<>())
    .def("direction", &Halfedge::direction)

    .def("is_fictitious", &Halfedge::is_fictitious)
    .def("source", &aos2::source, ri)
    .def("target", &aos2::target, ri)
    .def("twin", &aos2::twin, ri)
    .def("face", &aos2::face, ri)
    .def("next", &aos2::next, ri)
    .def("prev", &aos2::prev, ri)
    .def("curve", &aos2::curve, ri)
    .def("ccb", &aos2::ccb, ri)
#ifdef CGALPY_AOS2_HALFEDGE_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Face. Therefore, we cannot directly refere to any of them, e.g.,
    // `Face::set_data`. Instead, we introduce lambda functions that call the
    // appropriate member functions.
    .def("set_data", [](Halfedge& h, py::object obj) { h.set_data(obj); })
    .def("data", [](const Halfedge& h)->py::object { return h.data(); })
#endif
    ;
}
