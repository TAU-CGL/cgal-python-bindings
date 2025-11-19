// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_RT_HPP
#define CGALPY_EXPORT_RT_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/to_string.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

//
template <typename C>
void export_rt(C& c) {
  using RT = typename C::Type;

  c.def(py::init<const RT&>())
    .def(py::init_implicit<double>())
    .def(py::self == py::self, py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self, py::sig("def __ne__(self, arg: object, /) -> bool"))
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self += py::self)
    .def(py::self -= py::self)
    .def(py::self *= py::self)
    .def(py::self /= py::self)
    .def(-py::self)
    .def_cast(py::self + py::self)
    .def_cast(py::self - py::self)
    .def_cast(py::self * py::self)
    .def_cast(py::self / py::self)
    .def_cast(int() * py::self)
    .def_cast(float() * py::self)
    .def_cast(int() + py::self)
    .def_cast(float() + py::self)
    .def_cast(int() - py::self)
    .def_cast(float() - py::self)
    .def_cast(py::self * int())
    .def_cast(py::self * float())
    .def_cast(py::self + int())
    .def_cast(py::self + float())
    .def_cast(py::self - int())
    .def_cast(py::self - float())
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
