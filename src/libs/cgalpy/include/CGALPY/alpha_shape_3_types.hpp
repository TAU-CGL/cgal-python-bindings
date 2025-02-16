// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_3_TYPES_HPP
#define CGALPY_ALPHA_SHAPE_3_TYPES_HPP

#include "CGALPY/alpha_shape_3_config.hpp"
#include "CGALPY/triangulation_3_types.hpp"

namespace as3 {

using Ec = Exact_comparison<exact_comparison()>::type;
using Alpha_shape_3 = Alpha_shape<CGALPY_AS3, tri3::Triangulation_3, Ec>::type;

using Classification_type = Alpha_shape_3::Classification_type;
using Gt = Alpha_shape_3::Gt;
using FT = Alpha_shape_3::FT;
using Point = Alpha_shape_3::Point;
using size_type = Alpha_shape_3::size_type;

using Cell_handle = Alpha_shape_3::Cell_handle;
using Vertex_handle = Alpha_shape_3::Vertex_handle;
using Vertex = Alpha_shape_3::Vertex;
using Edge = Alpha_shape_3::Edge;
using Facet = Alpha_shape_3::Facet;

using Cell_circulator = Alpha_shape_3::Cell_circulator;
using Facet_circulator = Alpha_shape_3::Facet_circulator;

using Cell_iterator = Alpha_shape_3::Cell_iterator;
using Facet_iterator = Alpha_shape_3::Facet_iterator;
using Edge_iterator = Alpha_shape_3::Edge_iterator;
using Vertex_iterator = Alpha_shape_3::Vertex_iterator;

using Finite_cells_iterator = Alpha_shape_3::Finite_cells_iterator;
using Finite_facets_iterator = Alpha_shape_3::Finite_facets_iterator;
using Finite_edges_iterator = Alpha_shape_3::Finite_edges_iterator;
using Finite_vertices_iterator = Alpha_shape_3::Finite_vertices_iterator;

using Locate_type = Alpha_shape_3::Locate_type;
using Weighted_tag = Alpha_shape_3::Weighted_tag;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
using Mode = Alpha_shape_3::Mode;
using Alpha_iterator = Alpha_shape_3::Alpha_iterator;
using Alpha_status = CGAL::Alpha_status<FT>;
#endif

}

#endif
