// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_conic_traits_2.h>

#include "CGALPY/envelope_3_types.hpp"
#include "CGALPY/add_attr.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "CGALPY/env_3_concepts/Env_traits_classes.hpp"
#include "CGALPY/env_3_concepts/export_EnvelopeTraits_3.hpp"

namespace py = nanobind;

void export_env_sphere_traits_3(py::module_& m) {
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rat_kernel = CGAL::Cartesian<Nt_traits::Rational>;
  using Alg_kernel = CGAL::Cartesian<Nt_traits::Algebraic>;
  using Aos2_gt = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  using Gt = env3::Geometry_traits_3;

  if (add_attr<Gt>(m, "Env_sphere_traits_3")) return;

  py::class_<Gt, Aos2_gt> traits_c(m, "Env_sphere_traits_3");

  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
    Aos_directional_x_monotone_traits_classes<Gt>
      m_aos_directional_x_monotone_traits_2_classes;
    Env_traits_classes<Gt> m_env_traits_3_classes;
  };
  Concepts concepts;
  export_AosTraits_2<Gt>(traits_c, concepts);
  export_AosDirectionalXMonotoneTraits_2<Gt>(traits_c, concepts);
  export_EnvelopeTraits_3<Gt>(traits_c, concepts);
}
