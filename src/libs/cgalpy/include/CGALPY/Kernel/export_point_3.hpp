// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_POINT_3_HPP
#define CGALPY_EXPORT_POINT_3_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/config.hpp"
#include "CGALPY/Hash_rational_point.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

//
template <typename Kernel_>
py::object cartesians_p3(const typename Kernel_::Point_3& p)
{ return make_iterator(p.cartesian_begin(), p.cartesian_end()); }

// Export a two-dimensional point of a kernel.
template <typename Kernel_, typename C>
void export_point_3(C& c) {
  using Ker = Kernel_;
  using Ft = typename Ker::FT;
  using Rt = typename Ker::RT;
  using Pnt = typename Ker::Point_3;
  using Vec = typename Ker::Vector_3;

  c.def(py::init<>())
    .def(py::init<const Ft&, const Ft&, const Ft&>())
    .def(py::init<const Rt&, const Rt&, const Rt&, const Rt&>())
    .def(py::init<const Pnt&>())
    .def(py::init_implicit<const CGAL::Origin&>())
    .def("assign", py::overload_cast<const Pnt&>(&Pnt::operator=))
    .def("x", &Pnt::x, doc::Point_3_x)    .def("y", &Pnt::y, doc::Point_3_y)    .def("z", &Pnt::z, doc::Point_3_z)    .def("hx", &Pnt::hx, doc::Point_3_hx)    .def("hy", &Pnt::hy, doc::Point_3_hy)    .def("hz", &Pnt::hz, doc::Point_3_hz)    .def("hw", &Pnt::hw, doc::Point_3_hw)    .def("bbox", &Pnt::bbox, doc::Point_3_bbox)    .def("cartesian", &Pnt::cartesian, doc::Point_3_cartesian)    .def("__getitem__", &Pnt::operator[], doc::Point_3_operator)    .def("dimension", &Pnt::dimension, doc::Point_3_dimension)    .def(py::self == py::self, py::sig("def __eq__(self, arg: object, /) -> bool"))
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
    // .setattr("__doc__", "Point_3") NB
    ;

  c.def("cartesians", &cartesians_p3<Ker>, py::keep_alive<0, 1>());

  using Cci = typename Ker::Cartesian_const_iterator_3;

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
