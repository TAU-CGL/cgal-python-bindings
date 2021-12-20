// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>
#include <boost/static_assert.hpp>
#include <boost/python/stl_iterator.hpp>

#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_vertical_decomposition_2.h>
#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>

#include "CGALPY/Python_functor.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include <CGALPY/Arr_overlay_traits.hpp>
#include <CGALPY/Arr_overlay_function_traits.hpp>

void export_vertex();
void export_halfedge();
void export_face();

bp::class_<aos2::Geometry_traits_2> export_arr_linear_traits();
bp::class_<aos2::Geometry_traits_2> export_arr_segment_traits();
bp::class_<aos2::Geometry_traits_2> export_arr_non_caching_segment_traits();
bp::class_<aos2::Geometry_traits_2> export_arr_circle_segment_traits();
bp::class_<aos2::Geometry_traits_2> export_arr_conic_traits();
bp::class_<aos2::Geometry_traits_2> export_arr_algebraic_segment_traits();
bp::class_<aos2::Geometry_traits_2> export_arr_geodesic_arc_on_sphere_traits();

bp::object export_gps_segment_traits();
bp::object export_gps_circle_segment_traits();
bp::object export_gps_traits();

namespace aos2 {

namespace bp = boost::python;

typedef typename boost::variant<Vertex_const_handle, Halfedge_const_handle,
                                Face_const_handle>      variant;

typedef Arr_overlay_function_traits<Arrangement_2, Arrangement_2, Arrangement_2,
                                    bp::object> Arr_overlay_function_traits;
typedef Arr_overlay_traits<Arrangement_2, Arrangement_2, Arrangement_2,
                                    bp::object> Arr_overlay_traits;

// Free functions
Vertex& insert_point_default(Arrangement_2& arr, Point_2& p)
{ return *(CGAL::insert_point(arr, p)); }

template <typename PointLocation>
Vertex& insert_point(Arrangement_2& arr, Point_2& p, PointLocation& pl)
{ return *(CGAL::insert_point(arr, p, pl)); }

template<typename CurveType>
void insert_curve(Arrangement_2& arr, CurveType& c) { CGAL::insert(arr, c); }

void insert_curves(Arrangement_2& arr, bp::list& lst) {
  if (!lst) return;
  if (bp::extract<X_monotone_curve_2>(lst[0]).check()) {
    // copying into a vector because of an apparent bug with
    // bp::stl_input_iterator
    auto begin = bp::stl_input_iterator< X_monotone_curve_2 >(lst);
    auto end = bp::stl_input_iterator< X_monotone_curve_2 >();
    auto v = std::vector<X_monotone_curve_2>(begin, end);
    CGAL::insert(arr, v.begin(), v.end());
  }
  else if (bp::extract<Curve_2>(lst[0]).check()) {
    // copying into a vector because of an apparent bug with
    // bp::stl_input_iterator
    auto begin = bp::stl_input_iterator< Curve_2 >(lst);
    auto end = bp::stl_input_iterator< Curve_2 >();
    auto v = std::vector<Curve_2>(begin, end);
    CGAL::insert(arr, v.begin(), v.end());
  }
}

Halfedge& insert_non_intersecting_curve_default(Arrangement_2& arr,
                                                X_monotone_curve_2& c)
{ return *(CGAL::insert_non_intersecting_curve(arr, c)); }

template <typename PointLocation>
Halfedge& insert_non_intersecting_curve(Arrangement_2& arr,
                                        X_monotone_curve_2& c, PointLocation& pl)
{ return *(CGAL::insert_non_intersecting_curve(arr, c, pl)); }

void insert_non_intersecting_curves(Arrangement_2& arr, bp::list& lst) {
  // copying into a vector because of an apparent bug with
  // bp::stl_input_iterator
  auto begin = bp::stl_input_iterator< X_monotone_curve_2 >(lst);
  auto end = bp::stl_input_iterator< X_monotone_curve_2 >();
  auto v = std::vector<X_monotone_curve_2>(begin, end);
  CGAL::insert_non_intersecting_curves(arr, v.begin(), v.end());
}

template <typename CurveType>
bool do_intersect_default(Arrangement_2& arr, CurveType& c)
{ return CGAL::do_intersect(arr, c); }

template <typename CurveType, typename PointLocation>
bool do_intersect(Arrangement_2& arr, X_monotone_curve_2& c, PointLocation& pl)
{ return CGAL::do_intersect(arr, c, pl); }

//
template <typename OverlayTraits>
void overlay(Arrangement_2& arr1, Arrangement_2& arr2,
             Arrangement_2& arr_res,
             OverlayTraits& traits)
{ CGAL::overlay(arr1, arr2, arr_res, traits); }

//
Face& remove_edge_free(Arrangement_2& arr, Halfedge& e) {
  auto handle = e.twin();
  return *(CGAL::remove_edge(arr, handle));
}

bool remove_vertex_free(Arrangement_2& arr, Vertex& v) {
  return CGAL::remove_vertex(arr, Vertex_iterator(&v));
}

template<typename T1, typename T2>
void decompose_helper2(const Vertex& vertex, T1& below,
                       T2& above, bp::list& lst) {
  bp::tuple inner = bp::make_tuple(below, above);
  bp::tuple outer = bp::make_tuple(vertex, inner);
  lst.append(outer);
}

template<typename T1>
void decompose_helper1(const Vertex& vertex, T1& below,
                       boost::optional<variant>& above, bp::list& lst) {
  if (!above) {
    bp::object none;
    decompose_helper2<T1, bp::object>(vertex, below, none, lst);
    return;
  }
  auto var = (above.get());
  if (Vertex_const_handle* v = boost::get<Vertex_const_handle>(&var)) {
    decompose_helper2<T1, const Vertex>(vertex, below, *(*v), lst);
  }
  else if (Halfedge_const_handle* e = boost::get<Halfedge_const_handle>(&var)) {
    decompose_helper2<T1, const Halfedge>(vertex, below, *(*e), lst);
  }
  else if (Face_const_handle* f = boost::get<Face_const_handle>(&var)) {
    decompose_helper2<T1, const Face>(vertex, below, *(*f), lst);
  }
}

void decompose_helper(const Vertex& vertex, boost::optional<variant>& below,
                      boost::optional<variant>& above, bp::list& lst) {
  if (!below) {
    bp::object none;
    decompose_helper1<bp::object>(vertex, none, above, lst);
    return;
  }
  auto var = (below.get());
  if (Vertex_const_handle* v = boost::get<Vertex_const_handle>(&var)) {
    decompose_helper1<const Vertex>(vertex, *(*v), above, lst);
   }
  else if (Halfedge_const_handle* e = boost::get<Halfedge_const_handle>(&var)) {
    decompose_helper1<const Halfedge>(vertex, *(*e), above, lst);
  }
  else if (Face_const_handle* f = boost::get<Face_const_handle>(&var)) {
    decompose_helper1<const Face>(vertex, *(*f), above, lst);
  }
}

void decompose(Arrangement_2& arr, bp::list& lst) {
  auto v = std::vector<std::pair<Arrangement_2::Vertex_const_handle,
                                 std::pair<boost::optional<variant>,
                                   boost::optional<variant>>>>();
  CGAL::decompose(arr, std::back_inserter(v));
  for (auto o : v) {
    const Vertex& vertex = *(o.first);
    boost::optional<variant>& below = o.second.first;
    boost::optional<variant>& above = o.second.second;
    decompose_helper(vertex, below, above, lst);
  }
}

void zone_default(Arrangement_2& arr, X_monotone_curve_2& c, bp::list& res) {
  auto v = std::vector<variant>();
  CGAL::zone(arr, c, std::back_inserter(v));
  for (auto o : v) {
    if (Vertex_const_handle* v = boost::get<Vertex_const_handle>(&o)) {
      res.append(*(*(v)));
    } else if (Halfedge_const_handle* h =
        boost::get<Halfedge_const_handle>(&o)) {
          res.append(*(*(h)));
    } else if (Face_const_handle* f = boost::get<Face_const_handle>(&o)) {
        res.append(*(*(f)));
    }
  }
}

template <typename PointLocation>
void zone(Arrangement_2& arr, X_monotone_curve_2& c, bp::list& res,
          PointLocation& pl) {
  auto v = std::vector<variant>();
  CGAL::zone(arr, c, std::back_inserter(v), pl);
  for (auto o : v) {
    if (Vertex_const_handle* v = boost::get<Vertex_const_handle>(&o)) {
      res.append(*(*(v)));
    } else if (Halfedge_const_handle* h =
      boost::get<Halfedge_const_handle>(&o)) {
      res.append(*(*(h)));
    } else if (Face_const_handle* f = boost::get<Face_const_handle>(&o)) {
      res.append(*(*(f)));
    }
  }
}

// Arrangement methods
template <typename Aos>
typename Aos::Halfedge& insert_from_left_vertex(Aos& arr,
                                                typename Aos::X_monotone_curve_2& c,
                                                Vertex& v)
{ return *(arr.insert_from_left_vertex(c,(Vertex_iterator(&v)))); }

template <typename Aos>
typename Aos::Halfedge& insert_from_right_vertex(Aos& arr,
                                                 typename Aos::X_monotone_curve_2& c,
                                                 Vertex& v)
{ return *(arr.insert_from_right_vertex(c, (Vertex_iterator(&v)))); }

template <typename Aos>
typename Aos::Halfedge& insert_edge_in_face_interior(Aos& arr,
                                                     typename Aos::X_monotone_curve_2& c,
                                                     typename Aos::Face& f)
{ return *(arr.insert_in_face_interior(c, (Face_iterator(&f)))); }

template <typename Aos>
typename Aos::Vertex& insert_vertex_in_face_interior(Aos& arr,
                                                     typename Aos::Point_2& p,
                                                     typename Aos::Face& f)
{ return *(arr.insert_in_face_interior(p, (Face_iterator(&f)))); }

template <typename Aos>
typename Aos::Halfedge& insert_at_vertices(Aos& arr,
                                           typename Aos::X_monotone_curve_2& c,
                                           typename Aos::Vertex& v1,
                                           typename Aos::Vertex& v2) {
  return *(arr.insert_at_vertices(c, (Vertex_iterator(&v1)),
                                  (Vertex_iterator(&v2))));
}

template <typename Aos>
typename Aos::Vertex& modify_vertex(Aos& arr,
                                    typename Aos::Vertex& v,
                                    typename Aos::Point_2& p)
{ return *(arr.modify_vertex(Vertex_iterator(&v), p)); }

template <typename Aos>
typename Aos::Face& remove_isolated_vertex(Aos& arr,
                                           typename Aos::Vertex& v)
{ return *(arr.remove_isolated_vertex(Vertex_iterator(&v))); }

template <typename Aos>
typename Aos::Halfedge& modify_edge(Aos& arr,
                                    typename Aos::Halfedge& e,
                                    typename Aos::X_monotone_curve_2& c)
{ return *(arr.modify_edge(Halfedge_iterator(&e), c)); }

template <typename Aos>
typename Aos::Halfedge& split_edge(Aos& arr,
                                   typename Aos::Halfedge& e,
                                   typename Aos::X_monotone_curve_2& c1,
                     X_monotone_curve_2& c2)
{ return *(arr.split_edge(Halfedge_iterator(&e), c1, c2)); }

template <typename Aos>
typename Aos::Halfedge& merge_edge(Aos& arr,
                                   typename Aos::Halfedge& e1,
                                   typename Aos::Halfedge& e2,
                     X_monotone_curve_2& c)
{ return *(arr.merge_edge(Halfedge_iterator(&e1), Halfedge_iterator(&e2), c)); }

template <typename Aos>
typename Aos::Face& remove_edge(Aos& arr,
                                typename Aos::Halfedge& e) {
  auto handle = e.twin();
  return *(arr.remove_edge(handle));
}

template <typename Aos>
typename Aos::Vertex_iterator vertices_begin(Aos& arr)
{ return arr.vertices_begin(); }

template <typename Aos>
typename Aos::Vertex_iterator vertices_end(Aos& arr)
{ return arr.vertices_end(); }

template <typename Aos>
typename Aos::Halfedge_iterator halfedges_begin(Aos& arr)
{ return arr.halfedges_begin(); }

template <typename Aos>
typename Aos::Halfedge_iterator halfedges_end(Aos& arr)
{ return arr.halfedges_end(); }

template <typename Aos>
typename Aos::Edge_iterator edges_begin(Aos& arr) { return arr.edges_begin(); }

template <typename Aos>
typename Aos::Edge_iterator edges_end(Aos& arr) { return arr.edges_end(); }

template <typename Aos>
typename Aos::Face_iterator faces_begin(Aos& arr) { return arr.faces_begin(); }

template <typename Aos>
typename Aos::Face_iterator faces_end(Aos& arr) { return arr.faces_end(); }

template <typename Aos>
typename Aos::Face_iterator unbounded_faces_begin(Aos& arr)
{ return arr.unbounded_faces_begin(); }

template <typename Aos>
typename Aos::Face_iterator unbounded_faces_end(Aos& arr)
{ return arr.unbounded_faces_end(); }

template <typename Aos>
typename Aos::Face& unbounded_face(Aos& arr)
{ return *(arr.unbounded_face()); }

template <typename Aos>
typename Aos::Face& fictitious_face(Aos& arr)
{ return *(arr.fictitious_face()); }

template <typename Aos>
void assign(Aos& arr, Aos& input_arr) { arr.assign(input_arr); }

}

