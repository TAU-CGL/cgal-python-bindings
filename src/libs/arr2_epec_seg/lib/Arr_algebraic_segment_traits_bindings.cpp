#include <Common.h>
#ifdef ARR_ALGEBRAIC_SEGMENT_TRAITS

bp::list to_double(TPoint_2& p)
{
  bp::list lst = bp::list();
  auto pair = p.to_double();
  lst.append(pair.first);
  lst.append(pair.second);
  return lst;
}

template<typename T, typename S>
void export_Construct_point_2_call_operator(boost::python::class_<Construct_point_2>& cp_2_binding)
{
  namespace bp = boost::python;
  cp_2_binding.def<TPoint_2(Construct_point_2::*)(const T&, const S&)>("__call__", &Construct_point_2::operator());
}

template<typename T, typename S, typename R>
void export_Construct_point_2_call_operator(boost::python::class_<Construct_point_2>& cp_2_binding)
{
  namespace bp = boost::python;
  cp_2_binding.def<TPoint_2(Construct_point_2::*)(const T&, const S&, R)>("__call__", &Construct_point_2::operator());
}

template <typename PT>
boost::python::class_<typename PT::Construct_polynomial> bind_construct_polynomial(const char* name)
{
  typedef typename PT:Construct_polynomial T;
  using namespace boost::python;
  return class_<T>(name)
    .def(init<>)
    ;

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

template <typename PT>
boost::python::class_<typename PT::Type> bind_polynomial(const char* name)
{
  typedef typename PT::Type T;
  using namespace boost::python;
  return class_<T>(name)
    .def(init<>())
    .def(init<PT::Coefficient_type&>())
    .def(init<PT::Coefficient_type&, PT::Coefficient_type&>())
    .def(init<PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&>())
    .def(init<PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&>())
    .def(init<PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&>())
    .def(init<PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&>())
    .def(init<PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&,
      PT::Coefficient_type&>())
    .def(init<PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&, PT::Coefficient_type&,
      PT::Coefficient_type&, PT::Coefficient_type&>())
    //.def(init<Number&>())
    //.def(init<const Number&, const Number&, const Number&>())
    .def("abs", &T::abs)
    .def("coefficients", range<return_value_policy<copy_const_reference>>(&T::begin, &T::end))
    .def<CGAL::Comparison_result(T::*)(const T::NT&) const>("compare", &T::compare)
    .def("degree", &T::degree)
    .def("diff", &T::diff)
    .def("divide_by_x", &T::divide_by_x)
    .def("is_zero", &T::is_zero)
    .def("reversal", &T::reversal)
    .def("scalar_div", &T::scalar_div)
    .def("scale", &T::scale)
    .def("scale_down", &T::scale_down)
    .def("scale_up", &T::scale_up)
    .def("sign", &T::sign)
    .def<CGAL::Sign(T::*)(const T::NT&) const>("sign_at", &T::sign_at)
    .def("simplify_coefficients", &T::simplify_coefficients)
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self * self)
    .def(int() * self)
    .def(PT::Coefficient_type() * self)
    .def(self *= self)
    .def("__getitem__", &T::operator[], return_value_policy<copy_const_reference>())
    .def(self_ns::str(self_ns::self))
    ;
}

void export_Arr_algebraic_segment_traits()
{
  using namespace boost::python;
  class_<Number>("Number")
    .def(init<>())
    .def(init<int>())
    .def("value", &Number::longValue)
    .def(self_ns::str(self_ns::self))
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
    .def(self + self)
    .def(self += self)
    .def(self - self)
    .def(self -= self)
    .def(self *= self)
    ;


  class_<Construct_polynomial_2>("Construct_polynomial_2")
    .def(init<>())
    .def<Polynomial_2(Construct_polynomial_2::*)(const Polynomial_1&, const Polynomial_1&) const>("__call__", &Construct_polynomial_2::operator())
    ;

  class_<Construct_polynomial_1>("Construct_polynomial_1")
    .def(init<>())
    .def<Polynomial_1(Construct_polynomial_1::*)(const Number&, const Number&) const>("__call__", &Construct_polynomial_1::operator())
    ;

  bind_polynomial<PT_1>("Polynomial_1");
  bind_polynomial<PT_2>("Polynomial_2");

  class_<TPoint_2>("TPoint_2")
    .def(init<TPoint_2&>())
    .def("curve", &TPoint_2::curve)
    .def("arcno", &TPoint_2::arcno)
    .def("to_double", &to_double)
    .def(self_ns::str(self_ns::self))
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
  export_Construct_point_2_call_operator<Algebraic_real_1, Curve_2, int>(cp_2_binding);
  export_Construct_point_2_call_operator<Algebraic_real_1, X_monotone_curve_2>(cp_2_binding);
  export_Construct_point_2_call_operator<Algebraic_real_1, Algebraic_real_1>(cp_2_binding);
  export_Construct_point_2_call_operator<Bound, Bound>(cp_2_binding);
  export_Construct_point_2_call_operator<int, int>(cp_2_binding);

  class_<Construct_x_monotone_segment_2>("Construct_x_monotone_segment_2", no_init)
    .def("__call__", &construct_x_monotone_segment_2_call_operator0)
    .def("__call__", &construct_x_monotone_segment_2_call_operator1)
    ;

  class_<Traits>("Traits")
    .def(init<>())
    .def("construct_curve_2_object", &Traits::construct_curve_2_object)
    .def("construct_tpoint_2_object", &Traits::construct_point_2_object)
    .def("construct_x_monotone_segment_2_object", &Traits::construct_x_monotone_segment_2_object)
    ;

  
}

#endif
