// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"

namespace bp = boost::python;

bp::list to_double(aos2::Point_2& p) {
  bp::list lst = bp::list();
  auto pair = p.to_double();
  lst.append(pair.first);
  lst.append(pair.second);
  return lst;
}

template<typename T, typename S>
void export_construct_point_2_call_operator(bp::class_<aos2::Construct_point_2>& cp_2_binding)
{
  cp_2_binding.def<aos2::Point_2(aos2::Construct_point_2::*)(const T&, const S&)>("__call__", &aos2::Construct_point_2::operator());
}

template<typename T, typename S, typename R>
void export_construct_point_2_call_operator(bp::class_<aos2::Construct_point_2>& cp_2_binding)
{
  cp_2_binding.def<aos2::Point_2(aos2::Construct_point_2::*)(const T&, const S&, R)>("__call__", &aos2::Construct_point_2::operator());
}

void construct_x_monotone_segment_2_call_operator0(aos2::Construct_x_monotone_segment_2& construct, aos2::Curve_2& cv, aos2::Point_2& end_min, aos2::Point_2& end_max, bp::list& lst)
{
  auto v = std::vector<aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(cv, end_min, end_max, it);
  for (auto xcv : v) lst.append(xcv);
}

void construct_x_monotone_segment_2_call_operator1(aos2::Construct_x_monotone_segment_2& construct, aos2::Point_2& p, aos2::Point_2& q, bp::list& lst)
{
  auto v = std::vector<aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(p, q, it);
  for (auto xcv : v) lst.append(xcv);
}

void construct_x_monotone_segment_2_call_operator2(aos2::Construct_x_monotone_segment_2& construct, aos2::Curve_2& cv, aos2::Point_2& p, aos2::Geometry_traits_2::Site_of_point& site_of_p, bp::list& lst)
{
  auto v = std::vector<aos2::X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(cv, p, site_of_p, it);
  for (auto xcv : v) lst.append(xcv);
}

//template <typename PT>
//bp::class_<typename PT::Construct_polynomial> bind_construct_polynomial(const char* name)
//{
//  typedef typename PT::Construct_polynomial T;
//  typedef typename PT::Type P;
//  return bp::class_<T>(name)
//    .def(init<>())
//    ;
//}

template <typename PT>
typename PT::Type* init_polynomial(bp::list& lst) {
  typedef typename PT::Type P;
  typedef typename PT::Coefficient_type CT;
  bp::stl_input_iterator<CT> begin(lst), end;
  return new P(begin, end);
}

template <typename PT>
bp::class_<typename PT::Type> bind_polynomial(const char* name) {
  typedef typename PT::Type P;
  typedef typename PT::Coefficient_type CT;
  return bp::class_<P>(name)
    .def(bp::init<>())
    .def(bp::init<CT&>())
    .def(bp::init<CT&, CT&>())
    .def(bp::init<CT&, CT&, CT&>())
    .def(bp::init<CT&, CT&, CT&, CT&>())
    .def(bp::init<CT&, CT&, CT&, CT&, CT&>())
    .def(bp::init<CT&, CT&, CT&, CT&, CT&, CT&>())
    .def(bp::init<CT&, CT&, CT&, CT&, CT&, CT&, CT&>())
    .def(bp::init<CT&, CT&, CT&, CT&, CT&, CT&, CT&, CT&>())
    .def("__init__", make_constructor(&init_polynomial<PT>))
    .def("abs", &P::abs)
    .def("coefficients", bp::range<Copy_const_reference>(&P::begin, &P::end))
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
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(bp::self * bp::self)
    .def(int() * bp::self)
    .def(CT() * bp::self)
    .def(bp::self *= bp::self)
    .def("__getitem__", &P::operator[], Copy_const_reference())
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    ;
}

template<typename PT>
bp::class_<typename PT::Shift> bind_shift(const char* name) {
  return bp::class_<typename PT::Shift>(name)
    .def(bp::init<>())
    .def("__call__", &PT::Shift::operator())
    ;
}

template<typename PT>
bp::class_<typename PT::Swap> bind_swap(const char* name) {
  return bp::class_<typename PT::Swap>(name)
    .def(bp::init<>())
    .def("__call__", &PT::Swap::operator())
    ;
}

