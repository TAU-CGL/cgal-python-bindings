// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_2_TYPES_HPP
#define CGALPY_ALPHA_SHAPE_2_TYPES_HPP

#include <CGAL/tags.h>

#include "CGALPY/config.hpp"
#include "CGALPY/alpha_shape_2_config.hpp"
#include "CGALPY/triangulation_2_types.hpp"

namespace as2 {

typedef Exact_comparison<exact_comparison()>::type        Ec;
typedef CGAL::Alpha_shape_2<Triangulation_2, Ec>          Alpha_shape_2;

typedef Alpha_shape_2::Classification_type              Classification_type;
typedef Alpha_shape_2::Classification_type              Mode;
typedef Alpha_shape_2::Gt                               Gt;
typedef Alpha_shape_2::Point                            Point;
typedef Alpha_shape_2::FT                               FT;
typedef Alpha_shape_2::size_type                        size_type
typedef Alpha_shape_2::Alpha_iterator                   Alpha_iterator;
typedef Alpha_shape_2::Alpha_shape_vertices_iterator
  Alpha_shape_vertices_iterator;
typedef Alpha_shape_2::Alpha_shape_edges_iterator
  Alpha_shape_edges_iterator;

}

#endif
