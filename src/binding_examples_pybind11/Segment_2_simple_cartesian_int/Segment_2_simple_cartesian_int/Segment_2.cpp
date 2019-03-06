#define BOOST_PYTHON_STATIC_LIB
#define CGAL_HEADER_ONLY 1
#include <boost/python.hpp>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Point_2.h>

typedef int                                           Number_type;
typedef CGAL::Simple_cartesian<Number_type>           Kernel;
typedef CGAL::Point_2<Kernel>            Point_2;
typedef CGAL::Segment_2<Kernel>                       Segment_2;

BOOST_PYTHON_MODULE(Segment_2)
{
	using namespace boost::python;
	class_<Point_2>("Point_2", init<int, int>())
		.def("x", &Point_2::x, return_value_policy<copy_const_reference>())
		.def("y", &Point_2::y, return_value_policy<copy_const_reference>())
		;
	class_<Segment_2>("Segment_2", init<const Point_2,const Point_2>())
		;
}