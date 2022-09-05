// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_FT_HPP
#define CGALPY_EXPORT_FT_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>

#include "CGALPY/to_string.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

//
template <typename C>
void export_ft(C& c) {
  using FT = typename C::Type;

  c.def(py::init<double>())
    .def(py::init<FT>())
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

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
}

#endif
