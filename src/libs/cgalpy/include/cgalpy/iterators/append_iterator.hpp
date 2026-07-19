// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria   <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_ITERATORS_APPEND_ITERATOR_HPP
#define CGALPY_ITERATORS_APPEND_ITERATOR_HPP

#include <iterator>

#include <nanobind/nanobind.h>

namespace py = nanobind;

// append_iterator is similar to back_insert_iterator.
// Instead of applying 'container.push_back(element)' it applies
// lst.append(element), where lst is the bound Python list.
//
// T defines value_type for algorithms and iterator-traits queries. It does not
// affect nanobind conversion or Python list append behavior. Use a concrete T
// when a producer constructs value_type objects; void supports consumers that
// only assign already constructed values.
template <typename T = void>
class append_iterator {
private:
  py::list& m_lst;

public:
  using iterator_category = std::output_iterator_tag;
  using value_type = T;
  using difference_type = void;
  using pointer = void;
  using reference = void;

  append_iterator(py::list& lst) : m_lst(lst) {}

  template <typename Value>
  const Value& operator=(const Value& value) const {
    m_lst.append(value);
    return value;
  }

  // Make sure the assignment operator is available
  append_iterator& operator=(const append_iterator& other) {
    m_lst = other.m_lst;
    return *this;
  }

  append_iterator& operator*() { return *this; }
  append_iterator& operator++() { return *this; }
  append_iterator operator++(int) { return *this; }
};

append_iterator(py::list&) -> append_iterator<void>;

#endif
