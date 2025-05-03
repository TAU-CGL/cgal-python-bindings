// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
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
#include <CGAL/Periodic_3_triangulation_traits_3.h>
#include <CGAL/Periodic_3_regular_triangulation_traits_3.h>
#include <CGAL/Periodic_3_Delaunay_triangulation_traits_3.h>

#include "CGALPY/config.hpp"
#include "CGALPY/alpha_shape_3_config.hpp"
#include "CGALPY/triangulation_3_values.hpp"

namespace tri3 {

// Indicates whether a triangulation hierarchy is selected
constexpr bool hierarchy() { return DETECT_EXIST(CGALPY_TRI3_HIERARCHY); }

// Indicates whether a vertex with infois selected
constexpr bool vertex_with_info()
{ return DETECT_EXIST(CGALPY_TRI3_VERTEX_WITH_INFO); }

// Indicates whether a vertex with infois selected
constexpr bool cell_with_info()
{ return DETECT_EXIST(CGALPY_TRI3_CELL_WITH_INFO); }

//! Indicates whether the selected triangulation is periodic
constexpr bool is_periodic() {
  return ((CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_PLAIN) ||        \
          (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR) ||      \
          (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_DELAUNAY));
}

//! Indicates whether the selected triangulation is regular
constexpr bool is_regular() {
  return ((CGALPY_TRI3 == CGALPY_TRI3_REGULAR) ||               \
          (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR));
}

//! Indicates whether the selected triangulation is Delaunay
constexpr bool is_delaunay() {
  return ((CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY) ||              \
          (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_DELAUNAY));
}

// Traits
template <int i, typename K> struct Tr { using type = K; };
template <typename K> struct Tr<CGALPY_TRI3_PERIODIC_PLAIN, K>
{ using type = CGAL::Periodic_3_triangulation_traits_3<K>; };
template <typename K> struct Tr<CGALPY_TRI3_PERIODIC_REGULAR, K>
{ using type = CGAL::Periodic_3_regular_triangulation_traits_3<K>; };
template <typename K> struct Tr<CGALPY_TRI3_PERIODIC_DELAUNAY, K>
{ using type = CGAL::Periodic_3_Delaunay_triangulation_traits_3<K>; };

// Vertex periodic triangulation
template <bool b> struct Vertex_periodic {};
template <> struct Vertex_periodic<false>
{ using type = CGAL::Triangulation_ds_vertex_base_3<>; };
template <> struct Vertex_periodic<true>
{ using type = CGAL::Periodic_3_triangulation_ds_vertex_base_3<>; };

// Vertex base selection
template <bool b, typename Vb, typename Tr> struct Vertex_base_name {};
template <typename Vb, typename Tr>
struct Vertex_base_name<true, Vb, Tr>
{ using type = CGAL::Regular_triangulation_vertex_base_3<Tr, Vb>; };
template <typename Vb, typename Tr>
struct Vertex_base_name<false, Vb, Tr>
{ using type = CGAL::Triangulation_vertex_base_3<Tr, Vb>; };

// Vertex with info
template <bool b, typename Vb, typename Data, typename Tr>
struct Vertex_with_info {};
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_info<false, Vb, Data, Tr> { using type = Vb; };
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_info<true, Vb, Data, Tr>
{ using type = CGAL::Triangulation_vertex_base_with_info_3<Data, Tr, Vb>; };

// Vertex triangulation hierarchy
template <bool b, typename Vb> struct Vertex_hierarchy {};
template <typename Vb> struct Vertex_hierarchy<false, Vb> { using type = Vb; };
template <typename Vb> struct Vertex_hierarchy<true, Vb>
{ using type = CGAL::Triangulation_hierarchy_vertex_base_3<Vb> ; };

// Vertex alpha shape
template <int i, typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape {};
template <typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<CGALPY_AS3_PLAIN, Vb, Tr, ExactComparison>
{ using type = CGAL::Alpha_shape_vertex_base_3<Tr, Vb, ExactComparison>; };
template <typename Vb, typename Tr, typename ExactComparison>
struct Vertex_alpha_shape<CGALPY_AS3_FIXED, Vb, Tr, ExactComparison>
{ using type = CGAL::Fixed_alpha_shape_vertex_base_3<Tr, Vb>; };

// Cell periodic triangulation
template <bool b> struct Cell_periodic {};
template <> struct Cell_periodic<false>
{ using type = CGAL::Triangulation_ds_cell_base_3<>; };
template <> struct Cell_periodic<true>
{ using type = CGAL::Periodic_3_triangulation_ds_cell_base_3<>; };

// Cell base selection
template <bool i, typename Cb, typename Tr> struct Cell_base_name {};
template <typename Cb, typename Tr>
struct Cell_base_name<true, Cb, Tr>
{ using type = CGAL::Regular_triangulation_cell_base_3<Tr, Cb>; };
template <typename Cb, typename Tr>
struct Cell_base_name<false, Cb, Tr>
{ using type = CGAL::Triangulation_cell_base_3<Tr, Cb>; };

// Cell with info
template <bool b, typename Fb, typename Data, typename Tr>
struct Cell_with_info {};
template <typename Fb, typename Data, typename Tr>
struct Cell_with_info<false, Fb, Data, Tr> { using type = Fb; };
template <typename Fb, typename Data, typename Tr>
struct Cell_with_info<true, Fb, Data, Tr>
{ using type = CGAL::Triangulation_cell_base_with_info_3<Data, Tr, Fb>; };

// Cell alpha shape
template <int i, typename Cb, typename Tr, typename ExactComparison>
struct Cell_alpha_shape {};
template <typename Cb, typename Tr, typename ExactComparison>
struct Cell_alpha_shape<CGALPY_AS3_PLAIN, Cb, Tr, ExactComparison>
{ using type = CGAL::Alpha_shape_cell_base_3<Tr, Cb, ExactComparison>; };
template <typename Cb, typename Tr, typename ExactComparison>
struct Cell_alpha_shape<CGALPY_AS3_FIXED, Cb, Tr, ExactComparison>
{ using type = CGAL::Alpha_shape_cell_base_3<Tr, Cb, ExactComparison>; };

// Concurrency
template <int i> struct Concurrency {};
template <> struct Concurrency<CGALPY_TRI3_CONCURRENCY_SEQUENTIAL>
{ using type = CGAL::Sequential_tag; };
template <> struct Concurrency<CGALPY_TRI3_CONCURRENCY_PARALLEL>
{ using type = CGAL::Parallel_tag; };

// Location policy
template <int i> struct Location_policy {};
template <> struct Location_policy<CGALPY_TRI3_LOCATION_POLICY_FAST>
{ using type = CGAL::Fast_location; };
template <> struct Location_policy<CGALPY_TRI3_LOCATION_POLICY_COMPACT>
{ using type = CGAL::Compact_location; };

// Base triangulation
template <int i, typename Tr, typename Tds, typename Lp>
struct Tri3 {};

template <typename Tr, typename Tds, typename Lp>
struct Tri3<CGALPY_TRI3_PLAIN, Tr, Tds, Lp> {
  using type = typename CGAL::Triangulation_3<Tr, Tds>;
  using base_type = type;
};

template <typename Tr, typename Tds, typename Lp>
struct Tri3<CGALPY_TRI3_REGULAR, Tr, Tds, Lp> {
  using type = typename CGAL::Regular_triangulation_3<Tr, Tds>;
  using base_type = typename type::Base_tr;
};

template <typename Tr, typename Tds, typename Lp>
struct Tri3<CGALPY_TRI3_DELAUNAY, Tr, Tds, Lp> {
  using type = typename CGAL::Delaunay_triangulation_3<Tr, Tds>;
  using base_type = typename type::Tr_Base;
};

template <typename Tr, typename Tds, typename Lp>
struct Tri3<CGALPY_TRI3_PERIODIC_PLAIN, Tr, Tds, Lp> {
  using type = typename CGAL::Periodic_3_triangulation_3<Tr, Tds>;
  using base_type = type;
};

template <typename Tr, typename Tds, typename Lp>
struct Tri3<CGALPY_TRI3_PERIODIC_REGULAR, Tr, Tds, Lp> {
  using type  = typename CGAL::Periodic_3_regular_triangulation_3<Tr, Tds>;
  using base_type = typename type::Tr_Base;
};

template <typename Tr, typename Tds, typename Lp>
struct Tri3<CGALPY_TRI3_PERIODIC_DELAUNAY, Tr, Tds, Lp> {
  using type = typename CGAL::Periodic_3_Delaunay_triangulation_3<Tr, Tds>;
  using base_type = typename type::Base;
};

// Hierarchy
template <bool b, typename Tr> struct Th {};
template <typename Tr> struct Th<false, Tr>
{ using type = Tr; };
template <typename Tr> struct Th<true, Tr>
{ using type = CGAL::Triangulation_hierarchy_3<Tr>; };

}

#endif
