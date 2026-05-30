// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>

#include <CGAL/Point_set_3.h>
#include <CGAL/Shape_detection/Region_growing.h>

#include "CGALPY/kernel_type.hpp"
#include "cgalpy/Sd_docstrings.hpp"

namespace py = nanobind;
namespace SD = CGAL::Shape_detection;
namespace PS = SD::Point_set;
namespace sd_doc = cgalpy::sd::docstrings;

void export_region_growing(py::module_& m) {
  using Kernel_ = Kernel;
  using Point_3 = Kernel_::Point_3;
  using Input_range = CGAL::Point_set_3<Point_3>;
  using Point_set = Input_range;
  using Point_map   = typename Input_range::Point_map;
  using Normal_map  = typename Input_range::Vector_map;
  // Point Set
  py::class_<PS::K_neighbor_query<Kernel_, Point_set::Index,
                                  Point_set::Point_map>>(
      m, "K_neighbor_query_3",
      sd_doc::Shape_detection_Point_set_K_neighbor_query_class)
    .def(py::init<const Point_set&>(),
         py::arg("input_range"),
         sd_doc::Shape_detection_Point_set_K_neighbor_query_K_neighbor_query)

    .def_static("init_with_params", [](const Point_set& ps, const py::dict& np) {
      return PS::K_neighbor_query<Kernel_, Point_set::Index, Point_set::Point_map>(ps);
    },
      py::arg("ps"), py::arg("np"),
      sd_doc::Shape_detection_Point_set_K_neighbor_query_K_neighbor_query)
    .def("__call__", &PS::K_neighbor_query<Kernel_, Point_set::Index,
         Point_set::Point_map>::operator(),
         py::arg("query"), py::arg("neighbors"),
         sd_doc::Shape_detection_Point_set_K_neighbor_query_operator_call)
    ;

  using Lsqcf_ps3 =
    PS::Least_squares_circle_fit_region<Kernel_, Point_set::Index, Point_set::Point_map, Point_set::Vector_map>;
  py::class_<Lsqcf_ps3>(
      m, "Least_squares_circle_fit_region_3",
      sd_doc::Shape_detection_Point_set_Least_squares_circle_fit_region_class)
    .def_static("init_with_params", [](const py::dict& np) {
      return Lsqcf_ps3();
    },
      py::arg("np"),
      sd_doc::Shape_detection_Point_set_Least_squares_circle_fit_region_Least_squares_circle_fit_region)
    // .def("is_part_of_region", &Lsqcf_ps3::is_part_of_region, // only for 2D
    //      "implements RegionType::is_part_of_region().\n"
    //      "This function controls if a point with the item query is within the maximum_distance from the corresponding circle and if the angle between its normal and the circle radius is within the maximum_angle. If both conditions are satisfied, it returns true, otherwise false.\n"
    //      "\n"
    //      "Parameters\n"
    //      "• query:	item of the query point\n"
    //      "• region:	inlier items of the region\n"
    //      "\n"
    //      "Returns\n"
    //      "• Boolean: true or false \n")
    .def("is_valid_region", &Lsqcf_ps3::is_valid_region,
         py::arg("region"),
         sd_doc::Shape_detection_Point_set_Least_squares_circle_fit_region_is_valid_region)
    .def("primitive", &Lsqcf_ps3::primitive,
         sd_doc::Shape_detection_Point_set_Least_squares_circle_fit_region_primitive)
    .def("region_index_map", &Lsqcf_ps3::region_index_map,
         sd_doc::Shape_detection_Point_set_Least_squares_circle_fit_region_region_index_map)
    // .def("update", &Lsqcf_ps3::update, // also only for 2D
    //      "implements RegionType::update().\n"
    //      "This function fits the least squares circle to all points from the region.\n"
    //      "\n"
    //      "Parameters\n"
    //      "• region:	indices of points included in the region\n"
    //      "\n"
    //      "Returns\n"
    //      "• Boolean: true if the circle fitting succeeded and false otherwise\n"
    //      "\n"
    //      "Precondition\n"
    //      "• region.size() > 0 \n")
    ;


  ;

}
