// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_APPLY_ITTERATOR_HPP
#define CGALPY_APPLY_ITTERATOR_HPP

#include <boost/python.hpp>

namespace bp = boost::python;

// append_iterator is similar to back_insert_iterator.
// Instead of applying 'container.push_back(element)' it applies
// lst.append(element), where lst is the bound Python list.
template <typename UnaryOperation>
class apply_iterator {
private:
  UnaryOperation m_op;

public:
  apply_iterator(UnaryOperation op) : m_op(op) {}

  // Make sure the assignment operator is available
  apply_iterator& operator=(const apply_iterator& other) {
    m_op = other.m_op;
    return *this;
  }

  template <typename T>
  const T& operator=(const T& t) const { m_op(t); return t; }

  apply_iterator& operator*() { return *this; }
  apply_iterator& operator++() { return *this; }
  apply_iterator operator++(int) { return *this; }
};

#endif
