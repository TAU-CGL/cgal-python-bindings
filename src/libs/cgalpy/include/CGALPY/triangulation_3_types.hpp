// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_3_TYPES_HPP
#define CGALPY_TRIANGULATION_3_TYPES_HPP

#include "CGALPY/alpha_shape_3_config.hpp"
#include "CGALPY/triangulation_3_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace tri3 {

typedef as3::Exact_comparison<as3::exact_comparison()>::type    Ec;
typedef Tr<CGALPY_TRI3, Kernel>::type                           Traits;

// Vertex type
typedef Vertex_periodic<is_periodic()>::type                    Vbd;
typedef Vertex_base_name<is_regular(), Vbd, Traits>::type       Vb;
typedef Vertex_with_info<vertex_with_info(), Vb, bp::object, Traits>::type
                                                                Vbi;
#ifdef CGALPY_AS3
typedef Vertex_alpha_shape<CGALPY_AS3, Vbi, Traits, Ec>::type   Vbia;
#else
typedef Vbi                                                     Vbia;
#endif
typedef Vertex_hierarchy<hierarchy(), Vbia>::type               V;

// Cell type
typedef Cell_periodic<is_periodic()>::type                      Cbd;
typedef Cell_base_name<is_regular(), Cbd, Traits>::type         Cb;
typedef Cell_with_info<cell_with_info(), Cb, bp::object, Traits>::type
                                                                Cbi;
# ifdef CGALPY_AS3
typedef Cell_alpha_shape<CGALPY_AS3, Cbi, Traits, Ec>::type     C;
#else
typedef Cbi                                                     C;
#endif

// Concurency
typedef Concurrency<CGALPY_TRI3_CONCURRENCY>::type              Con;
typedef CGAL::Triangulation_data_structure_3<V, C, Con>         Tds;
typedef Location_policy<CGALPY_TRI3_LOCATION_POLICY>::type      Lp;
typedef Base_tri<CGALPY_TRI3, Traits, Tds, Lp>::type            Btr;
typedef Tri<hierarchy(), Btr>::type                             Triangulation_3;

typedef Triangulation_3::Geom_traits                  Geom_traits;
typedef Triangulation_3::Triangulation_data_structure
  Triangulation_data_structure;
typedef Triangulation_3::Lock_data_structure          Lock_data_structure;
typedef Triangulation_3::Locate_type                  Locate_type;

typedef Triangulation_3::Point                        Point;
typedef Triangulation_3::Segment                      Segment;
typedef Triangulation_3::Triangle                     Triangle;
typedef Triangulation_3::Tetrahedron                  Tetrahedron;
typedef Triangulation_3::Vertex                       Vertex;
typedef Triangulation_3::Edge                         Edge;
typedef Triangulation_3::Facet                        Facet;
typedef Triangulation_3::Cell                         Cell;
typedef Triangulation_3::size_type                    size_type;
typedef Triangulation_3::difference_type              difference_type;
typedef Triangulation_3::Concurrency_tag              Concurrency_tag;

typedef Triangulation_3::Vertex_handle                Vertex_handle;
typedef Triangulation_3::Cell_handle                  Cell_handle;
typedef Triangulation_3::Simplex                      Simplex;

typedef Triangulation_3::All_vertices_iterator        All_vertices_iterator;
typedef Triangulation_3::All_edges_iterator           All_edges_iterator;
typedef Triangulation_3::All_facets_iterator          All_facets_iterator;
typedef Triangulation_3::All_cells_iterator           All_cells_iterator;

typedef Triangulation_3::Finite_vertices_iterator     Finite_vertices_iterator;
typedef Triangulation_3::Finite_edges_iterator        Finite_edges_iterator;
typedef Triangulation_3::Finite_facets_iterator       Finite_facets_iterator;
typedef Triangulation_3::Finite_cells_iterator        Finite_cells_iterator;

typedef Triangulation_3::Point_iterator               Point_iterator;
typedef Triangulation_3::Facet_iterator               Facet_iterator;
typedef Triangulation_3::Cell_iterator                Cell_iterator;
typedef Triangulation_3::Segment_cell_iterator        Segment_cell_iterator;
typedef Triangulation_3::Segment_simplex_iterator     Segment_simplex_iterator;

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
