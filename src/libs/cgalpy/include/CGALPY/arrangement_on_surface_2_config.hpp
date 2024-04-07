// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARRANGEMENT_ON_SURFACE_2_CONFIG_HPP
#define CGALPY_ARRANGEMENT_ON_SURFACE_2_CONFIG_HPP

#include <nanobind/nanobind.h>

#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arr_non_caching_segment_traits_2.h>
#include <CGAL/Arr_circle_segment_traits_2.h>
#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/Envelope_3/Envelope_pm_dcel.h>
#include <CGAL/Arrangement_on_surface_2.h>
#include <CGAL/Arrangement_on_surface_with_history_2.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Arrangement_with_history_2.h>
#include <CGAL/Arr_spherical_topology_traits_2.h>
#include <CGAL/Gps_traits_2.h>
#include <CGAL/Gps_segment_traits_2.h>
#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/Boolean_set_operations_2/Gps_default_dcel.h>
#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>
#include <CGAL/Arr_curve_data_traits_2.h>
#include <CGAL/Arr_consolidated_curve_data_traits_2.h>
#include <CGAL/Env_sphere_traits_3.h>
#include <CGAL/Env_triangle_traits_3.h>
#include <CGAL/Env_surface_data_traits_3.h>

#include "CGALPY/config.hpp"
#include "CGALPY/envelope_3_config.hpp"
#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

// Geometry traits options
#define CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS           0
#define CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS                      1
#define CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS              2
#define CGALPY_AOS2_CONIC_GEOMETRY_TRAITS                       3
#define CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS      4
#define CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS                      5
#define CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS         6
#define CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS        7
#define CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS                     8
#define CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS           9

#ifndef CGALPY_AOS2_GEOMETRY_TRAITS
#define CGALPY_AOS2_GEOMETRY_TRAITS                             8
#endif

namespace aos2 {

// Indicates whether arrangement with history is prescribed
constexpr bool aos2_with_history()
{ return DETECT_EXIST(CGALPY_AOS2_WITH_HISTORY); }

// Indicates whether the curve type is extended with consolidated data
constexpr bool aos2_consolidated_curve_data()
{ return DETECT_EXIST(CGALPY_AOS2_CONSOLIDATED_CURVE_DATA); }

// Indicates whether the curve type is extended with data
constexpr bool aos2_curve_data()
{ return DETECT_EXIST(CGALPY_AOS2_CURVE_DATA); }

// Indicates whether the vertex type is extended
constexpr bool is_vertex_extended()
{ return DETECT_EXIST(CGALPY_AOS2_VERTEX_EXTENDED); }

// Indicates whether the halfedge type is extended
constexpr bool is_halfedge_extended()
{ return DETECT_EXIST(CGALPY_AOS2_HALFEDGE_EXTENDED); }

// Indicates whether the halfedge type is extended
constexpr bool is_face_extended()
{ return DETECT_EXIST(CGALPY_AOS2_FACE_EXTENDED); }

// Curve data & Consolidated curve data
template <bool wh, bool ccd, bool cd, typename Btr> struct Cd_tr {};

template <typename Btr>
struct Cd_tr<false, false, false, Btr> {
  using Cgt = Btr;
  using Ccgt = Btr;
};

class Curve_data_merge {
public:
  /*! Construct
   */
  Curve_data_merge() {}

  /*! Destruct
   */
  ~Curve_data_merge() {}

  static void set_func(py::object func) { s_func = func; }
  static void reset_func() { s_func = py::none(); }
  static py::object func() { return s_func; }

