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
#include "CGALPY/export_sqrt_extension.hpp"

namespace py = nanobind;

namespace aos2 {

double coordNT_to_double(Geometry_traits_2::CoordNT& c)
{ return CGAL::to_double(c); }

}

void export_arr_circle_segment_traits_2(py::module_& m) {
  using Gt = CGAL::Arr_circle_segment_traits_2<Kernel>;
  using Coord_nt = Gt::CoordNT;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Gt>(m, "Arr_circle_segment_traits_2")) return;

  if (! add_attr<Coord_nt>(m, "Coord_nt")) {
    py::class_<Coord_nt> cnc_c(m, "Coord_nt");
    export_sqrt_extension(cnc_c);
  }

  py::class_<Gt> traits_c(m, "Arr_circle_segment_traits_2");
  traits_c.def(py::init<>());
  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
    Aos_directional_x_monotone_traits_classes<Gt>
      m_aos_directional_x_monotone_traits_2_classes;
  } concepts;
  export_AosTraits_2<Gt>(traits_c, concepts);
  export_AosDirectionalXMonotoneTraits_2<Gt>(traits_c, concepts);

  auto& pnt_c = *(concepts.m_aos_basic_traits_2_classes.m_point_2);
  pnt_c.def(py::init<FT&, FT&>())
    .def(py::init<Coord_nt&, Coord_nt&>())
    .def(py::init<FT&, Coord_nt&>())
    .def(py::init<Coord_nt&, FT&>())
    .def(py::init<int, int>())
    .def("x", &aos2::Point_2::x)
    .def("y", &aos2::Point_2::y)
    .def(py::self == py::self)
    .def(py::self != py::self)
    // .setattr("__hash__", py::object()) NB
    ;

  add_insertion(pnt_c, "__str__");
  add_insertion(pnt_c, "__repr__");

  //! \todo Handle the functions that return reference-counted objects.
  auto& xcv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
  xcv_c
    // .def(py::init_implicit<Segment_2&>())
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Line_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Circle_2&, aos2::Point_2&, aos2::Point_2&, CGAL::Orientation>())
    .def("source", &aos2::X_monotone_curve_2::source, ri)
    .def("target", &aos2::X_monotone_curve_2::target, ri)
    .def("is_directed_right", &aos2::X_monotone_curve_2::is_directed_right)
    .def("left", &aos2::X_monotone_curve_2::left, ri)
    .def("right", &aos2::X_monotone_curve_2::right, ri)
    .def("orientation", &aos2::X_monotone_curve_2::orientation)
    .def("is_linear", &aos2::X_monotone_curve_2::is_linear)
    .def("is_circular", &aos2::X_monotone_curve_2::is_circular)
    .def("supporting_line", &aos2::X_monotone_curve_2::supporting_line, ri)
    .def("supporting_circle", &aos2::X_monotone_curve_2::supporting_circle, ri)
    .def("bbox", &aos2::X_monotone_curve_2::bbox)
    ;

  add_insertion(xcv_c, "__str__");
  add_insertion(xcv_c, "__repr__");

  //! \todo Handle the functions that return reference-counted objects.
  auto& cv_c = *(concepts.m_aos_traits_2_classes.m_curve_2);
  cv_c.def(py::init_implicit<Segment_2&>())
    .def(py::init<Point_2&, Point_2&>())
    .def(py::init<Line_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init_implicit<Circle_2&>())
    .def(py::init<Point_2&, FT&>())
    .def(py::init<Point_2&, FT&, CGAL::Orientation>())
    .def(py::init<Circle_2&, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Point_2&, FT&, CGAL::Orientation, aos2::Point_2&, aos2::Point_2&>())
    // TODO: error below: non-constant-expression cannot be narrowed from type 'int' to 'NT' (aka 'double')
    // .def(py::init<Point_2&, int, CGAL::Orientation, aos2::Point_2&, aos2::Point_2&>())
    .def(py::init<Point_2&, Point_2&, Point_2&>())
    .def("is_circular", &aos2::Curve_2::is_circular)
    .def("is_full", &aos2::Curve_2::is_full)
    .def("is_linear", &aos2::Curve_2::is_linear)
    .def("orientation", &aos2::Curve_2::orientation)
    .def("source", &aos2::Curve_2::source, ri)
    .def("supporting_line", &aos2::Curve_2::supporting_line, ri)
    .def("supporting_circle", &aos2::Curve_2::supporting_circle, ri)
    .def("target", &aos2::Curve_2::target, ri)
    ;

  add_attr<Gt::Rational_point_2>(traits_c, "Rational_point_2");
  add_attr<Gt::Rational_segment_2>(traits_c, "Rational_segment_2");
  add_attr<Gt::Rational_circle_2>(traits_c, "Rational_circle_2");
  add_attr<Coord_nt>(traits_c, "Coord_nt");
}
