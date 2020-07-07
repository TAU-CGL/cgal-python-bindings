// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARRANGEMENT_2_CONFIG_HPP
#define CGALPY_ARRANGEMENT_2_CONFIG_HPP

#define CGALPY_ARR2_DEFAULT_DCEL 0
#define CGALPY_ARR2_FACE_EXTENDED_DCEL 1
#define CGALPY_ARR2_EXTENDED_DCEL 4

#ifndef CGALPY_ARR2_DCEL
#define CGALPY_ARR2_DCEL 0
#endif

#define CGALPY_ARR2_LINEAR_GEOMETRY_TRAITS 0
#define CGALPY_ARR2_SEGMENT_GEOMETRY_TRAITS 1
#define CGALPY_ARR2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS 2
#define CGALPY_ARR2_CONIC_GEOMETRY_TRAITS 3
#define CGALPY_ARR2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS 4
#define CGALPY_ARR2_CIRCLE_SEGMENT_GEOMETRY_TRAITS 5

#ifndef CGALPY_ARR2_GEOMETRY_TRAITS
#define CGALPY_ARR2_GEOMETRY_TRAITS 1
#endif

#endif //CGALPY_ARRANGEMENT_2_CONFIG_HPP
