// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_GENERAL_POLYGON_2_HPP
#define CGALPY_EXPORT_GENERAL_POLYGON_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/make_iterator.hpp"

#include <CGAL/Gps_circle_segment_traits_2.h>

// The following might be needed in the future when extraction support is added
// to specific curves types. At that point we can add support for extraction
// of general polygons with holes bounded by curves of the specific types.

// #include <CGAL/CORE_algebraic_number_traits.h>
// #include <CGAL/Arr_conic_traits_2.h>
// #include <CGAL/Arr_Bezier_curve_traits_2.h>

namespace py = nanobind;

namespace bso2 {

// Initialize a general polygon from a list of x-monotone curves.
template <typename GeneralPolygon_2>
void init_polygon_2(GeneralPolygon_2* pgn, py::list& lst) {
  using Gpgn = GeneralPolygon_2;
  using Xcv = typename Gpgn::X_monotone_curve_2;
  auto begin = stl_forward_iterator<Xcv>(lst);
  auto end = stl_forward_iterator<Xcv>(lst, false);
  new (pgn) Gpgn(begin, end);   // placement new
}

}

// Export the attributes of General_polygon_2.
template <typename GeneralPolygon_2>
inline void
export_general_polygon_2(py::class_<GeneralPolygon_2>& pgn_c) {
  using Gpgn = GeneralPolygon_2;
  pgn_c.def(py::init<>())
    .def(py::init<const Gpgn&>())
    .def("__init__", &bso2::init_polygon_2<Gpgn>)
    .def("push_back", &Gpgn::push_back)
    .def("orientation", &Gpgn::orientation)
    .def("is_empty", &Gpgn::is_empty)
    .def("size", &Gpgn::size)
    .def("clear", &Gpgn::clear)
    .def("reverse_orientation", &Gpgn::reverse_orientation)
    ;

  // Support limited traits
  using Cs_pgn = CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_2;
  if constexpr (std::is_same<Gpgn, Cs_pgn>::value)
    pgn_c.def("bbox", &Gpgn::bbox);

  using Cci = typename Gpgn::Curve_const_iterator;
  add_iterator<Cci, Cci>("Curve_iterator", pgn_c);
  pgn_c.def("curves",
            [] (const Gpgn& pgn)
            { return make_iterator(pgn.curves_begin(), pgn.curves_end()); },
            py::keep_alive<0, 1>());

  add_insertion(pgn_c, "__str__");
  add_insertion(pgn_c, "__repr__");

  // Support extraction only for geometry traits that do support extraction:
  // using Cs_pgn = CGAL::Gps_circle_segment_traits_2<Kernel>::Polygon_2;
  // using Nt_traits = CGAL::CORE_algebraic_number_traits;
  // using Rational = Nt_traits::Rational;
  // using Algebraic = Nt_traits::Algebraic;
  // using Rat_kernel = CGAL::Cartesian<Rational>;
  // using Alg_kernel = CGAL::Cartesian<Algebraic>;
  // using Conic_traits = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  // using Conic_pgn = CGAL::Gps_traits_2<Conic_traits>::Polygon_2;
  // using Bezier_traits = CGAL::Arr_Bezier_curve_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  // using Bezier_pgn = CGAL::Gps_traits_2<Bezier_traits>::Polygon_2;
  // None of the traits above support extraction....
  // if constexpr ((std::is_same<Gpgn, Cs_pgn>::value) ||
  //               (std::is_same<Gpgn, Conic_pgn>::value)
  //               (std::is_same<Gpgn, Bezier_pgn>::value))
  //   add_extraction(pgn_c);
}

/*! Capture the call to export a Polygon_2<> and ensure that it is not invoked.
 */
template <typename Kernel, typename Container>
inline void export_general_polygon_2
(py::class_<CGAL::Polygon_2<Kernel, Container>>& /* pgn_c */) {
  throw std::runtime_error("Attempting to export Polygon_2 as General_polygon_2!");
}

#endif
