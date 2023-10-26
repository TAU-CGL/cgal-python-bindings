// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_segment_traits_2.h>

#include "CGALPY/envelope_3_types.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

void export_env_tri_traits_3(py::module_& m) {
  using Aos2_gt = CGAL::Arr_segment_traits_2<Kernel>;
  using Gt = env3::Geometry_traits_3;

  if (add_attr<Gt>(m, "Env_triangle_traits_3")) return;

  py::class_<Gt, Aos2_gt> traits_c(m, "Env_triangle_traits_3");
}
