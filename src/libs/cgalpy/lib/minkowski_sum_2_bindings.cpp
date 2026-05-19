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
#include "CGALPY/cartesian_product.hpp"

namespace py = nanobind;

// Docstrings
namespace {
  const char* MINKOWSKI_SUM_2_DOC = R"pbdoc(
Compute the Minkowski sum of two polygons.
)pbdoc";

  const char* MINKOWSKI_SUM_BY_FULL_CONVOLUTION_2_DOC = R"pbdoc(
Compute the Minkowski sum of two polygons using the full convolution method.
)pbdoc";

  const char* MINKOWSKI_SUM_BY_REDUCED_CONVOLUTION_2_DOC = R"pbdoc(
Compute the Minkowski sum of two polygons using the reduced convolution method.
)pbdoc";

  const char* APPROXIMATED_OFFSET_2_DOC = R"pbdoc(
Compute an approximated offset of a polygon by a given radius.
)pbdoc";

  const char* APPROXIMATED_INSET_2_DOC = R"pbdoc(
Compute an approximated inset of a polygon by a given radius.
)pbdoc";

  const char* OFFSET_POLYGON_2_DOC = R"pbdoc(
Compute the exact offset of a rational polygon by a given radius.
)pbdoc";

  const char* INSET_POLYGON_2_DOC = R"pbdoc(
Compute the exact inset of a rational polygon by a given radius.
)pbdoc";
}

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
void bind_mink_sum_decomp_one_strategy(py::module_& m, ...) {}

template <typename T1, typename T2, typename T3,
          typename = decltype(T3()(T1(), typename target<T1>::type())),
          typename = decltype(T3()(T2(), typename target<T2>::type()))>
void bind_mink_sum_decomp_one_strategy(py::module_& m, bool) {
  m.def("minkowski_sum_2",
        static_cast<Polygon_with_holes_2(*)(const T1&, const T2&, const T3&)>
          (&CGAL::minkowski_sum_2<Kernel, Point_2_container, T3>),
        MINKOWSKI_SUM_2_DOC,
        py::arg("p"), py::arg("q"), py::arg("strategy"));
}

// Two Decomposition Staretegies
template <typename T1, typename T2, typename T3, typename T4>
void bind_mink_sum_decomp_two_strategies(py::module_& m, ...) {}

template <typename T1, typename T2, typename T3, typename T4,
          typename = decltype(T3()(T1(), typename target<T1>::type())),
          typename = decltype(T4()(T2(), typename target<T1>::type()))>
