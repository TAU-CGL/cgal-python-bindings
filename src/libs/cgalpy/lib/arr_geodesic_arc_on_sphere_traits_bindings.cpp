// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace py = nanobind;

py::object export_arr_geodesic_arc_on_sphere_traits() {
  using GT = CGAL::Arr_geodesic_arc_on_sphere_traits_2<Kernel>;

  auto traits = py::class_<GT>("Arr_geodesic_arc_on_sphere_traits_2")
    .def(py::init<>());
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
