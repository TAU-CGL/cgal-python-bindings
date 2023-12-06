// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARRANGEMENT_ON_SURFACE_2_TYPES_HPP
#define CGALPY_ARRANGEMENT_ON_SURFACE_2_TYPES_HPP

#include <boost/static_assert.hpp>

#include <CGAL/Arr_dcel.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arrangement_with_history_2.h>
#include <CGAL/Env_surface_data_traits_3.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/arrangement_on_surface_2_config.hpp"
#include "CGALPY/envelope_3_config.hpp"

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
#include <CGAL/Arr_algebraic_segment_traits_2.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Polynomial_traits_d.h>
#include <CGAL/Polynomial_type_generator.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS
#include <CGAL/Arr_Bezier_curve_traits_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
#include <CGAL/Arr_circle_segment_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
#include <CGAL/Arr_linear_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
#include <CGAL/Arr_non_caching_segment_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_polyline_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
#include <CGAL/Arr_segment_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS
#include <CGAL/Arr_rational_function_traits_2.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

namespace aos2 {

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
using Integer = CORE::BigInt;
using Algebraic_kernel_d_1 = CGAL::Algebraic_kernel_d_1<Integer>;
using Algebraic_kernel_d_2 =
  CGAL::Algebraic_curve_kernel_2<Algebraic_kernel_d_1>;
using Polynomial_1 = Algebraic_kernel_d_1::Polynomial_1;
using Polynomial_2 = Algebraic_kernel_d_2::Polynomial_2;
using Algebraic_real_1 = Algebraic_kernel_d_2::Algebraic_real_1;
using Bound = Algebraic_kernel_d_2::Bound;

using PT_2 = CGAL::Polynomial_traits_d<Polynomial_2>;
using Construct_polynomial_2 = PT_2::Construct_polynomial;
using Polynomial_1 = PT_2::Coefficient_type;
using Polynomial_traits_1 = CGAL::Polynomial_traits_d<Polynomial_1>;
using Construct_polynomial_1 = Polynomial_traits_1::Construct_polynomial;

using Agt = CGAL::Arr_algebraic_segment_traits_2<Integer>;
using Construct_curve_2 = typename Agt::Construct_curve_2;
using Construct_point_2 = typename Agt::Construct_point_2;
using Construct_x_monotone_segment_2 = typename Agt::Construct_x_monotone_segment_2;

#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS
using Nt_traits = CGAL::CORE_algebraic_number_traits;
using Rat_kernel = typename CGAL::Cartesian<Nt_traits::Rational>;
using Alg_kernel = typename CGAL::Cartesian<Nt_traits::Algebraic>;
using Agt =
  typename CGAL::Arr_Bezier_curve_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;

#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
using Agt = typename CGAL::Arr_circle_segment_traits_2<Kernel>;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
using Nt_traits = CGAL::CORE_algebraic_number_traits;
using Rat_kernel = typename CGAL::Cartesian<Nt_traits::Rational>;
using Alg_kernel = typename CGAL::Cartesian<Nt_traits::Algebraic>;
using Agt = typename CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;

#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
using Agt = CGAL::Arr_geodesic_arc_on_sphere_traits_2<Kernel>;

#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
using Agt = typename CGAL::Arr_linear_traits_2<Kernel>;

#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
using Agt = typename CGAL::Arr_non_caching_segment_traits_2<Kernel>;

#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS
using Sgt = typename CGAL::Arr_segment_traits_2<Kernel>;
using Agt = typename CGAL::Arr_polyline_traits_2<Sgt>;

#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
using Agt = typename CGAL::Arr_segment_traits_2<Kernel>;
using Is_in_x_range_2 = typename Agt::Is_in_x_range_2;
using Is_in_y_range_2 = typename Agt::Is_in_y_range_2;

#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS
using Integer = CORE::BigInt;
using Rational = CORE::BigRat;
using Algebraic_kernel_d_1 = CGAL::Algebraic_kernel_d_1<Integer>;
using Polynomial_1 = Algebraic_kernel_d_1::Polynomial_1;
using Polynomial_traits_1 = CGAL::Polynomial_traits_d<Polynomial_1>;

using Agt = CGAL::Arr_rational_function_traits_2<Algebraic_kernel_d_1>;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

// 3D Envelope
constexpr bool env_surface_data()
{ return DETECT_EXIST(CGALPY_ENV3_SURFACE_DATA); }

using Base_egt =
  Base_env_tr<envelope_3_bindings(), CGALPY_ENV3_GEOMETRY_TRAITS, Agt>::type;
using Cnv = CGAL::_Default_convert_func<py::object, py::object>;
using Egt = Env_tr<envelope_3_bindings(), env_surface_data(), Base_egt,
                     py::object, py::object, Cnv>::type;

// General polygon
using Ggt = Bso_tr<boolean_set_operations_2_bindings(), Egt>::type;

// Curve Data & Consolidated curve data:
// #if defined(CGALPY_AOS2_WITH_HISTORY)
// using Cd_cv_data = Ggt::Curve_2*;
// using Cd_xcv_data = CGAL::_Unique_list<Cd_cv_data>;
// using Cd_merge = CGAL::_Consolidate_unique_lists<Cd_cv_data>;
// using Cd_convert = CGAL::_Default_convert_func<Cd_cv_data, Cd_xcv_data>;
// #elif defined(CGALPY_AOS2_CONSOLIDATED_CURVE_DATA)
// using Cd_cv_data = py::object;
// using Cd_xcv_data = CGAL::_Unique_list<Cd_cv_data>;
// using Cd_merge = CGAL::_Consolidate_unique_lists<Cd_cv_data>;
// using Cd_convert = CGAL::_Default_convert_func<Cd_cv_data, Cd_xcv_data>;
// #else
// using Cd_cv_data = py::object;
// using Cd_xcv_data = py::object;
// using Cd_merge = py::object;
// using Cd_convert = CGAL::_Default_convert_func<Cd_cv_data, Cd_xcv_data>;
// #endif
using My_cd_tr =
  Cd_tr<aos2_with_history(), aos2_consolidated_curve_data(),
        aos2_curve_data(), Ggt>;
using Cgt = My_cd_tr::Cgt;
using Ccgt = My_cd_tr::Ccgt;

// The arrangement on surface with history is parameterized with a geometry
// traits that os not extended with the consolidated curve-data traits (and
// the curve-data traits itself).
// Define the final geometry traits in the hierarchy:
#if defined(CGALPY_AOS2_WITH_HISTORY)
using Fgt = Ggt;
#else
using Fgt = Ccgt;
#endif

// Define the geometry traits hierarchy:
using Arr_geometry_traits_2 = Agt;
using Env_geometry_traits_2 = Egt;
using Gps_geometry_traits_2 = Ggt;
using Cd_geometry_traits_2 = Cgt;
using Ccd_geometry_traits_2 = Ccgt;
using Final_geometry_traits_2 = Fgt;

// General
using Point_2 = typename Fgt::Point_2;
using Curve_2 = typename Fgt::Curve_2;
using X_monotone_curve_2 = typename Cgt::X_monotone_curve_2;

// Vertex
using Vb = CGAL::Arr_vertex_base<Fgt::Point_2>;
using Vbe = Vertex_env<envelope_3_bindings(), Vb, Fgt>::type;
using V = Vertex_extended<is_vertex_extended(), Vbe, py::object>::type;

// Halfedge
using Hb = Halfedge_gps<boolean_set_operations_2_bindings(), Fgt>::type;
using Hbe = Halfedge_env<envelope_3_bindings(), Hb, Fgt>::type;
using H = Halfedge_extended<is_halfedge_extended(), Hbe, py::object>::type;

// Face
using Fb = Face_gps<boolean_set_operations_2_bindings()>::type;
using Fbe = Face_env<envelope_3_bindings(), Fb, Fgt>::type;
using F = Face_extended<is_face_extended(), Fbe, py::object>::type;

// Define the arrangement-on-surface types:
using Dcel = CGAL::Arr_dcel<Fgt, V, H, F>;
using Arrangement_on_surface_2 =
  With_history<aos2_with_history(), Fgt, Dcel>::Aos;
using Arrangement_2 = With_history<aos2_with_history(), Fgt, Dcel>::Arr;
using Arrangement_on_surface_with_history_2 =
  With_history<aos2_with_history(), Fgt, Dcel>::Aos_with_history;
using Arrangement_with_history_2 =
  With_history<aos2_with_history(), Fgt, Dcel>::Arr_with_history;

// Define the actual traits:
using Geometry_traits_2 = Arrangement_on_surface_2::Geometry_traits_2;
using Topology_traits = Arrangement_on_surface_2::Topology_traits;

//! \todo Add support for Basic geometry traits and x-monotone
// typedef ....                                 Ba_geometry_traits_2;
// typedef ....                                 Xm_geometry_traits_2;
//! \todo Add support for Data-curve geometry traits
//! \todo Add support for counting and tracing geometry traits
// typedef ....                                 Dc_geometry_traits_2;
// typedef ....                                 Trc_geometry_traits_2;

using Vertex_iterator = Arrangement_on_surface_2::Vertex_iterator;
using Halfedge_iterator = Arrangement_on_surface_2::Halfedge_iterator;
using Face_iterator = Arrangement_on_surface_2::Face_iterator;

using Vertex_handle = Arrangement_on_surface_2::Vertex_handle;
using Halfedge_handle = Arrangement_on_surface_2::Halfedge_handle;
using Face_handle = Arrangement_on_surface_2::Face_handle;

using Vertex_const_handle = Arrangement_on_surface_2::Vertex_const_handle;
using Halfedge_const_handle = Arrangement_on_surface_2::Halfedge_const_handle;
using Face_const_handle = Arrangement_on_surface_2::Face_const_handle;

using Vertex = Arrangement_on_surface_2::Vertex;
using Halfedge = Arrangement_on_surface_2::Halfedge;
using Face = Arrangement_on_surface_2::Face;

using Isolated_vertex_iterator =
  Arrangement_on_surface_2::Isolated_vertex_iterator;
using Ccb_halfedge_circulator =
  Arrangement_on_surface_2::Ccb_halfedge_circulator;
using Halfedge_around_vertex_circulator =
  Arrangement_on_surface_2::Halfedge_around_vertex_circulator;
}

#endif //CGALPY_ARRANGEMENT_ON_SURFACE_2_TYPES_HPP
