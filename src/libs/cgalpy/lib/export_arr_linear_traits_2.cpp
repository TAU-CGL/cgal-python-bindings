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

void export_arr_linear_traits_2(py::module_& m) {
  using Gt = CGAL::Arr_linear_traits_2<Kernel>;
  using Pnt = Gt::Point_2;
  using Cv = Gt::Curve_2;
  using overload = void (Cv::*)();
  using Segment = Gt::Segment_2;
  using Ray = Gt::Ray_2;
  using Line = Gt::Line_2;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Gt>(m, "Arr_linear_traits_2")) return;

  py::class_<Gt, Kernel> traits_c(m, "Arr_linear_traits_2");
  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
    Aos_landmark_traits_classes<Gt> m_aos_landmark_traits_2_classes;
    Aos_open_boundary_traits_classes<Gt> m_aos_open_boundary_traits_2_classes;
    Aos_approximate_traits_classes<Gt> m_aos_approximate_traits_2_classes;
    Aos_construct_x_monotone_curve_traits_classes<Gt>
      m_aos_construct_x_monotone_curve_traits_2_classes;
  };
  Concepts concepts;
  export_AosTraits_2<Gt>(traits_c, concepts);
  export_AosLandmarkTraits_2<Gt>(traits_c, concepts);
  export_AosOpenBoundaryTraits_2<Gt>(traits_c, concepts);

  auto& cv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
  cv_c.def(py::init_implicit<const Segment&>())
    .def(py::init_implicit<const Ray&>())
    .def(py::init_implicit<const Line&>())
    .def(py::init<const Pnt&, const Pnt&>())
    .def("source", &Cv::source, ri)
    .def("target", &Cv::target, ri)
    .def("is_segment", &Cv::is_segment)
    .def("segment", &Cv::segment)
    .def("is_ray", &Cv::ray)
    .def("ray", &Cv::ray)
    .def("is_line", &Cv::is_line)
    .def("line", &Cv::line)
    .def("supporting_line", &Cv::supporting_line, ri)
    .def("is_vertical", &Cv::is_vertical)
    .def("left", &Cv::left, ri)
    .def("right", &Cv::right, ri)
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
}
