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

#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/config.hpp"
#include "CGALPY/make_iterator.hpp"

namespace bp = nanobind;

//
template <typename Kernel_>
py::object cartesians_v3(const typename Kernel_::Vector_3& v)
{ return make_iterator(v.cartesian_begin(), v.cartesian_end()); }

// Export a two-dimensional vector of a kernel.
template <typename Kernel_, typename C>
void export_vector_3(C& c) {
  using Ker = Kernel_;
  using Ft = typename Ker::FT;
  using Rt = typename Ker::RT;
  using Pnt = typename Ker::Point_3;
  using Vec = typename Ker::Vector_3;
  using Line = typename Ker::Line_3;
  using Ray = typename Ker::Ray_3;
  using Seg = typename Ker::Segment_3;

  c.def(bp::init<Pnt&, Pnt&>())
    .def(bp::init<Line>())
    .def(bp::init<Ray>())
    .def(bp::init<Seg>())
    .def(bp::init<Ft&, Ft&, Ft&, Ft&>())
    .def(bp::init<Ft&, Ft&, Ft&>())
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
    .def(bp::self == bp::self, py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(bp::self != bp::self, py::sig("def __ne__(self, arg: object, /) -> bool"))
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(-bp::self)
    .def(bp::self * bp::self)
    .def(bp::self * Ft())
    .def(Ft() * bp::self)
    .def(bp::self *= Ft())
    .def(bp::self / Ft())
    .def(bp::self /= Ft())
    //.def(py::self * Rt())
    //.def(Rt() * bp::self)
    //.def(bp::self *= Rt())
    //.def(bp::self / Rt())
    //.def(bp::self /= Rt())
    //.setattr("__hash__", &hash<Vec>)
    ;

  c.def("cartesians", &cartesians_v3<Ker>, py::keep_alive<0, 1>());

  using Cci = typename Ker::Cartesian_const_iterator_3;

#if (CGALPY_KERNEL != CGALPY_KERNEL_EPEC)
  add_iterator<Cci, Cci, const Ft&>("Cartesian_iterator", c);
#else
  add_iterator<Cci, Cci, Ft>("Cartesian_iterator", c);
#endif

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
