// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <vector>

#include <nanobind/nanobind.h>

#include <CGAL/convex_hull_2.h>
#include <CGAL/ch_eddy.h>
#include <CGAL/ch_jarvis.h>
#include <CGAL/ch_melkman.h>

#include "CGALPY/convex_hull_2_config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

void ch_akl_toussaint(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_akl_toussaint(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_bykat(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_bykat(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_eddy(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_eddy(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_graham_andrew(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_graham_andrew(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_jarvis(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_jarvis(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_melkman(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_melkman(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void convex_hull_2(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::convex_hull_2(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

bool is_ccw_strongly_convex_2(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v = std::vector<Point_2>(begin, end);
  return CGAL::is_ccw_strongly_convex_2(v.begin(), v.end());
}

bool is_cw_strongly_convex_2(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v = std::vector<Point_2>(begin, end);
  return CGAL::is_cw_strongly_convex_2(v.begin(), v.end());
}

void lower_hull_points_2(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::lower_hull_points_2(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void upper_hull_points_2(py::list& lst, py::list& res) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::upper_hull_points_2(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void export_convex_hull_2_bindings(py::module_& m) {
  m.def("ch_akl_toussaint", &ch_akl_toussaint);
  m.def("ch_bykat", &ch_bykat);
  m.def("ch_eddy", &ch_eddy);
  m.def("ch_graham_andrew", &ch_graham_andrew);
  m.def("ch_jarvis", &ch_jarvis);
  m.def("ch_melkman", &ch_melkman);
  m.def("convex_hull_2", &convex_hull_2);
  m.def("is_ccw_strongly_convex_2", &is_ccw_strongly_convex_2);
  m.def("is_cw_strongly_convex_2", &is_cw_strongly_convex_2);
  m.def("lower_hull_points_2", lower_hull_points_2);
  m.def("upper_hull_points_2", upper_hull_points_2);
}
