#define CGAL_HEADER_ONLY 1
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_extended_dcel.h>

namespace py = pybind11;
enum Color { BLUE, RED, WHITE };

typedef CGAL::Cartesian<CGAL::Exact_rational>                   Kernel;
typedef CGAL::Arr_segment_traits_2<Kernel>                      Traits_2;
typedef Traits_2::Point_2                                       Point_2;
typedef Traits_2::X_monotone_curve_2                            Segment_2;
typedef CGAL::Arr_extended_dcel<Traits_2, Color, bool, int>     Dcel;
typedef CGAL::Arrangement_2<Traits_2, Dcel>                     Arrangement_2;
typedef Arrangement_2::Vertex_iterator							Vertex_iterator;
typedef Arrangement_2::Vertex									Vertex;
typedef Arrangement_2::Halfedge_iterator						Halfedge_iterator;
typedef Arrangement_2::Ccb_halfedge_circulator					Ccb_halfedge_circulator;
typedef Arrangement_2::Halfedge									Halfedge;
typedef Arrangement_2::Face_iterator							Face_iterator;
typedef Arrangement_2::Face										Face;

Halfedge_iterator insert_non_intersecting_curve(Arrangement_2 &arr, Segment_2 &s)
{
	return CGAL::insert_non_intersecting_curve(arr, s);
}

void insert(Arrangement_2 &arr, Segment_2 &s)
{
	return CGAL::insert(arr, s);
}


void print_arr(Arrangement_2 &arr)
{
	Arrangement_2::Vertex_iterator            vit;
	std::cout << "The arrangement vertices:" << std::endl;
	for (vit = arr.vertices_begin(); vit != arr.vertices_end(); ++vit) {
		std::cout << '(' << vit->point() << ") - ";
		switch (vit->data()) {
		case BLUE: std::cout << "BLUE." << std::endl; break;
		case RED: std::cout << "RED." << std::endl; break;
		case WHITE: std::cout << "WHITE." << std::endl; break;
		}
	}

	std::cout << "Printing boundary sizes" << std::endl;
	Arrangement_2::Face_iterator              fit;
	for (fit = arr.faces_begin(); fit != arr.faces_end(); ++fit) {
		if (!fit->is_unbounded()) {
			std::cout << fit->data() << std::endl;
		}
	}
}



PYBIND11_MODULE(Arrangement_2, m)
{
	using namespace py;

	enum_<Color>(m, "Color")
		.value("BLUE", Color::BLUE)
		.value("RED", Color::RED)
		.value("WHITE", Color::WHITE)
		.export_values();

	class_<Arrangement_2>(m, "Arrangement_2")
		.def(init<>())
		.def("vertices_begin", py::overload_cast<>(&Arrangement_2::vertices_begin))
		.def("vertices_end", py::overload_cast<>(&Arrangement_2::vertices_end))
		.def("faces_begin", overload_cast<>(&Arrangement_2::faces_begin))
		.def("faces_end", overload_cast<>(&Arrangement_2::faces_end))
		/*
		.def("vertices_begin", [](Arrangement_2 &a) {return a.vertices_begin(); })
		.def("vertices_end", [](Arrangement_2 &a) {return a.vertices_end(); })
		.def("faces_begin", [](Arrangement_2 &a) {return a.faces_begin(); })
		.def("faces_end", [](Arrangement_2 &a) {return a.faces_end(); })
		*/
		;

	class_<Point_2>(m, "Point_2")
		.def(init<int, int>())
		.def("x", &Point_2::x)
		.def("y", &Point_2::y)
		;
	class_<CGAL::Exact_rational>(m, "Exact_Rational")
		.def("val", &CGAL::Exact_rational::to_double)
		;

	class_<Segment_2>(m, "Segment_2")
		.def(init<Point_2, Point_2>())
		;

	class_<Halfedge_iterator>(m, "Halfedge_iterator")
		.def("val", &Halfedge_iterator::operator*, return_value_policy::reference_internal)
		.def("inc", [](Halfedge_iterator &a) {return ++a; })
		.def(py::self == self)
		.def(self != self)
		;

	class_<Ccb_halfedge_circulator>(m, "Ccb_halfedge_circulator")
		.def("val", &Ccb_halfedge_circulator::operator*, return_value_policy::reference_internal)
		.def("inc", [](Ccb_halfedge_circulator &a) {return ++a; })
		.def(self == self)
		.def(self != self)
		;

	class_<Halfedge>(m, "Halfedge")
		.def("source", py::overload_cast<>(&Halfedge::source))
		.def("target", py::overload_cast<>(&Halfedge::target))
		;

	class_<Vertex_iterator>(m, "Vertex_iterator")
		.def("inc", [](Vertex_iterator &a) {return ++a; })
		.def("val", &Vertex_iterator::operator*, return_value_policy::reference_internal)
		.def(self == self)
		.def(self != self)
		;

	class_<Vertex>(m, "Vertex")
		.def(init<>())
		.def("degree", &Vertex::degree)
		.def("data", py::overload_cast<>(&Vertex::data), return_value_policy::reference_internal)
		.def("set_data", &Vertex::set_data)
		.def("point", py::overload_cast<>(&Vertex::point))
		;

	class_<Face_iterator>(m, "Face_iterator")
		.def("inc", [](Face_iterator &a) {return ++a; })
		.def("val", &Face_iterator::operator*, return_value_policy::reference_internal)
		.def(self == self)
		.def(self != self)
		;

	class_<Face>(m, "Face")
		.def("is_unbounded", &Face::is_unbounded)
		.def("outer_ccb", overload_cast<>(&Face::outer_ccb))
		.def("data", py::overload_cast<>(&Face::data), return_value_policy::reference_internal)
		.def("set_data", &Face::set_data)
		;

	//m.def("insert_non_intersecting_curve", &CGAL::insert_non_intersecting_curve<Traits_2, CGAL::Default_planar_topology<Traits_2, Dcel>::Traits>);
	m.def("insert_non_intersecting_curve", &insert_non_intersecting_curve);
	m.def("insert_old", &insert);
	m.def("insert", [](Arrangement_2 &arr, Segment_2 &s) {return CGAL::insert(arr, s); });
	m.def("print_arr", &print_arr);

	
}