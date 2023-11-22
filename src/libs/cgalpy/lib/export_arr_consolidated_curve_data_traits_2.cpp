// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_curve_data_traits_2.h>
#include <CGAL/Arr_consolidated_curve_data_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

void export_arr_consolidated_curve_data_traits_2(py::module_& m) {
  using Gt = aos2::Ccd_geometry_traits_2;
  using Base_gt = aos2::Cd_geometry_traits_2;

  if (! add_attr<Gt>(m, "Arr_consolidated_curve_data_traits_2")) {
    py::class_<Gt, Base_gt> ccdt_c(m, "Arr_consolidated_curve_data_traits_2");
    ccdt_c.def(py::init<>());
  }
}
