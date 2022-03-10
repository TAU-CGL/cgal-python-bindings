// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/python.hpp>

#include "CGALPY/polygon_partitioning_types.hpp"
#include "CGALPY/polygon_2_types.hpp"

#include <CGAL/partition_2.h>

namespace bp = boost::python;

namespace pp2 {

void approx_convex_partition_2(Polygon_2& p, bp::list& res) {
  auto v = std::vector<Polygon_2>();
  CGAL::approx_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v) res.append(c_polygon);
}

void greene_approx_convex_partition_2(Polygon_2& p, bp::list& res) {
  auto v = std::vector<Polygon_2>();
  CGAL::greene_approx_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v) res.append(c_polygon);
}

void optimal_convex_partition_2(Polygon_2& p, bp::list& res) {
  auto v = std::vector<Polygon_2>();
  CGAL::optimal_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v) res.append(c_polygon);
}

void y_monotone_partition_2(Polygon_2& p, bp::list& res) {
  auto v = std::vector<Polygon_2>();
  CGAL::y_monotone_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto ym_polygon : v) res.append(ym_polygon);
}

bool partition_is_valid_2(Polygon_2& p, bp::list& polygon_lst) {
  auto begin = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = bp::stl_input_iterator<Polygon_2>();
  auto v = std::vector<Polygon_2>(begin, end);
  return CGAL::partition_is_valid_2(p.vertices_begin(), p.vertices_end(), v.begin(), v.end());
}

bool convex_partition_is_valid_2(Polygon_2& p, bp::list& polygon_lst) {
  auto begin = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = bp::stl_input_iterator<Polygon_2>();
  auto v = std::vector<Polygon_2>(begin, end);
  return CGAL::convex_partition_is_valid_2(p.vertices_begin(), p.vertices_end(), v.begin(), v.end());
}

bool y_monotone_partition_is_valid_2(Polygon_2& p, bp::list& polygon_lst) {
  auto begin = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = bp::stl_input_iterator<Polygon_2>();
  auto v = std::vector<Polygon_2>(begin, end);
  return CGAL::y_monotone_partition_is_valid_2(p.vertices_begin(), p.vertices_end(), v.begin(), v.end());
}

bool is_y_monotone_2(Polygon_2& p)
{ return CGAL::is_y_monotone_2(p.vertices_begin(), p.vertices_end()); }

bool is_convex_2(Polygon_2& p)
{ return CGAL::is_convex_2(p.vertices_begin(), p.vertices_end()); }

template<typename T>
void polygon_vertical_decomposition_2(Polygon_vertical_decomposition_2& pvd,
                                      T& polygon, bp::list& res) {
  auto v = std::vector<Polygon_2>();
  pvd(polygon, std::back_inserter(v));
  for (auto trapezoid : v) res.append(trapezoid);
}

template<typename T>
void polygon_triangulation_decomposition_2
(Polygon_triangulation_decomposition_2& ptd, T& polygon, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  ptd(polygon, std::back_inserter(v));
  for (auto triangle : v) res.append(triangle);
}

void small_side_angle_bisector_decomposition_2
(Small_side_angle_bisector_decomposition_2& ssabd, Polygon_2& polygon, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  ssabd(polygon, std::back_inserter(v));
  for (auto c_polygon : v) res.append(c_polygon);
}

}

void export_polygon_partition_2() {
  typedef pp2::Polygon_2                                        Pgn_2;
  typedef pp2::Polygon_with_holes_2                             Pwh_2;
  typedef pp2::Polygon_vertical_decomposition_2                 PVD_2;
  typedef pp2::Polygon_triangulation_decomposition_2            PTD_2;
  typedef pp2::Small_side_angle_bisector_decomposition_2        SSABD_2;

  bp::def("approx_convex_partition_2", &pp2::approx_convex_partition_2);
  bp::def("greene_approx_convex_partition_2", &pp2::greene_approx_convex_partition_2);
  bp::def("optimal_convex_partition_2", &pp2::optimal_convex_partition_2);
  bp::def("y_monotone_partition_2", &pp2::y_monotone_partition_2);
  bp::def("partition_is_valid_2", &pp2::partition_is_valid_2);
  bp::def("convex_partition_is_valid_2", &pp2::convex_partition_is_valid_2);
  bp::def("y_monotone_partition_is_valid_2", &pp2::y_monotone_partition_is_valid_2);
  bp::def("is_y_monotone_2", &pp2::is_y_monotone_2);
  bp::def("is_convex_2", &pp2::is_convex_2);

  bp::class_<PVD_2>("Polygon_vertical_decomposition")
    .def(bp::init<>())
    .def("__call__", &pp2::polygon_vertical_decomposition_2<Pgn_2>)
    .def("__call__", &pp2::polygon_vertical_decomposition_2<Pwh_2>)
    ;

  bp::class_<PTD_2>("Polygon_triangulation_decomposition")
    .def(bp::init<>())
    .def("__call__", &pp2::polygon_triangulation_decomposition_2<Pgn_2>)
    .def("__call__", &pp2::polygon_triangulation_decomposition_2<Pwh_2>)
    ;

  bp::class_<SSABD_2>("Small_side_angle_bisector_decomposition")
    .def(bp::init<>())
    .def("__call__", &pp2::small_side_angle_bisector_decomposition_2)
    ;
}
