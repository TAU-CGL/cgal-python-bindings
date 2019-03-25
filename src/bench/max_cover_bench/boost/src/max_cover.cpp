#define BOOST_PYTHON_STATIC_LIB
#define CGAL_HEADER_ONLY 1
#include <boost/python.hpp>
#define CGAL_HEADER_ONLY 1
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/stl_iterator.hpp>
#include <fstream>
#include <sstream>
#include <ostream>
#include <queue>
#include <list>
#include <string>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arr_extended_dcel.h>

namespace po = boost::program_options;

typedef CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef Kernel::FT                                              Number_type;
typedef CGAL::Arr_circle_segment_traits_2<Kernel>               Traits;
typedef Traits::CoordNT                                         CoordNT;
typedef Traits::Point_2                                         Point_2;
typedef Traits::Curve_2                                         Curve;
typedef Traits::Rational_point_2                                Rational_point;
typedef Traits::Rational_segment_2                              Segment;

typedef CGAL::_X_monotone_circle_segment_2<CGAL::Epeck, 1>		Circle_segment;

typedef Traits::Rational_circle_2                               Circle;
typedef CGAL::Arr_extended_dcel<Traits, int, bool, int>   Dcel;
typedef CGAL::Arrangement_2<Traits, Dcel>                       Arrangement_2;

typedef Arrangement_2::Vertex_iterator							Vertex_iterator;
typedef Arrangement_2::Vertex									Vertex;
typedef Arrangement_2::Inner_ccb_iterator						Inner_ccb_iterator;
typedef Arrangement_2::Halfedge_iterator						Halfedge_iterator;
typedef Arrangement_2::Ccb_halfedge_circulator					Ccb_halfedge_circulator;
typedef Arrangement_2::Halfedge_around_vertex_circulator		Halfedge_around_vertex_circulator;
typedef Arrangement_2::Halfedge									Halfedge;
typedef Arrangement_2::Edge_iterator							Edge_iterator;
typedef Arrangement_2::Face_iterator							Face_iterator;
typedef Arrangement_2::Face										Face;

typedef std::vector<std::string>                                vs;

// Converts a C++ vector to a python list
template <class T>
boost::python::list to_python_list(std::vector<T> vector) {
  typename std::vector<T>::iterator iter;
  boost::python::list list;
  for (iter = vector.begin(); iter != vector.end(); ++iter) {
    list.append(*iter);
  }
  return list;
}

boost::python::list initialize(double i, std::string filename)
{
  Kernel::FT r_square = i;
  std::vector<Curve> curves;
  std::ifstream is;
  is.open(filename.c_str());
  if (!is.is_open()) {
    std::cerr << "Failed to open " << filename << "!" << std::endl;
    return to_python_list(curves);
  }
  size_t n;
  is >> n;
  curves.resize(n);
  for (auto i = 0; i < n; ++i) {
    Kernel::FT x, y;
    is >> x >> y;
    curves[i] = Curve(Circle(Rational_point(x, y), r_square));
  }
  return to_python_list(curves);
}

boost::python::list initialize1(double i, std::string filename)
{
  Kernel::FT r_square = i;
  boost::python::list curves;
  std::ifstream is;
  is.open(filename.c_str());
  if (!is.is_open()) {
    std::cerr << "Failed to open " << filename << "!" << std::endl;
    return curves;
  }
  size_t n;
  is >> n;
  for (auto i = 0; i < n; ++i) {
    Kernel::FT x, y;
    is >> x >> y;
    curves.append(Curve(Circle(Rational_point(x, y), r_square)));
  }
  return curves;

}

void insert_list(Arrangement_2& arr, boost::python::list& lst)
{
  auto v = std::vector< Curve >(boost::python::stl_input_iterator< Curve >(lst),
    boost::python::stl_input_iterator< Curve >());
  CGAL::insert(arr, v.begin(), v.end());
  //CGAL::insert(arr, boost::python::stl_input_iterator<Curve>(lst), boost::python::stl_input_iterator<Curve>());
}

void insert(Arrangement_2& arr, Curve& s) { CGAL::insert(arr, s); }

//Kernel
Kernel::Equal_2 k_eq2(Kernel& k)
{
  return (Kernel::Equal_2)(k.equal_2_object());
}

