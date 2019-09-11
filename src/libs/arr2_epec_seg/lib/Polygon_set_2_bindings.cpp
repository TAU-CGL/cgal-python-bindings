#include <Config.hpp>
#include <Common.hpp>

void polygons_with_holes(Polygon_set_2& ps, boost::python::list& lst)
{
  auto v = std::vector<Polygon_with_holes_2>();
  ps.polygons_with_holes(std::back_inserter(v));
  for (auto pwh : v)
  {
    lst.append(pwh);
  }
}

void insert0(Polygon_set_2& ps, Polygon_2& pgn)
{
  ps.insert(pgn);
}

void insert1(Polygon_set_2& ps, Polygon_with_holes_2& pwh)
{
  ps.insert(pwh);
}

void insert2(Polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<Polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);

  ps.insert(v0.begin(), v0.end(), v1.begin(), v1.end());

}

void complement0(Polygon_set_2& ps0, Polygon_set_2& ps1)
{
  ps0.complement(ps1);
}
template <typename T>
void intersection(Polygon_set_2& ps, T& other)
{
  ps.intersection(other);
}

void export_Polygon_set_2()
{
  using namespace boost::python;
  class_<Polygon_set_2>("Polygon_set_2")
    .def(init<>())
    .def(init<const Polygon_2&>())
    .def(init<const Polygon_with_holes_2&>())
    .def("is_empty", &Polygon_set_2::is_empty)
    .def("is_plane", &Polygon_set_2::is_plane)
    .def("number_of_polygons_with_holes", &Polygon_set_2::number_of_polygons_with_holes)
    .def("polygons_with_holes", &polygons_with_holes)
    //.def("arrangement", &Polygon_set_2::arrangement)
    .def("clear", &Polygon_set_2::clear)
    .def("insert", &insert0)
    .def("insert", &insert1)
    .def("insert", &insert2)
    .def<void (Polygon_set_2::*) ()>("complement", &Polygon_set_2::complement)
    .def("complement", &complement0)
    .def("intersection", &intersection<Polygon_set_2>)
    .def("intersection", &intersection<Polygon_2>)
    .def("intersection", &intersection<Polygon_with_holes_2>)

    ;
}