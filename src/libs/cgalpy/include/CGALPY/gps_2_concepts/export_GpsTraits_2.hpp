// Copyright (c) 2021 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_GPSTRAITS_2_HPP
#define CGALPY_EXPORT_GPSTRAITS_2_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"
#include "CGALPY/export_general_polygon_2.hpp"
#include "CGALPY/export_general_polygon_with_holes_2.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace bso2 {

/* Export the operator() of the Construct_polygon_2 function object.
 * \todo This operator doesn't construct a new general Polygon_2, but rather
 * initializes one.
 * The CGAL code should be fixed to reflect it. Either change the name of the
 * nested type (and the name of the corresponding concept) or change the
 * function.
 */
template <typename T>
void ctr_pgn_op(const typename T::Construct_polygon_2& ctr,
                const py::list& curves,
                typename T::Polygon_2& pgn) {
  using Xcv = typename T::X_monotone_curve_2;
  auto begin = stl_input_iterator<Xcv>(curves);
  auto end = stl_input_iterator<Xcv>(curves, false);
  ctr(begin, end, pgn);
}

/* Export an operator() of the Construct_polygon_with_holes_2 function object
 * that constructs a (general) polygin with holes.
 * However, do not export the original operator that accepts C++ range of holes.
 * Instead, export a function that accepts the boundary polygon and a list of
 * Python objects that represent the holes.
 */
template <typename T>
typename T::Polygon_with_holes_2
ctr_pwh_op(const typename T::Construct_polygon_with_holes_2 ctr,
           const typename T::Polygon_2& boundary,
           const py::list& holes) {
  using Pgn = typename T::Polygon_2;
  auto begin = stl_input_iterator<const Pgn&>(holes);
  auto end = stl_input_iterator<const Pgn&>(holes, false);
  return ctr(boundary, begin, end);
}

}

//
template <typename T, typename C, typename Concepts>
void export_GpsTraits_2(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  using Pgn = typename T::Polygon_2;
  using Pwh = typename T::Polygon_with_holes_2;
  using Ctr_pgn = typename T::Construct_polygon_2;
  using Ctr_curves_2 = typename T::Construct_curves_2;
  using Ctr_pwh = typename T::Construct_polygon_with_holes_2;
  using Ctr_outer_boundary = typename T::Construct_outer_boundary;
  using Ctr_holes = typename T::Construct_holes;
  using Is_unbounded = typename T::Is_unbounded;

  auto& classes = concepts.m_traits_classes;

  // Polygon_2
  if (! add_attr<Pgn>(c, "Polygon_2")) {
    classes.m_polygon_2 = new py::class_<Pgn>(c, "Polygon_2");
    export_general_polygon_2<Pgn>(*(classes.m_polygon_2));
  }

  // Polygon_with_holes_2
  if (! add_attr<Pwh>(c, "Polygon_with_holes_2")) {
    classes.m_polygon_with_holes_2 =
      new py::class_<Pwh>(c, "Polygon_with_holes_2");
    export_general_polygon_with_holes_2<Pwh>
      (*(classes.m_polygon_with_holes_2));
  }

  // Construct_polygon_2
  classes.m_construct_polygon_2 =
    new py::class_<Ctr_pgn>(c, "Construct_polygon_2");
  auto& ctr_pgn_c = *(classes.m_construct_polygon_2);
  ctr_pgn_c.def("__call__", &bso2::ctr_pgn_op<T>);

  // Construct_curves_2
  classes.m_construct_curves_2 =
    new py::class_<Ctr_curves_2>(c, "Construct_curves_2");
  auto& ctr_curves_c = *(classes.m_construct_curves_2);
  using Cci = typename T::Curve_const_iterator;
  using Xcv = typename T::X_monotone_curve_2;
  add_iterator<Cci, Cci, Xcv>("Curve_iterator", ctr_curves_c);
  ctr_curves_c.def("curves",
                   [](const Pgn& pgn) {
                     return make_iterator(pgn.curves_begin(), pgn.curves_end());
                   },
                   py::keep_alive<0, 1>());

  // Construct_polygon_with_holes_2
  classes.m_construct_polygon_with_holes_2 =
    new py::class_<Ctr_pwh>(c, "Construct_polygon_with_holes_2");
  auto& ctr_pwh_c = *(classes.m_construct_polygon_with_holes_2);
  using Ctr_pwh_op1 = Pwh(Ctr_pwh::*)(const Pgn&) const;
  ctr_pwh_c.def("__call__", static_cast<Ctr_pwh_op1>(&Ctr_pwh::operator()));
  ctr_pwh_c.def("__call__", &bso2::ctr_pwh_op<T>);

  // Construct_outer_boundary
  classes.m_construct_outer_boundary =
    new py::class_<Ctr_outer_boundary>(c, "Construct_outer_boundary");
  classes.m_construct_outer_boundary->
    def("__call__", &Ctr_outer_boundary::operator());

  // Construct_holes
  classes.m_construct_holes = new py::class_<Ctr_holes>(c, "Construct_holes");
  classes.m_construct_holes->def("__call__", &Ctr_holes::operator());

  // Is unbounded
  classes.m_is_unbounded = new py::class_<Is_unbounded>(c, "Is_unbounded");
  classes.m_is_unbounded->def("__call__", &Is_unbounded::operator());

  c.def("construct_polygon_2_object", &T::construct_polygon_2_object)
    .def("construct_curves_2_object", &T::construct_curves_2_object)
    .def("construct_outer_boundary_object", &T::construct_outer_boundary_object)
    .def("construct_holes_object", &T::construct_holes_object)
    .def("construct_polygon_with_holes_2_object",
         &T::construct_polygon_with_holes_2_object)
    .def("is_unbounded_object", &T::is_unbounded_object)
    ;

  exported = true;
}

#endif
