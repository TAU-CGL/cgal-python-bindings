// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>

#include "CGALPY/polygon_set_2_types.hpp"
#include "CGALPY/python_iterator_templates.hpp"

namespace bp = boost::python;

Point_2& left_vertex(Polygon_2& P) { return *(P.left_vertex()); }
Point_2& right_vertex(Polygon_2& P) { return *(P.right_vertex()); }
Point_2& top_vertex(Polygon_2& P) { return *(P.top_vertex()); }
Point_2& bottom_vertex(Polygon_2& P) { return *(P.bottom_vertex()); }

static Polygon_2* init_from_list(bp::list& lst) {
  auto begin = bp::stl_input_iterator< Point_2 >(lst);
  auto end = bp::stl_input_iterator< Point_2 >();
  return new Polygon_2(begin, end);
}

CopyIterator<Polygon_2::Edge_const_iterator>* edges_iterator(Polygon_2& P) {
  return new CopyIterator<Polygon_2::Edge_const_iterator>(P.edges_begin(), P.edges_end());
}

void export_polygon_2() {
  bp::class_<Polygon_2>("Polygon_2")
    .def(bp::init<>())
    .def(bp::init<const Polygon_2&>())
    .def("__init__", make_constructor(&init_from_list))
    .def("push_back", &Polygon_2::push_back)
    .def("is_simple", &Polygon_2::is_simple)
    .def("is_convex", &Polygon_2::is_convex)
    .def("orientation", &Polygon_2::orientation)
    .def("oriented_side", &Polygon_2::oriented_side)
    .def("bounded_side", &Polygon_2::bounded_side)
    .def("is_empty", &Polygon_2::is_empty)
    .def("is_counterclockwise_oriented", &Polygon_2::is_counterclockwise_oriented)
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
    .def("vertices", range<return_internal_reference<>>(&Polygon_2::vertices_begin, &Polygon_2::vertices_end))
    .def("edges", &edges_iterator, bp::return_value_policy<bp::manage_new_object>())
    .def<const Point_2& (Polygon_2::*)(std::size_t) const>("__getitem__", &Polygon_2::operator[], bp::return_value_policy<bp::copy_const_reference>())
    .def("left_vertex", &left_vertex, bp::return_internal_reference<>())
    .def("right_vertex", &right_vertex, bp::return_internal_reference<>())
    .def("top_vertex", &top_vertex, bp::return_internal_reference<>())
    .def("bottom_vertex", &bottom_vertex, bp::return_internal_reference<>())
    .def<const Point_2& (Polygon_2::*)(std::size_t) const>("vertex", &Polygon_2::vertex, bp::return_value_policy<bp::copy_const_reference>())
    .def("edge", &Polygon_2::edge)
    .def("clear", &Polygon_2::clear)
    .def("reverse_orientation", &Polygon_2::reverse_orientation)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    ;

  bind_copy_iterator<CopyIterator<Polygon_2::Edge_const_iterator>>("Polygon_edges_iterator");
}
