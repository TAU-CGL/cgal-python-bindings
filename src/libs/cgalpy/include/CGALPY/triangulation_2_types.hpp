// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_2_TYPES_HPP
#define CGALPY_TRIANGULATION_2_TYPES_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/config.hpp"
#include "CGALPY/triangulation_2_config.hpp"
#include "CGALPY/alpha_shape_2_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace tri2 {

using Ec = as2::Exact_comparison<as2::exact_comparison()>::type;

using Traits = Tr<CGALPY_TRI2, Kernel>::type;

// Vertex
using Vb = Vertex_base_name<is_periodic(), CGALPY_TRI2, Traits>::type;
using Vbi = Vertex_with_info<vertex_with_info(), Vb, py::object, Traits>::type;
using Vbia = Vertex_alpha_shape<alpha_shape_2_bindings(), Vbi, Traits, Ec>::type;
using V = Vertex_hierarchy<hierarchy(), Vbia>::type;

// Face
using Fb = Face_base_name<is_periodic(), CGALPY_TRI2, Traits>::type;
using Fbi = Face_with_info<face_with_info(), Fb, py::object, Traits>::type;
using Fbic = Face_constrained<is_constrained(), Fbi, Traits>::type;
using F = Face_alpha_shape<alpha_shape_2_bindings(), Fbic, Traits, Ec>::type;

  // Triangulation data structure
using Tds = CGAL::Triangulation_data_structure_2<V, F>;
using Itag = Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG>::type;
using Btr = Base_tri<CGALPY_TRI2, Traits, Tds, Itag>::type;
using Triangulation_2 = Tri<hierarchy(), is_periodic(), Btr>::type;

using Geom_traits = Triangulation_2::Geom_traits;
using Triangulation_data_structure =
  Triangulation_2::Triangulation_data_structure;
using Locate_type = Triangulation_2::Locate_type;

using Point = Triangulation_2::Point;
using Segment = Triangulation_2::Segment;
using Triangle = Triangulation_2::Triangle;
using Vertex = Triangulation_2::Vertex;
using Edge = Triangulation_2::Edge;
using Face = Triangulation_2::Face;
using size_type = Triangulation_2::size_type;
using difference_type = Triangulation_2::difference_type;

using Vertex_handle = Triangulation_2::Vertex_handle;
using Face_handle = Triangulation_2::Face_handle;

using All_vertices_iterator = Triangulation_2::All_vertices_iterator;
using All_edges_iterator = Triangulation_2::All_edges_iterator;
using All_faces_iterator = Triangulation_2::All_faces_iterator;
using Finite_vertices_iterator = Triangulation_2::Finite_vertices_iterator;
using Finite_edges_iterator = Triangulation_2::Finite_edges_iterator;
using Finite_faces_iterator = Triangulation_2::Finite_faces_iterator;
using Point_iterator = Triangulation_2::Point_iterator;

using Vertex_circulator = Triangulation_2::Vertex_circulator;
using Edge_circulator = Triangulation_2::Edge_circulator;
using Face_circulator = Triangulation_2::Face_circulator;

using Points = Triangulation_2::Points;
using Line_face_circulator = Triangulation_2::Line_face_circulator;
using All_vertex_handles = Triangulation_2::All_vertex_handles;
using Finite_vertex_handles = Triangulation_2::Finite_vertex_handles;
using All_edges = Triangulation_2::All_edges;
using Finite_edges = Triangulation_2::Finite_edges;
using All_face_handles = Triangulation_2::All_face_handles;
using Finite_face_handles = Triangulation_2::Finite_face_handles;

} // End of namespace tri2

#endif
