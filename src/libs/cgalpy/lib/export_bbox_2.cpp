// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>

#include <CGAL/Bbox_2.h>
#include "cgalpy/to_string.hpp"

namespace py = nanobind;

//
void export_bbox_2(py::class_<CGAL::Bbox_2>& c) {
  c.def(py::init<>())
    .def(py::init<double, double, double, double>())
    .def("dimension", &CGAL::Bbox_2::dimension)
    .def("dilate", &CGAL::Bbox_2::dilate)
    .def("xmin", &CGAL::Bbox_2::xmin)
    .def("ymin", &CGAL::Bbox_2::ymin)
    .def("xmax", &CGAL::Bbox_2::xmax)
    .def("ymax", &CGAL::Bbox_2::ymax)
    .def("min", &CGAL::Bbox_2::min)
    .def("max", &CGAL::Bbox_2::max)
    .def("__str__", to_string<CGAL::Bbox_2>)
    .def("__repr__", to_string<CGAL::Bbox_2>)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    .def(py::self += py::self)
    .def(py::self + py::self)
    ;
}
