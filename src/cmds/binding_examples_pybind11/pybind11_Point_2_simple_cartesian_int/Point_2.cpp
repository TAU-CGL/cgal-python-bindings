#include <pybind11/pybind11.h>
namespace py = pybind11;

#define CGAL_HEADER_ONLY 1
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Point_2.h>

typedef int                                           Number_type;
typedef CGAL::Simple_cartesian<Number_type>           Kernel;
typedef CGAL::Point_2<Kernel>            Point_2;

PYBIND11_MODULE(Point_2, m)
{
	using namespace py;
	class_<Point_2>(m, "Point_2")
		.def(init<int, int>())
		.def("x", &Point_2::x)
		.def("y", &Point_2::y)
		;
}