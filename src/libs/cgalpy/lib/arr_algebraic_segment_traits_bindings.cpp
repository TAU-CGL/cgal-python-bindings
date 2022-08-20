// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_algebraic_segment_traits_2.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Polynomial_traits_d.h>
#include <CGAL/Polynomial_type_generator.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

py::list to_double(aos2::Point_2& p) {
  py::list lst = py::list();
  auto pair = p.to_double();
  lst.append(pair.first);
  lst.append(pair.second);
  return lst;
}

template<typename T, typename S>
void export_construct_point_2_call_operator(py::class_<aos2::Construct_point_2>& cp_2_binding)
{
  cp_2_binding.def<aos2::Point_2(aos2::Construct_point_2::*)(const T&, const S&)>("__call__", &aos2::Construct_point_2::operator());
}

template<typename T, typename S, typename R>
void export_construct_point_2_call_operator(py::class_<aos2::Construct_point_2>& cp_2_binding)
{
  cp_2_binding.def<aos2::Point_2(aos2::Construct_point_2::*)(const T&, const S&, R)>("__call__", &aos2::Construct_point_2::operator());
}

void construct_x_monotone_segment_2_call_operator0(aos2::Construct_x_monotone_segment_2& construct, aos2::Curve_2& cv, aos2::Point_2& end_min, aos2::Point_2& end_max, py::list& lst)
{
  auto v = std::vector<aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(cv, end_min, end_max, it);
  for (auto xcv : v) lst.append(xcv);
}

void construct_x_monotone_segment_2_call_operator1(aos2::Construct_x_monotone_segment_2& construct, aos2::Point_2& p, aos2::Point_2& q, py::list& lst)
{
  auto v = std::vector<aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(p, q, it);
  for (auto xcv : v) lst.append(xcv);
}

void construct_x_monotone_segment_2_call_operator2(aos2::Construct_x_monotone_segment_2& construct, aos2::Curve_2& cv, aos2::Point_2& p, aos2::Geometry_traits_2::Site_of_point& site_of_p, py::list& lst)
{
  auto v = std::vector<aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(cv, p, site_of_p, it);
  for (auto xcv : v) lst.append(xcv);
}

//template <typename PT>
//py::class_<typename PT::Construct_polynomial> bind_construct_polynomial(const char* name)
//{
//  typedef typename PT::Construct_polynomial T;
//  typedef typename PT::Type P;
//  return py::class_<T>(m, name)
//    .def(init<>())
//    ;
//}

template <typename PT>
void init_polynomial(typename PT::Type& pol, py::list& lst) {
  typedef typename PT::Type P;
  typedef typename PT::Coefficient_type CT;
  auto begin = stl_input_iterator<CT>(lst);
  auto end = stl_input_iterator<CT>(lst, false);
  new (&pol) P(begin, end);
}

template <typename PT>
py::class_<typename PT::Type> bind_polynomial(py::module_&m, const char* name) {
  typedef typename PT::Type P;
  typedef typename PT::Coefficient_type CT;
  py::class_<P> c(m, name);
  c.def(py::init<>())
    .def(py::init<CT&>())
    .def(py::init<CT&, CT&>())
    .def(py::init<CT&, CT&, CT&>())
    .def(py::init<CT&, CT&, CT&, CT&>())
    .def(py::init<CT&, CT&, CT&, CT&, CT&>())
    .def(py::init<CT&, CT&, CT&, CT&, CT&, CT&>())
    .def(py::init<CT&, CT&, CT&, CT&, CT&, CT&, CT&>())
    .def(py::init<CT&, CT&, CT&, CT&, CT&, CT&, CT&, CT&>())
    .def("__init__", &init_polynomial<PT>)
    .def("abs", &P::abs)
    .def("coefficients", py::range<Copy_const_reference>(&P::begin, &P::end))
    .def("compare", static_cast<CGAL::Comparison_result (P::*) (const typename P::NT&) const>(&P::compare))
    .def("degree", &P::degree)
    .def("diff", &P::diff)
    .def("divide_by_x", &P::divide_by_x)
    .def("is_zero", &P::is_zero)
    .def("reversal", &P::reversal)
    .def("scalar_div", &P::scalar_div)
    .def("scale", &P::scale)
    .def("scale_down", &P::scale_down)
    .def("scale_up", &P::scale_up)
    .def("sign", &P::sign)
    .def("sign_at", static_cast<CGAL::Sign(P::*)(const typename P::NT&) const>(&P::sign_at))
    .def("simplify_coefficients", &P::simplify_coefficients)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self * py::self)
    .def(int() * py::self)
    .def(CT() * py::self)
    .def(py::self *= py::self)
    .def("__getitem__", &P::operator[], Copy_const_reference())
    ;

  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");

  return c;
}

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

