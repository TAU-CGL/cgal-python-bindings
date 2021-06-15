// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_3_TYPES_HPP
#define CGALPY_TRIANGULATION_3_TYPES_HPP

#include "CGALPY/triangulation_3_config.hpp"
#include "CGALPY/alpha_shape_3_exact_comparison.hpp"
#include "CGALPY/kernel_types.hpp"

// 3D triangulation vertex-base type
#if ((CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_PLAIN_WITH_INFO) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_REGULAR_WITH_INFO) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#endif

#if ((CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR) || \
     (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Alpha_shape_vertex_base_3.h>
#elif ((CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR) || \
       (CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Fixed_alpha_shape_vertex_base_3.h>
#endif

// 3D triangulation cell-base type
#if ((CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_PLAIN_WITH_INFO) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_REGULAR_WITH_INFO) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#endif

#if ((CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE) ||    \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR) || \
     (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Alpha_shape_cell_base_3.h>
#elif ((CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR) || \
       (CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
#include <CGAL/Fixed_alpha_shape_cell_base_3.h>
#endif

// 3D triangulation traits
#if CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_KERNEL
// Nothing to include
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC_PLAIN
#include <CGAL/Periodic_3_triangulation_3.h>
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC_REGULAR
#include <CGAL/Periodic_3_regular_triangulation_traits_3.h>
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC_DELAUNAY
#include <CGAL/Periodic_3_Delaunay_triangulation_traits_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_TRAITS");
#endif

// 3D Triangulation
#if CGALPY_TRI3 == CGALPY_TRI3_PLAIN
#include <CGAL/Triangulation_3.h>
#elif CGALPY_TRI3 == CGALPY_TRI3_REGULAR
#include <CGAL/Regular_triangulation_3.h>
#elif CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
#include <CGAL/Delaunay_triangulation_3.h>
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_PLAIN
#include <CGAL/Periodic_3_triangulation_3.h>
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR
#include <CGAL/Periodic_3_regular_triangulation_3.h>
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_DELAUNAY
#include <CGAL/Periodic_3_Delaunay_triangulation_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3");
#endif

namespace tri3 {

// 3D triangulation vertex base
#if CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_PLAIN
typedef CGAL::Triangulation_vertex_base_3<Kernel>               Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_PLAIN_WITH_INFO
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_REGULAR
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel, Vb0> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE
typedef CGAL::Triangulation_vertex_base_3<Kernel>               Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0, as3::Exact_comparison> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel> Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0, as3::Exact_comparison> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Alpha_shape_vertex_base_3<Kernel, Vb0, as3::Exact_comparison> Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel, Vb0> Vb1;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb1>      Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE
typedef CGAL::Triangulation_vertex_base_3<Kernel>               Vb0;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb0>      Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel> Vb0;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb0>      Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb0>      Vertex_base;
#elif CGALPY_TRI3_VERTEX_BASE == CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_vertex_base_3<Kernel>       Vb0;
typedef CGAL::Triangulation_vertex_base_with_info_3<size_t, Kernel, Vb0> Vb1;
typedef CGAL::Fixed_alpha_shape_vertex_base_3<Kernel, Vb1>      Vertex_base;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_VERTEX_BASE");
#endif

// 3D triangulation cell base
#if CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_PLAIN
typedef CGAL::Triangulation_cell_base_3<Kernel>                 Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_PLAIN_WITH_INFO
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel> Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_REGULAR
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel, Cb0> Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE
typedef CGAL::Triangulation_cell_base_3<Kernel>                 Cb0;
  typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0, as3::Exact_comparison> Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel> Cb0;
typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0, as3::Exact_comparison> Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Alpha_shape_cell_base_3<Kernel, Cb0, as3::Exact_comparison> Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel, Cb0> Cb1;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb1>        Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE
typedef CGAL::Triangulation_cell_base_3<Kernel>                 Cb0;
typedef CGAL::Fiexed_alpha_shape_cell_base_3<Kernel, Cb0>       Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel> Cb0;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb0>        Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb0>        Cell_base;
#elif CGALPY_TRI3_CELL_BASE == CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO
typedef CGAL::Regular_triangulation_cell_base_3<Kernel>         Cb0;
typedef CGAL::Triangulation_cell_base_with_info_3<size_t, Kernel, Cb0> Cb1;
typedef CGAL::Fixed_alpha_shape_cell_base_3<Kernel, Cb1>        Cell_base;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_CELL_BASE");
#endif

// 3D triangulation concurrency
#if CGALPY_TRI3_CONCURRENCY == CGALPY_TRI3_CONCURRENCY_SEQUENTIAL
typedef CGAL::Sequential_tag                                       Concurrency_tag;
#elif CGALPY_TRI3_CONCURRENCY == CGALPY_TRI3_CONCURRENCY_PARALLEL
typedef CGAL::Parallel_tag                                         Concurrency_tag;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_CONCURRENCY");
#endif

typedef CGAL::Triangulation_data_structure_3<Vertex_base, Cell_base, Concurrency_tag> Tds;

// 3D triangulation traits
#if CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_KERNEL
typedef Kernel                                                     Traits;
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC_PLAIN
typedef CGAL::Periodic_3_triangulation_traits_3<Kernel>            Traits;
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC_REGULAR
typedef CGAL::Periodic_3_regular_triangulation_traits_3<Kernel>    Traits;
#elif CGALPY_TRI3_TRAITS == CGALPY_TRI3_TRAITS_PERIODIC_DELAUNAY
typedef CGAL::Periodic_3_Delaunay_triangulation_traits_3<Kernel>   Traits;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_TRAITS");
#endif

// 3D triangulation location policy
#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_FAST
typedef CGAL::Fast_location                                        Location_policy;
#elif CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
typedef CGAL::Compact_location                                     Location_policy;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_LOCATION_POLICY");
#endif
#endif

// 3D triangulation
#if CGALPY_TRI3 == CGALPY_TRI3_PLAIN
typedef CGAL::Triangulation_3<Traits, Tds>                           Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_REGULAR
typedef CGAL::Regular_triangulation_3<Traits, Tds>                   Triangulation_3;
typedef Triangulation_3::Weighted_point                              Weighted_point;
typedef Triangulation_3::Bare_point                                  Bare_point;
#elif CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
typedef CGAL::Delaunay_triangulation_3<Traits, Tds, Location_policy> Delaunay_triangulation_3;
typedef Delaunay_triangulation_3                                     Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_PLAIN
typedef CGAL::Periodic_3_triangulation_3<Traits, Tds>                Triangulation_3;
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR
typedef CGAL::Periodic_3_regular_triangulation_3<Traits, Tds>        Triangulation_3;
typedef Triangulation_3::Weighted_point                              Weighted_point;
typedef Triangulation_3::Bare_point                                  Bare_point;
#elif CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_DELAUNAY
typedef CGAL::Periodic_3_Delaunay_triangulation_3<Traits, Tds>       Delaunay_triangulation_3;
typedef Delaunay_triangulation_3                                     Triangulation_3;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3");
#endif

typedef Triangulation_3::Point          Point;
typedef Triangulation_3::Vertex         Vertex;
typedef Triangulation_3::Cell           Cell;
typedef Triangulation_3::Facet          Facet;
typedef Triangulation_3::Edge           Edge;

typedef Triangulation_3::Vertex_handle  Vertex_handle;
typedef Triangulation_3::Cell_handle    Cell_handle;

typedef Triangulation_3::Locate_type    Locate_type;

} // End of namespace tri3

#endif