// Export common members of Aos types
template <typename Aos>
void export_aos(bp::class_<Aos>& co) {
  typedef bp::return_internal_reference<>                 RIR;

  co
#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT
    .def(bp::init<>())
    .def(bp::init<Aos&>())
    .def("halfedges", bp::range<RIR>(&aos2::halfedges_begin<Aos>, &aos2::halfedges_end<Aos>))
    .def("vertices", bp::range<RIR>(&aos2::vertices_begin<Aos>, &aos2::vertices_end<Aos>))
    .def("faces", bp::range<RIR>(&aos2::faces_begin<Aos>, &aos2::faces_end<Aos>))
    .def("edges", bp::range<RIR>(&aos2::edges_begin<Aos>, &aos2::edges_end<Aos>))
    .def("unbounded_faces", bp::range<RIR>(&aos2::unbounded_faces_begin<Aos>, &aos2::unbounded_faces_end<Aos>))
    .def("fictitious_face", &aos2::fictitious_face<Aos>, RIR())
    .def("insert_from_left_vertex", &aos2::insert_from_left_vertex<Aos>, RIR())
    .def("insert_from_right_vertex", &aos2::insert_from_right_vertex<Aos>, RIR())
    .def("insert_in_face_interior", &aos2::insert_edge_in_face_interior<Aos>, RIR())
    .def("insert_in_face_interior", &aos2::insert_vertex_in_face_interior<Aos>, RIR())
    .def("insert_at_vertices", &aos2::insert_at_vertices<Aos>, RIR())
    .def("modify_vertex", &aos2::modify_vertex<Aos>, RIR())
    .def("remove_isolated_vertex", &aos2::remove_isolated_vertex<Aos>, RIR())
    .def("modify_edge", &aos2::modify_edge<Aos>, RIR())
    .def("split_edge", &aos2::split_edge<Aos>, RIR())
    .def("merge_edge", &aos2::merge_edge<Aos>, RIR())
    .def("remove_edge", &aos2::remove_edge<Aos>, RIR())
    .def("is_empty", &Aos::is_empty)
    .def("is_valid", &Aos::is_valid)
    .def("number_of_edges", &Aos::number_of_edges)
    .def("number_of_faces", &Aos::number_of_faces)
    .def("number_of_halfedges", &Aos::number_of_halfedges)
    .def("number_of_isolated_vertices", &Aos::number_of_isolated_vertices)
    .def("number_of_unbounded_faces", &Aos::number_of_unbounded_faces)
    .def("number_of_vertices", &Aos::number_of_vertices)
    .def("assign", &aos2::assign<Aos>)
    .def("clear", &Aos::clear)
#endif

    // supported only by some traits
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
#endif

    ;
}

