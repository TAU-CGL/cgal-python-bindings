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
using Traits>::typ = Cell_with_info<cell_with_info(), Cb, py::object,;
                                                                Cbi;
# ifdef CGALPY_AS3
using C = Cell_alpha_shape<CGALPY_AS3, Cbi, Traits, Ec>::type;
#else
using C = Cbi;
#endif

// Concurency
using Con = Concurrency<CGALPY_TRI3_CONCURRENCY>::type;
using Tds = CGAL::Triangulation_data_structure_3<V, C, Con>;
using Lp = Location_policy<CGALPY_TRI3_LOCATION_POLICY>::type;
using Btr = Base_tri<CGALPY_TRI3, Traits, Tds, Lp>::type;
using Triangulation_3 = Tri<hierarchy(), Btr>::type;

using Geom_traits = Triangulation_3::Geom_traits;
using Triangulation_data_structure =
  Triangulation_3::Triangulation_data_structure;

using Lock_data_structure = Triangulation_3::Lock_data_structure;
using Locate_type = Triangulation_3::Locate_type;

using Point = Triangulation_3::Point;
using Segment = Triangulation_3::Segment;
using Triangle = Triangulation_3::Triangle;
using Tetrahedron = Triangulation_3::Tetrahedron;
using Vertex = Triangulation_3::Vertex;
using Edge = Triangulation_3::Edge;
using Facet = Triangulation_3::Facet;
using Cell = Triangulation_3::Cell;
using size_type = Triangulation_3::size_type;
using difference_type = Triangulation_3::difference_type;
using Concurrency_tag = Triangulation_3::Concurrency_tag;

using Vertex_handle = Triangulation_3::Vertex_handle;
using Cell_handle = Triangulation_3::Cell_handle;
using Simplex = Triangulation_3::Simplex;

using All_vertices_iterator = Triangulation_3::All_vertices_iterator;
using All_edges_iterator = Triangulation_3::All_edges_iterator;
using All_facets_iterator = Triangulation_3::All_facets_iterator;
using All_cells_iterator = Triangulation_3::All_cells_iterator;

using Finite_vertices_iterator = Triangulation_3::Finite_vertices_iterator;
using Finite_edges_iterator = Triangulation_3::Finite_edges_iterator;
using Finite_facets_iterator = Triangulation_3::Finite_facets_iterator;
using Finite_cells_iterator = Triangulation_3::Finite_cells_iterator;

using Point_iterator = Triangulation_3::Point_iterator;
using Facet_iterator = Triangulation_3::Facet_iterator;
using Cell_iterator = Triangulation_3::Cell_iterator;
using Segment_cell_iterator = Triangulation_3::Segment_cell_iterator;
using Segment_simplex_iterator = Triangulation_3::Segment_simplex_iterator;

#if ((CGALPY_TRI3 == CGALPY_TRI3_PLAIN) || \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_PLAIN))
using Vertex_circulator = Triangulation_3::Vertex_circulator;
using Edge_circulator = Triangulation_3::Edge_circulator;
using Cell_circulator = Triangulation_3::Cell_circulator;
#endif

#if ((CGALPY_TRI3 == CGALPY_TRI3_REGULAR) || \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR))
using Weighted_point = Triangulation_3::Weighted_point;
using Bare_point = Triangulation_3::Bare_point;
#endif

} // End of namespace tri3

#endif
