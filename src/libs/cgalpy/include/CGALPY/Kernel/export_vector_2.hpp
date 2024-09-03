// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_VECTOR_2_HPP
#define CGALPY_EXPORT_VECTOR_2_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/config.hpp"
#include "CGALPY/kernel_config.hpp"
#include "CGALPY/kernel_type.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/make_iterator.hpp"

namespace bp = nanobind;

//
template <typename Kernel_>
py::object cartesians_v2(const typename Kernel_::Vector_2& v)
{ return make_iterator(v.cartesian_begin(), v.cartesian_end()); }

// Export a two-dimensional vector of a kernel.
template <typename Kernel_, typename C>
void export_vector_2(C& c) {
  using Ker = Kernel_;
  using Ft = typename Ker::FT;
  using Rt = typename Ker::RT;
  using Pnt = typename Ker::Point_2;
  using Vec = typename Ker::Vector_2;
  using Line = typename Ker::Line_2;
  using Ray = typename Ker::Ray_2;
  using Seg = typename Ker::Segment_2;

  c.def(bp::init<const Rt&, const Rt&, const Rt&>())
    .def(bp::init<const Ft&, const Ft&>())
    .def(bp::init<Pnt&, Pnt&>())
    .def(bp::init<Line>())
    .def(bp::init<Ray>())
    .def(bp::init<Seg>())
    .def("hx", &Vec::hx)
    .def("hy", &Vec::hy)
    .def("hw", &Vec::hw)
    .def("x", &Vec::x)
    .def("y", &Vec::y)
    .def("squared_length", &Vec::squared_length)
    .def("homogeneous", &Vec::homogeneous)
    .def("cartesian", &Vec::cartesian)
    .def("__getitem__", &Vec::operator[])
    .def("dimension", &Vec::dimension)
    .def("direction", &Vec::direction)
    .def("transform", &Vec::transform)
    .def("perpendicular", &Vec::perpendicular)
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

  if (! is_exact_ft()) {
    c.def("cartesians", &cartesians_v2<Ker>, py::keep_alive<0, 1>());

    using Cci = typename Ker::Cartesian_const_iterator_2;
    add_iterator<Cci, Cci>("Cartesian_iterator", c);
  }

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
