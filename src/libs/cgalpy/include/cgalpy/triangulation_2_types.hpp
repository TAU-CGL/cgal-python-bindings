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

#include "cgalpy/config.hpp"
#include "cgalpy/alpha_shape_2_config.hpp"
#include "cgalpy/kernel_types.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/triangulation_2_config.hpp"

namespace py = nanobind;

namespace cgalpy {
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
using Triangulation_data_structure_2 = CGAL::Triangulation_data_structure_2<V, F>;
using Tds = Triangulation_data_structure_2;
using Itag = Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG>::type;

using My_tri2 = Tri2<CGALPY_TRI2, Traits, Tds, Itag>;
using Triangulation_2 = My_tri2::Triangulation_2;
using Triangulation_hierarchy_2 = Th<hierarchy(), is_periodic(), Triangulation_2>::type;

#if (CGALPY_TRI2 == CGALPY_TRI2_PLAIN)
#elif (CGALPY_TRI2 == CGALPY_TRI2_REGULAR)
using Regular_triangulation_2 = My_tri2::Regular_triangulation_2;
#elif (CGALPY_TRI2 == CGALPY_TRI2_DELAUNAY)
using Delaunay_triangulation_2 = My_tri2::Delaunay_triangulation_2;
#elif (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED)
using Constrained_triangulation_2 = My_tri2::Constrained_triangulation_2;
#elif (CGALPY_TRI2 == CGALPY_TRI2_CONSTRAINED_DELAUNAY)
using Constrained_delaunay_triangulation_2 = My_tri2::Constrained_delaunay_triangulation_2;
using Constrained_triangulation_2 = My_tri2::Constrained_triangulation_2;
#endif

using Geom_traits = Triangulation_2::Geom_traits;
using Triangulation_data_structure = Triangulation_2::Triangulation_data_structure;
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

//!
template <typename Triangulation_>
Vertex& insert_point1(Triangulation_& tri, const Point& p) { return *(tri.insert(p)); }

//!
template <typename Triangulation_>
Vertex& insert_point2(Triangulation_& tri, const Point& p, Face& f) { return *(tri.insert(p, Face_handle(&f))); }

//!
template <typename Triangulation_>
Vertex& insert_point3(Triangulation_& tri, const Point& p, Locate_type lt, Face& loc, int li)
{ return *(tri.insert(p, lt, Face_handle(&loc), li)); }

//!
template <typename Triangulation_>
int insert_points(Triangulation_& t, py::list& lst) {
  auto begin = stl_forward_iterator<Point>(lst);
  auto end = stl_forward_iterator<Point>(lst, false);
  return t.insert(begin, end);
}

}
} // namespace cgalpy // End of namespace tri2

#endif
