// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_3_TYPES_HPP
#define CGALPY_TRIANGULATION_3_TYPES_HPP

#include "CGALPY/alpha_shape_3_config.hpp"
#include "CGALPY/triangulation_3_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace tri3 {

typedef as3::Exact_comparison<as3::exact_comparison()>::type    Ec;
typedef Tr<CGALPY_TRI3_TRAITS, Kernel>::type                    Traits;

typedef Vertex_base_name<CGALPY_TRI3_VERTEX_BASE, Traits>::type Vb;
typedef Vertex_with_info<vertex_with_info(), Vb, bp::object, Traits>::type
                                                                Vbi;
typedef Vertex_hierarchy<hierarchy(), Vbi>::type                V;
// typedef Vertex_alpha_shape<alpha_shape_3_bindings(), CGALPY_AS3, Vbi, Traits,
//                            Ec>::type                            Vbia;
// typedef Vertex_hierarchy<hierarchy(), Vbia>::type               Vbiah;
// typedef Vertex_periodic<is_periodic(), Vbiah, Traits>::type     V;

typedef Cell_base_name<CGALPY_TRI3_CELL_BASE, Traits>::type     Cb;
typedef Cell_with_info<cell_with_info(), Cb, bp::object, Traits>::type
                                                                C;
// typedef Cell_with_info<cell_with_info(), Cb, bp::object, Traits>::type
//                                                                 Cbi;
// typedef Cell_alpha_shape<alpha_shape_3_bindings(), Cbi, Traits, Ec>::type
//                                                                 Cbia;
// typedef Cell_periodic<is_periodic(), Cbia, Traits>::type        C;

typedef Concurrency<CGALPY_TRI3_CONCURRENCY>::type              Con;
typedef CGAL::Triangulation_data_structure_3<V, C, Con>         Tds;
typedef Location_policy<CGALPY_TRI3_LOCATION_POLICY>::type      Lp;
typedef Base_tri<CGALPY_TRI3, Traits, Tds, Lp>::type            Btr;
typedef Tri<hierarchy(), Btr>::type                             Triangulation_3;

typedef Triangulation_3::Point                        Point;
typedef Triangulation_3::Segment                      Segment;
typedef Triangulation_3::Triangle                     Triangle;
typedef Triangulation_3::Vertex                       Vertex;
typedef Triangulation_3::Edge                         Edge;
typedef Triangulation_3::Facet                        Facet;
typedef Triangulation_3::Cell                         Cell;
typedef Triangulation_3::size_type                    size_type;
typedef Triangulation_3::difference_type              difference_type;

typedef Triangulation_3::Vertex_handle                Vertex_handle;
typedef Triangulation_3::Cell_handle                  Cell_handle;

typedef Triangulation_3::All_vertices_iterator        All_vertices_iterator;
typedef Triangulation_3::Finite_vertices_iterator     Finite_vertices_iterator;
typedef Triangulation_3::All_edges_iterator           All_edges_iterator;
typedef Triangulation_3::Finite_edges_iterator        Finite_edges_iterator;
typedef Triangulation_3::Cell_handle                  Cell_handle;
typedef Triangulation_3::All_cells_iterator           All_cells_iterator;
typedef Triangulation_3::Finite_cells_iterator        Finite_cells_iterator;
typedef Triangulation_3::Point_iterator               Point_iterator;
typedef Triangulation_3::Locate_type                  Locate_type;

#if ((CGALPY_TRI3 == CGALPY_TRI3_PLAIN) || \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_PLAIN))
typedef Triangulation_3::Vertex_circulator            Vertex_circulator;
typedef Triangulation_3::Edge_circulator              Edge_circulator;
typedef Triangulation_3::Cell_circulator              Cell_circulator;
#endif

#if ((CGALPY_TRI3 == CGALPY_TRI3_REGULAR) || \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR))
typedef Triangulation_3::Weighted_point               Weighted_point;
typedef Triangulation_3::Bare_point                   Bare_point;
#endif

} // End of namespace tri3

#endif
