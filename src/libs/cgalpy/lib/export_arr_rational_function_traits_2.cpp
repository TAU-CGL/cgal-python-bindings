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
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

extern void export_kernel_algebraic(py::module_&);

//
template<typename T> T ipower(T& p, int i) { return CGAL::ipower(p, i); }

//
template<typename T> T shift(T& p, int i) { return CGAL::shift(p, i); }

using Integer = CORE::BigInt;
using Alg_kernel = CGAL::Algebraic_kernel_d_1<Integer>;
using Gt = CGAL::Arr_rational_function_traits_2<Alg_kernel>;
using Cv = Gt::Curve_2;
using Xcv = Gt::X_monotone_curve_2;
using Ctr_cv = Gt::Construct_curve_2;
using Ctr_xcv = Gt::Construct_x_monotone_curve_2;
using Alg_real = Gt::Algebraic_real_1;
using Rational = Gt::Rational;
using Bound = Gt::Bound;

/* Construct a curve from a list of rationals, a bound, and a Boolean
 */
Cv ctr_cv_from_rats(const Ctr_cv& ctr, const py::list& rats,
                  const Alg_real& x_s, bool dir_right) {
  auto begin = stl_input_iterator<Rational>(rats);
  auto end = stl_input_iterator<Rational>(rats, false);
  return ctr(begin, end, x_s, dir_right);
}

/* Construct an `x`-monotone curve from a list of rational numerators, a
 * list of rational denominators, and two bounds.
 */
Xcv ctr_xcv_from_rats(const Ctr_xcv& ctr,
                   const py::list& numers, const py::list& demons,
                   const Alg_real& x_s, const Alg_real& x_t) {
  auto begin1 = stl_input_iterator<Rational>(numers);
  auto end1 = stl_input_iterator<Rational>(numers, false);
  auto begin2 = stl_input_iterator<Rational>(demons);
  auto end2 = stl_input_iterator<Rational>(demons, false);
  return ctr(begin1, end1, begin2, end2, x_s, x_t);
}

/*! Export a big integera type.
 * \todo Export the arithmetic kernel and its related types, e.g., Integer,
 * Rational, and Bound separately.
*/
void export_integer(py::module_& m) {
  py::class_<Integer> integer_c(m, "Integer");
  integer_c.def(py::init<>())
    .def(py::init_implicit<int>())
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self *= py::self)
    ;

  add_insertion(integer_c, "__str__");
  add_insertion(integer_c, "__repr__");
}

//
void export_rational(py::module_& m) {
  py::class_<Rational> rational_c(m, "Rational");
  rational_c.def(py::init<>())
    .def(py::init<const Integer&, const Integer&>())
    .def(py::init_implicit<double>())
    .def(py::init_implicit<int>())
    .def(py::init_implicit<const Integer&>())
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self *= py::self)
    ;

  add_insertion(rational_c, "__str__");
  add_insertion(rational_c, "__repr__");
}

//
void export_bound(py::module_& m) {
  py::class_<Bound> bound_c(m, "Bound");
  bound_c.def(py::init<>())
    .def(py::init<double>())
    .def(py::init_implicit<int>())
    .def(py::init_implicit<const Integer&>())
    .def(py::self + py::self)
    .def(py::self += py::self)
    .def(py::self - py::self)
    .def(py::self -= py::self)
    .def(py::self *= py::self)
    ;

  add_insertion(bound_c, "__str__");
  add_insertion(bound_c, "__repr__");
}

/*! Export the Arr_rational_traits_2 traits class instance.
 * \todo Combine the calls here with the calls in the algebraic traits.
 */
