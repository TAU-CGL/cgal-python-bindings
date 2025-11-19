// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <string>

#include <boost/multiprecision/gmp.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

//!
void export_mpz_int(py::module_& m) {
  using mpz_int = boost::multiprecision::mpz_int;
  if (! add_attr<mpz_int>(m, "mpz_int")) {
    py::class_<mpz_int> mpz_int_c(m, "mpz_int");
    mpz_int_c.def(py::init<const mpz_int&>())
      .def(py::init_implicit<int>())
      .def(py::init_implicit<double>())
      .def(py::init<const std::string&>())
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

//!
void export_mpq_rational(py::module_& m) {
  using mpz_int = boost::multiprecision::mpz_int;
  using mpq_rat = boost::multiprecision::mpq_rational;

  if (! add_attr<mpq_rat>(m, "mpq_rational")) {
    // Observe that the wrapping of the attributes numerator() and denominator() is wrapped using member-like access
    py::class_<mpq_rat> mpq_rat_c(m, "mpq_rational");
    mpq_rat_c.def(py::init<const mpq_rat&>())
      .def(py::init_implicit<mpz_int>())
      .def(py::init<const mpz_int&, const mpz_int&>())
      .def(py::init_implicit<double>())
      .def(py::init<const std::string&>())
      .def("numerator", [](const mpq_rat& r) { return boost::multiprecision::numerator(r); })
      .def("denominator", [](const mpq_rat& r) { return boost::multiprecision::denominator(r); })
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

    add_insertion(mpq_rat_c, "__str__");
    add_insertion(mpq_rat_c, "__repr__");
  }
}
