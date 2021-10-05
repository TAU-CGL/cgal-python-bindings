// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include <CGAL/Gps_circle_segment_traits_2.h>

#include "CGALPY/polygon_2_types.hpp"

namespace bp = boost::python;

typedef typename CGAL::Gps_circle_segment_traits_2<Kernel> CSTraits;
typedef typename CSTraits::Polygon_2                       General_polygon_2;
typedef General_polygon_2::X_monotone_curve_2
  CS_traits_X_monotone_curve_2;
typedef General_polygon_2::Curve_iterator                  Curve_iterator;

static General_polygon_2* init_from_list(bp::list& lst) {
  auto begin = bp::stl_input_iterator<CS_traits_X_monotone_curve_2>(lst);
  auto end = bp::stl_input_iterator<CS_traits_X_monotone_curve_2>();
  return new General_polygon_2(begin, end);
}

Curve_iterator curves_begin(General_polygon_2& p) { return p.curves_begin(); }

Curve_iterator curves_end(General_polygon_2& p) { return p.curves_end(); }

void export_general_polygon_2() {
  bp::class_<General_polygon_2>("General_polygon_2")
    .def(bp::init<>())
    .def(bp::init<General_polygon_2>())
    .def("__init__", make_constructor(&init_from_list))
    .def("push_back", &General_polygon_2::push_back)
    .def("orientation", &General_polygon_2::orientation)
    .def("is_empty", &General_polygon_2::is_empty)
    .def("size", &General_polygon_2::size)
    .def("bbox", &General_polygon_2::bbox)
    .def("curves", bp::range<bp::return_internal_reference<>>(&curves_begin, &curves_end))
    .def("clear", &General_polygon_2::clear)
    .def("reverse_orientation", &General_polygon_2::reverse_orientation)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    ;
}
