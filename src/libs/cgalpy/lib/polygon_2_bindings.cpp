// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "cgalpy/polygon_2_types.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/add_attr.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/make_iterator.hpp"
#include "cgalpy/add_extraction.hpp"

#include "cgalpy/Pol2_docstrings.hpp"

#include <CGAL/Polygon_2_algorithms.h>
#ifdef CGALPY_HAS_VISUAL
#define CGAL_USE_BASIC_VIEWER
#include <CGAL/draw_polygon_2.h>
#endif

namespace py = nanobind;
namespace pol2_doc = cgalpy::pol2::docstrings;

namespace cgalpy {
namespace pol2 {

Point_2& left_vertex(Polygon_2& pgn) { return *(pgn.left_vertex()); }
Point_2& right_vertex(Polygon_2& pgn) { return *(pgn.right_vertex()); }
Point_2& top_vertex(Polygon_2& pgn) { return *(pgn.top_vertex()); }
Point_2& bottom_vertex(Polygon_2& pgn) { return *(pgn.bottom_vertex()); }

// Initialize a polygon from a list of vertices.
void init_polygon_2(Polygon_2* pgn, py::list& lst) {
  auto begin = stl_forward_iterator<Point_2>(lst);
  auto end = stl_forward_iterator<Point_2>(lst, false);
  new (pgn) Polygon_2(begin, end);      // placement new
}

/*!
 */
FT area_2_1(const py::list& points, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  FT area;
  CGAL::area_2(begin, end, area, kernel);
  return area;
}

/*!
 */
FT area_2_2(const py::list& points) { return area_2_1(points, Kernel()); }

/*!
 */
py::object bottom_vertex_2_1(const py::list& points, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  auto result_it = CGAL::bottom_vertex_2(begin, end, kernel);
  if (result_it == end) {
    throw std::invalid_argument("Cannot find bottom vertex of an empty list.");
  }
  // Re-create a fresh 'begin' iterator to calculate the index distance safely.
  // Note: This relies on stl_forward_iterator being a true Forward Iterator, not a single-pass Input Iterator.
  auto start_it = stl_forward_iterator<Point_2>(points);
  std::size_t index = std::distance(start_it, result_it);

  // Return the exact same Python object that was passed in!
  return points[index];
}

/*!
 */
py::object bottom_vertex_2_2(const py::list& points) { return bottom_vertex_2_1(points, Kernel()); }

/*!
 */
CGAL::Bounded_side bounded_side_2_1(const py::list& points, const Point_2& pnt, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  return CGAL::bounded_side_2(begin, end, pnt, kernel);
}

/*!
 */
CGAL::Bounded_side bounded_side_2_2(const py::list& points, const Point_2& pnt)
{ return bounded_side_2_1(points, pnt, Kernel()); }

/*!
 */
bool is_convex_2_1(const py::list& points, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  return CGAL::is_convex_2(begin, end, kernel);
}

/*!
 */
bool is_convex_2_2(const py::list& points) { return is_convex_2_1(points, Kernel()); }

/*!
 */
bool is_simple_2_1(const py::list& points, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  return CGAL::is_simple_2(begin, end, kernel);
}

/*!
 */
bool is_simple_2_2(const py::list& points) { return is_simple_2_1(points, Kernel()); }

/*!
 */
py::object left_vertex_2_1(const py::list& points, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  auto result_it = CGAL::left_vertex_2(begin, end, kernel);
  if (result_it == end) {
    throw std::invalid_argument("Cannot find left vertex of an empty list.");
  }
  // Re-create a fresh 'begin' iterator to calculate the index distance safely.
  // Note: This relies on stl_forward_iterator being a true Forward Iterator, not a single-pass Input Iterator.
  auto start_it = stl_forward_iterator<Point_2>(points);
  std::size_t index = std::distance(start_it, result_it);

  // Return the exact same Python object that was passed in!
  return points[index];
}

/*!
 */
py::object left_vertex_2_2(const py::list& points) { return left_vertex_2_1(points, Kernel()); }

/*! While this introduces an $O(N)$ memory copy, it is entirely unavoidable when
 * passing Python sequence data to a C++ algorithm that requires bidirectional
 * traversal, which is the case with CGAL::orientation_2() at the time this code
 * was written.
 */
CGAL::Orientation orientation_2_1(const py::list& points, const Kernel& kernel) {
  std::vector<Point_2> cpp_points;
  cpp_points.reserve(py::len(points));
  for (auto p : points) cpp_points.push_back(py::cast<Point_2>(p));
  return CGAL::orientation_2(cpp_points.begin(), cpp_points.end(), kernel);
}

/*!
 */
CGAL::Orientation orientation_2_2(const py::list& points) { return orientation_2_1(points, Kernel()); }

/*! While this introduces an $O(N)$ memory copy, it is entirely unavoidable when
 * passing Python sequence data to a C++ algorithm that requires bidirectional
 * traversal, which is the case with CGAL::orientation_2() at the time this code
 * was written.
 */
CGAL::Oriented_side oriented_side_2_1(const py::list& points, const Point_2& pnt, const Kernel& kernel) {
  std::vector<Point_2> cpp_points;
  cpp_points.reserve(py::len(points));
  for (auto p : points) cpp_points.push_back(py::cast<Point_2>(p));
  return CGAL::oriented_side_2(cpp_points.begin(), cpp_points.end(), pnt, kernel);
}

/*!
 */
CGAL::Oriented_side oriented_side_2_2(const py::list& points, const Point_2& pnt)
{ return oriented_side_2_1(points, pnt, Kernel()); }

/*!
 */
FT polygon_area_2_1(const py::list& points, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  return CGAL::polygon_area_2(begin, end, kernel);
}

/*!
 */
FT polygon_area_2_2(const py::list& points) { return polygon_area_2_1(points, Kernel()); }

/*!
 */
py::object right_vertex_2_1(const py::list& points, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  auto result_it =  CGAL::right_vertex_2(begin, end, kernel);
  if (result_it == end) {
    throw std::invalid_argument("Cannot find right vertex of an empty list.");
  }
  // Re-create a fresh 'begin' iterator to calculate the index distance safely.
  // Note: This relies on stl_forward_iterator being a true Forward Iterator, not a single-pass Input Iterator.
  auto start_it = stl_forward_iterator<Point_2>(points);
  std::size_t index = std::distance(start_it, result_it);

  // Return the exact same Python object that was passed in!
  return points[index];
}

/*!
 */
py::object right_vertex_2_2(const py::list& points) { return right_vertex_2_1(points, Kernel()); }

/*!
 */
py::object top_vertex_2_1(const py::list& points, const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_2>(points);
  auto end = stl_forward_iterator<Point_2>(points, false);
  auto result_it = CGAL::top_vertex_2(begin, end, kernel);
  if (result_it == end) {
    throw std::invalid_argument("Cannot find top vertex of an empty list.");
  }
  // Re-create a fresh 'begin' iterator to calculate the index distance safely.
  // Note: This relies on stl_forward_iterator being a true Forward Iterator, not a single-pass Input Iterator.
  auto start_it = stl_forward_iterator<Point_2>(points);
  std::size_t index = std::distance(start_it, result_it);

  // Return the exact same Python object that was passed in!
  return points[index];
}

/*!
 */
py::object top_vertex_2_2(const py::list& points) { return top_vertex_2_1(points, Kernel()); }

}
} // namespace cgalpy

