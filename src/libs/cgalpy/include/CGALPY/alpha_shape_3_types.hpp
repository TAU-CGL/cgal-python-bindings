// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_3_TYPES_HPP
#define CGALPY_ALPHA_SHAPE_3_TYPES_HPP

#include "CGALPY/alpha_shape_3_config.hpp"

// Alpha shape type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
#include <CGAL/Alpha_shape_3.h>
#elif CGALPY_AS3 == CGALPY_AS3_FIXED
#include <CGAL/Fixed_alpha_shape_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_ALPHA_SHAPE");
#endif

namespace as3 {

// Alpha shape type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
  typedef CGAL::Alpha_shape_3<tri3::Triangulation_3, Exact_comparison>
                                                                Alpha_shape_3;
#elif CGALPY_AS3 == CGALPY_AS3_FIXED
  typedef CGAL::Fixed_alpha_shape_3<tri3::Triangulation_3>      Alpha_shape_3;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AS3");
#endif

typedef Alpha_shape_3::Point                    Point;

typedef Alpha_shape_3::Cell_handle              Cell_handle;
typedef Alpha_shape_3::Vertex_handle            Vertex_handle;
typedef Alpha_shape_3::Facet                    Facet;
typedef Alpha_shape_3::Edge                     Edge;

typedef Alpha_shape_3::Cell_circulator          Cell_circulator;
typedef Alpha_shape_3::Facet_circulator         Facet_circulator;

typedef Alpha_shape_3::Cell_iterator            Cell_iterator;
typedef Alpha_shape_3::Facet_iterator           Facet_iterator;
typedef Alpha_shape_3::Edge_iterator            Edge_iterator;
typedef Alpha_shape_3::Vertex_iterator          Vertex_iterator;

typedef Alpha_shape_3::Finite_cells_iterator    Finite_cells_iterator;
typedef Alpha_shape_3::Finite_facets_iterator   Finite_facets_iterator;
typedef Alpha_shape_3::Finite_edges_iterator    Finite_edges_iterator;
typedef Alpha_shape_3::Finite_vertices_iterator Finite_vertices_iterator;

typedef Alpha_shape_3::size_type                Size_type;
typedef Alpha_shape_3::Locate_type              Locate_type;
typedef Alpha_shape_3::Weighted_tag             Weighted_tag;

typedef Alpha_shape_3::NT                       NT;

typedef Alpha_shape_3::Classification_type      Classification_type;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
typedef Alpha_shape_3::Mode                     Mode;
typedef Alpha_shape_3::Alpha_iterator           Alpha_iterator;
typedef CGAL::Alpha_status<NT>                  Alpha_status;
#endif

}

#endif
