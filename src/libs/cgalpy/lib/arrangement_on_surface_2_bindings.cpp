// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <boost/python.hpp>
#include <boost/static_assert.hpp>
#include <boost/python/stl_iterator.hpp>

#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_vertical_decomposition_2.h>
#include <CGAL/Arr_default_overlay_traits.h>
#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>

#include "CGALPY/Python_functor.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"

void export_vertex();
void export_halfedge();
void export_face();

void export_arr_linear_traits();
void export_arr_segment_traits();
void export_arr_circle_segment_traits();
void export_arr_conic_traits();
void export_arr_algebraic_segment_traits();

namespace bp = boost::python;

typedef typename CGAL::Arr_naive_point_location<Arrangement_2>         Naive_pl;
typedef typename CGAL::Arr_walk_along_line_point_location<Arrangement_2> Wal_pl;
typedef typename CGAL::Arr_landmarks_point_location<Arrangement_2> Landmarks_pl;
typedef typename CGAL::Arr_trapezoid_ric_point_location<Arrangement_2>
  Trapezoid_pl;

#if (CGALPY_AOS2_DCEL == CGALPY_AOS2_FACE_EXTENDED_DCEL) || \
  (CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL)
#include <CGALPY/Arr_python_face_overlay_traits.hpp>
typedef Arr_python_face_overlay_traits<Arrangement_2, Arrangement_2,
                                       Arrangement_2, Face::Data>
  Arr_face_overlay_traits;
#endif

#if CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL
#include <CGALPY/Arr_python_overlay_traits.hpp>
typedef Arr_python_overlay_traits<Arrangement_2, Arrangement_2, Arrangement_2,
                                  Face::Data> Arr_overlay_traits;
#endif

// Free functions
Vertex& insert_point_default(Arrangement_2& arr, TPoint_2& p)
{ return *(CGAL::insert_point(arr, p)); }

template <typename PointLocation>
Vertex& insert_point(Arrangement_2& arr, TPoint_2& p, PointLocation& pl)
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

void overlay(Arrangement_2& arr1, Arrangement_2& arr2, Arrangement_2& arr_res)
{ CGAL::overlay(arr1, arr2, arr_res); }

#if (CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL) ||    \
  (CGALPY_AOS2_DCEL == CGALPY_AOS2_FACE_EXTENDED_DCEL)

void overlay_fex(Arrangement_2& arr1, Arrangement_2& arr2,
                 Arrangement_2& arr_res, Arr_face_overlay_traits& traits)
{ CGAL::overlay(arr1, arr2, arr_res, traits); }

#endif

#if CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL

void overlay_ex(Arrangement_2& arr1, Arrangement_2& arr2,
                Arrangement_2& arr_res, Arr_overlay_traits& traits)
{ CGAL::overlay(arr1, arr2, arr_res, traits); }

#endif

Face& remove_edge_free(Arrangement_2& arr, Halfedge& e) {
  auto handle = e.twin();
  return *(CGAL::remove_edge(arr, handle));
}

bool remove_vertex_free(Arrangement_2& arr, Vertex& v)
{ return CGAL::remove_vertex(arr, Vertex_iterator(&v)); }

void decompose(Arrangement_2& arr, bp::list& lst) {
  auto v = std::vector<std::pair<Arrangement_2::Vertex_const_handle,
                                 std::pair<Object, Object>>>();
  CGAL::decompose(arr, std::back_inserter(v));
  for (auto& p : v) {
    bp::tuple inner = bp::make_tuple(p.second.first, p.second.second);
    bp::tuple outer = bp::make_tuple(*(p.first), inner);
    lst.append(outer);
  }
}

void zone_default(Arrangement_2& arr, X_monotone_curve_2& c, bp::list& lst) {
  auto v = std::vector<Object>();
  CGAL::zone(arr, c, std::back_inserter(v));
  for (auto o : v) lst.append(o);
}

template <typename PointLocation>
void zone(Arrangement_2& arr, X_monotone_curve_2& c, bp::list& lst,
          PointLocation& pl) {
  auto v = std::vector<Object>();
  CGAL::zone(arr, c, std::back_inserter(v), pl);
  for (auto o : v) lst.append(o);
}

//Arrangement methods
Halfedge& insert_from_left_vertex(Arrangement_2& arr, X_monotone_curve_2& c,
                                  Vertex& v)
{ return *(arr.insert_from_left_vertex(c,(Vertex_iterator(&v)))); }

Halfedge& insert_from_right_vertex(Arrangement_2& arr, X_monotone_curve_2& c,
                                   Vertex& v)
{ return *(arr.insert_from_right_vertex(c, (Vertex_iterator(&v)))); }

Halfedge& insert_edge_in_face_interior(Arrangement_2& arr,
                                       X_monotone_curve_2& c, Face& f)
{ return *(arr.insert_in_face_interior(c, (Face_iterator(&f)))); }