//Vertex/Edge/Face

template <class c> 
typename c::Data& get_data(c& a) { return a.data(); }

//Iterators

template <class iterator>
iterator iterator_inc(iterator& a) { return ++a; }

template <class iterator>
iterator iterator_copy(iterator& a) { return a; }

//Edge_iterator

void Edge_iterator_inc(Edge_iterator& a) { ++a; }

//printing
void print_point(CGAL::Point_2<CGAL::Epeck> &p) { std::cout << p << std::endl; }
void print_point1(Point_2 &p) { std::cout << p << std::endl; }

BOOST_PYTHON_MODULE(max_cover)
{
	using namespace boost::python;
  enum_<CGAL::Sign>("Sign")
    ;
  class_ <Kernel>("Kernel")
    .def(init<>())
    .def("equal_2_object", &k_eq2)
    ;

  class_<Traits>("Traits")
    .def(init<>())
    .def("equal_2_object", &Traits::equal_2_object)
    .def("compare_xy_2_object", &Traits::compare_xy_2_object)
    ;
    
  class_<Traits::Compare_xy_2>("Traits_compare_xy_2")
    .def<CGAL::Sign (Traits::Compare_xy_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Compare_xy_2::operator())
    ;

  class_<Traits::Equal_2>("Traits_equal_2_object")
    .def<bool (Traits::Equal_2::*)(const Point_2&, const Point_2&) const>("__call__", &Traits::Equal_2::operator())
    ;

  class_<Kernel::Equal_2>("Kernel_equal_2_object")
    .def<bool (Kernel::Equal_2::*)(const Rational_point&, const Rational_point&) const>("__call__", &Kernel::Equal_2::operator())
    ;

  class_<Arrangement_2>("Arrangement")
    .def(init<>())
    .def(init<const Traits*>())
    .def<Vertex_iterator(Arrangement_2::*)()>("vertices_begin", &Arrangement_2::vertices_begin)
    .def<Vertex_iterator(Arrangement_2::*)()>("vertices_end", &Arrangement_2::vertices_end)
    .def<Edge_iterator(Arrangement_2::*)()>("edges_begin", &Arrangement_2::edges_begin)
    .def<Edge_iterator(Arrangement_2::*)()>("edges_end", &Arrangement_2::edges_end)
    .def<Halfedge_iterator(Arrangement_2::*)()>("halfedges_begin", &Arrangement_2::halfedges_begin)
    .def<Halfedge_iterator(Arrangement_2::*)()>("halfedges_end", &Arrangement_2::halfedges_end)
    .def<Face_iterator(Arrangement_2::*)()>("faces_begin", &Arrangement_2::faces_begin)
    .def<Face_iterator(Arrangement_2::*)()>("faces_end", &Arrangement_2::faces_end)
    .def<Face_iterator(Arrangement_2::*)()>("unbounded_face", &Arrangement_2::unbounded_face)
    ;
  class_<Point_2>("Point")
    .def(init<int, int>())
    .def("x", &Point_2::x, return_value_policy<copy_const_reference>())
    .def("y", &Point_2::y, return_value_policy<copy_const_reference>())
    ;

  class_<Segment>("Segment")
    .def("source", &Segment::source)
    .def("target", &Segment::target)
    ;

  class_<Circle_segment>("Circle_segment")
    .def("source", &Circle_segment::source, return_value_policy<copy_const_reference>())
    .def("target", &Circle_segment::target, return_value_policy<copy_const_reference>())
    .def("supporting_circle", &Circle_segment::supporting_circle)
    ;

  class_<Curve>("Curve")
    .def(init<Circle&>())
    .def("supporting_circle", &Curve::supporting_circle, return_value_policy<copy_const_reference>())
    .def("source", &Curve::source, return_value_policy<copy_const_reference>())
    .def("target", &Curve::target, return_value_policy<copy_const_reference>())
    ;

  class_<Circle>("Circle")
    .def(init<Rational_point, Kernel::FT>())
    .def("center", &Circle::center)
    ;

  class_<CGAL::Point_2<CGAL::Epeck>>("Point_2")
    ;

  class_<Inner_ccb_iterator>("Inner_ccb_iterator")
    .def("val", &Inner_ccb_iterator::operator*)
    .def("inc", &iterator_inc<Inner_ccb_iterator>)
    .def(self == self)
    .def(self != self)
    ;

  class_<Halfedge_iterator>("Halfedge_iterator")
    .def("val", &Halfedge_iterator::operator*, return_value_policy<reference_existing_object>())
    .def("inc", &iterator_inc<Halfedge_iterator>)
    .def(self == self)
    .def(self != self)
    ;

  class_<Ccb_halfedge_circulator>("Ccb_halfedge_circulator")
    .def("val", &Ccb_halfedge_circulator::operator*, return_value_policy<copy_non_const_reference>())
    .def("inc", iterator_inc<Ccb_halfedge_circulator>)
    .def(self == self)
    .def(self != self)
    .def("copy", iterator_copy<Ccb_halfedge_circulator>)
    ;

  class_<Halfedge_around_vertex_circulator>("Halfedge_around_vertex_circulator")
    .def("val", &Halfedge_around_vertex_circulator::operator*, return_value_policy<copy_non_const_reference>())
    .def("inc", iterator_inc<Halfedge_around_vertex_circulator>)
    .def(self == self)
    .def(self != self)
    .def("copy", iterator_copy<Halfedge_around_vertex_circulator>)
    ;

  class_<Halfedge>("Halfedge")
    .def<Vertex_iterator (Halfedge::*)()>("source", &Halfedge::source)
    .def<Vertex_iterator(Halfedge::*)()>("target", &Halfedge::target)
    .def<Halfedge_iterator(Halfedge::*)()>("twin", &Halfedge::twin)
    .def<Face_iterator(Halfedge::*)()>("face", &Halfedge::face)
    .def<Halfedge::X_monotone_curve& (Halfedge::*)()>("curve", &Halfedge::curve, return_value_policy<reference_existing_object>())
    .def("set_data", &Halfedge::set_data)
    .def("data", &get_data<Halfedge>, return_value_policy<copy_non_const_reference>()) //elementary type
    ;

  class_<Edge_iterator>("Edge_iterator")
    .def("inc", &Edge_iterator_inc) //return type is not Edge_iterator
    .def("val", &Edge_iterator::operator*, return_internal_reference<>())
    .def(self == self)
    .def(self != self)
    ;

  class_<Vertex_iterator>("Vertex_iterator")
    .def("inc", &iterator_inc<Vertex_iterator>)
    .def("val", &Vertex_iterator::operator*, return_internal_reference<>())
    .def(self == self)
    .def(self != self)
    .def("copy", &iterator_copy<Vertex_iterator>)
    ;

  class_<Vertex>("Vertex")
    .def(init<>())
    .def("degree", &Vertex::degree)
    .def("data", &get_data<Vertex>, return_value_policy<copy_non_const_reference>()) //elementary type
    .def("set_data", &Vertex::set_data)
    .def<Point_2& (Vertex::*)()>("point", &Vertex::point, return_internal_reference<>())
    .def<Halfedge_around_vertex_circulator (Vertex::*)()>("incident_halfedges", &Vertex::incident_halfedges)
    ;

  class_<Face_iterator>("Face_iterator")
    .def("inc", iterator_inc<Face_iterator>)
    .def("val", &Face_iterator::operator*, return_internal_reference<>())
    .def(self == self)
    .def(self != self)
    ;

  class_<Face>("Face")
    .def("is_unbounded", &Face::is_unbounded)
    .def<Ccb_halfedge_circulator (Face::*)()>("outer_ccb", &Face::outer_ccb)
    .def<Inner_ccb_iterator (Face::*)()>("inner_ccbs_begin", &Face::inner_ccbs_begin)
    .def<Inner_ccb_iterator(Face::*)()>("inner_ccbs_end", &Face::inner_ccbs_end)
    .def("data", &get_data<Face>, return_value_policy<copy_non_const_reference>()) //elementary type
    .def("set_data", &Face::set_data)
    ;

  boost::python::def("pp", &print_point);
  boost::python::def("pp2",&print_point1);
  boost::python::def("insert", &insert);
  boost::python::def("insert_list", &insert_list);
  boost::python::def("initialize", &initialize);
}