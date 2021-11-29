// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <boost/python.hpp>

#include <CGAL/minkowski_sum_2.h>
#include <CGAL/approximated_offset_2.h>

#include "CGALPY/minkowski_sum_2_types.hpp"
#include "CGALPY/polygon_partitioning_types.hpp"
#include "CGALPY/export_general_polygon_2.hpp"
#include "CGALPY/export_general_polygon_with_holes_2.hpp"

namespace bp = boost::python;

namespace ms2 {

#if 0
// The binding for Minkowski sum by decomposition is temporarily commented out
// until a bug in CGAL is fixed. Otherwise, the code does not compile.

//////// By Decomposition ////////
// ===============================
// A utility to obtain an output iterator that dereferences a (simple) polygon
// target<T>::type select:
//   std::list<T>::iterator if T is Polygon_2
//   std::list<T::Polygon_2>::iterator if T is Polygon_with_holes_2
template <typename T> class has { typedef void type; };

template <typename T, typename = void> struct target {
  typedef typename std::list<T>::iterator type;
};

template <typename T>
struct target<T, typename has<typename T::Polygon_2>::type> {
  typedef typename std::list<typename T::Polygon_2>::iterator type;
};

// One Decomposition Strategy
template <typename T1, typename T2, typename T3>
void bind_mink_sum_decomp_one_strategy_3T(...) {}

static int cnt1(0);
template <typename T1, typename T2, typename T3,
          typename = decltype(T3()(T1(), typename target<T1>::type())),
          typename = decltype(T3()(T2(), typename target<T2>::type()))>
void bind_mink_sum_decomp_one_strategy_3T(bool) {
  bp::def<Polygon_with_holes_2(const T1&, const T2&, const T3&)>
    ("minkowski_sum_2", &CGAL::minkowski_sum_2<Kernel, Point_2_container, T3>);
  std::cout << "NUM1: " << ++cnt1 << std::endl;
}

template <typename T1, typename T2>
void bind_mink_sum_decomp_one_strategy_2T() {
  bind_mink_sum_decomp_one_strategy_3T<T1, T2, ms2::Polygon_nop_decomposition_2>(true);
  bind_mink_sum_decomp_one_strategy_3T<T1, T2, pp2::Polygon_vertical_decomposition_2>(true);
  bind_mink_sum_decomp_one_strategy_3T<T1, T2, pp2::Polygon_triangulation_decomposition_2>(true);
  bind_mink_sum_decomp_one_strategy_3T<T1, T2, pp2::Small_side_angle_bisector_decomposition_2>(true);
}

template <typename T>
void bind_mink_sum_decomp_one_strategy_1T() {
  bind_mink_sum_decomp_one_strategy_2T<T, Polygon_2>();
  bind_mink_sum_decomp_one_strategy_2T<T, Polygon_with_holes_2>();
}

void bind_mink_sum_decomp_one_strategy() {
  bind_mink_sum_decomp_one_strategy_1T<Polygon_2>();
  bind_mink_sum_decomp_one_strategy_1T<Polygon_with_holes_2>();
}

// Two Decomposition Staretegies
template <typename T1, typename T2, typename T3, typename T4>
void bind_mink_sum_decomp_two_strategies_4T(...) {}

static int cnt2(0);
template <typename T1, typename T2, typename T3, typename T4,
          typename = decltype(T3()(T1(), typename target<T1>::type())),
          typename = decltype(T4()(T2(), typename target<T1>::type()))>
void bind_mink_sum_decomp_two_strategies_4T(bool) {
  bp::def<Polygon_with_holes_2(const T1&, const T2&, const T3&, const T4&)>
    ("minkowski_sum_2", &CGAL::minkowski_sum_2<Kernel, Point_2_container, T3, T4>);
  std::cout << "NUM2: " << ++cnt2 << std::endl;
}

template <typename T1, typename T2, typename T3>
void bind_mink_sum_decomp_two_strategies_3T() {
  bind_mink_sum_decomp_two_strategies_4T<T1, T2, T3, ms2::Polygon_nop_decomposition_2>(true);
  bind_mink_sum_decomp_two_strategies_4T<T1, T2, T3, pp2::Polygon_vertical_decomposition_2>(true);
  bind_mink_sum_decomp_two_strategies_4T<T1, T2, T3, pp2::Polygon_triangulation_decomposition_2>(true);
  bind_mink_sum_decomp_two_strategies_4T<T1, T2, T3, pp2::Small_side_angle_bisector_decomposition_2>(true);
}

template <typename T1, typename T2>
void bind_mink_sum_decomp_two_strategies_2T() {
  bind_mink_sum_decomp_two_strategies_3T<T1, T2, ms2::Polygon_nop_decomposition_2>();
  bind_mink_sum_decomp_two_strategies_3T<T1, T2, pp2::Polygon_vertical_decomposition_2>();
  bind_mink_sum_decomp_two_strategies_3T<T1, T2, pp2::Polygon_triangulation_decomposition_2>();
  bind_mink_sum_decomp_two_strategies_3T<T1, T2, pp2::Small_side_angle_bisector_decomposition_2>();
}

template <typename T>
void bind_mink_sum_decomp_two_strategies_1T() {
  bind_mink_sum_decomp_two_strategies_2T<T, Polygon_2>();
  bind_mink_sum_decomp_two_strategies_2T<T, Polygon_with_holes_2>();
}

void bind_mink_sum_decomp_two_strategies() {
  bind_mink_sum_decomp_two_strategies_1T<Polygon_2>();
  bind_mink_sum_decomp_two_strategies_1T<Polygon_with_holes_2>();
}

#endif

// Default Minkowski Sum
template <typename T1, typename T2>
Polygon_with_holes_2 minkowski_sum_2(const T1& P, const T2& Q)
{ return CGAL::minkowski_sum_2(P, Q); }

template <typename T1, typename T2>
Polygon_with_holes_2
minkowski_sum_by_full_convolution_2(const T1& P, const T2& Q)
{ return CGAL::minkowski_sum_by_full_convolution_2(P, Q); }

template <typename T1, typename T2>
Polygon_with_holes_2
minkowski_sum_by_reduced_convolution_2(const T1& P, const T2& Q)
{ return CGAL::minkowski_sum_by_reduced_convolution_2(P, Q); }

Circle_segment_polygon_with_holes_2
approximated_offset_2(const Polygon_2& p, const FT& r, double eps)
{ return CGAL::approximated_offset_2(p, r, eps); }

Circle_segment_polygon_with_holes_2
approximated_offset_2_pwh(const Polygon_with_holes_2& pwh,
                          const FT& r, double eps)
{ return CGAL::approximated_offset_2(pwh, r, eps); }

void approximated_inset_2(const Polygon_2& p, const FT& r,
                          double eps, bp::list& lst) {
  auto v = std::vector<Circle_segment_polygon_2>();
  CGAL::approximated_inset_2(p, r, eps, std::back_inserter(v));
  for (auto p : v) lst.append(p);
}

}

