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
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

namespace cgalpy {
namespace aos2 {

double coordNT_to_double(Geometry_traits_2::CoordNT& c)
{ return CGAL::to_double(c); }

}
} // namespace cgalpy

void export_arr_circle_segment_traits_2(py::module_& m) {
  using Gt = CGAL::Arr_circle_segment_traits_2<Kernel>;
  using Coord_nt = Gt::CoordNT;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Gt>(m, "Arr_circle_segment_traits_2")) return;

  if (! add_attr<Coord_nt>(m, "CoordNT")) {
    py::class_<Coord_nt> cnc_c(m, "CoordNT");
    export_sqrt_extension(cnc_c);
  }

  py::class_<Gt> traits_c(m, "Arr_circle_segment_traits_2",
                            aos2_doc::Arr_circle_segment_traits_2_class);
  traits_c.def(py::init<>(),
               "Construct a default circle-segment traits object.");
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
  pnt_c.def(py::init<FT&, FT&>(),
            py::arg("x"), py::arg("y"),
            aos2_doc::Arr_circle_segment_traits_2_Point_2_Point_2_1)
    .def(py::init<Coord_nt&, Coord_nt&>(),
         py::arg("x"), py::arg("y"),
         aos2_doc::Arr_circle_segment_traits_2_Point_2_Point_2_2)
    .def(py::init<FT&, Coord_nt&>(),
         py::arg("x"), py::arg("y"),
         aos2_doc::Arr_circle_segment_traits_2_Point_2_Point_2_2)
    .def(py::init<Coord_nt&, FT&>(),
         py::arg("x"), py::arg("y"),
         aos2_doc::Arr_circle_segment_traits_2_Point_2_Point_2_2)
    .def(py::init<int, int>(),
         py::arg("x"), py::arg("y"),
         aos2_doc::Arr_circle_segment_traits_2_Point_2_Point_2_1)
    .def("x", &cgalpy::aos2::Point_2::x,
         aos2_doc::Arr_circle_segment_traits_2_Point_2_x)
    .def("y", &cgalpy::aos2::Point_2::y,
         aos2_doc::Arr_circle_segment_traits_2_Point_2_y)
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
    .def(py::init<Point_2&, Point_2&>(),
         py::arg("source"), py::arg("target"),
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_X_monotone_curve_2)
    .def(py::init<Line_2&, cgalpy::aos2::Point_2&, cgalpy::aos2::Point_2&>(),
         py::arg("line"), py::arg("source"), py::arg("target"),
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_X_monotone_curve_2_1)
    .def(py::init<Circle_2&, cgalpy::aos2::Point_2&, cgalpy::aos2::Point_2&, CGAL::Orientation>(),
         py::arg("circle"), py::arg("source"), py::arg("target"), py::arg("orientation"),
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_X_monotone_curve_2_2)
    .def("source", &cgalpy::aos2::X_monotone_curve_2::source, ri,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_source)
    .def("target", &cgalpy::aos2::X_monotone_curve_2::target, ri,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_target)
    .def("is_directed_right", &cgalpy::aos2::X_monotone_curve_2::is_directed_right,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_is_directed_right)
    .def("left", &cgalpy::aos2::X_monotone_curve_2::left, ri,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_left)
    .def("right", &cgalpy::aos2::X_monotone_curve_2::right, ri,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_right)
    .def("orientation", &cgalpy::aos2::X_monotone_curve_2::orientation,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_orientation)
    .def("is_linear", &cgalpy::aos2::X_monotone_curve_2::is_linear,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_is_linear)
    .def("is_circular", &cgalpy::aos2::X_monotone_curve_2::is_circular,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_is_circular)
    .def("supporting_line", &cgalpy::aos2::X_monotone_curve_2::supporting_line, ri,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_supporting_line)
    .def("supporting_circle", &cgalpy::aos2::X_monotone_curve_2::supporting_circle, ri,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_supporting_circle)
    .def("bbox", &cgalpy::aos2::X_monotone_curve_2::bbox,
         aos2_doc::Arr_circle_segment_traits_2_X_monotone_curve_2_bbox)
    ;

  add_insertion(xcv_c, "__str__");
  add_insertion(xcv_c, "__repr__");

  //! \todo Handle the functions that return reference-counted objects.
  auto& cv_c = *(concepts.m_aos_traits_2_classes.m_curve_2);
  cv_c.def(py::init_implicit<Segment_2&>(),
           py::arg("segment"),
           aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2)
    .def(py::init<Point_2&, Point_2&>(),
         py::arg("source"), py::arg("target"),
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2_1)
    .def(py::init<Line_2&, cgalpy::aos2::Point_2&, cgalpy::aos2::Point_2&>(),
         py::arg("line"), py::arg("source"), py::arg("target"),
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2_2)
    .def(py::init_implicit<Circle_2&>(),
         py::arg("circle"),
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2_3)
    .def(py::init<Point_2&, FT&>(),
         py::arg("center"), py::arg("radius"),
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2_4)
    .def(py::init<Point_2&, FT&, CGAL::Orientation>(),
         py::arg("center"), py::arg("radius"), py::arg("orientation"),
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2_4)
    .def(py::init<Circle_2&, cgalpy::aos2::Point_2&, cgalpy::aos2::Point_2&>(),
         py::arg("circle"), py::arg("source"), py::arg("target"),
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2_5)
    .def(py::init<Point_2&, FT&, CGAL::Orientation, cgalpy::aos2::Point_2&, cgalpy::aos2::Point_2&>(),
         py::arg("center"), py::arg("radius"), py::arg("orientation"),
         py::arg("source"), py::arg("target"),
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2_6)
    // TODO: error below: non-constant-expression cannot be narrowed from type 'int' to 'NT' (aka 'double')
    // .def(py::init<Point_2&, int, CGAL::Orientation, cgalpy::aos2::Point_2&, cgalpy::aos2::Point_2&>())
    .def(py::init<Point_2&, Point_2&, Point_2&>(),
         py::arg("source"), py::arg("mid"), py::arg("target"),
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_Curve_2_7)
    .def("is_circular", &cgalpy::aos2::Curve_2::is_circular,
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_is_circular)
    .def("is_full", &cgalpy::aos2::Curve_2::is_full,
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_is_full)
    .def("is_linear", &cgalpy::aos2::Curve_2::is_linear,
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_is_linear)
    .def("orientation", &cgalpy::aos2::Curve_2::orientation,
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_orientation)
    .def("source", &cgalpy::aos2::Curve_2::source, ri,
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_source)
    .def("supporting_line", &cgalpy::aos2::Curve_2::supporting_line, ri,
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_supporting_line)
    .def("supporting_circle", &cgalpy::aos2::Curve_2::supporting_circle, ri,
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_supporting_circle)
    .def("target", &cgalpy::aos2::Curve_2::target, ri,
         aos2_doc::Arr_circle_segment_traits_2_Curve_2_target)
    ;

  add_insertion(cv_c, "__str__");
  add_insertion(cv_c, "__repr__");

  add_attr<Gt::Rational_point_2>(traits_c, "Rational_point_2");
  add_attr<Gt::Rational_segment_2>(traits_c, "Rational_segment_2");
  add_attr<Gt::Rational_circle_2>(traits_c, "Rational_circle_2");
  add_attr<Coord_nt>(traits_c, "CoordNT");
}
