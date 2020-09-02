// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#ifdef CGALPY_BOOLEAN_SET_OPERATIONS_BINDINGS
#include "CGALPY/common.hpp"

#include <CGAL/connect_holes.h>
#include <CGAL/Boolean_set_operations_2.h>

void complement0(Polygon_2& pgn, Polygon_with_holes_2& res)
{
  CGAL::complement(pgn, res);
}

void complement1(Polygon_with_holes_2& pgn, boost::python::list& lst)
{
  auto v = std::vector<Polygon_with_holes_2>();
  CGAL::complement(pgn, std::back_inserter(v));
  for (auto p : v) { lst.append(p); }
}

template <typename T0, typename T1>
bool do_intersect(T0& p0, T1& p1)
{
  return CGAL::do_intersect(p0, p1);
}

template<typename T0, typename T1>
bool do_intersect_range(boost::python::list& polygon_lst,
                        boost::python::list& pwh_lst)
{
  auto begin0 = boost::python::stl_input_iterator<T0>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<T0>();
  auto begin1 = boost::python::stl_input_iterator<T1>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  return do_intersect(v0.begin(), v0.end(), v1.begin(), v1.end());
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

template<typename T0, typename T1>
void intersection_range(boost::python::list& polygon_lst, boost::python::list& pwh_lst, boost::python::list& lst)
{
  auto begin0 = boost::python::stl_input_iterator<T0>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<T0>();
  auto begin1 = boost::python::stl_input_iterator<T1>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::intersection(v0.begin(), v0.end(), v1.begin(), v1.end(), std::back_inserter(res));
  for (auto p : res) lst.append(p);
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
bool join_linear(T0& p0, T1& p1, Polygon_with_holes_2& pwh)
{
  return CGAL::join(p0, p1, pwh);
}

template<typename T0, typename T1>
void join_range(boost::python::list& polygon_lst, boost::python::list& pwh_lst,
                boost::python::list& lst)
{
  auto begin0 = boost::python::stl_input_iterator<T0>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<T0>();
  auto begin1 = boost::python::stl_input_iterator<T1>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::join(v0.begin(), v0.end(), v1.begin(), v1.end(),
             std::back_inserter(res));
  for (auto p : res) lst.append(p);
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

template<typename T0, typename T1>
void symmetric_difference_range(boost::python::list& polygon_lst,
                                boost::python::list& pwh_lst,
                                boost::python::list& lst)
{
  auto begin0 = boost::python::stl_input_iterator<T0>(polygon_lst);
  auto end0 = boost::python::stl_input_iterator<T0>();
  auto begin1 = boost::python::stl_input_iterator<T1>(pwh_lst);
  auto end1 = boost::python::stl_input_iterator<T1>();
  auto v0 = std::vector<T0>(begin0, end0);
  auto v1 = std::vector<T1>(begin1, end1);
  auto res = std::vector<T1>();
  CGAL::symmetric_difference(v0.begin(), v0.end(), v1.begin(), v1.end(),
                             std::back_inserter(res));
  for (auto p : res) lst.append(p);
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


void export_boolean_set_operations_2()
{
  using namespace boost::python;
  def("complement", complement0);
  def("complement", complement1);
  def("do_intersect", &do_intersect<Polygon_2, Polygon_2>);
  def("do_intersect", &do_intersect<Polygon_2, Polygon_with_holes_2>);
  def("do_intersect", &do_intersect<Polygon_with_holes_2, Polygon_2>);
  def("do_intersect", &do_intersect<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("do_intersect", &do_intersect_range<Polygon_2, Polygon_with_holes_2>);
  def("intersection", &intersection_linear<Polygon_2, Polygon_2>);
  def("intersection", &intersection_linear<Polygon_2, Polygon_with_holes_2>);
  def("intersection", &intersection_linear<Polygon_with_holes_2, Polygon_2>);
  def("intersection", &intersection_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("intersection", &intersection_range<Polygon_2, Polygon_with_holes_2>);
  def("difference", &difference_linear<Polygon_2, Polygon_2>);
  def("difference", &difference_linear<Polygon_2, Polygon_with_holes_2>);
  def("difference", &difference_linear<Polygon_with_holes_2, Polygon_2>);
  def("difference", &difference_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("join", &join_linear<Polygon_2, Polygon_2>);
  def("join", &join_linear<Polygon_with_holes_2, Polygon_2>);
  def("join", &join_linear<Polygon_2, Polygon_with_holes_2>);
  def("join", &join_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("join", &join_range<Polygon_2, Polygon_with_holes_2>);
  def("symmetric_difference", &symmetric_difference_linear<Polygon_2, Polygon_2>);
  def("symmetric_difference", &symmetric_difference_linear<Polygon_2, Polygon_with_holes_2>);
  def("symmetric_difference", &symmetric_difference_linear<Polygon_with_holes_2, Polygon_2>);
  def("symmetric_difference", &symmetric_difference_linear<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("symmetric_difference", &symmetric_difference_range<Polygon_2, Polygon_with_holes_2>);
  def("oriented_side", &oriented_side<Polygon_2, Polygon_2>);
  def("oriented_side", &oriented_side<Polygon_2, Polygon_with_holes_2>);
  def("oriented_side", &oriented_side<Polygon_with_holes_2, Polygon_2>);
  def("oriented_side", &oriented_side<Polygon_with_holes_2, Polygon_with_holes_2>);
  def("connect_holes", &connect_holes);


}
#endif
