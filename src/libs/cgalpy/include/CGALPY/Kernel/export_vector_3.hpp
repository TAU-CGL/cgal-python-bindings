// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_VECTOR_3_HPP
#define CGALPY_EXPORT_VECTOR_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"

namespace bp = nanobind;

// Export a two-dimensional vector of a kernel.
template <typename Kernel, typename C>
void export_vector_3(C& c) {
  using FT = typename Kernel::FT;
  using RT = typename Kernel::RT;
  using Pnt = typename Kernel::Point_3;
  using Vec = typename Kernel::Vector_3;
  using Line = typename Kernel::Line_3;
  using Ray = typename Kernel::Ray_3;
  using Seg = typename Kernel::Segment_3;

  c.def(bp::init<Pnt&, Pnt&>())
    .def(bp::init<Line>())
    .def(bp::init<Ray>())
    .def(bp::init<Seg>())
    .def(bp::init<FT&, FT&, FT&, FT&>())
    .def(bp::init<FT&, FT&, FT&>())
    .def(bp::init<Pnt&, Pnt&>())
    .def("assign", py::overload_cast<const Vec&>(&Vec::operator=))
    .def("hx", &Vec::hx)
    .def("hy", &Vec::hy)
    .def("hz", &Vec::hz)
    .def("hw", &Vec::hw)
    .def("x", &Vec::x)
    .def("y", &Vec::y)
    .def("z", &Vec::z)
    .def("squared_length", &Vec::squared_length)
    .def("homogeneous", &Vec::homogeneous)
    .def("cartesian", &Vec::cartesian)
    .def("__getitem__", &Vec::operator[])
    .def("dimension", &Vec::dimension)
    .def("direction", &Vec::direction)
    .def("transform", &Vec::transform)
    .def(bp::self == bp::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(bp::self != bp::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(-bp::self)
    .def(bp::self * bp::self)
    .def(bp::self * FT())
    .def(FT() * bp::self)
    .def(bp::self *= FT())
    .def(bp::self / FT())
    .def(bp::self /= FT())
    //.def(py::self * RT())
    //.def(RT() * bp::self)
    //.def(bp::self *= RT())
    //.def(bp::self / RT())
    //.def(bp::self /= RT())
    //.setattr("__hash__", &hash<Vec>)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
}

#endif
