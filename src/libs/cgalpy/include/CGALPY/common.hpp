// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_COMMON_HPP
#define CGALPY_COMMON_HPP
#define BOOST_PYTHON_STATIC_LIB 1
#define CGAL_DO_NOT_USE_BOOST_MP 1
#include "config.hpp"

#include <boost/python.hpp>
#include <boost/make_shared.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/stl_iterator.hpp>
#include <boost/python/tuple.hpp>
#if CGALPY_KERNEL == CGALPY_EPIC_KERNEL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#else
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#endif
#include <CGAL/CORE_BigInt.h>
#include <CGAL/Sqrt_extension.h>
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_LINEAR_TRAITS
#include <CGAL/Arr_linear_traits_2.h>
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_SEGMENT_TRAITS
#include <CGAL/Arr_segment_traits_2.h>
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_NON_CACHING_SEGMENT_TRAITS
#include <CGAL/Arr_non_caching_segment_traits_2.h>
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_CIRCLE_SEGMENT_TRAITS
#include <CGAL/Arr_circle_segment_traits_2.h>
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_CONIC_TRAITS
#include <CGAL/Arr_conic_traits_2.h>
#endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_ALGEBRAIC_SEGMENT_TRAITS
#include <CGAL/Arr_algebraic_segment_traits_2.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Polynomial_traits_d.h>
#include <CGAL/Polynomial_type_generator.h>
#endif
#include <CGAL/Bbox_2.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Direction_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Aff_transformation_2.h>
#include <CGAL/Arrangement_2.h>
#if CGALPY_DCEL == CGALPY_EXTENDED_DCEL || CGALPY_DCEL == CGALPY_FACE_EXTENDED_DCEL
#include <CGAL/Arr_extended_dcel.h>
#endif
using namespace boost::python;
namespace bp = boost::python;

#if CGALPY_KERNEL == CGALPY_EPIC_KERNEL
typedef typename CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef typename bp::return_value_policy<bp::copy_const_reference>       Kernel_return_value_policy;
#endif
#if CGALPY_KERNEL == CGALPY_EPEC_KERNEL
typedef typename CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef typename bp::return_value_policy<bp::return_by_value>            Kernel_return_value_policy;
#endif

typedef typename CORE::BigInt                                            BigInt;
typedef typename CGAL::Gmpz                                              Gmpz;
typedef typename CGAL::Gmpq                                              Gmpq;
typedef typename Kernel::FT                                              FT;
#if CGALPY_KERNEL == CGALPY_EPEC_KERNEL
typedef typename CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef typename bp::return_value_policy<bp::return_by_value>            Kernel_return_value_policy;
#endif
//typedef typename CGAL::Sqrt_extension <FT, FT>                           CoordNT;
typedef typename Kernel::RT                                              RT;
//typedef typename CGAL::Arr_circle_segment_traits_2<Kernel>::CoordNT      CoordNT;
typedef typename CGAL::Object                                            Object;
typedef typename Kernel::Point_2                                         Point_2;
typedef typename Kernel::Segment_2                                       Segment_2;
typedef typename Kernel::Line_2                                          Line_2;
typedef typename Kernel::Ray_2                                           Ray_2;
typedef typename CGAL::Bbox_2                                            Bbox_2;
typedef typename Kernel::Direction_2                                     Direction_2;
typedef typename Kernel::Vector_2                                        Vector_2;
typedef typename Kernel::Circle_2                                        Circle_2;
typedef typename Kernel::Triangle_2                                      Triangle_2;
typedef typename std::list<Point_2>                                      Point_2_container;
typedef typename CGAL::Polygon_2<Kernel, Point_2_container>              Polygon_2;
typedef typename CGAL::Polygon_with_holes_2<Kernel, Point_2_container>   Polygon_with_holes_2;
typedef typename CGAL::Polygon_set_2<Kernel, Point_2_container>          Polygon_set_2;
typedef typename Kernel::Iso_rectangle_2                                 Iso_rectangle_2;
typedef typename CGAL::Aff_transformation_2<Kernel>                      Aff_transformation_2;

typedef typename Kernel::Point_3                                         Point_3;
typedef typename Kernel::Weighted_point_3                                Weighted_point_3;
typedef typename CGAL::Aff_transformation_3<Kernel>                      Aff_transformation_3;

