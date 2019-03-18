#include <pybind11/pybind11.h>
namespace py = pybind11;

#define CGAL_HEADER_ONLY 1
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Point_2.h>

typedef int                                           Number_type;
typedef CGAL::Simple_cartesian<Number_type>           Kernel;
typedef CGAL::Point_2<Kernel>            Point_2;
typedef CGAL::Segment_2<Kernel>                       Segment_2;

PYBIND11_MODULE(Segment_2, m)
{
	using namespace py;
	class_<Segment_2>(m, "Segment_2")
		.def(init<Point_2, Point_2>())
		;

}