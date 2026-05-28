// Copyright (c) 2024 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_counting_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

//
void export_arr_counting_traits_2(py::module_& m) {
  using Gt = cgalpy::aos2::Cnt_geometry_traits_2;
  using Base_gt = Gt::Base;

  if (add_attr<Gt>(m, "Arr_counting_traits_2")) return;

  py::class_<Gt, Base_gt> traits_c(m, "Arr_counting_traits_2");
  traits_c.def(py::init<>());
}
