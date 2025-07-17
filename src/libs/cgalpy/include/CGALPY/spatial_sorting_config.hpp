// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_SPATIAL_SORTING_CONFIG_HPP
#define CGALPY_SPATIAL_SORTING_CONFIG_HPP

#include <CGAL/tags.h>

#include "CGALPY/spatial_sorting_values.hpp"

// Concurrency
template <int i> struct Concurrency {};
template <> struct Concurrency<CGALPY_ST_CONCURRENCY_SEQUENTIAL>
{ using type = CGAL::Sequential_tag; };
template <> struct Concurrency<CGALPY_ST_CONCURRENCY_PARALLEL>
{ using type = CGAL::Parallel_tag; };

#endif
