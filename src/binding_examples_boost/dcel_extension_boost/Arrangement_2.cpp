#define BOOST_PYTHON_STATIC_LIB
#define CGAL_HEADER_ONLY 1
#include <boost/python.hpp>
#include <CGAL/Cartesian.h>
#include <CGAL/Exact_rational.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_extended_dcel.h>

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
typename Vertex_iterator(Arrangement_2::*vertices_begin1)() = &Arrangement_2::vertices_begin;
typename Vertex_iterator(Arrangement_2::*vertices_end1)() = &Arrangement_2::vertices_end;
typename Face_iterator(Arrangement_2::*faces_begin1)() = &Arrangement_2::faces_begin;
typename Face_iterator(Arrangement_2::*faces_end1)() = &Arrangement_2::faces_end;
typename Halfedge_iterator(Halfedge_iterator::*hit_inc)(int) = &Halfedge_iterator::operator++;
typename Vertex_iterator(Vertex_iterator::*vit_inc)(int) = &Vertex_iterator::operator++;
typename Vertex::Data& (Vertex::*vertex_data)() = &Vertex::data;
typename Vertex::Point& (Vertex::*vertex_point)() = &Vertex::point;

BOOST_PYTHON_MODULE(Arrangement_2)
{
	using namespace boost::python;
	enum_<Color>("Color")
		.value("BLUE", Color::BLUE)
		.value("RED", Color::RED)
		.value("WHITE", Color::WHITE)
		.export_values();

	class_<Arrangement_2>("Arrangement_2")
		.def(init<>())
		.def("vertices_begin", vertices_begin1)
		.def("vertices_end", vertices_end1)
		.def("faces_begin", faces_begin1)
		.def("faces_end", faces_end1)
		;

	class_<Point_2>("Point_2")
		.def(init<int, int>())
		.def("x", &Point_2::x, return_value_policy<copy_const_reference>())
		.def("y", &Point_2::y, return_value_policy<copy_const_reference>())
		;

	class_<Segment_2>("Segment_2")
		.def(init<Point_2, Point_2>())
		;
	
	class_<Halfedge_iterator>("Halfedge_iterator")
		.def("val", &Halfedge_iterator::operator*, return_internal_reference<>())
		.def("inc", hit_inc)
		.def(self == self)
		.def(self != self)
		;
	
	class_<Halfedge>("Halfedge")
		;

	class_<Vertex_iterator>("Vertex_iterator")
		.def("inc", vit_inc)
		.def("val", &Vertex_iterator::operator*, return_internal_reference<>())
		.def(self == self)
		.def(self != self)
		;
	
	class_<Vertex>("Vertex")
		.def(init<>())
		.def("degree", &Vertex::degree)
		.def("data", vertex_data, return_value_policy<copy_non_const_reference>())
		.def("set_data", &Vertex::set_data)
		.def("point", vertex_point, return_internal_reference<>())
		;
	
	class_<Face_iterator>("Face_iterator")
		.def(init<>())
		;

	def("insert_non_intersecting_curve", &insert_non_intersecting_curve);
	def("insert", &insert);
	def("print_arr", &print_arr);
	
	
}