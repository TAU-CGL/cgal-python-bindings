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
#include "CGALPY/stl_forward_iterator.hpp"

namespace py = nanobind;

//
void export_gps_traits_2(py::module_& m) {
  using Agt = aos2::Arr_geometry_traits_2;
  using Ggt = CGAL::Gps_traits_2<Agt>;

  if (add_attr<Ggt>(m, "Gps_traits_2")) return;

  py::class_<Ggt, Agt> traits_c(m, "Gps_traits_2");
  traits_c.def(py::init<>());
  struct Concepts {
    Gps_traits_classes<Ggt> m_gps_traits_2_classes;
  } concepts;
  export_GpsTraits_2<Ggt>(traits_c, concepts);
}
