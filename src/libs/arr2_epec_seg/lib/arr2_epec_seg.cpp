#include <Config.hpp>
#include <Common.hpp>

void export_Kernel();
void export_Arrangement_2();
void export_Intersections_2();
void export_Point_location();
void export_Vertex();
void export_Halfedge();
void export_Face();

void export_Arr_linear_traits();
void export_Arr_segment_traits();
void export_Arr_circle_segment_traits();
void export_Arr_algebraic_segment_traits();

BOOST_PYTHON_MODULE(arr2_epec_seg)
{
  using namespace boost::python;
  
  export_Kernel();
  export_Arrangement_2();
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

}