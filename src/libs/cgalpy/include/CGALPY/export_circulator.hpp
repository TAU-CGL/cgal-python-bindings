// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_CIRCULATOR_HPP
#define CGALPY_EXPORT_CIRCULATOR_HPP

#include <nanobind/nanobind.h>

#include <CGAL/circulator.h>

#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

//!
template <typename Circulator,
          typename ValueType = decltype(*std::declval<Circulator>()),
          py::rv_policy Policy = py::rv_policy::reference_internal,
          typename... Extra,
          typename PyClass>
void export_circulator(PyClass& cls, const char* name) {
  if (! add_attr<Circulator>(cls, name)) {
    constexpr auto ri(py::rv_policy::reference_internal);
    py::class_<Circulator>(cls, name)
      .def("next", [](Circulator circ)->Circulator { return ++circ; })
      .def("prev", [](Circulator circ)->Circulator { return --circ; })
      .def("size", [](Circulator circ)->std::size_t { return CGAL::circulator_size(circ); })
      .def("value", [](Circulator circ)->ValueType { return *circ; }, ri)
      .def("__eq__", [](const Circulator a, const Circulator b) { return a == b; })
      ;
  }
}

//!
template <typename Circulator,
          typename ValueType = decltype(*std::declval<Circulator>()),
          py::rv_policy Policy = py::rv_policy::reference_internal,
          typename... Extra,
          typename PyClass>
void export_dereference_circulator(PyClass& cls, const char* name) {
  if (! add_attr<Circulator>(cls, name)) {
    constexpr auto ri(py::rv_policy::reference_internal);
    py::class_<Circulator>(cls, name)
      .def("next", [](Circulator circ)->Circulator { return ++circ; })
      .def("prev", [](Circulator circ)->Circulator { return --circ; })
      .def("size", [](Circulator circ)->std::size_t { return CGAL::circulator_size(circ); })
      .def("value", [](Circulator circ)->ValueType { return **circ; }, ri)
      .def("__eq__", [](const Circulator a, const Circulator b) { return a == b; })
      ;
  }
}

#endif
