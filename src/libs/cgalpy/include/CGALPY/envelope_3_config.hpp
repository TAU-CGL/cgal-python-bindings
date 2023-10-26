// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ENVELOPE_3_CONFIG_HPP
#define CGALPY_ENVELOPE_3_CONFIG_HPP

//! \todo Move to cmake
#define CGALPY_ENV3_PLANE_GEOMETRY_TRAITS	0
#define CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS      1
#define CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS    2

#include <CGAL/tags.h>
#include <CGAL/Env_plane_traits_3.h>
#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#include <CGAL/Env_sphere_traits_3.h>
#include <CGAL/Env_triangle_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>

#include "CGALPY/config.hpp"

namespace env3 {

// Traits
template <int i, typename K> struct Base_tr { using type = K; };
template <typename K> struct Base_tr<CGALPY_ENV3_PLANE_GEOMETRY_TRAITS, K>
{ using type = CGAL::Env_plane_traits_3<K>; };
template <typename K> struct Base_tr<CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS, K> {
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rat_kernel = CGAL::Cartesian <Nt_traits::Rational>;
  using Alg_kernel = CGAL::Cartesian <Nt_traits::Algebraic>;
  using Conic_tr = CGAL::Arr_conic_traits_2<Rat_kernel,Alg_kernel, Nt_traits>;
  using type = CGAL::Env_sphere_traits_3<Conic_tr>;
};
template <typename K> struct Base_tr<CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS, K>
{ using type = CGAL::Env_triangle_traits_3<K>; };

// Surface data
constexpr bool surface_data() { return DETECT_EXIST(CGALPY_ENV3_SURFACE_DATA); }

template <bool b, typename Btr, typename XyData, typename SData, typename Cnv>
struct Tr {};

template <typename Btr, typename XyData, typename SData, typename Cnv>
struct Tr<false, Btr, XyData, SData, Cnv> { using type = Btr; };

template <typename Btr, typename XyData, typename SData, typename Cnv>
struct Tr<true, Btr, XyData, SData, Cnv>
{ using type = CGAL::Env_surface_data_traits_3<Btr, XyData, SData, Cnv>; };

}

#endif
