// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#ifdef CGALPY_POLYGON_PARTITIONING_BINDINGS
#include "CGALPY/common.hpp"

#include <CGAL/partition_2.h>
#include <CGAL/Polygon_vertical_decomposition_2.h>
#include <CGAL/Polygon_triangulation_decomposition_2.h>
#include <CGAL/Small_side_angle_bisector_decomposition_2.h>

typedef typename CGAL::Polygon_vertical_decomposition_2<Kernel, Point_2_container> Polygon_vertical_decomposition_2;
typedef typename CGAL::Polygon_triangulation_decomposition_2<Kernel, Point_2_container> Polygon_triangulation_decomposition_2;
typedef typename CGAL::Small_side_angle_bisector_decomposition_2<Kernel, Point_2_container> Small_side_angle_bisector_decomposition_2;


void approx_convex_partition_2(Polygon_2& p, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  CGAL::approx_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v)
  {
    res.append(c_polygon);
  }
}

void greene_approx_convex_partition_2(Polygon_2& p, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  CGAL::greene_approx_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v)
  {
    res.append(c_polygon);
  }
}

void optimal_convex_partition_2(Polygon_2& p, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  CGAL::optimal_convex_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto c_polygon : v)
  {
    res.append(c_polygon);
  }
}

void y_monotone_partition_2(Polygon_2& p, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  CGAL::y_monotone_partition_2(p.vertices_begin(), p.vertices_end(), std::back_inserter(v));
  for (auto ym_polygon : v)
  {
    res.append(ym_polygon);
  }
}

bool partition_is_valid_2(Polygon_2& p, bp::list& polygon_lst)
{
  auto begin = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = bp::stl_input_iterator<Polygon_2>();
  auto v = std::vector<Polygon_2>(begin, end);
  return CGAL::partition_is_valid_2(p.vertices_begin(), p.vertices_end(), v.begin(), v.end());
}

bool convex_partition_is_valid_2(Polygon_2& p, bp::list& polygon_lst)
{
  auto begin = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = bp::stl_input_iterator<Polygon_2>();
  auto v = std::vector<Polygon_2>(begin, end);
  return CGAL::convex_partition_is_valid_2(p.vertices_begin(), p.vertices_end(), v.begin(), v.end());
}

bool y_monotone_partition_is_valid_2(Polygon_2& p, bp::list& polygon_lst)
{
  auto begin = bp::stl_input_iterator<Polygon_2>(polygon_lst);
  auto end = bp::stl_input_iterator<Polygon_2>();
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

template<typename T>
void polygon_vertical_decomposition_2(Polygon_vertical_decomposition_2& pvd, T& polygon, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  pvd(polygon, std::back_inserter(v));
  for (auto trapezoid : v)
  {
    res.append(trapezoid);
  }
}


template<typename T>
void polygon_triangulation_decomposition_2(Polygon_triangulation_decomposition_2& ptd, T& polygon, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  ptd(polygon, std::back_inserter(v));
  for (auto triangle : v)
  {
    res.append(triangle);
  }
}


void small_side_angle_bisector_decomposition_2(Small_side_angle_bisector_decomposition_2& ssabd, Polygon_2& polygon, bp::list& res)
{
  auto v = std::vector<Polygon_2>();
  ssabd(polygon, std::back_inserter(v));
  for (auto c_polygon : v)
  {
    res.append(c_polygon);
  }
}

void export_polygon_partition_2()
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

  class_<Polygon_vertical_decomposition_2>("Polygon_vertical_decomposition")
    .def(init<>())
    .def("__call__", &polygon_vertical_decomposition_2<Polygon_2>)
    .def("__call__", &polygon_vertical_decomposition_2<Polygon_with_holes_2>)
    ;

  class_<Polygon_triangulation_decomposition_2>("Polygon_triangulation_decomposition")
    .def(init<>())
    .def("__call__", &polygon_triangulation_decomposition_2<Polygon_2>)
    .def("__call__", &polygon_triangulation_decomposition_2<Polygon_with_holes_2>)
    ;

  class_<Small_side_angle_bisector_decomposition_2>("Small_side_angle_bisector_decomposition")
    .def(init<>())
    .def("__call__", &small_side_angle_bisector_decomposition_2)
    ;
}
#endif
