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

#include <CGAL/HalfedgeDS_vertex_max_base_with_id.h>
#include <CGAL/HalfedgeDS_halfedge_max_base_with_id.h>
#include <CGAL/HalfedgeDS_face_max_base_with_id.h>

// Geometry traits options
#define CGALPY_POL3_KERNEL_GEOMETRY_TRAITS              0
#define CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS        1

namespace pol3 {

// Indicates whether the vertex type has an id
constexpr bool vertex_with_id()
{ return DETECT_EXIST(CGALPY_POL3_VERTEX_WITH_ID); }

// Indicates whether the halfedge type has an id
constexpr bool halfedge_with_id()
{ return DETECT_EXIST(CGALPY_POL3_HALFEDGE_WITH_ID); }

// Indicates whether the halfedge type has an id
constexpr bool face_with_id()
{ return DETECT_EXIST(CGALPY_POL3_FACE_WITH_ID); }

// Indicates whether the vertex type is extended
constexpr bool is_vertex_extended()
{ return DETECT_EXIST(CGALPY_POL3_VERTEX_EXTENDED); }

// Indicates whether the halfedge type is extended
constexpr bool is_halfedge_extended()
{ return DETECT_EXIST(CGALPY_POL3_HALFEDGE_EXTENDED); }

// Indicates whether the halfedge type is extended
constexpr bool is_face_extended()
{ return DETECT_EXIST(CGALPY_POL3_FACE_EXTENDED); }

// Vertex expansion
template <bool b, typename Refs, typename Point> struct Vertex_with_id {};
template <typename Refs, typename Point>
struct Vertex_with_id<false, Refs, Point>
{ typedef CGAL::HalfedgeDS_vertex_base<Refs, CGAL::Tag_true, Point> type; };
template <typename Refs, typename Point>
struct Vertex_with_id<true, Refs, Point>
{
  typedef CGAL::HalfedgeDS_vertex_max_base_with_id<Refs, Point, std::size_t>
                                                                type;
};

template <bool b, typename Vb, typename Data> struct Vertex_extended {};
template <typename Vb, typename Data>
struct Vertex_extended<false, Vb, Data> { typedef Vb type; };
template <typename Vb, typename Data>
struct Vertex_extended<true, Vb, Data>
{ typedef Hds_vertex_with_data<Vb, Data> type; };

// Halfedge expansion
template <bool b, typename Refs, typename Data>
struct Halfedge_extended {};
template <typename Refs, typename Data>
struct Halfedge_extended<false, Refs, Data>
{ typedef CGAL::HalfedgeDS_halfedge_base<Refs, CGAL::Tag_true,
                                         CGAL::Tag_true, CGAL::Tag_true> type; };
template <typename Refs, typename Data>
struct Halfedge_extended<true, Refs, Data>
{ typedef Hds_halfedge_with_data<Refs, Data> type; };

// Face expansion
template <bool b, typename Refs, typename Plane> struct Face_with_id {};
template <typename Refs, typename Plane>
struct Face_with_id<false, Refs, Plane>
{ typedef CGAL::HalfedgeDS_face_base<Refs, CGAL::Tag_true, Plane> type; };
template <typename Refs, typename Plane>
struct Face_with_id<true, Refs, Plane>
{
  typedef CGAL::HalfedgeDS_face_max_base_with_id<Refs, CGAL::Tag_false,
                                                 std::size_t>   type;
};

template <bool b, typename Fb, typename Data> struct Face_extended {};
template <typename Fb, typename Data>
struct Face_extended<false, Fb, Data> { typedef Fb type; };
template <typename Fb, typename Data>
struct Face_extended<true, Fb, Data>
{ typedef Hds_face_with_data<Fb, Data> type; };

}

#endif