  /*! Apply the callback function
   */
  py::object operator()(py::object a, py::object b) const
  { return (s_func.is_none()) ? a : s_func(a, b); }

private:
  //! The callback function to apply
  inline static py::object s_func = py::none();
};

template <typename Btr>
struct Cd_tr<false, false, true, Btr> {
  using Cv_data = py::object;
  using Xcv_data = py::object;
  using Merge = Curve_data_merge;
  using Convert = CGAL::_Default_convert_func<Cv_data, Xcv_data>;
  using Cgt =
    CGAL::Arr_curve_data_traits_2<Btr, Xcv_data, Merge, Cv_data, Convert>;
  using Ccgt = Cgt;
};

template <typename Btr>
struct Cd_tr<false, true, true, Btr> {
  using Cv_data = py::object;
  using Xcv_data = CGAL::_Unique_list<Cv_data>;
  using Merge = CGAL::_Consolidate_unique_lists<Cv_data>;
  using Convert = CGAL::_Default_convert_func<Cv_data, Xcv_data>;
  using Cgt =
    CGAL::Arr_curve_data_traits_2<Btr, Xcv_data, Merge, Cv_data, Convert>;
  using Ccgt = CGAL::Arr_consolidated_curve_data_traits_2<Btr, Cv_data>;
};

template <typename Btr>
struct Cd_tr<true, true, true, Btr> {
  using Cv_data = typename Btr::Curve_2*;
  using Xcv_data = CGAL::_Unique_list<Cv_data>;
  using Merge = CGAL::_Consolidate_unique_lists<Cv_data>;
  using Convert = CGAL::_Default_convert_func<Cv_data, Xcv_data>;
  using Cgt =
    CGAL::Arr_curve_data_traits_2<Btr, Xcv_data, Merge, Cv_data, Convert>;
  using Ccgt = CGAL::Arr_consolidated_curve_data_traits_2<Btr, Cv_data>;
};

// 3D Envelope traits
template <bool b, int i, typename Base> struct Base_env_tr {};

template <int i, typename Base>
struct Base_env_tr<false, i, Base> { using type = Base; };

#ifdef CGALPY_ENVELOPE_3_BINDINGS
template <typename Base>
struct Base_env_tr<true, CGALPY_ENV3_PLANE_GEOMETRY_TRAITS, Base> {
  using Linear_tr = CGAL::Arr_linear_traits_2<Kernel>;
  BOOST_STATIC_ASSERT_MSG(std::is_same<Base, Linear_tr>::value,
                          "Mismatch Aos2/Env3 geometry traits!");
  using type = CGAL::Env_plane_traits_3<Kernel>;
};

template <typename Base>
struct Base_env_tr<true, CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS, Base> {
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rat_kernel = CGAL::Cartesian<Nt_traits::Rational>;
  using Alg_kernel = CGAL::Cartesian<Nt_traits::Algebraic>;
  using Conic_tr = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  BOOST_STATIC_ASSERT_MSG(std::is_same<Base, Conic_tr>::value,
                          "Mismatch Aos2/Env3 geometry traits!");
  using type = CGAL::Env_sphere_traits_3<Base>;
};

template <typename Base>
struct Base_env_tr<true, CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS, Base> {
  using Seg_tr = CGAL::Arr_segment_traits_2<Kernel>;
  BOOST_STATIC_ASSERT_MSG(std::is_same<Base, Seg_tr>::value,
                          "Mismatch Aos2/Env3 geometry traits!");
  using type = CGAL::Env_triangle_traits_3<Kernel>;
};
#endif

// Surface data

template <bool e, bool b, typename Btr, typename XyData, typename SData,
          typename Cnv>
struct Env_tr {};

template <bool b, typename Btr, typename XyData, typename SData, typename Cnv>
struct Env_tr<false, b, Btr, XyData, SData, Cnv> { using type = Btr; };

template <typename Btr, typename XyData, typename SData, typename Cnv>
struct Env_tr<true, false, Btr, XyData, SData, Cnv> { using type = Btr; };

template <typename Btr, typename XyData, typename SData, typename Cnv>
struct Env_tr<true, true, Btr, XyData, SData, Cnv>
{ using type = CGAL::Env_surface_data_traits_3<Btr, XyData, SData, Cnv>; };

// 2D regualrized Boolean set operation traits

template <bool b, typename Base> struct Bso_tr {};
template <typename Base>
struct Bso_tr<false, Base> { using type = Base; };
template <>
struct Bso_tr<true, CGAL::Arr_segment_traits_2<Kernel>> {
  using Base_gt = CGAL::Arr_segment_traits_2<Kernel>;
  using type = CGAL::Gps_segment_traits_2<Kernel, Point_2_container, Base_gt>;
};
template <>
struct Bso_tr<true, CGAL::Arr_non_caching_segment_traits_2<Kernel>> {
  using Base_gt = CGAL::Arr_non_caching_segment_traits_2<Kernel>;
  using type = CGAL::Gps_segment_traits_2<Kernel, Point_2_container, Base_gt>;
};
template <>
struct Bso_tr<true, CGAL::Arr_circle_segment_traits_2<Kernel>> {
  using type = CGAL::Gps_circle_segment_traits_2<Kernel>;
};
template <typename Base>
struct Bso_tr<true, Base> { using type = CGAL::Gps_traits_2<Base>; };

// Boolean set operations extension

template <bool b, typename Tr> struct Halfedge_gps {};
template <typename Tr>
struct Halfedge_gps<false, Tr>
{ using type = CGAL::Arr_halfedge_base<typename Tr::X_monotone_curve_2>; };
template <typename Tr>
struct Halfedge_gps<true, Tr>
{ using type = CGAL::Gps_halfedge_base<typename Tr::X_monotone_curve_2>; };

template <bool b> struct Face_gps {};
template <> struct Face_gps<false> { using type = CGAL::Arr_face_base; };
template <> struct Face_gps<true> { using type = CGAL::Gps_face_base; };

// Envelope 3 extension

template <bool b, typename Vb, typename Tr> struct Vertex_env {};
template <typename Vb, typename Tr>
struct Vertex_env<false, Vb, Tr> { using type = Vb; };
template <typename Vb, typename Tr>
struct Vertex_env<true, Vb, Tr> {
  using Data = typename Tr::Xy_monotone_surface_3;
  using type = CGAL::Envelope_3::Envelope_pm_vertex<Vb, Data>;
};

template <bool b, typename Vb, typename Tr> struct Halfedge_env {};
template <typename Vb, typename Tr>
struct Halfedge_env<false, Vb, Tr> { using type = Vb; };
template <typename Vb, typename Tr>
struct Halfedge_env<true, Vb, Tr> {
  using Data = typename Tr::Xy_monotone_surface_3;
  using type = CGAL::Envelope_3::Envelope_pm_halfedge<Vb, Data>;
};

template <bool b, typename Vb, typename Tr> struct Face_env {};
template <typename Vb, typename Tr>
struct Face_env<false, Vb, Tr> { using type = Vb; };
template <typename Vb, typename Tr>
struct Face_env<true, Vb, Tr> {
  using Data = typename Tr::Xy_monotone_surface_3;
  using type = CGAL::Envelope_3::Envelope_pm_face<Vb, Data>;
};

// General extension

template <bool b, typename Vb, typename Data> struct Vertex_extended {};
template <typename Vb, typename Data>
struct Vertex_extended<false, Vb, Data> { using type = Vb; };
template <typename Vb, typename Data>
struct Vertex_extended<true, Vb, Data>
{ using type = CGAL::Arr_extended_vertex<Vb, Data>; };

template <bool b, typename Hb, typename Data> struct Halfedge_extended {};
template <typename Hb, typename Data>
struct Halfedge_extended<false, Hb, Data> { using type = Hb; };
template <typename Hb, typename Data> struct Halfedge_extended<true, Hb, Data>
{ using type = CGAL::Arr_extended_halfedge<Hb, Data>; };

template <bool b, typename Fb, typename Data> struct Face_extended {};
template <typename Fb, typename Data>
struct Face_extended<false, Fb, Data> { using type = Fb; };
template <typename Fb, typename Data> struct Face_extended<true, Fb, Data>
{ using type = CGAL::Arr_extended_face<Fb, Data>; };

// Aos types
template <bool WithHistory, typename GeomTraits, typename TopolTraits>
struct With_history {};

template <typename GeomTraits, typename TopolTraits>
struct With_history<false, GeomTraits, TopolTraits> {
  using Gt = GeomTraits;
  using Tt = TopolTraits;
  using Aos = CGAL::Arrangement_on_surface_2<Gt, Tt>;
  using Aos_with_history = void;

