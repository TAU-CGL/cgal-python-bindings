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

#include <CGAL/Arr_default_dcel.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arrangement_with_history_2.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/arrangement_on_surface_2_config.hpp"

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
#include <CGAL/Arr_linear_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
#include <CGAL/Arr_segment_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
#include <CGAL/Arr_non_caching_segment_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
#include <CGAL/Arr_circle_segment_traits_2.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
#include <CGAL/Arr_algebraic_segment_traits_2.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Polynomial_traits_d.h>
#include <CGAL/Polynomial_type_generator.h>
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

namespace aos2 {

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
typedef typename CGAL::Arr_segment_traits_2<Kernel>             BGT;
typedef typename BGT::Is_in_x_range_2                           Is_in_x_range_2;
typedef typename BGT::Is_in_y_range_2                           Is_in_y_range_2;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
typedef typename CGAL::Arr_non_caching_segment_traits_2<Kernel> BGT;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
typedef typename CGAL::Arr_linear_traits_2<Kernel>              BGT;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
typedef typename CGAL::Arr_circle_segment_traits_2<Kernel>      BGT;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
typedef typename CGAL::CORE_algebraic_number_traits             NtTraits;
typedef typename CGAL::Cartesian <NtTraits::Rational>           RatKernel;
typedef typename CGAL::Cartesian <NtTraits::Algebraic>          AlgKernel;
typedef typename CGAL::Arr_conic_traits_2<RatKernel,AlgKernel, NtTraits>    BGT;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
typedef CORE::BigInt                                            Integer;
typedef CGAL::Algebraic_kernel_d_1<Integer>                     Algebraic_kernel_d_1;
typedef CGAL::Algebraic_curve_kernel_2<Algebraic_kernel_d_1>    Algebraic_kernel_d_2;
typedef Algebraic_kernel_d_1::Polynomial_1                      Polynomial_1;
typedef Algebraic_kernel_d_2::Polynomial_2                      Polynomial_2;
typedef Algebraic_kernel_d_2::Algebraic_real_1                  Algebraic_real_1;
typedef Algebraic_kernel_d_2::Bound                             Bound;

typedef CGAL::Polynomial_traits_d<Polynomial_2>                 PT_2;
typedef PT_2::Construct_polynomial                              Construct_polynomial_2;
typedef PT_2::Coefficient_type                                  Polynomial_1;
typedef CGAL::Polynomial_traits_d<Polynomial_1>                 PT_1;
typedef PT_1::Construct_polynomial                              Construct_polynomial_1;

typedef typename CGAL::Arr_algebraic_segment_traits_2<Integer>  BGT;
typedef typename BGT::Construct_curve_2                         Construct_curve_2;
typedef typename BGT::Construct_point_2                         Construct_point_2;
typedef typename BGT::Construct_x_monotone_segment_2            Construct_x_monotone_segment_2;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
typedef CGAL::Arr_geodesic_arc_on_sphere_traits_2<Kernel>       BGT;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

typedef Tr<boolean_set_operations_2_bindings(), BGT>::type      GT;
typedef typename GT::Point_2                                    Point_2;
typedef typename GT::Curve_2                                    Curve_2;
typedef typename GT::X_monotone_curve_2                         X_monotone_curve_2;

typedef CGAL::Arr_vertex_base<GT::Point_2>                              Vb;
typedef Vertex_extended<is_vertex_extended(), Vb, py::object>::type     V;

typedef Halfedge_gps<boolean_set_operations_2_bindings(), GT>::type     Hb;
typedef Halfedge_extended<is_halfedge_extended(), Hb, py::object>::type H;

typedef Face_gps<boolean_set_operations_2_bindings()>::type             Fb;
typedef Face_extended<is_face_extended(), Fb, py::object>::type         F;

typedef CGAL::Arr_dcel_base<V, H, F>                        Dcel;
typedef Aos<CGALPY_AOS2_TYPE, GT, Dcel>::aos                Arrangement_on_surface_2;
typedef Aos<CGALPY_AOS2_TYPE, GT, Dcel>::arr                Arrangement_2;
typedef Aos<CGALPY_AOS2_TYPE, GT, Dcel>::arr_with_history   Arrangement_with_history_2;

// Backward compatibility:
typedef Arrangement_2::Geometry_traits_2                    Geometry_traits_2;
typedef Arrangement_2::Point_2                              Arr_point_2;
typedef Arrangement_2::Vertex_iterator                      Vertex_iterator;
typedef Arrangement_2::Vertex_const_handle                  Vertex_const_handle;
typedef Arrangement_2::Isolated_vertex_iterator             Isolated_vertex_iterator;
typedef Arrangement_2::Vertex                               Vertex;
typedef Arrangement_2::Inner_ccb_iterator                   Inner_ccb_iterator;
typedef Arrangement_2::Ccb_halfedge_circulator              Ccb_halfedge_circulator;
typedef Arrangement_2::Halfedge_around_vertex_circulator    Halfedge_around_vertex_circulator;
typedef Arrangement_2::Halfedge_iterator                    Halfedge_iterator;
typedef Arrangement_2::Edge_iterator                        Edge_iterator;
typedef Arrangement_2::Halfedge_const_handle                Halfedge_const_handle;
typedef Arrangement_2::Halfedge                             Halfedge;
typedef Arrangement_2::Face_iterator                        Face_iterator;
typedef Arrangement_2::Face_const_handle                    Face_const_handle;
typedef Arrangement_2::Face                                 Face;

}

#endif //CGALPY_ARRANGEMENT_ON_SURFACE_2_TYPES_HPP
