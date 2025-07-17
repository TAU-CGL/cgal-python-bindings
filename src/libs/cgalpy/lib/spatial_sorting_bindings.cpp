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

// //!
void spatial_sort_2(py::list& points) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
#if 1
  std::vector<Point_2> v(begin, end);
  CGAL::spatial_sort<Concurrency>(v.begin(), v.end());
#else
  CGAL::spatial_sort<Concurrency>(begin, end);
#endif
}

// //!
// void spatial_sort_3(py::list& points) {
//   auto begin = stl_forward_iterator<Point_3>(points);
//   auto end = stl_forward_iterator<Point_3>(points, false);
//   CGAL::spatial_sort<Concurrency>(begin, end);
// }

// #if defined(CGALPY_KERNEL_BINDINGS)

// //!
// void spatial_sort_d(py::list& points) {
//   auto begin = stl_forward_iterator<Point_d>(points);
//   auto end = stl_forward_iterator<Point_d>(points, false);
//   CGAL::spatial_sort<Concurrency>(begin, end);
// }

// #endif

}

void export_spatial_sorting(py::module_& m) {
  //   m.def("spatial_sort", &st::spatial_sort_2);
  //   m.def("spatial_sort", &st::spatial_sort_3);
// #if defined(CGALPY_KERNEL_BINDINGS)
//   m.def("spatial_sort", &st::spatial_sort_d);
// #endif
}
