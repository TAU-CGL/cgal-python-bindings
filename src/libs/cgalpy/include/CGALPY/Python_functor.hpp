// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_PYTHON_FUNCTOR
#define CGALPY_PYTHON_FUNCTOR

#include <boost/core/ref.hpp>

#include <nanobind/nanobind.h>

#include "CGALPY/reference_wrapper_caster.hpp"

namespace py = nanobind;

//https://stackoverflow.com/a/26833886 regarding calling a python functor in C++
template <typename T0, typename T1>
class Python_functor_1 {
private:
  py::object m_python_functor;

public:
  Python_functor_1() {}
  Python_functor_1(py::object python_functor) : m_python_functor(python_functor)
  {}

  T1 operator()(T0 a) const { return py::cast<T1>(m_python_functor(a)); }
};

template <typename T0, typename T1, typename T2>
class Python_functor_2 {
private:
  py::object m_python_functor;

public:
  Python_functor_2() {}
  Python_functor_2(py::object python_functor) :
    m_python_functor(python_functor)
  {}

  T2 operator()(T0 a, T1 b) const
  { return py::cast<T2>(m_python_functor(a, b)); }
};

template <typename T0, typename T1, typename T2>
class Python_functor_2_ref {
private:
  py::object m_python_functor;

public:
  Python_functor_2_ref() {}
  Python_functor_2_ref(py::object python_functor) :
    m_python_functor(python_functor)
  {}

  T2 operator()(const T0& a, const T1& b) const
  { return py::cast<T2>(m_python_functor(a, b)); }
};

template <typename T0, typename T1, typename T2>
class Python_functor_3_ref {
private:
  py::object m_python_functor;

public:
  Python_functor_3_ref() : m_python_functor(py::none()) {}
  Python_functor_3_ref(py::object python_functor) :
    m_python_functor(python_functor)
  {}
  ~Python_functor_3_ref() { m_python_functor = py::none(); }

  void operator()(const T0& a, const T1& b, T2& r) const {
    // By default, arguments are copied into new Python objects.
    // Override this behavior by the use of boost::ref()
    if (! m_python_functor.is_none()) m_python_functor(a, b, std::ref(r));
  }
};

#endif
