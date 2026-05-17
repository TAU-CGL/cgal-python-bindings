// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <boost/iterator/function_output_iterator.hpp>

#include <CGAL/partition_2.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/polygon_partitioning_types.hpp"
#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

namespace py = nanobind;

namespace pp2 {

//
// void approx_convex_partition_2(const Polygon_2& p, py::list& res) {
//   auto v = std::vector<Polygon_2>();
//   CGAL::approx_convex_partition_2(p.vertices_begin(), p.vertices_end(),
//                                   std::back_inserter(v));
//   for (auto c_polygon : v) res.append(c_polygon);
// }

//
// void greene_approx_convex_partition_2(const Polygon_2& p, py::list& res) {
//   auto v = std::vector<Polygon_2>();
//   CGAL::greene_approx_convex_partition_2(p.vertices_begin(), p.vertices_end(),
//                                          std::back_inserter(v));
//   for (auto c_polygon : v) res.append(c_polygon);
// }

//
// void optimal_convex_partition_2(const Polygon_2& p, py::list& res) {
//   auto v = std::vector<Polygon_2>();
//   CGAL::optimal_convex_partition_2(p.vertices_begin(), p.vertices_end(),
//                                    std::back_inserter(v));
//   for (auto c_polygon : v) res.append(c_polygon);
// }

// //
// void y_monotone_partition_2(const Polygon_2& p, py::list& res) {
//   auto v = std::vector<Polygon_2>();
//   CGAL::y_monotone_partition_2(p.vertices_begin(), p.vertices_end(),
//                                std::back_inserter(v));
//   for (auto ym_polygon : v) res.append(ym_polygon);
// }

/*! Bind partition is valid.
 */
// bool partition_is_valid_2(const Polygon_2& p, const py::list& lst) {
//   auto begin = stl_forward_iterator<Polygon_2>(lst);
//   auto end = stl_forward_iterator<Polygon_2>(lst, false);
//   return CGAL::partition_is_valid_2(p.vertices_begin(), p.vertices_end(),
//                                     begin, end, traits);
// }

//
// bool convex_partition_is_valid_2(Polygon_2& p, const py::list& lst) {
//   auto begin = stl_forward_iterator<Polygon_2>(lst);
//   auto end = stl_forward_iterator<Polygon_2>(lst, false);
//   return CGAL::convex_partition_is_valid_2(p.vertices_begin(), p.vertices_end(),
//                                            begin, end, traits);
// }

//
bool y_monotone_partition_is_valid_2(const Polygon_2& p, const py::list& lst) {
  // The following is completely dumb!
  // Apparently, the concept CGAL::Partition_traits_2 perscribes the nested
  // type Polygon_2 to be CGAL::Polygon_2<Kernel, std::list<Point_2>>.
  // However, we use the type CGAL::Polygon_2<Kernel, std::vector<Point_2>>,
  // So we copy all input polygons...
  auto sbegin = stl_forward_iterator<Polygon_2>(lst);
  auto send = stl_forward_iterator<Polygon_2>(lst, false);
  using My_polygon_2 = CGAL::Polygon_2<Kernel, std::list<Kernel::Point_2>>;
  std::vector<My_polygon_2> pgns(lst.size());
  std::transform(sbegin, send, pgns.begin(),
                 [] (const Polygon_2& src) -> My_polygon_2 {
                   My_polygon_2 trg(src.vertices_begin(), src.vertices_end());
                   return trg;
                 });
  auto begin = pgns.begin();
  auto end = pgns.end();
  return CGAL::y_monotone_partition_is_valid_2(p.vertices_begin(),
                                               p.vertices_end(), begin, end);
}

//
bool is_y_monotone_2(Polygon_2& p)
{ return CGAL::is_y_monotone_2(p.vertices_begin(), p.vertices_end()); }

//
bool is_convex_2(Polygon_2& p)
{ return CGAL::is_convex_2(p.vertices_begin(), p.vertices_end()); }

//
template<typename T>
py::list polygon_vertical_decomposition_2(Polygon_vertical_decomposition_2& pvd,
                                          T& polygon) {
  py::list res;
  auto op = [&] (const Polygon_2& pgn) mutable
  { res.append(pgn); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  pvd(polygon, it);
  return res;
}

//
template <typename T>
py::list polygon_triangulation_decomposition_2
(Polygon_triangulation_decomposition_2& ptd, T& polygon)
{
  py::list res;
  auto op = [&] (const Polygon_2& pgn) mutable
  { res.append(pgn); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  ptd(polygon, it);
  return res;
}

//
py::list small_side_angle_bisector_decomposition_2
(Small_side_angle_bisector_decomposition_2& ssabd, Polygon_2& polygon)
{
  py::list res;
  auto op = [&] (const Polygon_2& pgn) mutable
  { res.append(pgn); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  ssabd(polygon, it);
  return res;
}

}

void export_polygon_partition_2(py::module_& m) {
  using Pgn_2 = pp2::Polygon_2;
  using Pwh_2 = pp2::Polygon_with_holes_2;
  using PVD_2 = pp2::Polygon_vertical_decomposition_2;
  using PTD_2 = pp2::Polygon_triangulation_decomposition_2;
  using SSABD_2 = pp2::Small_side_angle_bisector_decomposition_2;

  // m.def("approx_convex_partition_2", &pp2::approx_convex_partition_2);
  // m.def("greene_approx_convex_partition_2",
  //       &pp2::greene_approx_convex_partition_2);
  // m.def("optimal_convex_partition_2", &pp2::optimal_convex_partition_2);
  // m.def("y_monotone_partition_2", &pp2::y_monotone_partition_2);
  // m.def("partition_is_valid_2", &pp2::partition_is_valid_2);
  // m.def("convex_partition_is_valid_2", &pp2::convex_partition_is_valid_2);
  // m.def("y_monotone_partition_is_valid_2",
  //       &pp2::y_monotone_partition_is_valid_2);
  m.def("is_y_monotone_2", &pp2::is_y_monotone_2, py::arg("polygon"));
  m.def("is_convex_2", &pp2::is_convex_2, py::arg("polygon"));

  if (! add_attr<PVD_2>(m, "Polygon_vertical_decomposition")) {
    py::class_<PVD_2>(m, "Polygon_vertical_decomposition")
      .def(py::init<>())
      .def("__call__", &pp2::polygon_vertical_decomposition_2<Pgn_2>,
      py::arg("polygon"))
      .def("__call__", &pp2::polygon_vertical_decomposition_2<Pwh_2>,
      py::arg("polygon_with_holes"))
    ;
  }

  if (! add_attr<PTD_2>(m, "Polygon_triangulation_decomposition")) {
    py::class_<PTD_2>(m, "Polygon_triangulation_decomposition")
      .def(py::init<>())
      .def("__call__", &pp2::polygon_triangulation_decomposition_2<Pgn_2>,
      py::arg("polygon"))
      .def("__call__", &pp2::polygon_triangulation_decomposition_2<Pwh_2>,
      py::arg("polygon_with_holes"))
      ;
  }

  if (! add_attr<SSABD_2>(m, "Small_side_angle_bisector_decomposition")) {
    py::class_<SSABD_2>(m, "Small_side_angle_bisector_decomposition")
      .def(py::init<>())
      .def("__call__", &pp2::small_side_angle_bisector_decomposition_2,
      py::arg("polygon"))
      ;
  }
}
