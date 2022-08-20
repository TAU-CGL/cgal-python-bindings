// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <boost/iterator/function_input_iterator.hpp>

#include "CGALPY/polygon_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

namespace pol2 {

Point_2& left_vertex(Polygon_2& P) { return *(P.left_vertex()); }
Point_2& right_vertex(Polygon_2& P) { return *(P.right_vertex()); }
Point_2& top_vertex(Polygon_2& P) { return *(P.top_vertex()); }
Point_2& bottom_vertex(Polygon_2& P) { return *(P.bottom_vertex()); }

void init_from_list(Polygon_2& pgn, py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  new (&pgn) Polygon_2(begin, end);
}

CopyIterator<Polygon_2::Edge_const_iterator>* edges_iterator(Polygon_2& P) {
  return new CopyIterator<Polygon_2::Edge_const_iterator>(P.edges_begin(), P.edges_end());
}

}

void export_polygon_2(py::module_& m) {
  using Polygon_2 = pol2::Polygon_2;

  py::class_<Polygon_2> c(m, "Polygon_2");
  c.def(py::init<>())
    .def(py::init<const Polygon_2&>())
    // .def("__init__", make_constructor(&pol2::init_from_list)) NB
    .def("__init__", &pol2::init_from_list)
    .def("push_back", &Polygon_2::push_back)
    .def("is_simple", &Polygon_2::is_simple)
    .def("is_convex", &Polygon_2::is_convex)
    .def("orientation", &Polygon_2::orientation)
    .def("oriented_side", &Polygon_2::oriented_side)
    .def("bounded_side", &Polygon_2::bounded_side)
    .def("is_empty", &Polygon_2::is_empty)
    .def("is_counterclockwise_oriented",
         &Polygon_2::is_counterclockwise_oriented)
    .def("is_clockwise_oriented", &Polygon_2::is_clockwise_oriented)
    .def("is_collinear_oriented", &Polygon_2::is_collinear_oriented)
    .def("has_on_positive_side", &Polygon_2::has_on_positive_side)
    .def("has_on_negative_side", &Polygon_2::has_on_negative_side)
    .def("has_on_boundary", &Polygon_2::has_on_boundary)
    .def("has_on_bounded_side", &Polygon_2::has_on_bounded_side)
    .def("has_on_unbounded_side", &Polygon_2::has_on_unbounded_side)
    .def("size", &Polygon_2::size)
    .def("area", &Polygon_2::area)
    .def("bbox", &Polygon_2::bbox)
    // .def("vertices",
    //      py::range<py::return_internal_reference<>>(&Polygon_2::vertices_begin,
    //                                                 &Polygon_2::vertices_end))
    // .def("edges", &pol2::edges_iterator, Manage_new_object()) NB
    // .def<const Point_2& (Polygon_2::*)(std::size_t) const>
    //   ("__getitem__", &Polygon_2::operator[], Copy_const_reference())
    // .def("left_vertex", &pol2::left_vertex, py::return_internal_reference<>())
    // .def("right_vertex", &pol2::right_vertex, py::return_internal_reference<>())
    // .def("top_vertex", &pol2::top_vertex, py::return_internal_reference<>())
    // .def("bottom_vertex", &pol2::bottom_vertex, py::return_internal_reference<>())
    // .def<const Point_2& (Polygon_2::*)(std::size_t) const>
    //   ("vertex", &Polygon_2::vertex, Copy_const_reference())
    .def("edge", &Polygon_2::edge)
    .def("clear", &Polygon_2::clear)
    .def("reverse_orientation", &Polygon_2::reverse_orientation)
    .def(py::self == py::self)
    .def(py::self != py::self)
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");

  // bind_copy_iterator<CopyIterator<Polygon_2::Edge_const_iterator>>("Polygon_edges_iterator"); NB
}
