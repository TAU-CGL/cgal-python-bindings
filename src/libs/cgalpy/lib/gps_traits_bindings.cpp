// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Gps_traits_2.h>
#include <CGAL/General_polygon_set_2.h>

#include "CGALPY/general_polygon_set_2_types.hpp"
#include "CGALPY/gps_2_concepts/export_GpsTraits_2.hpp"
#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"

namespace py = nanobind;

namespace bso2 {

template <typename T>
void init_polygon_2(typename T::Polygon_2& pgn, py::list& lst) {
  auto begin = stl_input_iterator<typename T::X_monotone_curve_2>(lst);
  auto end = stl_input_iterator<typename T::X_monotone_curve_2>(lst, false);
  new (&pgn) typename T::Polygon_2(begin, end);
}

template <typename T>
typename T::Polygon_2::Curve_iterator curves_begin(typename T::Polygon_2& p)
{ return p.curves_begin(); }

template <typename T>
typename T::Polygon_2::Curve_iterator curves_end(typename T::Polygon_2& p)
{ return p.curves_end(); }

}

py::class_<aos2::Geometry_traits_2> export_arr_conic_traits();
py::class_<aos2::Geometry_traits_2> export_arr_algebraic_segment_traits();

py::object export_gps_traits(py::module_& m) {
  using GT = bso2::Geometry_traits_2;
  // auto traits = py::class_<GT>("Traits_2");

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
  auto traits = export_arr_conic_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  auto traits = export_arr_algebraic_segment_traits();
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

  struct Concepts {
    Gps_traits_classes<GT> m_traits_classes;
  } concepts;
  export_GpsTraits_2<GT>(traits, concepts);
  auto* tco = concepts.m_traits_classes.m_polygon_2;
  if (tco) {
    tco->def("__init__", &bso2::init_polygon_2<GT>);
    tco->def("curves", py::range<py::return_internal_reference<>>
             (&bso2::curves_begin<GT>, &bso2::curves_end<GT>));
  }
  return traits;
}
