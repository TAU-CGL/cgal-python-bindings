// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#ifdef CGALPY_MINKOWSKI_SUM_2_BINDINGS
#include "CGALPY/common.hpp"

#include <CGAL/Gps_circle_segment_traits_2.h>

typedef typename CGAL::Gps_circle_segment_traits_2<Kernel> CSTraits;
typedef typename CSTraits::Polygon_2                       General_polygon_2;
typedef typename CSTraits::Polygon_with_holes_2
  General_polygon_with_holes_2;
typedef typename CGAL::General_polygon_set_2<CSTraits>     General_polygon_set_2;
typedef General_polygon_2::X_monotone_curve_2
  CS_traits_X_monotone_curve_2;
typedef General_polygon_2::Curve_iterator                  Curve_iterator;

General_polygon_with_holes_2* init_General_polygon_with_holes_2(General_polygon_2& p, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator< General_polygon_2 >(lst);
  auto end = boost::python::stl_input_iterator< General_polygon_2 >();
  return new General_polygon_with_holes_2(p, begin, end);
}

General_polygon_with_holes_2::Hole_iterator holes_begin(General_polygon_with_holes_2& p)
{
  return p.holes_begin();
}

General_polygon_with_holes_2::Hole_iterator holes_end(General_polygon_with_holes_2& p)
{
  return p.holes_end();
}

General_polygon_2& outer_boundary(General_polygon_with_holes_2& p) { return p.outer_boundary(); }

void export_general_polygon_with_holes_2()
{
  using namespace boost::python;
  class_<General_polygon_with_holes_2>("General_polygon_with_holes_2")
    .def(init<General_polygon_2&>())
    .def("__init__", make_constructor(&init_General_polygon_with_holes_2))
    .def("is_unbounded", &General_polygon_with_holes_2::is_unbounded)
    .def("outer_boundary", &outer_boundary, return_internal_reference<>())
    .def("holes", range<return_internal_reference<>>(&holes_begin, &holes_end))
    .def("number_of_holes", &General_polygon_with_holes_2::number_of_holes)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;
}
#endif
