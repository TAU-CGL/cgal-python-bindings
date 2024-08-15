// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AFF_TRANSFORMATION_2_HPP
#define CGALPY_EXPORT_AFF_TRANSFORMATION_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/to_string.hpp"

namespace py = nanobind;

//
template <typename Kernel_>
typename Kernel_::Point_2
transform_point(typename Kernel_::Aff_transformation_2& t,
                typename Kernel_::Point_2& p)
{ return t.transform(p); }

//
template <typename Kernel_>
typename Kernel_::Vector_2
transform_vector(typename Kernel_::Aff_transformation_2& t,
                 typename Kernel_::Vector_2& v)
{ return t.transform(v); }

//
template <typename Kernel_>
typename Kernel_::Direction_2
transform_direction(typename Kernel_::Aff_transformation_2& t,
                    typename Kernel_::Direction_2& d)
{ return t.transform(d); }

//
template <typename Kernel_>
typename Kernel_::Line_2
transform_line(typename Kernel_::Aff_transformation_2& t,
               typename Kernel_::Line_2& l)
{ return t.transform(l); }

//
template <typename Kernel_>
typename Kernel_::Point_2
op_ptr(typename Kernel_::Aff_transformation_2& t,
       typename Kernel_::Point_2& p)
{ return t(p); }

//
template <typename Kernel_>
typename Kernel_::Vector_2
op_vtr(typename Kernel_::Aff_transformation_2& t,
       typename Kernel_::Vector_2& v)
{ return t(v); }

//
template <typename Kernel_>
typename Kernel_::Direction_2
op_dir(typename Kernel_::Aff_transformation_2& t,
       typename Kernel_::Direction_2& d)
{ return t(d); }

//
template <typename Kernel_>
typename Kernel_::Line_2
op_ln(typename Kernel_::Aff_transformation_2& t,
      typename Kernel_::Line_2& l)
{ return t(l); }

// Export a two-dimensional triangle
template <typename Kernel_, typename C>
void export_aff_transformation_2(C& c) {
  using Ker = Kernel_;
  using Rt = typename Ker::RT;
  using Vec_2 = typename Ker::Vector_2;
  using Dir_2 = typename Ker::Direction_2;
  using Aff_2 = typename Ker::Aff_transformation_2;
  c.def(py::init<const CGAL::Identity_transformation&>(),
        py::arg("IDENTITY"), "introduces an identity transformation.")
    .def(py::init<const CGAL::Translation, const Vec_2&>(),
         py::arg("TRANSLATION"), py::arg("v"), "introduces a translation by a vector v.")
    .def(py::init<const CGAL::Rotation, const Dir_2&, const Rt&, const Rt&>(),
         py::arg("ROTATION"), py::arg("d"), py::arg("num"), py::arg("den") = Rt(1),
         "approximates the rotation over the angle indicated by direction d, such that the differences between the sines and cosines of the rotation given by d and the approximating rotation are at most num/den each.")
    .def(py::init<const CGAL::Rotation, const Rt&, const Rt&, const Rt&>(),
         py::arg("ROTATION"), py::arg("sine_rho"), py::arg("cosine_rho"), py::arg("hw") = Rt(1),
         "introduces a rotation by the angle rho.")
    .def(py::init<const CGAL::Scaling, const Rt&, const Rt&>(),
         py::arg("SCALING"), py::arg("s"), py::arg("hw") = Rt(1),
         "introduces a scaling by a scale factor s/hw.")
    .def(py::init<const CGAL::Reflection, const typename Ker::Line_2&>(),
         py::arg("REFLECTION"), py::arg("l"), "introduces a reflection by a line l.")
    .def(py::init<const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&, const Rt&>(),
         py::arg("m00"), py::arg("m01"), py::arg("m02"), py::arg("m10"), py::arg("m11"), py::arg("m12"), py::arg("hw") = Rt(1),
         "introduces a general affine transformation in the 3×3 matrix form ⎛⎝⎜m00m100m01m110m02m12hw⎞⎠⎟.")
    .def(py::init<const Rt&, const Rt&, const Rt&, const Rt&, const Rt&>(),
         py::arg("m00"), py::arg("m01"), py::arg("m10"), py::arg("m11"), py::arg("hw") = Rt(1),
         "introduces a general affine transformation in the 3×3 matrix form ⎛⎝⎜m00m100m01m110m02m12⎞⎠⎟.")
    .def("transform", &transform_point<Ker>,
         py::sig("def transform(self, p: Point_2, /) -> Point_2"))
    .def("transform", &transform_vector<Ker>,
         py::sig("def transform(self, v: Vector_2, /) -> Vector_2"))
    .def("transform", &transform_direction<Ker>,
         py::sig("def transform(self, d: Direction_2, /) -> Direction_2"))
    .def("transform", &transform_line<Ker>,
         py::sig("def transform(self, l: Line_2, /) -> Line_2"))
    .def("__call__", &op_ptr<Ker>,
         py::sig("def __call__(self, p: Point_2, /) -> Point_2"))
    .def("__call__", &op_vtr<Ker>,
         py::sig("def __call__(self, v: Vector_2, /) -> Vector_2"))
    .def("__call__", &op_dir<Ker>,
         py::sig("def __call__(self, d: Direction_2, /) -> Direction_2"))
    .def("__call__", &op_ln<Ker>,
         py::sig("def __call__(self, l: Line_2, /) -> Line_2"))
    .def(py::self * py::self, py::arg("s"), "composes two affine transformations.")
    .def("inverse", &Aff_2::inverse, "gives the inverse transformation.")
    .def(py::self == py::self, py::arg("s"), "compares two affine transformations.")
    .def("is_even", &Aff_2::is_even, "returns true, if the transformation is not reflecting, i.e. the determinant of the involved linear transformation is non-negative.")
    .def("is_odd", &Aff_2::is_odd, "returns true, if the transformation is reflecting.")
    .def("is_scaling", &Aff_2::is_scaling, "returns true, if the object was constructed using the tag CGAL::Scaling, or is the result of the composition of only such scaling transformation objects.")
    .def("is_translation", &Aff_2::is_translation, "returns true, if the object was constructed using the tag CGAL::Translation, or is the result of the composition of only such translation transformation objects.")
    .def("is_rotation", &Aff_2::is_rotation, "returns true, if the object was constructed using the tag CGAL::Rotation, or is the result of the composition of only such rotation transformation objects.")
    .def("is_reflection", &Aff_2::is_reflection, "returns true, if the object was constructed using the tag CGAL::Reflection, or is the result of the composition of only such reflection transformation objects.")
    .def("cartesian", &Aff_2::cartesian, py::arg("i"), py::arg("j"), "returns entry mij in a matrix representation in which m22 is 1.")
    .def("m", &Aff_2::m, py::arg("i"), py::arg("j"), "returns entry mij in a matrix representation in which m22 is 1.")
    .def("homogeneous", &Aff_2::homogeneous, py::arg("i"), py::arg("j"), "returns entry mij in some fixed matrix representation.")
    .def("hm", &Aff_2::hm, py::arg("i"), py::arg("j"), "returns entry mij in some fixed matrix representation.")
    ;
}

#endif
