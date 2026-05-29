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

namespace cgalpy {
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

//! Wrap the function that obtains the real circulator
auto halfedges_around_face_circulator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hafc = CGAL::Halfedge_around_face_circulator<Prn>;
  return Hafc(Halfedge_handle(&h), prn);
}

//! Wrap the iterator
auto halfedges_around_face_iterator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Hafi = CGAL::Halfedge_around_face_iterator<Prn>;
  Hafi begin, end;
  boost::tie(begin, end) = CGAL::halfedges_around_face(Halfedge_handle(&h), prn);
  return make_iterator(begin, end);
}

//! Wrap the function that obtains the real circulator
auto faces_around_target_circulator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Fatc = CGAL::Face_around_target_circulator<Prn>;
  return Fatc(Halfedge_handle(&h), prn);
}

//! Wrap the iterator
auto faces_around_target_iterator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Fati = CGAL::Face_around_target_iterator<Prn>;
  Fati begin, end;
  boost::tie(begin, end) = CGAL::faces_around_target(Halfedge_handle(&h), prn);
  return make_iterator(begin, end);
}

// CGAL::Face_around_face_circulator<Graph> is only a placeholder in
// current CGAL headers; it has no constructor/increment/dereference API.
// Keep the Face_around_face_iterator binding below instead.

//! Wrap the iterator
auto faces_around_face_iterator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Fafi = CGAL::Face_around_face_iterator<Prn>;
  Fafi begin, end;
  boost::tie(begin, end) = CGAL::faces_around_face(Halfedge_handle(&h), prn);
  return make_iterator(begin, end);
}

//! Wrap the function that obtains the real circulator
auto vertices_around_target_circulator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Vatc = CGAL::Vertex_around_target_circulator<Prn>;
  return Vatc(Halfedge_handle(&h), prn);
}

//! Wrap the iterator
auto vertices_around_target_iterator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Vati = CGAL::Vertex_around_target_iterator<Prn>;
  Vati begin, end;
  boost::tie(begin, end) = CGAL::vertices_around_target(Halfedge_handle(&h), prn);
  return make_iterator(begin, end);
}

//! Wrap the function that obtains the real circulator
auto vertices_around_face_circulator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Vafc = CGAL::Vertex_around_face_circulator<Prn>;
  return Vafc(Halfedge_handle(&h), prn);
}

//! Wrap the iterator
auto vertices_around_face_iterator(Halfedge& h, const Polyhedron_3& prn) {
  using Prn = Polyhedron_3;
  using Vafi = CGAL::Vertex_around_face_iterator<Prn>;
  Vafi begin, end;
  boost::tie(begin, end) = CGAL::vertices_around_face(Halfedge_handle(&h), prn);
  return make_iterator(begin, end);
}

/// @}

}
} // namespace cgalpy

