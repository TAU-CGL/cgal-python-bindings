// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_GENERAL_POLYGON_WITH_HOLES_2_HPP
#define CGALPY_EXPORT_GENERAL_POLYGON_WITH_HOLES_2_HPP

#include <type_traits>

#include <nanobind/nanobind.h>

#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/add_extraction.hpp"

// The following might be needed in the future when extraction support is added
// to specific curves types. At that point we can add support for extraction
// of general polygons with holes bounded by curves of the specific types.

// #include <CGAL/Gps_circle_segment_traits_2.h>
// #include <CGAL/CORE_algebraic_number_traits.h>
// #include <CGAL/Arr_conic_traits_2.h>
// #include <CGAL/Arr_Bezier_curve_traits_2.h>

namespace py = nanobind;

// Initialize a general polygon with hole from the outer boundary and a list
// of holes.
template <typename GeneralPolygonWithHoles_2>
void init_polygon_with_holes_2(GeneralPolygonWithHoles_2* pwh,
                               typename
                               GeneralPolygonWithHoles_2::General_polygon_2& p,
                               py::list& lst) {
  using Gpwh = GeneralPolygonWithHoles_2;
  using Gpgn = typename Gpwh::General_polygon_2;
  auto begin = stl_forward_iterator<Gpgn>(lst);
  auto end = stl_forward_iterator<Gpgn>(lst, false);
  new (pwh) Gpwh(p, begin, end);        // placement new
}

// Export the attributes of General_polygon_with_holes_2.
template <typename Type>
void export_general_polygon_with_holes_2(py::class_<Type>& pwh_c) {
  using Gpwh = Type;
  using Gpgn = typename Gpwh::General_polygon_2;

  pwh_c.def(py::init<Gpwh&>())
    .def(py::init<Gpgn&>())
    .def("__init__", &init_polygon_with_holes_2<Gpwh>)
    .def("is_unbounded", &Gpwh::is_unbounded)

    // Use `py::overload_cast` to cast overloaded functions.
    // 1. As a convention, add the suffix `_mutable` to the mutable version.
    // 2. Wrap the mutable method with the `reference_internal` call policy.
    // 3. Add the `const_` tag to the overloaded const function, as the
    //    overloading is based on constness.
    .def("outer_boundary_mutable", py::overload_cast<>(&Gpwh::outer_boundary),
         py::rv_policy::reference_internal)
    .def("outer_boundary",
         py::overload_cast<>(&Gpwh::outer_boundary, py::const_))
    .def("add_hole", py::overload_cast<const Gpgn&>(&Gpwh::add_hole))
    .def("erase_hole", &Gpwh::erase_hole)
    .def("has_holes", &Gpwh::has_holes)
    .def("number_of_holes", &Gpwh::number_of_holes)
    .def("clear", &Gpwh::clear)
    .def("is_plane", &Gpwh::is_plane)
    ;

  using Hci = typename Gpwh::Hole_const_iterator;
  add_iterator<Hci, Hci>("Hole_iterator", pwh_c);
  pwh_c.def("holes",
            [] (const Gpwh& pwh)
            { return make_iterator(pwh.holes_begin(), pwh.holes_end()); },
            py::keep_alive<0, 1>());

  add_insertion(pwh_c, "__str__");
  add_insertion(pwh_c, "__repr__");

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

/*! Capture the call to export a Polygon_with_holes_2<> and ensure that it
 * is not invoked.
 */
template <typename Kernel, typename Container>
inline
void export_general_polygon_with_holes_2
(py::class_<CGAL::Polygon_with_holes_2<Kernel, Container>>& /* pwh_c */) {
  throw std::runtime_error("Attempting to export Polygon_with_holes_2 as General_polygon_with_holes_2!");
}

#endif
