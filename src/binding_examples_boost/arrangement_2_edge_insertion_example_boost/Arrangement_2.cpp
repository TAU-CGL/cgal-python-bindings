#define BOOST_PYTHON_STATIC_LIB
#define CGAL_HEADER_ONLY 1
#include <boost/python.hpp>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include "arr_print.h"

typedef int                                           Number_type;
typedef CGAL::Simple_cartesian<Number_type>           Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>            Traits_2;
typedef Traits_2::Point_2                             Point_2;
typedef Traits_2::X_monotone_curve_2                  Segment_2;
typedef CGAL::Arrangement_2<Traits_2>                 Arrangement_2;
typedef Arrangement_2::Vertex_handle                  Vertex_handle;
typedef Arrangement_2::Vertex						  Vertex;
typedef Arrangement_2::Halfedge_handle                Halfedge_handle;
typedef Arrangement_2::Halfedge						  Halfedge;
typedef Arrangement_2::Face_handle					  Face_handle;


//how to deal with overloaded methods in boost python
typename Vertex_handle (Halfedge::*source1)() = &Halfedge::source;
typename Vertex_handle (Halfedge::*target1)() = &Halfedge::target;
typename Face_handle(Arrangement_2::*unbounded_face1)() = &Arrangement_2::unbounded_face;
typename Halfedge_handle(Arrangement_2::*insert_in_face_interior1)(const Segment_2&, Face_handle) = &Arrangement_2::insert_in_face_interior;
typename Halfedge_handle(Arrangement_2::*insert_from_left_vertex1)(const Segment_2&, Vertex_handle, Face_handle) = &Arrangement_2::insert_from_left_vertex;
typename Halfedge_handle(Arrangement_2::*insert_from_right_vertex1)(const Segment_2&, Vertex_handle, Face_handle) = &Arrangement_2::insert_from_right_vertex;
typename Halfedge_handle(Arrangement_2::*insert_at_vertices1)(const Segment_2&, Vertex_handle, Vertex_handle, Face_handle) = &Arrangement_2::insert_at_vertices;
void (*print_arrangement1)(const Arrangement_2&) = &print_arrangement<Arrangement_2>;


BOOST_PYTHON_MODULE(Arrangement_2)
{
	using namespace boost::python;
	class_<Point_2>("Point_2", init<int, int>())
		.def("x", &Point_2::x, return_value_policy<copy_const_reference>())
		.def("y", &Point_2::y, return_value_policy<copy_const_reference>())
		;

	class_<Segment_2>("Segment_2", init<Point_2, Point_2>())
		;
	
	class_<Halfedge_handle>("Halfedge_handle")
		.def("val", &Halfedge_handle::operator*, return_internal_reference<>())
		;

	class_<Halfedge>("Halfedge")
		.def("source", source1)
		.def("target", target1)
		;

	class_<Vertex_handle>("Vertex_handle")
		;

	class_<Face_handle>("Face_handle", init<>())
		;

	
	class_<Arrangement_2>("Arrangement_2", init<>())
		.def("unbounded_face", unbounded_face1)
		.def("insert_in_face_interior", insert_in_face_interior1)
		.def("insert_from_left_vertex", insert_from_left_vertex1)
		.def("insert_from_right_vertex", insert_from_right_vertex1)
		.def("insert_at_vertices", insert_at_vertices1)
		;

	def("print_arrangement", print_arrangement1);
	
}