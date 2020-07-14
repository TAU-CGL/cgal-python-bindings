// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ALPHA_SHAPE_3_TYPES_HPP
#define CGALPY_ALPHA_SHAPE_3_TYPES_HPP

#include <CGALPY/alpha_shape_3_config.hpp>

#if ((CGALPY_TRI3_VERTEX_BASE != CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE) && \
     (CGALPY_TRI3_VERTEX_BASE != CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_WITH_INFO) && \
     (CGALPY_TRI3_VERTEX_BASE != CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR) && \
     (CGALPY_TRI3_VERTEX_BASE != CGALPY_TRI3_VERTEX_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO) && \
     (CGALPY_TRI3_VERTEX_BASE != CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE) && \
     (CGALPY_TRI3_VERTEX_BASE != CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) && \
     (CGALPY_TRI3_VERTEX_BASE != CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR) && \
     (CGALPY_TRI3_VERTEX_BASE != CGALPY_TRI3_VERTEX_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_VERTEX_BASE");
#endif

#if ((CGALPY_TRI3_CELL_BASE != CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE) && \
     (CGALPY_TRI3_CELL_BASE != CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_WITH_INFO) && \
     (CGALPY_TRI3_CELL_BASE != CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR) && \
     (CGALPY_TRI3_CELL_BASE != CGALPY_TRI3_CELL_BASE_ALPHA_SHAPE_REGULAR_WITH_INFO) && \
     (CGALPY_TRI3_CELL_BASE != CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE) && \
     (CGALPY_TRI3_CELL_BASE != CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_WITH_INFO) && \
     (CGALPY_TRI3_CELL_BASE != CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR) && \
     (CGALPY_TRI3_CELL_BASE != CGALPY_TRI3_CELL_BASE_FIXED_ALPHA_SHAPE_REGULAR_WITH_INFO))
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_TRI3_CELL_BASE");
#endif

// Alpha shape type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
#include <CGAL/Alpha_shape_3.h>
#elif CGALPY_AS3 == CGALPY_AS3_FIXED
#include <CGAL/Fixed_alpha_shape_3.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_ALPHA_SHAPE");
#endif

// Exact comparison
namespace as3 {
#if CGALPY_AS3_EXACT_COMPARISON == 0
typedef CGAL::Tag_false         Exact_comparison;
#elif CGALPY_AS3_EXACT_COMPARISON == 1
typedef CGAL::Tag_true          Exact_comparison;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AS3_EXACT_COMPARISON");
#endif
}

// Alpha shape type
#if CGALPY_AS3 == CGALPY_AS3_PLAIN
typedef CGAL::Alpha_shape_3<Triangulation_3, As3_exact_comparison> Alpha_shape_3;
#elif CGALPY_AS3 == CGALPY_AS3_FIXED
typedef CGAL::Fixed_alpha_shape_3<Triangulation_3>                 Alpha_shape_3;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AS3");
#endif

typedef Alpha_shape_3::Point                    As_point;

typedef Alpha_shape_3::Cell_handle              As_cell_handle;
typedef Alpha_shape_3::Vertex_handle            As_vertex_handle;
typedef Alpha_shape_3::Facet                    As_facet;
typedef Alpha_shape_3::Edge                     As_edge;

typedef Alpha_shape_3::Cell_circulator          As_cell_circulator;
typedef Alpha_shape_3::Facet_circulator         As_facet_circulator;

typedef Alpha_shape_3::Cell_iterator            As_cell_iterator;
typedef Alpha_shape_3::Facet_iterator           As_facet_iterator;
typedef Alpha_shape_3::Edge_iterator            As_edge_iterator;
typedef Alpha_shape_3::Vertex_iterator          As_vertex_iterator;

typedef Alpha_shape_3::Finite_cells_iterator    As_finite_cells_iterator;
typedef Alpha_shape_3::Finite_facets_iterator   As_finite_facets_iterator;
typedef Alpha_shape_3::Finite_edges_iterator    As_finite_edges_iterator;
typedef Alpha_shape_3::Finite_vertices_iterator As_finite_vertices_iterator;

typedef Alpha_shape_3::size_type                As_size_type;
typedef Alpha_shape_3::Locate_type              As_locate_type;
typedef Alpha_shape_3::Weighted_tag             As_weighted_tag;

typedef Alpha_shape_3::NT                       As_nt;

typedef Alpha_shape_3::Classification_type      As_classification_type;

#if CGALPY_AS3 == CGALPY_AS3_PLAIN
typedef Alpha_shape_3::Mode                     As_mode;
typedef Alpha_shape_3::Alpha_iterator           As_alpha_iterator;
typedef CGAL::Alpha_status<As_nt>               As_alpha_status;
#endif

#endif
