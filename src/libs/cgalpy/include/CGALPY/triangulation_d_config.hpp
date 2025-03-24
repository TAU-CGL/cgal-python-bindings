// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_3_CONFIG_HPP
#define CGALPY_TRIANGULATION_3_CONFIG_HPP

#include <CGAL/Triangulation_face.h>
#include <CGAL/Triangulation_full_cell.h>
#include <CGAL/Triangulation_vertex.h>
#include <CGAL/Triangulation.h>
#include <CGAL/Regular_triangulation.h>
#include <CGAL/Delaunay_triangulation.h>

#include "CGALPY/config.hpp"
#include "CGALPY/kernel_d_types.hpp"

#define CGALPY_TRID_PLAIN                       0
#define CGALPY_TRID_REGULAR                     1
#define CGALPY_TRID_DELAUNAY                    2

#define CGALPY_TRID_STORAGE_POLICY_DEFAULT      0
#define CGALPY_TRID_STORAGE_POLICY_MIRROR       1

#define CGALPY_TRID_DIMENSION_TAG_DYNAMIC       0
#define CGALPY_TRID_DIMENSION_TAG_STATIC        1

#ifndef CGALPY_TRID_DIMENSION_TAG
#define CGALPY_TRID_DIMENSION_TAG               CGALPY_TRID_DIMENSION_TAG_DYNAMIC
#endif

#ifndef CGALPY_TRID_DIMENSION
#define CGALPY_TRID_DIMENSION                   2
#endif

namespace trid {

// Indicates whether a vertex with infois selected
constexpr bool vertex_with_data()
{ return DETECT_EXIST(CGALPY_TRID_VERTEX_WITH_DATA); }

// Indicates whether a vertex with infois selected
constexpr bool cell_with_data()
{ return DETECT_EXIST(CGALPY_TRID_CELL_WITH_DATA); }

// Indicates whether the selected triangulation is regular
constexpr bool is_regular() {
  return (CGALPY_TRID == CGALPY_TRID_REGULAR);
}

// Vertex with data
template <bool b, typename Vb, typename Data, typename Tr>
struct Vertex_with_data {};
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_data<false, Vb, Data, Tr>
{ using type = CGAL::Triangulation_vertex<Tr, CGAL::No_vertex_data, Vb>; };
template <typename Vb, typename Data, typename Tr>
struct Vertex_with_data<true, Vb, Data, Tr>
{ using type = CGAL::Triangulation_vertex<Tr, Data, Vb>; };

// Storage policy
template <int i> struct Storage_policy {};
template <> struct Storage_policy<CGALPY_TRID_STORAGE_POLICY_DEFAULT>
{ using type = CGAL::TDS_full_cell_default_storage_policy; };
template <> struct Storage_policy<CGALPY_TRID_STORAGE_POLICY_MIRROR>
{ using type = CGAL::TDS_full_cell_mirror_storage_policy; };

// Cell with data
template <bool b, typename Fb, typename Data, typename Tr>
struct Full_cell_with_data {};
template <typename Fb, typename Data, typename Tr>
struct Full_cell_with_data<false, Fb, Data, Tr>
{ using type = CGAL::Triangulation_full_cell<Tr, CGAL::No_full_cell_data, Fb>; };
template <typename Fb, typename Data, typename Tr>
struct Full_cell_with_data<true, Fb, Data, Tr>
{ using type = CGAL::Triangulation_full_cell<Tr, Data, Fb>; };

// Dimensionality
template <int i, int d> struct Dimensionality {};
template <int d> struct Dimensionality<CGALPY_TRID_DIMENSION_TAG_DYNAMIC, d>
{ using type = CGAL::Dynamic_dimension_tag; };
template <int d> struct Dimensionality<CGALPY_TRID_DIMENSION_TAG_STATIC, d>
{ using type = CGAL::Dimension_tag<d>; };

// Main triangulation
template <int i, typename Tr, typename Tds> struct Tri {};
template <typename Tr, typename Tds>
struct Tri<CGALPY_TRID_PLAIN, Tr, Tds>
{ using type = CGAL::Triangulation<Tr, Tds>; };
template <typename Tr, typename Tds>
struct Tri<CGALPY_TRID_REGULAR, Tr, Tds>
{ using type = CGAL::Regular_triangulation<Tr, Tds>; };
template <typename Tr, typename Tds>
struct Tri<CGALPY_TRID_DELAUNAY, Tr, Tds>
{ using type = CGAL::Delaunay_triangulation<Tr, Tds>; };

}

#endif
