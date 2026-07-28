// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARRANGEMENT_ON_SURFACE_2_VALUES_HPP
#define CGALPY_ARRANGEMENT_ON_SURFACE_2_VALUES_HPP

// Geometry traits options
#define CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS           0
#define CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS                      1
#define CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS              2
#define CGALPY_AOS2_CONIC_GEOMETRY_TRAITS                       3
#define CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS      4
#define CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS                      5
#define CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS         6
#define CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS        7
#define CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS                     8
#define CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS           9

#ifndef CGALPY_AOS2_GEOMETRY_TRAITS
#define CGALPY_AOS2_GEOMETRY_TRAITS                             8
#endif

#endif
