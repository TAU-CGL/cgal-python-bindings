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

#include "CGALPY/config.hpp"
#include "CGALPY/triangulation_2_config.hpp"
#include "CGALPY/alpha_shape_2_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace tri2 {

typedef as2::Exact_comparison<as2::exact_comparison()>::type    Ec;

typedef Tr<CGALPY_TRI2, Kernel>::type                           Traits;

// Vertex
typedef Vertex_base_name<is_periodic(), CGALPY_TRI2, Traits>::type
                                                                Vb;
typedef Vertex_with_info<vertex_with_info(), Vb, py::object, Traits>::type
                                                                Vbi;
typedef Vertex_alpha_shape<alpha_shape_2_bindings(), Vbi, Traits, Ec>::type
                                                                Vbia;
typedef Vertex_hierarchy<hierarchy(), Vbia>::type               V;

// Face
typedef Face_base_name<is_periodic(), CGALPY_TRI2, Traits>::type
                                                                Fb;
typedef Face_with_info<face_with_info(), Fb, py::object, Traits>::type
                                                                Fbi;
typedef Face_alpha_shape<alpha_shape_2_bindings(), Fbi, Traits, Ec>::type
                                                                F;

  // Triangulation data structure
typedef CGAL::Triangulation_data_structure_2<V, F>              Tds;
typedef Intersection_tag<CGALPY_TRI2_INTERSECTION_TAG>::type    Itag;
typedef Base_tri<CGALPY_TRI2, Traits, Tds, Itag>::type          Btr;
typedef Tri<hierarchy(), is_periodic(), Btr>::type              Triangulation_2;

typedef Triangulation_2::Geom_traits                  Geom_traits;
typedef Triangulation_2::Triangulation_data_structure
  Triangulation_data_structure;
typedef Triangulation_2::Locate_type                  Locate_type;

typedef Triangulation_2::Point                        Point;
typedef Triangulation_2::Segment                      Segment;
typedef Triangulation_2::Triangle                     Triangle;
typedef Triangulation_2::Vertex                       Vertex;
typedef Triangulation_2::Edge                         Edge;
typedef Triangulation_2::Face                         Face;
typedef Triangulation_2::size_type                    size_type;
typedef Triangulation_2::difference_type              difference_type;

typedef Triangulation_2::Vertex_handle                Vertex_handle;
typedef Triangulation_2::Face_handle                  Face_handle;

typedef Triangulation_2::All_vertices_iterator        All_vertices_iterator;
typedef Triangulation_2::All_edges_iterator           All_edges_iterator;
typedef Triangulation_2::All_faces_iterator           All_faces_iterator;
typedef Triangulation_2::Finite_vertices_iterator     Finite_vertices_iterator;
typedef Triangulation_2::Finite_edges_iterator        Finite_edges_iterator;
typedef Triangulation_2::Finite_faces_iterator        Finite_faces_iterator;
typedef Triangulation_2::Point_iterator               Point_iterator;

typedef Triangulation_2::Vertex_circulator            Vertex_circulator;
typedef Triangulation_2::Edge_circulator              Edge_circulator;
typedef Triangulation_2::Face_circulator              Face_circulator;

typedef Triangulation_2::Points                       Points;
typedef Triangulation_2::Line_face_circulator         Line_face_circulator;
typedef Triangulation_2::All_vertex_handles           All_vertex_handles;
typedef Triangulation_2::Finite_vertex_handles        Finite_vertex_handles;
typedef Triangulation_2::All_edges                    All_edges;
typedef Triangulation_2::Finite_edges                 Finite_edges;
typedef Triangulation_2::All_face_handles             All_face_handles;
typedef Triangulation_2::Finite_face_handles          Finite_face_handles;

} // End of namespace tri2

#endif //CGALPY_TRIANGULATION_2_TYPES_HPP
