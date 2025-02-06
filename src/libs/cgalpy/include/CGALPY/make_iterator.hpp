// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_MAKE_ITERATOR_HPP
#define CGALPY_MAKE_ITERATOR_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/iterator_state.hpp"
#include "CGALPY/circulator_state.hpp"

namespace py = nanobind;

/* The internal representation of an iterator wrapper.  the `__iter__` attribute
 * wraps a function that accepts an iterator state and simply returns it. We can
 * either accept the state by value (and return it) or accept by reference and
 * return the reference. If we return the reference, we must Python taking
 * ownership of it (to prevent Python destructing the state, even when the
 * Python wrapper is garbage collected.
 */
template <py::rv_policy Policy,
          typename Iterator, typename Sentinel, typename ValueType,
          typename... Extra,
          typename C>
void add_iterator_impl(const char* name, C& c, Extra&&... extra) {
  using state = iterator_state<Iterator, Sentinel>;
  if (add_attr<state>(c, name)) return;

  constexpr auto ri(py::rv_policy::reference_internal);
  py::class_<state>(c, name)
    .def("__iter__", [](state& s) -> state& { return s; }, ri)
    .def("__next__", [](state& s) -> ValueType {
                       if (! s.first_or_done) ++s.it;
                       else s.first_or_done = false;
                       if (s.it == s.end) {
                         s.first_or_done = true;
                         throw py::stop_iteration();
                       }
                       return *s.it;
                     },
      std::forward<Extra>(extra)..., Policy)
    ;
}

// Add (wrap) an iterator
template <typename Iterator, typename Sentinel,
          typename ValueType = decltype(*std::declval<Iterator>()),
          py::rv_policy Policy = py::rv_policy::reference_internal,
          typename... Extra,
          typename C>
void add_iterator(const char* name, C& c, Extra&&... extra) {
  add_iterator_impl<Policy, Iterator, Sentinel, ValueType,
                    Extra...>(name, c, std::forward<Extra>(extra)...);
}

// There are cases (actually so far only one), where the value type of the
// circulator is a handle; in such cases we need to return the dereference
// of the handle; thus the double application of the dereference operator.
template <py::rv_policy Policy,
          typename Iterator, typename Sentinel, typename ValueType,
          typename... Extra,
          typename C>
void add_dereference_iterator_impl(const char* name, C& c, Extra&&... extra) {
  using state = iterator_state<Iterator, Sentinel>;
  if (add_attr<state>(c, name)) return;

  constexpr auto ri(py::rv_policy::reference_internal);
  py::class_<state>(c, name)
    .def("__iter__", [](state& s) -> state& { return s; }, ri)
    .def("__next__", [](state& s) -> ValueType {
                       if (! s.first_or_done) ++s.it;
                       else s.first_or_done = false;
                       if (s.it == s.end) {
                         s.first_or_done = true;
                         throw py::stop_iteration();
                       }
                       return **s.it;
                     },
      std::forward<Extra>(extra)..., Policy)
    ;
}

// Add (wrap) an iterator
template <typename Iterator, typename Sentinel,
          typename ValueType = decltype(*std::declval<Iterator>()),
          py::rv_policy Policy = py::rv_policy::reference_internal,
          typename... Extra,
          typename C>
void add_dereference_iterator(const char* name, C& c, Extra&&... extra) {
  add_dereference_iterator_impl<Policy, Iterator, Sentinel, ValueType,
                                Extra...>(name, c, std::forward<Extra>(extra)...);
}

//
template <py::rv_policy Policy,
          typename Iterator, typename Sentinel, typename ValueType,
          typename... Extra,
          typename C>
void add_iterator_of_circulator_impl(const char* name, C& c, Extra&&... extra) {
  using state = iterator_state<Iterator, Sentinel>;
  using sub_state = circulator_state<ValueType>;
  if (add_attr<state>(c, name)) return;

  constexpr auto ri(py::rv_policy::reference_internal);
  py::class_<state>(c, name)
    .def("__iter__", [](state& s) -> state& { return s; }, ri)
    .def("__next__", [](state& s) -> sub_state {
                       if (! s.first_or_done) ++s.it;
                       else s.first_or_done = false;
                       if (s.it == s.end) {
                         s.first_or_done = true;
                         throw py::stop_iteration();
                       }
                       return sub_state{*s.it, *s.it, true, false};
                     },
      std::forward<Extra>(extra)..., Policy)
    ;
}

// Add (wrap) an iterator
template <typename Iterator, typename Sentinel,
          typename ValueType = decltype(*std::declval<Iterator>()),
          py::rv_policy Policy = py::rv_policy::reference_internal,
          typename... Extra,
          typename C>
void add_iterator_of_circulator(const char* name, C& c, Extra&&... extra) {
  add_iterator_of_circulator_impl<Policy, Iterator, Sentinel, ValueType,
                                  Extra...>(name, c, std::forward<Extra>(extra)...);
}

// Obtain a Python iterator
template <typename Iterator, typename Sentinel>
py::object make_iterator(Iterator begin, Sentinel end) {
  using state = iterator_state<Iterator, Sentinel>;
  return py::cast(state{begin, end, true});
}

#endif
