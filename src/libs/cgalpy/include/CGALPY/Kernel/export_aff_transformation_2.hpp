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

// Export a two-dimensional triangle
template <typename Kernel_, typename C>
void export_aff_transformation_2(C& c) {
  using Ker = Kernel_;
  using Rt = typename Ker::RT;
  using Vec_2 = typename Ker::Vector_2;
  using Dir_2 = typename Ker::Direction_2;
  using Aff_2 = typename Ker::Aff_transformation_2;

  c.def(py::init<>())
    .def(py::init<Rt&, Rt&, Rt&, Rt&, Rt&>())
    .def(py::init<Rt, Rt, Rt, Rt>())
    .def(py::init<Rt&, Rt&, Rt&, Rt&, Rt&, Rt&, Rt&>())
    .def(py::init<Rt, Rt, Rt, Rt, Rt, Rt, Rt>())
    .def(py::init<CGAL::Translation, const Vec_2&>())
    .def(py::init<CGAL::Rotation, const Dir_2&, const Rt&, const Rt&>())
    .def(py::init<CGAL::Rotation, const Dir_2&, const Rt, const Rt>())
    .def(py::init<CGAL::Rotation, const Rt&, const Rt&, const Rt&>())
    .def(py::init<CGAL::Rotation, const Rt, const Rt, const Rt>())
    .def(py::init<CGAL::Scaling, const Rt&, const Rt&>())
    .def(py::init<CGAL::Scaling, const Rt, const Rt>())
    .def("transform", &transform_point<Ker>)
    .def("transform", &transform_vector<Ker>)
    .def("transform", &transform_direction<Ker>)
    .def("transform", &transform_line<Ker>)
    .def("inverse", &Aff_2::inverse)
    .def("is_even", &Aff_2::is_even)
    .def("is_odd", &Aff_2::is_odd)
    .def("cartesian", &Aff_2::cartesian)
    .def("m", &Aff_2::m)
    .def("homogeneous", &Aff_2::homogeneous)
    .def("hm", &Aff_2::hm)
    .def("__str__", to_string<Aff_2>)
    .def("__repr__", to_string<Aff_2>)
    .def(py::self * py::self)
    ;
}

#endif