typedef typename CGAL::Rotation                                          Rotation;
typedef typename CGAL::Scaling                                           Scaling;
typedef typename CGAL::Translation                                       Translation;
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_SEGMENT_TRAITS
typedef typename CGAL::Arr_segment_traits_2<Kernel>                      Traits;
# endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_NON_CACHING_SEGMENT_TRAITS
typedef typename CGAL::Arr_non_caching_segment_traits_2<Kernel>          Traits;
# endif
#if CGALPY_GEOMETRY_TRAITS == CGALPY_ARR_LINEAR_TRAITS
typedef typename CGAL::Arr_linear_traits_2<Kernel>                       Traits;
#endif
#if CGALPY_GEOMETRY_TRAITS ==  CGALPY_ARR_CIRCLE_SEGMENT_TRAITS
typedef typename CGAL::Arr_circle_segment_traits_2<Kernel>               Traits;
#endif
#if CGALPY_GEOMETRY_TRAITS ==  CGALPY_ARR_ALGEBRAIC_SEGMENT_TRAITS
typedef typename BigInt                                                  Integer;
typedef typename CGAL::Arr_algebraic_segment_traits_2<Integer>           Traits;
typedef typename Traits::Construct_curve_2                               Construct_curve_2;
typedef typename Traits::Construct_point_2                               Construct_point_2;
typedef typename Traits::Construct_x_monotone_segment_2                  Construct_x_monotone_segment_2;
typedef typename Traits::Polynomial_2                                    Polynomial_2;
typedef CGAL::Polynomial_traits_d<Polynomial_2>                          PT_2;
typedef PT_2::Construct_polynomial                                       Construct_polynomial_2;
typedef PT_2::Coefficient_type                                           Polynomial_1;
typedef CGAL::Polynomial_traits_d<Polynomial_1>                          PT_1;
typedef PT_1::Construct_polynomial                                       Construct_polynomial_1;
typedef typename Traits::Algebraic_kernel_d_1                            Algebraic_kernel_d_1;
typedef typename Algebraic_kernel_d_1::Polynomial_1                      Polynomial_1;
typedef typename Traits::Algebraic_real_1                                Algebraic_real_1;
typedef typename Traits::Bound                                           Bound;
#endif
typedef typename Traits::Point_2                                         TPoint_2;
typedef typename Traits::Curve_2                                         Curve_2;
typedef typename Traits::X_monotone_curve_2                              X_monotone_curve_2;
#if CGALPY_DCEL ==  CGALPY_EXTENDED_DCEL
typedef CGAL::Arr_extended_dcel<Traits, bp::object, bp::object, bp::object>  Dcel;
typedef CGAL::Arrangement_2<Traits, Dcel>                                Arrangement_2;
#endif
#if CGALPY_DCEL ==  CGALPY_FACE_EXTENDED_DCEL
typedef CGAL::Arr_face_extended_dcel<Traits, bp::object>                 Dcel;
typedef CGAL::Arrangement_2<Traits, Dcel>                                Arrangement_2;
#endif
#if CGALPY_DCEL == CGALPY_DEFAULT_DCEL
typedef typename CGAL::Arrangement_2<Traits>                             Arrangement_2;
#endif

typedef typename Arrangement_2::Vertex_iterator							              Vertex_iterator;
typedef typename Arrangement_2::Vertex_const_handle                       Vertex_const_handle;
typedef typename Arrangement_2::Isolated_vertex_iterator                  Isolated_vertex_iterator;
typedef typename Arrangement_2::Vertex									                  Vertex;
typedef typename Arrangement_2::Inner_ccb_iterator						            Inner_ccb_iterator;
typedef typename Arrangement_2::Ccb_halfedge_circulator					          Ccb_halfedge_circulator;
typedef typename Arrangement_2::Halfedge_around_vertex_circulator		      Halfedge_around_vertex_circulator;
typedef typename Arrangement_2::Halfedge_iterator									        Halfedge_iterator;
typedef typename Arrangement_2::Edge_iterator                             Edge_iterator;
typedef typename Arrangement_2::Halfedge_const_handle                     Halfedge_const_handle;
typedef typename Arrangement_2::Halfedge									                Halfedge;
typedef typename Arrangement_2::Face_iterator							                Face_iterator;
typedef typename Arrangement_2::Face_const_handle                         Face_const_handle;
typedef typename Arrangement_2::Face										                  Face;


#endif //COMMON_HPP
