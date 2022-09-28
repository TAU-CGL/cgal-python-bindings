// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYHEDRON_3_CONFIG_HPP
#define CGALPY_POLYHEDRON_3_CONFIG_HPP

#include "CGALPY/Hds_vertex_with_data.hpp"
#include "CGALPY/Hds_halfedge_with_data.hpp"
#include "CGALPY/Hds_face_with_data.hpp"

namespace pol3 {

// Indicates whether the vertex type is extended
constexpr bool is_vertex_extended()
{ return DETECT_EXIST(CGALPY_POL3_VERTEX_EXTENDED); }

// Indicates whether the halfedge type is extended
constexpr bool is_halfedge_extended()
{ return DETECT_EXIST(CGALPY_POL3_HALFEDGE_EXTENDED); }

// Indicates whether the halfedge type is extended
constexpr bool is_facet_extended()
{ return DETECT_EXIST(CGALPY_POL3_FACET_EXTENDED); }

// Vertex expension
template <bool b, typename Refs, typename Point, typename Data>
struct Vertex_extended {};
template <typename Refs, typename Point, typename Data>
struct Vertex_extended<false, Refs, Point, Data>
{ typedef CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point> type; };
template <typename Refs, typename Point, typename Data>
struct Vertex_extended<true, Refs, Point, Data>
{ typedef Hds_vertex_with_data<Refs, Point, Data> type; };

// Halfedge expension
template <bool b, typename Refs, typename Data>
struct Halfedge_extended {};
template <typename Refs, typename Data>
struct Halfedge_extended<false, Refs, Data>
{ typedef CGAL::HalfedgeDS_halfedge_base<Refs, CGAL::Tag_true,
                                         CGAL::Tag_true, CGAL::Tag_true> type; };
template <typename Refs, typename Data>
struct Halfedge_extended<true, Refs, Data>
{ typedef Hds_halfedge_with_data<Refs, Data> type; };

// Facet expension
template <bool b, typename Refs, typename Plane, typename Data>
struct Facet_extended {};
template <typename Refs, typename Plane, typename Data>
struct Facet_extended<false, Refs, Plane, Data>
{ typedef CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane> type; };
template <typename Refs, typename Plane, typename Data>
struct Facet_extended<true, Refs, Plane, Data>
{ typedef Hds_face_with_data<Refs, Plane, Data> type; };

}

#endif