// Export Polygon_2.
void export_polygon_2(py::module_& m) {
  constexpr auto ri(py::rv_policy::reference_internal);
  using Pgn = cgalpy::pol2::Polygon_2;
  using Pnt = Pgn::Point_2;

  if (! add_attr<Pgn>(m, "Polygon_2")) {
    py::class_<Pgn> pgn_c(m, "Polygon_2", pol2_doc::Polygon_2_class);
    pgn_c.def(py::init<>())
      .def(py::init<const Pgn&>())
      .def("__init__", &cgalpy::pol2::init_polygon_2, py::arg("vertices"))
      .def("push_back", &Pgn::push_back, py::arg("point"),
           pol2_doc::Polygon_2_push_back)
      .def("is_simple", &Pgn::is_simple, pol2_doc::Polygon_2_is_simple)
      .def("is_convex", &Pgn::is_convex, pol2_doc::Polygon_2_is_convex)
      .def("orientation", &Pgn::orientation, pol2_doc::Polygon_2_orientation)
      .def("oriented_side", &Pgn::oriented_side, py::arg("point"),
           pol2_doc::Polygon_2_oriented_side)
      .def("bounded_side", &Pgn::bounded_side, py::arg("point"),
           pol2_doc::Polygon_2_bounded_side)
      .def("is_empty", &Pgn::is_empty, pol2_doc::Polygon_2_is_empty)
      .def("is_counterclockwise_oriented", &Pgn::is_counterclockwise_oriented,
           pol2_doc::Polygon_2_is_counterclockwise_oriented)
      .def("is_clockwise_oriented", &Pgn::is_clockwise_oriented,
           pol2_doc::Polygon_2_is_clockwise_oriented)
      .def("is_collinear_oriented", &Pgn::is_collinear_oriented,
           pol2_doc::Polygon_2_is_collinear_oriented)
      .def("has_on_positive_side", &Pgn::has_on_positive_side, py::arg("point"),
           pol2_doc::Polygon_2_has_on_positive_side)
      .def("has_on_negative_side", &Pgn::has_on_negative_side, py::arg("point"),
           pol2_doc::Polygon_2_has_on_negative_side)
      .def("has_on_boundary", &Pgn::has_on_boundary, py::arg("point"),
           pol2_doc::Polygon_2_has_on_boundary)
      .def("has_on_bounded_side", &Pgn::has_on_bounded_side, py::arg("point"),
           pol2_doc::Polygon_2_has_on_bounded_side)
      .def("has_on_unbounded_side", &Pgn::has_on_unbounded_side, py::arg("point"),
           pol2_doc::Polygon_2_has_on_unbounded_side)
      .def("size", &Pgn::size, pol2_doc::Polygon_2_size)
      .def("area", &Pgn::area, pol2_doc::Polygon_2_area)
      .def("bbox", &Pgn::bbox, pol2_doc::Polygon_2_bbox)
      .def("__getitem__",
           static_cast<const Pnt&(Pgn::*)(std::size_t)const>(&Pgn::operator[]),
           py::arg("i"), pol2_doc::Polygon_2_operator_subscript)
      .def("left_vertex", &cgalpy::pol2::left_vertex, pol2_doc::Polygon_2_left_vertex)
      .def("right_vertex", &cgalpy::pol2::right_vertex, pol2_doc::Polygon_2_right_vertex)
      .def("top_vertex", &cgalpy::pol2::top_vertex, pol2_doc::Polygon_2_top_vertex)
      .def("bottom_vertex", &cgalpy::pol2::bottom_vertex, pol2_doc::Polygon_2_bottom_vertex)

      // Use `py::overload_cast` to cast overloaded functions.
      // 1. As a convention, add the suffix `_mutable` to the mutable version.
      // 2. Wrap the mutable method with the `reference_internal` call policy.
      // 3. Add the `const_` tag to the overloaded const function, as the
      //    overloading is based on constness.
      .def("vertex_mutable", py::overload_cast<size_t>(&Pgn::vertex), ri,
           py::arg("i"))
      .def("vertex", py::overload_cast<size_t>(&Pgn::vertex, py::const_),
           py::arg("i"))

      .def("edge", &Pgn::edge, py::arg("i"), pol2_doc::Polygon_2_edge)
      .def("clear", &Pgn::clear, pol2_doc::Polygon_2_clear)
      .def("reverse_orientation", &Pgn::reverse_orientation,
           pol2_doc::Polygon_2_reverse_orientation)
      .def(py::self == py::self)
      .def(py::self != py::self)
      ;

    using Eci = Pgn::Edge_const_iterator;
    add_iterator<Eci, Eci>("Edge_iterator", pgn_c);
    pgn_c.def("edges", [] (const Pgn& pgn) { return make_iterator(pgn.edges_begin(), pgn.edges_end()); },
              py::keep_alive<0, 1>(), pol2_doc::Polygon_2_edges);
    using Vci = Pgn::Vertex_const_iterator;
    add_iterator<Vci, Vci>("Vertex_iterator", pgn_c);
    pgn_c.def("vertices", [] (const Pgn& pgn) { return make_iterator(pgn.vertices_begin(), pgn.vertices_end()); },
              py::keep_alive<0, 1>(), pol2_doc::Polygon_2_vertices);

    add_insertion(pgn_c, "__str__");
    add_insertion(pgn_c, "__repr__");
    add_extraction(pgn_c);
  }

  // Free functions
  m.def("area_2", &cgalpy::pol2::area_2_1, py::arg("points"), py::arg("kernel"),
        pol2_doc::area_2);
  m.def("area_2", &cgalpy::pol2::area_2_2, py::arg("points"),
        pol2_doc::area_2);
  m.def("bottom_vertex_2", &cgalpy::pol2::bottom_vertex_2_1,
        py::arg("points"), py::arg("kernel"),
        pol2_doc::bottom_vertex_2_1);
  m.def("bottom_vertex_2", &cgalpy::pol2::bottom_vertex_2_2, py::arg("points"),
        pol2_doc::bottom_vertex_2_1);
  m.def("bounded_side_2", &cgalpy::pol2::bounded_side_2_1,
        py::arg("points"), py::arg("point"), py::arg("kernel"),
        pol2_doc::bounded_side_2_1);
  m.def("bounded_side_2", &cgalpy::pol2::bounded_side_2_2,
        py::arg("points"), py::arg("point"),
        pol2_doc::bounded_side_2_1);
  m.def("is_convex_2", &cgalpy::pol2::is_convex_2_1,
        py::arg("points"), py::arg("kernel"),
        pol2_doc::is_convex_2_1);
  m.def("is_convex_2", &cgalpy::pol2::is_convex_2_2, py::arg("points"),
        pol2_doc::is_convex_2_1);
  m.def("is_simple_2", &cgalpy::pol2::is_simple_2_1,
        py::arg("points"), py::arg("kernel"),
        pol2_doc::is_simple_2_1);
  m.def("is_simple_2", &cgalpy::pol2::is_simple_2_2, py::arg("points"),
        pol2_doc::is_simple_2_1);
  m.def("left_vertex_2", &cgalpy::pol2::left_vertex_2_1,
        py::arg("points"), py::arg("kernel"),
        pol2_doc::left_vertex_2_1);
  m.def("left_vertex_2", &cgalpy::pol2::left_vertex_2_2, py::arg("points"),
        pol2_doc::left_vertex_2_1);
  m.def("orientation_2", &cgalpy::pol2::orientation_2_1,
        py::arg("points"), py::arg("kernel"),
        pol2_doc::orientation_2_1);
  m.def("orientation_2", &cgalpy::pol2::orientation_2_2, py::arg("points"),
        pol2_doc::orientation_2_1);
  m.def("oriented_side_2", &cgalpy::pol2::oriented_side_2_1,
        py::arg("points"), py::arg("point"), py::arg("kernel"),
        pol2_doc::oriented_side_2_1);
  m.def("oriented_side_2", &cgalpy::pol2::oriented_side_2_2,
        py::arg("points"), py::arg("point"),
        pol2_doc::oriented_side_2_1);
  m.def("polygon_area_2", &cgalpy::pol2::polygon_area_2_1,
        py::arg("points"), py::arg("kernel"),
        pol2_doc::polygon_area_2);
  m.def("polygon_area_2", &cgalpy::pol2::polygon_area_2_2, py::arg("points"),
        pol2_doc::polygon_area_2);
  m.def("right_vertex_2", &cgalpy::pol2::right_vertex_2_1,
        py::arg("points"), py::arg("kernel"),
        pol2_doc::right_vertex_2_1);
  m.def("right_vertex_2", &cgalpy::pol2::right_vertex_2_2, py::arg("points"),
        pol2_doc::right_vertex_2_1);
  m.def("top_vertex_2", &cgalpy::pol2::top_vertex_2_1,
        py::arg("points"), py::arg("kernel"),
        pol2_doc::top_vertex_2_1);
  m.def("top_vertex_2", &cgalpy::pol2::top_vertex_2_2, py::arg("points"),
        pol2_doc::top_vertex_2_1);

#ifdef CGALPY_HAS_VISUAL
  using Draw = void(*)(const Pgn&, const char*);
  m.def("draw", static_cast<Draw>(CGAL::draw),  py::arg("pgn"), py::arg("title") = "");
#endif
}
