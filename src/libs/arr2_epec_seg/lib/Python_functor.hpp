#ifndef PYTHON_FUNCTOR
#define PYTHON_FUNCTOR

#include <boost/python.hpp>
namespace bp = boost::python;
template<typename T>
class Python_functor
{
private:
  bp::object m_python_functor;
public:
  Python_functor()
  {}
  Python_functor(bp::object python_functor) : m_python_functor(python_functor)
  {}

  T operator()(T a, T b)
  {
    return bp::extract<T>(m_python_functor(a, b));
  }
};

#endif // !PYTHON_FUNCTOR