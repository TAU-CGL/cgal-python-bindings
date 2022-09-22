// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARRANGEMENT_ON_SURFACE_2_CONFIG_HPP
#define CGALPY_ARRANGEMENT_ON_SURFACE_2_CONFIG_HPP

#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_non_caching_segment_traits_2.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arrangement_on_surface_with_history_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arrangement_with_history_2.h>
#include <CGAL/Arr_spherical_topology_traits_2.h>
#include <CGAL/Gps_traits_2.h>
#include <CGAL/Gps_segment_traits_2.h>
#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/Boolean_set_operations_2/Gps_default_dcel.h>

#include "CGALPY/config.hpp"
#include "CGALPY/kernel_types.hpp"

// Arrangement type options
#define CGALPY_AOS2_ARRANGEMENT                         0
#define CGALPY_AOS2_ARRANGEMENT_ON_SURFACE              1

#ifndef CGALPY_AOS2_TYPE
#define CGALPY_AOS2_TYPE                                0
#endif

// Geometry traits options
#define CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS                      0
#define CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS                     1
#define CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS         2
#define CGALPY_AOS2_CONIC_GEOMETRY_TRAITS                       3
#define CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS           4
#define CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS              5
#define CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS                      6
#define CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS           7
#define CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS      8

#ifndef CGALPY_AOS2_GEOMETRY_TRAITS
#define CGALPY_AOS2_GEOMETRY_TRAITS                             1
#endif

namespace aos2 {

// Indicates whether the vertex type is extended
constexpr bool is_vertex_extended()
{ return DETECT_EXIST(CGALPY_AOS2_VERTEX_EXTENDED); }

// Indicates whether the halfedge type is extended
constexpr bool is_halfedge_extended()
{ return DETECT_EXIST(CGALPY_AOS2_HALFEDGE_EXTENDED); }

// Indicates whether the halfedge type is extended
constexpr bool is_face_extended()
{ return DETECT_EXIST(CGALPY_AOS2_FACE_EXTENDED); }

// Boolean set operation

template <bool b, typename BaseTr> struct Tr {};
template <typename Base>
struct Tr<false, Base> { typedef Base type; };
template <>
struct Tr<true, CGAL::Arr_segment_traits_2<Kernel>> {
  typedef CGAL::Gps_segment_traits_2<Kernel, Point_2_container> type;
};
template <>
struct Tr<true, CGAL::Arr_non_caching_segment_traits_2<Kernel>> {
  typedef CGAL::Gps_segment_traits_2<Kernel, Point_2_container> type;
};
template <>
struct Tr<true, CGAL::Arr_circle_segment_traits_2<Kernel>> {
  typedef CGAL::Gps_circle_segment_traits_2<Kernel> type;
};
template <typename Base>
struct Tr<true, Base> { typedef CGAL::Gps_traits_2<Base> type; };

template <bool b, typename Tr> struct Halfedge_gps {};
template <typename Tr>
struct Halfedge_gps<false, Tr>
{ typedef CGAL::Arr_halfedge_base<typename Tr::X_monotone_curve_2> type; };
template <typename Tr>
struct Halfedge_gps<true, Tr>
{ typedef CGAL::Gps_halfedge_base<typename Tr::X_monotone_curve_2> type; };

template <bool b> struct Face_gps {};
template <> struct Face_gps<false> { typedef CGAL::Arr_face_base type; };
template <> struct Face_gps<true> { typedef CGAL::Gps_face_base type; };

// General extension

template <bool b, typename Vb, typename Data> struct Vertex_extended {};
template <typename Vb, typename Data>
struct Vertex_extended<false, Vb, Data> { typedef Vb type; };
template <typename Vb, typename Data>
struct Vertex_extended<true, Vb, Data>
{ typedef CGAL::Arr_extended_vertex<Vb, Data> type; };

template <bool b, typename Hb, typename Data> struct Halfedge_extended {};
template <typename Hb, typename Data>
struct Halfedge_extended<false, Hb, Data> { typedef Hb type; };
template <typename Hb, typename Data> struct Halfedge_extended<true, Hb, Data>
{ typedef CGAL::Arr_extended_halfedge<Hb, Data> type; };

template <bool b, typename Fb, typename Data> struct Face_extended {};
template <typename Fb, typename Data>
struct Face_extended<false, Fb, Data> { typedef Fb type; };
template <typename Fb, typename Data> struct Face_extended<true, Fb, Data>
{ typedef CGAL::Arr_extended_face<Fb, Data> type; };

// Aos type
template <int i, typename GeomTraits, typename Dcel> struct Aos {
  typedef typename CGAL::Default_planar_topology<GeomTraits, Dcel>::Traits      Topol_traits;
  typedef CGAL::Arrangement_on_surface_2<GeomTraits, Topol_traits>              aos;
  typedef CGAL::Arrangement_2<GeomTraits, Dcel>                                 arr;
  typedef CGAL::Arrangement_with_history_2<GeomTraits, Dcel>                    arr_with_history;
};
template <typename GeomTraits, typename Dcel>
struct Aos<CGALPY_AOS2_ARRANGEMENT_ON_SURFACE, GeomTraits, Dcel> {
  typedef CGAL::Arr_spherical_topology_traits_2<GeomTraits, Dcel>               Topol_traits;
  typedef CGAL::Arrangement_on_surface_2<GeomTraits, Topol_traits>              aos;
  typedef CGAL::Arrangement_on_surface_with_history_2<GeomTraits, Topol_traits> aos_with_history;
};

} // end of aos2 namespace

#endif //CGALPY_ARRANGEMENT_ON_SURFACE_2_CONFIG_HPP
