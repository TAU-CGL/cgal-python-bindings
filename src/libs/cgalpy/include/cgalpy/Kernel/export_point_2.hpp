// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_POINT_2_HPP
#define CGALPY_EXPORT_POINT_2_HPP

#include <type_traits>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "cgalpy/add_extraction.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/config.hpp"
#include "cgalpy/kernel_config.hpp"
#include "cgalpy/Hash_rational_point.hpp"
#include "cgalpy/make_iterator.hpp"

namespace py = nanobind;

//
template <typename Kernel_>
py::object cartesians_p2(const typename Kernel_::Point_2& p)
{ return make_iterator(p.cartesian_begin(), p.cartesian_end()); }

// Export a two-dimensional point of a kernel.
template <typename Kernel_, typename C>
void export_point_2(C& c) {
  using Ker = Kernel_;
  using Ft = typename Ker::FT;
  using Rt = typename Ker::RT;
  using Pnt = typename Ker::Point_2;
  using Vec = typename Ker::Vector_2;

  if constexpr (is_exact_ft()) {
    c.def(py::init<const Ft&, const Ft&>(),
          py::sig("def __init__(self, x: FT | float, y: FT | float, /) -> None"))
      .def(py::init<const Rt&, const Rt&, const Rt&>(),
           py::sig("def __init__(self, x: RT | float, y: RT | float, w: RT | float, /) -> None"));
  }
  else {
    c.def(py::init<const Ft&, const Ft&>())
      .def(py::init<const Rt&, const Rt&, const Rt&>());
  }

  c.def(py::init<>())
    .def(py::init<Pnt&>())
    .def(py::init_implicit<const CGAL::Origin&>())
    .def("assign", static_cast<Pnt& (Pnt::*)(const Pnt&)>(&Pnt::operator=))
    .def("x", &Pnt::x)
    .def("y", &Pnt::y)
    .def("hx", &Pnt::hx)
    .def("hy", &Pnt::hy)
    .def("hw", &Pnt::hw)
    .def("bbox", &Pnt::bbox)
    .def("cartesian", &Pnt::cartesian)
    .def("__getitem__", &Pnt::operator[])
    .def("dimension", &Pnt::dimension)
    .def(py::self == py::self, py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self, py::sig("def __ne__(self, arg: object, /) -> bool"))
    .def(py::self > py::self)
    .def(py::self < py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self - py::self)
    .def(py::self += Vec())
    .def(py::self -= Vec())
    .def(py::self + Vec())
    .def(py::self - Vec())
    .def("__hash__", &hash_rational_point<is_exact_ft(), Pnt>)
    // .setattr("__doc__", "Point_2") NB
    ;

  c.def("cartesians", &cartesians_p2<Ker>, py::keep_alive<0, 1>());

  using Cci = typename Ker::Cartesian_const_iterator_2;

  // There might be a better (automatic) way to handle this instead of a hard-coded '#if'...
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
