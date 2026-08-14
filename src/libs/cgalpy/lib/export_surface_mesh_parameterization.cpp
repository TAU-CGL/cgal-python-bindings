// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <fstream>
#include <stdexcept>
#include <string>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <CGAL/boost/graph/helpers.h>
#include <CGAL/Polygon_mesh_processing/measure.h>
#include <CGAL/Surface_mesh_parameterization/Barycentric_mapping_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/Discrete_authalic_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/Discrete_conformal_map_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/Error_code.h>
#include <CGAL/Surface_mesh_parameterization/IO/File_off.h>
#include <CGAL/Surface_mesh_parameterization/Mean_value_coordinates_parameterizer_3.h>
#include <CGAL/Surface_mesh_parameterization/parameterize.h>

#include "cgalpy/sm/surface_mesh_parameterization_types.hpp"
#include "cgalpy/Smp_docstrings.hpp"

namespace py = nanobind;

namespace {

namespace Smp = CGAL::Surface_mesh_parameterization;
namespace Pmp = CGAL::Polygon_mesh_processing;

using Surface_mesh_3 = cgalpy::sm::Surface_mesh_3;
using Point_2 = Kernel::Point_2;
using Vertex_descriptor = boost::graph_traits<Surface_mesh_3>::vertex_descriptor;
using Halfedge_descriptor = boost::graph_traits<Surface_mesh_3>::halfedge_descriptor;
using UV_pmap = Surface_mesh_3::Property_map<Vertex_descriptor, Point_2>;

enum class Parameterization_method {
  Mean_value,
  Barycentric,
  Discrete_authalic,
  Discrete_conformal
};

Smp::Error_code parameterize_impl(Surface_mesh_3& tmesh,
                                  Halfedge_descriptor bhd,
                                  UV_pmap uv_map,
                                  Parameterization_method method) {
  switch (method) {
   case Parameterization_method::Mean_value:
    return Smp::parameterize(
      tmesh,
      Smp::Mean_value_coordinates_parameterizer_3<Surface_mesh_3>(),
      bhd,
      uv_map);

   case Parameterization_method::Barycentric:
    return Smp::parameterize(
      tmesh,
      Smp::Barycentric_mapping_parameterizer_3<Surface_mesh_3>(),
      bhd,
      uv_map);

   case Parameterization_method::Discrete_authalic:
    return Smp::parameterize(
      tmesh,
      Smp::Discrete_authalic_parameterizer_3<Surface_mesh_3>(),
      bhd,
      uv_map);

   case Parameterization_method::Discrete_conformal:
    return Smp::parameterize(
      tmesh,
      Smp::Discrete_conformal_map_parameterizer_3<Surface_mesh_3>(),
      bhd,
      uv_map);
  }

  return Smp::ERROR_WRONG_PARAMETER;
}

Smp::Error_code parameterize_to_off(Surface_mesh_3& tmesh,
                                    const std::string& output_filename,
                                    Parameterization_method method = Parameterization_method::Mean_value) {
  if (!CGAL::is_triangle_mesh(tmesh)) return Smp::ERROR_NON_TRIANGULAR_MESH;

  Halfedge_descriptor bhd = Pmp::longest_border(tmesh).first;
  if (bhd == boost::graph_traits<Surface_mesh_3>::null_halfedge()) {
    return Smp::ERROR_NO_TOPOLOGICAL_DISC;
  }

  UV_pmap uv_map = tmesh.add_property_map<Vertex_descriptor, Point_2>("h:uv").first;

  Smp::Error_code err = parameterize_impl(tmesh, bhd, uv_map, method);
  if (err != Smp::OK) return err;

  std::ofstream out(output_filename);
  if (!out) throw std::runtime_error("Cannot open output file!");

  Smp::IO::output_uvmap_to_off(tmesh, bhd, uv_map, out);
  return err;
}

} // namespace

void export_surface_mesh_parameterization(py::module_& m) {
  py::enum_<Parameterization_method>(
    m,
    "Parameterization_method",
    "Selects the surface-mesh parameterization algorithm used by parameterize_to_off().")
    .value("MEAN_VALUE", Parameterization_method::Mean_value)
    .value("BARYCENTRIC", Parameterization_method::Barycentric)
    .value("DISCRETE_AUTHALIC", Parameterization_method::Discrete_authalic)
    .value("DISCRETE_CONFORMAL", Parameterization_method::Discrete_conformal);

  py::enum_<Smp::Error_code>(
    m,
    "Error_code",
    cgalpy::smp::docstrings::Surface_mesh_parameterization_Error_code)
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

  m.def("parameterize_to_off",
        &parameterize_to_off,
        py::arg("mesh"), py::arg("output_filename"),
        py::arg("method") = Parameterization_method::Mean_value,
        "Parameterizes a triangulated surface mesh and writes the UV map to an OFF file.");
}
