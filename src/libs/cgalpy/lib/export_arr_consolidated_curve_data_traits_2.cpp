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
#include "CGALPY/export_arr_curve_data_traits_2.h"

namespace py = nanobind;

void export_arr_consolidated_curve_data_traits_2(py::module_& m) {
  using Aos_wh = aos2::Arrangement_on_surface_with_history_2;
  using Gt = Aos_wh::Geometry_traits_2;
  using Data = Gt::Curve_2*;
  using Ccgt = CGAL::Arr_consolidated_curve_data_traits_2<Gt, Data>;
  using Ul = CGAL::_Unique_list<Data>;
  using Cul = CGAL::_Consolidate_unique_lists<Data>;
  using Cgt = CGAL::Arr_curve_data_traits_2<Gt, Ul, Cul, Data>;

  using Base_cv = Cgt::Curve_2;
  using Cv = Ccgt::Curve_2;
  using Base_xcv = Cgt::X_monotone_curve_2;
  using Xcv = Ccgt::X_monotone_curve_2;

  export_arr_curve_data_traits_2<Cgt>(m);

  if (! add_attr<Ccgt>(m, "Arr_consolidated_curve_data_traits_2")) {
    py::class_<Ccgt, Cgt> ccdt_c(m, "Arr_consolidated_curve_data_traits_2");
    ccdt_c.def(py::init<>());
  }
}
