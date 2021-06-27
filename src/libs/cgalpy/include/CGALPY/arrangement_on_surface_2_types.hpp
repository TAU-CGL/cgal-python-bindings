// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARRANGEMENT_ON_SURFACE_2_TYPES_HPP
#define CGALPY_ARRANGEMENT_ON_SURFACE_2_TYPES_HPP

#include <boost/static_assert.hpp>

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
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

#include <CGAL/Arrangement_2.h>
#if CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL || \
  CGALPY_AOS2_DCEL == CGALPY_AOS2_FACE_EXTENDED_DCEL
#include <CGAL/Arr_extended_dcel.h>
#endif

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
typedef typename CGAL::Arr_segment_traits_2<Kernel>             Traits;
typedef typename Traits::Is_in_x_range_2                        Is_in_x_range_2;
typedef typename Traits::Is_in_y_range_2                        Is_in_y_range_2;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
typedef typename CGAL::Arr_non_caching_segment_traits_2<Kernel> Traits;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
typedef typename CGAL::Arr_linear_traits_2<Kernel>              Traits;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
typedef typename CGAL::Arr_circle_segment_traits_2<Kernel>      Traits;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
typedef typename CGAL::CORE_algebraic_number_traits             NtTraits;
typedef typename CGAL::Cartesian <NtTraits::Rational>           RatKernel;
typedef typename CGAL::Cartesian <NtTraits::Algebraic>          AlgKernel;
typedef typename CGAL::Arr_conic_traits_2<RatKernel,AlgKernel, NtTraits>    Traits;
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
typedef typename CORE::BigInt                                   Integer;
typedef typename CGAL::Arr_algebraic_segment_traits_2<Integer>  Traits;
typedef typename Traits::Construct_curve_2                      Construct_curve_2;
typedef typename Traits::Construct_point_2                      Construct_point_2;
typedef typename Traits::Construct_x_monotone_segment_2         Construct_x_monotone_segment_2;
typedef typename Traits::Polynomial_2                           Polynomial_2;
typedef CGAL::Polynomial_traits_d<Polynomial_2>                 PT_2;
typedef PT_2::Construct_polynomial                              Construct_polynomial_2;
typedef PT_2::Coefficient_type                                  Polynomial_1;
typedef CGAL::Polynomial_traits_d<Polynomial_1>                 PT_1;
typedef PT_1::Construct_polynomial                              Construct_polynomial_1;
typedef typename Traits::Algebraic_kernel_d_1                   Algebraic_kernel_d_1;
typedef typename Algebraic_kernel_d_1::Polynomial_1             Polynomial_1;
typedef typename Traits::Algebraic_real_1                       Algebraic_real_1;
typedef typename Traits::Bound                                  Bound;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

typedef typename Traits::Point_2                                TPoint_2;
typedef typename Traits::Curve_2                                Curve_2;
typedef typename Traits::X_monotone_curve_2                     X_monotone_curve_2;

#if CGALPY_AOS2_DCEL ==  CGALPY_AOS2_EXTENDED_DCEL
typedef CGAL::Arr_extended_dcel<Traits, bp::object, bp::object, bp::object> Dcel;
typedef CGAL::Arrangement_2<Traits, Dcel>                       Arrangement_2;
#elif CGALPY_AOS2_DCEL ==  CGALPY_AOS2_FACE_EXTENDED_DCEL
typedef CGAL::Arr_face_extended_dcel<Traits, bp::object>        Dcel;
typedef CGAL::Arrangement_2<Traits, Dcel>                       Arrangement_2;
#elif CGALPY_AOS2_DCEL == CGALPY_AOS2_DEFAULT_DCEL
typedef typename CGAL::Arrangement_2<Traits>                    Arrangement_2;
#else
BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_DCEL");
#endif

typedef typename Arrangement_2::Point_2                         Arr_point_2;
typedef typename Arrangement_2::Vertex_iterator                 Vertex_iterator;
typedef typename Arrangement_2::Vertex_const_handle             Vertex_const_handle;
typedef typename Arrangement_2::Isolated_vertex_iterator        Isolated_vertex_iterator;
typedef typename Arrangement_2::Vertex                          Vertex;
typedef typename Arrangement_2::Inner_ccb_iterator              Inner_ccb_iterator;
typedef typename Arrangement_2::Ccb_halfedge_circulator         Ccb_halfedge_circulator;
typedef typename Arrangement_2::Halfedge_around_vertex_circulator Halfedge_around_vertex_circulator;
typedef typename Arrangement_2::Halfedge_iterator               Halfedge_iterator;
typedef typename Arrangement_2::Edge_iterator                   Edge_iterator;
typedef typename Arrangement_2::Halfedge_const_handle           Halfedge_const_handle;
typedef typename Arrangement_2::Halfedge                        Halfedge;
typedef typename Arrangement_2::Face_iterator                   Face_iterator;
typedef typename Arrangement_2::Face_const_handle               Face_const_handle;
typedef typename Arrangement_2::Face                            Face;

#endif //CGALPY_ARRANGEMENT_ON_SURFACE_2_TYPES_HPP
