// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/bind_polynomial.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

//
template<typename T> T ipower(T& p, int i) { return CGAL::ipower(p, i); }

//
template<typename T> T shift(T& p, int i) { return CGAL::shift(p, i); }

//
py::object export_arr_rational_function_traits(py::module_& m) {
  using GT = aos2::Geometry_traits_2;
  using Integer = GT::Integer;
  using Polynomial = GT::Polynomial_1;
  using Xcv = GT::X_monotone_curve_2;
  using AR1 = GT::Algebraic_real_1;
  using Bound = GT::Bound;

  py::class_<GT> traits_c(m, "Arr_rational_function_traits_2");
  traits_c.def(py::init<>())
    .def("construct_x_monotone_curve_2_object",
         &GT::construct_x_monotone_curve_2_object)
    ;

  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
  };
  Concepts concepts;
  export_AosTraits_2<GT>(traits_c, concepts);

  using Ctr_xcv = GT::Construct_x_monotone_curve_2;
  using ctr_xcv_op0 = Xcv(Ctr_xcv::*)(const Polynomial&)const;
  using ctr_xcv_op1 = Xcv(Ctr_xcv::*)(const Polynomial&,
                                      const AR1&, const AR1&)const;
  using ctr_xcv_op2 = Xcv(Ctr_xcv::*)(const Polynomial&, const Polynomial&,
                                      const AR1&, const AR1&)const;

  py::class_<Ctr_xcv>(traits_c, "Construct_x_monotone_curve_2")
    .def("__call__", static_cast<ctr_xcv_op0>(&Ctr_xcv::operator()))
    .def("__call__", static_cast<ctr_xcv_op1>(&Ctr_xcv::operator()))
    .def("__call__", static_cast<ctr_xcv_op2>(&Ctr_xcv::operator()))
    ;

  py::class_<Integer> integer_c(m, "Integer");
  integer_c.def(py::init<>())
    .def(py::init_implicit<int>())
    .def("value", &Integer::longValue)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self *= py::self)
    ;

  add_insertion(integer_c, "__str__");
  add_insertion(integer_c, "__repr__");

  py::class_<AR1> ar1_c(m, "Algebraic_real_1");
  ar1_c.def(py::init<>())
    .def(py::init<AR1&>())
    .def(py::init_implicit<int>())
    .def(py::init_implicit<Bound>())
    .def(py::init<AR1::Rational&>())
    .def(py::init<const aos2::Polynomial_1&, AR1::Rational, AR1::Rational>())
    .def("bisect", &AR1::bisect)
    .def<CGAL::Comparison_result(AR1::*)(const AR1&) const>("compare", &AR1::compare)
    .def("degree", &AR1::degree)
    .def("high", &AR1::high)
    .def("is_rational", &AR1::is_rational)
    .def("is_root_of", &AR1::is_root_of)
    .def("low", &AR1::low)
    .def("polynomial", &AR1::polynomial)
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

  add_insertion(ar1_c, "__str__");
  add_insertion(ar1_c, "__repr__");

  py::class_<Bound> bound_c(m, "Bound");
  bound_c.def(py::init<>())
    .def(py::init<double>())
    .def("value", &Bound::longValue)
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self *= py::self)
    ;

  add_insertion(bound_c, "__str__");
  add_insertion(bound_c, "__repr__");

  bind_polynomial<aos2::PT_1>(m, "Polynomial_1");
  m.def("shift", &shift<Polynomial>);
  m.def("ipower", &ipower<Polynomial>);

  auto& pnt_c = *(concepts.m_basic_traits_classes.m_point_2);
  add_insertion(pnt_c, "__str__");
  add_insertion(pnt_c, "__repr__");

 auto& xcv_c = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  add_insertion(xcv_c, "__str__");
  add_insertion(xcv_c, "__repr__");

  // Add convenient attributes:
  add_attr<Integer>(traits_c, "Integer");
  add_attr<Integer>(traits_c, "Polynomial_1");
  add_attr<Integer>(traits_c, "Algebraic_real_1");
  add_attr<Integer>(traits_c, "Bound");

  return traits_c;
}
