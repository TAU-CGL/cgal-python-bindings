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
  using Point = Aos::Point_2;
  using Halfedge_around_vertex_circulator =
    Aos::Halfedge_around_vertex_circulator;

  py::class_<Vertex> vertex_c(c, "Vertex");
  vertex_c.def(py::init<>())

    // As a convention, add the suffix `_mutable` to the mutable version.
    // Wrap the mutable method with the `reference_internal` call policy.
    .def("point_mutable", [](Vertex* v)->const Point& { return v->point(); },
         py::rv_policy::reference_internal)
    .def("point", [](const Vertex* v)->const Point& { return v->point(); },
         py::rv_policy::reference_internal)

    .def("is_isolated", &Vertex::is_isolated)
    .def("degree", &Vertex::degree)
    .def("incident_halfedges", &aos2::halfedge_around_vertex_iterator)
#ifdef CGALPY_AOS2_VERTEX_EXTENDED
    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("data_mutual", py::overload_cast<>(&Vertex::data),
         py::rv_policy::reference_internal)
    .def("data", py::overload_cast<>(&Vertex::data, py::const_))

    .def("set_data", &Vertex::set_data)
#endif
    ;

  bind_iterator<Iterator_from_circulator<Halfedge_around_vertex_circulator>>
    (vertex_c, "Halfedge_around_vertex_iterator");
}
