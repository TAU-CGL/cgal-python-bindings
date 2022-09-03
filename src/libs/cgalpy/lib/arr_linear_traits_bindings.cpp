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
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

typedef typename aos2::Geometry_traits_2::Curve_2 Curve_2;

void set_left(Curve_2& c, Point_2& p) { c.set_left(p); }

void set_right(Curve_2& c, Point_2& p) { c.set_right(p); }

py::object export_arr_linear_traits(py::module_& m) {
  using GT = CGAL::Arr_linear_traits_2<Kernel>;
  using Pnt = GT::Point_2;
  using Cv = GT::Curve_2;
  using overload = void (Cv::*)();
  using Segment = GT::Segment_2;
  using Ray = GT::Ray_2;
  using Line = GT::Line_2;

  py::class_<GT> traits_c(m, "Arr_linear_traits_2");
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
  export_AosTraits_2<GT>(traits_c, concepts);
  export_AosLandmarkTraits_2<GT>(traits_c, concepts);
  export_AosOpenBoundaryTraits_2<GT>(traits_c, concepts);

  auto& cv_c = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  cv_c.def(py::init<Segment&>())
    .def(py::init<Ray&>())
    .def(py::init<Line&>())
    .def(py::init<Pnt&, Pnt&>())
    .def("source", &Cv::source)
    .def("target", &Cv::target)
    .def("line", &Cv::line)
    .def("is_vertical", &Cv::is_vertical)
    .def("is_segment", &Cv::is_segment)
    .def("segment", &Cv::segment)
    .def("is_ray", &Cv::ray)
    .def("is_line", &Cv::is_line)
    .def("line", &Cv::line)
    .def("supporting_line", &Cv::supporting_line)
    .def("left", &Cv::left)
    .def("right", &Cv::right)
    .def("set_left", static_cast<overload>(&Cv::set_left))
    .def("set_left", set_left)
    .def("set_right", static_cast<overload>(&Cv::set_right))
    .def("set_right", set_right)
    .def("is_directed_right", &Cv::is_directed_right)
    .def("is_in_x_range", &Cv::is_in_x_range)
    .def("is_in_y_range", &Cv::is_in_y_range)
    .def("is_degenerate", &Cv::is_degenerate)
    .def("bbox", &Cv::bbox)
    ;

  add_attr<Segment>(traits_c, "Segment_2");
  add_attr<Ray>(traits_c, "Ray_2");
  add_attr<Line>(traits_c, "Line_2");

  add_insertion(cv_c, "__str__");
  add_insertion(cv_c, "__repr__");

  return traits_c;
}
