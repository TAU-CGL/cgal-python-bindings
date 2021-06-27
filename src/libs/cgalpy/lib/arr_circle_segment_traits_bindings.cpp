// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace bp = boost::python;

typedef typename Traits::CoordNT CoordNT;

static double coordNT_to_double(CoordNT& c) { return CGAL::to_double(c); }

void export_arr_circle_segment_traits() {
  bp::scope traits_scope = bp::class_<Traits>("Traits")
    .def(bp::init<>())
    ;

  bp::class_<CoordNT>("CoordNT")
    .def(bp::init<>())
    .def(bp::init<CoordNT&>())
    .def(bp::init<int&>())
    .def(bp::init<CoordNT::NT&>())
    .def(bp::init<int, int, int>())
    .def(bp::init< CoordNT::NT, CoordNT::NT, CoordNT::ROOT>())
    .def<FT& (CoordNT::*)()>("a0", &CoordNT::a0, bp::return_value_policy<bp::copy_non_const_reference>())
    .def<FT& (CoordNT::*)()>("a1", &CoordNT::a1, bp::return_value_policy<bp::copy_non_const_reference>())
    .def<const FT& (CoordNT::*)() const>("root", &CoordNT::root, bp::return_value_policy<bp::copy_const_reference>())
    .def("is_extended", &CoordNT::is_extended, bp::return_value_policy<bp::copy_const_reference>())
    .def("simplify", &CoordNT::simplify)
    .def("is_zero", &CoordNT::is_zero)
    .def("sign", &CoordNT::sign)
    .def("abs", &CoordNT::abs)
    .def("to_double", &coordNT_to_double)
    //.def("compare", &CoordNT::compare)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self != bp::self)
    .def(bp::self < bp::self)
    .def(bp::self > bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(bp::self * bp::self)
    .def(bp::self *= bp::self)
    .def(bp::self / bp::self)
    .def(bp::self /= bp::self)
    ;

  bp::class_<TPoint_2>("Point_2")
    .def(bp::init<>())
    .def(bp::init<FT&, FT&>())
    .def(bp::init<CoordNT&, CoordNT&>())
    .def("x", &TPoint_2::x, bp::return_value_policy<bp::copy_const_reference>())
    .def("y", &TPoint_2::y, bp::return_value_policy<bp::copy_const_reference>())
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .setattr("__hash__", bp::object());
    ;

  bp::class_<X_monotone_curve_2>("X_monotone_curve_2")
    .def(bp::init<>())
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Line_2&, TPoint_2&, TPoint_2&>())
    .def(bp::init<Circle_2&, TPoint_2&, TPoint_2&, CGAL::Orientation>())
    .def("source", &X_monotone_curve_2::source, bp::return_value_policy<bp::copy_const_reference>())
    .def("target", &X_monotone_curve_2::target, bp::return_value_policy<bp::copy_const_reference>())
    .def("is_directed_right", &X_monotone_curve_2::is_directed_right)
    .def("left", &X_monotone_curve_2::left, bp::return_value_policy<bp::copy_const_reference>())
    .def("right", &X_monotone_curve_2::right, bp::return_value_policy<bp::copy_const_reference>())
    .def("orientation", &X_monotone_curve_2::orientation)
    .def("is_linear", &X_monotone_curve_2::is_linear)
    .def("is_circular", &X_monotone_curve_2::is_circular)
    .def("supporting_line", &X_monotone_curve_2::supporting_line)
    .def("supporting_circle", &X_monotone_curve_2::supporting_circle)
    .def("bbox", &X_monotone_curve_2::bbox)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    ;

  bp::class_<Curve_2>("Curve_2")
    .def(bp::init<>())
    .def(bp::init<Segment_2&>())
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Line_2&, TPoint_2&, TPoint_2&>())
    .def(bp::init<Circle_2&>())
    .def(bp::init<Point_2&, FT&, CGAL::Orientation>())
    .def(bp::init<Circle_2&, TPoint_2&, TPoint_2&>())
    .def(bp::init<Point_2&, FT&, CGAL::Orientation, TPoint_2&, TPoint_2&>())
    .def(bp::init<Point_2&, Point_2&, Point_2&>())
    .def("is_full", &Curve_2::is_full)
    .def("source", &Curve_2::source, bp::return_value_policy<bp::copy_const_reference>())
    .def("target", &Curve_2::target, bp::return_value_policy<bp::copy_const_reference>())
    .def("orientation", &Curve_2::orientation)
    .def("is_linear", &Curve_2::is_linear)
    .def("is_circular", &Curve_2::is_circular)
    .def("supporting_line", &Curve_2::supporting_line, bp::return_value_policy<bp::copy_const_reference>())
    .def("supporting_circle", &Curve_2::supporting_circle, bp::return_value_policy<bp::copy_const_reference>())
    ;
}
