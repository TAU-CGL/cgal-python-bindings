// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_BIND_POLYNOMIAL_HPP
#define CGALPY_BIND_POLYNOMIAL_HPP

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include <CGAL/Polynomial.h>
#include <CGAL/Polynomial_traits_d.h>
#include <CGAL/Polynomial_type_generator.h>

#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

template <typename PT>
void init_polynomial(typename PT::Type& pol, py::list& lst) {
  using P = typename PT::Type;
  using CT = typename PT::Coefficient_type;
  auto begin = stl_input_iterator<CT>(lst);
  auto end = stl_input_iterator<CT>(lst, false);
  new (&pol) P(begin, end);
}

template <typename PT>
py::class_<typename PT::Type> bind_polynomial(py::module_& m, const char* name) {
  using P = typename PT::Type;
  using CT = typename PT::Coefficient_type ;
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

    // plus (+)
    .def(py::self + py::self)
    .def(int() + py::self)
    .def(py::self + int())
    .def(CT() + py::self)
    .def(py::self + CT())
    .def(py::self += py::self)

    // minus (-)
    .def(py::self - py::self)
    .def(int() - py::self)
    .def(py::self - int())
    .def(CT() - py::self)
    .def(py::self - CT())
    .def(py::self -= py::self)

    // multiply
    .def(py::self * py::self)
    .def(int() * py::self)
    .def(py::self * int())
    .def(CT() * py::self)
    .def(py::self * CT())
    .def(py::self *= py::self)

    // Unary
    .def(-py::self)

    .def("__getitem__", &P::operator[])
    ;

  using Ci = typename P::const_iterator;
  add_iterator<Ci, Ci, const CT&>("Coefficient_iterator", c);
  c.def("coefficients",
        [](const P& polynomial)
        { make_iterator(polynomial.begin(), polynomial.end()); },
        py::keep_alive<0, 1>());

    // .def("coefficients", py::range<Copy_const_reference>(&P::begin, &P::end))
  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");

  return c;
}

#endif
