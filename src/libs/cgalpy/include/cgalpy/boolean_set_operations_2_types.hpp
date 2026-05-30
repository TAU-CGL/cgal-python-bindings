// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_BOOLEAN_SET_OPERATIONS_2_TYPES_HPP
#define CGALPY_BOOLEAN_SET_OPERATIONS_2_TYPES_HPP

//! \todo Replace the following with meta programming
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
#include "cgalpy/polygon_set_2_types.hpp"
#else
#include "cgalpy/general_polygon_set_2_types.hpp"
#endif

#endif
