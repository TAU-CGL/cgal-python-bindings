// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Polygonal_schema_min_items.h>

#include "cgalpy/sm/surface_mesh_topology_types.hpp"

namespace py = nanobind;

namespace {

namespace Smt = CGAL::Surface_mesh_topology;

} // namespace

void export_surface_mesh_topology(py::module_& m) {
  py::class_<Smt::Polygonal_schema_min_items>(m, "Polygonal_schema_min_items")
    .def(py::init<>());
}
