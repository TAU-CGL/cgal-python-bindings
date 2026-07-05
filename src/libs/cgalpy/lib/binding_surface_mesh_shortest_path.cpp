// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Surface_mesh_shortest_path/barycentric.h>

#include "cgalpy/sm/surface_mesh_shortest_path_types.hpp"

namespace py = nanobind;

namespace {

namespace Smsh = CGAL::Surface_mesh_shortest_paths_3;

} // namespace

void export_surface_mesh_shortest_path(py::module_& m) {
  py::enum_<Smsh::Barycentric_coordinates_type>(m, "Barycentric_coordinates_type")
    .value("BARYCENTRIC_COORDINATES_INVALID", Smsh::BARYCENTRIC_COORDINATES_INVALID)
    .value("BARYCENTRIC_COORDINATES_ON_VERTEX", Smsh::BARYCENTRIC_COORDINATES_ON_VERTEX)
    .value("BARYCENTRIC_COORDINATES_ON_BOUNDARY", Smsh::BARYCENTRIC_COORDINATES_ON_BOUNDARY)
    .value("BARYCENTRIC_COORDINATES_ON_BOUNDED_SIDE", Smsh::BARYCENTRIC_COORDINATES_ON_BOUNDED_SIDE)
    .value("BARYCENTRIC_COORDINATES_ON_UNBOUNDED_SIDE", Smsh::BARYCENTRIC_COORDINATES_ON_UNBOUNDED_SIDE);
}
