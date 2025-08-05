// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/polyhedron_3_types.hpp"

namespace py = nanobind;

namespace pol3 {

const Halfedge& vertex_halfedge(const Vertex& v) { return (*(v.halfedge())); }
const void vertex_set_halfedge(Vertex& v, Halfedge& h)
{ v.set_halfedge(Polyhedron_3::Halfedge_handle(&h)); }

}

//! Export Polhedron Vertex
void export_pol3_vertex(py::class_<pol3::Polyhedron_3>& prn_c) {
  using Prn = pol3::Polyhedron_3;
  using Vertex = Prn::Vertex;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Vertex>(prn_c, "Vertex")) return;

  py::class_<Vertex> vertex_c(prn_c, "Vertex");
  vertex_c.def(py::init<>())
    .def("degree", [](const Vertex& v) { return v.degree(); })
    .def("halfedge", pol3::vertex_halfedge, ri)
    .def("is_bivalent", [](const Vertex& v) { return v.is_bivalent(); })
    .def("is_trivalent", [](const Vertex& v) { return v.is_trivalent(); })
    .def("point", [](const Vertex& v){ return v.point(); }, ri)
    .def("set_halfedge", pol3::vertex_set_halfedge)
    .def("vertex_degree", [](const Vertex& v) { return v.vertex_degree(); })
#ifdef CGALPY_POL3_VERTEX_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Vertex. Therefore, we cannot directly refere to any of them, e.g.,
    // `Vertex::set_data`. Instead, we introduce lambda functions that call
    // the appropriate member functions.
    .def("set_data", [](Vertex& v, py::object obj) { v.set_data(obj); })
    .def("data", [](const Vertex& v)->py::object { return v.data(); })
#endif
    ;

}
