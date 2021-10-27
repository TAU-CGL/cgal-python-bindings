// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_BOOLEAN_SET_OPERATIONS_2_TYPES_HPP
#define CGALPY_BOOLEAN_SET_OPERATIONS_2_TYPES_HPP

//! \todo Replace the following with meta programming
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
#include "CGALPY/polygon_set_2_types.hpp"
#else
#include "CGALPY/general_polygon_set_2_types.hpp"
#endif

#endif
