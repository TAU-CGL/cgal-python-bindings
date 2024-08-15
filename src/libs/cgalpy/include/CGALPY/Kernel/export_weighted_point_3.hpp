// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_WEIGHTED_POINT_3_HPP
#define CGALPY_EXPORT_WEIGHTED_POINT_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Origin.h>

#include "CGALPY/add_extraction.hpp"
#include "CGALPY/to_string.hpp"

namespace py = nanobind;

// Export a 3D weighted point.
template <typename Kernel_, typename C>
void export_weighted_point_3(C& c) {
  using Ker = Kernel_;
  using Ft = typename Ker::FT;
  using Pnt_3 = typename Ker::Point_3;
  using Wd_pnt_3 = typename Ker::Weighted_point_3;
  c.def(py::init<const CGAL::Origin&>(),
        py::arg("ORIGIN"), "introduces a weighted point with Cartesian coordinates (0,0,0) and weight 0.")
    .def(py::init<const Pnt_3&>(),
         py::arg("p"), "introduces a weighted point from point p and weight 0.")
    .def(py::init<const Pnt_3&, Ft&>(),
         py::arg("p"), py::arg("w"), "introduces a weighted point from point p and weight w.")
    .def(py::init<Ft&, Ft&, Ft&>(),
         py::arg("x"), py::arg("y"), py::arg("z"), "introduces a weighted point with coordinates x, y, z and weight 0.")
    .def("point", &Wd_pnt_3::point, "returns the point of the weighted point.")
    .def("weight", &Wd_pnt_3::weight, "returns the weight of the weighted point.")
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"), "Test for equality.")
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"), "Test for inequality.")
    .def("hx", &Wd_pnt_3::hx, "returns the homogeneous x coordinate.")
    .def("hy", &Wd_pnt_3::hy, "returns the homogeneous y coordinate.")
    .def("hz", &Wd_pnt_3::hz, "returns the homogeneous z coordinate.")
    .def("hw", &Wd_pnt_3::hw, "returns the homogenizing coordinate.")
    .def("x", &Wd_pnt_3::x, "returns the Cartesian x coordinate, that is hx()/hw().")
    .def("y", &Wd_pnt_3::y, "returns the Cartesian y coordinate, that is hy()/hw().")
    .def("z", &Wd_pnt_3::z, "returns the Cartesian z coordinate, that is hz()/hw().")
    .def("homogeneous", &Wd_pnt_3::homogeneous, py::arg("i"), "returns the i'th homogeneous coordinate of p.")
    .def("cartesian", &Wd_pnt_3::cartesian, py::arg("i"), "returns the i'th Cartesian coordinate of p.")
    .def("__getitem__", &Wd_pnt_3::operator[], py::arg("i"), "returns cartesian(i).")
    .def("dimension", &Wd_pnt_3::dimension, "returns the dimension (the constant 3).")
    .def("bbox", &Wd_pnt_3::bbox, "returns a bounding box containing p.")
    .def("transform", &Wd_pnt_3::transform,
         py::arg("t"), "returns the weighted point obtained by applying t on p.")
    .def("__str__", to_string<Wd_pnt_3>)
    .def("__repr__", to_string<Wd_pnt_3>)
    ;

  add_extraction(c);
}

#endif
