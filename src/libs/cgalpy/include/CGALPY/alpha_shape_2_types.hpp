// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_2_TYPES_HPP
#define CGALPY_ALPHA_SHAPE_2_TYPES_HPP

#include <CGAL/Alpha_shape_2.h>

#include "CGALPY/config.hpp"
#include "CGALPY/alpha_shape_2_config.hpp"
#include "CGALPY/triangulation_2_types.hpp"

namespace cgalpy {
namespace as2 {

using Ec = Exact_comparison<exact_comparison()>::type;
using Alpha_shape_2 = CGAL::Alpha_shape_2<tri2::Triangulation_2, Ec>;

using Classification_type = Alpha_shape_2::Classification_type;
using Mode = Alpha_shape_2::Mode;
using Gt = Alpha_shape_2::Gt;
using Point = Alpha_shape_2::Point;
using FT = Alpha_shape_2::FT;
using size_type = Alpha_shape_2::size_type;

using Tds = Alpha_shape_2::Tds;

using Vertex = Alpha_shape_2::Vertex;
using Edge = Alpha_shape_2::Edge;
using Vertex_handle = Alpha_shape_2::Vertex_handle;
using Face_handle = Alpha_shape_2::Face_handle;

using Alpha_iterator = Alpha_shape_2::Alpha_iterator;
using Alpha_shape_vertices_iterator =
  Alpha_shape_2::Alpha_shape_vertices_iterator;
using Alpha_shape_edges_iterator = Alpha_shape_2::Alpha_shape_edges_iterator;

}
} // namespace cgalpy

#endif
