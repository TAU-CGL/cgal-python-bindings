// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

bp::list to_double(TPoint_2& p)
{
  bp::list lst = bp::list();
  auto pair = p.to_double();
  lst.append(pair.first);
  lst.append(pair.second);
  return lst;
}

template<typename T, typename S>
void export_construct_point_2_call_operator(boost::python::class_<Construct_point_2>& cp_2_binding)
{
  namespace bp = boost::python;
  cp_2_binding.def<TPoint_2(Construct_point_2::*)(const T&, const S&)>("__call__", &Construct_point_2::operator());
}

template<typename T, typename S, typename R>
void export_construct_point_2_call_operator(boost::python::class_<Construct_point_2>& cp_2_binding)
{
  namespace bp = boost::python;
  cp_2_binding.def<TPoint_2(Construct_point_2::*)(const T&, const S&, R)>("__call__", &Construct_point_2::operator());
}

void construct_x_monotone_segment_2_call_operator0(Construct_x_monotone_segment_2& construct, Curve_2& cv, TPoint_2& end_min, TPoint_2& end_max, bp::list& lst)
{
  auto v = std::vector<X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(cv, end_min, end_max, it);
  for (auto xcv : v)
  {
    lst.append(xcv);
  }
}

void construct_x_monotone_segment_2_call_operator1(Construct_x_monotone_segment_2& construct, TPoint_2& p, TPoint_2& q, bp::list& lst)
{
  auto v = std::vector<X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(p, q, it);
  for (auto xcv : v)
  {
    lst.append(xcv);
  }
}

void construct_x_monotone_segment_2_call_operator2(Construct_x_monotone_segment_2& construct, Curve_2& cv, TPoint_2& p, Traits::Site_of_point& site_of_p, bp::list& lst)
{
  auto v = std::vector<X_monotone_curve_2>();
  auto it = std::back_inserter(v);
  construct(cv, p, site_of_p, it);
  for (auto xcv : v)
  {
    lst.append(xcv);
  }
}

//template <typename PT>
//boost::python::class_<typename PT::Construct_polynomial> bind_construct_polynomial(const char* name)
//{
//  typedef typename PT::Construct_polynomial T;
//  typedef typename PT::Type P;
//  using namespace boost::python;
//  return class_<T>(name)
//    .def(init<>())
//    ;
//}

template <typename PT>
typename PT::Type* init_polynomial(bp::list& lst)
{
  typedef typename PT::Type P;
  typedef typename PT::Coefficient_type CT;
  bp::stl_input_iterator<CT> begin(lst), end;
  return new P(begin, end);
}

template <typename PT>
bp::class_<typename PT::Type> bind_polynomial(const char* name)
{
  typedef typename PT::Type P;
  typedef typename PT::Coefficient_type CT;
  using namespace boost::python;
  return class_<P>(name)
    .def(init<>())
    .def(init<CT&>())
    .def(init<CT&, CT&>())
    .def(init<CT&, CT&, CT&>())
    .def(init<CT&, CT&, CT&, CT&>())
    .def(init<CT&, CT&, CT&, CT&, CT&>())
    .def(init<CT&, CT&, CT&, CT&, CT&, CT&>())
    .def(init<CT&, CT&, CT&, CT&, CT&, CT&, CT&>())
    .def(init<CT&, CT&, CT&, CT&, CT&, CT&, CT&, CT&>())
    .def("__init__", make_constructor(&init_polynomial<PT>))
    .def("abs", &P::abs)
    .def("coefficients", range<return_value_policy<copy_const_reference>>(&P::begin, &P::end))
    .def<CGAL::Comparison_result(P::*)(const P::NT&) const>("compare", &P::compare)
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
    .def<CGAL::Sign(P::*)(const P::NT&) const>("sign_at", &P::sign_at)
    .def("simplify_coefficients", &P::simplify_coefficients)
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self * self)
    .def(int() * self)
    .def(CT() * self)
    .def(self *= self)
    .def("__getitem__", &P::operator[], return_value_policy<copy_const_reference>())
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    ;
}

template<typename PT>
bp::class_<typename PT::Shift> bind_shift(const char* name)
{
  return class_<typename PT::Shift>(name)
    .def(init<>())
    .def("__call__", &PT::Shift::operator())
    ;
}

template<typename PT>
bp::class_<typename PT::Swap> bind_swap(const char* name)
{
  return class_<typename PT::Swap>(name)
    .def(init<>())
    .def("__call__", &PT::Swap::operator())
    ;
}

template<typename T>
T ipower(T& p, int i)
{
  return CGAL::ipower(p, i);
}

