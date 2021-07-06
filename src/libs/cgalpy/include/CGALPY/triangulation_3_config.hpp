// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_3_CONFIG_HPP
#define CGALPY_TRIANGULATION_3_CONFIG_HPP

#include <CGAL/Triangulation_vertex_base_3.h>
#include <CGAL/Regular_triangulation_vertex_base_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <CGAL/Periodic_3_triangulation_ds_vertex_base_3.h>
#include <CGAL/Triangulation_hierarchy_vertex_base_3.h>
#include <CGAL/Alpha_shape_vertex_base_3.h>
#include <CGAL/Fixed_alpha_shape_vertex_base_3.h>

#include <CGAL/Triangulation_cell_base_3.h>
#include <CGAL/Regular_triangulation_cell_base_3.h>
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#include <CGAL/Periodic_3_triangulation_ds_cell_base_3.h>
#include <CGAL/Alpha_shape_cell_base_3.h>
#include <CGAL/Fixed_alpha_shape_cell_base_3.h>

#include <CGAL/Triangulation_data_structure_3.h>

#include <CGAL/Triangulation_3.h>
#include <CGAL/Regular_triangulation_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Periodic_3_triangulation_3.h>
#include <CGAL/Periodic_3_regular_triangulation_3.h>
#include <CGAL/Periodic_3_Delaunay_triangulation_3.h>

#include <CGAL/Triangulation_hierarchy_3.h>
#include <CGAL/Periodic_3_triangulation_3.h>
#include <CGAL/Periodic_3_regular_triangulation_traits_3.h>
#include <CGAL/Periodic_3_Delaunay_triangulation_traits_3.h>

#include "CGALPY/config.hpp"
#include "CGALPY/alpha_shape_3_config.hpp"

#define CGALPY_TRI3_VERTEX_BASE_PLAIN                   0
#define CGALPY_TRI3_VERTEX_BASE_REGULAR                 1

#define CGALPY_TRI3_CELL_BASE_PLAIN                     0
#define CGALPY_TRI3_CELL_BASE_REGULAR                   1

#define CGALPY_TRI3_CONCURRENCY_SEQUENTIAL              0
#define CGALPY_TRI3_CONCURRENCY_PARALLEL                1

#define CGALPY_TRI3_TRAITS_KERNEL                       0
#define CGALPY_TRI3_TRAITS_PERIODIC_PLAIN               1
#define CGALPY_TRI3_TRAITS_PERIODIC_REGULAR             2
#define CGALPY_TRI3_TRAITS_PERIODIC_DELAUNAY            3

#define CGALPY_TRI3_LOCATION_POLICY_FAST                0
#define CGALPY_TRI3_LOCATION_POLICY_COMPACT             1

#define CGALPY_TRI3_PLAIN                               0
#define CGALPY_TRI3_REGULAR                             1
#define CGALPY_TRI3_DELAUNAY                            2
#define CGALPY_TRI3_PERIODIC_PLAIN                      3
#define CGALPY_TRI3_PERIODIC_REGULAR                    4
#define CGALPY_TRI3_PERIODIC_DELAUNAY                   5

