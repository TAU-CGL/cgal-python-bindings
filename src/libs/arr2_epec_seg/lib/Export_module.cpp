#include <Config.hpp>
#include <Common.hpp>



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
void export_Minkowski_sum_2();
void export_Boolean_set_operations_2();

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

#if CGALPY_TRAITS == CGALPY_ARR_LINEAR_TRAITS
  export_Arr_linear_traits();
#endif
#if CGALPY_TRAITS == CGALPY_ARR_SEGMENT_TRAITS
  export_Arr_segment_traits();
#endif
#if CGALPY_TRAITS == CGALPY_ARR_CIRCLE_SEGMENT_TRAITS
  export_Arr_circle_segment_traits();
#endif
#if CGALPY_TRAITS == CGALPY_ARR_ALGEBRAIC_SEGMENT_TRAITS
  export_Arr_algebraic_segment_traits();
#endif

#ifdef MINKOWSKI_SUM
  export_Polygon_2();
  export_Polygon_with_holes_2();
  export_Minkowski_sum_2();
  export_Boolean_set_operations_2();
#endif
}