// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <type_traits>

#include <boost/multiprecision/cpp_int.hpp>
#if CGAL_USE_GMP
#include <boost/multiprecision/gmp.hpp>
#endif

#include <nanobind/nanobind.h>

#include <boost/math/special_functions/ellint_2.hpp>

#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/kernel_types.hpp"
#include "cgalpy/arrangement_on_surface_2_types.hpp"
#include "cgalpy/aos_2_concepts/export_AosTraits_2.hpp"
#include "cgalpy/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "cgalpy/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_traits_classes.hpp"
#include "cgalpy/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"
#include "cgalpy/export_boost_multiprecision.hpp"
#include "cgalpy/Kernel/export_ft.hpp"
#include "cgalpy/Kernel/export_point_2.hpp"
#include "cgalpy/Kernel/export_segment_2.hpp"
#include "cgalpy/Kernel/export_circle_2.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

void export_arr_conic_traits_2(py::module_& m) {
  //TODO export RatKernel, AlgKernel
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rat_kernel = CGAL::Cartesian<Nt_traits::Rational>;
  using Alg_kernel = CGAL::Cartesian<Nt_traits::Algebraic>;
  using Gt = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  using Pnt = Gt::Point_2;
  using Cv = Gt::Curve_2;
  using Xcv = Gt::X_monotone_curve_2;
  using Integer = Nt_traits::Integer;
  using Rational = Nt_traits::Rational;
  using Algebraic = Alg_kernel::FT;
  using Rat_pnt = Rat_kernel::Point_2;
  using Rat_seg = Rat_kernel::Segment_2;
  using Rat_circle = Rat_kernel::Circle_2;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Gt>(m, "Arr_conic_traits_2")) return;

  py::class_<Gt> traits_c(m, "Arr_conic_traits_2",
                            aos2_doc::Arr_conic_traits_2_class);
  traits_c.def(py::init<>(),
               "Construct a default conic traits object.")
    .def("construct_curve_2_object", &Gt::construct_curve_2_object,
         aos2_doc::Arr_conic_traits_2_construct_curve_2_object)
    .def("construct_x_monotone_curve_2_object",
         &Gt::construct_x_monotone_curve_2_object,
         aos2_doc::Arr_conic_traits_2_construct_x_monotone_curve_2_object)
    ;

  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
    Aos_directional_x_monotone_traits_classes<Gt>
      m_aos_directional_x_monotone_traits_2_classes;
  } concepts;

  // Export multiprecision integer and rational unlimited precision types
#if CGAL_USE_GMP
  using mpq_rat = boost::multiprecision::mpq_rational;
  using mpz_int = boost::multiprecision::mpz_int;
  if constexpr (std::is_same_v<Rational, mpq_rat>) {
    if (! add_attr<mpz_int>(m, "mpz_int")) {
      py::class_<mpz_int> mpz_int_c(m, "mpz_int");
      export_boost_multiprecision_int(mpz_int_c);
    }
    add_attr<Integer>(traits_c, "Integer");

    if (! add_attr<mpq_rat>(m, "mpq_rational")) {
      py::class_<mpq_rat> mpq_rat_c(m, "mpq_rational");
      export_boost_multiprecision_rational(mpq_rat_c);
    }
    add_attr<Rational>(traits_c, "Rational");
  }
