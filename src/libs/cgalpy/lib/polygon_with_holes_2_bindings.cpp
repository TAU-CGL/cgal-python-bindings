// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace py = nanobind;

namespace pol2 {

Polygon_with_holes_2* init_polygon_with_holes_2(Polygon_2& p, py::list& lst) {
  auto begin = py::stl_input_iterator< Polygon_2 >(lst);
  auto end = py::stl_input_iterator< Polygon_2 >();
  return new Polygon_with_holes_2(p, begin, end);
}

Polygon_with_holes_2::Hole_const_iterator holes_begin(Polygon_with_holes_2& p)
{ return p.holes_begin(); }

Polygon_with_holes_2::Hole_const_iterator holes_end(Polygon_with_holes_2& p)
{ return p.holes_end(); }

Polygon_2& outer_boundary(Polygon_with_holes_2& p)
{ return p.outer_boundary(); }

}

void export_polygon_with_holes_2() {
  typedef pol2::Polygon_2               Polygon_2;
  typedef pol2::Polygon_with_holes_2    Polygon_with_holes_2;

  py::class_<Polygon_with_holes_2>("Polygon_with_holes_2")
    .def(py::init<Polygon_2&>())
    .def("__init__", make_constructor(&pol2::init_polygon_with_holes_2))
    .def("is_unbounded", &Polygon_with_holes_2::is_unbounded)
    .def("outer_boundary", &pol2::outer_boundary,
         py::return_internal_reference<>())
    .def("holes", py::range<py::return_internal_reference<>>(&pol2::holes_begin,
                                                             &pol2::holes_end))
    .def("number_of_holes", &Polygon_with_holes_2::number_of_holes)
    .def("bbox", &Polygon_with_holes_2::bbox)
    .def(py::self_ns::str(py::self_ns::self))
    .def(py::self_ns::repr(py::self_ns::self))
    .def(py::self == py::self)
    .def(py::self != py::self)
    ;
}
