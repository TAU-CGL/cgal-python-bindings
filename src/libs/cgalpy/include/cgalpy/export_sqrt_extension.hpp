// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_SQRT_EXTENSION_HPP
#define CGALPY_EXPORT_SQRT_EXTENSION_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "cgalpy/add_insertion.hpp"
#include "cgalpy/add_extraction.hpp"

namespace py = nanobind;

//
template <typename C>
void export_sqrt_extension(C& c) {
  using Se = typename C::Type;
  using NT = typename Se::NT;
  using Root = typename Se::ROOT;

  constexpr auto ri(py::rv_policy::reference_internal);

  c.def(py::init<>())
    .def(py::init<const Se&>())
    .def(py::init_implicit<int>())
    .def(py::init<int, int, int>())
    .def(py::init_implicit<NT>())
    .def(py::init<NT, NT, Root>())
    .def(py::self == py::self,
         py::sig("def __eq__(self, arg: object, /) -> bool"))
    .def(py::self != py::self,
         py::sig("def __ne__(self, arg: object, /) -> bool"))
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
    .def(NT() * py::self)
    .def(NT() / py::self)
    .def(NT() + py::self)
    .def(NT() - py::self)
    .def(int() * py::self)
    .def(int() / py::self)
    .def(int() + py::self)
    .def(int() - py::self)
    .def("a0", py::overload_cast<>(&Se::a0, py::const_), ri)
    .def("a1", py::overload_cast<>(&Se::a1, py::const_), ri)
    .def("root", py::overload_cast<>(&Se::root, py::const_), ri)

    //! \todo fix the CGAL interface
    // .def("a0", py::overload_cast<>(&Se::is_extended, py::const_), ri)

    .def("is_extended", &Se::is_extended)
    .def("is_zero", &Se::is_zero)
    .def("simplify", &Se::simplify)
    .def("sign", &Se::sign)
    .def("abs", &Se::abs)
    .def("compare", py::overload_cast<const NT&>(&Se::compare, py::const_))
    .def("to_double", [](const Se& n)->double const { return CGAL::to_double(n); })
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
  add_extraction(c);
}

#endif
