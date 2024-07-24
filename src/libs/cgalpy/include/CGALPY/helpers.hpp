#ifndef CGALPY_HELPERS_HPP
#define CGALPY_HELPERS_HPP

#include <nanobind/nanobind.h>
#include <vector>

namespace py = nanobind;

namespace pmp {

// helper
template <typename T>
py::list vec2list(const T& vec) {
  py::list retv;
  for (const auto& item : vec) {
    retv.append(item);
  }
  return retv;
}

template <typename T>
std::vector<T> list2vec(const py::list& list) {
  std::vector<T> retv;
  retv.reserve(py::len(list));
  for (const auto& item : list) {
    retv.push_back(py::cast<T>(item));
  }
  return retv;
}

} // namespace pmp

#endif // CGALPY_HELPERS_HPP
