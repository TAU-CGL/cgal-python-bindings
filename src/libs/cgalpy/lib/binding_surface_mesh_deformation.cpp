// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Surface_mesh_deformation.h>

#include "cgalpy/sm/surface_mesh_deformation_types.hpp"

namespace py = nanobind;

void export_surface_mesh_deformation(py::module_& m) {
  py::enum_<CGAL::Deformation_algorithm_tag>(m, "Deformation_algorithm_tag")
    .value("ORIGINAL_ARAP", CGAL::ORIGINAL_ARAP)
    .value("SPOKES_AND_RIMS", CGAL::SPOKES_AND_RIMS)
    .value("SRE_ARAP", CGAL::SRE_ARAP);
}
