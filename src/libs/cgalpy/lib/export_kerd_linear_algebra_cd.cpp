// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"

//!
void export_linear_algebra_cd(py::class_<Linear_algebra_cartesian_d>& lacd_c) {
  using Lacd = Linear_algebra_cartesian_d;
  using Matd = Lacd::Matrix;
  using Vecd = Lacd::Vector;

  if (! add_attr<Vecd>(lacd_c, "Vector")) {
    py::class_<Vecd> vecd_c(lacd_c, "Vector");
    vecd_c.def(py::init<>())
      .def(py::init<int>())
      .def("__getitem__", [](const Vecd& v, int i){ return v[i]; })
      .def("__setitem__", [](Vecd& v, int i, const FT_d& val){ v[i] = val; })
      ;
  }

  if (! add_attr<Matd>(lacd_c, "Matrix")) {
    py::class_<Matd> matd_c(lacd_c, "Matrix");
    matd_c.def(py::init<>())
      .def(py::init<int>())
      .def(py::init<int, int>())
      .def("__getitem__", [](const Matd& m, int i, int j){ return m(i, j); })
      .def("__setitem__", [](Matd& m, int i, int j, const FT_d& val){ m(i, j) = val; })
        ;
  }

  lacd_c.def(py::init<>())
    .def_static("determinant", py::overload_cast<const Matd&>(&Lacd::determinant))
    ;
}
