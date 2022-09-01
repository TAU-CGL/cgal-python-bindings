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
Iterator_from_circulator<Halfedge_around_vertex_circulator>*
halfedge_around_vertex_iterator(Vertex& v) {
  return new Iterator_from_circulator<Halfedge_around_vertex_circulator>(v.incident_halfedges());
}

}

//
void export_vertex(py::class_<aos2::Arrangement_on_surface_2>& c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Vertex = Aos::Vertex;
  using Face = Aos::Face;
  using Point = Aos::Point_2;
  using Halfedge_around_vertex_circulator =
    Aos::Halfedge_around_vertex_circulator;
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Vertex> vertex_c(c, "Vertex");
  vertex_c.def(py::init<>())

    // As a convention, add the suffix `_mutable` to the mutable version.
    // Wrap the mutable method with the `reference_internal` call policy.
    .def("point_mutable", [](Vertex& v)->Point& { return v.point(); }, ri)
    .def("point", [](const Vertex& v)->const Point& { return v.point(); }, ri)

    .def("is_isolated", [](const Vertex& v)->bool { return v.is_isolated(); })
    .def("degree", &Vertex::degree)
    .def("face", [](const Vertex& v)->const Face& { return *(v.face()); }, ri)
    .def("incident_halfedges", &aos2::halfedge_around_vertex_iterator)
#ifdef CGALPY_AOS2_VERTEX_EXTENDED
    // The member functions set_data() and data() are defined in a base class of
    // Face. Therefore, we cannot directly refere to any of them, e.g.,
    // `Face::set_data`. Instead, we introduce lambda functions that call the
    // appropriate member functions.
    .def("set_data", [](Vertex& v, py::object obj) { v.set_data(obj); })
    .def("data", [](const Vertex& v)->py::object { return v.data(); })
#endif
    ;

  bind_iterator<Iterator_from_circulator<Halfedge_around_vertex_circulator>>
    (vertex_c, "Halfedge_around_vertex_iterator");
}
