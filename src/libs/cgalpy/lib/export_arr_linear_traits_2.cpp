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

#include "cgalpy/arrangement_on_surface_2_types.hpp"
#include "cgalpy/aos_2_concepts/export_AosTraits_2.hpp"
#include "cgalpy/aos_2_concepts/export_AosLandmarkTraits_2.hpp"
#include "cgalpy/aos_2_concepts/export_AosOpenBoundaryTraits_2.hpp"

#include "cgalpy/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_landmark_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_open_boundary_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_approximate_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

using Curve_2 = typename cgalpy::aos2::Geometry_traits_2::Curve_2;

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

  py::class_<Gt, Kernel> traits_c(m, "Arr_linear_traits_2",
                                    aos2_doc::Arr_linear_traits_2_class);
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
  cv_c.def(py::init_implicit<const Segment&>(), py::arg("segment"),
           aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_X_monotone_curve_2)
    .def(py::init_implicit<const Ray&>(), py::arg("ray"),
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_X_monotone_curve_2_1)
    .def(py::init_implicit<const Line&>(), py::arg("line"),
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_X_monotone_curve_2_2)
    .def(py::init<const Pnt&, const Pnt&>(),
         py::arg("source"), py::arg("target"),
         "Construct a linear x-monotone curve from two endpoints.")
    .def("source", &Cv::source, ri,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_source)
    .def("target", &Cv::target, ri,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_target)
    .def("is_segment", &Cv::is_segment,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_is_segment)
    .def("segment", &Cv::segment,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_segment)
    .def("is_ray", &Cv::ray,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_is_ray)
    .def("ray", &Cv::ray,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_ray)
    .def("is_line", &Cv::is_line,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_is_line)
    .def("line", &Cv::line,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_line)
    .def("supporting_line", &Cv::supporting_line, ri,
         aos2_doc::Arr_linear_traits_2_X_monotone_curve_2_supporting_line)
    .def("is_vertical", &Cv::is_vertical,
         "Return whether the curve is vertical.")
    .def("left", &Cv::left, ri,
         "Return the left endpoint.")
    .def("right", &Cv::right, ri,
         "Return the right endpoint.")
    .def("set_left", static_cast<overload>(&Cv::set_left),
         "Set the left endpoint to the current source/target endpoint.")
    .def("set_left", set_left, py::arg("point"),
         "Set the left endpoint.")
    .def("set_right", static_cast<overload>(&Cv::set_right),
         "Set the right endpoint to the current source/target endpoint.")
    .def("set_right", set_right, py::arg("point"),
         "Set the right endpoint.")
    .def("is_directed_right", &Cv::is_directed_right,
         "Return whether the curve is directed from left to right.")
    .def("is_in_x_range", &Cv::is_in_x_range, py::arg("point"),
         "Return whether the point is in the x-range of the curve.")
    .def("is_in_y_range", &Cv::is_in_y_range, py::arg("point"),
         "Return whether the point is in the y-range of the curve.")
    .def("is_degenerate", &Cv::is_degenerate,
         "Return whether the curve is degenerate.")
    .def("bbox", &Cv::bbox,
         "Return the bounding box of the curve.")
    ;

  add_attr<Segment>(traits_c, "Segment_2");
  add_attr<Ray>(traits_c, "Ray_2");
  add_attr<Line>(traits_c, "Line_2");

  add_insertion(cv_c, "__str__");
  add_insertion(cv_c, "__repr__");
}
