// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <array>
#include <cstddef>
#include <tuple>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>

#include <CGAL/Surface_mesh_approximation/approximate_triangle_mesh.h>

#include "cgalpy/sm/surface_mesh_approximation_types.hpp"
#include "cgalpy/Sma_docstrings.hpp"

namespace py = nanobind;

namespace {

namespace Sma = CGAL::Surface_mesh_approximation;

using Surface_mesh_3 = cgalpy::sm::Surface_mesh_3;
using Point_3 = Kernel::Point_3;
using Indexed_triangle = std::array<std::size_t, 3>;
using Approximation_result =
  std::tuple<bool, std::vector<Point_3>, std::vector<Indexed_triangle>>;

//! Approximate a triangle mesh and return the manifold flag, anchor points, and indexed triangles.
Approximation_result approximate_triangle_mesh(const Surface_mesh_3& tmesh,
                                               Sma::Verbose_level verbose_level = Sma::SILENT,
                                               std::size_t max_number_of_proxies = 200,
                                               Sma::Seeding_method seeding_method = Sma::HIERARCHICAL,
                                               double min_error_drop = 0.0,
                                               std::size_t number_of_relaxations = 5,
                                               std::size_t number_of_iterations = 20,
                                               double subdivision_ratio = 5.0,
                                               bool relative_to_chord = false,
                                               bool with_dihedral_angle = false,
                                               bool optimize_anchor_location = true,
                                               bool pca_plane = false) {
  std::vector<Point_3> anchors;
  std::vector<Indexed_triangle> triangles;

  bool is_manifold = Sma::approximate_triangle_mesh
    (tmesh,
     CGAL::parameters::verbose_level(verbose_level)
                     .max_number_of_proxies(max_number_of_proxies)
                     .seeding_method(seeding_method)
                     .min_error_drop(min_error_drop)
                     .number_of_relaxations(number_of_relaxations)
                     .number_of_iterations(number_of_iterations)
                     .subdivision_ratio(subdivision_ratio)
                     .relative_to_chord(relative_to_chord)
                     .with_dihedral_angle(with_dihedral_angle)
                     .optimize_anchor_location(optimize_anchor_location)
                     .pca_plane(pca_plane)
                     .anchors(std::back_inserter(anchors))
                     .triangles(std::back_inserter(triangles)));

  return std::make_tuple(is_manifold, anchors, triangles);
}

} // namespace

void export_surface_mesh_approximation(py::module_& m) {
  py::enum_<Sma::Verbose_level>(m, "Verbose_level", cgalpy::sma::docstrings::Surface_mesh_approximation_Verbose_level)
    .value("SILENT", Sma::SILENT)
    .value("MAIN_STEPS", Sma::MAIN_STEPS)
    .value("VERBOSE", Sma::VERBOSE);

  py::enum_<Sma::Seeding_method>(m, "Seeding_method", cgalpy::sma::docstrings::Surface_mesh_approximation_Seeding_method)
    .value("RANDOM", Sma::RANDOM)
    .value("INCREMENTAL", Sma::INCREMENTAL)
    .value("HIERARCHICAL", Sma::HIERARCHICAL);

  m.def("approximate_triangle_mesh",
        &approximate_triangle_mesh,
        py::arg("tmesh"),
        py::arg("verbose_level") = Sma::SILENT,
        py::arg("max_number_of_proxies") = std::size_t(200),
        py::arg("seeding_method") = Sma::HIERARCHICAL,
        py::arg("min_error_drop") = 0.0,
        py::arg("number_of_relaxations") = std::size_t(5),
        py::arg("number_of_iterations") = std::size_t(20),
        py::arg("subdivision_ratio") = 5.0,
        py::arg("relative_to_chord") = false,
        py::arg("with_dihedral_angle") = false,
        py::arg("optimize_anchor_location") = true,
        py::arg("pca_plane") = false,
        "Approximates a triangle mesh and returns (is_manifold, anchors, triangles).");
}
