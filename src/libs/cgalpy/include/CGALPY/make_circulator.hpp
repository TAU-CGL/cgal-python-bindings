// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_MAKE_CIRCULATOR_HPP
#define CGALPY_MAKE_CIRCULATOR_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/circulator_state.hpp"

namespace py = nanobind;

//
template <py::rv_policy Policy,
          typename Iterator, typename ValueType,
          typename... Extra,
          typename C>
void add_circulator_impl(const char* name, C& c, Extra&&... extra) {
  using state = circulator_state<Iterator>;
  if (add_attr<state>(c, name)) return;

  constexpr auto ri(py::rv_policy::reference_internal);
  py::class_<state>(c, name)
    .def("__iter__", [](state& s) -> state& { return s; }, ri)
    .def("__next__", [](state& s) -> ValueType { return *s.it++; },
      std::forward<Extra>(extra)..., Policy)
    ;
}

// Add (wrap) a circulator
template <typename Iterator,
          typename ValueType = decltype(*std::declval<Iterator>()),
          py::rv_policy Policy = py::rv_policy::reference_internal,
          typename... Extra,
          typename C>
void add_circulator(const char* name, C& c, Extra&&... extra) {
  add_circulator_impl<Policy, Iterator, ValueType,
                      Extra...>(name, c, std::forward<Extra>(extra)...);
}

// There are cases (actually so far only one), where the value type of the
// circulator is a handle; in such cases we need to return the dereference
// of the handle; thus the double application of the dereference operator.
template <py::rv_policy Policy,
          typename Iterator, typename ValueType,
          typename... Extra,
          typename C>
void add_dereference_circulator_impl(const char* name, C& c, Extra&&... extra) {
  using state = circulator_state<Iterator>;
  if (add_attr<state>(c, name)) return;

  constexpr auto ri(py::rv_policy::reference_internal);
  py::class_<state>(c, name)
    .def("__iter__", [](state& s) -> state& { return s; }, ri)
    .def("__next__", [](state& s) -> ValueType { return **s.it++; },
      std::forward<Extra>(extra)..., Policy)
    ;
}

// Add (wrap) a circulator
template <typename Iterator,
          typename ValueType = decltype(**std::declval<Iterator>()),
          py::rv_policy Policy = py::rv_policy::reference_internal,
          typename... Extra,
          typename C>
void add_dereference_circulator(const char* name, C& c, Extra&&... extra) {
  add_dereference_circulator_impl<Policy, Iterator, ValueType,
                                  Extra...>(name, c, std::forward<Extra>(extra)...);
}

// Obtain a Python circulator
template <typename Iterator>
py::object make_circulator(Iterator begin) {
  using state = circulator_state<Iterator>;
  return py::cast(state{begin, true, false});
}

#endif
