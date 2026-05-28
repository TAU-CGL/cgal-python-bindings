// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_OP_PNT_PNT_HPP
#define CGALPY_EXPORT_OP_PNT_PNT_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/aos_2_concepts/has_op_pnt_pnt.hpp"

template <typename Gt, typename Functor>
static void impl(py::class_<Functor>& functor_c, std::false_type) {}

template <typename Gt, typename Functor>
static void impl(py::class_<Functor>& functor_c, Op_pnt_pnt_result) {
  using Pnt = typename Gt::Point_2;
  functor_c.def("__call__", py::overload_cast<const Pnt&, const Pnt&>
                (&Functor::operator(), py::const_),
                py::arg("p1"), py::arg("p2"),
                "Call the functor on two points.");
}

template <typename Gt, typename Functor>
void export_op_pnt_pnt(py::class_<Functor>& functor_c)
{ impl<Gt>(functor_c, typename has_op_pnt_pnt<Functor, Gt>::type()); }

#endif
