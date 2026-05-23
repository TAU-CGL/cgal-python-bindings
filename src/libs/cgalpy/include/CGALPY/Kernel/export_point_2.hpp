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

#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/config.hpp"
#include "CGALPY/kernel_config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/make_iterator.hpp"

#include "cgalpy/Kernel_23_docstrings.hpp"

namespace py = nanobind;

//
template <typename Kernel_>
py::object cartesians_p2(const typename Kernel_::Point_2& p)
{ return make_iterator(p.cartesian_begin(), p.cartesian_end()); }

// Export a two-dimensional point of a kernel.
template <typename Kernel_, typename C>
void export_point_2(C& c) {
  namespace doc = cgalpy::docstrings::Kernel_23;
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
    c.def(py::init<const Ft&, const Ft&>(), doc::Point_2_Point_2_2)
      .def(py::init<const Rt&, const Rt&, const Rt&>(), doc::Point_2_Point_2_3);
  }

  c.def(py::init<>(), doc::Point_2_Point_2_4)
    .def(py::init<Pnt&>(), doc::Point_2_Point_2_5)
    .def(py::init_implicit<const CGAL::Origin&>())
    .def("x", &Pnt::x, doc::Point_2_x)    .def("y", &Pnt::y, doc::Point_2_y)    .def("hx", &Pnt::hx, doc::Point_2_hx)    .def("hy", &Pnt::hy, doc::Point_2_hy)    .def("hw", &Pnt::hw, doc::Point_2_hw)    .def("bbox", &Pnt::bbox, doc::Point_2_bbox)    .def("cartesian", &Pnt::cartesian, doc::Point_2_cartesian)    .def("__getitem__", &Pnt::operator[], doc::Point_2_operator)    .def("dimension", &Pnt::dimension, doc::Point_2_dimension)    .def(py::self == py::self, py::sig("def __eq__(self, arg: object, /) -> bool"))
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
