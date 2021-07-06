// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_2_TYPES_HPP
#define CGALPY_TRIANGULATION_2_TYPES_HPP

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
#include <CGAL/Periodic_2_triangulation_2.h>
#include <CGAL/Periodic_2_Delaunay_triangulation_traits_2.h>

#include "CGALPY/config.hpp"
#include "CGALPY/triangulation_2_config.hpp"
#include "CGALPY/alpha_shape_2_config.hpp"
#include "CGALPY/kernel_types.hpp"

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

// Traits
template <int i, typename K> struct Tr {};
template <typename K> struct Tr<CGALPY_TRI2_TRAITS_KERNEL, K>
{ typedef K type; };
template <typename K> struct Tr<CGALPY_TRI2_TRAITS_PERIODIC_PLAIN, K>
{ typedef CGAL::Periodic_2_triangulation_traits_2<K> type; };
template <typename K> struct Tr<CGALPY_TRI2_TRAITS_PERIODIC_DELAUNAY, K>
{ typedef CGAL::Periodic_2_Delaunay_triangulation_traits_2<K> type; };

// Vertex base selection
template <int i, typename Tr> struct Vertex_base_name {};
template <typename Tr>
struct Vertex_base_name<CGALPY_TRI2_VERTEX_BASE_PLAIN, Tr>
{ typedef CGAL::Triangulation_vertex_base_2<Tr> type; };
template <typename Tr>
struct Vertex_base_name<CGALPY_TRI2_VERTEX_BASE_REGULAR, Tr>
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

// Vertex periodic triangulation
template <bool b, typename Vb, typename Tr> struct Vertex_periodic {};
template <typename Vb, typename Tr> struct Vertex_periodic<false, Vb, Tr>
{ typedef Vb type; };
template <typename Vb, typename Tr> struct Vertex_periodic<true, Vb, Tr>
{ typedef CGAL::Periodic_2_triangulation_vertex_base_2<Tr, Vb> type; };

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
template <int i, typename Tr> struct Face_base_name {};
template <typename Tr>
struct Face_base_name<CGALPY_TRI2_FACE_BASE_PLAIN, Tr>
{ typedef CGAL::Triangulation_face_base_2<Tr> type; };
template <typename Tr>
struct Face_base_name<CGALPY_TRI2_FACE_BASE_REGULAR, Tr>
{ typedef CGAL::Regular_triangulation_face_base_2<Tr> type; };

// Face with info
template <bool b, typename Fb, typename Data, typename Tr>
struct Face_with_info {};
template <typename Fb, typename Data, typename Tr>
struct Face_with_info<false, Fb, Data, Tr> { typedef Fb type; };
template <typename Fb, typename Data, typename Tr>
struct Face_with_info<true, Fb, Data, Tr>
{ typedef CGAL::Triangulation_face_base_with_info_2<Data, Tr, Fb> type; };

// Face periodic triangulation
template <bool b, typename Fb, typename Tr> struct Face_periodic {};
template <typename Fb, typename Tr> struct Face_periodic<false, Fb, Tr>
{ typedef Fb type; };
template <typename Fb, typename Tr> struct Face_periodic<true, Fb, Tr>
{ typedef CGAL::Periodic_2_triangulation_face_base_2<Tr, Fb> type; };

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
template <bool b, typename Tr> struct Tri {};
template <typename Tr> struct Tri<false, Tr>
{ typedef Tr type; };
template <typename Tr> struct Tri<true, Tr>
{ typedef CGAL::Triangulation_hierarchy_2<Tr> type; };

// Final types
typedef as2::Exact_comparison<as2::exact_comparison()>::type    Ec;

typedef Tr<CGALPY_TRI2_TRAITS, Kernel>::type                    Traits;

typedef Vertex_base_name<CGALPY_TRI2_VERTEX_BASE, Traits>::type Vb;
typedef Vertex_with_info<vertex_with_info(), Vb, bp::object, Traits>::type
                                                                Vbi;
typedef Vertex_alpha_shape<alpha_shape_2_bindings(), Vbi, Traits, Ec>::type
                                                                Vbia;
typedef Vertex_hierarchy<hierarchy(), Vbia>::type               Vbiah;
typedef Vertex_periodic<is_periodic(), Vbiah, Traits>::type     V;

typedef Face_base_name<CGALPY_TRI2_FACE_BASE, Traits>::type     Fb;
typedef Face_with_info<face_with_info(), Fb, bp::object, Traits>::type
                                                                Fbi;
typedef Face_alpha_shape<alpha_shape_2_bindings(), Fbi, Traits, Ec>::type
                                                                Fbia;
typedef Face_periodic<is_periodic(), Fbia, Traits>::type        F;

typedef CGAL::Triangulation_data_structure_2<V, F>              Tds;
typedef Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG>::type    Itag;
typedef Base_tri<CGALPY_TRI2, Traits, Tds, Itag>::type          Btr;
typedef Tri<hierarchy(), Btr>::type                             Triangulation_2;

typedef Triangulation_2::Point                        Point;
typedef Triangulation_2::Segment                      Segment;
typedef Triangulation_2::Triangle                     Triangle;
typedef Triangulation_2::Vertex                       Vertex;
typedef Triangulation_2::Face                         Face;
typedef Triangulation_2::Edge                         Edge;
typedef Triangulation_2::size_type                    size_type;
typedef Triangulation_2::difference_type              difference_type;

typedef Triangulation_2::Vertex_handle                Vertex_handle;
typedef Triangulation_2::Face_handle                  Face_handle;

typedef Triangulation_2::Vertex_circulator            Vertex_circulator;
typedef Triangulation_2::All_vertices_iterator        All_vertices_iterator;
typedef Triangulation_2::Finite_vertices_iterator     Finite_vertices_iterator;
typedef Triangulation_2::Edge_circulator              Edge_circulator;
typedef Triangulation_2::All_edges_iterator           All_edges_iterator;
typedef Triangulation_2::Finite_edges_iterator        Finite_edges_iterator;
typedef Triangulation_2::Face_circulator              Face_circulator;
typedef Triangulation_2::Face_handle                  Face_handle;
typedef Triangulation_2::All_faces_iterator           All_faces_iterator;
typedef Triangulation_2::Finite_faces_iterator        Finite_faces_iterator;
typedef Triangulation_2::Point_iterator               Point_iterator;

} // End of namespace tri2

#endif //CGALPY_TRIANGULATION_2_TYPES_HPP
