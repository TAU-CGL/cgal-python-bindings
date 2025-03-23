// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_TRIANGULATION_D_TYPES_HPP
#define CGALPY_TRIANGULATION_D_TYPES_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Triangulation_ds_vertex.h>
#include <CGAL/Triangulation_ds_full_cell.h>

#include "CGALPY/triangulation_d_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace trid {

using Traits = Kernel_d;

// Vertex type
using Vb = CGAL::Triangulation_ds_vertex<Traits>;
using V = Vertex_with_data<vertex_with_data(), Vb, py::object, Traits>::type;

// Cell type
using Sd = Storage_policy<CGALPY_TRID_STORAGE_POLICY>::type;
using Cb = CGAL::Triangulation_ds_full_cell<Traits, Sd>;
using C = Full_cell_with_data<cell_with_data(), Cb, py::object, Traits>::type;

// Dimensionality
D = Dimensionality<CGALPY_TRID_DIMENSION_TAG, CGALPY_TRID_DIMENSION>::type;

// Data structure
using Tds = CGAL::Triangulation_data_structure<D, V, C>;
using Sp = Storage_policy<CGALPY_TRID_STORAGE_POLICY>::type;
using Triangulation_d = Tri<CGALPY_TRID, Traits, Tds>::type;
using Geom_traits = Triangulation_d::Geom_traits;
using Triangulation_data_structure =
  Triangulation_d::Triangulation_data_structure;

using Point = Triangulation_d::Point;
using Segment = Triangulation_d::Segment;
using Triangle = Triangulation_d::Triangle;
using Tetrahedron = Triangulation_d::Tetrahedron;
using Vertex = Triangulation_d::Vertex;
using Edge = Triangulation_d::Edge;
using Facet = Triangulation_d::Facet;
using Cell = Triangulation_d::Cell;
using size_type = Triangulation_d::size_type;
using difference_type = Triangulation_d::difference_type;
using Concurrency_tag = Triangulation_d::Concurrency_tag;

using Vertex_handle = Triangulation_d::Vertex_handle;
using Cell_handle = Triangulation_d::Cell_handle;
using Simplex = Triangulation_d::Simplex;

using All_vertices_iterator = Triangulation_d::All_vertices_iterator;
using All_edges_iterator = Triangulation_d::All_edges_iterator;
using All_facets_iterator = Triangulation_d::All_facets_iterator;
using All_cells_iterator = Triangulation_d::All_cells_iterator;

using Finite_vertices_iterator = Triangulation_d::Finite_vertices_iterator;
using Finite_edges_iterator = Triangulation_d::Finite_edges_iterator;
using Finite_facets_iterator = Triangulation_d::Finite_facets_iterator;
using Finite_cells_iterator = Triangulation_d::Finite_cells_iterator;

using Point_iterator = Triangulation_d::Point_iterator;
using Facet_iterator = Triangulation_d::Facet_iterator;
using Cell_iterator = Triangulation_d::Cell_iterator;
using Segment_cell_iterator = Triangulation_d::Segment_cell_iterator;
using Segment_simplex_iterator = Triangulation_d::Segment_simplex_iterator;

#if (CGALPY_TRID == CGALPY_TRID_PLAIN)
using Vertex_circulator = Triangulation_d::Vertex_circulator;
using Edge_circulator = Triangulation_d::Edge_circulator;
using Cell_circulator = Triangulation_d::Cell_circulator;
#endif

#if (CGALPY_TRID == CGALPY_TRID_REGULAR)
using Weighted_point = Triangulation_d::Weighted_point;
using Bare_point = Triangulation_d::Bare_point;
#endif

} // End of namespace trid

#endif
