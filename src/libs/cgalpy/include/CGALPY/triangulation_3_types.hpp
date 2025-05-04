// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_3_TYPES_HPP
#define CGALPY_TRIANGULATION_3_TYPES_HPP

#include <nanobind/nanobind.h>

#include "CGALPY/alpha_shape_3_config.hpp"
#include "CGALPY/triangulation_3_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace tri3 {

using Ec = as3::Exact_comparison<as3::exact_comparison()>::type;
using Traits = Tr<CGALPY_TRI3, Kernel>::type;

// Vertex type
using Vbd = Vertex_periodic<is_periodic()>::type;
using Vb = Vertex_base_name<is_regular(), Vbd, Traits>::type;
using Vbi = Vertex_with_info<vertex_with_info(), Vb, py::object, Traits>::type;

#ifdef CGALPY_AS3
using Vbia = Vertex_alpha_shape<CGALPY_AS3, Vbi, Traits, Ec>::type;
#else
using Vbia = Vbi;
#endif
using V = Vertex_hierarchy<hierarchy(), Vbia>::type;

// Cell type
using Cbd = Cell_periodic<is_periodic()>::type;
using Cb = Cell_base_name<is_regular(), Cbd, Traits>::type;
using Cbi = Cell_with_info<cell_with_info(), Cb, py::object, Traits>::type;
# ifdef CGALPY_AS3
using C = Cell_alpha_shape<CGALPY_AS3, Cbi, Traits, Ec>::type;
#else
using C = Cbi;
#endif

// Concurency
using Con = Concurrency<CGALPY_TRI3_CONCURRENCY>::type;
using Tds = CGAL::Triangulation_data_structure_3<V, C, Con>;
using Lp = Location_policy<CGALPY_TRI3_LOCATION_POLICY>::type;

using Btri = Tri3<CGALPY_TRI3, Traits, Tds, Lp>::base_type;
using Tri = Tri3<CGALPY_TRI3, Traits, Tds, Lp>::type;
using Triangulation_hierarchy_3 = Th<hierarchy(), Tri>::type;

#if ((CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_PLAIN) ||   \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR) || \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_DELAUNAY))
using Periodic_triangulation_3 = Btri;
#if (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR)
using Periodic_regular_triangulation_3 = Tri;
#elif (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_DELAUNAY)
using Periodic_delaunay_triangulation_3 = Tri;
#endif
#else
using Triangulation_3 = Btri;
#if (CGALPY_TRI3 == CGALPY_TRI3_REGULAR)
using Regular_triangulation_3 = Tri;
#elif (CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY)
using Delaunay_triangulation_3 = Tri;
#endif
#endif

using Geom_traits = Tri::Geom_traits;
using Triangulation_data_structure = Tri::Triangulation_data_structure;

using Lock_data_structure = Tri::Lock_data_structure;
using Locate_type = Tri::Locate_type;

using Point = Tri::Point;
using Segment = Tri::Segment;
using Triangle = Tri::Triangle;
using Tetrahedron = Tri::Tetrahedron;
using Vertex = Tri::Vertex;
using Edge = Tri::Edge;
using Facet = Tri::Facet;
using Cell = Tri::Cell;
using size_type = Tri::size_type;
using difference_type = Tri::difference_type;
using Concurrency_tag = Tri::Concurrency_tag;

using Vertex_handle = Tri::Vertex_handle;
using Cell_handle = Tri::Cell_handle;
using Simplex = Tri::Simplex;

using All_vertices_iterator = Tri::All_vertices_iterator;
using All_edges_iterator = Tri::All_edges_iterator;
using All_facets_iterator = Tri::All_facets_iterator;
using All_cells_iterator = Tri::All_cells_iterator;

using Finite_vertices_iterator = Tri::Finite_vertices_iterator;
using Finite_edges_iterator = Tri::Finite_edges_iterator;
using Finite_facets_iterator = Tri::Finite_facets_iterator;
using Finite_cells_iterator = Tri::Finite_cells_iterator;

using Point_iterator = Tri::Point_iterator;
using Facet_iterator = Tri::Facet_iterator;
using Cell_iterator = Tri::Cell_iterator;
using Segment_cell_iterator = Tri::Segment_cell_iterator;
using Segment_simplex_iterator = Tri::Segment_simplex_iterator;

using Edge_circulator = Tri::Facet_circulator;
using Cell_circulator = Tri::Cell_circulator;

#if ((CGALPY_TRI3 == CGALPY_TRI3_REGULAR) || \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR))
using Weighted_point = Tri::Weighted_point;
using Bare_point = Tri::Bare_point;
#endif

} // End of namespace tri3

#endif
