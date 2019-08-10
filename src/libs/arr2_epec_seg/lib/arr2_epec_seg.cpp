#include <Common.h>

void export_Kernel();
void export_Arrangement_2();
void export_Intersections_2();
void export_Point_location();
void export_Vertex();
void export_Halfedge();
void export_Face();

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

}