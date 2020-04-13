// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/config.hpp"
#ifdef CGALPY_MINKOWSKI_SUM_2_BINDINGS
#include "CGALPY/common.hpp"

#include <CGAL/minkowski_sum_2.h>
#include <CGAL/approximated_offset_2.h>

typedef typename CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_with_holes_2 General_polygon_with_holes_2;
typedef typename CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_2 General_polygon_2;


template <typename T1, typename T2>
Polygon_with_holes_2 minkowski_sum_2(T1& P, T2& Q)
{
  return CGAL::minkowski_sum_2(P, Q);
}

template <typename T1, typename T2>
Polygon_with_holes_2 minkowski_sum_by_full_convolution_2(T1& P, T2& Q)
{
  return CGAL::minkowski_sum_by_full_convolution_2(P, Q);
}

template <typename T1, typename T2>
Polygon_with_holes_2 minkowski_sum_by_reduced_convolution_2(T1& P, T2& Q)
{
  return CGAL::minkowski_sum_by_reduced_convolution_2(P, Q);
}

General_polygon_with_holes_2 approximated_offset_2(Polygon_2& p, FT& r, double eps)
{
  return CGAL::approximated_offset_2(p, r, eps);
}

General_polygon_with_holes_2 approximated_offset_2_pwh(Polygon_with_holes_2& pwh, FT& r, double eps)
{
  return CGAL::approximated_offset_2(pwh, r, eps);
}

void approximated_inset_2(Polygon_2& p, FT& r, double eps, boost::python::list& lst)
{
  auto v = std::vector<General_polygon_2>();
  CGAL::approximated_inset_2(p, r, eps, std::back_inserter(v));
  for (auto p : v)
  {
    lst.append(p);
  }
}

void export_minkowski_sum_2()
{
  using namespace boost::python;
  def("minkowski_sum_2", &minkowski_sum_2<Polygon_2, Polygon_2>);
  def("minkowski_sum_2", &minkowski_sum_2<Polygon_2, Polygon_with_holes_2>);
  def("minkowski_sum_2", &minkowski_sum_2<Polygon_with_holes_2, Polygon_2>);
  def("minkowski_sum_2", &minkowski_sum_2<Polygon_with_holes_2, Polygon_with_holes_2>);

  def("minkowski_sum_by_full_convolution_2", &minkowski_sum_by_full_convolution_2<Polygon_2, Polygon_2>);

  def("minkowski_sum_by_reduced_convolution_2", &minkowski_sum_by_reduced_convolution_2<Polygon_2, Polygon_2>);
  def("minkowski_sum_by_reduced_convolution_2", &minkowski_sum_by_reduced_convolution_2<Polygon_2, Polygon_with_holes_2>);
  def("minkowski_sum_by_reduced_convolution_2", &minkowski_sum_by_reduced_convolution_2<Polygon_with_holes_2, Polygon_2>);
  def("minkowski_sum_by_reduced_convolution_2", &minkowski_sum_by_reduced_convolution_2<Polygon_with_holes_2, Polygon_with_holes_2>);

  def("approximated_offset_2", &approximated_offset_2);
  def("approximated_offset_2", &approximated_offset_2_pwh);
  //def("approximated_inset_2", &approximated_inset_2);
}

#endif
