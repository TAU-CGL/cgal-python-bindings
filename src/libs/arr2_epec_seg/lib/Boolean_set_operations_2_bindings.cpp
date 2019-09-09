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


void export_Boolean_set_operations_2()
{
  using namespace boost::python;
  def("complement", complement0);
  def("complement", complement1);
}