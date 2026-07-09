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
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

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

  py::class_<Path_on_surface>(m, "Path_on_surface")
    .def(py::init<const Surface_mesh_3&>(),
         py::arg("mesh"),
         py::keep_alive<1, 2>(),
         "Creates an empty path on the given surface mesh.")
    .def(py::init<const Path_on_surface&>(),
         py::arg("other"),
         "Copy constructor.")
    .def("is_empty",
         &Path_on_surface::is_empty,
         "Returns true iff the path is empty.")
    .def("length",
         &Path_on_surface::length,
         "Returns the number of darts in the path.")
    .def("is_closed",
         &Path_on_surface::is_closed,
         "Returns true iff the path is closed.")
    .def("clear",
         &Path_on_surface::clear,
         "Clears the path.")
    .def("push_back_by_index",
         [](Path_on_surface& path,
            std::size_t index,
            bool flip,
            bool update_isclosed) {
           path.push_back_by_index(index, flip, update_isclosed);
         },
         py::arg("index"),
         py::arg("flip") = false,
         py::arg("update_isclosed") = true,
         "Adds the dart with the given index at the end of the path.")
    .def("push_back_by_indices",
         [](Path_on_surface& path,
            const std::vector<std::size_t>& indices,
            bool update_isclosed) {
           for (std::size_t index : indices) {
             path.push_back_by_index(index, false, update_isclosed);
           }
         },
         py::arg("indices"),
         py::arg("update_isclosed") = true,
         "Adds the darts with the given indices at the end of the path.")
    .def("extend_straight_positive",
         [](Path_on_surface& path,
            std::size_t nb,
            bool update_isclosed) {
           path.extend_straight_positive(nb, update_isclosed);
         },
         py::arg("nb") = 1,
         py::arg("update_isclosed") = true,
         "Extends the path straight in the positive direction.")
    .def("extend_straight_negative",
         [](Path_on_surface& path,
            std::size_t nb,
            bool update_isclosed) {
           path.extend_straight_negative(nb, update_isclosed);
         },
         py::arg("nb") = 1,
         py::arg("update_isclosed") = true,
         "Extends the path straight in the negative direction.")
    .def("extend_positive_turn",
         [](Path_on_surface& path,
            std::size_t nb,
            bool update_isclosed) {
           path.extend_positive_turn(nb, update_isclosed);
         },
         py::arg("nb") = 1,
         py::arg("update_isclosed") = true,
         "Extends the path with a positive turn.")
    .def("extend_negative_turn",
         [](Path_on_surface& path,
            std::size_t nb,
            bool update_isclosed) {
           path.extend_negative_turn(nb, update_isclosed);
         },
         py::arg("nb") = 1,
         py::arg("update_isclosed") = true,
         "Extends the path with a negative turn.")
    .def("simplify_flips",
         &Path_on_surface::simplify_flips,
         py::arg("show_flips_left") = false,
         "Simplifies flips in the path when possible.")
    .def("nb_flips",
         &Path_on_surface::nb_flips,
         "Returns the number of flips in the path.")
    .def("cut",
         &Path_on_surface::cut,
         py::arg("n"),
         py::arg("update_isclosed") = true,
         "Cuts the path to keep only the first n darts.")
    .def("initialize_random_starting_dart",
         [](Path_on_surface& path,
            bool update_isclosed) {
           return path.initialize_random_starting_dart(update_isclosed);
         },
         py::arg("update_isclosed") = true,
         "Initializes this path to a random starting dart.");

  py::class_<Curves_on_surface_topology>(m, "Curves_on_surface_topology")
    .def(py::init<const Surface_mesh_3&, bool>(),
         py::arg("mesh"),
         py::arg("display_time") = false,
         py::keep_alive<1, 2>(),
         "Creates a topology helper for curves on a surface mesh.")
    .def("is_minimal_quadrangulation_computed",
         &Curves_on_surface_topology::is_minimal_quadrangulation_computed,
         "Returns true iff the minimal quadrangulation is computed.")
    .def("compute_minimal_quadrangulation",
         &Curves_on_surface_topology::compute_minimal_quadrangulation,
         py::arg("display_time") = true,
         "Computes the minimal quadrangulation if needed.")
    .def("is_contractible",
         [](const Curves_on_surface_topology& cst,
            const Path_on_surface& path,
            bool display_time) {
           return cst.is_contractible(path, display_time);
         },
         py::arg("path"),
         py::arg("display_time") = false,
         "Returns true iff the path is contractible.")
    .def("are_freely_homotopic",
         [](const Curves_on_surface_topology& cst,
            const Path_on_surface& path_1,
            const Path_on_surface& path_2,
            bool display_time) {
           return cst.are_freely_homotopic(path_1, path_2, display_time);
         },
         py::arg("path_1"),
         py::arg("path_2"),
         py::arg("display_time") = false,
         "Returns true iff the two paths are freely homotopic.")
    .def("are_base_point_homotopic",
         [](const Curves_on_surface_topology& cst,
            const Path_on_surface& path_1,
            const Path_on_surface& path_2,
            bool display_time) {
           return cst.are_base_point_homotopic(path_1, path_2, display_time);
         },
         py::arg("path_1"),
         py::arg("path_2"),
         py::arg("display_time") = false,
         "Returns true iff the two paths are base point homotopic.")
    .def("is_homotopic_to_simple_cycle",
         [](const Curves_on_surface_topology& cst,
            const Path_on_surface& path,
            bool display_time) {
           return cst.is_homotopic_to_simple_cycle(path, display_time);
         },
         py::arg("path"),
         py::arg("display_time") = false,
         "Returns true iff the path is homotopic to a simple cycle.")
    .def("set_verbose",
         &Curves_on_surface_topology::set_verbose,
         py::arg("is_verbose"),
         "Sets whether warning messages are displayed.");

  m.def("open_path_homotopy",
        &open_path_homotopy,
        py::arg("mesh"),
        "Runs the open path homotopy example on a surface mesh.");
}
