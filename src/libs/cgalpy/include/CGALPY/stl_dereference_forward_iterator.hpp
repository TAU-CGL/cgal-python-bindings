// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_STL_DEREFERENCE_FORWARD_ITERATOR_HPP
#define CGALPY_STL_DEREFERENCE_FORWARD_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T, typename ValueType = decltype(*std::declval<T>())>
struct stl_dereference_forward_iterator :
  boost::iterator_facade<stl_dereference_forward_iterator<T>, T, std::forward_iterator_tag, T> {

  // Default constructor.
  // Workaround the lack of default constructor for py::detail::fast_iterator.
  // stl_dereference_forward_iterator() {}
  stl_dereference_forward_iterator() : m_it(py::list().end()) {}

  stl_dereference_forward_iterator(const py::list& lst, bool isbegin = true) :
    m_it((isbegin) ? lst.begin() : lst.end())
  {}

  void increment() { ++m_it; }

  T dereference() const { return T(&(py::cast<ValueType>(*m_it))); }

  bool equal(stl_dereference_forward_iterator<T> const& o) const { return m_it == o.m_it; }

private:
  py::detail::fast_iterator m_it;
};

#endif