  using Dcel = typename Tt::Dcel;
  using Arr = CGAL::Arrangement_2<Gt, Dcel>;
  using Arr_with_history = void;
};

template <typename GeomTraits, typename TopolTraits>
struct With_history<true, GeomTraits, TopolTraits> {
  using Gt = GeomTraits;
  using Tt = TopolTraits;
  using Aos_with_history = CGAL::Arrangement_on_surface_with_history_2<Gt, Tt>;
  using Aos = typename Aos_with_history::Base_arrangement_2;
  using Geometry_traits_2 = typename Aos::Geometry_traits_2;

  // using Dcel = typename Tt::Dcel;
  // using Arr = CGAL::Arrangement_2<Geometry_traits_2, typename Aos::Dcel>;
  // using Arr_with_history = CGAL::Arrangement_with_history_2<Gt, Dcel>;
};

// Geometry traits types:
//using Gaost = CGAL::Arr_geodesic_arc_on_sphere_traits_2<Kernel>;

// template <typename Dcel>
// struct Aos<Gaost, Dcel> {
//   using Topol_traits = CGAL::Arr_spherical_topology_traits_2<Gaost, Dcel>;
//   using aos = CGAL::Arrangement_on_surface_2<Gaost, Topol_traits>;
//   using aos_with_history =
//     CGAL::Arrangement_on_surface_with_history_2<Gaost, Topol_traits>;
// };

} // end of aos2 namespace

#endif //CGALPY_ARRANGEMENT_ON_SURFACE_2_CONFIG_HPP
