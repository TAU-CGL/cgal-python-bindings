#include <CGAL/Cartesian.h>
#include <CGAL/Quotient.h>
#include <CGAL/MP_Float.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_polyline_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/IO/Arr_iostream.h>

typedef CGAL::Quotient<CGAL::MP_Float>                  Number_type;
typedef CGAL::Cartesian<Number_type>                    Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>              Segment_traits_2;
typedef CGAL::Arr_polyline_traits_2<Segment_traits_2>   Traits_2;
typedef Traits_2::Point_2                               Point_2;
typedef Traits_2::X_monotone_curve_2                    Polyline_2;
typedef CGAL::Arrangement_2<Traits_2>                   Arrangement_2;
typedef Segment_traits_2::X_monotone_curve_2            XSegment_2;
typedef Arrangement_2::Vertex_handle                    Vertexh;

int main ()
{
  Point_2 p1(1,3), p2(2,4), p3(3,5);
  std::list<XSegment_2> segs;
  XSegment_2 s1(p1, p2);
  segs.push_back(s1);
  XSegment_2 s2(p2, p3);
  segs.push_back(s2);

  Traits_2 traits;
  Polyline_2 c1 =
    traits.construct_x_monotone_curve_2_object()(segs.begin(), segs.end());
  Arrangement_2 arr1(&traits), arr2(&traits);
  arr1.insert_in_face_interior(c1 , arr1.unbounded_face()) ;

  std::ofstream out_file("arr_ex_io1.dat");
  out_file << arr1;
  out_file.close();

  std::ifstream in_file("arr_ex_io1.dat");
  if (in_file.is_open()) {
    std::cout << "open" << std::endl;
    in_file >> arr2;
    in_file.close();
  }
  else {
    std::cout << "not open" << std::endl;
  }

  std::ofstream out_file2("arr_ex_io2.dat");
  out_file2 << arr2;
  out_file2.close();

  return 0;
}
