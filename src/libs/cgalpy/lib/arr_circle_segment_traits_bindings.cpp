// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"

namespace bp = boost::python;

typedef typename aos2::Geometry_traits_2::CoordNT CoordNT;

static double coordNT_to_double(CoordNT& c) { return CGAL::to_double(c); }

bp::class_<aos2::Geometry_traits_2> export_arr_circle_segment_traits() {
  typedef aos2::Geometry_traits_2       GT;

  bp::class_<CoordNT>("CoordNT")
    .def(bp::init<>())
    .def(bp::init<CoordNT&>())
    .def(bp::init<int&>())
    .def(bp::init<CoordNT::NT&>())
    .def(bp::init<int, int, int>())
    .def(bp::init< CoordNT::NT, CoordNT::NT, CoordNT::ROOT>())
    .def<FT& (CoordNT::*)()>("a0", &CoordNT::a0, Copy_non_const_reference())
    .def<FT& (CoordNT::*)()>("a1", &CoordNT::a1, Copy_non_const_reference())
    .def<const FT& (CoordNT::*)() const>("root", &CoordNT::root, Copy_const_reference())
    .def("is_extended", &CoordNT::is_extended, Copy_const_reference())
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

  auto traits = bp::class_<GT>("Geometry_traits_2")
    .def(bp::init<>());
  bp::scope traits_scope(traits);
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_directional_x_monotone_traits_classes<GT> m_directional_x_monotone_traits_classes;
  } concepts;
  export_AosTraits_2<GT, Return_by_value>(traits, concepts);
  export_AosDirectionalXMonotoneTraits_2<GT, Return_by_value>(traits, concepts);


  auto& p2_co = *(concepts.m_basic_traits_classes.m_point_2);
  p2_co
    .def(bp::init<FT&, FT&>())
    .def(bp::init<CoordNT&, CoordNT&>())
    .def(bp::init<double, double>())
    .def("x", &aos2::Point_2::x, Copy_const_reference())
    .def("y", &aos2::Point_2::y, Copy_const_reference())
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .setattr("__hash__", bp::object());
    ;

  auto& xcv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_co
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Line_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(bp::init<Circle_2&, aos2::Point_2&, aos2::Point_2&, CGAL::Orientation>())
    .def("source", &aos2::X_monotone_curve_2::source, Copy_const_reference())
    .def("target", &aos2::X_monotone_curve_2::target, Copy_const_reference())
    .def("is_directed_right", &aos2::X_monotone_curve_2::is_directed_right)
    .def("left", &aos2::X_monotone_curve_2::left, Copy_const_reference())
    .def("right", &aos2::X_monotone_curve_2::right, Copy_const_reference())
    .def("orientation", &aos2::X_monotone_curve_2::orientation)
    .def("is_linear", &aos2::X_monotone_curve_2::is_linear)
    .def("is_circular", &aos2::X_monotone_curve_2::is_circular)
    .def("supporting_line", &aos2::X_monotone_curve_2::supporting_line)
    .def("supporting_circle", &aos2::X_monotone_curve_2::supporting_circle)
    .def("bbox", &aos2::X_monotone_curve_2::bbox)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    ;

  auto& cv_co = *(concepts.m_traits_classes.m_curve_2);
  cv_co
    .def(bp::init<Segment_2&>())
    .def(bp::init<Point_2&, Point_2&>())
    .def(bp::init<Line_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(bp::init<Circle_2&>())
    .def(bp::init<Point_2&, FT&, CGAL::Orientation>())
    .def(bp::init<Circle_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(bp::init<Point_2&, FT&, CGAL::Orientation, aos2::Point_2&, aos2::Point_2&>())
    .def(bp::init<Point_2&, Point_2&, Point_2&>())
    .def("is_full", &aos2::Curve_2::is_full)
    .def("source", &aos2::Curve_2::source, Copy_const_reference())
    .def("target", &aos2::Curve_2::target, Copy_const_reference())
    .def("orientation", &aos2::Curve_2::orientation)
    .def("is_linear", &aos2::Curve_2::is_linear)
    .def("is_circular", &aos2::Curve_2::is_circular)
    .def("supporting_line", &aos2::Curve_2::supporting_line,
         Copy_const_reference())
    .def("supporting_circle", &aos2::Curve_2::supporting_circle,
         Copy_const_reference())
    ;

  return traits;
}
