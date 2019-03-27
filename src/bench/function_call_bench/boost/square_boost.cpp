#include <math.h>

#include <boost/python.hpp>

namespace bp = boost::python;

double square(double i) { return sqrt(i); }

double square_times(double i, int times) {
  double res(0);
  for (auto j = 0; j < times; ++j) {
    res += i;
    res = sqrt(res);
  }
  return res;
}

BOOST_PYTHON_MODULE(square_boost)
{
  bp::def("square", square);
  bp::def("squareTimes", square_times);
}
