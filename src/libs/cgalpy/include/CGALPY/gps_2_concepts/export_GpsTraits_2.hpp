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

//
template <typename T>
typename T::Polygon_2
export_ctr_gp_2_op(const typename T::Construct_polygon_2& ctr,
                   py::list& curves) {
  using Xcv = typename T::X_monotone_curve_2;
  auto begin = stl_input_iterator<Xcv>(curves);
  auto end = stl_input_iterator<Xcv>(curves, false);
  return ctr(begin, end);
}

//
template <typename T>
typename T::Polygon_with_holes_2
export_ctr_gpwh_2_op
(const typename T::Construct_general_polygon_with_holes_2 ctr,
 const typename T::Polygon_2& boundary, py::list& holes) {
  using Xcv = typename T::Polygon_2;
  auto begin = stl_input_iterator<const Xcv&>(holes);
  auto end = stl_input_iterator<const Xcv&>(holes, false);
  return ctr(boundary, begin, end);
}

}

//
template <typename T, typename C, typename Concepts>
void export_GpsTraits_2(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  using Polygon_2 = typename T::Polygon_2;
  using Polygon_with_holes_2 = typename T::Polygon_with_holes_2;
  using Construct_polygon_2 = typename T::Construct_polygon_2;
  using Construct_polygon_with_holes_2 =
    typename T::Construct_polygon_with_holes_2;
  using Construct_outer_boundary = typename T::Construct_outer_boundary;
  using Construct_holes = typename T::Construct_holes;
  using Is_unbounded = typename T::Is_unbounded;

  auto& classes = concepts.m_traits_classes;

  // Polygon_2
  if (! add_attr<Polygon_2>("Polygon_2", c)) {
    classes.m_polygon_2 = new py::class_<Polygon_2>(c, "Polygon_2");
    export_general_polygon_2<Polygon_2>(*(classes.m_polygon_2));
  }

  // Polygon_with_holes_2
  if (! add_attr<Polygon_with_holes_2>("Polygon_with_holes_2", c)) {
    classes.m_polygon_with_holes_2 =
      new py::class_<Polygon_with_holes_2>(c, "Polygon_with_holes_2");
    export_general_polygon_with_holes_2<Polygon_with_holes_2>
      (*(classes.m_polygon_with_holes_2));
  }

  // Construct_polygon_2
  classes.m_construct_polygon_2 =
    new py::class_<Construct_polygon_2>(c, "Construct_polygon_2");
  // classes.m_construct_polygon_2->def("__call__", &bso2::export_ctr_gp_2_op<T>);

  // Construct_polygon_with_holes_2
  classes.m_construct_polygon_with_holes_2 =
    new py::class_<Construct_polygon_with_holes_2>
    (c, "Construct_polygon_with_holes_2");
  using Ctr_gpwh1 = Polygon_with_holes_2(Construct_polygon_with_holes_2::*)
    (const Polygon_2&) const;
  classes.m_construct_polygon_with_holes_2->
    def("__call__", static_cast<Ctr_gpwh1>
        (&Construct_polygon_with_holes_2::operator()));
  // classes.m_construct_polygon_with_holes_2->
  //   def("__call__", &bso2::export_ctr_gpwh_2_op<T>);

  // Construct_outer_boundary
  classes.m_construct_outer_boundary =
    new py::class_<Construct_outer_boundary>(c, "Construct_outer_boundary");
  classes.m_construct_outer_boundary->
    def("__call__", &Construct_outer_boundary::operator());

  // Construct_holes
  classes.m_construct_holes =
    new py::class_<Construct_holes>(c, "Construct_holes");
  classes.m_construct_holes->def("__call__", &Construct_holes::operator());

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
