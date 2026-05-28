// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_POLYHEDRON_3_CONFIG_HPP
#define CGALPY_POLYHEDRON_3_CONFIG_HPP

#include "CGALPY/config.hpp"
#include "CGALPY/Hds_vertex_with_data.hpp"
#include "CGALPY/Hds_halfedge_with_data.hpp"
#include "CGALPY/Hds_face_with_data.hpp"

// Geometry traits options
#define CGALPY_POL3_KERNEL_GEOMETRY_TRAITS              0
#define CGALPY_POL3_WITH_NORMALS_GEOMETRY_TRAITS        1

namespace cgalpy {
namespace pol3 {

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
template <bool b, typename Vb, typename Data> struct Vertex_extended {};
template <typename Vb, typename Data>
struct Vertex_extended<false, Vb, Data> { using type = Vb; };
template <typename Vb, typename Data>
struct Vertex_extended<true, Vb, Data>
{ using type = Hds_vertex_with_data<Vb, Data>; };

// Halfedge expansion
template <bool b, typename Hb, typename Data>
struct Halfedge_extended {};
template <typename Hb, typename Data>
struct Halfedge_extended<false, Hb, Data> { using type = Hb; };
template <typename Hb, typename Data>
struct Halfedge_extended<true, Hb, Data>
{ using type = Hds_halfedge_with_data<Hb, Data>; };

// Face expansion
template <bool b, typename Fb, typename Data> struct Face_extended {};
template <typename Fb, typename Data>
struct Face_extended<false, Fb, Data> { using type = Fb; };
template <typename Fb, typename Data>
struct Face_extended<true, Fb, Data>
{ using type = Hds_face_with_data<Fb, Data>; };

}
} // namespace cgalpy

#endif
