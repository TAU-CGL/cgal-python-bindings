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

namespace py = nanobind;

// namespace nanobind {
// namespace detail {

// //
// template <typename Access,
//           rv_policy Policy,
//           typename Iterator,
//           typename Sentinel,
//           typename ValueType,
//           typename... Extra>
// struct iterator_state {
//   Iterator it;
//   Sentinel end;
//   bool first_or_done;
// };

// // Note: these helpers take the iterator by non-const reference because some
// // iterators in the wild can't be dereferenced when const. The & after Iterator
// // is required for MSVC < 16.9. SFINAE cannot be reused for result_type due to
// // bugs in ICC, NVCC, and PGI compilers. See PR #3293.
// template <typename Iterator, typename SFINAE =
//           decltype(*std::declval<Iterator&>())>
// struct iterator_access {
//   using result_type = decltype(*std::declval<Iterator&>());
//   // NOLINTNEXTLINE(readability-const-return-type) // PR #3263
//   result_type operator()(Iterator& it) const { return *it; }
// };

// //
// template <typename Access,
//           typename C,
//           rv_policy Policy,
//           typename Iterator,
//           typename Sentinel,
//           typename ValueType,
//           typename... Extra>
// object make_iterator_impl(C& c, Iterator&& first, Sentinel&& last,
//                           Extra&&... extra)
// {
//   using state =
//     iterator_state<Access, Policy, Iterator, Sentinel, ValueType, Extra...>;
//   // TODO: state captures only the types of Extra, not the values

//   const py::handle info = py::type<state>();
//   if (! info.is_valid() || ! type_check(info)) {
//     // class_<state>(handle(), "iterator", pybind11::module_local())
//     class_<state>(c, "iterator")
//       .def("__iter__", [](state& s) -> state & { return s; })
//       .def("__next__",
//            [](state& s) -> ValueType {
//              if (! s.first_or_done) ++s.it;
//              else s.first_or_done = false;
//              if (s.it == s.end) {
//                s.first_or_done = true;
//                throw stop_iteration();
//              }
//              return Access()(s.it);
//              // NOLINTNEXTLINE(readability-const-return-type) // PR #3263
//            },
//            std::forward<Extra>(extra)...,
//            Policy);
//   }

//   return cast(state{std::forward<Iterator>(first),
//                     std::forward<Sentinel>(last), true});
// }

// }

// /// Makes a python iterator from a first and past-the-end C++ InputIterator.
// template <rv_policy Policy = rv_policy::reference_internal,
//           typename C,
//           typename Iterator,
//           typename Sentinel,
//           typename ValueType =
//             typename detail::iterator_access<Iterator>::result_type,
//           typename... Extra>
// object make_iterator(C& c, Iterator&& first, Sentinel&& last, Extra&&... extra)
// {
//   return detail::make_iterator_impl<detail::iterator_access<Iterator>,
//                                     C,
//                                     Policy,
//                                     Iterator,
//                                     Sentinel,
//                                     ValueType,
//                                     Extra...>(c,
//                                               std::forward<Iterator>(first),
//                                               std::forward<Sentinel>(last),
//                                               std::forward<Extra>(extra)...);
// }

// }

//
template <typename Iterator, typename Sentinel>
struct iterator_state {
  Iterator it;
  Sentinel end;
  bool first_or_done;
};

template <py::rv_policy Policy,
          typename Iterator, typename Sentinel, typename ValueType,
          typename... Extra,
          typename C>
void add_iterator_impl(const char* name, C& c, Extra&&... extra) {
  using state = iterator_state<Iterator, Sentinel>;
  if (add_attr<state>(name, c)) return;

  py::class_<state>(c, name)
    .def("__iter__", [](state& s)->state& { return s; })
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
  add_iterator_impl<Policy,
                    Iterator, Sentinel, ValueType,
                    Extra...>(name, c, std::forward<Extra>(extra)...);
}

// Obtain a Python iterator
template <typename Iterator, typename Sentinel>
py::object make_iterator(Iterator begin, Sentinel end) {
  using state = iterator_state<Iterator, Sentinel>;
  return py::cast(state{begin, end, true});
}

#endif
