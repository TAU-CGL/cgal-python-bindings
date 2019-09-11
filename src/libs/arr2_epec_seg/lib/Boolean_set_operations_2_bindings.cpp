#include <Config.hpp>
#include <Common.hpp>

void complement0(Polygon_2& pgn, Polygon_with_holes_2& res)
{
  CGAL::complement(pgn, res);
}

void complement1(Polygon_with_holes_2& pgn, boost::python::list& lst)
{
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::complement(pgn, std::back_inserter(v));
  for (auto p : v)
  {
    lst.append(p);
  }
}

template <typename T0, typename T1>
bool do_intersect(T0& p0, T1& p1)
{
  return CGAL::do_intersect(p0, p1);
}

template <typename T0, typename T1>
void intersection_linear(T0& p0, T1& p1, boost::python::list& lst)
{
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::intersection(p0, p1, std::back_inserter(v));
  for (auto p : v)
  {
    lst.append(p);
  }
}

template <typename T0, typename T1>
void difference_linear(T0& p0, T1& p1, boost::python::list& lst)
{
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::difference(p0, p1, std::back_inserter(v));
  for (auto p : v)
  {
    lst.append(p);
  }
}

template <typename T0, typename T1>
void symmetric_difference_linear(T0& p0, T1& p1, boost::python::list& lst)
{
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::symmetric_difference(p0, p1, std::back_inserter(v));
  for (auto p : v)
  {
    lst.append(p);
  }
}

template <typename T0, typename T1>
CGAL::Oriented_side oriented_side(T0& p0, T1& p1)
{
  return CGAL::oriented_side(p0, p1);
}

void connect_holes(Polygon_with_holes_2& pwh, boost::python::list& lst)
{
  auto v = std::vector<Point_2>();
  CGAL::connect_holes(pwh, std::back_inserter(v));
  for (auto p : v)
  {
    lst.append(p);
  }
}


void export_Boolean_set_operations_2()
{
  using namespace boost::python;
  def("complement", complement0);
  def("complement", complement1);
  def("do_intersect", &do_intersect<Polygon_2, Polygon_2>);
  def("do_intersect", &do_intersect<Polygon_2, Polygon_with_holes_2>);
  def("do_intersect", &do_intersect<Polygon_with_holes_2, Polygon_2>);
  def("do_intersect", &do_intersect<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("intersection", &intersection_linear<Polygon_2, Polygon_2>);
  def("intersection", &intersection_linear<Polygon_2, Polygon_with_holes_2>);
  def("intersection", &intersection_linear<Polygon_with_holes_2, Polygon_2>);
  def("intersection", &intersection_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("difference", &difference_linear<Polygon_2, Polygon_2>);
  def("difference", &difference_linear<Polygon_2, Polygon_with_holes_2>);
  def("difference", &difference_linear<Polygon_with_holes_2, Polygon_2>);
  def("difference", &difference_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("symmetric_difference", &symmetric_difference_linear<Polygon_2, Polygon_2>);
  def("symmetric_difference", &symmetric_difference_linear<Polygon_2, Polygon_with_holes_2>);
  def("symmetric_difference", &symmetric_difference_linear<Polygon_with_holes_2, Polygon_2>);
  def("symmetric_difference", &symmetric_difference_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("oriented_side", &oriented_side<Polygon_2, Polygon_2>);
  def("oriented_side", &oriented_side<Polygon_2, Polygon_with_holes_2>);
  def("oriented_side", &oriented_side<Polygon_with_holes_2, Polygon_2>);
  def("oriented_side", &oriented_side<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("connect_holes", &connect_holes);


}