// Copyright (c) 2026 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

#include <vector>

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <CGAL/Surface_mesh_deformation.h>

#include "cgalpy/sm/surface_mesh_deformation_types.hpp"

namespace py = nanobind;

namespace {

using Surface_mesh_3 = cgalpy::sm::Surface_mesh_3;
using Point_3 = Kernel::Point_3;
using Vector_3 = Kernel::Vector_3;
using Vertex_descriptor = boost::graph_traits<Surface_mesh_3>::vertex_descriptor;
using Surface_mesh_deformation = CGAL::Surface_mesh_deformation<Surface_mesh_3>;

} // namespace

void export_surface_mesh_deformation(py::module_& m) {
  py::enum_<CGAL::Deformation_algorithm_tag>(m, "Deformation_algorithm_tag")
    .value("ORIGINAL_ARAP", CGAL::ORIGINAL_ARAP)
    .value("SPOKES_AND_RIMS", CGAL::SPOKES_AND_RIMS)
    .value("SRE_ARAP", CGAL::SRE_ARAP);

  py::class_<Surface_mesh_deformation>(m, "Surface_mesh_deformation")
    .def(py::init<Surface_mesh_3&>(), py::arg("mesh"), py::keep_alive<1, 2>())
    .def("insert_roi_vertex",
         &Surface_mesh_deformation::insert_roi_vertex,
         py::arg("vd"),
         "Inserts one vertex into the region of interest.")
    .def("insert_roi_vertices",
         [](Surface_mesh_deformation& self, const Surface_mesh_3& mesh) {
           for (Vertex_descriptor vd : vertices(mesh)) self.insert_roi_vertex(vd);
         },
         py::arg("mesh"),
         "Inserts all vertices of the given surface mesh into the region of interest.")
    .def("insert_roi_vertices",
         [](Surface_mesh_deformation& self, const std::vector<Vertex_descriptor>& vertices) {
           self.insert_roi_vertices(vertices.begin(), vertices.end());
         },
         py::arg("vertices"),
         "Inserts a sequence of vertices into the region of interest.")
    .def("insert_control_vertex",
         &Surface_mesh_deformation::insert_control_vertex,
         py::arg("vd"),
         "Inserts one control vertex.")
    .def("insert_control_vertices",
         [](Surface_mesh_deformation& self, const std::vector<Vertex_descriptor>& vertices) {
           self.insert_control_vertices(vertices.begin(), vertices.end());
         },
         py::arg("vertices"),
         "Inserts a sequence of control vertices.")
    .def("preprocess",
         &Surface_mesh_deformation::preprocess,
         "Preprocesses after ROI or control vertex changes.")
    .def("set_target_position",
         &Surface_mesh_deformation::set_target_position,
         py::arg("vd"), py::arg("target_position"),
         "Sets the target position of a control vertex.")
    .def("deform",
         [](Surface_mesh_deformation& self) { self.deform(); },
         "Deforms using the current iteration and tolerance settings.")
    .def("deform",
         [](Surface_mesh_deformation& self, unsigned int iterations, double tolerance) {
           self.deform(iterations, tolerance);
         },
         py::arg("iterations"), py::arg("tolerance"),
         "Deforms using one-time iteration and tolerance parameters.")
    .def("set_iterations",
         &Surface_mesh_deformation::set_iterations,
         py::arg("iterations"),
         "Sets the default number of deformation iterations.")
    .def("set_tolerance",
         &Surface_mesh_deformation::set_tolerance,
         py::arg("tolerance"),
         "Sets the default deformation tolerance.")
    .def("translate",
         [](Surface_mesh_deformation& self, Vertex_descriptor vd, const Vector_3& vector) {
           self.translate(vd, vector);
         },
         py::arg("vd"), py::arg("vector"),
         "Translates one control vertex target position by a vector.")
    .def("translate",
         [](Surface_mesh_deformation& self, const std::vector<Vertex_descriptor>& vertices, const Vector_3& vector) {
           self.translate(vertices.begin(), vertices.end(), vector);
         },
         py::arg("vertices"), py::arg("vector"),
         "Translates a sequence of control vertex target positions by a vector.")
    .def("reset",
         &Surface_mesh_deformation::reset,
         "Restores ROI vertices to their initial positions.")
    .def("overwrite_initial_geometry",
         &Surface_mesh_deformation::overwrite_initial_geometry,
         "Sets the current ROI vertex positions as the new initial geometry.");
}