// Overlay traits
template <bool VertexExtended, bool HalfedgeExtended, bool FaceExtended>
void bind_overlay_function_traits() {
  bp::class_<aos2::Arr_overlay_function_traits>("Arr_overlay_function_traits")
    .def(bp::init<>())
    .def(bp::init<bp::object, bp::object, bp::object, bp::object, bp::object,
                  bp::object, bp::object, bp::object, bp::object, bp::object>())
    .def("set_vv_v", &aos2::Arr_overlay_function_traits::set_vv_v)
    .def("set_ve_v", &aos2::Arr_overlay_function_traits::set_ve_v)
    .def("set_vf_v", &aos2::Arr_overlay_function_traits::set_vf_v)
    .def("set_ev_v", &aos2::Arr_overlay_function_traits::set_ev_v)
    .def("set_fv_v", &aos2::Arr_overlay_function_traits::set_fv_v)
    .def("set_ee_v", &aos2::Arr_overlay_function_traits::set_ee_v)
    .def("set_ee_e", &aos2::Arr_overlay_function_traits::set_ee_e)
    .def("set_ef_e", &aos2::Arr_overlay_function_traits::set_ef_e)
    .def("set_fe_e", &aos2::Arr_overlay_function_traits::set_fe_e)
    .def("set_ff_f", &aos2::Arr_overlay_function_traits::set_ff_f)
    ;
}