Vertex& insert_vertex_in_face_interior(Arrangement_2& arr, TPoint_2& p, Face& f)
{ return *(arr.insert_in_face_interior(p, (Face_iterator(&f)))); }

Halfedge& insert_at_vertices(Arrangement_2& arr, X_monotone_curve_2& c,
                             Vertex& v1, Vertex& v2) {
  return *(arr.insert_at_vertices(c, (Vertex_iterator(&v1)),
                                  (Vertex_iterator(&v2))));
}

Vertex& modify_vertex(Arrangement_2& arr, Vertex& v, TPoint_2& p)
{ return *(arr.modify_vertex(Vertex_iterator(&v), p)); }

Face& remove_isolated_vertex(Arrangement_2& arr, Vertex& v)
{ return *(arr.remove_isolated_vertex(Vertex_iterator(&v))); }

Halfedge& modify_edge(Arrangement_2& arr, Halfedge& e, X_monotone_curve_2& c)
{ return *(arr.modify_edge(Halfedge_iterator(&e), c)); }

Halfedge& split_edge(Arrangement_2& arr, Halfedge& e, X_monotone_curve_2& c1,
                     X_monotone_curve_2& c2)
{ return *(arr.split_edge(Halfedge_iterator(&e), c1, c2)); }

Halfedge& merge_edge(Arrangement_2& arr, Halfedge& e1, Halfedge& e2,
                     X_monotone_curve_2& c)
{ return *(arr.merge_edge(Halfedge_iterator(&e1), Halfedge_iterator(&e2), c)); }

Face& remove_edge(Arrangement_2& arr, Halfedge& e) {
  auto handle = e.twin();
  return *(arr.remove_edge(handle));
}

Vertex_iterator vertices_begin(Arrangement_2& arr)
{ return arr.vertices_begin(); }

Vertex_iterator vertices_end(Arrangement_2& arr) { return arr.vertices_end(); }

Halfedge_iterator halfedges_begin(Arrangement_2& arr)
{ return arr.halfedges_begin(); }

Halfedge_iterator halfedges_end(Arrangement_2& arr)
{ return arr.halfedges_end(); }

Edge_iterator edges_begin(Arrangement_2& arr) { return arr.edges_begin(); }

Edge_iterator edges_end(Arrangement_2& arr) { return arr.edges_end(); }

Face_iterator faces_begin(Arrangement_2& arr) { return arr.faces_begin(); }

Face_iterator faces_end(Arrangement_2& arr) { return arr.faces_end(); }

Face_iterator unbounded_faces_begin(Arrangement_2& arr)
{ return arr.unbounded_faces_begin(); }

Face_iterator unbounded_faces_end(Arrangement_2& arr)
{ return arr.unbounded_faces_end(); }

Face& unbounded_face(Arrangement_2& arr) { return *(arr.unbounded_face()); }

Face& fictitious_face(Arrangement_2& arr) { return *(arr.fictitious_face()); }

void assign(Arrangement_2& arr, Arrangement_2& input_arr)
{ arr.assign(input_arr); }

