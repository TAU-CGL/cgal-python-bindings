// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPORT_ENVELOPETRAITS_3_HPP
#define CGALPY_EXPORT_ENVELOPETRAITS_3_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/env_3_concepts/Env_traits_classes.hpp"
#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

//! Export the 3D envelope traits function objects
template <typename T, typename C, typename Concepts>
void export_EnvelopeTraits_3(C& c, Concepts& concepts) {
  static bool exported = false;
  if (exported) return;

  using Surface_3 = typename T::Surface_3;
  using Xy_monotone_surface_3 = typename T::Xy_monotone_surface_3;
  using Make_xy_monotone_3 = typename T::Make_xy_monotone_3;
  using Cpb_2 = typename T::Construct_projected_boundary_2;
  using Cpi_2 = typename T::Construct_projected_intersections_2;
  using Cmp_z_at_xy_3 = typename T::Compare_z_at_xy_3;
  using Cmp_z_at_xy_above_3 = typename T::Compare_z_at_xy_above_3;
  using Cmp_z_at_xy_below_3 = typename T::Compare_z_at_xy_below_3;

  // Surface_3
  if (! add_attr<Surface_3>(c, "Surface_3")) {
    classes.m_surface_3 = new py::class_<Surface_3>(c, "Surface_3");
    classes.m_surface_3->def(py::init<>());
    classes.m_surface_3->def(py::init<const Surface_3&>());
  }

  // Xy_monotone_surface_3
  if (! add_attr<Xy_monotone_surface_3>(c, "Xy_monotone_surface_3")) {
    classes.xy_monotone_surface_3 =
      new py::class_<Xy_monotone_surface_3>(c, "Xy_monotone_surface_3");
    classes.xy_monotone_surface_3->def(py::init<>());
    classes.xy_monotone_surface_3->def(py::init<const Xy_monotone_surface_3&>());
  }

  // Make_xy_monotone_3
  if (! add_attr<Make_xy_monotone_3>(c, "Make_xy_monotone_3")) {
    classes.m_make_xy_monotone_3 =
      new py::class_<Make_xy_monotone_3>(c, "Make_xy_monotone_3");
    classes.m_make_xy_monotone_3->def("__call__", &Make_xy_monotone_3::operator());
  }

  // Construct_projected_boundary_2
  if (! add_attr<Cpb_2>(c, "Construct_projected_boundary_2")) {
    classes.m_cpb_2 = new py::class_<Cpb_2>(c, "Construct_projected_boundary_2");
    classes.m_cpb_2->def("__call__", &Cpb_2::operator());
  }

  // Construct_projected_intersections_2
  if (! add_attr<Cpi_2>(c, "Construct_projected_intersections_2")) {
    classes.m_cpi_2 =
      new py::class_<Cpi_2>(c, "Construct_projected_intersections_2");
    classes.m_cpi_2->def("__call__", &Cpi_2::operator());
  }

  // Compare_z_at_xy_3
  if (! add_attr<Cmp_z_at_xy_3>(c, "Compare_z_at_xy_3")) {
    classes.m_cmp_z_at_xy_3 =
      new py::class_<Cmp_z_at_xy_3>(c, "Compare_z_at_xy_3");
    classes.m_cmp_z_at_xy_3->def("__call__", &Cmp_z_at_xy_3::operator());
  }

  // Compare_z_at_xy_above_3
  if (! add_attr<Cmp_z_at_xy_above_3>(c, "Compare_z_at_xy_above_3")) {
    classes.m_cmp_z_at_xy_above_3 =
      new py::class_<Cmp_z_at_xy_above_3>(c, "Compare_z_at_xy_above_3");
    classes.m_cmp_z_at_xy_above_3->def("__call__",
                                       &Cmp_z_at_xy_above_3::operator());
  }

  // Compare_z_at_xy_below_3
  if (! add_attr<Cmp_z_at_xy_below_3>(c, "Compare_z_at_xy_below_3")) {
    classes.m_cmp_z_at_xy_below_3 =
      new py::class_<Cmp_z_at_xy_below_3>(c, "Compare_z_at_xy_below_3");
    classes.m_cmp_z_at_xy_below_3->def("__call__",
                                       &Cmp_z_at_xy_below_3::operator());
  }
}

#endif
