// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/multiprecision/gmp.hpp>

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

template <typename PyClass>
void export_mpz_int(PyClass& cls) {
  using mpz_int = boost::multiprecision::mpz_int;
  if (! add_attr<mpz_int>(cls, "mpz_int")) {
    py::class_<mpz_int> mpz_int_c(cls, "mpz_int");
    mpz_int_c.def(py::init<const mpz_int&>())
      .def(py::init_implicit<int>())
      .def(py::init_implicit<double>())
      .def(py::self == py::self,
           py::sig("def __eq__(self, arg: object, /) -> bool"))
      .def(py::self != py::self,
           py::sig("def __ne__(self, arg: object, /) -> bool"))
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
      .def_cast(int() + py::self)
      .def_cast(int() - py::self)
      .def_cast(py::self * int())
      .def_cast(py::self + int())
      .def_cast(py::self - int())
       ;

    add_insertion(mpz_int_c, "__str__");
    add_insertion(mpz_int_c, "__repr__");
  }
}
