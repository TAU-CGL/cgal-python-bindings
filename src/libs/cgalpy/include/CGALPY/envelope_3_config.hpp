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
#include <CGAL/Env_sphere_traits_3.h>
#include <CGAL/Env_triangle_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>

#include "CGALPY/config.hpp"

namespace env3 {

#define CGALPY_ENV3_PLANE_GEOMETRY_TRAITS	0
#define CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS      1
#define CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS    2

// Traits
template <int i, typename K> struct Base_tr { typedef K type; };
template <typename K> struct Base_tr<CGALPY_ENV3_PLANE_GEOMETRY_TRAITS, K>
{ typedef CGAL::Env_plane_traits_3<K> type; };
template <typename K> struct Base_tr<CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS, K>
{ typedef CGAL::Env_sphere_traits_3<K> type; };
template <typename K> struct Base_tr<CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS, K>
{ typedef CGAL::Env_triangle_traits_3<K> type; };

// Surface data
constexpr bool surface_data() { return DETECT_EXIST(CGALPY_ENV3_SURFACE_DATA); }

template <bool b, typename Btr, typename XyData, typename SData, typename Cnv>
struct Tr {};

template <typename Btr, typename XyData, typename SData, typename Cnv>
struct Tr<false, Btr, XyData, SData, Cnv>
{ typedef Btr type; };

template <typename Btr, typename XyData, typename SData, typename Cnv>
struct Tr<true, Btr, XyData, SData, Cnv>
{ typedef CGAL::Env_surface_data_traits_3<Btr, XyData, SData, Cnv> type; };

}

#endif
