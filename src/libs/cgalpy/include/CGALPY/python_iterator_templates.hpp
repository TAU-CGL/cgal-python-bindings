// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_PYTHON_ITERATOR_TEMPLATES
#define CGALPY_PYTHON_ITERATOR_TEMPLATES

#include <nanobind/nanobind.h>

#define CGALPY_DEPRECATED_HEADER "CGALPY/python_iterator_templates.hpp"
#define CGALPY_REPLACEMENT_HEADER "CGALPY/make_iterator.hpp"
#define CGALPY_INTERNAL_DEPRECATED_MESSAGE_STATUS "Warning: "
#define CGALPY_INTERNAL_DEPRECATED_MESSAGE_DEPRECATED_HEADER \
  "The header `" CGALPY_DEPRECATED_HEADER "` is deprecated"
#define CGALPY_INTERNAL_NO_DEPRECATED_CODE_MESSAGE "."
#define CGALPY_INTERNAL_DEPRECATED_MESSAGE_HEADERS \
  CGALPY_INTERNAL_DEPRECATED_MESSAGE_DEPRECATED_HEADER \
  CGALPY_INTERNAL_NO_DEPRECATED_CODE_MESSAGE \
  " Please use `" CGALPY_REPLACEMENT_HEADER "` instead. "

#define CGALPY_INTERNAL_DEPRECATED_MESSAGE \
  CGALPY_INTERNAL_DEPRECATED_MESSAGE_STATUS \
  CGALPY_INTERNAL_DEPRECATED_MESSAGE_HEADERS

#  if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__DMC__)
#    pragma message (CGALPY_INTERNAL_DEPRECATED_MESSAGE)
#  elif (__GNUC__ > 0) || defined(__HP_aCC) || defined(__SUNPRO_CC) || defined(__IBMCPP__)
     // warning does not expand its arguments, issue a warning and add the message.
#    warning "A deprecated header has been included."
#    pragma message (CGALPY_INTERNAL_DEPRECATED_MESSAGE)
#  endif //defined


namespace py = nanobind;

inline py::object pass_through(py::object const& o) { return o; }

// These template classes are used to allow more natural iteration in Python

template <typename circulator>
class Iterator_from_circulator {
private:
  bool first = true;
  circulator m_first;
  circulator m_curr;

public:
  Iterator_from_circulator(circulator first) : m_first(first), m_curr(first) {}

  typename circulator::value_type& next() {
    if (m_curr != 0) {
      if (first || m_curr != m_first) {
        first = false;
        return *m_curr++;
      }
    }
    throw py::stop_iteration();
    return *m_curr;
  }
};

template <typename circulator>
class Iterator_from_const_circulator {
private:
  bool first = true;
  circulator m_first;
  circulator m_curr;

public:
  Iterator_from_const_circulator(circulator first) : m_first(first), m_curr(first) {}

  const typename circulator::value_type& next() {
    if (m_curr != 0) {
      if (first || m_curr != m_first) {
        first = false;
        return *m_curr++;
      }
    }
    throw py::stop_iteration();
    return *m_curr;
  }
};

template <typename iterator>
class Iterator_of_circulators {
  typedef Iterator_from_circulator<typename iterator::value_type> modified_circulator;
  iterator m_curr;
  iterator m_end;

public:
  Iterator_of_circulators(iterator begin, iterator end) :
    m_curr(begin),
    m_end(end)
  {}

  modified_circulator* next() {
    if (m_curr != m_end)
      return new modified_circulator(modified_circulator(*(m_curr++)));
    throw py::stop_iteration();
    return new modified_circulator(modified_circulator(*m_curr));
  }
};

template<typename iterator, typename Parent>
void bind_iterator_of_circulators(Parent& parent, const char* python_name) {
  py::class_<iterator>(parent, python_name)
    .def("__iter__", &pass_through)
    .def("__next__", &iterator::next, py::rv_policy::reference_internal)
    ;
}

template<typename iterator, typename Parent>
void bind_iterator(Parent& parent, const char* python_name) {
  py::class_<iterator>(parent, python_name)
    .def("__iter__", &pass_through)
    .def("__next__", &iterator::next)
    ;
}


// For iterators that don't dereference to a reference of an existing object
template <typename iterator>
class Copy_iterator {
private:
  iterator m_curr;
  iterator m_end;
public:
  Copy_iterator(iterator begin, iterator end) : m_curr(begin), m_end(end) {}
  typename iterator::value_type next() {
    if (m_curr != m_end) return *m_curr++;
    throw py::stop_iteration();
    return *m_curr;
  }
};

template <typename circulator>
class Copy_iterator_from_circulator {
private:
  bool first = true;
  circulator m_first;
  circulator m_curr;

public:
  Copy_iterator_from_circulator(circulator first) :
    m_first(first),
    m_curr(first)
  {}

  typename circulator::value_type next() {
    if (m_curr != 0) {
      if (first || m_curr != m_first) {
        first = false;
        return *(m_curr++);
      }
    }
    throw py::stop_iteration();
    return *m_curr;
  }
};

template<typename iterator>
void bind_copy_iterator(py::module_& m, const char* python_name) {
  py::class_<iterator>(m, python_name)
    .def("__iter__", &pass_through)
    .def("__next__", &iterator::next)
    ;
}
#endif
