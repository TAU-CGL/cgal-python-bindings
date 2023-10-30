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
#include "CGALPY/envelope_3_types.hpp"
#include "CGALPY/add_attr.hpp"

#include "CGALPY/env_3_concepts/Env_traits_classes.hpp"
#include "CGALPY/env_3_concepts/export_EnvelopeTraits_3.hpp"
#include "CGALPY/add_insertion.hpp"

void export_arr_linear_traits_2(py::module_&);

namespace py = nanobind;

void export_env_plane_traits_3(py::module_& m) {
  using Linear_gt = CGAL::Arr_linear_traits_2<Kernel>;
  using Gt = CGAL::Env_plane_traits_3<Kernel>;
  using Env_pln = Gt::_Env_plane;
  using Pln = Gt::Plane_3;
  using Line_2 = Gt::Line_2;

  if (add_attr<Gt>(m, "Env_plane_traits_3")) return;

  if (! add_attr<Linear_gt>(m, "Arr_linear_traits_2"))
    export_arr_linear_traits_2(m);
  py::class_<Gt, Linear_gt> traits_c(m, "Env_plane_traits_3");

  py::class_<Env_pln>(m, "_Env_plane")
    .def(py::init<>())
    .def(py::init<const Pln&>())
    .def(py::init<const Pln&, const Line_2&>())
    .def("is_vertical", &Env_pln::is_vertical)
    .def("is_all_plane", &Env_pln::is_all_plane)
    .def("plane", &Env_pln::plane)
    .def("line", &Env_pln::line)
    ;

  struct Concepts {
    Env_traits_classes<Gt> m_env_traits_3_classes;
  };
  Concepts concepts;
  export_EnvelopeTraits_3<Gt>(traits_c, concepts);

  using Xy_monotone_srf_3 = typename Gt::Xy_monotone_surface_3;
  using Cmp_z_at_xy_3 = typename Gt::Compare_z_at_xy_3;
  auto& cmp_z_at_xy_3_c = *(concepts.m_env_traits_3_classes.m_cmp_z_at_xy_3);
  cmp_z_at_xy_3_c.def("__call__",
                      py::overload_cast<const Xy_monotone_srf_3&,
                                        const Xy_monotone_srf_3&>
                      (&Cmp_z_at_xy_3::operator(), py::const_));
}
