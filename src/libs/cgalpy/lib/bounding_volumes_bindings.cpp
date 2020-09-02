// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#ifdef CGALPY_BOUNDING_VOLUMES_BINDINGS
#include "CGALPY/common.hpp"

#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
typedef typename CGAL::Min_circle_2_traits_2<Kernel>       Min_circle_2_traits_2;
typedef typename Min_circle_2_traits_2::Circle             Optimisation_circle_2;
typedef typename CGAL::Min_circle_2<Min_circle_2_traits_2> Min_circle_2;

Min_circle_2* init_min_circle_2_from_list(bp::list& lst, bool random)
{
  auto begin = boost::python::stl_input_iterator<Point_2>(lst);
  auto end = boost::python::stl_input_iterator<Point_2>();
  return new Min_circle_2(begin, end, random);
}

void insert_list(Min_circle_2& mc, bp::list& lst)
{
  auto begin = boost::python::stl_input_iterator<Point_2>(lst);
  auto end = boost::python::stl_input_iterator<Point_2>();
  mc.insert(begin, end);
}

void export_bounding_volumes()
{
  using namespace boost::python;
  class_<Optimisation_circle_2>("Optimization_circle_2")
    .def(init<>())
    //.def("set", &Optimisation_circle_2::set)
    .def("center", &Optimisation_circle_2::center, return_internal_reference<>())
    .def("squared_radius", &Optimisation_circle_2::squared_radius, Kernel_return_value_policy())
    .def("is_empty", &Optimisation_circle_2::is_empty)
    .def("is_degenerate", &Optimisation_circle_2::is_degenerate)
    .def(self == self)
    .def(self != self)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;

  class_<Min_circle_2, boost::noncopyable>("Min_circle_2")
    .def(init<>())
    .def(init<const Point_2&>())
    .def(init<const Point_2&, const Point_2&>())
    .def(init<const Point_2&, const Point_2&, const Point_2&>())
    .def("__init__", make_constructor(&init_min_circle_2_from_list))
    .def("number_of_points", &Min_circle_2::number_of_points)
    .def("number_of_support_points", &Min_circle_2::number_of_support_points)
    .def("points", range<return_internal_reference<>>(&Min_circle_2::points_begin, &Min_circle_2::points_end))
    .def("support_points", range<return_internal_reference<>>(&Min_circle_2::support_points_begin, &Min_circle_2::support_points_end))
    .def("support_point", &Min_circle_2::support_point, return_internal_reference<>())
    .def("circle", &Min_circle_2::circle, return_value_policy<copy_const_reference>())
    .def("bounded_side", &Min_circle_2::bounded_side)
    .def("has_on_bounded_side", &Min_circle_2::has_on_bounded_side)
    .def("has_on_boundary", &Min_circle_2::has_on_boundary)
    .def("has_on_unbounded_side", &Min_circle_2::has_on_unbounded_side)
    .def("is_empty", &Min_circle_2::is_empty)
    .def("is_degenerate", &Min_circle_2::is_degenerate)
    .def<void (Min_circle_2::*) (const Point_2&)>("insert", &Min_circle_2::insert)
    .def("insert", &insert_list)
    .def("clear", &Min_circle_2::clear)
    .def("is_valid", &Min_circle_2::is_valid)
    ;
}
#endif
