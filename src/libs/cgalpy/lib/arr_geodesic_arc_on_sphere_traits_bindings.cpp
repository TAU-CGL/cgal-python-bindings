// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace py = nanobind;

py::class_<aos2::Geometry_traits_2> export_arr_geodesic_arc_on_sphere_traits() {
  typedef aos2::Geometry_traits_2       GT;

  auto traits = py::class_<GT>("Geometry_traits_2")
    .def(py::init<>());
  py::scope traits_scope(traits);
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_directional_x_monotone_traits_classes<GT> m_directional_x_monotone_traits_classes;
  } concepts;
  export_AosTraits_2<GT, Return_by_value>(traits, concepts);
  export_AosDirectionalXMonotoneTraits_2<GT, Return_by_value>(traits, concepts);

  return traits;
}
