#include <Common.h>

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

#ifdef EXTENDED_DCEL
BOOST_PYTHON_MODULE(arr2_epec_seg_ex)
#else
BOOST_PYTHON_MODULE(arr2_epec_seg)
#endif // EXTENDED_DCEL
{
  using namespace boost::python;

  
  export_Kernel();
  export_Arrangement_2();
  export_Vertex();
  export_Halfedge();
  export_Face();
  export_Intersections_2();
  export_Point_location();

#ifdef ARR_LINEAR_TRAITS
  export_Arr_linear_traits();
#endif
#ifdef ARR_SEGMENT_TRAITS
  export_Arr_segment_traits();
#endif
#ifdef ARR_CIRCLE_SEGMENT_TRAITS
  export_Arr_circle_segment_traits();
#endif
#ifdef ARR_ALGEBRAIC_SEGMENT_TRAITS
  export_Arr_algebraic_segment_traits();
#endif

}