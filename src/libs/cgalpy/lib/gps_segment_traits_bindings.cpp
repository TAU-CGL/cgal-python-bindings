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

//
py::object export_gps_segment_traits(py::module_& m) {
  using Agt = aos2::Arr_geometry_traits_2;
  using Ggt = CGAL::Gps_segment_traits_2<Kernel, Point_2_container>;

  py::class_<Ggt, Agt> traits_c(m, "Gps_segment_traits_2");
  traits_c.def(py::init<>());
  struct Concepts {
    Gps_traits_classes<Ggt> m_traits_classes;
  } concepts;
  export_GpsTraits_2<Ggt>(traits_c, concepts);

  return traits_c;
}
