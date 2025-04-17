// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/kernel_type.hpp"
#include "CGALPY/Triangulation_cw_ccw_2.hpp"
#include "CGALPY/Triangulation_2.hpp"
#include "CGALPY/triangulation_2_types.hpp"

namespace py = nanobind;

void export_2d_range_and_neighbor_search(py::module_& m) {
  using K = Kernel;
  using Tri2 = CGAL::Triangulation_2<K>;
  using Tricc2 = CGAL::Triangulation_cw_ccw_2;

  if (! add_attr<Tricc2>(m, "Triangulation_cw_ccw_2")) {
    py::class_<Tricc2> cw_ccw_c(m, "Triangulation_cw_ccw_2");
    tri2::export_triangulation_cw_ccw_2<Tri2>(cw_ccw_c);
  }

  if (! add_attr<Tri2>(m, "Triangulation_2")) {
    py::class_<Tri2, Tricc2> tri_c(m, "Triangulation_2");
    tri2::export_triangulation_2<Tri2>(tri_c);
  }
}
