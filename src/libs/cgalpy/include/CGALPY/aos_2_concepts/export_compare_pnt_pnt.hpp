// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_AOS_COMPARE_PNT_PNT_HPP
#define CGALPY_EXPORT_AOS_COMPARE_PNT_PNT_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/aos_2_concepts/has_op.hpp"

template <typename Gt, typename Functor>
static void impl(py::class_<Functor>& functor_c, std::false_type) {}

template <typename Gt, typename Functor>
static void impl(py::class_<Functor>& functor_c, Cmp_result_pnt_pnt) {
  using Pnt = typename Gt::Point_2;
  functor_c.def("__call__", py::overload_cast<const Pnt&, const Pnt&>
                (&Functor::operator(), py::const_));
}

template <typename Gt, typename Functor>
void export_compare_pnt_pnt(py::class_<Functor>& functor_c)
{ impl<Gt>(functor_c, typename has_op<Functor, Gt>::type()); }

#endif
