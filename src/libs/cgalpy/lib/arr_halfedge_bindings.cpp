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

Iterator_from_circulator<Ccb_halfedge_circulator>* ccb(Halfedge& e)
{ return new Iterator_from_circulator<Ccb_halfedge_circulator>(e.ccb()); }

}

//
void export_halfedge(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Halfedge = Aos::Halfedge;
  constexpr auto ka = py::keep_alive<0, 1>();
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Halfedge>(c, "Halfedge")
    .def(py::init<>())
    .def("direction", &Halfedge::direction)
    .def("source", &aos2::source, ri, ka)
    .def("target", &aos2::target, ri, ka)
    .def("twin", &aos2::twin)
    .def("face", &aos2::face)
    .def("next", &aos2::next)
    .def("prev", &aos2::prev)
    .def("curve", &aos2::curve)
    .def("ccb", &aos2::ccb)
#ifdef CGALPY_AOS2_HALFEDGE_EXTENDED
    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("data_mutual", py::overload_cast<>(&Halfedge::data),
         py::rv_policy::reference_internal)
    .def("data", py::overload_cast<>(&Halfedge::data, py::const_))

    .def("set_data", &Halfedge::set_data)
#endif
    ;
}
