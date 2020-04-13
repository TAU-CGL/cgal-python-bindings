// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

void export_kernel();
void export_arrangement_2();
void export_intersections_2();
void export_point_location();
void export_object();
void export_vertex();
void export_halfedge();
void export_face();

void export_arr_linear_traits();
void export_arr_segment_traits();
void export_arr_circle_segment_traits();
void export_arr_algebraic_segment_traits();

void export_polygon_2();
void export_polygon_with_holes_2();
void export_polygon_partition_2();
void export_polygon_set_2();
void export_general_polygon_2();
void export_general_polygon_with_holes_2();
void export_general_polygon_set_2();
void export_polygon_with_holes_2();
void export_minkowski_sum_2();
void export_boolean_set_operations_2();

void export_triangulations();
void export_convex_hull_2_bindings();

void export_spatial_searching();
void export_bounding_volumes();

BOOST_PYTHON_MODULE(CGALPY_MODULE_NAME)
{
  using namespace boost::python;

  export_kernel();
  export_arrangement_2();
  export_object();
  export_vertex();
  export_halfedge();
  export_face();
  export_intersections_2();

  export_polygon_2();
  export_polygon_with_holes_2();
  export_polygon_set_2();

#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_LINEAR_TRAITS
  export_arr_linear_traits();
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_SEGMENT_TRAITS
  export_arr_segment_traits();
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_CIRCLE_SEGMENT_TRAITS
  export_arr_circle_segment_traits();
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_ALGEBRAIC_SEGMENT_TRAITS
  export_arr_algebraic_segment_traits();
#endif

#ifdef CGALPY_CONVEX_HULL_BINDINGS
  export_convex_hull_2_bindings();
#endif
#ifdef CGALPY_TRIANGULATION_2_BINDINGS
  export_triangulations();
#endif
#ifdef CGALPY_SPATIAL_SEARCHING_BINDINGS
  export_spatial_searching();
#endif
#ifdef CGALPY_BOUNDING_VOLUMES_BINDINGS
  export_bounding_volumes();
#endif
#ifdef CGALPY_BOOLEAN_SET_OPERATIONS_BINDINGS
  export_boolean_set_operations_2();
#endif
#ifdef CGALPY_POLYGON_PARTITIONING_BINDINGS
  export_polygon_partition_2();
#endif
#ifdef CGALPY_POINT_LOCATION_BINDINGS
  export_point_location();
#endif
#ifdef CGALPY_MINKOWSKI_SUM_2_BINDINGS
  export_minkowski_sum_2();
  export_general_polygon_2();
  export_general_polygon_with_holes_2();
  export_general_polygon_set_2();
#endif
}
