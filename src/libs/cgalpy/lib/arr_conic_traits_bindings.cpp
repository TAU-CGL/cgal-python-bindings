// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_conic_traits_2.h>
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

namespace py = nanobind;

py::object export_arr_conic_traits(py::module_& m) {
  //TODO export RatKernel, AlgKernel
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rat_kernel = CGAL::Cartesian <Nt_traits::Rational>;
  using Alg_kernel = CGAL::Cartesian <Nt_traits::Algebraic>;
  using GT = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  using Curve_2 = GT::Curve_2;
  using X_monotone_curve_2 = GT::X_monotone_curve_2;
  using Rational = GT::Rational;

  auto traits = py::class_<GT>(m, "Arr_conic_traits_2")
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

  py::class_<GT::Integer>(m, "Integer")
    .def(py::init<>())
    .def(py::init<const GT::Integer&>())
    .def(py::init<int>())
    ;

  auto& p_co = *(concepts.m_basic_traits_classes.m_point_2);
  p_co
    .def(py::init<double, double>())
    ;

  auto& xcv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_co.def("left", &X_monotone_curve_2::left)
    .def("right", &X_monotone_curve_2::right)
    ;

  auto& cv_co = *(concepts.m_traits_classes.m_curve_2);
  //TODO add constructors
  cv_co.def(py::init<double, double, double, double, double, double>())
    .def(py::init<double, double, double, double, double, double,
         CGAL::Orientation, const GT::Point_2&, const GT::Point_2&>())
    .def(py::init<const Rational&, const Rational&, const Rational&,
         const Rational&, const Rational&, const Rational&>())
    .def(py::init<const Rational&, const Rational&, const Rational&,
         const Rational&, const Rational&, const Rational&,
         CGAL::Orientation, const GT::Point_2&, const GT::Point_2&>())
    .def("source", &Curve_2::source)
    .def("target", &Curve_2::target)
    .def("orientation", &Curve_2::orientation)
    .def("is_valid", &Curve_2::is_valid)
    .def("is_x_monotone", &Curve_2::is_x_monotone)
    .def("is_y_monotone", &Curve_2::is_y_monotone)
    .def("is_full_conic", &Curve_2::is_full_conic)
    .def("bbox", &Curve_2::bbox)
    .def("set_source", &Curve_2::set_source)
    .def("set_target", &Curve_2::set_target)
    .def("r", &Curve_2::r)
    .def("s", &Curve_2::s)
    .def("t", &Curve_2::t)
    .def("u", &Curve_2::u)
    .def("v", &Curve_2::v)
    .def("w", &Curve_2::w)
    ;

  add_insertion(cv_co, "__str__");
  add_insertion(cv_co, "__repr__");

  return traits;
}