void export_minkowski_sum_2() {
  typedef ms2::Polygon_2                                Pgn;
  typedef ms2::Polygon_with_holes_2                     Pwh;
  typedef ms2::Circle_segment_polygon_2                 CS_pgn;
  typedef ms2::Circle_segment_polygon_with_holes_2      CS_pwh;

  bp::scope ms_scope = bp::scope();

#if 0
  // By decomposition
  // ================
  ms2::bind_mink_sum_decomp_one_strategy();
  ms2::bind_mink_sum_decomp_two_strategies();
#endif

  bp::def("minkowski_sum_2", &ms2::minkowski_sum_2<Pgn, Pgn>);
  bp::def("minkowski_sum_2", &ms2::minkowski_sum_2<Pgn, Pwh>);
  bp::def("minkowski_sum_2", &ms2::minkowski_sum_2<Pwh, Pgn>);
  bp::def("minkowski_sum_2", &ms2::minkowski_sum_2<Pwh, Pwh>);

  bp::def("minkowski_sum_by_full_convolution_2",
          &ms2::minkowski_sum_by_full_convolution_2<Pgn, Pgn>);

  bp::def("minkowski_sum_by_reduced_convolution_2",
          &ms2::minkowski_sum_by_reduced_convolution_2<Pgn, Pgn>);
  bp::def("minkowski_sum_by_reduced_convolution_2",
          &ms2::minkowski_sum_by_reduced_convolution_2<Pgn, Pwh>);
  bp::def("minkowski_sum_by_reduced_convolution_2",
          &ms2::minkowski_sum_by_reduced_convolution_2<Pwh, Pgn>);
  bp::def("minkowski_sum_by_reduced_convolution_2",
          &ms2::minkowski_sum_by_reduced_convolution_2<Pwh, Pwh>);

  bp::def("approximated_offset_2", &ms2::approximated_offset_2);
  bp::def("approximated_offset_2", &ms2::approximated_offset_2_pwh);
  bp::def("approximated_inset_2", &ms2::approximated_inset_2);

  static const char pgn[] = "Circle_segment_polygon_2";
  bp::class_<CS_pgn>* co_pgn(nullptr);
  export_general_polygon_2<CS_pgn, pgn>(ms_scope, co_pgn);

  static const char pwh[] = "Circle_segment_polygon_with_holes_2";
  bp::class_<CS_pwh>* co_pwh(nullptr);
  export_general_polygon_with_holes_2<CS_pwh, pwh>(ms_scope, co_pwh);
}
