// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ENV_TRAITS_CLASSES_HPP
#define CGALPY_ENV_TRAITS_CLASSES_HPP

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename T>
struct Env_traits_classes {
  using Surface_3 = typename T::Surface_3;
  using Xy_monotone_surface_3 = typename T::Xy_monotone_surface_3;
  using Make_xy_monotone_3 = typename T::Make_xy_monotone_3;
  using Cpb_2 = typename T::Construct_projected_boundary_2;
  using Cpi_2 = typename T::Construct_projected_intersections_2;
  using Cmp_z_at_xy_3 = typename T::Compare_z_at_xy_above_3;
  using Cmp_z_at_xy_above_3 = typename T::Compare_z_at_xy_above_3;
  using Cmp_z_at_xy_below_3 = typename T::Compare_z_at_xy_below_3;

  // Constructor
  Env_traits_classes() :
    m_surface_3(nullptr),
    m_xy_monotone_surface_3(nullptr),
    m_make_xy_monotone_3(nullptr),
    m_cpb_2(nullptr),
    m_cpi_2(nullptr),
    m_cmp_z_at_xy_3(nullptr),
    m_cmp_z_at_xy_above_3(nullptr),
    m_cmp_z_at_xy_below_3(nullptr)
  {}

  // Destructor
  ~Env_traits_classes() {
    if (m_surface_3) delete m_surface_3;
    if (m_xy_monotone_surface_3) delete m_xy_monotone_surface_3;
    if (m_make_xy_monotone_3) delete m_make_xy_monotone_3;
    if (m_cpb_2) delete m_cpb_2;
    if (m_cpi_2) delete m_cpi_2;
    if (m_cmp_z_at_xy_3) delete m_cmp_z_at_xy_3;
    if (m_cmp_z_at_xy_above_3) delete m_cmp_z_at_xy_above_3;
    if (m_cmp_z_at_xy_below_3) delete m_cmp_z_at_xy_below_3;
  }

  // Class objects
  py::class_<Surface_3>* m_surface_3;
  py::class_<Xy_monotone_surface_3>* m_xy_monotone_surface_3;
  py::class_<Make_xy_monotone_3>* m_make_xy_monotone_3;
  py::class_<Cpb_2>* m_cpb_2;
  py::class_<Cpi_2>* m_cpi_2;
  py::class_<Cmp_z_at_xy_3>* m_cmp_z_at_xy_3;
  py::class_<Cmp_z_at_xy_above_3>* m_cmp_z_at_xy_above_3;
  py::class_<Cmp_z_at_xy_below_3>* m_cmp_z_at_xy_below_3;
};

#endif
