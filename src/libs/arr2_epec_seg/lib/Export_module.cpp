// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "config.hpp"
#include "common.hpp"

#define BOOST_PYTHON_STATIC_LIB 1

void export_Kernel();
void export_Arrangement_2();
void export_Intersections_2();
void export_Point_location();
void export_Object();
void export_Vertex();
void export_Halfedge();
void export_Face();

void export_Arr_linear_traits();
void export_Arr_segment_traits();
void export_Arr_circle_segment_traits();
void export_Arr_algebraic_segment_traits();

void export_Polygon_2();
void export_Polygon_with_holes_2();
void export_Polygon_set_2();
void export_General_polygon_2();
void export_General_polygon_with_holes_2();
void export_General_polygon_set_2();
void export_Polygon_with_holes_2();
void export_Minkowski_sum_2();
void export_Boolean_set_operations_2();

void export_Triangulations();

BOOST_PYTHON_MODULE(CGALPY_MODULE_NAME)
{
  using namespace boost::python;

  export_Kernel();
  export_Arrangement_2();
  export_Object();
  export_Vertex();
  export_Halfedge();
  export_Face();
  export_Intersections_2();
  export_Point_location();

#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_LINEAR_TRAITS
  export_Arr_linear_traits();
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_SEGMENT_TRAITS
  export_Arr_segment_traits();
  export_Triangulations();
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_CIRCLE_SEGMENT_TRAITS
  export_Arr_circle_segment_traits();
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_ALGEBRAIC_SEGMENT_TRAITS
  export_Arr_algebraic_segment_traits();
#endif

#ifdef MINKOWSKI_SUM
  export_Polygon_2();
  export_Polygon_with_holes_2();
  export_Polygon_set_2();
  export_General_polygon_2();
  export_General_polygon_with_holes_2();
  export_General_polygon_set_2();
  export_Minkowski_sum_2();
  export_Boolean_set_operations_2();
#endif
}
