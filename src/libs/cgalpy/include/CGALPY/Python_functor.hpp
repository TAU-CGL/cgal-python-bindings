// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#ifndef CGALPY_PYTHON_FUNCTOR
#define CGALPY_PYTHON_FUNCTOR

#include <boost/python.hpp>
namespace bp = boost::python;

//https://stackoverflow.com/a/26833886 regarding calling a python functor in C++
template <typename T0, typename T1>
class Python_functor_1 {
private:
  bp::object m_python_functor;

public:
  Python_functor_1() {}
  Python_functor_1(bp::object python_functor) : m_python_functor(python_functor)
  {}

  T1 operator()(T0 a) const { return bp::extract<T1>(m_python_functor(a)); }
};

template <typename T0, typename T1, typename T2>
class Python_functor_2 {
private:
  bp::object m_python_functor;

public:
  Python_functor_2() {}
  Python_functor_2(bp::object python_functor) : m_python_functor(python_functor)
  {}

  T2 operator()(T0 a, T1 b) const { return bp::extract<T2>(m_python_functor(a, b)); }
};

template <typename T0, typename T1, typename T2>
class Python_functor_2_ref {
private:
  bp::object m_python_functor;

public:
  Python_functor_2_ref() {}
  Python_functor_2_ref(bp::object python_functor) : m_python_functor(python_functor)
  {}

  T2 operator()(const T0& a, const T1& b) const { return bp::extract<T2>(m_python_functor(a, b)); }
};

#endif // !PYTHON_FUNCTOR