template <>
void bind_overlay_function_traits<false, false, false>() {
  bp::class_<aos2::Arr_overlay_function_traits>("Arr_overlay_function_traits")
    .def(bp::init<>());
}

template <>
void bind_overlay_function_traits<false, false, true>() {
  bp::class_<aos2::Arr_overlay_function_traits>("Arr_overlay_function_traits")
    .def(bp::init<>())
    .def(bp::init<bp::object>())
    ;
}

void export_arrangement_on_surface_2() {
  typedef aos2::Arrangement_2                           Arr;
  typedef aos2::Arrangement_with_history_2              Awh;
  typedef Arr::Geometry_traits_2                        Tr;
  typedef Tr::Point_2                                   Point;
  typedef Tr::Curve_2                                   Curve;
  typedef Tr::X_monotone_curve_2                        X_monotone_curve;
  typedef CGAL::Arr_naive_point_location<Arr>           Naive_pl;
  typedef CGAL::Arr_walk_along_line_point_location<Arr> Wal_pl;
  typedef CGAL::Arr_landmarks_point_location<Arr>       Landmarks_pl;
  typedef CGAL::Arr_trapezoid_ric_point_location<Arr>   Trapezoid_pl;

  typedef bp::return_internal_reference<>               RIR;

  bp::enum_<CGAL::Arr_halfedge_direction>("Arr_halfedge_direction")
    .value("ARR_RIGHT_TO_LEFT", CGAL::Arr_halfedge_direction::ARR_RIGHT_TO_LEFT)
    .value("ARR_LEFT_TO_RIGHT", CGAL::Arr_halfedge_direction::ARR_LEFT_TO_RIGHT)
    .export_values()
    ;

  bp::enum_<CGAL::Arr_curve_end>("Arr_curve_end")
    .value("ARR_MIN_END", CGAL::Arr_curve_end::ARR_MIN_END)
    .value("ARR_MAX_END", CGAL::Arr_curve_end::ARR_MAX_END)
    .export_values()
    ;

  // Export the traits classes
#ifdef CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  auto traits_object = export_gps_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  auto traits_object = export_gps_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
  auto traits_object = export_gps_circle_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
  auto traits_object = export_gps_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
  auto traits_object = export_gps_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  auto traits_object = export_gps_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  auto traits_object = export_gps_traits();
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_GPS_TRAITS");
#endif
#else
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  auto traits_object = export_arr_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  auto traits_object = export_arr_non_caching_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
  auto traits_object = export_arr_linear_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
  auto traits_object = export_arr_circle_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
  auto traits_object = export_arr_conic_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  auto traits_object = export_arr_algebraic_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  auto traits_object = export_arr_geodesic_arc_on_sphere_traits();
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif
#endif

#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT
  {
    // Arrangement
    auto arr_co = bp::class_<Arr>("Arrangement_2");
    export_aos<Arr>(arr_co);
    arr_co
      .def("unbounded_face", &aos2::unbounded_face<Arr>, RIR())
      .def("number_of_vertices_at_infinity", &Arr::number_of_vertices_at_infinity);

    bp::scope arr_scope = arr_co;
    export_vertex();
    export_halfedge();
    export_face();
    arr_scope.attr("Geometry_traits_2") = traits_object;
  }
#elif CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT_ON_SURFACE
  {
    // Arrangement
    auto arr_co = bp::class_<Arr>("Arrangement_2");
    export_aos<Arr>(arr_co);

    bp::scope arr_scope = arr_co;
    export_vertex();
    export_halfedge();
    export_face();
    arr_scope.attr("Geometry_traits_2") = traits_object;
  }
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_TYPE");
#endif

  //free functions
  bp::def("insert_point", &aos2::insert_point<Naive_pl>, RIR());
#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT
  bp::def("insert_point", &aos2::insert_point_default, RIR());
  bp::def("insert_point", &aos2::insert_point<Wal_pl>, RIR());
  bp::def("insert_point", &aos2::insert_point<Trapezoid_pl>, RIR());
#endif
  bp::def("insert", &aos2::insert_curve<X_monotone_curve>);
  bp::def("insert", &aos2::insert_curve<Curve>);
  bp::def("insert", &aos2::insert_curves);
  bp::def("insert_non_intersecting_curve", &aos2::insert_non_intersecting_curve<Naive_pl>, RIR());
#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT
  bp::def("insert_non_intersecting_curve", &aos2::insert_non_intersecting_curve_default, RIR());
  bp::def("insert_non_intersecting_curve", &aos2::insert_non_intersecting_curve<Wal_pl>, RIR());
  bp::def("insert_non_intersecting_curve", &aos2::insert_non_intersecting_curve<Trapezoid_pl>, RIR());
#endif
  bp::def("insert_non_intersecting_curves", &aos2::insert_non_intersecting_curves);
  bp::def("decompose", &aos2::decompose);
  bp::def("zone", &aos2::zone<Naive_pl>);
#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT
  bp::def("zone", &aos2::zone_default);
  bp::def("zone", &aos2::zone<Wal_pl>);
  bp::def("zone", &aos2::zone<Trapezoid_pl>);
#endif
  //supported only by some traits

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  bp::def("zone", &aos2::zone<Landmarks_pl>);
  bp::def("do_intersect", &aos2::do_intersect<X_monotone_curve, Landmarks_pl>);
  bp::def("insert_point", &aos2::insert_point<Landmarks_pl>, RIR());
  bp::def("insert_non_intersecting_curve",
          &aos2::insert_non_intersecting_curve<Landmarks_pl>, RIR());
#endif

  bp::def("do_intersect", &aos2::do_intersect<X_monotone_curve, Naive_pl>);
#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT
  bp::def("do_intersect", &aos2::do_intersect_default<X_monotone_curve>);
  bp::def("do_intersect", &aos2::do_intersect<X_monotone_curve, Wal_pl>);
  bp::def("do_intersect", &aos2::do_intersect<X_monotone_curve, Trapezoid_pl>);
#endif

  bp::def("remove_edge", &aos2::remove_edge_free, RIR());
  bp::def("remove_vertex", &aos2::remove_vertex_free);

  // Export overlay & overlay traits
  bind_overlay_function_traits<aos2::is_vertex_extended(),
                               aos2::is_halfedge_extended(),
                               aos2::is_face_extended()>();
  bp::class_<aos2::Arr_overlay_traits>("Arr_overlay_traits")
    .def(bp::init<>())
    .def(bp::init<bp::object>())
    .def(bp::init<bp::object, bp::object, bp::object, bp::object, bp::object,
                  bp::object, bp::object, bp::object, bp::object, bp::object>())
    .def("set_vv_v", &aos2::Arr_overlay_traits::set_vv_v)
    .def("set_ve_v", &aos2::Arr_overlay_traits::set_ve_v)
    .def("set_vf_v", &aos2::Arr_overlay_traits::set_vf_v)
    .def("set_ev_v", &aos2::Arr_overlay_traits::set_ev_v)
    .def("set_fv_v", &aos2::Arr_overlay_traits::set_fv_v)
    .def("set_ee_v", &aos2::Arr_overlay_traits::set_ee_v)
    .def("set_ee_e", &aos2::Arr_overlay_traits::set_ee_e)
    .def("set_ef_e", &aos2::Arr_overlay_traits::set_ef_e)
    .def("set_fe_e", &aos2::Arr_overlay_traits::set_fe_e)
    .def("set_ff_f", &aos2::Arr_overlay_traits::set_ff_f)
    ;

  bp::def("overlay", &aos2::overlay<aos2::Arr_overlay_function_traits>);
  bp::def("overlay", &aos2::overlay<aos2::Arr_overlay_traits>);
}