void export_arr_rational_function_traits_2(py::module_& m) {
  using Integer = CORE::BigInt;
  using Alg_kernel = CGAL::Algebraic_kernel_d_1<Integer>;
  using Gt = CGAL::Arr_rational_function_traits_2<Alg_kernel>;
  using Rational = Gt::Rational;
  using Polynomial = Gt::Polynomial_1;
  using Pnt = Gt::Point_2;
  using Cv = Gt::Curve_2;
  using Xcv = Gt::X_monotone_curve_2;
  using Ar = Gt::Algebraic_real_1;
  using Bound = Gt::Bound;
  using Pt = Gt::Polynomial_traits_1;

  // Export the algebraic kernel.
  //\ This should probably move to somewhere else.
  export_kernel_algebraic(m);

  // Export the rational-function traits itself.
  if (add_attr<Gt>(m, "Arr_rational_function_traits_2")) return;

  py::class_<Gt> traits_c(m, "Arr_rational_function_traits_2");
  traits_c.def(py::init<>())
    .def(py::init<const Gt&>())
    .def(py::init<Alg_kernel*>())
    .def("construct_curve_2_object", &Gt::construct_curve_2_object)
    .def("construct_x_monotone_curve_2_object",
         &Gt::construct_x_monotone_curve_2_object)
    ;

  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
  };
  Concepts concepts;
  export_AosTraits_2<Gt>(traits_c, concepts);

  // Export additional point attributes:
  auto& pnt_c = *(concepts.m_aos_basic_traits_2_classes.m_point_2);
  pnt_c.def("x", py::overload_cast<>(&Pnt::x, py::const_))
    .def("y", py::overload_cast<>(&Pnt::y, py::const_))
    ;
  add_insertion(pnt_c, "__str__");
  add_insertion(pnt_c, "__repr__");

  // Export additional curve attributes:
  //! \todo Export missing constructors (operator()).
  using Ctr_cv = Gt::Construct_curve_2;

  using ctr_cv_op0 = Cv(Ctr_cv::*)(const Polynomial&)const;
  using ctr_cv_op1 = Cv(Ctr_cv::*)(const Polynomial&,
                                   const Ar&, const Ar&)const;
  using ctr_cv_op2 = Cv(Ctr_cv::*)(const Polynomial&, const Polynomial&)const;
  using ctr_cv_op3 = Cv(Ctr_cv::*)(const Polynomial&, const Polynomial&,
                                   const Ar&, bool)const;

  py::class_<Ctr_cv>(traits_c, "Construct_curve_2")
    .def("__call__", static_cast<ctr_cv_op0>(&Ctr_cv::operator()))
    .def("__call__", static_cast<ctr_cv_op1>(&Ctr_cv::operator()))
    .def("__call__", static_cast<ctr_cv_op2>(&Ctr_cv::operator()))
    .def("__call__", static_cast<ctr_cv_op3>(&Ctr_cv::operator()))
    .def("__call__", &ctr_cv_from_rats)
    ;

  // Export additional x-monotone curve attributes:
  //! \todo Export missing constructors (operator()).
  using Ctr_xcv = Gt::Construct_x_monotone_curve_2;
  using ctr_xcv_op0 = Xcv(Ctr_xcv::*)(const Polynomial&)const;
  using ctr_xcv_op1 = Xcv(Ctr_xcv::*)(const Polynomial&,
                                      const Ar&, const Ar&)const;
  using ctr_xcv_op2 = Xcv(Ctr_xcv::*)(const Polynomial&, const Polynomial&,
                                      const Ar&, const Ar&)const;

  py::class_<Ctr_xcv>(traits_c, "Construct_x_monotone_curve_2")
    .def("__call__", static_cast<ctr_xcv_op0>(&Ctr_xcv::operator()))
    .def("__call__", static_cast<ctr_xcv_op1>(&Ctr_xcv::operator()))
    .def("__call__", static_cast<ctr_xcv_op2>(&Ctr_xcv::operator()))
    .def("__call__", &ctr_xcv_from_rats)
    ;

  auto& xcv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
  add_insertion(xcv_c, "__str__");
  add_insertion(xcv_c, "__repr__");

  // Export relevant attributes:
  if (! add_attr<Integer>(m, "Integer")) export_integer(m);
  if (! add_attr<Rational>(m, "Rational")) export_rational(m);
  if (! add_attr<Bound>(m, "Bound")) export_bound(m);

  if (! add_attr<Polynomial>(m, "Polynomial_1"))
    bind_polynomial<Pt>(m, "Polynomial_1");
  m.def("shift", &shift<Polynomial>);
  m.def("ipower", &ipower<Polynomial>);

  // Add convenient attributes to the module:
  add_attr<Integer>(traits_c, "Integer");
  add_attr<Rational>(traits_c, "Rational");
  add_attr<Bound>(traits_c, "Bound");
  add_attr<Ar>(m, "Algebraic_real_1");
  add_attr<Polynomial>(traits_c, "Polynomial_1");

  // Add convenient attributes to the traits:
  add_attr<Integer>(traits_c, "Integer");
  add_attr<Rational>(traits_c, "Rational");
  add_attr<Bound>(traits_c, "Bound");
  add_attr<Ar>(traits_c, "Algebraic_real_1");
  add_attr<Polynomial>(traits_c, "Polynomial_1");
}
