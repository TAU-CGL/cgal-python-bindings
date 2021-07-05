// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_2_CONFIG_HPP
#define CGALPY_TRIANGULATION_2_CONFIG_HPP

#define CGALPY_TRI2_PLAIN                       0
#define CGALPY_TRI2_REGULAR                     1
#define CGALPY_TRI2_DELAUNAY                    2
#define CGALPY_TRI2_CONSTRAINED                 3
#define CGALPY_TRI2_CONSTRAINED_DELAUNAY        4
#define CGALPY_TRI2_PERIODIC_PLAIN              5
#define CGALPY_TRI2_PERIODIC_DELAUNAY           6

#define CGALPY_TRI2_TRAITS_KERNEL               0
#define CGALPY_TRI2_TRAITS_PERIODIC_PLAIN       1
#define CGALPY_TRI2_TRAITS_PERIODIC_DELAUNAY    2

#define CGALPY_TRI2_VERTEX_BASE_PLAIN           0
#define CGALPY_TRI2_VERTEX_BASE_REGULAR         1

#define CGALPY_TRI2_FACE_BASE_PLAIN             0
#define CGALPY_TRI2_FACE_BASE_REGULAR           1

#define CGALPY_TRI2_INTERSECTION_TAG_NCI                                0
#define CGALPY_TRI2_INTERSECTION_TAG_NCI_REQUIRING_CONSTRUCTIONS        1
#define CGALPY_TRI2_INTERSECTION_TAG_EXACT_PREDICATES                   2
#define CGALPY_TRI2_INTERSECTION_TAG_EXACT_INTERSECTIONS                3

#endif
