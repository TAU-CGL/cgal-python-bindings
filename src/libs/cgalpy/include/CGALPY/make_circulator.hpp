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

namespace py = nanobind;

//
template <typename Iterator>
struct circulator_state {
  Iterator it;
  Iterator end;
  bool first;
  bool done;
};

template <py::rv_policy Policy,
          typename Iterator, typename ValueType,
          typename... Extra,
          typename C>
void add_circulator_impl(const char* name, C& c, Extra&&... extra) {
  using state = circulator_state<Iterator>;
  if (add_attr<state>(c, name)) return;

  py::class_<state>(c, name)
    .def("__iter__", [](state& s) -> state& { return s; })
    .def("__next__", [](state& s) -> ValueType {
                       if (s.first) {
                         s.first = false;
                         return *s.it++;
                       }
                       if (s.it == s.end) s.done = true;
                       if (s.done) throw py::stop_iteration();
                       return *s.it++;
                     },
      std::forward<Extra>(extra)..., Policy)
    ;
}

// Add (wrap) an circulator
template <typename Iterator,
          typename ValueType = decltype(*std::declval<Iterator>()),
          py::rv_policy Policy = py::rv_policy::reference_internal,
          typename... Extra,
          typename C>
void add_circulator(const char* name, C& c, Extra&&... extra) {
  add_circulator_impl<Policy, Iterator, ValueType,
                      Extra...>(name, c, std::forward<Extra>(extra)...);
}

// Obtain a Python circulator
template <typename Iterator>
py::object make_circulator(Iterator begin) {
  using state = circulator_state<Iterator>;
  return py::cast(state{begin, begin, true, false});
}

#endif
