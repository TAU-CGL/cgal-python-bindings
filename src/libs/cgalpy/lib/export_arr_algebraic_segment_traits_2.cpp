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

#include <CGAL/Arr_algebraic_segment_traits_2.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Polynomial_traits_d.h>
#include <CGAL/Polynomial_type_generator.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/bind_polynomial.hpp"

namespace py = nanobind;

py::list to_double(cgalpy::aos2::Point_2& p) {
  py::list lst = py::list();
  auto pair = p.to_double();
  lst.append(pair.first);
  lst.append(pair.second);
  return lst;
}

template<typename T, typename S>
void export_ctr_pnt_operator(py::class_<cgalpy::aos2::Construct_point_2>& co) {
  using Ctr_pnt = cgalpy::aos2::Point_2(cgalpy::aos2::Construct_point_2::*)(const T&, const S&);
  co.def("__call__", static_cast<Ctr_pnt>(&cgalpy::aos2::Construct_point_2::operator()));
}

template<typename T, typename S, typename R>
void export_ctr_pnt_operator(py::class_<cgalpy::aos2::Construct_point_2>& co) {
  using Ctr_pnt =
    cgalpy::aos2::Point_2(cgalpy::aos2::Construct_point_2::*)(const T&, const S&, R);
  co.def("__call__", static_cast<Ctr_pnt>(&cgalpy::aos2::Construct_point_2::operator()));
}

//! Construct `x`-monotone segments from a curve and two points.
py::list ctr_xseg_operator0(cgalpy::aos2::Construct_x_monotone_segment_2& construct,
                            cgalpy::aos2::Curve_2& cv,
                            cgalpy::aos2::Point_2& end_min, cgalpy::aos2::Point_2& end_max) {
  auto v = std::vector<cgalpy::aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(cv, end_min, end_max, it);
  py::list lst;
  for (auto xcv : v) lst.append(xcv);
  return lst;
}

// Construct `x`-monotone segments from two points.
py::list ctr_xseg_operator1(cgalpy::aos2::Construct_x_monotone_segment_2& construct,
                            cgalpy::aos2::Point_2& p, cgalpy::aos2::Point_2& q) {
  auto v = std::vector<cgalpy::aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(p, q, it);
  py::list lst;
  for (auto xcv : v) lst.append(xcv);
  return lst;
}

//
py::list ctr_xseg_operator2(cgalpy::aos2::Construct_x_monotone_segment_2& construct,
                            cgalpy::aos2::Curve_2& cv, cgalpy::aos2::Point_2& p,
                            cgalpy::aos2::Geometry_traits_2::Site_of_point& site_of_p)
{
  auto v = std::vector<cgalpy::aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(cv, p, site_of_p, it);
  py::list lst;
  for (auto xcv : v) lst.append(xcv);
  return lst;
}

//template <typename PT>
//py::class_<typename PT::Construct_polynomial> bind_construct_polynomial(const char* name)
//{
//  using T = typename PT::Construct_polynomial;
//  using P = typename PT::Type;
//  return py::class_<T>(m, name)
//    .def(init<>())
//    ;
//}

template<typename PT>
py::class_<typename PT::Shift> bind_shift(py::module_& m, const char* name) {
  return py::class_<typename PT::Shift>(m, name)
    .def(py::init<>())
    .def("__call__", &PT::Shift::operator())
    ;
}

template<typename PT>
py::class_<typename PT::Swap> bind_swap(py::module_& m, const char* name) {
  return py::class_<typename PT::Swap>(m, name)
    .def(py::init<>())
    .def("__call__", &PT::Swap::operator())
    ;
}

template<typename T>
T ipower(T& p, int i) { return CGAL::ipower(p, i); }

