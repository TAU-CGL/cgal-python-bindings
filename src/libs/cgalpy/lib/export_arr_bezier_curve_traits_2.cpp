// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_Bezier_curve_traits_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/add_attr.hpp"
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

namespace cgalpy {
namespace aos2 {

// Initialize a Bezier curve from a list of rational control points.
void init_from_points(Curve_2* cv, py::list& lst) {
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rational = Nt_traits::Rational;
  using Rat_kernel = CGAL::Cartesian<Rational>;
  auto begin = stl_forward_iterator<Rat_kernel::Point_2>(lst);
  auto end = stl_forward_iterator<Rat_kernel::Point_2>(lst, false);
  new (cv) Curve_2(begin, end);         // placement new
}

}
} // namespace cgalpy

//
void export_arr_bezier_traits_2(py::module_& m) {
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rational = Nt_traits::Rational;
  using Algebraic = Nt_traits::Algebraic;
  using Rat_kernel = CGAL::Cartesian<Rational>;
  using Alg_kernel = CGAL::Cartesian<Algebraic>;
  using Gt = CGAL::Arr_Bezier_curve_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  using Pnt = Gt::Point_2;
  using Cv = Gt::Curve_2;
  using Xcv = Gt::X_monotone_curve_2;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Gt>(m, "Arr_bezier_curve_traits_2")) return;

  auto traits = py::class_<Gt>(
    m, "Arr_bezier_curve_traits_2",
    aos2_doc::Arr_Bezier_curve_traits_2_class)
    .def(py::init<>(),
         "Construct a default Bezier-curve traits object.")
    ;

  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
    Aos_directional_x_monotone_traits_classes<Gt>
      m_aos_directional_x_monotone_traits_2_classes;
  } concepts;
  export_AosTraits_2<Gt>(traits, concepts);
  export_AosDirectionalXMonotoneTraits_2<Gt>(traits, concepts);

  // Point_2
  auto& pnt_c = *(concepts.m_aos_basic_traits_2_classes.m_point_2);
  pnt_c.def(py::init<double, double>(),
            py::arg("x"), py::arg("y"),
            aos2_doc::Arr_Bezier_curve_traits_2_Point_2_Point_2_3)
    ;

  // X_monotone_curve_2
  auto& xcv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
  xcv_c.def("supporting_curve", &Xcv::supporting_curve,
              aos2_doc::Arr_Bezier_curve_traits_2_X_monotone_curve_2_supporting_curve)
    .def("xid", &Xcv::xid,
         "Return the internal x-monotone subcurve identifier.")
    .def("source", &Xcv::source, ri,
         aos2_doc::Arr_Bezier_curve_traits_2_X_monotone_curve_2_source)
    .def("target", &Xcv::target, ri,
         aos2_doc::Arr_Bezier_curve_traits_2_X_monotone_curve_2_target)
    .def("left", &Xcv::left, ri,
         aos2_doc::Arr_Bezier_curve_traits_2_X_monotone_curve_2_left)
    .def("right", &Xcv::right, ri,
         aos2_doc::Arr_Bezier_curve_traits_2_X_monotone_curve_2_right)
    .def("is_vertical", &Xcv::is_vertical,
         "Return whether the x-monotone Bezier subcurve is vertical.")
    .def("is_directed_right", &Xcv::is_directed_right,
         "Return whether the subcurve is directed from left to right.")
    .def("parameter_range", &Xcv::parameter_range,
         aos2_doc::Arr_Bezier_curve_traits_2_X_monotone_curve_2_parameter_range)
    .def("point_position", &Xcv::point_position,
         py::arg("point"), py::arg("cache"),
         "Return the approximate parameter-space position of a point on the subcurve.")
    ;

  // Curve_2
  auto& cv_c = *(concepts.m_aos_traits_2_classes.m_curve_2);
  //TODO add constructors
  cv_c.def("__init__", &cgalpy::aos2::init_from_points,
           py::arg("control_points"),
           aos2_doc::Arr_Bezier_curve_traits_2_Curve_2_Curve_2_1)
    .def("id", &Cv::id,
         "Return the internal Bezier curve identifier.")
    .def("x_polynomial", &Cv::x_polynomial,
         "Return the x-coordinate polynomial.")
    .def("x_norm", &Cv::x_norm,
         "Return the x-coordinate normalization factor.")
    .def("y_polynomial", &Cv::y_polynomial,
         "Return the y-coordinate polynomial.")
    .def("y_norm", &Cv::y_norm,
         "Return the y-coordinate normalization factor.")
    .def("number_of_control_points", &Cv::number_of_control_points,
         aos2_doc::Arr_Bezier_curve_traits_2_Curve_2_number_of_control_points)
    .def("control_point", &Cv::control_point, ri, py::arg("k"),
         aos2_doc::Arr_Bezier_curve_traits_2_Curve_2_control_point)
    // .def("control_points", &Cv::)
    // .def("__call__", &Cv::)
    // .def("__call__", &Cv::)
    // .def("sample", &Cv::)
    // .def("get_t_at_x", &Cv::)
    // .def("get_t_at_y", &Cv::)
    .def("has_same_support", &Cv::has_same_support, py::arg("curve"),
         "Return whether two Bezier curves have the same support.")
    .def("bbox", &Cv::bbox, ri,
         "Return the bounding box of the Bezier curve.")
    .def("has_no_self_intersections", &Cv::has_no_self_intersections,
         "Return whether the Bezier curve has no self-intersections.")
    ;

  add_insertion(cv_c, "__str__");
  add_insertion(cv_c, "__repr__");
  add_extraction(cv_c);
}
