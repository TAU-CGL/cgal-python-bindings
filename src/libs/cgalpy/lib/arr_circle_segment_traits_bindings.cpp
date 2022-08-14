// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"

namespace py = nanobind;

typedef typename aos2::Geometry_traits_2::CoordNT CoordNT;

static double coordNT_to_double(CoordNT& c) { return CGAL::to_double(c); }

py::class_<aos2::Geometry_traits_2> export_arr_circle_segment_traits() {
  typedef aos2::Geometry_traits_2       GT;

  py::class_<CoordNT>("CoordNT")
    .def(py::init<>())
    .def(py::init<CoordNT&>())
    .def(py::init<int&>())
    .def(py::init<CoordNT::NT&>())
    .def(py::init<int, int, int>())
    .def(py::init< CoordNT::NT, CoordNT::NT, CoordNT::ROOT>())
    .def<FT& (CoordNT::*)()>("a0", &CoordNT::a0)
    .def<FT& (CoordNT::*)()>("a1", &CoordNT::a1)
    .def<const FT& (CoordNT::*)() const>("root", &CoordNT::root)
    .def("is_extended", &CoordNT::is_extended)
    .def("simplify", &CoordNT::simplify)
    .def("is_zero", &CoordNT::is_zero)
    .def("sign", &CoordNT::sign)
    .def("abs", &CoordNT::abs)
    .def("to_double", &coordNT_to_double)
    //.def("compare", &CoordNT::compare)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self * py::self)
    .def(py::self *= py::self)
    .def(py::self / py::self)
    .def(py::self /= py::self)
    // .def(py::self_ns::str(py::self_ns::self)) NB
    // .def(py::self_ns::repr(py::self_ns::self)) NB
    ;

  auto traits = py::class_<GT>("Geometry_traits_2")
    .def(py::init<>());
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
    .def(py::init<FT&, FT&>())
    .def(py::init<CoordNT&, CoordNT&>())
    .def(py::init<double, double>())
    .def("x", &aos2::Point_2::x)
    .def("y", &aos2::Point_2::y)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .setattr("__hash__", py::object())
    // .def(py::self_ns::str(py::self_ns::self)) NB
    // .def(py::self_ns::repr(py::self_ns::self)) NB
    ;

  auto& xcv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_co
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Line_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Circle_2&, aos2::Point_2&, aos2::Point_2&, CGAL::Orientation>())
    .def("source", &aos2::X_monotone_curve_2::source)
    .def("target", &aos2::X_monotone_curve_2::target)
    .def("is_directed_right", &aos2::X_monotone_curve_2::is_directed_right)
    .def("left", &aos2::X_monotone_curve_2::left)
    .def("right", &aos2::X_monotone_curve_2::right)
    .def("orientation", &aos2::X_monotone_curve_2::orientation)
    .def("is_linear", &aos2::X_monotone_curve_2::is_linear)
    .def("is_circular", &aos2::X_monotone_curve_2::is_circular)
    .def("supporting_line", &aos2::X_monotone_curve_2::supporting_line)
    .def("supporting_circle", &aos2::X_monotone_curve_2::supporting_circle)
    .def("bbox", &aos2::X_monotone_curve_2::bbox)
    // .def(py::self_ns::str(py::self_ns::self)) NB
    // .def(py::self_ns::repr(py::self_ns::self)) NB
    ;

  auto& cv_co = *(concepts.m_traits_classes.m_curve_2);
  cv_co
    .def(py::init<Segment_2&>())
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Line_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Circle_2&>())
    .def(py::init<Point_2&, FT&, CGAL::Orientation>())
    .def(py::init<Circle_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Point_2&, FT&, CGAL::Orientation, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Point_2&, Point_2&, Point_2&>())
    .def("is_full", &aos2::Curve_2::is_full)
    .def("source", &aos2::Curve_2::source)
    .def("target", &aos2::Curve_2::target)
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
