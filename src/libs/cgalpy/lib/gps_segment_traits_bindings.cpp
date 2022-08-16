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

py::object export_arr_segment_traits(py::module_&);
py::object export_arr_non_caching_segment_traits(py::module_&);

py::object export_gps_segment_traits(py::module_& m) {
  using AGT = CGAL::Arr_segment_traits_2<Kernel>;
  using GT = CGAL::Gps_segment_traits_2<Kernel, Point_2_container>;

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  export_arr_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  export_arr_non_caching_segment_traits(m);
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

  py::class_<GT, AGT> traits_co(m, "Gps_segment_traits_2");
  traits_co.def(py::init<>());
  struct Concepts {
    Gps_traits_classes<GT> m_traits_classes;
  } concepts;
  export_GpsTraits_2<GT>(traits_co, concepts);
  return traits_co;
}
