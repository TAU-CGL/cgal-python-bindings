// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Surface_mesh_parameterization/Error_code.h>

#include "cgalpy/sm/surface_mesh_parameterization_types.hpp"

namespace py = nanobind;

namespace {

namespace Smp = CGAL::Surface_mesh_parameterization;

} // namespace

void export_surface_mesh_parameterization(py::module_& m) {
  py::enum_<Smp::Error_code>(m, "Error_code")
    .value("OK", Smp::OK)
    .value("ERROR_EMPTY_MESH", Smp::ERROR_EMPTY_MESH)
    .value("ERROR_NON_TRIANGULAR_MESH", Smp::ERROR_NON_TRIANGULAR_MESH)
    .value("ERROR_NO_TOPOLOGICAL_DISC", Smp::ERROR_NO_TOPOLOGICAL_DISC)
    .value("ERROR_NO_TOPOLOGICAL_BALL", Smp::ERROR_NO_TOPOLOGICAL_BALL)
    .value("ERROR_BORDER_TOO_SHORT", Smp::ERROR_BORDER_TOO_SHORT)
    .value("ERROR_NON_CONVEX_BORDER", Smp::ERROR_NON_CONVEX_BORDER)
    .value("ERROR_CANNOT_SOLVE_LINEAR_SYSTEM", Smp::ERROR_CANNOT_SOLVE_LINEAR_SYSTEM)
    .value("ERROR_NO_1_TO_1_MAPPING", Smp::ERROR_NO_1_TO_1_MAPPING)
    .value("ERROR_WRONG_PARAMETER", Smp::ERROR_WRONG_PARAMETER);

  m.def("get_error_message",
        [](Smp::Error_code error_code) {
          return Smp::get_error_message(static_cast<int>(error_code));
        },
        py::arg("error_code"));

  m.def("get_error_message",
        [](int error_code) {
          return Smp::get_error_message(error_code);
        },
        py::arg("error_code"));
}
