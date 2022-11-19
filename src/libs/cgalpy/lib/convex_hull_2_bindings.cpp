// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <vector>

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>

#include <CGAL/convex_hull_2.h>
#include <CGAL/ch_eddy.h>
#include <CGAL/ch_jarvis.h>
#include <CGAL/ch_melkman.h>
#include <CGAL/ch_akl_toussaint.h>

#include "CGALPY/convex_hull_2_config.hpp"
#include "CGALPY/kernel_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

//! ch_akl_toussaint
py::list ch_akl_toussaint(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::ch_akl_toussaint(begin, end, it);
  return res;
}

//! ch_bykat
py::list ch_bykat(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::ch_bykat(begin, end, it);
  return res;
}

//! ch_eddy
py::list ch_eddy(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::ch_eddy(begin, end, it);
  return res;
}

//! ch_graham_andrew
py::list ch_graham_andrew(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::ch_graham_andrew(begin, end, it);
  return res;
}

//! ch_jarvis
py::list ch_jarvis(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::ch_jarvis(begin, end, it);
  return res;
}

//! ch_melkman
py::list ch_melkman(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::ch_melkman(begin, end, it);
  return res;
}

//! convex_hull_2
py::list convex_hull_2(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::convex_hull_2(begin, end, it);
  return res;
}

//! is_ccw_strongly_convex_2
bool is_ccw_strongly_convex_2(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  return CGAL::is_ccw_strongly_convex_2(begin, end);
}

//! is_cw_strongly_convex_2
bool is_cw_strongly_convex_2(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  return CGAL::is_cw_strongly_convex_2(begin, end);
}

//! lower_hull_points_2
py::list lower_hull_points_2(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::lower_hull_points_2(begin, end, it);
  return res;
}

//! upper_hull_points_2
py::list upper_hull_points_2(py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  py::list res;
  auto op = [&] (const Point_2& p) mutable { res.append(p); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::upper_hull_points_2(begin, end, it);
  return res;
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
