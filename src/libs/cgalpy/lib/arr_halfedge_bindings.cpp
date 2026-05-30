// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/stl/unique_ptr.h>

#include <CGAL/Envelope_3/Envelope_base.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/arrangement_on_surface_2_types.hpp"
#include "cgalpy/Aos2_docstrings.hpp"
#include "cgalpy/export_circulator.hpp"
#include "cgalpy/make_iterator.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

namespace cgalpy {
namespace aos2 {

template <typename T>
bool has_handle() {
  using Rep = typename T::Rep;
  return false;
}

//!
Vertex& source(Halfedge& e) { return (*(e.source())); }
Vertex& target(Halfedge& e) { return (*(e.target())); }
Halfedge& next(Halfedge& e) { return (*(e.next())); }
Halfedge& prev(Halfedge& e) { return (*(e.prev())); }
Halfedge& twin(Halfedge& e) { return (*(e.twin())); }
Face& face(Halfedge& e) { return (*(e.face())); }

//! Wrap the function that obtains the real circulator
auto ccb_circulator(const Halfedge& h) { return h.ccb(); }

//! Wrap the iterator
py::object ccb_iterator(const Halfedge& h)
{ return make_iterator_from_circulator(h.ccb()); }

//!
#ifdef CGALPY_ENVELOPE_3_BINDINGS
py::object surfaces(const Halfedge& h)
{ return make_iterator(h.surfaces_begin(), h.surfaces_end()); }
#endif

}
} // namespace cgalpy

//
void export_halfedge(py::class_<cgalpy::aos2::Arrangement_on_surface_2>& c) {
  using Aos = cgalpy::aos2::Arrangement_on_surface_2;
  using Xcv = Aos::X_monotone_curve_2;
  using He = Aos::Halfedge;
  using Ahd = CGAL::Arr_halfedge_direction;
  constexpr auto ri(py::rv_policy::reference_internal);

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  using Env_data = He::Halfedge_data;
  using Dd = CGAL::Dac_decision;
#endif

  if (add_attr<He>(c, "Halfedge")) return;
  py::class_<He> halfedge_c(c, "Halfedge",
                            aos2_doc::Arrangement_on_surface_2_Halfedge_class);
  halfedge_c.def(py::init<>(),
                 aos2_doc::Arrangement_on_surface_2_Halfedge_Halfedge)
    .def("direction", [](const He& h)->Ahd { return h.direction(); },
         aos2_doc::Arrangement_on_surface_2_Halfedge_direction)
    .def("is_fictitious", &He::is_fictitious,
         aos2_doc::Arrangement_on_surface_2_Halfedge_is_fictitious)
    .def("source", &cgalpy::aos2::source, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_source)
    .def("target", &cgalpy::aos2::target, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_target)
    .def("twin", &cgalpy::aos2::twin, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_twin)
    .def("face", &cgalpy::aos2::face, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_face)
    .def("next", &cgalpy::aos2::next, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_next)
    .def("prev", &cgalpy::aos2::prev, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_prev)

    // As a convention, add the suffix `_mutable` to the mutable version.
    // Wrap the mutable method with the `reference_internal` call policy.
    // An unsafe curve that is referenced counted will most likely die when the
    // Aos data structure that holds dies, as the reference counter will vanish.
    // Not all x-monotone curves are referenced counted (perhaps they should...).
    .def("curve_unsafe_mutable", [](He& h)->Xcv& { return h.curve(); }, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_curve)
    .def("curve_unsafe", [](He& h)->Xcv& { return h.curve(); }, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_curve)
    .def("curve",
         [](const He& h)->std::unique_ptr<Xcv>
         { return std::make_unique<Xcv>(h.curve()); }, ri,
         aos2_doc::Arrangement_on_surface_2_Halfedge_curve)
    .def("ccb", &cgalpy::aos2::ccb_iterator, py::keep_alive<0, 1>(),
         aos2_doc::Arrangement_on_surface_2_Halfedge_ccb)

    // Wrap also the function that obtains the real circulator
    .def("ccb_circulator", &cgalpy::aos2::ccb_circulator,
         aos2_doc::Arrangement_on_surface_2_Halfedge_ccb)

#ifdef CGALPY_AOS2_HALFEDGE_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Face. Therefore, we cannot directly refere to any of them, e.g.,
    // `Face::set_data`. Instead, we introduce lambda functions that calls the
    // appropriate member functions.
    .def("set_data", [](He& h, py::object obj) { h.set_data(obj); },
         py::arg("data"), py::keep_alive<1, 2>(),
         aos2_doc::Arr_extended_halfedge_set_data)
    .def("data", [](const He& h)->py::object { return h.data(); },
         aos2_doc::Arr_extended_halfedge_data)
#endif

#ifdef CGALPY_ENVELOPE_3_BINDINGS
    .def("number_of_surfaces", [](He& h) { return h.number_of_surfaces(); },
         "Return the number of envelope surfaces incident to the halfedge.")
    .def("surfaces", &cgalpy::aos2::surfaces, py::keep_alive<0, 1>(),
         "Iterate over envelope surfaces incident to the halfedge.")
#endif
    ;

#ifdef CGALPY_ENVELOPE_3_BINDINGS
  using Si = He::Data_const_iterator;
  add_iterator<Si, Si>("Surface_iterator", halfedge_c);
#endif

  using Chcc = Aos::Ccb_halfedge_const_circulator;
  add_iterator_from_circulator<Chcc>("Halfedge_iterator", halfedge_c);

  //! Wrap also the real circulator
  export_circulator<Chcc>(halfedge_c, "Halfedge_circulator");
}