py::object export_arr_algebraic_segment_traits() {
  using Integer = CORE::BigInt;
  using AR1 = Algebraic_kernel_d_2::Algebraic_real_1;
  using GT = CGAL::Arr_algebraic_segment_traits_2<Integer>;

  py::class_<aos2::Integer> integer_co("Integer");
  integer_co.def(py::init<>())
    .def(py::init<int>())
    .def("value", &aos2::Integer::longValue)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self *= py::self)
    ;

  add_insertion(integer_co, "__str__");
  add_insertion(integer_co, "__repr__");

  py::class_<aos2::Algebraic_real_1> ar1_co("Algebraic_real_1");
  ar1_co.def(py::init<>())
    .def(py::init<AR1&>())
    .def(py::init<int>())
    .def(py::init<AR1::Rational&>())
    .def(py::init<const aos2::Polynomial_1&, AR1::Rational, AR1::Rational>())
    .def("bisect", &AR1::bisect)
    .def< CGAL::Comparison_result(AR1::*)(const AR1&) const>("compare", &AR1::compare)
    .def("degree", &AR1::degree)
    .def("high", &AR1::high)
    .def("is_rational", &AR1::is_rational)
    .def("is_root_of", &AR1::is_root_of)
    .def("low", &AR1::low)
    .def("polynomial", &AR1::polynomial, Copy_const_reference())
    .def("rational", &AR1::rational)
    .def("rational_between", &AR1::rational_between)
    .def("refine", &AR1::refine)
    .def("refine_to", &AR1::refine_to)
    .def("sign_at_low", &AR1::sign_at_low)
    .def("simplify", &AR1::simplify)
    .def("to_double", &AR1::to_double)
    .def("upper", &AR1::upper)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    ;

  add_insertion(ar1_co, "__str__");
  add_insertion(ar1_co, "__repr__");

  py::class_<aos2::Bound> bound_co("Bound");
  bound_co.def(py::init<>())
    .def("value", &aos2::Bound::longValue)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self *= py::self)
    ;

  add_insertion(bound_co, "__str__");
  add_insertion(bound_co, "__repr__");

  //bind_construct_polynomial<PT_1>("Construct_polynomial_1");
  //bind_construct_polynomial<PT_2>("Construct_polynomial_2");

  bind_polynomial<aos2::PT_1>(m, "Polynomial_1");
  bind_polynomial<aos2::PT_2>(m, "Polynomial_2");

  bind_shift<aos2::PT_1>(m, "PT_1_Shift");
  bind_shift<aos2::PT_2>(m, "PT_2_Shift");

  bind_swap<aos2::PT_1>(m, "PT_1_Swap");
  bind_swap<aos2::PT_2>(m, "PT_2_Swap");

  m.def("ipower", &ipower<aos2::Polynomial_1>);
  m.def("ipower", &ipower<aos2::Polynomial_2>);

  auto traits = py::class_<GT>(m, "Arr_algebraic_segment_traits");
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
  };
  Concepts concepts;
  export_AosTraits_2<GT, Return_by_value>(traits, concepts);
  traits
    .def("construct_curve_2_object", &GT::construct_curve_2_object)
    .def("construct_tpoint_2_object", &GT::construct_point_2_object)
    .def("construct_x_monotone_segment_2_object", &GT::construct_x_monotone_segment_2_object)
    ;

  auto& p2_co = *(concepts.m_basic_traits_classes.m_point_2);
  p2_co.def("curve", &aos2::Point_2::curve)
    .def("arcno", &aos2::Point_2::arcno)
    .def("to_double", &to_double)
    .def(py::self == py::self)
    .def(py::self != py::self)
    .def(py::self != py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    ;

  add_insertion(p2_co, "__str__");
  add_insertion(p2_co, "__repr__");

  auto& xcv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_co
    .def("curve", &aos2::X_monotone_curve_2::curve)
    .def("is_vertical", &aos2::X_monotone_curve_2::is_vertical)
    .def("is_finite", &aos2::X_monotone_curve_2::is_finite)
    .def("curve_end", &aos2::X_monotone_curve_2::curve_end)
    .def<int (aos2::X_monotone_curve_2::*)() const>("arcno", &aos2::X_monotone_curve_2::arcno)
    .def("x", &aos2::X_monotone_curve_2::x, Copy_const_reference())
    ;

  auto& cv_co = *(concepts.m_traits_classes.m_curve_2);
  cv_co
    .def("polynomial_2", &aos2::Curve_2::polynomial_2)
    ;

  py::class_<aos2::Construct_curve_2>(m, "Construct_curve_2")
    .def("__call__", &aos2::Construct_curve_2::operator());
  ;

  auto cp_2_binding = py::class_<aos2::Construct_point_2>(m, "Construct_point_2");
  export_construct_point_2_call_operator<AR1, aos2::Curve_2, int>(cp_2_binding);
  export_construct_point_2_call_operator<AR1, aos2::X_monotone_curve_2>(cp_2_binding);
  export_construct_point_2_call_operator<AR1, AR1>(cp_2_binding);
  export_construct_point_2_call_operator<aos2::Bound, aos2::Bound>(cp_2_binding);
  export_construct_point_2_call_operator<int, int>(cp_2_binding);

  py::class_<aos2::Construct_x_monotone_segment_2>(m, "Construct_x_monotone_segment_2")
    .def("__call__", &construct_x_monotone_segment_2_call_operator0)
    .def("__call__", &construct_x_monotone_segment_2_call_operator1)
    .def("__call__", &construct_x_monotone_segment_2_call_operator2)
    ;

  py::enum_<GT::Site_of_point>(m, "Site_of_point")
    .value("POINT_IN_INTERIOR", GT::Site_of_point::POINT_IN_INTERIOR)
    .value("MIN_ENDPOINT", GT::Site_of_point::MIN_ENDPOINT)
    .value("MAX_ENDPOINT", GT::Site_of_point::MAX_ENDPOINT)
    .export_values()
    ;

  return traits;
}
