// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/kernel_types.hpp"
#include "cgalpy/polyhedron_3_types.hpp"
#include "cgalpy/Pol3_docstrings.hpp"

namespace py = nanobind;
namespace pol3_doc = cgalpy::pol3::docstrings;

namespace cgalpy {
namespace pol3 {

const Halfedge& opposite(const Halfedge& h) { return *(h.opposite()); }
const Halfedge& next(const Halfedge& h) { return *(h.next()); }
const Halfedge& next_on_vertex(const Halfedge& h) { return *(h.next_on_vertex()); }
const Halfedge& prev(const Halfedge& h) { return *(h.prev()); }
const Halfedge& prev_on_vertex(const Halfedge& h) { return *(h.prev_on_vertex()); }
const Face& face(const Halfedge& h) { return *(h.face()); }
const Face& facet(const Halfedge& h) { return *(h.facet()); }
const Vertex& vertex(const Halfedge& h) { return *(h.vertex()); }

}
} // namespace cgalpy

// Export Polyhedron Halfedge
void export_pol3_halfedge(py::class_<cgalpy::pol3::Polyhedron_3>& prn_c) {
  using Prn = cgalpy::pol3::Polyhedron_3;
  using Halfedge = Prn::Halfedge;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Halfedge>(prn_c, "Halfedge")) return;

  py::class_<Halfedge> halfedge_c(prn_c, "Halfedge", pol3_doc::Polyhedron_3_Halfedge_class);
  halfedge_c.def(py::init<>(), pol3_doc::Polyhedron_3_Halfedge_Halfedge)
    .def("facet", &cgalpy::pol3::facet, ri, pol3_doc::Polyhedron_3_Halfedge_facet)
    .def("face", &cgalpy::pol3::face, ri, pol3_doc::Polyhedron_3_Halfedge_facet)
    .def("facet_degree", [](const Halfedge& h) { return h.facet_degree(); }, pol3_doc::Polyhedron_3_Halfedge_facet_degree)
    .def("is_border", [](const Halfedge& h) { return h.is_border(); }, pol3_doc::Polyhedron_3_Halfedge_is_border)
    .def("is_border_edge", [](const Halfedge& h) { return h.is_border_edge(); }, pol3_doc::Polyhedron_3_Halfedge_is_border_edge)
    .def("is_quad", [](const Halfedge& h) { return h.is_quad(); }, pol3_doc::Polyhedron_3_Halfedge_is_quad)
    .def("is_bivalent", [](const Halfedge& h) { return h.is_bivalent(); }, pol3_doc::Polyhedron_3_Halfedge_is_bivalent)
    .def("is_triangle", [](const Halfedge& h) { return h.is_triangle(); }, pol3_doc::Polyhedron_3_Halfedge_is_triangle)
    .def("is_trivalent", [](const Halfedge& h){  return h.is_trivalent(); }, pol3_doc::Polyhedron_3_Halfedge_is_trivalent)
    .def("next", &cgalpy::pol3::next, ri, pol3_doc::Polyhedron_3_Halfedge_next)
    .def("next_on_vertex", &cgalpy::pol3::next_on_vertex, ri, pol3_doc::Polyhedron_3_Halfedge_next_on_vertex)
    .def("prev", &cgalpy::pol3::prev, ri, pol3_doc::Polyhedron_3_Halfedge_prev)
    .def("prev_on_vertex", &cgalpy::pol3::prev_on_vertex, ri, pol3_doc::Polyhedron_3_Halfedge_prev_on_vertex)
    .def("opposite", &cgalpy::pol3::opposite, ri, pol3_doc::Polyhedron_3_Halfedge_opposite)
    .def("vertex", &cgalpy::pol3::vertex, ri, pol3_doc::Polyhedron_3_Halfedge_vertex)
    .def("vertex_degree", [](const Halfedge& h) { return h.vertex_degree(); }, pol3_doc::Polyhedron_3_Halfedge_vertex_degree)
#ifdef CGALPY_POL3_HALFEDGE_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Halfedge. Therefore, we cannot directly refere to any of them, e.g.,
    // `Halfedge::set_data`. Instead, we introduce lambda functions that call
    // the appropriate member functions.
    .def("set_data", [](Halfedge& h, py::object obj) { h.set_data(obj); },
         py::arg("data"),
         "Sets the user data stored on the halfedge.")
    .def("data", [](const Halfedge& h)->py::object { return h.data(); },
         "Returns the user data stored on the halfedge.")
#endif
    ;

}
