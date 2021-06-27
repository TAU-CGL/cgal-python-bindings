// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>

#include "CGALPY/convex_hull_2_types.hpp"

namespace bp = boost::python;

void ch_akl_toussaint(bp::list& lst, bp::list& res) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_akl_toussaint(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_bykat(bp::list& lst, bp::list& res) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_bykat(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_eddy(bp::list& lst, bp::list& res) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_eddy(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_graham_andrew(bp::list& lst, bp::list& res)
{
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_graham_andrew(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_jarvis(bp::list& lst, bp::list& res) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_jarvis(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void ch_melkman(bp::list& lst, bp::list& res) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::ch_melkman(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void convex_hull_2(bp::list& lst, bp::list& res) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::convex_hull_2(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

bool is_ccw_strongly_convex_2(bp::list& lst) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v = std::vector<Point_2>(begin, end);
  return CGAL::is_ccw_strongly_convex_2(v.begin(), v.end());
}

bool is_cw_strongly_convex_2(bp::list& lst) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v = std::vector<Point_2>(begin, end);
  return CGAL::is_cw_strongly_convex_2(v.begin(), v.end());
}

void lower_hull_points_2(bp::list& lst, bp::list& res) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::lower_hull_points_2(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void upper_hull_points_2(bp::list& lst, bp::list& res) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  auto v0 = std::vector<Point_2>(begin, end);
  auto v1 = std::vector<Point_2>();
  CGAL::upper_hull_points_2(v0.begin(), v0.end(), std::back_inserter(v1));
  for (auto p : v1) res.append(p);
}

void export_convex_hull_2_bindings() {
  bp::def("ch_akl_toussaint", &ch_akl_toussaint);
  bp::def("ch_bykat", &ch_bykat);
  bp::def("ch_eddy", &ch_eddy);
  bp::def("ch_graham_andrew", &ch_graham_andrew);
  bp::def("ch_jarvis", &ch_jarvis);
  bp::def("ch_melkman", &ch_melkman);
  bp::def("convex_hull_2", &convex_hull_2);
  bp::def("is_ccw_strongly_convex_2", &is_ccw_strongly_convex_2);
  bp::def("is_cw_strongly_convex_2", &is_cw_strongly_convex_2);
  bp::def("lower_hull_points_2", lower_hull_points_2);
  bp::def("upper_hull_points_2", upper_hull_points_2);
}
