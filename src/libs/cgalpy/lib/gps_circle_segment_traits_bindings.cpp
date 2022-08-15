// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>

#include "CGALPY/general_polygon_set_2_types.hpp"
#include "CGALPY/gps_2_concepts/export_GpsTraits_2.hpp"
#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"

namespace py = nanobind;

py::class_<aos2::Geometry_traits_2> export_arr_circle_segment_traits(py::module_&);

namespace bso2 {

// template <typename T>
// typename T::Polygon_2* init_polygon_2(py::list& lst) {
//   auto begin = py::stl_input_iterator<typename T::X_monotone_curve_2>(lst);
//   auto end = py::stl_input_iterator<typename T::X_monotone_curve_2>();
//   return new typename T::Polygon_2(begin, end);
// }

template <typename T>
typename T::Polygon_2::Curve_iterator curves_begin(typename T::Polygon_2& p)
{ return p.curves_begin(); }

template <typename T>
typename T::Polygon_2::Curve_iterator curves_end(typename T::Polygon_2& p)
{ return p.curves_end(); }

}

py::object export_gps_circle_segment_traits(py::module_& m) {
  auto traits = export_arr_circle_segment_traits(m);

  using GT = bso2::Geometry_traits_2;

  struct Concepts {
    Gps_traits_classes<GT> m_traits_classes;
  } concepts;
  export_GpsTraits_2<GT>(traits, concepts);
  auto* tco = concepts.m_traits_classes.m_polygon_2;
  if (tco) {
    // tco->def("__init__", make_constructor(&bso2::init_polygon_2<GT>)); NB
    // tco->def("curves", py::range<py::return_internal_reference<>>
    //          (&bso2::curves_begin<GT>, &bso2::curves_end<GT>)); NB
  }
  return traits;
}
