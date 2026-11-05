// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_IS_POLYHEDRON_3_MAP_HPP
#define CGALPY_IS_POLYHEDRON_3_MAP_HPP

#include <type_traits>

#include <CGAL/Polyhedron_3_fwd.h>

namespace cgalpy {

//! Determines whether `T` is an instance of `CGAL::Polyhedron_3`.
template <typename T>
struct is_polyhedron_3 : std::false_type {};

template <typename Traits, typename Items, template <typename, typename, typename> class HDS, typename Alloc>
struct is_polyhedron_3<CGAL::Polyhedron_3<Traits, Items, HDS, Alloc>> : std::true_type {};

template <typename T>
inline constexpr bool is_polyhedron_3_v = is_polyhedron_3<std::remove_cv_t<T>>::value;

} // namespace cgalpy

#endif
