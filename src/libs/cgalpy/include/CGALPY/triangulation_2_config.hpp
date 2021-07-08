// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_2_CONFIG_HPP
#define CGALPY_TRIANGULATION_2_CONFIG_HPP

#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Regular_triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Periodic_2_triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_hierarchy_vertex_base_2.h>
#include <CGAL/Alpha_shape_vertex_base_2.h>

#include <CGAL/Triangulation_face_base_2.h>
#include <CGAL/Regular_triangulation_face_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Periodic_2_triangulation_face_base_2.h>
#include <CGAL/Alpha_shape_face_base_2.h>

#include <CGAL/Triangulation_data_structure_2.h>

#include <CGAL/Triangulation_2.h>
#include <CGAL/Regular_triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Periodic_2_triangulation_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_2.h>

#include <CGAL/Triangulation_hierarchy_2.h>
#include <CGAL/Periodic_2_triangulation_hierarchy_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_traits_2.h>

#include "CGALPY/config.hpp"
#include "CGALPY/alpha_shape_2_config.hpp"

#define CGALPY_TRI2_PLAIN                       0
#define CGALPY_TRI2_REGULAR                     1
#define CGALPY_TRI2_DELAUNAY                    2
#define CGALPY_TRI2_CONSTRAINED                 3
#define CGALPY_TRI2_CONSTRAINED_DELAUNAY        4
#define CGALPY_TRI2_PERIODIC_PLAIN              5
#define CGALPY_TRI2_PERIODIC_DELAUNAY           6

#define CGALPY_TRI2_FACE_BASE_PLAIN             0
#define CGALPY_TRI2_FACE_BASE_REGULAR           1

#define CGALPY_TRI2_INTERSECTION_TAG_NCI                                0
#define CGALPY_TRI2_INTERSECTION_TAG_NCI_REQUIRING_CONSTRUCTIONS        1
#define CGALPY_TRI2_INTERSECTION_TAG_EXACT_PREDICATES                   2
#define CGALPY_TRI2_INTERSECTION_TAG_EXACT_INTERSECTIONS                3