void export_arrangement_on_surface_2() {
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

  {
    bp::scope aos_scope = bp::class_<Arrangement_2>("Arrangement_2")
      .def(bp::init<>())
      .def(bp::init<Arrangement_2&>())
      .def("halfedges", bp::range<bp::return_internal_reference<>>(&halfedges_begin, &halfedges_end))
      .def("vertices", bp::range<bp::return_internal_reference<>>(&vertices_begin, &vertices_end))
      .def("faces", bp::range<bp::return_internal_reference<>>(&faces_begin, &faces_end))
      .def("edges", bp::range<bp::return_internal_reference<>>(&edges_begin, &edges_end))
      .def("unbounded_face", &unbounded_face, bp::return_internal_reference<>())
      .def("unbounded_faces", bp::range<bp::return_internal_reference<>>(&unbounded_faces_begin, &unbounded_faces_end))
      .def("fictitious_face", &fictitious_face, bp::return_internal_reference<>())
      .def("insert_from_left_vertex", &insert_from_left_vertex, bp::return_internal_reference<>())
      .def("insert_from_right_vertex", &insert_from_right_vertex, bp::return_internal_reference<>())
      .def("insert_in_face_interior", &insert_edge_in_face_interior, bp::return_internal_reference<>())
      .def("insert_in_face_interior", &insert_vertex_in_face_interior, bp::return_internal_reference<>())
      .def("insert_at_vertices", &insert_at_vertices, bp::return_internal_reference<>())
      .def("modify_vertex", &modify_vertex, bp::return_internal_reference<>())
      .def("remove_isolated_vertex", &remove_isolated_vertex, bp::return_internal_reference<>())
      .def("modify_edge", &modify_edge, bp::return_internal_reference<>())
      .def("split_edge", &split_edge, bp::return_internal_reference<>())
      .def("merge_edge", &merge_edge, bp::return_internal_reference<>())
      .def("remove_edge", &remove_edge, bp::return_internal_reference<>())
      .def("is_empty", &Arrangement_2::is_empty)
      .def("is_valid", &Arrangement_2::is_valid)
      .def("number_of_edges", &Arrangement_2::number_of_edges)
      .def("number_of_faces", &Arrangement_2::number_of_faces)
      .def("number_of_halfedges", &Arrangement_2::number_of_halfedges)
      .def("number_of_isolated_vertices", &Arrangement_2::number_of_isolated_vertices)
      .def("number_of_unbounded_faces", &Arrangement_2::number_of_unbounded_faces)
      .def("number_of_vertices", &Arrangement_2::number_of_vertices)
      .def("number_of_vertices_at_infinity", &Arrangement_2::number_of_vertices_at_infinity)
      .def("assign", &assign)
      .def("clear", &Arrangement_2::clear)

      //supported only by some traits
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
      .def(bp::self_ns::str(bp::self_ns::self))
      .def(bp::self_ns::repr(bp::self_ns::self))
#endif
      ;

    export_vertex();
    export_halfedge();
    export_face();

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
    export_arr_linear_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
    export_arr_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
    export_arr_circle_segment_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
    export_arr_conic_traits();
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
    export_arr_algebraic_segment_traits();
#else
    BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif
  }

  //free functions
  bp::def("insert_point", &insert_point_default, bp::return_internal_reference<>());
  bp::def("insert_point", &insert_point<Naive_pl>, bp::return_internal_reference<>());
  bp::def("insert_point", &insert_point<Wal_pl>, bp::return_internal_reference<>());
  bp::def("insert_point", &insert_point<Trapezoid_pl>, bp::return_internal_reference<>());
  bp::def("insert", &insert_curve<X_monotone_curve_2>);
  bp::def("insert", &insert_curve<Curve_2>);
  bp::def("insert", &insert_curves);
  bp::def("insert_non_intersecting_curve", &insert_non_intersecting_curve_default, bp::return_internal_reference<>());
  bp::def("insert_non_intersecting_curve", &insert_non_intersecting_curve<Naive_pl>, bp::return_internal_reference<>());
  bp::def("insert_non_intersecting_curve", &insert_non_intersecting_curve<Wal_pl>, bp::return_internal_reference<>());
  bp::def("insert_non_intersecting_curve", &insert_non_intersecting_curve<Trapezoid_pl>, bp::return_internal_reference<>());
  bp::def("insert_non_intersecting_curves", &insert_non_intersecting_curves);
  bp::def("decompose", &decompose);
  bp::def("zone", &zone_default);
  bp::def("zone", &zone<Naive_pl>);
  bp::def("zone", &zone<Wal_pl>);
  bp::def("zone", &zone<Trapezoid_pl>);
  //supported only by some traits

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  bp::def("zone", &zone<Landmarks_pl>);
  bp::def("do_intersect", &do_intersect<X_monotone_curve_2, Landmarks_pl>);
  bp::def("insert_point", &insert_point<Landmarks_pl>, bp::return_internal_reference<>());
  bp::def("insert_non_intersecting_curve", &insert_non_intersecting_curve<Landmarks_pl>, bp::return_internal_reference<>());
#endif

  bp::def("overlay", &overlay);

#if (CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL) ||  \
  (CGALPY_AOS2_DCEL == CGALPY_AOS2_FACE_EXTENDED_DCEL)
  bp::def("overlay", &overlay_fex);
#endif

#if CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL
  bp::def("overlay", &overlay_ex);
#endif

  bp::def("do_intersect", &do_intersect_default<X_monotone_curve_2>);
  bp::def("do_intersect", &do_intersect<X_monotone_curve_2, Naive_pl>);
  bp::def("do_intersect", &do_intersect<X_monotone_curve_2, Wal_pl>);
  bp::def("do_intersect", &do_intersect<X_monotone_curve_2, Trapezoid_pl>);
  // These fail for some reason (no matching function found)
  /*
  def("do_intersect", &do_intersect_default<Curve_2>);
  def("do_intersect", &do_intersect<Curve_2, Naive_pl>);
  def("do_intersect", &do_intersect<Curve_2, Wal_pl>);
  def("do_intersect", &do_intersect<Curve_2, Trapezoid_pl>);
  */
  bp::def("remove_edge", &remove_edge_free, bp::return_internal_reference<>());
  bp::def("remove_vertex", &remove_vertex_free);

#if (CGALPY_AOS2_DCEL == CGALPY_AOS2_FACE_EXTENDED_DCEL) || \
    (CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL)
  bp::class_<Arr_face_overlay_traits>("Arr_face_overlay_traits", bp::init<bp::object>())
    ;
#endif

#if CGALPY_AOS2_DCEL == CGALPY_AOS2_EXTENDED_DCEL
  bp::class_<Arr_overlay_traits>("Arr_overlay_traits", bp::init<bp::object,
                                 bp::object, bp::object, bp::object,
                                 bp::object, bp::object, bp::object,
                                 bp::object, bp::object, bp::object>())
    ;
#endif
}
