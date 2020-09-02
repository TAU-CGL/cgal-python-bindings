// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

void set_left(Curve_2& c, Point_2& p) { c.set_left(p); }
void set_right(Curve_2& c, Point_2& p) { c.set_right(p); }

void export_arr_linear_traits()
{
  using namespace boost::python;
  class_<Curve_2>("Curve_2")
    .def(init<>())
    .def(init<Segment_2&>())
    .def(init<Ray_2&>())
    .def(init<Line_2&>())
    .def("source", &Curve_2::source, return_value_policy<copy_const_reference>())
    .def("target", &Curve_2::target, return_value_policy<copy_const_reference>())
    .def("line", &Curve_2::line)
    .def("is_vertical", &Curve_2::is_vertical)
    .def("is_segment", &Curve_2::is_segment)
    .def("segment", &Curve_2::segment)
    .def("is_ray", &Curve_2::ray)
    .def("is_line", &Curve_2::is_line)
    .def("line", &Curve_2::line)
    .def("supporting_line", &Curve_2::supporting_line, return_value_policy<copy_const_reference>())
    .def("left", &Curve_2::left, return_value_policy<copy_const_reference>())
    .def("right", &Curve_2::right, return_value_policy<copy_const_reference>())
    .def<void (Curve_2::*)()>("set_left", &Curve_2::set_left)
    .def("set_left", set_left)
    .def<void (Curve_2::*)()>("set_right", &Curve_2::set_right)
    .def("set_right", set_right)
    .def("is_directed_right", &Curve_2::is_directed_right)
    .def("is_in_x_range", &Curve_2::is_in_x_range)
    .def("is_in_y_range", &Curve_2::is_in_y_range)
    .def("is_degenerate", &Curve_2::is_degenerate)
    .def("bbox", &Curve_2::bbox)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;
}
