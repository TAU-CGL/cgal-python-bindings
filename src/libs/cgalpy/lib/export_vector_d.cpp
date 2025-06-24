// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>

#include "CGALPY/kernel_d_types.hpp"

//!
void export_vector_d(py::class_<Vector_d>& vecd_c) {
  using Kerd = Kernel_d;
  using Vecd = Vector_d;

  vecd_c.def(py::init<>())
    .def("__getitem__", &Vecd::operator[])
    ;
}