void bind_mink_sum_decomp_two_strategies(py::module_& m, bool) {
  m.def("minkowski_sum_2",
        static_cast<Polygon_with_holes_2(*)(const T1&, const T2&,
                                            const T3&, const T4&)>
        (&CGAL::minkowski_sum_2<Kernel, Point_2_container, T3, T4>),
        MINKOWSKI_SUM_2_DOC,
        py::arg("p"), py::arg("q"), py::arg("strategy1"), py::arg("strategy2"));
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

//
Conic_polygon_with_holes_2
offset_polygon_2(const Rat_polygon_2& pgn, const Rat_FT& r,
                 const Conic_traits& traits)
{ return CGAL::offset_polygon_2(pgn, r, traits); }

//
py::list inset_polygon_2(const Rat_polygon_2& pgn, const Rat_FT& r,
                         const Conic_traits& traits) {
  py::list lst;
  auto op = [&] (const Conic_polygon_2& pwh) mutable { lst.append(pwh); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::inset_polygon_2(pgn, r, traits, it);
  return lst;
}

//
template <typename Arg, typename ... Types> struct Wrapper_one_strategy {
  void operator()(Arg& arg) {
    bind_mink_sum_decomp_one_strategy<Types...>(arg, true);
  }
};

//
template <typename Arg, typename ... Types> struct Wrapper_two_strategies {
  void operator()(Arg& arg) {
    bind_mink_sum_decomp_two_strategies<Types...>(arg, true);
  }
};

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

  using Pnp = ms2::Polygon_nop_decomposition_2;
  using Pvd = pp2::Polygon_vertical_decomposition_2;
  using Ptd = pp2::Polygon_triangulation_decomposition_2;
  using Ssabd = pp2::Small_side_angle_bisector_decomposition_2;

  CGALPY::Type_list<Pgn, Pwh> polygon_types;
  CGALPY::Type_list<Pnp, Pvd, Ptd, Ssabd> strategy_types;

  CGALPY::cartesian_product<ms2::Wrapper_one_strategy>(m, polygon_types,
                                                       polygon_types,
                                                       strategy_types);
  CGALPY::cartesian_product<ms2::Wrapper_two_strategies>(m, polygon_types,
                                                         polygon_types,
                                                         strategy_types,
                                                         strategy_types);
#endif

  m.def("minkowski_sum_2", &ms2::minkowski_sum_2<Pgn, Pgn>, MINKOWSKI_SUM_2_DOC, py::arg("p"), py::arg("q"));
  m.def("minkowski_sum_2", &ms2::minkowski_sum_2<Pgn, Pwh>, MINKOWSKI_SUM_2_DOC, py::arg("p"), py::arg("q"));
  m.def("minkowski_sum_2", &ms2::minkowski_sum_2<Pwh, Pgn>, MINKOWSKI_SUM_2_DOC, py::arg("p"), py::arg("q"));
  m.def("minkowski_sum_2", &ms2::minkowski_sum_2<Pwh, Pwh>, MINKOWSKI_SUM_2_DOC, py::arg("p"), py::arg("q"));

  m.def("minkowski_sum_by_full_convolution_2",
        &ms2::minkowski_sum_by_full_convolution_2<Pgn, Pgn>,
        MINKOWSKI_SUM_BY_FULL_CONVOLUTION_2_DOC,
        py::arg("p"), py::arg("q"));

  m.def("minkowski_sum_by_reduced_convolution_2",
        &ms2::minkowski_sum_by_reduced_convolution_2<Pgn, Pgn>,
        MINKOWSKI_SUM_BY_REDUCED_CONVOLUTION_2_DOC,
        py::arg("p"), py::arg("q"));
  m.def("minkowski_sum_by_reduced_convolution_2",
        &ms2::minkowski_sum_by_reduced_convolution_2<Pgn, Pwh>,
        MINKOWSKI_SUM_BY_REDUCED_CONVOLUTION_2_DOC,
        py::arg("p"), py::arg("q"));
  m.def("minkowski_sum_by_reduced_convolution_2",
        &ms2::minkowski_sum_by_reduced_convolution_2<Pwh, Pgn>,
        MINKOWSKI_SUM_BY_REDUCED_CONVOLUTION_2_DOC,
        py::arg("p"), py::arg("q"));
  m.def("minkowski_sum_by_reduced_convolution_2",
        &ms2::minkowski_sum_by_reduced_convolution_2<Pwh, Pwh>,
        MINKOWSKI_SUM_BY_REDUCED_CONVOLUTION_2_DOC,
        py::arg("p"), py::arg("q"));

  m.def("approximated_offset_2", &ms2::approximated_offset_2,
        APPROXIMATED_OFFSET_2_DOC,
        py::arg("p"), py::arg("r"), py::arg("eps"));
  m.def("approximated_offset_2", &ms2::approximated_offset_2_pwh,
        APPROXIMATED_OFFSET_2_DOC,
        py::arg("pwh"), py::arg("r"), py::arg("eps"));
  m.def("approximated_inset_2", &ms2::approximated_inset_2,
        APPROXIMATED_INSET_2_DOC,
        py::arg("p"), py::arg("r"), py::arg("eps"));

  m.def("offset_polygon_2", &ms2::offset_polygon_2,
        OFFSET_POLYGON_2_DOC,
        py::arg("pgn"), py::arg("r"), py::arg("traits"));
  m.def("inset_polygon_2", &ms2::inset_polygon_2,
        INSET_POLYGON_2_DOC,
        py::arg("pgn"), py::arg("r"), py::arg("traits"));

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