void export_pol3_bgl(py::module_& m) {
  using Prn = cgalpy::pol3::Polyhedron_3;
  using Vertex = Prn::Vertex;
  using Halfedge = Prn::Halfedge;
  using Face = Prn::Face;

  constexpr auto ri(py::rv_policy::reference_internal);

  // Iterators and Circulators

  // Halfedges around target
  // We use the dereference circulator, because we need to dereference twice
  using Hati = CGAL::Halfedge_around_target_iterator<Prn>;
  add_dereference_iterator<Hati, Hati, Halfedge&>("Halfedge_around_target_iterator", m);
  m.def("halfedges_around_target", &cgalpy::pol3::halfedges_around_target_v_iterator, py::arg("v"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex v as target",
        py::keep_alive<0, 1>());
  m.def("halfedges_around_target", &cgalpy::pol3::halfedges_around_target_h_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex target(h,g) as target",
        py::keep_alive<0, 1>());

  using Hatc = CGAL::Halfedge_around_target_circulator<Prn>;
  export_dereference_circulator<Hatc, Halfedge&>(m, "Halfedge_around_target_circulator");
  m.def("halfedges_around_target_circulator", &cgalpy::pol3::halfedges_around_target_v_circulator, py::arg("v"), py::arg("g"),
        "Obtain a circulator over all halfedges with vertex v as target");
  m.def("halfedges_around_target_circulator", &cgalpy::pol3::halfedges_around_target_h_circulator, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all halfedges with vertex target(h,g) as target");

  // Halfedges around source
  // We use the dereference circulator, because we need to dereference twice
  using Hasi = CGAL::Halfedge_around_source_iterator<Prn>;
  add_dereference_iterator<Hasi, Hasi, Halfedge&>("Halfedge_around_source_iterator", m);
  m.def("halfedges_around_source", &cgalpy::pol3::halfedges_around_source_v_iterator, py::arg("v"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex v as source",
        py::keep_alive<0, 1>());
  m.def("halfedges_around_source", &cgalpy::pol3::halfedges_around_source_h_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all halfedges with vertex source(h,g) as source",
        py::keep_alive<0, 1>());

  using Hasc = CGAL::Halfedge_around_source_circulator<Prn>;
  export_dereference_circulator<Hasc, Halfedge&>(m, "Halfedge_around_source_circulator");
  m.def("halfedges_around_source_circulator", &cgalpy::pol3::halfedges_around_source_v_circulator, py::arg("v"), py::arg("g"),
        "Obtain a circulator over all halfedges with vertex v as source");
  m.def("halfedges_around_source_circulator", &cgalpy::pol3::halfedges_around_source_h_circulator, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all halfedges with vertex source(h,g) as source");

  // Halfedges around face
  // We use the dereference circulator, because we need to dereference twice
  using Hafi = CGAL::Halfedge_around_face_iterator<Prn>;
  add_dereference_iterator<Hafi, Hafi, Halfedge&>("Halfedge_around_face_iterator", m);
  m.def("halfedges_around_face", &cgalpy::pol3::halfedges_around_face_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all halfedges incident to the same face or border as h",
        py::keep_alive<0, 1>());

  using Hafc = CGAL::Halfedge_around_face_circulator<Prn>;
  export_dereference_circulator<Hafc, Halfedge&>(m, "Halfedge_around_face_circulator");
  m.def("halfedges_around_face_circulator", &cgalpy::pol3::halfedges_around_face_circulator, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all halfedges incident to the same face or border as h");

  // Faces around target
  // We use the dereference circulator, because we need to dereference twice
  using Fati = CGAL::Face_around_target_iterator<Prn>;
  add_dereference_iterator<Fati, Fati, Face&>("Face_around_target_iterator", m);
  m.def("Face_around_target", &cgalpy::pol3::faces_around_target_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all faces around vertex target(h,g)",
        py::keep_alive<0, 1>());

  using Fatc = CGAL::Face_around_target_circulator<Prn>;
  export_dereference_circulator<Fatc, Face&>(m, "Face_around_target_circulator");
  m.def("faces_around_target_circulator", &cgalpy::pol3::faces_around_target_circulator, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all faces around vertex target(h,g)");

  // Faces around face
  // We use the dereference circulator, because we need to dereference twice
  using Fafi = CGAL::Face_around_face_iterator<Prn>;
  add_dereference_iterator<Fafi, Fafi, Face&>("Face_around_face_iterator", m);
  m.def("Face_around_face", &cgalpy::pol3::faces_around_face_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all faces adjacent to the same face face(h,g)",
        py::keep_alive<0, 1>());

  // Do not export Face_around_face_circulator here.  CGAL currently exposes it
  // as an empty placeholder type.  The iterator API Face_around_face remains
  // exported above and is the usable binding.

  // Vertices around target
  // We use the dereference circulator, because we need to dereference twice
  using Vati = CGAL::Vertex_around_target_iterator<Prn>;
  add_dereference_iterator<Vati, Vati, Vertex&>("Vertex_around_target_iterator", m);
  m.def("Vertex_around_target", &cgalpy::pol3::vertices_around_target_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all vertices adjacent to the vertex target(h,g)",
        py::keep_alive<0, 1>());

  using Vatc = CGAL::Vertex_around_target_circulator<Prn>;
  export_dereference_circulator<Vatc, Vertex&>(m, "Vertex_around_target_circulator");
  m.def("vertices_around_target_circulator", &cgalpy::pol3::vertices_around_target_circulator, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all vertices adjacent to the vertex target(h,g)");

  // Vertices around face
  // We use the dereference circulator, because we need to dereference twice
  using Vafi = CGAL::Vertex_around_face_iterator<Prn>;
  add_dereference_iterator<Vafi, Vafi, Vertex&>("Vertex_around_face_iterator", m);
  m.def("Vertex_around_face", &cgalpy::pol3::vertices_around_face_iterator, py::arg("h"), py::arg("g"),
        "Obtain an iterator over all vertices adjacent to the face face(h,g)",
        py::keep_alive<0, 1>());

  using Vafc = CGAL::Vertex_around_face_circulator<Prn>;
  export_dereference_circulator<Vafc, Vertex&>(m, "Vertex_around_face_circulator");
  m.def("vertices_around_face_circulator", &cgalpy::pol3::vertices_around_face_circulator, py::arg("h"), py::arg("g"),
        "Obtain a circulator over all vertices adjacent to the face face(h,g)");
}
