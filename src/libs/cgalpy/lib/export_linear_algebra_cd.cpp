// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <cgalpy/Kernel_d_docstrings.hpp>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_d_types.hpp"

namespace doc = cgalpy::docstrings::Kernel_d;

//!
void export_linear_algebra_cd(py::class_<Linear_algebra_cartesian_d>& lacd_c) {
  using Lacd = Linear_algebra_cartesian_d;
  using Matd = Lacd::Matrix;
  using Vecd = Lacd::Vector;

  if (! add_attr<Vecd>(lacd_c, "Vector")) {
    py::class_<Vecd> vecd_c(lacd_c, "Vector");
    vecd_c.doc() = doc::Vector__class__;
    vecd_c.def(py::init<>(), doc::Vector_Vector)
      .def(py::init<int>(), doc::Vector_Vector_1)
      .def("__getitem__", [](const Vecd& v, int i){ return v[i]; }, doc::Vector_operator)
      .def("__setitem__", [](Vecd& v, int i, const FT_d& val){ v[i] = val; }, doc::Vector_operator_1)
      ;
  }

  if (! add_attr<Matd>(lacd_c, "Matrix")) {
    py::class_<Matd> matd_c(lacd_c, "Matrix");
    matd_c.doc() = doc::Matrix__class__;
    matd_c.def(py::init<>(), doc::Matrix_Matrix)
      .def(py::init<int>(), doc::Matrix_Matrix_1)
      .def(py::init<int, int>(), doc::Matrix_Matrix_2)
      .def("__getitem__",
           [](const Matd& m, py::tuple idx) {
             if (idx.size() != 2) throw std::runtime_error("Expected 2 indices");
             int i = py::cast<int>(idx[0]);
             int j = py::cast<int>(idx[1]);
             return m(i, j);
           }, doc::Matrix_operator)
      .def("__setitem__",
           [](Matd& m, py::tuple idx, const FT_d& val) {
             if (idx.size() != 2) throw std::runtime_error("Expected 2 indices");
             int i = py::cast<int>(idx[0]);
             int j = py::cast<int>(idx[1]);
             m(i, j) = val;
           }, doc::Matrix_operator_1)
        ;
  }

  lacd_c.def(py::init<>())
    .def_static("determinant", py::overload_cast<const Matd&>(&Lacd::determinant))
    ;
}
