#ifndef CGALPY_EXPORT_WEIGHTED_POINT_2_HPP
#define CGALPY_EXPORT_WEIGHTED_POINT_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Weighted_point_3.h>

#include "CGALPY/to_string.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

template <typename Kernel_, typename C>
void export_weighted_point_2(C& c) {
  using Ker = Kernel_;
  using Ft = typename Ker::FT;
  using Pnt_2 = typename Ker::Point_2;
  using Wd_pnt_2 = typename Ker::Weighted_point_2;

  c.def(py::init<const CGAL::Origin&>(),
        py::arg("ORIGIN"), "introduces a weighted point with Cartesian coordinates (0,0) and weight 0.")
    .def(py::init<const Pnt_2&>(),
         py::arg("p"), "introduces a weighted point from point p and weight 0.")
    .def(py::init<const Pnt_2&, Ft&>(),
         py::arg("p"), py::arg("w"), "introduces a weighted point from point p and weight w.")
    .def(py::init<const Ft&, const Ft&>(),
         py::arg("x"), py::arg("y"), "introduces a weighted point with coordinates x, y, and weight 0.")
    .def("point", &Wd_pnt_2::point, "returns the point of the weighted point.")
    .def("weight", &Wd_pnt_2::weight, "returns the weight of the weighted point.")
    .def(py::self == py::self,
         py::sig("def __eq__(self, q: object, /) -> bool"), "Test for equality.")
    .def(py::self != py::self,
         py::sig("def __ne__(self, q: object, /) -> bool"), "Test for inequality.")
    .def("hx", &Wd_pnt_2::hx, "returns the homogeneous x coordinate.")
    .def("hy", &Wd_pnt_2::hy, "returns the homogeneous y coordinate.")
    .def("hw", &Wd_pnt_2::hw, "returns the homogenizing coordinate.")
    .def("x", &Wd_pnt_2::x, "returns the Cartesian x coordinate, that is hx()/hw().")
    .def("y", &Wd_pnt_2::y, "returns the Cartesian y coordinate, that is hy()/hw().")
    .def("homogeneous", &Wd_pnt_2::homogeneous, py::arg("i"), "returns the i'th homogeneous coordinate of p.")
    .def("cartesian", &Wd_pnt_2::cartesian, py::arg("i"), "returns the i'th Cartesian coordinate of p.")
    .def("__getitem__", &Wd_pnt_2::operator[], py::arg("i"), "returns cartesian(i).")
    .def("dimension", &Wd_pnt_2::dimension, "returns the dimension (the constant 2).")
    .def("bbox", &Wd_pnt_2::bbox, "returns a bounding box containing p.")
    .def("transform", &Wd_pnt_2::transform, py::arg("t"), "returns the weighted point obtained by applying t on p.")
    .def("__str__", to_string<Wd_pnt_2>)
    .def("__repr__", to_string<Wd_pnt_2>)
    ;

  add_extraction(c);
}

#endif
