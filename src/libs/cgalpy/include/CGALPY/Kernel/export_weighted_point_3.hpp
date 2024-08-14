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

#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"
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

  c.def(py::init<>())
    // .def(py::init<const CGAL::Origin&>())
    .def(py::init<const Pnt_3&>())
    .def(py::init<const Pnt_3&, const Ft&>())
    // .def(py::init<const Ft&, const Ft&, const Ft&>())
    // Accessors
    .def("point", &Wd_pnt_3::point)
    .def("weight", &Wd_pnt_3::weight)
    .def("x", &Wd_pnt_3::x)
    .def("y", &Wd_pnt_3::y)
    .def("z", &Wd_pnt_3::z)
    .def("hx", &Wd_pnt_3::hx)
    .def("hy", &Wd_pnt_3::hy)
    .def("hz", &Wd_pnt_3::hz)
    .def("hw", &Wd_pnt_3::hw)
    // Operations
    .def("__str__", to_string<Wd_pnt_3>)
    .def("__repr__", to_string<Wd_pnt_3>)
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    // Convenient operations
    .def("homogeneous", &Wd_pnt_3::homogeneous)
    .def("cartesian", &Wd_pnt_3::cartesian)
    // Ker::FT 	operator[] (int i) const
    // Cartesian_const_iterator 	cartesian_begin () const
    // Cartesian_const_iterator 	cartesian_end () const
    .def("dimension", &Wd_pnt_3::dimension)
    .def("bbox", &Wd_pnt_3::bbox)
    // .def("transform", &Wd_pnt_3::transform)
    //.setattr("__hash__", &hash<Point_3>)
    ;

  add_extraction(c);
}

#endif