namespace tri3 {

// Indicates whether a triangulation hierarchy is selected
constexpr bool hierarchy() { return DETECT_EXIST(CGALPY_TRI3_HIERARCHY); }

// Indicates whether a vertex with infois selected
constexpr bool vertex_with_info()
{ return DETECT_EXIST(CGALPY_TRI3_VERTEX_WITH_INFO); }

// Indicates whether a vertex with infois selected
constexpr bool cell_with_info()
{ return DETECT_EXIST(CGALPY_TRI3_CELL_WITH_INFO); }

// Indicates whether the selected triangulation is periodic
constexpr bool is_periodic() {
  return ((CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_PLAIN) ||        \
          (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR) ||      \
          (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_DELAUNAY));
}

// Traits
  template <int i, typename K> struct Tr {};
  template <typename K> struct Tr<CGALPY_TRI3_TRAITS_KERNEL, K>
{ typedef K type; };
template <typename K> struct Tr<CGALPY_TRI3_TRAITS_PERIODIC_PLAIN, K>
{ typedef CGAL::Periodic_3_triangulation_traits_3<K> type; };
template <typename K> struct Tr<CGALPY_TRI3_TRAITS_PERIODIC_REGULAR, K>
{ typedef CGAL::Periodic_3_regular_triangulation_traits_3<K> type; };
template <typename K> struct Tr<CGALPY_TRI3_TRAITS_PERIODIC_DELAUNAY, K>
{ typedef CGAL::Periodic_3_Delaunay_triangulation_traits_3<K> type; };

// Vertex base selection
template <int i, typename Tr> struct Vertex_base_name {};
template <typename Tr>
struct Vertex_base_name<CGALPY_TRI3_VERTEX_BASE_PLAIN, Tr>
{ typedef CGAL::Triangulation_vertex_base_3<Tr> type; };
template <typename Tr>
struct Vertex_base_name<CGALPY_TRI3_VERTEX_BASE_REGULAR, Tr>
{ typedef CGAL::Regular_triangulation_vertex_base_3<Tr> type; };

// Vertex with info
template <bool b, typename Vb, typename Data, typename Tr>
struct Vertex_with_info {};
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_info<false, Vb, Data, Tr> { typedef Vb type; };
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_info<true, Vb, Data, Tr>
{ typedef CGAL::Triangulation_vertex_base_with_info_3<Data, Tr, Vb> type; };

// Vertex triangulation hierarchy
template <bool b, typename Vb> struct Vertex_hierarchy {};
template <typename Vb> struct Vertex_hierarchy<false, Vb> { typedef Vb type; };
template <typename Vb> struct Vertex_hierarchy<true, Vb>
{ typedef CGAL::Triangulation_hierarchy_vertex_base_3<Vb> type; };

// // Vertex periodic triangulation
// template <bool b, typename Vb, typename Tr> struct Vertex_periodic {};
// template <typename Vb, typename Tr> struct Vertex_periodic<false, Vb, Tr>
// { typedef Vb type; };
// template <typename Vb, typename Tr> struct Vertex_periodic<true, Vb, Tr>
// { typedef CGAL::Periodic_3_triangulation_ds_vertex_base_3<Tr, Vb> type; };

// Vertex alpha shape
template <bool b, int i, typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape {};
template <int i, typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<false, i, Vb, Tr, ExactComparison>
{ typedef Vb type; };
template <typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<true, CGALPY_AS3_PLAIN, Vb, Tr, ExactComparison>
{ typedef CGAL::Alpha_shape_vertex_base_3<Tr, Vb, ExactComparison> type; };
template <typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<true, CGALPY_AS3_FIXED, Vb, Tr, ExactComparison>
{ typedef CGAL::Fixed_alpha_shape_vertex_base_3<Tr, Vb> type; };

// Cell base selection
template <int i, typename Tr> struct Cell_base_name {};
template <typename Tr>
struct Cell_base_name<CGALPY_TRI3_CELL_BASE_PLAIN, Tr>
{ typedef CGAL::Triangulation_cell_base_3<Tr> type; };
template <typename Tr>
struct Cell_base_name<CGALPY_TRI3_CELL_BASE_REGULAR, Tr>
{ typedef CGAL::Regular_triangulation_cell_base_3<Tr> type; };

// Cell with info
template <bool b, typename Fb, typename Data, typename Tr>
struct Cell_with_info {};
template <typename Fb, typename Data, typename Tr>
struct Cell_with_info<false, Fb, Data, Tr> { typedef Fb type; };
template <typename Fb, typename Data, typename Tr>
struct Cell_with_info<true, Fb, Data, Tr>
{ typedef CGAL::Triangulation_cell_base_with_info_3<Data, Tr, Fb> type; };

// // Cell periodic triangulation
// template <bool b, typename Fb, typename Tr> struct Cell_periodic {};
// template <typename Fb, typename Tr> struct Cell_periodic<false, Fb, Tr>
// { typedef Fb type; };
// template <typename Fb, typename Tr> struct Cell_periodic<true, Fb, Tr>
// { typedef CGAL::Periodic_3_triangulation_ds_cell_base_3<Tr, Fb> type; };

// Cell alpha shape
template <bool b, int i, typename Fb, typename Tr, typename ExactComparison>
struct Cell_alpha_shape {};
template <int i, typename Fb, typename Tr, typename ExactComparison>
struct Cell_alpha_shape<false, i, Fb, Tr, ExactComparison>
{ typedef Fb type; };
template <typename Fb, typename Tr, typename ExactComparison>
struct Cell_alpha_shape<true, CGALPY_AS3_PLAIN, Fb, Tr, ExactComparison>
{ typedef CGAL::Alpha_shape_cell_base_3<Tr, Fb, ExactComparison> type; };
template <typename Fb, typename Tr, typename ExactComparison>
struct Cell_alpha_shape<true, CGALPY_AS3_FIXED, Fb, Tr, ExactComparison>
{ typedef CGAL::Alpha_shape_cell_base_3<Tr, Fb, ExactComparison> type; };

// Concurrency
template <int i> struct Concurrency {};
template <> struct Concurrency<CGALPY_TRI3_CONCURRENCY_SEQUENTIAL>
{ typedef CGAL::Sequential_tag type; };
template <> struct Concurrency<CGALPY_TRI3_CONCURRENCY_PARALLEL>
{ typedef CGAL::Parallel_tag type; };

// Location policy
template <int i> struct Location_policy {};
template <> struct Location_policy<CGALPY_TRI3_LOCATION_POLICY_FAST>
{ typedef CGAL::Fast_location type; };
template <> struct Location_policy<CGALPY_TRI3_LOCATION_POLICY_COMPACT>
{ typedef CGAL::Compact_location type; };

// Main triangulation
template <int i, typename Tr, typename Tds, typename Lp>
struct Base_tri {};
template <typename Tr, typename Tds, typename Lp>
struct Base_tri<CGALPY_TRI3_PLAIN, Tr, Tds, Lp>
{ typedef CGAL::Triangulation_3<Tr, Tds> type; };
template <typename Tr, typename Tds, typename Lp>
struct Base_tri<CGALPY_TRI3_REGULAR, Tr, Tds, Lp>
{ typedef CGAL::Regular_triangulation_3<Tr, Tds> type; };
template <typename Tr, typename Tds, typename Lp>
struct Base_tri<CGALPY_TRI3_DELAUNAY, Tr, Tds, Lp>
{ typedef CGAL::Delaunay_triangulation_3<Tr, Tds> type; };
template <typename Tr, typename Tds, typename Lp>
struct Base_tri<CGALPY_TRI3_PERIODIC_PLAIN, Tr, Tds, Lp>
{ typedef CGAL::Periodic_3_triangulation_3<Tr, Tds> type; };
template <typename Tr, typename Tds, typename Lp>
struct Base_tri<CGALPY_TRI3_PERIODIC_DELAUNAY, Tr, Tds, Lp>
{ typedef CGAL::Periodic_3_Delaunay_triangulation_3<Tr, Tds> type; };

// Hierarchy
template <bool b, typename Tr> struct Tri {};
template <typename Tr> struct Tri<false, Tr>
{ typedef Tr type; };
template <typename Tr> struct Tri<true, Tr>
{ typedef CGAL::Triangulation_hierarchy_3<Tr> type; };

}

#endif
