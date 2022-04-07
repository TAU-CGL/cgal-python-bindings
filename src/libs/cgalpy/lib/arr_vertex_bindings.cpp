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

Iterator_from_circulator<Halfedge_around_vertex_circulator>*
halfedge_around_vertex_iterator(Vertex& v) {
  return new Iterator_from_circulator<Halfedge_around_vertex_circulator>(v.incident_halfedges());
}

}

void export_vertex() {
  typedef aos2::Arrangement_2   Arr2;
  typedef Arr2::Vertex          Vertex;
  typedef Arr2::Point_2         Point;

  py::class_<Vertex>("Vertex")
    .def(py::init<>())
    .def<Point& (Vertex::*)()>("point", &Vertex::point, py::return_internal_reference<>())
    .def("is_isolated", &Vertex::is_isolated)
    .def("degree", &Vertex::degree)
    .def("incident_halfedges", &aos2::halfedge_around_vertex_iterator, Manage_new_object())
#ifdef CGALPY_AOS2_VERTEX_EXTENDED
    .def<Vertex::Data& (Vertex::*)()>("data", &Vertex::data, Copy_non_const_reference())
    .def("set_data", &Vertex::set_data)
#endif
    ;

  bind_iterator<Iterator_from_circulator<Arr2::Halfedge_around_vertex_circulator>>("Halfedge_around_vertex_iterator");
}
