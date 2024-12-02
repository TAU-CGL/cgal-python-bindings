// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/tuple.h>

#include <CGAL/Kinetic_surface_reconstruction_3.h>
#include <CGAL/KSR_3/Graphcut.h>

#include "CGALPY/parse_named_parameters.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

void export_kinetic_surface_reconstruction(py::module_& m) {
  using Kernel_ = Kernel;
  using Intersection_kernel = CGAL::Exact_predicates_exact_constructions_kernel; // default KSR kernel
  using FT = typename Kernel_::FT;
  using Point_3 = typename Kernel_::Point_3;
  using Plane_3 = typename Kernel_::Plane_3;
  using Point_range = CGAL::Point_set_3<Point_3>;
  using Point_map = typename Point_range::Point_map;
  using Normal_map = typename Point_range::Vector_map;
  using KSP = CGAL::Kinetic_space_partition_3<Kernel, Intersection_kernel>;
  using KSR =
    CGAL::Kinetic_surface_reconstruction_3<Kernel, Point_range, Point_map,
                                           Normal_map>;
  using Graphcut = CGAL::KSR_3::Graphcut<KSP>;

  py::class_<KSR> ksr(m, "Kinetic_surface_reconstruction_3",
    "Pipeline for piecewise planar surface reconstruction from a point cloud via inside/outside labeling of a kinetic partition using min-cut.\n");
  ksr.def("__init__",
          [](KSR& ksr, Point_range& points, const py::dict& np = py::dict()) {
            // return a new instance of KSR
            if (np.contains("minimum_region_size")) {
              std::size_t minimum_region_size;
              try {
                minimum_region_size = py::cast<std::size_t>(np["minimum_region_size"]);
              }
              catch (const py::cast_error& e) {
                throw std::runtime_error("Error converting 'minimum_region_size' parameter.");
              }
              new (&ksr) KSR(points, internal::parse_named_parameters(np)
                             .minimum_region_size(minimum_region_size));
            }
            else {
              new (&ksr) KSR(points, internal::parse_named_parameters(np));
            }
          },
          py::arg("points"), py::arg("np") = py::dict(),
          "creates a Kinetic_shape_reconstruction_3 object.\n")
    .def("detect_planar_shapes",
         [](KSR& ksr, const py::dict& np = py::dict()) {
           return ksr.detect_planar_shapes(internal::parse_named_parameters(np));
         },
         py::arg("np") = py::dict(),
         "detects shapes in the provided point cloud and regularizes them.\n")
    .def("detected_planar_shapes",
         [](KSR& ksr) { return ksr.detected_planar_shapes(); },
      "returns the support planes of the detected and regularized shapes.\n")
    .def("detected_planar_shape_indices",
         [](KSR& ksr) { return ksr.detected_planar_shape_indices(); },
      "returns the indices of detected and regularized shapes.\n")
    .def("detection_and_partition",
         [](KSR& ksr, std::size_t k, const py::dict& np = py::dict()) {
           return ksr.detection_and_partition(k, internal::parse_named_parameters(np));
         },
         py::arg("k"), py::arg("np") = py::dict(),
         "detects and regularizes shapes in the provided point cloud and creates the kinetic space partition.\n"
         "Combines calls of detect_planar_shapes(), initialize_partition() and partition().\n")
    .def("initialize_partition",
         [](KSR& ksr, const py::dict& np = py::dict()) {
           return ksr.initialize_partition(internal::parse_named_parameters(np));
         },
      py::arg("np") = py::dict(),
      "initializes the kinetic partition.\n")
    // .def("kinetic_partition", [](const KSR& ksr) { // deprecated?
    //   return ksr.kinetic_partition();
    // },
    // "gives access to the kinetic partition.\n")
    .def("partition", [](KSR& ksr, std::size_t k) { return ksr.partition(k); },
      py::arg("k"),
      "propagates the kinetic polygons in the initialized partition.\n")
    .def("reconstruct",
         [](KSR& ksr, FT lambda,
            std::map<KSP::Face_support, bool> external_nodes) {
           std::vector<std::vector<std::size_t>> polys;
           std::vector<Point_3> vertices;
           ksr.reconstruct(lambda, external_nodes, std::back_inserter(vertices),
                           std::back_inserter(polys));
           return std::make_tuple(vertices, polys);
         },
         py::arg("lambda"), py::arg("external_nodes"),
         "uses min-cut to solve an inside/outside labeling of the volumes of the kinetic partition and provides the reconstructed surface as a list of indexed polygons.\n"
         "The external_nodes parameter allows to indicate the preferred labels for faces on the bounding box.\n")
    .def("reconstruct_with_ground",
         [](KSR& ksr, FT lambda) {
           std::vector<std::vector<std::size_t>> polys;
           std::vector<Point_3> vertices;
           ksr.reconstruct_with_ground(lambda, std::back_inserter(vertices),
                                       std::back_inserter(polys));
           return std::make_tuple(vertices, polys);
         },
      py::arg("lambda"),
      "uses min-cut to solve an inside/outside labeling of the volumes of the kinetic partition and provides the reconstructed surface as a list of indexed polygons.\n"
      "Estimates a horizontal ground plane within the detected shapes. Cells in the partition below the ground plane receive a weight to be labeled as inside. The z axis is considered as vertical upwards pointing.\n")
    ;

  py::enum_<KSR::KSP::Face_support>(ksr, "Face_support",
    "identifies the support of a face in the exported linear cell complex, which is either an input polygon, identified by a non-negative number, a side of the bounding box in the rotated coordinate system or a face of the octree used to partition the scene.")
    .value("ZMIN", KSR::KSP::ZMIN)
    .value("YMIN", KSR::KSP::YMIN)
    .value("XMAX", KSR::KSP::XMAX)
    .value("YMAX", KSR::KSP::YMAX)
    .value("XMIN", KSR::KSP::XMIN)
    .value("ZMAX", KSR::KSP::ZMAX)
    .value("OCTREE_FACE", KSR::KSP::OCTREE_FACE)
    .export_values()
    ;
}