template<typename T>
T ipower(T& p, int i) { return CGAL::ipower(p, i); }

bp::class_<aos2::Geometry_traits_2> export_arr_algebraic_segment_traits() {
  typedef aos2::Algebraic_real_1        AR1;
  typedef aos2::Geometry_traits_2       GT;

  bp::class_<aos2::Integer>("Integer")
    .def(bp::init<>())
    .def(bp::init<int>())
    .def("value", &aos2::Integer::longValue)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(bp::self *= bp::self)
    ;

  bp::class_<aos2::Algebraic_real_1>("Algebraic_real_1")
    .def(bp::init<>())
    .def(bp::init<AR1&>())
    .def(bp::init<int>())
    .def(bp::init<AR1::Rational&>())
    .def(bp::init<const aos2::Polynomial_1&, AR1::Rational, AR1::Rational>())
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
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self != bp::self)
    .def(bp::self < bp::self)
    .def(bp::self > bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    ;

  bp::class_<aos2::Bound>("Bound")
    .def(bp::init<>())
    .def("value", &aos2::Bound::longValue)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self + bp::self)
    .def(bp::self += bp::self)
    .def(bp::self - bp::self)
    .def(bp::self -= bp::self)
    .def(bp::self *= bp::self)
    ;

  //bind_construct_polynomial<PT_1>("Construct_polynomial_1");
  //bind_construct_polynomial<PT_2>("Construct_polynomial_2");

  bind_polynomial<aos2::PT_1>("Polynomial_1");
  bind_polynomial<aos2::PT_2>("Polynomial_2");

  bind_shift<aos2::PT_1>("PT_1_Shift");
  bind_shift<aos2::PT_2>("PT_2_Shift");

  bind_swap<aos2::PT_1>("PT_1_Swap");
  bind_swap<aos2::PT_2>("PT_2_Swap");

  bp::def("ipower", &ipower<aos2::Polynomial_1>);
  bp::def("ipower", &ipower<aos2::Polynomial_2>);

  auto traits = bp::class_<GT>("Geometry_traits_2");
  bp::scope traits_scope(traits);
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
  p2_co
    .def("curve", &aos2::Point_2::curve)
    .def("arcno", &aos2::Point_2::arcno)
    .def("to_double", &to_double)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    .def(bp::self == bp::self)
    .def(bp::self != bp::self)
    .def(bp::self != bp::self)
    .def(bp::self < bp::self)
    .def(bp::self > bp::self)
    .def(bp::self <= bp::self)
    .def(bp::self >= bp::self)
    ;

  auto& xcv_co = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_co
    .def("curve", &aos2::X_monotone_curve_2::curve, bp::return_internal_reference<>())
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

  bp::class_<aos2::Construct_curve_2>("Construct_curve_2", bp::no_init)
    .def("__call__", &aos2::Construct_curve_2::operator());
  ;

  auto cp_2_binding = bp::class_<aos2::Construct_point_2>("Construct_point_2", bp::no_init);
  export_construct_point_2_call_operator<AR1, aos2::Curve_2, int>(cp_2_binding);
  export_construct_point_2_call_operator<AR1, aos2::X_monotone_curve_2>(cp_2_binding);
  export_construct_point_2_call_operator<AR1, AR1>(cp_2_binding);
  export_construct_point_2_call_operator<aos2::Bound, aos2::Bound>(cp_2_binding);
  export_construct_point_2_call_operator<int, int>(cp_2_binding);

  bp::class_<aos2::Construct_x_monotone_segment_2>("Construct_x_monotone_segment_2", bp::no_init)
    .def("__call__", &construct_x_monotone_segment_2_call_operator0)
    .def("__call__", &construct_x_monotone_segment_2_call_operator1)
    .def("__call__", &construct_x_monotone_segment_2_call_operator2)
    ;

  bp::enum_<GT::Site_of_point>("Site_of_point")
    .value("POINT_IN_INTERIOR", GT::Site_of_point::POINT_IN_INTERIOR)
    .value("MIN_ENDPOINT", GT::Site_of_point::MIN_ENDPOINT)
    .value("MAX_ENDPOINT", GT::Site_of_point::MAX_ENDPOINT)
    .export_values()
    ;

  return traits;
}
