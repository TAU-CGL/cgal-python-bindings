// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/minkowski_sum_2.h>
#include <CGAL/approximated_offset_2.h>
#include <CGAL/offset_polygon_2.h>

#include "CGALPY/minkowski_sum_2_types.hpp"
#include "CGALPY/polygon_partitioning_types.hpp"
#include "CGALPY/export_general_polygon_2.hpp"
#include "CGALPY/export_general_polygon_with_holes_2.hpp"
#include "CGALPY/append_iterator.hpp"

namespace py = nanobind;

namespace ms2 {

#if CGAL_VERSION_NR > 1050500000
// The binding for Minkowski sum by decomposition is temporarily commented out
// until a bug in CGAL is fixed. Otherwise, the code does not compile.

//////// By Decomposition ////////
// ===============================
// A utility to obtain an output iterator that dereferences a (simple) polygon
// target<T>::type select:
//   std::list<T>::iterator if T is Polygon_2
//   std::list<T::Polygon_2>::iterator if T is Polygon_with_holes_2
template <typename... Ts> using void_t = void;

template <typename T, typename = void> struct target
{ using type = typename std::list<T>::iterator; };

template <typename T>
struct target<T, void_t<typename T::Polygon_2>>
{ using type = typename std::list<typename T::Polygon_2>::iterator; };

// One Decomposition Strategy
template <typename T1, typename T2, typename T3>
void bind_mink_sum_decomp_one_strategy_3T(py::module_& m, ...) {}

template <typename T1, typename T2, typename T3,
          typename = decltype(T3()(T1(), typename target<T1>::type())),
          typename = decltype(T3()(T2(), typename target<T2>::type()))>
void bind_mink_sum_decomp_one_strategy_3T(py::module_& m, bool) {
  m.def("minkowski_sum_2",
        static_cast<Polygon_with_holes_2(*)(const T1&, const T2&, const T3&)>
          (&CGAL::minkowski_sum_2<Kernel, Point_2_container, T3>));
}

template <typename T1, typename T2>
void bind_mink_sum_decomp_one_strategy_2T(py::module_& m) {
  bind_mink_sum_decomp_one_strategy_3T<T1, T2, ms2::Polygon_nop_decomposition_2>(m, true);
  bind_mink_sum_decomp_one_strategy_3T<T1, T2, pp2::Polygon_vertical_decomposition_2>(m, true);
  bind_mink_sum_decomp_one_strategy_3T<T1, T2, pp2::Polygon_triangulation_decomposition_2>(m, true);
  bind_mink_sum_decomp_one_strategy_3T<T1, T2, pp2::Small_side_angle_bisector_decomposition_2>(m, true);
}

template <typename T>
void bind_mink_sum_decomp_one_strategy_1T(py::module_& m) {
  bind_mink_sum_decomp_one_strategy_2T<T, Polygon_2>(m);
  bind_mink_sum_decomp_one_strategy_2T<T, Polygon_with_holes_2>(m);
}

void bind_mink_sum_decomp_one_strategy(py::module_& m) {
  bind_mink_sum_decomp_one_strategy_1T<Polygon_2>(m);
  bind_mink_sum_decomp_one_strategy_1T<Polygon_with_holes_2>(m);
}

// Two Decomposition Staretegies
template <typename T1, typename T2, typename T3, typename T4>
void bind_mink_sum_decomp_two_strategies_pair(py::module_& m, ...) {}

template <typename T1, typename T2, typename T3, typename T4,
          typename = decltype(T3()(T1(), typename target<T1>::type())),
          typename = decltype(T4()(T2(), typename target<T1>::type()))>
void bind_mink_sum_decomp_two_strategies_pair(py::module_& m, bool) {
  m.def("minkowski_sum_2",
        static_cast<Polygon_with_holes_2(*)(const T1&, const T2&, const T3&, const T4&)>
        (&CGAL::minkowski_sum_2<Kernel, Point_2_container, T3, T4>));
}

template <typename P1, typename P2, typename T> void
bind_mink_sum_decomp_two_strategies_inner(py::module_& m, T) {}

template <typename P1, typename P2, typename T1, typename T2, typename... Ts>
void bind_mink_sum_decomp_two_strategies_inner(py::module_& m,
                                               T1 arg1, T2 arg2, Ts... args) {
  using PT1 = typename std::remove_pointer<T1>::type;
  using PT2 = typename std::remove_pointer<T2>::type;
  bind_mink_sum_decomp_two_strategies_pair<P1, P2, PT1, PT2>(m, true);
  bind_mink_sum_decomp_two_strategies_pair<P1, P2, PT2, PT1>(m, true);
  bind_mink_sum_decomp_two_strategies_inner<P1, P2>(m, arg1, args...);
}

template <typename P1, typename P2, typename T>
void bind_mink_sum_decomp_two_strategies_outer(py::module_& m, T arg) {
  using PT = typename std::remove_pointer<T>::type;
  bind_mink_sum_decomp_two_strategies_pair<P1, P2, PT, PT>(m, true);
}

template <typename P1, typename P2, typename T1, typename... Ts>
void bind_mink_sum_decomp_two_strategies_outer(py::module_& m,
                                               T1 arg, Ts... args) {
  bind_mink_sum_decomp_two_strategies_inner<P1, P2>(m, arg, args...);
  bind_mink_sum_decomp_two_strategies_outer<P1, P2>(m, args...);
  using PT1 = typename std::remove_pointer<T1>::type;
  bind_mink_sum_decomp_two_strategies_pair<P1, P2, PT1, PT1>(m, true);
}

template <typename P1, typename P2>
void bind_mink_sum_decomp_two_strategies_2T(py::module_& m) {
  ms2::Polygon_nop_decomposition_2* pnp(nullptr);
  pp2::Polygon_vertical_decomposition_2* pvd(nullptr);
  pp2::Polygon_triangulation_decomposition_2* ptd(nullptr);
  pp2::Small_side_angle_bisector_decomposition_2* ssabd(nullptr);

  bind_mink_sum_decomp_two_strategies_outer<P1, P2>(m, pnp, pvd, ptd, ssabd);
}

template <typename T>
void bind_mink_sum_decomp_two_strategies_1T(py::module_& m) {
  bind_mink_sum_decomp_two_strategies_2T<T, Polygon_2>(m);
  bind_mink_sum_decomp_two_strategies_2T<T, Polygon_with_holes_2>(m);
}

void bind_mink_sum_decomp_two_strategies(py::module_& m) {
  bind_mink_sum_decomp_two_strategies_1T<Polygon_2>(m);
  bind_mink_sum_decomp_two_strategies_1T<Polygon_with_holes_2>(m);
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

py::list approximated_inset_2(const Polygon_2& p, const FT& r, double eps) {
  py::list lst;
  CGAL::approximated_inset_2(p, r, eps, append_iterator(lst));
  return lst;
}

Conic_polygon_with_holes_2
offset_polygon_2(const Rat_polygon_2& pgn, const Rat_FT& r,
                 const Conic_traits& traits)
{ return CGAL::offset_polygon_2(pgn, r, traits); }

}

void export_minkowski_sum_2(py::module_& m) {
  using Pgn = ms2::Polygon_2;
  using Pwh = ms2::Polygon_with_holes_2;
  using CS_pgn = ms2::Circle_segment_polygon_2;
  using CS_pwh = ms2::Circle_segment_polygon_with_holes_2;
  using Conic_pgn = ms2::Conic_polygon_2;
  using Conic_pwh = ms2::Conic_polygon_with_holes_2;

#if CGAL_VERSION_NR > 1050500000
  // By decomposition
  // ================
  ms2::bind_mink_sum_decomp_one_strategy(m);
  ms2::bind_mink_sum_decomp_two_strategies(m);
#endif

  m.def("minkowski_sum_2", &ms2::minkowski_sum_2<Pgn, Pgn>);
  m.def("minkowski_sum_2", &ms2::minkowski_sum_2<Pgn, Pwh>);
  m.def("minkowski_sum_2", &ms2::minkowski_sum_2<Pwh, Pgn>);
  m.def("minkowski_sum_2", &ms2::minkowski_sum_2<Pwh, Pwh>);

  m.def("minkowski_sum_by_full_convolution_2",
        &ms2::minkowski_sum_by_full_convolution_2<Pgn, Pgn>);

  m.def("minkowski_sum_by_reduced_convolution_2",
        &ms2::minkowski_sum_by_reduced_convolution_2<Pgn, Pgn>);
  m.def("minkowski_sum_by_reduced_convolution_2",
        &ms2::minkowski_sum_by_reduced_convolution_2<Pgn, Pwh>);
  m.def("minkowski_sum_by_reduced_convolution_2",
        &ms2::minkowski_sum_by_reduced_convolution_2<Pwh, Pgn>);
  m.def("minkowski_sum_by_reduced_convolution_2",
        &ms2::minkowski_sum_by_reduced_convolution_2<Pwh, Pwh>);

  m.def("approximated_offset_2", &ms2::approximated_offset_2);
  m.def("approximated_offset_2", &ms2::approximated_offset_2_pwh);
  m.def("approximated_inset_2", &ms2::approximated_inset_2);

  m.def("offset_polygon_2", &ms2::offset_polygon_2);

  if (add_attr<CS_pgn>(m, "Circle_segment_polygon_2")) return;
  auto cs_pgn_c = py::class_<CS_pgn>(m, "Circle_segment_polygon_2");
  export_general_polygon_2<CS_pgn>(cs_pgn_c);

  if (add_attr<CS_pwh>(m, "Circle_segment_polygon_with_holes_2")) return;
  auto cs_pwh_c = py::class_<CS_pwh>(m, "Circle_segment_polygon_with_holes_2");
  export_general_polygon_with_holes_2<CS_pwh>(cs_pwh_c);

  if (add_attr<Conic_pgn>(m, "Conic_polygon_2")) return;
  auto conic_pgn_c = py::class_<Conic_pgn>(m, "Conic_polygon_2");
  export_general_polygon_2<Conic_pgn>(conic_pgn_c);

  if (add_attr<Conic_pwh>(m, "Conic_polygon_with_holes_2")) return;
  auto conic_pwh_c = py::class_<Conic_pwh>(m, "Conic_polygon_with_holes_2");
  export_general_polygon_with_holes_2<Conic_pwh>(conic_pwh_c);
}
