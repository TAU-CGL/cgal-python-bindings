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
typedef General_polygon_2::X_monotone_curve_2
  CS_traits_X_monotone_curve_2;
typedef General_polygon_2::Curve_iterator                  Curve_iterator;

static General_polygon_2* init_from_list(boost::python::list& lst)
{
  auto begin =
    boost::python::stl_input_iterator<CS_traits_X_monotone_curve_2>(lst);
  auto end = boost::python::stl_input_iterator<CS_traits_X_monotone_curve_2>();
  return new General_polygon_2(begin, end);
}

Curve_iterator curves_begin(General_polygon_2& p)
{
  return p.curves_begin();
}

Curve_iterator curves_end(General_polygon_2& p)
{
  return p.curves_end();
}

void export_general_polygon_2()
{
  using namespace boost::python;
  class_<General_polygon_2>("General_polygon_2")
    .def(init<>())
    .def(init<General_polygon_2>())
    .def("__init__", make_constructor(&init_from_list))
    .def("push_back", &General_polygon_2::push_back)
    .def("orientation", &General_polygon_2::orientation)
    .def("is_empty", &General_polygon_2::is_empty)
    .def("size", &General_polygon_2::size)
    .def("bbox", &General_polygon_2::bbox)
    .def("curves", range<return_internal_reference<>>(&curves_begin, &curves_end))
    .def("clear", &General_polygon_2::clear)
    .def("reverse_orientation", &General_polygon_2::reverse_orientation)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;

#if CGALPY_GEOMETRY_TRAITS != CGALPY_ARR_CIRCLE_SEGMENT_TRAITS

  typedef typename CGAL::Arr_circle_segment_traits_2<Kernel>::Point_2 CSPoint_2;
  typedef typename CGAL::Arr_circle_segment_traits_2<Kernel>::CoordNT CoordNT;

  class_<CoordNT>("CoordNT")
    .def(init<>())
    .def(init<CoordNT&>())
    .def(init<int&>())
    .def(init<CoordNT::NT&>())
    .def(init<int, int, int>())
    .def(init< CoordNT::NT, CoordNT::NT, CoordNT::ROOT>())
    .def<FT & (CoordNT::*)()>("a0", &CoordNT::a0, return_value_policy<copy_non_const_reference>())
    .def<FT & (CoordNT::*)()>("a1", &CoordNT::a1, return_value_policy<copy_non_const_reference>())
    .def<const FT & (CoordNT::*)() const>("root", &CoordNT::root, return_value_policy<copy_const_reference>())
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

  class_<CSPoint_2>("T_Point")
    .def(init<>())
    .def(init<FT&, FT&>())
    .def(init<CoordNT&, CoordNT&>())
    .def("x", &CSPoint_2::x, return_value_policy<copy_const_reference>())
    .def("y", &CSPoint_2::y, return_value_policy<copy_const_reference>())
    .def(self == self)
    .def(self != self)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;

  class_< CS_traits_X_monotone_curve_2 >("CS_traits_X_monotone_curve_2")
    .def(init<>())
    .def(init<Point_2&, Point_2&>())
    .def(init<Line_2&, CSPoint_2&, CSPoint_2&>())
    .def(init<Circle_2&, CSPoint_2&, CSPoint_2&, CGAL::Orientation>())
    .def("source", &CS_traits_X_monotone_curve_2::source, return_value_policy<copy_const_reference>())
    .def("target", &CS_traits_X_monotone_curve_2::target, return_value_policy<copy_const_reference>())
    .def("is_directed_right", &CS_traits_X_monotone_curve_2::is_directed_right)
    .def("left", &CS_traits_X_monotone_curve_2::left, return_value_policy<copy_const_reference>())
    .def("right", &CS_traits_X_monotone_curve_2::right, return_value_policy<copy_const_reference>())
    .def("orientation", &CS_traits_X_monotone_curve_2::orientation)
    .def("is_linear", &CS_traits_X_monotone_curve_2::is_linear)
    .def("is_circular", &CS_traits_X_monotone_curve_2::is_circular)
    .def("supporting_line", &CS_traits_X_monotone_curve_2::supporting_line)
    .def("supporting_circle", &CS_traits_X_monotone_curve_2::supporting_circle)
    .def("bbox", &CS_traits_X_monotone_curve_2::bbox)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;
#endif
}
#endif
