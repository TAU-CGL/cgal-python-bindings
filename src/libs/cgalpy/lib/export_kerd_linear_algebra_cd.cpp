// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/Kerd_docstrings.hpp"

namespace kerd_doc = cgalpy::kerd::docstrings;

//!
void export_linear_algebra_cd(py::class_<Linear_algebra_cartesian_d>& lacd_c) {
  using Lacd = Linear_algebra_cartesian_d;
  using Matd = Lacd::Matrix;
  using Vecd = Lacd::Vector;

  if (! add_attr<Vecd>(lacd_c, "Vector")) {
    py::class_<Vecd> vecd_c(lacd_c, "Vector", kerd_doc::Vector_class);
    vecd_c.def(py::init<>(), kerd_doc::Vector_Vector)
      .def(py::init<int>(), py::arg("d"), kerd_doc::Vector_Vector_1)
      .def("__getitem__", [](const Vecd& v, int i){ return v[i]; },
           py::arg("i"), kerd_doc::Vector_operator_subscript)
      .def("__setitem__", [](Vecd& v, int i, const FT_d& val){ v[i] = val; },
           py::arg("i"), py::arg("value"),
           "Set the i-th component of the vector.")
      ;
  }

  if (! add_attr<Matd>(lacd_c, "Matrix")) {
    py::class_<Matd> matd_c(lacd_c, "Matrix", kerd_doc::Matrix_class);
    matd_c.def(py::init<>(), kerd_doc::Matrix_Matrix)
      .def(py::init<int>(), py::arg("n"), kerd_doc::Matrix_Matrix_1)
      .def(py::init<int, int>(), py::arg("m"), py::arg("n"),
           kerd_doc::Matrix_Matrix_2)
      .def("__getitem__", [](const Matd& m, int i, int j){ return m(i, j); },
           py::arg("i"), py::arg("j"), kerd_doc::Matrix_operator_call)
      .def("__setitem__", [](Matd& m, int i, int j, const FT_d& val){ m(i, j) = val; },
           py::arg("i"), py::arg("j"), py::arg("value"),
           "Set the matrix entry at row i and column j.")
        ;
  }

  lacd_c.def(py::init<>())
    .def_static("determinant", py::overload_cast<const Matd&>(&Lacd::determinant),
                py::arg("matrix"), "Compute the determinant of a matrix.")
    ;
}
