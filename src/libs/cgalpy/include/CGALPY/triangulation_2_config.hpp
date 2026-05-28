// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
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
#include <CGAL/Periodic_2_triangulation_traits_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_traits_2.h>

#include "CGALPY/config.hpp"
#include "CGALPY/alpha_shape_2_config.hpp"
#include "CGALPY/triangulation_2_values.hpp"

namespace cgalpy {
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

// Indicates whether the selected triangulation is constrained
constexpr bool is_constrained() {
  return ((CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED) ||        \
          (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY));
}

// Traits selection
template <int i, typename K> struct Tr { using type = K; };
template <typename K> struct Tr<CGALPY_TRI2_PERIODIC_PLAIN, K>
{ using type = CGAL::Periodic_2_triangulation_traits_2<K>; };
template <typename K> struct Tr<CGALPY_TRI2_PERIODIC_DELAUNAY, K>
{ using type = CGAL::Periodic_2_Delaunay_triangulation_traits_2<K>; };

// Vertex base selection
template <bool b, int i, typename Tr> struct Vertex_base_name {};
template <typename Tr>
struct Vertex_base_name<true, 0, Tr>
{ using type = CGAL::Periodic_2_triangulation_vertex_base_2<Tr>; };
template <int i, typename Tr>
struct Vertex_base_name<false, i, Tr>
{ using type = CGAL::Triangulation_vertex_base_2<Tr>; };
template <typename Tr>
struct Vertex_base_name<false, CGALPY_TRI2_REGULAR, Tr>
{ using type = CGAL::Regular_triangulation_vertex_base_2<Tr>; };

// Vertex with info
template <bool b, typename Vb, typename Data, typename Tr>
struct Vertex_with_info {};
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_info<false, Vb, Data, Tr> { using type = Vb; };
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_info<true, Vb, Data, Tr>
{ using type = CGAL::Triangulation_vertex_base_with_info_2<Data, Tr, Vb>; };

// Vertex triangulation hierarchy
template <bool b, typename Vb> struct Vertex_hierarchy {};
template <typename Vb> struct Vertex_hierarchy<false, Vb> { using type = Vb; };
template <typename Vb> struct Vertex_hierarchy<true, Vb>
{ using type = CGAL::Triangulation_hierarchy_vertex_base_2<Vb>; };

// Vertex alpha shape
template <bool b, typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape {};
template <typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<false, Vb, Tr, ExactComparison>
{ using type = Vb; };
template <typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<true, Vb, Tr, ExactComparison>
{ using type = CGAL::Alpha_shape_vertex_base_2<Tr, Vb, ExactComparison>; };

// Face base selection
template <bool b, int i, typename Tr> struct Face_base_name {};
template <typename Tr>
struct Face_base_name<true, 0, Tr>
{ using type = CGAL::Periodic_2_triangulation_face_base_2<Tr>; };
template <int i, typename Tr>
struct Face_base_name<false, i, Tr>
{ using type = CGAL::Triangulation_face_base_2<Tr>; };
template <typename Tr>
struct Face_base_name<false, CGALPY_TRI2_REGULAR, Tr>
{ using type = CGAL::Regular_triangulation_face_base_2<Tr>; };

// Face with info
template <bool b, typename Fb, typename Data, typename Tr>
struct Face_with_info {};
template <typename Fb, typename Data, typename Tr>
struct Face_with_info<false, Fb, Data, Tr> { using type = Fb; };
template <typename Fb, typename Data, typename Tr>
struct Face_with_info<true, Fb, Data, Tr>
{ using type = CGAL::Triangulation_face_base_with_info_2<Data, Tr, Fb>; };

// Face constrianed
template <bool b, typename Fb, typename Tr>
struct Face_constrained {};
template <typename Fb, typename Tr>
struct Face_constrained<false, Fb, Tr> { using type = Fb; };
template <typename Fb, typename Tr>
struct Face_constrained<true, Fb, Tr>
{ using type = CGAL::Constrained_triangulation_face_base_2<Tr, Fb>; };

// Face alpha shape
template <bool b, typename Fb, typename Tr, typename ExactComparison>
struct Face_alpha_shape {};
template <typename Fb, typename Tr, typename ExactComparison>
struct Face_alpha_shape<false, Fb, Tr, ExactComparison>
{ using type = Fb; };
template <typename Fb, typename Tr, typename ExactComparison>
struct Face_alpha_shape<true, Fb, Tr, ExactComparison>
{ using type = CGAL::Alpha_shape_face_base_2<Tr, Fb, ExactComparison>; };

// Intersection tag
template <int i> struct Intersection_tag {};
template <> struct Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG_NCI>
{ using type = CGAL::No_constraint_intersection_tag; };
template <> struct Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG_NCI_REQUIRING_CONSTRUCTIONS>
{ using type = CGAL::No_constraint_intersection_requiring_constructions_tag; };
template <> struct Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG_EXACT_PREDICATES>
{ using type = CGAL::Exact_predicates_tag; };
template <> struct Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG_EXACT_INTERSECTIONS>
{ using type = CGAL::Exact_intersections_tag; };

// Main triangulation
template <int i, typename Tr, typename Tds, typename Itag>
struct Tri2 {};

template <typename Tr, typename Tds, typename Itag>
struct Tri2<CGALPY_TRI2_PLAIN, Tr, Tds, Itag> {
  using Triangulation_2 = CGAL::Triangulation_2<Tr, Tds>;
};

template <typename Tr, typename Tds, typename Itag>
struct Tri2<CGALPY_TRI2_REGULAR, Tr, Tds, Itag> {
  using Regular_triangulation_2 = CGAL::Regular_triangulation_2<Tr, Tds>;
  using Triangulation_2 = typename Regular_triangulation_2::Triangulation_base;
};

template <typename Tr, typename Tds, typename Itag>
struct Tri2<CGALPY_TRI2_DELAUNAY, Tr, Tds, Itag> {
  using Delaunay_triangulation_2 = CGAL::Delaunay_triangulation_2<Tr, Tds>;
  using Triangulation_2 = typename Delaunay_triangulation_2::Triangulation;
};

template <typename Tr, typename Tds, typename Itag>
struct Tri2<CGALPY_TRI2_CONSTRAINED, Tr, Tds, Itag> {
  using Constrained_triangulation_2 = CGAL::Constrained_triangulation_2<Tr, Tds, Itag>;
  using Triangulation_2 = typename Constrained_triangulation_2::Triangulation;
};

template <typename Tr, typename Tds, typename Itag>
struct Tri2<CGALPY_TRI2_CONSTRAINED_DELAUNAY, Tr, Tds, Itag> {
  using Constrained_delaunay_triangulation_2 = CGAL::Constrained_Delaunay_triangulation_2<Tr, Tds, Itag>;
  using Constrained_triangulation_2 = typename Constrained_delaunay_triangulation_2::Base;
  using Triangulation_2 = typename Constrained_triangulation_2::Triangulation;
};

template <typename Tr, typename Tds, typename Itag>
struct Tri2<CGALPY_TRI2_PERIODIC_PLAIN, Tr, Tds, Itag> {
  using Periodic_2_triangulation_2 = CGAL::Periodic_2_triangulation_2<Tr, Tds>;
};

template <typename Tr, typename Tds, typename Itag>
struct Tri2<CGALPY_TRI2_PERIODIC_DELAUNAY, Tr, Tds, Itag> {
  using Periodic_2_delaunay_triangulation_2 = CGAL::Periodic_2_Delaunay_triangulation_2<Tr, Tds>;
  using Periodic_2_triangulation_2 = typename Periodic_2_delaunay_triangulation_2::Base;
};

// Hierarchy
template <bool b, bool, typename Tr> struct Th {};
template <typename Tr> struct Th<false, false, Tr>
{ using type = Tr; };
template <typename Tr> struct Th<true, false, Tr>
{ using type = CGAL::Triangulation_hierarchy_2<Tr>; };
template <typename Tr> struct Th<true, true, Tr>
{ using type = CGAL::Periodic_2_triangulation_hierarchy_2<Tr>; };

}
} // namespace cgalpy

#endif
