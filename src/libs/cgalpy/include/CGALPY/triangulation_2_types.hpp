// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_2_TYPES_HPP
#define CGALPY_TRIANGULATION_2_TYPES_HPP

#include "CGALPY/triangulation_2_config.hpp"
#include "CGALPY/kernel_types.hpp"

#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_2.h>

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

// 2D triangulation traits
#if CGALPY_TRI2_TRAITS == CGALPY_TRI2_TRAITS_KERNEL
// Nothing to include
#elif CGALPY_TRI2_TRAITS == CGALPY_TRI2_TRAITS_PERIODIC_PLAIN
#include <CGAL/Periodic_2_triangulation_2.h>
#elif CGALPY_TRI2_TRAITS == CGALPY_TRI2_TRAITS_PERIODIC_DELAUNAY
#include <CGAL/Periodic_2_Delaunay_triangulation_traits_2.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI2_TRAITS");
#endif

// Indicates whether a triangulation hierarchy is selected
constexpr bool tri2_hierarchy() {
#ifdef CGALPY_TRI2_HIERARCHY
  return true;
#else
  return false;
#endif
}

// Indicates whether a vertex with infois selected
constexpr bool tri2_vertex_with_info() {
#ifdef CGALPY_TRI2_VERTEX_WITH_INFO
  return true;
#else
  return false;
#endif
}

// Indicates whether a vertex with infois selected
constexpr bool tri2_face_with_info() {
#ifdef CGALPY_TRI2_FACE_WITH_INFO
  return true;
#else
  return false;
#endif
}

// Indicates whether the selected triangulation is periodic
constexpr bool tri2_is_periodic() {
  return ((CGALPY_TRI2 == CGALPY_TRI2_PERIODIC_PLAIN) ||        \
          (CGALPY_TRI2 == CGALPY_TRI2_PERIODIC_DELAUNAY));
}

// 2D triangulation traits
#if CGALPY_TRI2_TRAITS == CGALPY_TRI2_TRAITS_KERNEL
typedef Kernel                                                     Traits;
#elif CGALPY_TRI2_TRAITS == CGALPY_TRI2_TRAITS_PERIODIC_PLAIN
typedef CGAL::Periodic_2_triangulation_traits_2<Kernel>            Traits;
#elif CGALPY_TRI2_TRAITS == CGALPY_TRI2_TRAITS_PERIODIC_DELAUNAY
typedef CGAL::Periodic_2_Delaunay_triangulation_traits_2<Kernel>   Traits;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI2_TRAITS");
#endif

// Vertex base selection
template <int i> struct Vertex_base_name {};
template <> struct Vertex_base_name<CGALPY_TRI2_VERTEX_BASE_PLAIN>
{ typedef CGAL::Triangulation_vertex_base_2<Kernel> type; };
template <> struct Vertex_base_name<CGALPY_TRI2_VERTEX_BASE_REGULAR>
{ typedef CGAL::Regular_triangulation_vertex_base_2<Kernel> type; };

// Vertex with info
template <bool b, typename Vb, typename Data> struct Vertex_with_info {};
template <typename Vb, typename Data> struct Vertex_with_info<false, Vb, Data>
{ typedef Vb type; };
template <typename Vb, typename Data> struct Vertex_with_info<true, Vb, Data>
{ typedef CGAL::Triangulation_vertex_base_with_info_2<Data, Kernel, Vb> type; };

//
template <bool b, typename Vb> struct Vertex_hierarchy {};
template <typename Vb> struct Vertex_hierarchy<false, Vb> { typedef Vb type; };
template <typename Vb> struct Vertex_hierarchy<true, Vb>
{ typedef CGAL::Triangulation_hierarchy_vertex_base_2<Vb> type; };

//
template <bool b, typename Vb, typename Traits> struct Vertex_periodic {};
template <typename Vb, typename Traits> struct Vertex_periodic<false, Vb, Traits>
{ typedef Vb type; };
template <typename Vb, typename Traits> struct Vertex_periodic<true, Vb, Traits>
{ typedef CGAL::Periodic_2_triangulation_vertex_base_2<Traits, Vb> type; };

//
template <bool b, typename Vb, typename Traits, typename ExactComparison>
struct Vertex_alpha_shape {};
template <typename Vb, typename Traits, typename ExactComparison>
struct Vertex_alpha_shape<false, Vb, Traits, ExactComparison>
{ typedef Vb type; };
template <typename Vb, typename Traits, typename ExactComparison>
struct Vertex_alpha_shape<true, Vb, Traits, ExactComparison>
{ typedef CGAL::Alpha_shape_vertex_base_2<Traits, Vb, ExactComparison> type; };

typedef Vertex_base_name<CGALPY_TRI2_VERTEX_BASE>::type                 Vb;
typedef Vertex_with_info<tri2_vertex_with_info(), Vb, bp::object>::type Vbi;
typedef Vertex_hierarchy<tri2_hierarchy(), Vbi>::type                   Vbih;
typedef Vertex_periodic<tri2_is_periodic(), Vbih, Traits>::type         Vbihp;

// Vertex base selection
template <int i> struct Face_base_name {};
template <> struct Face_base_name<CGALPY_TRI2_FACE_BASE_PLAIN>
{ typedef CGAL::Triangulation_face_base_2<Kernel> type; };
template <> struct Face_base_name<CGALPY_TRI2_FACE_BASE_REGULAR>
{ typedef CGAL::Regular_triangulation_face_base_2<Kernel> type; };

// Face with info
template <bool b, typename Cb, typename Data> struct Face_with_info {};
template <typename Cb, typename Data> struct Face_with_info<false, Cb, Data>
{ typedef Cb type; };
template <typename Cb, typename Data> struct Face_with_info<true, Cb, Data>
{ typedef CGAL::Triangulation_face_base_with_info_2<Data, Kernel, Cb> type; };

//
template <bool b, typename Cb, typename Traits> struct Face_periodic {};
template <typename Cb, typename Traits> struct Face_periodic<false, Cb, Traits>
{ typedef Cb type; };
template <typename Cb, typename Traits> struct Face_periodic<true, Cb, Traits>
{ typedef CGAL::Periodic_2_triangulation_face_base_2<Traits, Cb> type; };

//
template <bool b, typename Cb, typename Traits, typename ExactComparison>
struct Face_alpha_shape {};
template <typename Cb, typename Traits, typename ExactComparison>
struct Face_alpha_shape<false, Cb, Traits, ExactComparison>
{ typedef Cb type; };
template <typename Cb, typename Traits, typename ExactComparison>
struct Face_alpha_shape<true, Cb, Traits, ExactComparison>
{ typedef CGAL::Alpha_shape_face_base_2<Traits, Cb, ExactComparison> type; };

typedef Face_base_name<CGALPY_TRI2_FACE_BASE>::type                 Cb;
typedef Face_with_info<tri2_face_with_info(), Cb, bp::object>::type Cbi;
typedef Face_periodic<tri2_is_periodic(), Cbi, Traits>::type        Cbip;

#endif //CGALPY_TRIANGULATION_2_TYPES_HPP
