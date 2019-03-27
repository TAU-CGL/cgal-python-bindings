#define BOOST_PYTHON_STATIC_LIB
#define CGAL_HEADER_ONLY 1
#include <boost/python.hpp>
#include <CGAL/Polygon_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef int                                           Number_type;
typedef CGAL::Simple_cartesian<Number_type>           Kernel;
typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Bbox_2 Bbox_2;

BOOST_PYTHON_MODULE(Polygon_2)
{
	using namespace boost::python;
	class_<Point_2>("Point_2", init<int, int>())
		//specifiying return policy is not needed in pybind11
		.def("x", &Point_2::x, return_value_policy<copy_const_reference>())
		.def("y", &Point_2::y, return_value_policy<copy_const_reference>())
		;
	class_<Polygon_2>("Polygon_2", init<>())
		.def("push_back", &Polygon_2::push_back)
		.def("is_simple", &Polygon_2::is_simple)
		.def("is_convex", &Polygon_2::is_convex)
		.def("area", &Polygon_2::area)
    .def("bbox", &Polygon_2::bbox)
		;

  class_<Bbox_2>("Bbox_2")
    .def("xmin", &Bbox_2::xmin)
    .def("ymin", &Bbox_2::ymin)
    .def("xmax", &Bbox_2::xmax)
    .def("ymax", &Bbox_2::ymax)
    ;
}