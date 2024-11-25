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

#include "CGALPY/parse_named_parameters.hpp"
#include "CGALPY/kernel_type.hpp"

namespace py = nanobind;
namespace SD = CGAL::Shape_detection;
namespace PS = SD::Point_set;

void export_region_growing(py::module_& m) {
  using Kernel_ = Kernel;
  using Point_3 = Kernel_::Point_3;
  using Input_range = CGAL::Point_set_3<Point_3>;
  using Point_set = Input_range;
  using Point_map   = typename Input_range::Point_map;
  using Normal_map  = typename Input_range::Vector_map;
  // Point Set
  py::class_<PS::K_neighbor_query<Kernel_, Point_set::Index,
                                  Point_set::Point_map>>(m, "K_neighbor_query_3",
    "K nearest neighbors search in a set of 3D points.\n"
    "This class returns the K nearest neighbors of a query point in a point set.")
    .def(py::init<const Point_set&>(),
         py::arg("input_range"),
         "initializes a Kd-tree with input points.\n\n"
         "Parameters\n"
         "• input_range: range of points to be indexed\n\n"
         "Precondition\n"
         "• input_range.size() > 0\n")

    .def_static("init_with_params", [](const Point_set& ps,
                                       const py::dict& params) {
        return PS::K_neighbor_query<Kernel_, Point_set::Index, Point_set::Point_map>(ps, internal::parse_named_parameters(params));
    },
      py::arg("ps"), py::arg("params"),
         "initializes a Kd-tree with input points.\n\n"
         "Parameters\n"
         "• input_range: range of points to be indexed\n\n"
         "Precondition\n"
         "• input_range.size() > 0\n"
         "• K > 0\n")
    .def("__call__", &PS::K_neighbor_query<Kernel_, Point_set::Index,
         Point_set::Point_map>::operator(),
         "implements NeighborQuery::operator()().\n"
         "This operator finds the K closest points to the point around the query point query using a Kd-tree. These references to the points are returned in neighbors.\n\n"
         "Parameters\n"
         "• query: Item of the query point\n"
         "• neighbors: Items of points, which are neighbors of the query point")
    ;

  using Lsqcf_ps3 = PS::Least_squares_circle_fit_region<Kernel_,
                                                        Point_set::Index, Point_set::Point_map, Point_set::Vector_map>;
  py::class_<Lsqcf_ps3>(m, "Least_squares_circle_fit_region_3",
    "Region type based on the quality of the least squares circle fit applied to 2D points.\n"
    "This class fits a circle to chunks of points in a 2D point set and controls the quality of this fit. If all quality conditions are satisfied, the chunk is accepted as a valid region, otherwise rejected.")
    .def_static("init_with_params", [](const py::dict& params) {
      return Lsqcf_ps3(internal::parse_named_parameters(params));
    })
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
         "implements RegionType::is_valid_region().\n"
         "This function controls if the estimated radius is between minimum_radius and maximum_radius and if the region contains at least min_region_size points.\n"
         "\n"
         "Parameters\n"
         "• region:	indices of points included in the region\n"
         "\n"
         "Returns\n"
         "• Boolean: true or false \n")
    .def("primitive", &Lsqcf_ps3::primitive,
         "implements RegionType::primitive().\n"
         "This function provides the last primitive that has been fitted with the region.\n"
         "\n"
         "Returns\n"
         "• Primitive: parameters that fits the region\n"
         "\n"
         "Precondition\n"
         "• successful fitted primitive via successful call of update(region) with a sufficient large region \n")
    .def("region_index_map", &Lsqcf_ps3::region_index_map,
         "implements RegionType::region_index_map().\n"
         "This function creates an empty property map that maps iterators on the input range Item to std::size_t\n")
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
