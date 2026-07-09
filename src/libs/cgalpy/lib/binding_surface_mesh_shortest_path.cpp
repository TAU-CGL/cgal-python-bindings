// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <array>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <CGAL/number_utils.h>
#include <CGAL/Random.h>
#include <CGAL/Surface_mesh_shortest_path.h>
#include <CGAL/Surface_mesh_shortest_path/barycentric.h>

#include "cgalpy/sm/surface_mesh_shortest_path_types.hpp"

namespace py = nanobind;

namespace {

namespace Smsh = CGAL::Surface_mesh_shortest_paths_3;

using Surface_mesh_3 = cgalpy::sm::Surface_mesh_3;
using Traits = CGAL::Surface_mesh_shortest_path_traits<Kernel, Surface_mesh_3>;
using Surface_mesh_shortest_path = CGAL::Surface_mesh_shortest_path<Traits>;
using Point_3 = Traits::Point_3;
using FT = Traits::FT;
using Barycentric_coordinates = Traits::Barycentric_coordinates;
using Python_barycentric_coordinates = std::array<double, 3>;
using Vertex_descriptor = boost::graph_traits<Surface_mesh_3>::vertex_descriptor;
using Face_descriptor = boost::graph_traits<Surface_mesh_3>::face_descriptor;

Barycentric_coordinates make_barycentric_coordinates(const Python_barycentric_coordinates& bc) {
  return Barycentric_coordinates{{FT(bc[0]), FT(bc[1]), FT(bc[2])}};
}

std::size_t random_face_index(const Surface_mesh_3& tmesh, unsigned int seed = 7915421) {
  CGAL::Random rand(seed);
  return static_cast<std::size_t>(rand.get_int(0, static_cast<int>(num_faces(tmesh))));
}

void add_source_point(Surface_mesh_shortest_path& self,
                      Face_descriptor face,
                      const Python_barycentric_coordinates& location) {
  self.add_source_point(face, make_barycentric_coordinates(location));
}

void add_source_vertex(Surface_mesh_shortest_path& self,
                       Vertex_descriptor vertex) {
  self.add_source_point(vertex);
}

Point_3 point(Surface_mesh_shortest_path& self,
              Face_descriptor face,
              const Python_barycentric_coordinates& location) {
  return self.point(face, make_barycentric_coordinates(location));
}

double shortest_distance_to_source_points(Surface_mesh_shortest_path& self,
                                          Vertex_descriptor vertex) {
  return CGAL::to_double(self.shortest_distance_to_source_points(vertex).first);
}

double shortest_distance_to_source_points(Surface_mesh_shortest_path& self,
                                          Face_descriptor face,
                                          const Python_barycentric_coordinates& location) {
  return CGAL::to_double
    (self.shortest_distance_to_source_points(face, make_barycentric_coordinates(location)).first);
}

std::vector<Point_3> shortest_path_points_to_source_points(Surface_mesh_shortest_path& self,
                                                           Vertex_descriptor vertex) {
  std::vector<Point_3> points;
  self.shortest_path_points_to_source_points(vertex, std::back_inserter(points));
  return points;
}

std::vector<Point_3> shortest_path_points_to_source_points(Surface_mesh_shortest_path& self,
                                                           Face_descriptor face,
                                                           const Python_barycentric_coordinates& location) {
  std::vector<Point_3> points;
  self.shortest_path_points_to_source_points(face, make_barycentric_coordinates(location),
                                             std::back_inserter(points));
  return points;
}

void write_shortest_paths_to_source_points(Surface_mesh_shortest_path& self,
                                           const Surface_mesh_3& tmesh,
                                           const std::string& filename) {
  std::ofstream output(filename);
  if (!output) throw std::runtime_error("Cannot open output file!");

  for (Vertex_descriptor vd : vertices(tmesh)) {
    std::vector<Point_3> points;
    self.shortest_path_points_to_source_points(vd, std::back_inserter(points));

    output << points.size() << " ";
    for (const auto& point : points) output << " " << point;
    output << std::endl;
  }
}

} // namespace

void export_surface_mesh_shortest_path(py::module_& m) {
  py::enum_<Smsh::Barycentric_coordinates_type>(m, "Barycentric_coordinates_type")
    .value("BARYCENTRIC_COORDINATES_INVALID", Smsh::BARYCENTRIC_COORDINATES_INVALID)
    .value("BARYCENTRIC_COORDINATES_ON_VERTEX", Smsh::BARYCENTRIC_COORDINATES_ON_VERTEX)
    .value("BARYCENTRIC_COORDINATES_ON_BOUNDARY", Smsh::BARYCENTRIC_COORDINATES_ON_BOUNDARY)
    .value("BARYCENTRIC_COORDINATES_ON_BOUNDED_SIDE", Smsh::BARYCENTRIC_COORDINATES_ON_BOUNDED_SIDE)
    .value("BARYCENTRIC_COORDINATES_ON_UNBOUNDED_SIDE", Smsh::BARYCENTRIC_COORDINATES_ON_UNBOUNDED_SIDE);

  py::class_<Surface_mesh_shortest_path>(m, "Surface_mesh_shortest_path")
    .def(py::init<const Surface_mesh_3&>(), py::arg("mesh"), py::keep_alive<1, 2>())
    .def("add_source_point",
         &add_source_point,
         py::arg("face"), py::arg("location"),
         "Adds a source point inside a face using barycentric coordinates.")
    .def("add_source_point",
         &add_source_vertex,
         py::arg("vertex"),
         "Adds a mesh vertex as a source point.")
    .def("build_sequence_tree",
         &Surface_mesh_shortest_path::build_sequence_tree,
         "Builds the internal sequence tree for the current source points.")
    .def("clear",
         &Surface_mesh_shortest_path::clear,
         "Removes all source points and clears the internal sequence tree.")
    .def("number_of_source_points",
         &Surface_mesh_shortest_path::number_of_source_points,
         "Returns the number of source points.")
    .def("point",
         &point,
         py::arg("face"), py::arg("location"),
         "Returns the 3D point at barycentric coordinates in a face.")
    .def("shortest_distance_to_source_points",
         py::overload_cast<Surface_mesh_shortest_path&, Vertex_descriptor>
           (&shortest_distance_to_source_points),
         py::arg("vertex"),
         "Returns the shortest distance from a vertex to the closest source point.")
    .def("shortest_distance_to_source_points",
         py::overload_cast<Surface_mesh_shortest_path&, Face_descriptor, const Python_barycentric_coordinates&>
           (&shortest_distance_to_source_points),
         py::arg("face"), py::arg("location"),
         "Returns the shortest distance from a face location to the closest source point.")
    .def("shortest_path_points_to_source_points",
         py::overload_cast<Surface_mesh_shortest_path&, Vertex_descriptor>
           (&shortest_path_points_to_source_points),
         py::arg("vertex"),
         "Returns the points of the shortest path from a vertex to the source points.")
    .def("shortest_path_points_to_source_points",
         py::overload_cast<Surface_mesh_shortest_path&, Face_descriptor, const Python_barycentric_coordinates&>
           (&shortest_path_points_to_source_points),
         py::arg("face"), py::arg("location"),
         "Returns the points of the shortest path from a face location to the source points.")
    .def("write_shortest_paths_to_source_points",
         &write_shortest_paths_to_source_points,
         py::arg("mesh"), py::arg("filename"),
         "Writes shortest-path polylines from all mesh vertices to the source points.");

  m.def("random_face_index",
        &random_face_index,
        py::arg("mesh"), py::arg("seed") = 7915421,
        "Returns a random face index using CGAL::Random.");
}
