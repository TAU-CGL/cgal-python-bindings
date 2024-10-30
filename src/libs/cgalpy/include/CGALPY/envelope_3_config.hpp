// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ENVELOPE_3_CONFIG_HPP
#define CGALPY_ENVELOPE_3_CONFIG_HPP

#include <CGAL/tags.h>
#include <CGAL/Env_plane_traits_3.h>
#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#include <CGAL/Env_sphere_traits_3.h>
#include <CGAL/Env_triangle_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>

#define CGALPY_ENV3_PLANE_GEOMETRY_TRAITS     0
#define CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS    1
#define CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS  2

#if ! defined(CGALPY_ENV3_GEOMETRY_TRAITS)
#define CGALPY_ENV3_GEOMETRY_TRAITS           CGALPY_ENV3_PLANE_GEOMETRY_TRAITS
#endif

#endif
