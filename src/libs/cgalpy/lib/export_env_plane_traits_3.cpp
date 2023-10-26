// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Env_plane_traits_3.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/envelope_3_types.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_landmark_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_open_boundary_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosLandmarkTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosOpenBoundaryTraits_2.hpp"
#include "CGALPY/env_3_concepts/Env_traits_classes.hpp"
#include "CGALPY/env_3_concepts/export_EnvelopeTraits_3.hpp"
#include "CGALPY/Kernel/export_ft.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"
#include "CGALPY/Kernel/export_circle_2.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

void export_env_plane_traits_3(py::module_& m) {
  using Aos2_gt = CGAL::Arr_linear_traits_2<Kernel>;
  using Gt = CGAL::Env_plane_traits_3<Kernel>;
  using Env_pln = Gt::_Env_plane;
  using Pln = Gt::Plane_3;
  using Line_2 = Gt::Line_2;

  if (add_attr<Gt>(m, "Env_plane_traits_3")) return;

  py::class_<Gt, Aos2_gt> traits_c(m, "Env_plane_traits_3");

  py::class_<Env_pln>(m, "_Env_plane")
    .def(py::init<>())
    .def(py::init<const Pln&>())
    .def(py::init<const Pln&, const Line_2&>())
    ;

  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
    Aos_landmark_traits_classes<Gt> m_aos_landmark_traits_2_classes;
    Aos_open_boundary_traits_classes<Gt> m_aos_open_boundary_traits_2_classes;
    Aos_approximate_traits_classes<Gt> m_aos_approximate_traits_2_classes;
    Aos_construct_x_monotone_curve_traits_classes<Gt>
      m_aos_construct_x_monotone_curve_traits_2_classes;
    Env_traits_classes<Gt> m_env_traits_3_classes;
  };
  Concepts concepts;
  export_AosTraits_2<Gt>(traits_c, concepts);
  export_AosLandmarkTraits_2<Gt>(traits_c, concepts);
  export_AosOpenBoundaryTraits_2<Gt>(traits_c, concepts);
  export_EnvelopeTraits_3<Gt>(traits_c, concepts);

  using Xy_monotone_srf_3 = typename Gt::Xy_monotone_surface_3;
  using Cmp_z_at_xy_3 = typename Gt::Compare_z_at_xy_3;
  auto& cmp_z_at_xy_3_c = *(concepts.m_env_traits_3_classes.m_cmp_z_at_xy_3);
  cmp_z_at_xy_3_c.def("__call__",
                      py::overload_cast<const Xy_monotone_srf_3&,
                                        const Xy_monotone_srf_3&>
                      (&Cmp_z_at_xy_3::operator(), py::const_));
}
