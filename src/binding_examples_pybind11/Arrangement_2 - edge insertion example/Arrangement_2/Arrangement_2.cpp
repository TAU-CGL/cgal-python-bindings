#define CGAL_HEADER_ONLY 1
#include <pybind11/pybind11.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include "arr_print.h"

namespace py = pybind11;
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


PYBIND11_MODULE(Arrangement_2, m)
{
	using namespace py;
	class_<Point_2>(m, "Point_2")
		.def(init<int, int>())
		.def("x", &Point_2::x)
		.def("y", &Point_2::y)
		;

	class_<Segment_2>(m, "Segment_2")
		.def(init<Point_2, Point_2>())
		;

	class_<Halfedge_handle>(m, "Halfedge_handle")
		.def("val", &Halfedge_handle::operator*, return_value_policy::reference_internal)
		;

	class_<Halfedge>(m, "Halfedge")
		.def("source", py::overload_cast<>(&Halfedge::source))
		.def("target", py::overload_cast<>(&Halfedge::target))
		;

	class_<Vertex_handle>(m, "Vertex_handle")
		;

	class_<Face_handle>(m, "Face_handle")
		.def(init<>())
		;

	class_<Arrangement_2>(m, "Arrangement_2")
		.def(init<>())
		.def("unbounded_face", py::overload_cast<>(&Arrangement_2::unbounded_face), return_value_policy::reference_internal)
		.def("insert_in_face_interior", py::overload_cast<const Segment_2&, Face_handle>(&Arrangement_2::insert_in_face_interior))
		.def("insert_from_left_vertex", py::overload_cast<const Segment_2&, Vertex_handle, Face_handle>(&Arrangement_2::insert_from_left_vertex))
		.def("insert_from_right_vertex", py::overload_cast<const Segment_2&, Vertex_handle, Face_handle>(&Arrangement_2::insert_from_right_vertex))
		.def("insert_at_vertices", py::overload_cast<const Segment_2&, Vertex_handle, Vertex_handle, Face_handle>(&Arrangement_2::insert_at_vertices))
		;
	
	m.def("print_arrangement", py::overload_cast<const Arrangement_2&>(&print_arrangement<Arrangement_2>));

}