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
template <typename T>
class Python_functor {
private:
  bp::object m_python_functor;

public:
  Python_functor() {}

  Python_functor(bp::object python_functor) : m_python_functor(python_functor)
  {}

  T operator()(T a, T b) { return bp::extract<T>(m_python_functor(a, b)); }
};

#endif // !PYTHON_FUNCTOR
