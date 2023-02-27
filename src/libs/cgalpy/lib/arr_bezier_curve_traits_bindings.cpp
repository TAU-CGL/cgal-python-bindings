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
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace aos2 {

// Initialize a Bezier curve from a list of control points..
void init_from_points(Curve_2* cv, py::list& lst) {
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  new (cv) Curve_2(begin, end);         // placement new
}

}

//
py::object export_arr_bezier_traits(py::module_& m) {
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rational = Nt_traits::Rational;
  using Algebraic = Nt_traits::Algebraic;
  using Rat_kernel = CGAL::Cartesian<Rational>;
  using Alg_kernel = CGAL::Cartesian<Algebraic>;
  using GT = CGAL::Arr_Bezier_curve_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  using Pnt = GT::Point_2;
  using Cv = GT::Curve_2;
  using Xcv = GT::X_monotone_curve_2;

  constexpr auto ri(py::rv_policy::reference_internal);

  auto traits = py::class_<GT>(m, "Arr_bezier_curve_traits_2")
    .def(py::init<>())
    ;

  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_directional_x_monotone_traits_classes<GT>
      m_directional_x_monotone_traits_classes;
  } concepts;
  export_AosTraits_2<GT>(traits, concepts);
  export_AosDirectionalXMonotoneTraits_2<GT>(traits, concepts);

  // Point_2
  auto& pnt_c = *(concepts.m_basic_traits_classes.m_point_2);
  pnt_c.def(py::init<double, double>())
    ;

  // X_monotone_curve_2
  auto& xcv_c = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_c.def("supporting_curve", &Xcv::supporting_curve)
    .def("xid", &Xcv::xid)
    .def("source", &Xcv::source, ri)
    .def("target", &Xcv::target, ri)
    .def("left", &Xcv::left, ri)
    .def("right", &Xcv::right, ri)
    .def("is_vertical", &Xcv::is_vertical)
    .def("is_directed_right", &Xcv::is_directed_right)
    .def("parameter_range", &Xcv::parameter_range)
    .def("point_position", &Xcv::point_position)
    ;

  // Curve_2
  auto& cv_c = *(concepts.m_traits_classes.m_curve_2);
  //TODO add constructors
  cv_c.def("__init__", &aos2::init_from_points)
    .def("id", &Cv::id)
    .def("x_polynomial", &Cv::x_polynomial)
    .def("x_norm", &Cv::x_norm)
    .def("y_polynomial", &Cv::y_polynomial)
    .def("y_norm", &Cv::y_norm)
    .def("number_of_control_points", &Cv::number_of_control_points)
    .def("control_point", &Cv::control_point, ri)
    // .def("control_points", &Cv::)
    // .def("__call__", &Cv::)
    // .def("__call__", &Cv::)
    // .def("sample", &Cv::)
    // .def("get_t_at_x", &Cv::)
    // .def("get_t_at_y", &Cv::)
    .def("has_same_support", &Cv::has_same_support)
    .def("bbox", &Cv::bbox, ri)
    .def("has_no_self_intersections", &Cv::has_no_self_intersections)
    ;

  add_insertion(cv_c, "__str__");
  add_insertion(cv_c, "__repr__");
  add_extraction(cv_c);

  return traits;
}
