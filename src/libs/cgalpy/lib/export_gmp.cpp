// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>

namespace py = nanobind;

void export_gmpz() {
  py::class_<CGAL::Gmpz>("Gmpz")
    .def(py::init<int>())
    .def(py::init<CGAL::Gmpz&>())
    .def("to_double", &CGAL::Gmpz::to_double)
    .def(py::self_ns::str(py::self_ns::self))
    .def(py::self_ns::repr(py::self_ns::self))
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self * py::self)
    .def(py::self *= py::self)
    .def(py::self / py::self)
    .def(py::self /= py::self)
    ;
}

void export_gmpq() {
  py::class_<CGAL::Gmpq>("Gmpq")
    .def(py::init<CGAL::Gmpz, CGAL::Gmpz>())
    .def(py::init<unsigned long, unsigned long>())
    .def(py::init<const std::string&>())
    .def(py::init<CGAL::Gmpq&>())
    .def(py::init<double>())
    .def("to_double", &CGAL::Gmpq::to_double)
    .def("numerator", &CGAL::Gmpq::numerator)
    .def("denominator", &CGAL::Gmpq::denominator)
    .def("size", &CGAL::Gmpq::size)
    .def(py::self_ns::str(py::self_ns::self))
    .def(py::self_ns::repr(py::self_ns::self))
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self * py::self)
    .def(py::self *= py::self)
    .def(py::self / py::self)
    .def(py::self /= py::self)
    .def(-py::self)
    ;
}
