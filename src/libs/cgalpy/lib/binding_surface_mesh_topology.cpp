// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include <CGAL/Curves_on_surface_topology.h>
#include <CGAL/Path_on_surface.h>
#include <CGAL/Polygonal_schema_min_items.h>

#include "cgalpy/sm/surface_mesh_topology_types.hpp"

namespace py = nanobind;

namespace {

namespace Smt = CGAL::Surface_mesh_topology;

using Surface_mesh_3 = cgalpy::sm::Surface_mesh_3;
using Path_on_surface = Smt::Path_on_surface<Surface_mesh_3>;
using Curves_on_surface_topology = Smt::Curves_on_surface_topology<Surface_mesh_3>;

void create_open_path_1(Path_on_surface& path) {
  path.push_back_by_index(88);
  for (int i = 0; i < 3; ++i) path.extend_positive_turn(2);
}

void create_open_path_2(Path_on_surface& path) {
  path.push_back_by_index(300);
  for (int i = 0; i < 3; ++i) path.extend_negative_turn(2);
}

void create_open_path_3(Path_on_surface& path) {
  path.push_back_by_index(87);
  path.extend_positive_turn(1);
  for (int i = 0; i < 3; ++i) path.extend_positive_turn(2);
  path.extend_positive_turn(1);
}

std::pair<bool, bool> open_path_homotopy(const Surface_mesh_3& sm) {
  Curves_on_surface_topology cst(sm);

  Path_on_surface p1(sm);
  Path_on_surface p2(sm);
  Path_on_surface p3(sm);

  create_open_path_1(p1);
  create_open_path_2(p2);
  create_open_path_3(p3);

  bool p1_p2 = cst.are_base_point_homotopic(p1, p2);
  bool p2_p3 = cst.are_base_point_homotopic(p2, p3);

  return {p1_p2, p2_p3};
}

} // namespace

void export_surface_mesh_topology(py::module_& m) {
  py::class_<Smt::Polygonal_schema_min_items>(m, "Polygonal_schema_min_items")
    .def(py::init<>());

  m.def("open_path_homotopy",
        &open_path_homotopy,
        py::arg("mesh"),
        "Runs the open path homotopy example on a surface mesh.");
}
