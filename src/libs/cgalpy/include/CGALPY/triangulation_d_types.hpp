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
#include <CGAL/Triangulation_data_structure.h>
#include <CGAL/Regular_triangulation.h>
#include <CGAL/Delaunay_triangulation.h>

#include "CGALPY/triangulation_d_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

namespace trid {

using Traits = Kernel_d;

// Vertex type
using Vb = CGAL::Triangulation_ds_vertex<void>;
using V = Vertex_with_data<vertex_with_data(), Vb, py::object, Traits>::type;

// Cell type
using Sd = Storage_policy<CGALPY_TRID_STORAGE_POLICY>::type;
using Cb = CGAL::Triangulation_ds_full_cell<void, Sd>;
using C = Full_cell_with_data<full_cell_with_data(), Cb, py::object, Traits>::type;

// Dimensionality
using D = Dimensionality<CGALPY_TRID_DIMENSION_TAG, CGALPY_TRID_DIMENSION>::type;

// Data structure
using Tds = CGAL::Triangulation_data_structure<D, V, C>;
using Sp = Storage_policy<CGALPY_TRID_STORAGE_POLICY>::type;
using Triangulation_d = Tri<CGALPY_TRID, Traits, Tds>::type;
using Geom_traits = Triangulation_d::Geom_traits;
using Triangulation_ds = Triangulation_d::Triangulation_ds;

using Tds_vertex = Tds::Vertex;
using Tds_full_cell = Tds::Full_cell;

using Vertex = Triangulation_d::Vertex;
using Full_cell = Triangulation_d::Full_cell;
using Facet = Triangulation_d::Facet;
using Face = Triangulation_d::Face;
using Point = Triangulation_d::Point;

using Vertex_handle = Triangulation_d::Vertex_handle;
using Vertex_iterator = Triangulation_d::Vertex_iterator;
using Vertex_const_handle = Triangulation_d::Vertex_const_handle;
using Vertex_const_iterator = Triangulation_d::Vertex_const_iterator;

using Full_cell_handle = Triangulation_d::Full_cell_handle;
using Full_cell_iterator = Triangulation_d::Full_cell_iterator;
using Full_cell_const_handle = Triangulation_d::Full_cell_const_handle;
using Full_cell_const_iterator = Triangulation_d::Full_cell_const_iterator;

using Facet_iterator = Triangulation_d::Facet_iterator;

using size_type = Triangulation_d::size_type;
using difference_type = Triangulation_d::difference_type;

using Locate_type = Triangulation_d::Locate_type;

using Finite_vertex_iterator = Triangulation_d::Finite_vertex_iterator;
using Finite_vertex_const_iterator =
  Triangulation_d::Finite_vertex_const_iterator;
using Finite_full_cell_iterator = Triangulation_d::Finite_full_cell_iterator;
using Finite_full_cell_const_iterator =
  Triangulation_d::Finite_full_cell_const_iterator;
using Finite_facet_iterator = Triangulation_d::Finite_facet_iterator;

using Weighted_tag = Triangulation_d::Weighted_tag;
using Periodic_tag = Triangulation_d::Periodic_tag;

using Rotor = Triangulation_d::Rotor;

#if CGALPY_TRID == CGALPY_TRID_REGULAR
using Regular_triangulation = CGAL::Regular_triangulation<Traits, Tds>;
#endif

#if CGALPY_TRID == CGALPY_TRID_DELAUNAY
using Delaunay_triangulation = CGAL::Delaunay_triangulation<Traits, Tds>;
#endif

} // End of namespace trid

#endif