namespace tri2 {

// Indicates whether a triangulation hierarchy is selected
constexpr bool hierarchy() { return DETECT_EXIST(CGALPY_TRI2_HIERARCHY); }

// Indicates whether a vertex with infois selected
constexpr bool vertex_with_info()
{ return DETECT_EXIST(CGALPY_TRI2_VERTEX_WITH_INFO); }

// Indicates whether a vertex with infois selected
constexpr bool face_with_info()
{ return DETECT_EXIST(CGALPY_TRI2_FACE_WITH_INFO); }

// Indicates whether the selected triangulation is periodic
constexpr bool is_periodic() {
  return ((CGALPY_TRI2 == CGALPY_TRI2_PERIODIC_PLAIN) ||        \
          (CGALPY_TRI2 == CGALPY_TRI2_PERIODIC_DELAUNAY));
}

// Traits selection
template <int i, typename K> struct Tr { typedef K type; };
template <typename K> struct Tr<CGALPY_TRI2_PERIODIC_PLAIN, K>
{ typedef CGAL::Periodic_2_triangulation_traits_2<K> type; };
template <typename K> struct Tr<CGALPY_TRI2_PERIODIC_DELAUNAY, K>
{ typedef CGAL::Periodic_2_Delaunay_triangulation_traits_2<K> type; };

// Vertex base selection
template <bool b, int i, typename Tr> struct Vertex_base_name {};
template <typename Tr>
struct Vertex_base_name<true, 0, Tr>
{ typedef CGAL::Periodic_2_triangulation_vertex_base_2<Tr> type; };
template <int i, typename Tr>
struct Vertex_base_name<false, i, Tr>
{ typedef CGAL::Triangulation_vertex_base_2<Tr> type; };
template <typename Tr>
struct Vertex_base_name<false, CGALPY_TRI2_REGULAR, Tr>
{ typedef CGAL::Regular_triangulation_vertex_base_2<Tr> type; };

// Vertex with info
template <bool b, typename Vb, typename Data, typename Tr>
struct Vertex_with_info {};
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_info<false, Vb, Data, Tr> { typedef Vb type; };
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_info<true, Vb, Data, Tr>
{ typedef CGAL::Triangulation_vertex_base_with_info_2<Data, Tr, Vb> type; };

// Vertex triangulation hierarchy
template <bool b, typename Vb> struct Vertex_hierarchy {};
template <typename Vb> struct Vertex_hierarchy<false, Vb> { typedef Vb type; };
template <typename Vb> struct Vertex_hierarchy<true, Vb>
{ typedef CGAL::Triangulation_hierarchy_vertex_base_2<Vb> type; };

// Vertex alpha shape
template <bool b, typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape {};
template <typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<false, Vb, Tr, ExactComparison>
{ typedef Vb type; };
template <typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<true, Vb, Tr, ExactComparison>
{ typedef CGAL::Alpha_shape_vertex_base_2<Tr, Vb, ExactComparison> type; };

// Face base selection
template <bool b, int i, typename Tr> struct Face_base_name {};
template <typename Tr>
struct Face_base_name<true, 0, Tr>
{ typedef CGAL::Periodic_2_triangulation_face_base_2<Tr> type; };
template <int i, typename Tr>
struct Face_base_name<false, i, Tr>
{ typedef CGAL::Triangulation_face_base_2<Tr> type; };
template <typename Tr>
struct Face_base_name<false, CGALPY_TRI2_REGULAR, Tr>
{ typedef CGAL::Regular_triangulation_face_base_2<Tr> type; };

// Face with info
template <bool b, typename Fb, typename Data, typename Tr>
struct Face_with_info {};
template <typename Fb, typename Data, typename Tr>
struct Face_with_info<false, Fb, Data, Tr> { typedef Fb type; };
template <typename Fb, typename Data, typename Tr>
struct Face_with_info<true, Fb, Data, Tr>
{ typedef CGAL::Triangulation_face_base_with_info_2<Data, Tr, Fb> type; };

// Face alpha shape
template <bool b, typename Fb, typename Tr, typename ExactComparison>
struct Face_alpha_shape {};
template <typename Fb, typename Tr, typename ExactComparison>
struct Face_alpha_shape<false, Fb, Tr, ExactComparison>
{ typedef Fb type; };
template <typename Fb, typename Tr, typename ExactComparison>
struct Face_alpha_shape<true, Fb, Tr, ExactComparison>
{ typedef CGAL::Alpha_shape_face_base_2<Tr, Fb, ExactComparison> type; };

// Intersection tag
template <int i> struct Intersection_tag {};
template <> struct Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG_NCI>
{ typedef CGAL::No_constraint_intersection_tag type; };
template <> struct Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG_NCI_REQUIRING_CONSTRUCTIONS>
{ typedef CGAL::No_constraint_intersection_requiring_constructions_tag type; };
template <> struct Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG_EXACT_PREDICATES>
{ typedef CGAL::Exact_predicates_tag type; };
template <> struct Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG_EXACT_INTERSECTIONS>
{ typedef CGAL::Exact_intersections_tag type; };

// Main triangulation
template <int i, typename Tr, typename Tds, typename Itag>
struct Base_tri {};
template <typename Tr, typename Tds, typename Itag>
struct Base_tri<CGALPY_TRI2_PLAIN, Tr, Tds, Itag>
{ typedef CGAL::Triangulation_2<Tr, Tds> type; };
template <typename Tr, typename Tds, typename Itag>
struct Base_tri<CGALPY_TRI2_REGULAR, Tr, Tds, Itag>
{ typedef CGAL::Regular_triangulation_2<Tr, Tds> type; };
template <typename Tr, typename Tds, typename Itag>
struct Base_tri<CGALPY_TRI2_DELAUNAY, Tr, Tds, Itag>
{ typedef CGAL::Delaunay_triangulation_2<Tr, Tds> type; };
template <typename Tr, typename Tds, typename Itag>
struct Base_tri<CGALPY_TRI2_CONSTRAINED, Tr, Tds, Itag>
{ typedef CGAL::Constrained_triangulation_2<Tr, Tds, Itag> type; };
template <typename Tr, typename Tds, typename Itag>
struct Base_tri<CGALPY_TRI2_CONSTRAINED_DELAUNAY, Tr, Tds, Itag>
{ typedef CGAL::Constrained_Delaunay_triangulation_2<Tr, Tds, Itag> type; };
template <typename Tr, typename Tds, typename Itag>
struct Base_tri<CGALPY_TRI2_PERIODIC_PLAIN, Tr, Tds, Itag>
{ typedef CGAL::Periodic_2_triangulation_2<Tr, Tds> type; };
template <typename Tr, typename Tds, typename Itag>
struct Base_tri<CGALPY_TRI2_PERIODIC_DELAUNAY, Tr, Tds, Itag>
{ typedef CGAL::Periodic_2_Delaunay_triangulation_2<Tr, Tds> type; };

// Hierarchy
template <bool b, bool, typename Tr> struct Tri {};
template <typename Tr> struct Tri<false, false, Tr>
{ typedef Tr type; };
template <typename Tr> struct Tri<true, false, Tr>
{ typedef CGAL::Triangulation_hierarchy_2<Tr> type; };
template <typename Tr> struct Tri<true, true, Tr>
{ typedef CGAL::Periodic_2_triangulation_hierarchy_2<Tr> type; };

}

#endif
