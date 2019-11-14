// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "config.hpp"
#include "common.hpp"
#include <CGAL/partition_2.h>
#include <CGAL/Partition_traits_2.h>

void approx_convex_partition_2(Polygon_2& p, boost::python::list& res)
{
  auto v = std::vector<Polygon_2>();
  CGAL::approx_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v)
  {
    res.append(c_polygon);
  }
}

void greene_approx_convex_partition_2(Polygon_2& p, boost::python::list& res)
{
  auto v = std::vector<Polygon_2>();
  CGAL::greene_approx_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v)
  {
    res.append(c_polygon);
  }
}

void optimal_convex_partition_2(Polygon_2& p, boost::python::list& res)
{
  auto v = std::vector<Polygon_2>();
  CGAL::optimal_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v)
  {
    res.append(c_polygon);
  }
}

void y_monotone_partition_2(Polygon_2& p, boost::python::list& res)
{
  auto v = std::vector<Polygon_2>();
  CGAL::y_monotone_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v)
  {
    res.append(c_polygon);
  }
}

bool partition_is_valid_2(Polygon_2& p, boost::python::list& polygon_lst)
{
  auto begin = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = boost::python::stl_input_iterator<Polygon_2>();
  auto v = std::vector<Polygon_2>(begin, end);
  return CGAL::partition_is_valid_2(p.vertices_begin(), p.vertices_end(), v.begin(), v.end());
}

bool convex_partition_is_valid_2(Polygon_2& p, boost::python::list& polygon_lst)
{
  auto begin = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = boost::python::stl_input_iterator<Polygon_2>();
  auto v = std::vector<Polygon_2>(begin, end);
  return CGAL::convex_partition_is_valid_2(p.vertices_begin(), p.vertices_end(), v.begin(), v.end());
}

bool y_monotone_partition_is_valid_2(Polygon_2& p, boost::python::list& polygon_lst)
{
  auto begin = boost::python::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = boost::python::stl_input_iterator<Polygon_2>();
  auto v = std::vector<Polygon_2>(begin, end);
  return CGAL::y_monotone_partition_is_valid_2(p.vertices_begin(), p.vertices_end(), v.begin(), v.end());
}

bool is_y_monotone_2(Polygon_2& p)
{
  return CGAL::is_y_monotone_2(p.vertices_begin(), p.vertices_end());
}

bool is_convex_2(Polygon_2& p)
{
  return CGAL::is_convex_2(p.vertices_begin(), p.vertices_end());
}


void export_Polygon_partition_2()
{
  using namespace boost::python;
  def("approx_convex_partition_2", &approx_convex_partition_2);
  def("greene_approx_convex_partition_2", &greene_approx_convex_partition_2);
  def("optimal_convex_partition_2", &optimal_convex_partition_2);
  def("y_monotone_partition_2", &y_monotone_partition_2);
  def("partition_is_valid_2", &partition_is_valid_2);
  def("convex_partition_is_valid_2", &convex_partition_is_valid_2);
  def("y_monotone_partition_is_valid_2", &y_monotone_partition_is_valid_2);
  def("is_y_monotone_2", &is_y_monotone_2);
  def("is_convex_2", &is_convex_2);
}