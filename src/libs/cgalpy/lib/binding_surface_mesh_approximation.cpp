// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Surface_mesh_approximation/approximate_triangle_mesh.h>

#include "cgalpy/sm/surface_mesh_approximation_types.hpp"

namespace py = nanobind;

namespace {

namespace Sma = CGAL::Surface_mesh_approximation;

} // namespace

void export_surface_mesh_approximation(py::module_& m) {
  py::enum_<Sma::Verbose_level>(m, "Verbose_level")
    .value("SILENT", Sma::SILENT)
    .value("MAIN_STEPS", Sma::MAIN_STEPS)
    .value("VERBOSE", Sma::VERBOSE);
}
