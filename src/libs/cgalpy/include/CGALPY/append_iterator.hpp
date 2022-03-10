// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_APPEND_ITTERATOR_HPP
#define CGALPY_APPEND_ITTERATOR_HPP

#include <boost/python.hpp>

namespace bp = boost::python;

// append_iterator is similar to back_insert_iterator.
// Instead of applying 'container.push_back(element)' it applies
// lst.append(element), where lst is the bound Python list.
class append_iterator {
private:
  bp::list& m_lst;

public:
  append_iterator(bp::list& lst) : m_lst(lst) {}

  template <typename T>
  const T& operator=(const T& t) const { m_lst.append(t); return t; }

  // Make sure the assignment operator is available
  append_iterator& operator=(const append_iterator& other) {
    m_lst = other.m_lst;
    return *this;
  }

  append_iterator& operator*() { return *this; }
  append_iterator& operator++() { return *this; }
  append_iterator operator++(int) { return *this; }
};

#endif
