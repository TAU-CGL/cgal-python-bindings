#ifndef CGALPY_EXPORT_BOOST_MULTIPRECISION_HPP
#define CGALPY_EXPORT_BOOST_MULTIPRECISION_HPP

// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <string>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/add_insertion.hpp"

namespace py = nanobind;
namespace bm = boost::multiprecision;

//!
template <typename C>
void export_boost_multiprecision_int(C& int_c) {
  using Int_type = typename C::Type;
  int_c.def(py::init<const Int_type&>())
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

  add_insertion(int_c, "__str__");
  add_insertion(int_c, "__repr__");
}

//!
template <typename C>
void export_boost_multiprecision_rational(C& rat_c) {
  using Rat_type = typename C::Type;
  using Int_type = std::decay_t<decltype(bm::numerator(std::declval<Rat_type>()))>;

  // Observe that the wrapping of the attributes numerator() and denominator() is wrapped using member-like access
  rat_c.def(py::init<const Rat_type>())
    .def(py::init_implicit<Int_type>())
    .def(py::init<const Int_type&, const Int_type&>())
    .def(py::init_implicit<double>())
    .def(py::init<const std::string&>())
    .def("numerator", [](const Rat_type r) { return bm::numerator(r); })
    .def("denominator", [](const Rat_type r) { return bm::denominator(r); })
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

  add_insertion(rat_c, "__str__");
  add_insertion(rat_c, "__repr__");
}

#endif
