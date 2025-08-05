// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/export_circulator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/polyhedron_3_types.hpp"

namespace pol3 {

/// \name BGL Iterators & Circulators
/// @{

//! Wrap the function that obtains the real circulator
auto halfedges_around_target_v_circulator(Vertex& v, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hatc = CGAL::Halfedge_around_target_circulator<Prn>;
  return Hatc(Vertex_handle(&v), prn);
}

//! Wrap the iterator
auto halfedges_around_target_v_iterator(Vertex& v, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hati = CGAL::Halfedge_around_target_iterator<Prn>;
  Hati begin, end;
  boost::tie(begin, end) = CGAL::halfedges_around_target(Vertex_handle(&v), prn);
  return make_iterator(begin, end);
}

//! Wrap the function that obtains the real circulator
auto halfedges_around_target_h_circulator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hatc = CGAL::Halfedge_around_target_circulator<Prn>;
  return Hatc(Halfedge_handle(&h), prn);
}

//! Wrap the iterator
auto halfedges_around_target_h_iterator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hati = CGAL::Halfedge_around_target_iterator<Prn>;
  Hati begin, end;
  boost::tie(begin, end) = CGAL::halfedges_around_target(Halfedge_handle(&h), prn);
  return make_iterator(begin, end);
}

//! Wrap the function that obtains the real circulator
auto halfedges_around_source_v_circulator(Vertex& v, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hasc = CGAL::Halfedge_around_source_circulator<Prn>;
  return Hasc(Vertex_handle(&v), prn);
}

//! Wrap the iterator
auto halfedges_around_source_v_iterator(Vertex& v, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hasi = CGAL::Halfedge_around_source_iterator<Prn>;
  Hasi begin, end;
  boost::tie(begin, end) = CGAL::halfedges_around_source(Vertex_handle(&v), prn);
  return make_iterator(begin, end);
}

//! Wrap the function that obtains the real circulator
auto halfedges_around_source_h_circulator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hasc = CGAL::Halfedge_around_source_circulator<Prn>;
  return Hasc(Halfedge_handle(&h), prn);
}

//! Wrap the iterator
auto halfedges_around_source_h_iterator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hasi = CGAL::Halfedge_around_source_iterator<Prn>;
  Hasi begin, end;
  boost::tie(begin, end) = CGAL::halfedges_around_source(Halfedge_handle(&h), prn);
  return make_iterator(begin, end);
}

/// @}

}

void export_pol3_bgl(py::module_& m) {
  using Prn = pol3::Polyhedron_3;
  using Vertex = Prn::Vertex;
  using Halfedge = Prn::Halfedge;
  using Face = Prn::Face;

  constexpr auto ri(py::rv_policy::reference_internal);

  // Iterators and Circulators

  // Halfedges around target
  // We use the dereference circulator, because we need to dereference twice
  using Hati = CGAL::Halfedge_around_target_iterator<Prn>;
  add_dereference_iterator<Hati, Hati, Halfedge&>("Halfedge_around_target_iterator", m);

  m.def("halfedges_around_target", &pol3::halfedges_around_target_v_iterator, py::arg("v"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex v as target",
        py::keep_alive<0, 1>());
  m.def("halfedges_around_target", &pol3::halfedges_around_target_h_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex target(h,g) as target",
        py::keep_alive<0, 1>());

  using Hatc = CGAL::Halfedge_around_target_circulator<Prn>;
  export_dereference_circulator<Hatc, Halfedge&>(m, "Halfedge_around_target_circulator");
  m.def("halfedges_around_target_circulator", &pol3::halfedges_around_target_v_circulator, py::arg("v"), py::arg("g"),
        "Obtain a circulator  over all halfedges with vertex v as target");
  m.def("halfedges_around_target_circulator", &pol3::halfedges_around_target_h_circulator, py::arg("h"), py::arg("g"),
        "Obtain a circulator  over all halfedges with vertex target(h,g) as target");

  // Halfedges around source
  // We use the dereference circulator, because we need to dereference twice
  using Hasi = CGAL::Halfedge_around_source_iterator<Prn>;
  add_dereference_iterator<Hasi, Hasi, Halfedge&>("Halfedge_around_source_iterator", m);

  m.def("halfedges_around_source", &pol3::halfedges_around_source_v_iterator, py::arg("v"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex v as source",
        py::keep_alive<0, 1>());
  m.def("halfedges_around_source", &pol3::halfedges_around_source_h_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex source(h,g) as source",
        py::keep_alive<0, 1>());

  using Hasc = CGAL::Halfedge_around_source_circulator<Prn>;
  export_dereference_circulator<Hasc, Halfedge&>(m, "Halfedge_around_source_circulator");
  m.def("halfedges_around_source_circulator", &pol3::halfedges_around_source_v_circulator, py::arg("v"), py::arg("g"),
        "Obtain a circulator  over all halfedges with vertex v as source");
  m.def("halfedges_around_source_circulator", &pol3::halfedges_around_source_h_circulator, py::arg("h"), py::arg("g"),
        "Obtain a circulator  over all halfedges with vertex source(h,g) as source");
}
