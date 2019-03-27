#define CGAL_HEADER_ONLY 1
#include <pybind11/pybind11.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Bbox_2.h>
namespace py = pybind11;
typedef int                                           Number_type;
typedef CGAL::Simple_cartesian<Number_type>           Kernel;
typedef CGAL::Point_2<Kernel> Point_2;
typedef CGAL::Polygon_2<Kernel> Polygon_2;
typedef CGAL::Bbox_2 Bbox_2;

PYBIND11_MODULE(Polygon_2, m)
{
using namespace py;
class_<Point_2>(m, "Point_2")
  .def(init<int, int>())
  .def("x", &Point_2::x)
  .def("y", &Point_2::y)
  ;

class_<Polygon_2>(m, "Polygon_2")
  .def(init<>())
  .def("push_back", &Polygon_2::push_back)
  .def("is_simple", &Polygon_2::is_simple)
  .def("is_convex", &Polygon_2::is_convex)
  .def("area", &Polygon_2::area)
  .def("bbox", &Polygon_2::bbox)
  ;

class_<Bbox_2>(m, "Bbox_2")
  .def("xmin", &Bbox_2::xmin)
  .def("ymin", &Bbox_2::ymin)
  .def("xmax", &Bbox_2::xmax)
  .def("ymax", &Bbox_2::ymax)
  ;


}