// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_IS_SURFACE_MESH_MAP_HPP
#define CGALPY_IS_SURFACE_MESH_MAP_HPP

#include <type_traits>

#include <CGAL/Surface_mesh/Surface_mesh_fwd.h>

namespace cgalpy {

//! Determines whether `T` is an instance of `CGAL::Surface_mesh`.
template <typename T>
struct is_surface_mesh : std::false_type {};

template <typename P>
struct is_surface_mesh<CGAL::Surface_mesh<P>> : std::true_type {};

template <typename T>
inline constexpr bool is_surface_mesh_v = is_surface_mesh<std::remove_cv_t<T>>::value;

} // namespace cgalpy

#endif