void export_arr_algebraic_segment_traits_2(py::module_& m) {
  using Integer = CORE::BigInt;
  using Gt = CGAL::Arr_algebraic_segment_traits_2<Integer>;
  using Cv = Gt::Curve_2;
  using Xcv = Gt::X_monotone_curve_2;

  using Alg_kernel_d_1 = CGAL::Algebraic_kernel_d_1<Integer>;
  using Alg_kernel_d_2 = CGAL::Algebraic_curve_kernel_2<Alg_kernel_d_1>;
  using Polynomial_1 = Alg_kernel_d_1::Polynomial_1;
  using Polynomial_2 = Alg_kernel_d_2::Polynomial_2;
  using Ar = Alg_kernel_d_2::Algebraic_real_1;
  using Pt1 = CGAL::Polynomial_traits_d<Polynomial_1>;
  using Pt2 = CGAL::Polynomial_traits_d<Polynomial_2>;

  if (add_attr<Gt>(m, "Arr_algebraic_segment_traits")) return;

  if (! add_attr<Integer>(m, "Integer")) {
    py::class_<Integer> integer_c(m, "Integer");
    integer_c.def(py::init<>())
      .def(py::init_implicit<int>())
      // .def("value", &Integer::longValue)
      .def(py::self + py::self)
      .def(py::self += py::self)
      .def(py::self - py::self)
      .def(py::self -= py::self)
      .def(py::self *= py::self)
      ;

    add_insertion(integer_c, "__str__");
    add_insertion(integer_c, "__repr__");
  }

  using Cmp = CGAL::Comparison_result(Ar::*)(const Ar&) const;
  if (! add_attr<Ar>(m, "Algebraic_real_1")) {
    py::class_<Ar> ar1_c(m, "Algebraic_real_1");
    ar1_c.def(py::init<>())
      .def(py::init<Ar&>())
      .def(py::init_implicit<int>())
      .def(py::init<Ar::Rational&>())
      .def(py::init<const cgalpy::aos2::Polynomial_1&, Ar::Rational, Ar::Rational>())
      .def("bisect", &Ar::bisect)
      .def("compare", static_cast<Cmp>(&Ar::compare<Ar>))
      .def("degree", &Ar::degree)
      .def("high", &Ar::high)
      .def("is_rational", &Ar::is_rational)
      .def("is_root_of", &Ar::is_root_of)
      .def("low", &Ar::low)
      .def("polynomial", &Ar::polynomial)
      .def("rational", &Ar::rational)
      .def("rational_between", &Ar::rational_between)
      .def("refine", &Ar::refine)
      .def("refine_to", &Ar::refine_to)
      .def("sign_at_low", &Ar::sign_at_low)
      .def("simplify", &Ar::simplify)
      .def("to_double", &Ar::to_double)
      .def("upper", &Ar::upper)
      .def(py::self == py::self)
      .def(py::self != py::self)
      .def(py::self != py::self)
      .def(py::self < py::self)
      .def(py::self > py::self)
      .def(py::self <= py::self)
      .def(py::self >= py::self)
      ;

    add_insertion(ar1_c, "__str__");
    add_insertion(ar1_c, "__repr__");
  }

  if (! add_attr<cgalpy::aos2::Bound>(m, "Bound")) {
    py::class_<cgalpy::aos2::Bound> bound_c(m, "Bound");
    bound_c.def(py::init<>())
      // .def("value", &cgalpy::aos2::Bound::longValue)
      .def(py::self + py::self)
      .def(py::self += py::self)
      .def(py::self - py::self)
      .def(py::self -= py::self)
      .def(py::self *= py::self)
      ;

    add_insertion(bound_c, "__str__");
    add_insertion(bound_c, "__repr__");
  }

  //bind_construct_polynomial<Pt1>("Construct_polynomial_1");
  //bind_construct_polynomial<Pt2>("Construct_polynomial_2");

  if (! add_attr<Pt1>(m, "Polynomial_1")) bind_polynomial<Pt1>(m, "Polynomial_1");
  if (! add_attr<Pt2>(m, "Polynomial_2")) bind_polynomial<Pt2>(m, "Polynomial_2");

  bind_shift<Pt1>(m, "Pt1_Shift");
  bind_shift<Pt2>(m, "Pt2_Shift");

  bind_swap<Pt1>(m, "Pt1_Swap");
  bind_swap<Pt2>(m, "Pt2_Swap");

  m.def("ipower", &ipower<cgalpy::aos2::Polynomial_1>);
  m.def("ipower", &ipower<cgalpy::aos2::Polynomial_2>);

  py::class_<Gt> traits_c(m, "Arr_algebraic_segment_traits");
  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
  };
  Concepts concepts;
  export_AosTraits_2<Gt>(traits_c, concepts);
  traits_c.def("construct_curve_2_object", &Gt::construct_curve_2_object)
    .def("construct_point_2_object", &Gt::construct_point_2_object)
    .def("construct_x_monotone_segment_2_object",
         &Gt::construct_x_monotone_segment_2_object)
    ;

  // Apparently, the Compare_xy_2::operator() accepts a 3rd parameter (of type
  // bool) with a default value. This prevents the normal bindings that takes
  // place by the call to export_AosTraits_2(). Instead, bind this operator:
  auto& cmp_xy_c = *(concepts.m_aos_basic_traits_2_classes.m_compare_xy_2);
  // cmp_xy_c.def("__call__", py::overload_cast<const Pnt&, const Pnt&, bool>
  //              (&Compare_xy_2::operator(), py::const_));
  using Compare_xy_2 = typename Gt::Compare_xy_2;
  cmp_xy_c.def("__call__", &Compare_xy_2::operator());

  // Enhance the bindings of the point_2 type
  auto& pnt_c = *(concepts.m_aos_basic_traits_2_classes.m_point_2);
  pnt_c.def("curve", &Gt::Point_2::curve)
    .def("arcno", &Gt::Point_2::arcno)
    .def("to_double", &to_double)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    ;

  add_insertion(pnt_c, "__str__");
  add_insertion(pnt_c, "__repr__");

  auto& xcv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
  xcv_c.def("curve", &Xcv::curve)
    .def("is_vertical", &Xcv::is_vertical)
    .def("is_finite", &Xcv::is_finite)
    .def("curve_end", &Xcv::curve_end)
    .def("arcno", py::overload_cast<>(&Xcv::arcno, py::const_))
    .def("x", &Xcv::x)
    ;

  auto& cv_c = *(concepts.m_aos_traits_2_classes.m_curve_2);
  cv_c.def("polynomial_2", &Cv::polynomial_2)
    ;

  using Ctr_cv = Gt::Construct_curve_2;
  py::class_<Ctr_cv>(traits_c, "Construct_curve_2")
    .def("__call__", &Ctr_cv::operator());
  ;

  using Ctr_pnt = Gt::Construct_point_2;
  py::class_<Ctr_pnt> ctr_pnt_c(traits_c, "Construct_point_2");
  export_ctr_pnt_operator<Ar, Cv, int>(ctr_pnt_c);
  export_ctr_pnt_operator<Ar, Xcv>(ctr_pnt_c);
  export_ctr_pnt_operator<Ar, Ar>(ctr_pnt_c);
  export_ctr_pnt_operator<cgalpy::aos2::Bound, cgalpy::aos2::Bound>(ctr_pnt_c);
  export_ctr_pnt_operator<Integer, Integer>(ctr_pnt_c);
  export_ctr_pnt_operator<int, int>(ctr_pnt_c);

  using Ctr_xseg = Gt::Construct_x_monotone_segment_2;
  py::class_<Ctr_xseg>(traits_c, "Construct_x_monotone_segment_2")
    .def("__call__", &ctr_xseg_operator0)
    .def("__call__", &ctr_xseg_operator1)
    .def("__call__", &ctr_xseg_operator2)
    ;

  py::enum_<Gt::Site_of_point>(traits_c, "Site_of_point")
    .value("POINT_IN_INTERIOR", Gt::Site_of_point::POINT_IN_INTERIOR)
    .value("MIN_ENDPOINT", Gt::Site_of_point::MIN_ENDPOINT)
    .value("MAX_ENDPOINT", Gt::Site_of_point::MAX_ENDPOINT)
    .export_values()
    ;
}
