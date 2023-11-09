// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/pair.h>

#include <CGAL/Algebraic_kernel_d_1.h>
#include <CGAL/Get_arithmetic_kernel.h>

#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

typedef CORE::BigInt                            Integer;
typedef CGAL::Algebraic_kernel_d_1<Integer>     Alg_kernel;

//
py::list solve_1(const Alg_kernel::Solve_1& solver,
                 const Alg_kernel::Polynomial_1& polynomial) {
  using Alg_real = Alg_kernel::Algebraic_real_1;
  using Isolator = Alg_kernel::Isolator;

  py::list lst;
  auto op =
    [&](const std::pair<Alg_real, int>& root) mutable { lst.append(root); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  solver(polynomial, it);
  return lst;
}

//
void export_alg_real(py::module_& m) {
  using Ar = Alg_kernel::Algebraic_real_1;
  using Polynomial = Alg_kernel::Polynomial_1;
  using Arithmetic_kernel =
    CGAL::Get_arithmetic_kernel<Integer>::Arithmetic_kernel;
  using Rational = Arithmetic_kernel::Rational;
  using Bound = Arithmetic_kernel::Rational;

  using Cmp = CGAL::Comparison_result(Ar::*)(const Ar&) const;

  if (add_attr<Ar>(m, "Algebraic_real_1")) return;

  py::class_<Ar> ar1_c(m, "Algebraic_real_1");
  ar1_c.def(py::init<>())
    .def(py::init<Ar&>())
    .def(py::init_implicit<int>())
    .def(py::init_implicit<Bound>())
    .def(py::init<const Rational&>())
    .def(py::init<const Polynomial&, const Rational&, const Rational&>())
    .def("bisect", &Ar::bisect)
    .def("compare", static_cast<Cmp>(&Ar::compare))
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

//
void export_kernal_algebraic(py::module_& m) {
  using Integer = CORE::BigInt;
  using Alg_kernel = CGAL::Algebraic_kernel_d_1<Integer>;
  using Polynomial = Alg_kernel::Polynomial_1;
  using Ar = Alg_kernel::Algebraic_real_1;
  using Arithmetic_kernel =
    CGAL::Get_arithmetic_kernel<Integer>::Arithmetic_kernel;
  using Bound = Arithmetic_kernel::Rational;
  using Rational = Arithmetic_kernel::Rational;
  using Solve = Alg_kernel::Solve_1;

  if (add_attr<Alg_kernel>(m, "Algebraic_kernel_d_1")) return;

  py::class_<Alg_kernel> ak_c(m, "Algebraic_kernel_d_1");
  ak_c.def(py::init<>())
    .def("solve_1_object",
           [](const Alg_kernel& k)->Solve { return k.solve_1_object(); })
    ;

  py::class_<Alg_kernel::Solve_1>(ak_c, "Solve_1")
    .def("__call__", &solve_1)
    ;

  m.def("to_double", &CGAL::to_double<Ar>);

  export_alg_real(m);
}
