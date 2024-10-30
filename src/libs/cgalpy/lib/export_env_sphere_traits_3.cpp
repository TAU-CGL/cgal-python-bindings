// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Env_sphere_traits_3.h>

#include "CGALPY/envelope_3_types.hpp"
#include "CGALPY/add_attr.hpp"

#include "CGALPY/env_3_concepts/Env_traits_classes.hpp"
#include "CGALPY/env_3_concepts/export_EnvelopeTraits_3.hpp"

void export_arr_conic_traits_2(py::module_&);

namespace py = nanobind;

void export_env_sphere_traits_3(py::module_& m) {
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rat_kernel = CGAL::Cartesian<Nt_traits::Rational>;
  using Alg_kernel = CGAL::Cartesian<Nt_traits::Algebraic>;
  using Conic_gt = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  using Gt = CGAL::Env_sphere_traits_3<Conic_gt>;

  if (add_attr<Gt>(m, "Env_sphere_traits_3")) return;

  if (! add_attr<Conic_gt>(m, "Arr_conic_traits_2"))
    export_arr_conic_traits_2(m);
  py::class_<Gt, Conic_gt> traits_c(m, "Env_sphere_traits_3");

  struct Concepts {
    Env_traits_classes<Gt> m_env_traits_3_classes;
  };
  Concepts concepts;
  export_EnvelopeTraits_3<Gt>(traits_c, concepts);
}
