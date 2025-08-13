// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/spatial_sort.h>

#if defined(CGALPY_KERNEL_BINDINGS)
#include "CGALPY/kernel_d_types.hpp"
#endif
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/spatial_sorting_types.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

namespace py = nanobind;

namespace st {

//!
void spatial_sort_2_1(py::list& points, const Kernel& kernel,
                      int threshold_hilbert, int threshold_multiscale, double ratio) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  std::vector<Point_2> v(begin, end);
  CGAL::spatial_sort<Concurrency>(v.begin(), v.end(), kernel, Policy(), threshold_hilbert, threshold_multiscale, ratio);
  points.clear();
  for (const auto& p : v) points.append(p);
}

//!
void spatial_sort_2_2(py::list& points) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  std::vector<Point_2> v(begin, end);
  CGAL::spatial_sort<Concurrency>(v.begin(), v.end(), Policy());
  points.clear();
  for (const auto& p : v) points.append(p);
}

//!
void spatial_sort_3_1(py::list& points, const Kernel& kernel,
                      int threshold_hilbert, int threshold_multiscale, double ratio) {
  auto begin = stl_forward_iterator<Point_3>(points);
  auto end = stl_forward_iterator<Point_3>(points, false);
  std::vector<Point_3> v(begin, end);
  CGAL::spatial_sort<Concurrency>(v.begin(), v.end(), kernel, Policy(), threshold_hilbert, threshold_multiscale, ratio);
  points.clear();
  for (const auto& p : v) points.append(p);
}

//!
void spatial_sort_3_2(py::list& points) {
  auto begin = stl_forward_iterator<Point_3>(points);
  auto end = stl_forward_iterator<Point_3>(points, false);
  std::vector<Point_3> v(begin, end);
  CGAL::spatial_sort<Concurrency>(v.begin(), v.end(), Policy());
  points.clear();
  for (const auto& p : v) points.append(p);
}

#if defined(CGALPY_KERNEL_BINDINGS)

//!
void spatial_sort_d_1(py::list& points, const Kernel_d& kernel,
                      int threshold_hilbert, int threshold_multiscale, double ratio) {
  auto begin = stl_forward_iterator<Point_d>(points);
  auto end = stl_forward_iterator<Point_d>(points, false);
  std::vector<Point_d> v(begin, end);
  CGAL::spatial_sort<Concurrency>(v.begin(), v.end(), kernel, Policy(), threshold_hilbert, threshold_multiscale, ratio);
  points.clear();
  for (const auto& p : v) points.append(p);
}

//!
void spatial_sort_d_2(py::list& points) {
  auto begin = stl_forward_iterator<Point_d>(points);
  auto end = stl_forward_iterator<Point_d>(points, false);
  std::vector<Point_d> v(begin, end);
  CGAL::spatial_sort<Concurrency>(v.begin(), v.end(), Policy());
  points.clear();
  for (const auto& p : v) points.append(p);
}

#endif

}

void export_spatial_sorting(py::module_& m) {
  m.def("spatial_sort", &st::spatial_sort_2_1, py::arg("points"), py::arg("kernel"),
        py::arg("threshold_hilbert") = 0, py::arg("threshold_multiscale") = 0, py::arg("ratio") = 0.0);
  m.def("spatial_sort", &st::spatial_sort_2_2, py::arg("points"));
  m.def("spatial_sort", &st::spatial_sort_3_1, py::arg("points"), py::arg("kernel"),
        py::arg("threshold_hilbert") = 0, py::arg("threshold_multiscale") = 0, py::arg("ratio") = 0.0);
  m.def("spatial_sort", &st::spatial_sort_3_2, py::arg("points"));
#if defined(CGALPY_KERNEL_BINDINGS)
  m.def("spatial_sort", &st::spatial_sort_d_1, py::arg("points"), py::arg("kernel"),
        py::arg("threshold_hilbert") = 0, py::arg("threshold_multiscale") = 0, py::arg("ratio") = 0.0);
  m.def("spatial_sort", &st::spatial_sort_d_2, py::arg("points"));
#endif
}
