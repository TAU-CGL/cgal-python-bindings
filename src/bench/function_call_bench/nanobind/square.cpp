#include <math.h>

#include <nanobind/nanobind.h>

namespace py = nanobind;

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

NB_MODULE(square_nanobind, m) {
  // m.doc() = "nanobind example plugin"; // optional module docstring

  m.def("square", &square, "A function that calculates the square root of a number");
  m.def("squareTimes", &square_times, "A function that calculates something");
}
