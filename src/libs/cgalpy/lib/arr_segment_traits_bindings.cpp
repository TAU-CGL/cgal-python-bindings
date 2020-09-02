// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

void set_left(Curve_2& c, Point_2& p) { c.set_left(p); }
void set_right(Curve_2& c, Point_2& p) { c.set_right(p); }

Segment_2 to_segment(Curve_2& c) { return Segment_2(c); }

void export_arr_segment_traits()
{
  using namespace boost::python;
  class_<Curve_2>("Curve_2")
    .def(init<>())
    .def(init<Segment_2&>())
    .def(init<Point_2&, Point_2&>())
    .def(init<Line_2&, Point_2&, Point_2&>())
    .def("source", &Curve_2::source, return_value_policy<copy_const_reference>())
    .def("target", &Curve_2::target, return_value_policy<copy_const_reference>())
    .def("line", &Curve_2::line, return_value_policy<copy_const_reference>())
    .def("is_vertical", &Curve_2::is_vertical)
    .def("flip", &Curve_2::flip)
    .def("left", &Curve_2::left, return_value_policy<copy_const_reference>())
    .def("right", &Curve_2::right, return_value_policy<copy_const_reference>())
    .def("set_left", &set_left)
    .def("set_right", &set_right)
    .def("is_directed_right", &Curve_2::is_directed_right)
    .def("is_in_x_range", &Curve_2::is_in_x_range)
    .def("is_in_y_range", &Curve_2::is_in_y_range)
    .def("bbox", &Curve_2::bbox)
    .def("segment", &to_segment)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
  ;
}
