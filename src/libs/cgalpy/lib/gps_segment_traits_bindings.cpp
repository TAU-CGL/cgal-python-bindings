// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Gps_segment_traits_2.h>
#include <CGAL/Polygon_set_2.h>

#include "CGALPY/polygon_set_2_types.hpp"
#include "CGALPY/gps_2_concepts/export_GpsTraits_2.hpp"
#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"

namespace py = nanobind;

py::class_<aos2::Geometry_traits_2> export_arr_segment_traits();
py::class_<aos2::Geometry_traits_2> export_arr_non_caching_segment_traits();

py::object export_gps_segment_traits() {
  typedef bso2::Geometry_traits_2       GT;

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  auto traits = export_arr_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  auto traits = export_arr_non_caching_segment_traits();
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

  py::scope traits_scope(traits);
  struct Concepts {
    Gps_traits_classes<GT> m_traits_classes;
  } concepts;
  export_GpsTraits_2<GT>(traits, concepts);
  return traits;
}
