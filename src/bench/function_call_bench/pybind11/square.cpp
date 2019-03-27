#include <math.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

double square(double i) { return sqrt(i); }

double square_times(double i, int times)
{
  double res(0);
  for (auto j = 0; j < times; ++j) {
    res += i;
    res = sqrt(res);
  }
  return res;
}

PYBIND11_MODULE(square, m) {
  m.doc() = "pybind11 example plugin"; // optional module docstring

  m.def("square", &square, "A function that calculates the square root of a number");
  m.def("squareTimes", &square_times, "A function that calculates something");
}
