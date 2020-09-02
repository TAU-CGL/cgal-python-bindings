// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#include "CGALPY/common.hpp"

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

void insert_range0(Polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<Polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);

  ps.insert(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void insert_range(Polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.insert(v.begin(), v.end());
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

void intersection(Polygon_set_2& ps0, Polygon_set_2& ps1, Polygon_set_2& ps2)
{
  ps0.intersection(ps1, ps2);
}

void intersection_range0(Polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<Polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);
  ps.intersection(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void intersection_range(Polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.intersection(v.begin(), v.end());
}

template <typename T>
void join(Polygon_set_2& ps, T& other)
{
  ps.join(other);
}

void join(Polygon_set_2& ps0, Polygon_set_2& ps1, Polygon_set_2& ps2)
{
  ps0.join(ps1, ps2);
}

void join_range0(Polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<Polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);
  ps.join(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void join_range(Polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.join(v.begin(), v.end());
}

template <typename T>
void difference(Polygon_set_2& ps, T& other)
{
  ps.difference(other);
}

void difference(Polygon_set_2& ps0, Polygon_set_2& ps1, Polygon_set_2& ps2)
{
  ps0.difference(ps1, ps2);
}

template <typename T>
void symmetric_difference(Polygon_set_2& ps, T& other)
{
  ps.symmetric_difference(other);
}

void symmetric_difference(Polygon_set_2& ps0, Polygon_set_2& ps1, Polygon_set_2& ps2)
{
  ps0.symmetric_difference(ps1, ps2);
}

void symmetric_difference_range0(Polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<Polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);
  ps.symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void symmetric_difference_range(Polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.symmetric_difference(v.begin(), v.end());
}

template <typename T>
bool do_intersect(Polygon_set_2& ps, T& other)
{
  return ps.do_intersect(other);
}

bool do_intersect_range0(Polygon_set_2& ps, boost::python::list& polygon_lst, boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<Polygon_2>();
  auto begin1 = boost::python::stl_input_iterator<Polygon_with_holes_2>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<Polygon_with_holes_2>();
  auto v0 = std::vector<Polygon_2>(begin0, end0);
  auto v1 = std::vector<Polygon_with_holes_2>(begin1, end1);
  return ps.do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
}

template <typename T>
void do_intersect_range(Polygon_set_2& ps, boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<T>(lst);
  auto end = boost::python::stl_input_iterator<T>();
  auto v = std::vector<T>(begin, end);
  ps.do_intersect(v.begin(), v.end());
}

template<typename T>
CGAL::Oriented_side oriented_side(Polygon_set_2& ps, T& other)
{
  return ps.oriented_side(other);
}

void export_polygon_set_2()
{
  using namespace boost::python;
  class_<Polygon_set_2>("Polygon_set_2")
    .def(init<>())
    .def(init<const Polygon_2&>())
    .def(init<const Polygon_with_holes_2&>())
    .def(init<const Polygon_set_2&>())
    .def("is_empty", &Polygon_set_2::is_empty)
    .def("is_plane", &Polygon_set_2::is_plane)
    .def("number_of_polygons_with_holes", &Polygon_set_2::number_of_polygons_with_holes)
    .def("polygons_with_holes", &polygons_with_holes)
    //.def("arrangement", &Polygon_set_2::arrangement)
    .def("clear", &Polygon_set_2::clear)
    .def("is_valid", &Polygon_set_2::is_valid)
    .def("insert", &insert0)
    .def("insert", &insert1)
    .def("insert", &insert_range0)
    .def("insert_polygons", &insert_range<Polygon_2>)
    .def("insert_polygons_with_holes", &insert_range<Polygon_with_holes_2>)
    .def<void (Polygon_set_2::*) ()>("complement", &Polygon_set_2::complement)
    .def("complement", &complement0)
    .def("intersection", &intersection<Polygon_set_2>)
    .def("intersection", &intersection<Polygon_2>)
    .def("intersection", &intersection<Polygon_with_holes_2>)
    .def<void (Polygon_set_2&, Polygon_set_2&, Polygon_set_2&)>("intersection", &intersection)
    .def("intersection", &intersection_range0)
    .def("intersection_polygons", &intersection_range<Polygon_2>)
    .def("intersection_polygons_with_holes", &intersection_range<Polygon_with_holes_2>)
    .def("join", &join<Polygon_set_2>)
    .def("join", &join<Polygon_2>)
    .def("join", &join<Polygon_with_holes_2>)
    .def<void (Polygon_set_2&, Polygon_set_2&, Polygon_set_2&)>("join", &join)
    .def("join", &join_range0)
    .def("join_polygons", &join_range<Polygon_2>)
    .def("join_polygons_with_holes", &join_range<Polygon_with_holes_2>)
    .def("difference", &difference<Polygon_set_2>)
    .def("difference", &difference<Polygon_2>)
    .def("difference", &difference<Polygon_with_holes_2>)
    .def<void (Polygon_set_2&, Polygon_set_2&, Polygon_set_2&)>("difference", &difference)
    .def("symmetric_difference", &symmetric_difference<Polygon_set_2>)
    .def("symmetric_difference", &symmetric_difference<Polygon_2>)
    .def("symmetric_difference", &symmetric_difference<Polygon_with_holes_2>)
    .def<void (Polygon_set_2&, Polygon_set_2&, Polygon_set_2&)>("symmetric_difference", &symmetric_difference)
    .def("symmetric_difference", &symmetric_difference_range0)
    .def("symmetric_difference_polygons", &symmetric_difference_range<Polygon_2>)
    .def("symmetric_difference_polygons_with_holes", &symmetric_difference_range<Polygon_with_holes_2>)
    .def("do_intersect", &do_intersect<Polygon_set_2>)
    .def("do_intersect", &do_intersect<Polygon_2>)
    .def("do_intersect", &do_intersect<Polygon_with_holes_2>)
    .def("do_intersect", &do_intersect_range0)
    .def("do_intersect_polygons", &do_intersect_range<Polygon_2>)
    .def("do_intersect_polygons_with_holes", &do_intersect_range<Polygon_with_holes_2>)
    .def("locate", &Polygon_set_2::locate)
    .def("oriented_side", &oriented_side<Point_2>)
    .def("oriented_side", &oriented_side<Polygon_set_2>)
    .def("oriented_side", &oriented_side<Polygon_2>)
    .def("oriented_side", &oriented_side<Polygon_with_holes_2>)
    ;
}
