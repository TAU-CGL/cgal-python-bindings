// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

typedef typename Traits::CoordNT CoordNT;

void export_arr_circle_segment_traits()
{
  using namespace boost::python;

  class_<CoordNT>("CoordNT")
    .def(init<>())
    .def(init<CoordNT&>())
    .def(init<int&>())
    .def(init<CoordNT::NT&>())
    .def(init<int, int, int>())
    .def(init< CoordNT::NT, CoordNT::NT, CoordNT::ROOT>())
    .def<FT& (CoordNT::*)()>("a0", &CoordNT::a0, return_value_policy<copy_non_const_reference>())
    .def<FT& (CoordNT::*)()>("a1", &CoordNT::a1, return_value_policy<copy_non_const_reference>())
    .def<const FT& (CoordNT::*)() const>("root", &CoordNT::root, return_value_policy<copy_const_reference>())
    .def("is_extended", &CoordNT::is_extended, return_value_policy<copy_const_reference>())
    .def("simplify", &CoordNT::simplify)
    .def("is_zero", &CoordNT::is_zero)
    .def("sign", &CoordNT::sign)
    .def("abs", &CoordNT::abs)
    //.def("compare", &CoordNT::compare)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self * self)
    .def(self *= self)
    .def(self / self)
    .def(self /= self)
    ;

  class_<TPoint_2>("TPoint")
    .def(init<>())
    .def(init<FT&, FT&>())
    .def(init<CoordNT&, CoordNT&>())
    .def("x", &TPoint_2::x, return_value_policy<copy_const_reference>())
    .def("y", &TPoint_2::y, return_value_policy<copy_const_reference>())
    .def(self == self)
    .def(self != self)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;

  class_<X_monotone_curve_2>("X_monotone_curve_2")
    .def(init<>())
    .def(init<Point_2&, Point_2&>())
    .def(init<Line_2&, TPoint_2&, TPoint_2&>())
    .def(init<Circle_2&, TPoint_2&, TPoint_2&, CGAL::Orientation>())
    .def("source", &X_monotone_curve_2::source, return_value_policy<copy_const_reference>())
    .def("target", &X_monotone_curve_2::target, return_value_policy<copy_const_reference>())
    .def("is_directed_right", &X_monotone_curve_2::is_directed_right)
    .def("left", &X_monotone_curve_2::left, return_value_policy<copy_const_reference>())
    .def("right", &X_monotone_curve_2::right, return_value_policy<copy_const_reference>())
    .def("orientation", &X_monotone_curve_2::orientation)
    .def("is_linear", &X_monotone_curve_2::is_linear)
    .def("is_circular", &X_monotone_curve_2::is_circular)
    .def("supporting_line", &X_monotone_curve_2::supporting_line)
    .def("supporting_circle", &X_monotone_curve_2::supporting_circle)
    .def("bbox", &X_monotone_curve_2::bbox)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;

  class_<Curve_2>("Curve_2")
    .def(init<>())
    .def(init<Segment_2&>())
    .def(init<Point_2&, Point_2&>())
    .def(init<Line_2&, TPoint_2&, TPoint_2&>())
    .def(init<Circle_2&>())
    .def(init<Point_2&, FT&, CGAL::Orientation>())
    .def(init<Circle_2&, TPoint_2&, TPoint_2&>())
    .def(init<Point_2&, FT&, CGAL::Orientation, TPoint_2&, TPoint_2&>())
    .def(init<Point_2&, Point_2&, Point_2&>())
    .def("is_full", &Curve_2::is_full)
    .def("source", &Curve_2::source, return_value_policy<copy_const_reference>())
    .def("target", &Curve_2::target, return_value_policy<copy_const_reference>())
    .def("orientation", &Curve_2::orientation)
    .def("is_linear", &Curve_2::is_linear)
    .def("is_circular", &Curve_2::is_circular)
    .def("supporting_line", &Curve_2::supporting_line, return_value_policy<copy_const_reference>())
    .def("supporting_circle", &Curve_2::supporting_circle, return_value_policy<copy_const_reference>())
    ;
}
