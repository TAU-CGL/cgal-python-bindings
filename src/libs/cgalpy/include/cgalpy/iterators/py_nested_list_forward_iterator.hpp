// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_ITERATORS_PY_NESTED_LIST_FORWARD_ITERATOR_HPP
#define CGALPY_ITERATORS_PY_NESTED_LIST_FORWARD_ITERATOR_HPP

#include <boost/iterator/iterator_facade.hpp>

#include <nanobind/nanobind.h>

#include "cgalpy/py_list_range.hpp"

namespace py = nanobind;

template <typename T>
struct py_nested_list_forward_iterator :
  boost::iterator_facade<py_nested_list_forward_iterator<T>, py_list_range<T>, std::forward_iterator_tag, py_list_range<T>> {

  py_nested_list_forward_iterator() : m_it(py::list().end()) {}

  py_nested_list_forward_iterator(const py::list& lst, bool isbegin = true) : m_it((isbegin) ? lst.begin() : lst.end()) {}

  void increment() { ++m_it; }

  // Dereference returns our lightweight range view, avoiding temporary C++ containers
  py_list_range<T> dereference() const { return py_list_range<T>(py::cast<py::list>(*m_it)); }

  bool equal(py_nested_list_forward_iterator<T> const& o) const { return m_it == o.m_it; }

private:
  py::detail::fast_iterator m_it;
};

#endif
