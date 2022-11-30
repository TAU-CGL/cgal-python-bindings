// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Arr_circle_segment_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

namespace aos2 {

double coordNT_to_double(Geometry_traits_2::CoordNT& c)
{ return CGAL::to_double(c); }

}

py::object export_arr_circle_segment_traits(py::module_& m) {
  using GT = CGAL::Arr_circle_segment_traits_2<Kernel>;
  using CoordNT = GT::CoordNT;

  py::class_<CoordNT> cnt_c(m, "CoordNT");
  cnt_c.def(py::init<>())
    .def(py::init<CoordNT&>())
    .def(py::init<int&>())
    .def(py::init<CoordNT::NT&>())
    .def(py::init<int, int, int>())
    .def(py::init<CoordNT::NT, CoordNT::NT, CoordNT::ROOT>())
    .def(py::init<FT, FT, int>())
    .def("a0", py::overload_cast<>(&CoordNT::a0))
    .def("a1", py::overload_cast<>(&CoordNT::a1))
    .def("root", py::overload_cast<>(&CoordNT::root, py::const_))
    .def("is_extended", &CoordNT::is_extended)
    .def("simplify", &CoordNT::simplify)
    .def("is_zero", &CoordNT::is_zero)
    .def("sign", &CoordNT::sign)
    .def("abs", &CoordNT::abs)
    .def("to_double", &aos2::coordNT_to_double)
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
    ;

  add_insertion(cnt_c, "__str__");
  add_insertion(cnt_c, "__repr__");

  py::class_<GT> traits_c(m, "Arr_circle_segment_traits_2");
  traits_c.def(py::init<>());
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_directional_x_monotone_traits_classes<GT>
      m_directional_x_monotone_traits_classes;
  } concepts;
  export_AosTraits_2<GT>(traits_c, concepts);
  export_AosDirectionalXMonotoneTraits_2<GT>(traits_c, concepts);

  auto& pnt_c = *(concepts.m_basic_traits_classes.m_point_2);
  pnt_c.def(py::init<FT&, FT&>())
    .def(py::init<CoordNT&, CoordNT&>())
    .def(py::init<FT&, CoordNT&>())
    .def(py::init<CoordNT&, FT&>())
    .def(py::init<int, int>())
    .def("x", &aos2::Point_2::x)
    .def("y", &aos2::Point_2::y)
    .def(py::self == py::self)
    .def(py::self != py::self)
    // .setattr("__hash__", py::object()) NB
    ;

  add_insertion(pnt_c, "__str__");
  add_insertion(pnt_c, "__repr__");

  auto& xcv_c = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_c
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
    ;

  add_insertion(xcv_c, "__str__");
  add_insertion(xcv_c, "__repr__");

  auto& cv_c = *(concepts.m_traits_classes.m_curve_2);
  cv_c.def(py::init<Segment_2&>())
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Line_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Circle_2&>())
    .def(py::init<Point_2&, FT&>())
    .def(py::init<Point_2&, FT&, CGAL::Orientation>())
    .def(py::init<Circle_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Point_2&, FT&, CGAL::Orientation, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Point_2&, int, CGAL::Orientation, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Point_2&, Point_2&, Point_2&>())
    .def("is_full", &aos2::Curve_2::is_full)
    .def("source", &aos2::Curve_2::source)
    .def("target", &aos2::Curve_2::target)
    .def("orientation", &aos2::Curve_2::orientation)
    .def("is_linear", &aos2::Curve_2::is_linear)
    .def("is_circular", &aos2::Curve_2::is_circular)
    .def("supporting_line", &aos2::Curve_2::supporting_line)
    .def("supporting_circle", &aos2::Curve_2::supporting_circle)
    ;

  traits_c.attr("CoordNT") = cnt_c;

  return traits_c;
}