#endif

  // Fall back; if other options exist, the corresponding wrappers whould be added
  if (! add_attr<Integer>(traits_c, "Integer")) {
    py::class_<Integer> int_c(traits_c, "Integer",
                              "Integer number type used by the conic traits.");
    int_c.def(py::init<const Integer&>(), py::arg("value"),
              "Copy-construct an integer.")
      .def(py::init_implicit<int>(), py::arg("value"),
           "Construct from a Python integer.")
      ;

    add_insertion(int_c, "__str__");
    add_insertion(int_c, "__repr__");
  }

  if (! add_attr<Rational>(traits_c, "Rational")) {
    py::class_<Rational> rat_c(traits_c, "Rational");
    export_ft(rat_c);
    rat_c.def(py::init<const Rational&>(), py::arg("value"),
              "Copy-construct a rational.")
      .def(py::init_implicit<Integer>(), py::arg("value"),
           "Construct from an integer.")
      .def(py::init<const Integer&, const Integer&>(),
           py::arg("numerator"), py::arg("denominator"),
           "Construct from numerator and denominator.")
      ;

    add_insertion(rat_c, "__str__");
    add_insertion(rat_c, "__repr__");
  }

  if (! add_attr<Algebraic>(traits_c, "Algebraic")) {
    py::class_<Algebraic> alg_c(traits_c, "Algebraic",
                                "Algebraic number type used by the conic traits.");
    alg_c.def(py::init_implicit<double>(), py::arg("value"),
              "Construct from a floating-point value.")
      .def(py::init_implicit<Rational>(), py::arg("value"),
           "Construct from a rational value.")
      ;

    add_insertion(alg_c, "__str__");
    add_insertion(alg_c, "__repr__");
  }

  if (! add_attr<Rat_pnt>(traits_c, "Rat_point_2")) {
    py::class_<Rat_pnt> rat_pnt_c(traits_c, "Rat_point_2");
    export_point_2<Rat_kernel>(rat_pnt_c);
  }

  if (! add_attr<Rat_seg>(traits_c, "Rat_segment_2")) {
    py::class_<Rat_seg> rat_seg_c(traits_c, "Rat_segment_2");
    export_segment_2<Rat_kernel>(rat_seg_c);
  }

  if (! add_attr<Rat_circle>(traits_c, "Rat_circle_2")) {
    py::class_<Rat_circle> rat_circle_c(traits_c, "Rat_circle_2");
    export_circle_2<Rat_kernel>(rat_circle_c);
  }

  if (! add_attr<Pnt>(traits_c, "Point_2")) {
    concepts.m_aos_basic_traits_2_classes.m_point_2 =
      new py::class_<Pnt>(traits_c, "Point_2",
                            aos2_doc::Arr_conic_traits_2_Point_2_class);
    auto& pnt_c = *(concepts.m_aos_basic_traits_2_classes.m_point_2);
    pnt_c.def(py::init<const Algebraic&, const Algebraic&>(),
              py::arg("x"), py::arg("y"),
              aos2_doc::Arr_conic_traits_2_Point_2_Point_2_3)
      // The following belong to the algebraic kernel, but for now we add them here
      .def(py::init<const Algebraic&, const Algebraic&, const Algebraic&>(),
           py::arg("hx"), py::arg("hy"), py::arg("hw"),
           aos2_doc::Arr_conic_traits_2_Point_2_Point_2_2)
      .def("x", &Pnt::x,
           "Return the x-coordinate of the point.")
      .def("y", &Pnt::y,
           "Return the y-coordinate of the point.")
      ;

    add_insertion(pnt_c, "__str__");
    add_insertion(pnt_c, "__repr__");
  }

  // Export Curve_2 and X_monotone_curve_2 (which derives from Curve_2)
  // explicitly here, instead of exporting them in export_AosTraits_2 and
  // aos_TraitsBasicTraitse_2, since X_monotone_curve_2 derives from Curve_2.
  // Export Cv:
  if (! add_attr<Cv>(traits_c, "Curve_2")) {
    concepts.m_aos_traits_2_classes.m_curve_2 =
      new py::class_<Cv>(traits_c, "Curve_2",
                           aos2_doc::Arr_conic_traits_2_Curve_2_class);
    auto& cv_c = *(concepts.m_aos_traits_2_classes.m_curve_2);
    cv_c.def(py::init<>(),
             aos2_doc::Arr_conic_traits_2_Curve_2_Curve_2)
      .def(py::init<const Cv&>(), py::arg("curve"),
           "Copy-construct a conic curve.")
      .def("is_valid", &Cv::is_valid,
           aos2_doc::Arr_conic_traits_2_Curve_2_is_valid)
      .def("r", &Cv::r, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_r)
      .def("s", &Cv::s, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_s)
      .def("t", &Cv::t, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_t)
      .def("u", &Cv::u, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_u)
      .def("v", &Cv::v, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_v)
      .def("w", &Cv::w, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_w)
      // .def("is_x_monotone", &Cv::is_x_monotone)
      // .def("is_y_monotone", &Cv::is_y_monotone)
      .def("is_full_conic", &Cv::is_full_conic,
           aos2_doc::Arr_conic_traits_2_Curve_2_is_full_conic)
      .def("source", py::overload_cast<>(&Cv::source, py::const_), ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_source)
      .def("target", py::overload_cast<>(&Cv::target, py::const_), ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_target)
      .def("orientation", &Cv::orientation,
           aos2_doc::Arr_conic_traits_2_Curve_2_orientation)
      // .def("bbox", &Cv::bbox)
      .def("set_source", &Cv::set_source, py::arg("point"),
           aos2_doc::Arr_conic_traits_2_Curve_2_set_source)
      .def("set_target", &Cv::set_target, py::arg("point"),
           aos2_doc::Arr_conic_traits_2_Curve_2_set_target)
      ;

    add_insertion(cv_c, "__str__");
    add_insertion(cv_c, "__repr__");
  }

  // Export X_monotone_curve_2:
  if (! add_attr<Xcv>(traits_c, "X_monotone_curve_2")) {
    // The inheritance Curve_2 - X_monotone_curve_2 is currently private:
    concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2 =
      // new py::class_<Xcv, Cv>(traits_c, "X_monotone_curve_2");
      new py::class_<Xcv>(traits_c, "X_monotone_curve_2",
                            aos2_doc::Arr_conic_traits_2_X_monotone_curve_2_class);
    auto& xcv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
    xcv_c
      .def("r", &Xcv::r, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_r)
      .def("s", &Xcv::s, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_s)
      .def("t", &Xcv::t, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_t)
      .def("u", &Xcv::u, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_u)
      .def("v", &Xcv::v, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_v)
      .def("w", &Xcv::w, ri,
           aos2_doc::Arr_conic_traits_2_Curve_2_w)
      .def("left", &Xcv::left, ri,
           aos2_doc::Arr_conic_traits_2_X_monotone_curve_2_left)
      .def("right", &Xcv::right, ri,
           aos2_doc::Arr_conic_traits_2_X_monotone_curve_2_right)
      .def("is_directed_right", &Xcv::is_directed_right, ri,
           "Return whether the arc is directed from left to right.")
      ;

    add_insertion(xcv_c, "__str__");
    add_insertion(xcv_c, "__repr__");
  }

  // Export additional concepts
  export_AosTraits_2<Gt>(traits_c, concepts);
  export_AosDirectionalXMonotoneTraits_2<Gt>(traits_c, concepts);

  // Export additional curve attributes:
  using Ctr_cv = Gt::Construct_curve_2;
  using ctr_cv_op0 = Cv(Ctr_cv::*)(const Pnt&, const Pnt&)const;
  using ctr_cv_op1 = Cv(Ctr_cv::*)(const Rat_seg&)const;
  using ctr_cv_op2 = Cv(Ctr_cv::*)(const Rat_circle&)const;
  using ctr_cv_op3 = Cv(Ctr_cv::*)(const Rat_circle&, CGAL::Orientation,
                                   const Pnt&, const Pnt&)const;
  using ctr_cv_op4 = Cv(Ctr_cv::*)(const Rat_pnt&, const Rat_pnt&,
                                   const Rat_pnt&)const;
  using ctr_cv_op5 = Cv(Ctr_cv::*)(const Rat_pnt&, const Rat_pnt&,
                                   const Rat_pnt&, const Rat_pnt&,
                                   const Rat_pnt&)const;
  using ctr_cv_op6 = Cv(Ctr_cv::*)(const Rational&, const Rational&,
                                   const Rational&, const Rational&,
                                   const Rational&, const Rational&)const;
  using ctr_cv_op7 = Cv(Ctr_cv::*)(const Rational&, const Rational&,
                                   const Rational&, const Rational&,
                                   const Rational&, const Rational&,
                                   CGAL::Orientation,
                                   const Pnt&, const Pnt&)const;
  using ctr_cv_op8 = Cv(Ctr_cv::*)(const Rational&, const Rational&,
                                   const Rational&, const Rational&,
                                   const Rational&, const Rational&,
                                   CGAL::Orientation, const Pnt&,
                                   const Rational&, const Rational&,
                                   const Rational&, const Rational&,
                                   const Rational&, const Rational&,
                                   const Pnt&,
                                   const Rational&, const Rational&,
                                   const Rational&, const Rational&,
                                   const Rational&, const Rational&)const;

  py::class_<Ctr_cv>(traits_c, "Construct_curve_2",
                       aos2_doc::Arr_conic_traits_2_Construct_curve_2_class)
    .def("__call__", static_cast<ctr_cv_op0>(&Ctr_cv::operator()),
         py::arg("source"), py::arg("target"),
         "Construct a conic arc between two conic points.")
    .def("__call__", static_cast<ctr_cv_op1>(&Ctr_cv::operator()),
         py::arg("segment"),
         aos2_doc::Arr_conic_traits_2_Construct_curve_2_operator_call)
    .def("__call__", static_cast<ctr_cv_op2>(&Ctr_cv::operator()),
         py::arg("circle"),
         aos2_doc::Arr_conic_traits_2_Construct_curve_2_operator_call_1)
    .def("__call__", static_cast<ctr_cv_op3>(&Ctr_cv::operator()),
         py::arg("circle"), py::arg("orientation"),
         py::arg("source"), py::arg("target"),
         aos2_doc::Arr_conic_traits_2_Construct_curve_2_operator_call_2)
    .def("__call__", static_cast<ctr_cv_op4>(&Ctr_cv::operator()),
         py::arg("p1"), py::arg("p2"), py::arg("p3"),
         aos2_doc::Arr_conic_traits_2_Construct_curve_2_operator_call_3)
    .def("__call__", static_cast<ctr_cv_op5>(&Ctr_cv::operator()),
         py::arg("p1"), py::arg("p2"), py::arg("p3"),
         py::arg("p4"), py::arg("p5"),
         aos2_doc::Arr_conic_traits_2_Construct_curve_2_operator_call_6)
    .def("__call__", static_cast<ctr_cv_op6>(&Ctr_cv::operator()),
         py::arg("r"), py::arg("s"), py::arg("t"),
         py::arg("u"), py::arg("v"), py::arg("w"),
         aos2_doc::Arr_conic_traits_2_Construct_curve_2_operator_call_4)
    .def("__call__", static_cast<ctr_cv_op7>(&Ctr_cv::operator()),
         py::arg("r"), py::arg("s"), py::arg("t"),
         py::arg("u"), py::arg("v"), py::arg("w"),
         py::arg("orientation"), py::arg("source"), py::arg("target"),
         aos2_doc::Arr_conic_traits_2_Construct_curve_2_operator_call_5)
    .def("__call__", static_cast<ctr_cv_op8>(&Ctr_cv::operator()),
         py::arg("r"), py::arg("s"), py::arg("t"),
         py::arg("u"), py::arg("v"), py::arg("w"),
         py::arg("orientation"),
         py::arg("app_source"), py::arg("r1"), py::arg("s1"),
         py::arg("t1"), py::arg("u1"), py::arg("v1"), py::arg("w1"),
         py::arg("app_target"), py::arg("r2"), py::arg("s2"),
         py::arg("t2"), py::arg("u2"), py::arg("v2"), py::arg("w2"),
         aos2_doc::Arr_conic_traits_2_Construct_curve_2_operator_call_7)
    ;

  using Ctr_xcv = Gt::Construct_x_monotone_curve_2;
  using ctr_xcv_op0 = Xcv(Ctr_xcv::*)(const Pnt&, const Pnt&)const;
  using ctr_xcv_op1 = Xcv(Ctr_xcv::*)(const Algebraic&, const Algebraic&,
                                      const Algebraic&,
                                      const Pnt&, const Pnt&)const;

  py::class_<Ctr_xcv>(
    traits_c, "Construct_x_monotone_curve_2",
    aos2_doc::Arr_conic_traits_2_Construct_x_monotone_curve_2_class)
    .def("__call__", static_cast<ctr_xcv_op0>(&Ctr_xcv::operator()),
         py::arg("source"), py::arg("target"),
         aos2_doc::Arr_conic_traits_2_Construct_x_monotone_curve_2_operator_call_1)
    .def("__call__", static_cast<ctr_xcv_op1>(&Ctr_xcv::operator()),
         py::arg("a"), py::arg("b"), py::arg("c"),
         py::arg("source"), py::arg("target"),
         aos2_doc::Arr_conic_traits_2_Construct_x_monotone_curve_2_operator_call_2)
    ;

#if CGAL_VERSION_NR > 1060200900
  using Approx_len = Gt::Approximate_length_2;
  py::class_<Approx_len>(traits_c, "Approximate_length_2",
                           "Functor that approximates the length of a conic arc.")
    .def("__call__", &Approx_len::operator(), py::arg("xcv"),
         "Approximate the length of the x-monotone conic arc.")
    ;
#endif

  // Convenient attributes
  add_attr<Integer>(traits_c, "Integer");
  add_attr<Rational>(traits_c, "Rational");
  add_attr<Algebraic>(traits_c, "Algebraic");
}
