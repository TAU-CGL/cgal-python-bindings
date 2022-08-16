// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <unordered_set>
#include <any>

#include <boost/assert.hpp>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_linear_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosLandmarkTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosOpenBoundaryTraits_2.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_landmark_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_open_boundary_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"

namespace py = nanobind;

typedef typename aos2::Geometry_traits_2::Curve_2 Curve_2;
using overload = void (Curve_2::*)();

void set_left(Curve_2& c, Point_2& p) { c.set_left(p); }

void set_right(Curve_2& c, Point_2& p) { c.set_right(p); }

py::object export_arr_linear_traits(py::module_& m) {
  using GT = CGAL::Arr_linear_traits_2<Kernel>;

  auto traits = py::class_<GT>(m, "Arr_linear_traits_2");
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_landmark_traits_classes<GT> m_landmark_traits_classes;
    Aos_open_boundary_traits_classes<GT> m_open_boundary_traits_classes;
    Aos_approximate_traits_classes<GT> m_approximate_traits_classes;
    Aos_construct_x_monotone_curve_traits_classes<GT>
      m_construct_x_monotone_curve_traits_classes;
  };
  Concepts concepts;
  export_AosTraits_2<GT, Copy_const_reference>(traits, concepts);
  export_AosLandmarkTraits_2<GT, Copy_const_reference>(traits, concepts);
  export_AosOpenBoundaryTraits_2<GT, Copy_const_reference>(traits, concepts);

  auto& cv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  cv_co
    .def(py::init<Segment_2&>())
    .def(py::init<Ray_2&>())
    .def(py::init<Line_2&>())
    .def("source", &Curve_2::source)
    .def("target", &Curve_2::target)
    .def("line", &Curve_2::line)
    .def("is_vertical", &Curve_2::is_vertical)
    .def("is_segment", &Curve_2::is_segment)
    .def("segment", &Curve_2::segment)
    .def("is_ray", &Curve_2::ray)
    .def("is_line", &Curve_2::is_line)
    .def("line", &Curve_2::line)
    .def("supporting_line", &Curve_2::supporting_line)
    .def("left", &Curve_2::left)
    .def("right", &Curve_2::right)
    .def("set_left", static_cast<overload>(&Curve_2::set_left))
    .def("set_left", set_left)
    .def("set_right", static_cast<overload>(&Curve_2::set_right))
    .def("set_right", set_right)
    .def("is_directed_right", &Curve_2::is_directed_right)
    .def("is_in_x_range", &Curve_2::is_in_x_range)
    .def("is_in_y_range", &Curve_2::is_in_y_range)
    .def("is_degenerate", &Curve_2::is_degenerate)
    .def("bbox", &Curve_2::bbox)
    // .def(py::self_ns::str(py::self_ns::self)) NB
    // .def(py::self_ns::repr(py::self_ns::self)) NB
    ;

  return traits;
}
