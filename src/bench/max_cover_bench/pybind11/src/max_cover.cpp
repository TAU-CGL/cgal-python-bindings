#define CGAL_HEADER_ONLY 1
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
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
namespace py = pybind11;
using namespace py;

std::vector<Curve> initialize(double i, std::string filename)
{
  Kernel::FT r_square = i;
  std::vector<Curve> curves;
  std::ifstream is;
  is.open(filename.c_str());
  if (!is.is_open()) {
    std::cerr << "Failed to open " << filename << "!" << std::endl;
    return curves;
  }
  size_t n;
  is >> n;
  curves.resize(n);
  for (auto i = 0; i < n; ++i) {
    Kernel::FT x, y;
    is >> x >> y;
    curves[i] = Curve(Circle(Rational_point(x, y), r_square));
  }
  return curves;
}

Arrangement_2 init2(int i, std::string filename)
{
  Traits traits;
  Arrangement_2 arr(&traits);
  auto curves = initialize(i, filename);
  for (const auto& cv : curves) insert(arr, cv);
  return arr;
}

Arrangement_2 init3(std::vector<Curve> &curves)
{
  Traits traits;
  Arrangement_2 arr(&traits);
  for (const auto& cv : curves) insert(arr, cv);
  return arr;
}

PYBIND11_MODULE(max_cover, m)
{
  enum_<CGAL::Sign>(m, "Sign")
    ;


  class_ <Kernel>(m, "Kernel")
    .def(init<>())
    .def("equal_2_object", [](Kernel& a) {return (Kernel::Equal_2)(a.equal_2_object()); })
    ;

  class_<Traits>(m, "Traits")
    .def(init<>())
    .def("equal_2_object", &Traits::equal_2_object)
    .def("compare_xy_2_object", &Traits::compare_xy_2_object)
    ;
  class_<Traits::Equal_2>(m, "Traits_equal_2_object")
    .def("__call__", py::overload_cast<const Point_2&, const Point_2&>(&Traits::Equal_2::operator(), py::const_))
    ;

  class_<Traits::Compare_xy_2>(m, "Traits_compare_xy_2")
    .def("__call__", py::overload_cast<const Point_2&, const Point_2&>(&Traits::Compare_xy_2::operator(), py::const_))
    ;

  class_<Kernel::Equal_2>(m, "Kernel_equal_2_object")
    .def("__call__", [](Kernel::Equal_2 a, Rational_point b, Rational_point c) {return a(b, c); })
    ;

  class_<Arrangement_2>(m, "Arrangement")
    .def(init<>())
    .def(init([](Traits traits) {return new Arrangement_2(&traits); }), return_value_policy::take_ownership)
    .def("vertices_begin", py::overload_cast<>(&Arrangement_2::vertices_begin))
    .def("vertices_end", py::overload_cast<>(&Arrangement_2::vertices_end))
    .def("edges_begin", py::overload_cast<>(&Arrangement_2::edges_begin))
    .def("edges_end", py::overload_cast<>(&Arrangement_2::edges_end))
    .def("halfedges_begin", py::overload_cast<>(&Arrangement_2::halfedges_begin))
    .def("halfedges_end", py::overload_cast<>(&Arrangement_2::halfedges_end))
    .def("faces_begin", overload_cast<>(&Arrangement_2::faces_begin))
    .def("faces_end", overload_cast<>(&Arrangement_2::faces_end))
    .def("unbounded_face", overload_cast<>(&Arrangement_2::unbounded_face))
    ;

  class_<Point_2>(m, "Point")
    .def(init<int, int>())
    .def("x", &Point_2::x)
    .def("y", &Point_2::y)
    ;

  class_<Segment>(m, "Segment")
    .def("source", &Segment::source)
    .def("target", &Segment::target)
    ;

  class_<Circle_segment>(m, "Circle_segment")
    .def("source", &Circle_segment::source)
    .def("target", &Circle_segment::target)
    .def("supporting_circle", &Circle_segment::supporting_circle)
    ;

  class_<Curve>(m, "Curve")
    .def(init<Circle&>())
    .def("supporting_circle", &Curve::supporting_circle)
    .def("source", &Curve::source)
    .def("target", &Curve::target)
    ;

  class_<Circle>(m, "Circle")
    .def(init<Rational_point, Kernel::FT>())
    .def("center", &Circle::center)
    ;

  class_<CGAL::Point_2<CGAL::Epeck>>(m, "Point_2")
    ;

  class_<Inner_ccb_iterator>(m, "Inner_ccb_iterator")
    .def("val", [](Inner_ccb_iterator& a) {Ccb_halfedge_circulator b = *a; return b; }, return_value_policy::copy)
    .def("inc", [](Inner_ccb_iterator& a) { return ++a; })
    .def(py::self == self)
    .def(self != self)
    ;

  class_<Halfedge_iterator>(m, "Halfedge_iterator")
    .def("val", &Halfedge_iterator::operator*, return_value_policy::reference_internal)
    .def("inc", [](Halfedge_iterator &a) {return ++a; })
    .def(py::self == self)
    .def(self != self)
    ;

  class_<Ccb_halfedge_circulator>(m, "Ccb_halfedge_circulator")
    .def("val", [](Ccb_halfedge_circulator &a) {return *a; }, return_value_policy::copy)
    .def("inc", [](Ccb_halfedge_circulator &a) {return ++a; })
    .def(self == self)
    .def(self != self)
    .def("copy", [](Ccb_halfedge_circulator &a) {return a; }, return_value_policy::copy)
    ;
  class_<Halfedge_around_vertex_circulator>(m, "Halfedge_around_vertex_circulator")
    .def("val", &Halfedge_around_vertex_circulator::operator*)
    .def("inc", [](Halfedge_around_vertex_circulator &a) {return ++a; })
    .def(self == self)
    .def(self != self)
    .def("copy", [](Halfedge_around_vertex_circulator &a) {return a; }, return_value_policy::copy)
    ;
  class_<Halfedge>(m, "Halfedge")
    .def("source", py::overload_cast<>(&Halfedge::source))
    .def("target", py::overload_cast<>(&Halfedge::target))
    .def("twin", py::overload_cast<>(&Halfedge::twin))
    .def("face", py::overload_cast<>(&Halfedge::face))
    .def("curve", py::overload_cast<>(&Halfedge::curve))
    .def("set_data", &Halfedge::set_data)
    .def("data", py::overload_cast<>(&Halfedge::data), return_value_policy::reference_internal)
    ;

  class_<Edge_iterator>(m, "Edge_iterator")
    .def("inc", [](Edge_iterator &a) {++a; }) //return type is not Edge_iterator
    .def("val", &Edge_iterator::operator*, return_value_policy::reference_internal)
    .def(self == self)
    .def(self != self)
    ;

  class_<Vertex_iterator>(m, "Vertex_iterator")
    .def("inc", [](Vertex_iterator &a) {return ++a; })
    .def("val", &Vertex_iterator::operator*, return_value_policy::reference_internal)
    .def(self == self)
    .def(self != self)
    .def("copy", [](Vertex_iterator &a) {return a; }, return_value_policy::copy)
    ;

  class_<Vertex>(m, "Vertex")
    .def(init<>())
    .def("degree", &Vertex::degree)
    .def("data", py::overload_cast<>(&Vertex::data), return_value_policy::reference_internal)
    .def("set_data", &Vertex::set_data)
    .def("point", py::overload_cast<>(&Vertex::point))
    .def("incident_halfedges", py::overload_cast<>(&Vertex::incident_halfedges))
    ;

  class_<Face_iterator>(m, "Face_iterator")
    .def("inc", [](Face_iterator &a) {return ++a; })
    .def("val", &Face_iterator::operator*, return_value_policy::reference_internal)
    .def(self == self)
    .def(self != self)
    ;

  class_<Face>(m, "Face")
    .def("is_unbounded", &Face::is_unbounded)
    .def("outer_ccb", py::overload_cast<>(&Face::outer_ccb))
    .def("inner_ccbs_begin", [](Face &f) {Inner_ccb_iterator it = f.inner_ccbs_begin(); return it; })
    .def("inner_ccbs_end", [](Face &f) {Inner_ccb_iterator it = f.inner_ccbs_end(); return it; })
    .def("data", py::overload_cast<>(&Face::data), return_value_policy::reference_internal)
    .def("set_data", &Face::set_data)
    ;

  m.def("pp", [](CGAL::Point_2<CGAL::Epeck> &p) {std::cout << p << std::endl; });
  m.def("pp2", [](Point_2 &p) { std::cout << p << std::endl; });
  m.def("insert", [](Arrangement_2 &arr, Curve &s) {CGAL::insert(arr, s); });
  m.def("initialize", &initialize, return_value_policy::take_ownership);
}