void export_arr_algebraic_segment_traits()
{
  using namespace boost::python;
  class_<Integer>("Integer")
    .def(init<>())
    .def(init<int>())
    .def("value", &Integer::longValue)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self *= self)
    ;

  class_<Algebraic_real_1>("Algebraic_real_1")
    .def(init<>())
    .def(init<Algebraic_real_1&>())
    .def(init<int>())
    .def(init<Algebraic_real_1::Rational&>())
    .def(init<const Polynomial_1&, Algebraic_real_1::Rational, Algebraic_real_1::Rational>())
    .def("bisect", &Algebraic_real_1::bisect)
    .def< CGAL::Comparison_result(Algebraic_real_1::*)(const Algebraic_real_1&) const>("compare", &Algebraic_real_1::compare)
    .def("degree", &Algebraic_real_1::degree)
    .def("high", &Algebraic_real_1::high)
    .def("is_rational", &Algebraic_real_1::is_rational)
    .def("is_root_of", &Algebraic_real_1::is_root_of)
    .def("low", &Algebraic_real_1::low)
    .def("polynomial", &Algebraic_real_1::polynomial, return_value_policy<copy_const_reference>())
    .def("rational", &Algebraic_real_1::rational)
    .def("rational_between", &Algebraic_real_1::rational_between)
    .def("refine", &Algebraic_real_1::refine)
    .def("refine_to", &Algebraic_real_1::refine_to)
    .def("sign_at_low", &Algebraic_real_1::sign_at_low)
    .def("simplify", &Algebraic_real_1::simplify)
    .def("to_double", &Algebraic_real_1::to_double)
    .def("upper", &Algebraic_real_1::upper)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    ;

  class_<Bound>("Bound")
    .def(init<>())
    .def("value", &Bound::longValue)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self *= self)
    ;

  enum_<Traits::Site_of_point>("Site_of_point")
    .value("POINT_IN_INTERIOR", Traits::Site_of_point::POINT_IN_INTERIOR)
    .value("MIN_ENDPOINT", Traits::Site_of_point::MIN_ENDPOINT)
    .value("MAX_ENDPOINT", Traits::Site_of_point::MAX_ENDPOINT)
    .export_values()
    ;

  //bind_construct_polynomial<PT_1>("Construct_polynomial_1");
  //bind_construct_polynomial<PT_2>("Construct_polynomial_2");

  bind_polynomial<PT_1>("Polynomial_1");
  bind_polynomial<PT_2>("Polynomial_2");

  bind_shift<PT_1>("PT_1_Shift");
  bind_shift<PT_2>("PT_2_Shift");

  bind_swap<PT_1>("PT_1_Swap");
  bind_swap<PT_2>("PT_2_Swap");

  class_<TPoint_2>("TPoint_2")
    .def(init<TPoint_2&>())
    .def("curve", &TPoint_2::curve)
    .def("arcno", &TPoint_2::arcno)
    .def("to_double", &to_double)
    .def(self_ns::str(self_ns::self))
    .def(self_ns::repr(self_ns::self))
    .def(self == self)
    .def(self != self)
    .def(self != self)
    .def(self < self)
    .def(self > self)
    .def(self <= self)
    .def(self >= self)
    ;

  class_<Curve_2>("Curve_2", no_init)
    .def("polynomial_2", &Curve_2::polynomial_2)
    ;

  class_<X_monotone_curve_2>("X_monotone_curve_2", no_init)
    .def("curve", &X_monotone_curve_2::curve, return_internal_reference<>())
    .def("is_vertical", &X_monotone_curve_2::is_vertical)
    .def("is_finite", &X_monotone_curve_2::is_finite)
    .def("curve_end", &X_monotone_curve_2::curve_end)
    .def<int (X_monotone_curve_2::*)() const>("arcno", &X_monotone_curve_2::arcno)
    .def("x", &X_monotone_curve_2::x, return_value_policy<copy_const_reference>())
    ;

  class_<Construct_curve_2>("Construct_curve_2", no_init)
    .def("__call__", &Construct_curve_2::operator());
    ;

  auto cp_2_binding = class_<Construct_point_2>("Construct_tpoint_2", no_init);
  export_construct_point_2_call_operator<Algebraic_real_1, Curve_2, int>(cp_2_binding);
  export_construct_point_2_call_operator<Algebraic_real_1, X_monotone_curve_2>(cp_2_binding);
  export_construct_point_2_call_operator<Algebraic_real_1, Algebraic_real_1>(cp_2_binding);
  export_construct_point_2_call_operator<Bound, Bound>(cp_2_binding);
  export_construct_point_2_call_operator<int, int>(cp_2_binding);

  class_<Construct_x_monotone_segment_2>("Construct_x_monotone_segment_2", no_init)
    .def("__call__", &construct_x_monotone_segment_2_call_operator0)
    .def("__call__", &construct_x_monotone_segment_2_call_operator1)
    .def("__call__", &construct_x_monotone_segment_2_call_operator2)
    ;

  class_<Traits>("Traits")
    .def(init<>())
    .def("construct_curve_2_object", &Traits::construct_curve_2_object)
    .def("construct_tpoint_2_object", &Traits::construct_point_2_object)
    .def("construct_x_monotone_segment_2_object", &Traits::construct_x_monotone_segment_2_object)
    ;

  def("ipower", &ipower<Polynomial_1>);
  def("ipower", &ipower<Polynomial_2>);
}
