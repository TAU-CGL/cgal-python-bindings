// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/partition_2.h>

#include "CGALPY/polygon_partitioning_types.hpp"
#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace pp2 {

//
void approx_convex_partition_2(const Polygon_2& p, py::list& res) {
  auto v = std::vector<Polygon_2>();
  CGAL::approx_convex_partition_2(p.vertices_begin(), p.vertices_end(),
                                  std::back_inserter(v));
  for (auto c_polygon : v) res.append(c_polygon);
}

//
void greene_approx_convex_partition_2(const Polygon_2& p, py::list& res) {
  auto v = std::vector<Polygon_2>();
  CGAL::greene_approx_convex_partition_2(p.vertices_begin(), p.vertices_end(),
                                         std::back_inserter(v));
  for (auto c_polygon : v) res.append(c_polygon);
}

//
void optimal_convex_partition_2(const Polygon_2& p, py::list& res) {
  auto v = std::vector<Polygon_2>();
  CGAL::optimal_convex_partition_2(p.vertices_begin(), p.vertices_end(),
                                   std::back_inserter(v));
  for (auto c_polygon : v) res.append(c_polygon);
}

//
void y_monotone_partition_2(const Polygon_2& p, py::list& res) {
  auto v = std::vector<Polygon_2>();
  CGAL::y_monotone_partition_2(p.vertices_begin(), p.vertices_end(),
                               std::back_inserter(v));
  for (auto ym_polygon : v) res.append(ym_polygon);
}

/*! Bind partition is valid.
 * \todo for some reason using stl_input_iterator does not work. Instead we
 * introduce and intermediate array.
 */
bool partition_is_valid_2(const Polygon_2& p, const py::list& lst) {
#if 1
  std::vector<Polygon_2> pgns;
  for (auto it = lst.begin(); it != lst.end(); ++it)
    pgns.push_back(py::cast<Polygon_2>(*it));
  auto begin = pgns.begin();
  auto end = pgns.end();
#else
  auto begin = stl_input_iterator<Polygon_2>(lst);
  auto end = stl_input_iterator<Polygon_2>(lst, false);
#endif
  return CGAL::partition_is_valid_2(p.vertices_begin(), p.vertices_end(),
                                    begin, end);
}

//
bool convex_partition_is_valid_2(Polygon_2& p, const py::list& lst) {
#if 1
  std::vector<Polygon_2> pgns;
  for (auto it = lst.begin(); it != lst.end(); ++it)
    pgns.push_back(py::cast<Polygon_2>(*it));
  auto begin = pgns.begin();
  auto end = pgns.end();
#else
  auto begin = stl_input_iterator<Polygon_2>(lst);
  auto end = stl_input_iterator<Polygon_2>(lst, false);
#endif
  return CGAL::convex_partition_is_valid_2(p.vertices_begin(), p.vertices_end(),
                                           begin, end);
}

//
bool y_monotone_partition_is_valid_2(Polygon_2& p, const py::list& lst) {
#if 1
  std::vector<Polygon_2> pgns;
  for (auto it = lst.begin(); it != lst.end(); ++it)
    pgns.push_back(py::cast<Polygon_2>(*it));
  auto begin = pgns.begin();
  auto end = pgns.end();
#else
  auto begin = stl_input_iterator<Polygon_2>(lst);
  auto end = stl_input_iterator<Polygon_2>(lst, false);
#endif
  return CGAL::y_monotone_partition_is_valid_2(p.vertices_begin(),
                                               p.vertices_end(),
                                               begin, end);
}

//
bool is_y_monotone_2(Polygon_2& p)
{ return CGAL::is_y_monotone_2(p.vertices_begin(), p.vertices_end()); }

//
bool is_convex_2(Polygon_2& p)
{ return CGAL::is_convex_2(p.vertices_begin(), p.vertices_end()); }

//
template<typename T>
void polygon_vertical_decomposition_2(Polygon_vertical_decomposition_2& pvd,
                                      T& polygon, py::list& res) {
  auto v = std::vector<Polygon_2>();
  pvd(polygon, std::back_inserter(v));
  for (auto trapezoid : v) res.append(trapezoid);
}

//
template<typename T>
void polygon_triangulation_decomposition_2
(Polygon_triangulation_decomposition_2& ptd, T& polygon, py::list& res)
{
  auto v = std::vector<Polygon_2>();
  ptd(polygon, std::back_inserter(v));
  for (auto triangle : v) res.append(triangle);
}

//
void small_side_angle_bisector_decomposition_2
(Small_side_angle_bisector_decomposition_2& ssabd,
 Polygon_2& polygon, py::list& res)
{
  auto v = std::vector<Polygon_2>();
  ssabd(polygon, std::back_inserter(v));
  for (auto c_polygon : v) res.append(c_polygon);
}

}

void export_polygon_partition_2(py::module_& m) {
  using Pgn_2 = pp2::Polygon_2;
  using Pwh_2 = pp2::Polygon_with_holes_2;
  using PVD_2 = pp2::Polygon_vertical_decomposition_2;
  using PTD_2 = pp2::Polygon_triangulation_decomposition_2;
  using SSABD_2 = pp2::Small_side_angle_bisector_decomposition_2;

  m.def("approx_convex_partition_2", &pp2::approx_convex_partition_2);
  m.def("greene_approx_convex_partition_2",
        &pp2::greene_approx_convex_partition_2);
  m.def("optimal_convex_partition_2", &pp2::optimal_convex_partition_2);
  m.def("y_monotone_partition_2", &pp2::y_monotone_partition_2);
  m.def("partition_is_valid_2", &pp2::partition_is_valid_2);
  m.def("convex_partition_is_valid_2", &pp2::convex_partition_is_valid_2);
  m.def("y_monotone_partition_is_valid_2",
        &pp2::y_monotone_partition_is_valid_2);
  m.def("is_y_monotone_2", &pp2::is_y_monotone_2);
  m.def("is_convex_2", &pp2::is_convex_2);

  py::class_<PVD_2>(m, "Polygon_vertical_decomposition")
    .def(py::init<>())
    .def("__call__", &pp2::polygon_vertical_decomposition_2<Pgn_2>)
    .def("__call__", &pp2::polygon_vertical_decomposition_2<Pwh_2>)
    ;

  py::class_<PTD_2>(m, "Polygon_triangulation_decomposition")
    .def(py::init<>())
    .def("__call__", &pp2::polygon_triangulation_decomposition_2<Pgn_2>)
    .def("__call__", &pp2::polygon_triangulation_decomposition_2<Pwh_2>)
    ;

  py::class_<SSABD_2>(m, "Small_side_angle_bisector_decomposition")
    .def(py::init<>())
    .def("__call__", &pp2::small_side_angle_bisector_decomposition_2)
    ;
}
