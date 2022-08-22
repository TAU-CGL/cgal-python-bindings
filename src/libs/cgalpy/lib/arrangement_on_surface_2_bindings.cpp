// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <boost/static_assert.hpp>

#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_vertical_decomposition_2.h>
#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>

#include "CGALPY/Python_functor.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/Arr_overlay_traits.hpp"
#include "CGALPY/Arr_overlay_function_traits.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_input_iterator.hpp"

void export_vertex(py::class_<aos2::Arrangement_on_surface_2>&);
void export_halfedge(py::class_<aos2::Arrangement_on_surface_2>&);
void export_face(py::class_<aos2::Arrangement_on_surface_2>&);

py::object export_arr_linear_traits(py::module_&);
py::object export_arr_segment_traits(py::module_&);
py::object export_arr_non_caching_segment_traits(py::module_&);
py::object export_arr_circle_segment_traits(py::module_&);
py::object export_arr_conic_traits(py::module_&);
py::object export_arr_algebraic_segment_traits(py::module_&);
py::object export_arr_geodesic_arc_on_sphere_traits(py::module_&);

py::object export_gps_segment_traits(py::module_&);
py::object export_gps_circle_segment_traits(py::module_&);
py::object export_gps_traits(py::module_&);

namespace aos2 {

namespace py = nanobind;

typedef typename boost::variant<Vertex_const_handle, Halfedge_const_handle,
                                Face_const_handle>      variant;

typedef Arr_overlay_function_traits<Arrangement_2, Arrangement_2, Arrangement_2,
                                    py::object> Arr_overlay_function_traits;
typedef Arr_overlay_traits<Arrangement_2, Arrangement_2, Arrangement_2,
                                    py::object> Arr_overlay_traits;

// Free functions

// Insert a list of curves into an arrangement.
void insert_curves(Arrangement_on_surface_2& arr, py::list& lst) {
  if (lst.size() == 0) return;
  if (py::isinstance<X_monotone_curve_2>(lst[0])) {
    auto begin = stl_input_iterator<X_monotone_curve_2>(lst);
    auto end = stl_input_iterator<X_monotone_curve_2>(lst, false);
    CGAL::insert(arr, begin, end);
  }
  else if (py::isinstance<Curve_2>(lst[0])) {
    auto begin = stl_input_iterator<Curve_2>(lst);
    auto end = stl_input_iterator<Curve_2>(lst, false);
    CGAL::insert(arr, begin, end);
  }
  else std::runtime_error("Attempting to insert a list of of object of unrecognized type to an arrangement!");
}

// Insert a list of x-monotone curves into an arrangement.
void insert_non_intersecting_curves(Arrangement_on_surface_2& arr,
                                    py::list& lst) {
  auto begin = stl_input_iterator<X_monotone_curve_2>(lst);
  auto end = stl_input_iterator<X_monotone_curve_2>(lst, false);
  CGAL::insert_non_intersecting_curves(arr, begin, end);
}

//
template <typename OverlayTraits>
void overlay(Arrangement_on_surface_2& arr1, Arrangement_on_surface_2& arr2,
             Arrangement_on_surface_2& arr_res,
             OverlayTraits& traits)
{ CGAL::overlay(arr1, arr2, arr_res, traits); }

//
Face& remove_edge_free(Arrangement_2& arr, Halfedge& e) {
  auto handle = e.twin();
  return *(CGAL::remove_edge(arr, handle));
}

//
bool remove_vertex_free(Arrangement_2& arr, Vertex& v)
{ return CGAL::remove_vertex(arr, Vertex_iterator(&v)); }

//
template<typename T1, typename T2>
void decompose_helper2(const Vertex& vertex, const T1& below,
                       const T2& above, py::list& lst) {
  py::tuple inner = py::make_tuple(below, above);
  py::tuple outer = py::make_tuple(vertex, inner);
  lst.append(outer);
}

//
template<typename T1>
void decompose_helper1(const Vertex& vertex, const T1& below,
                       const boost::optional<variant>& above, py::list& lst) {
  if (! above) {
    py::object none;
    decompose_helper2<T1, py::object>(vertex, below, none, lst);
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

//
typedef std::pair<Arrangement_2::Vertex_const_handle,
                  std::pair<boost::optional<variant>,
                            boost::optional<variant>>>        Decompose_result;

void decompose_helper(const Decompose_result& res, py::list& lst) {
  const Vertex& vertex = *(res.first);
  const boost::optional<variant>& below = res.second.first;
  const boost::optional<variant>& above = res.second.second;

  if (! below) {
    py::object none;
    decompose_helper1<py::object>(vertex, none, above, lst);
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

//
py::list decompose(Arrangement_2& arr) {
  py::list lst;
  auto op = [&] (const Decompose_result& res) mutable
            { decompose_helper(res, lst); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::decompose(arr, it);
  return lst;
}

// class Zone_object_visitor : public boost::static_visitor<py::object> {
// public:
//   template<typename T>
//     py::object operator()(T& operand) const {
//       return py::handle(*operand);
//   }
// }; NB

// py::list zone_default(Arrangement_2& arr, X_monotone_curve_2& c) {
//   py::list lst;
//   auto op =
//     [&] (const variant& o) mutable
//     { lst.append(boost::apply_visitor(Zone_object_visitor(), o)); };
//   // The argument type of boost::function_output_iterator (UnaryFunction) must
//   // be Assignable and Copy Constructible; hence the application of std::ref().
//   auto it = boost::make_function_output_iterator(std::ref(op));
//   CGAL::zone(arr, c, it);
//   return lst;
// } NB

// template <typename PointLocation>
// py::list zone(Arrangement_2& arr, X_monotone_curve_2& c, PointLocation& pl) {
//   py::list lst;
//   auto op =
//     [&] (const variant& o) mutable
//     { lst.append(boost::apply_visitor(Zone_object_visitor(), o)); };
//   // The argument type of boost::function_output_iterator (UnaryFunction) must
//   // be Assignable and Copy Constructible; hence the application of std::ref().
//   auto it = boost::make_function_output_iterator(std::ref(op));
//   CGAL::zone(arr, c, it, pl);
//   return lst;
// } NB

// Arrangement methods
template <typename Aos>
typename Aos::Halfedge&
insert_from_left_vertex(Aos& arr, typename Aos::X_monotone_curve_2& c,
                        Vertex& v)
{ return *(arr.insert_from_left_vertex(c,(Vertex_iterator(&v)))); }

//
template <typename Aos>
typename Aos::Halfedge&
insert_from_right_vertex(Aos& arr, typename Aos::X_monotone_curve_2& c,
                         Vertex& v)
{ return *(arr.insert_from_right_vertex(c, (Vertex_iterator(&v)))); }

//
template <typename Aos>
typename Aos::Halfedge&
insert_edge_in_face_interior(Aos& arr, typename Aos::X_monotone_curve_2& c,
                             typename Aos::Face& f)
{ return *(arr.insert_in_face_interior(c, (Face_iterator(&f)))); }

//
template <typename Aos>
typename Aos::Vertex& insert_vertex_in_face_interior(Aos& arr,
                                                     typename Aos::Point_2& p,
                                                     typename Aos::Face& f)
{ return *(arr.insert_in_face_interior(p, (Face_iterator(&f)))); }

//
template <typename Aos>
typename Aos::Halfedge& insert_at_vertices(Aos& arr,
                                           typename Aos::X_monotone_curve_2& c,
                                           typename Aos::Vertex& v1,
                                           typename Aos::Vertex& v2) {
  return *(arr.insert_at_vertices(c, (Vertex_iterator(&v1)),
                                  (Vertex_iterator(&v2))));
}

//
template <typename Aos>
typename Aos::Vertex& modify_vertex(Aos& arr,
                                    typename Aos::Vertex& v,
                                    typename Aos::Point_2& p)
{ return *(arr.modify_vertex(Vertex_iterator(&v), p)); }

//
template <typename Aos>
typename Aos::Face& remove_isolated_vertex(Aos& arr,
                                           typename Aos::Vertex& v)
{ return *(arr.remove_isolated_vertex(Vertex_iterator(&v))); }

//
template <typename Aos>
typename Aos::Halfedge& modify_edge(Aos& arr,
                                    typename Aos::Halfedge& e,
                                    typename Aos::X_monotone_curve_2& c)
{ return *(arr.modify_edge(Halfedge_iterator(&e), c)); }

//
template <typename Aos>
typename Aos::Halfedge& split_edge(Aos& arr,
                                   typename Aos::Halfedge& e,
                                   typename Aos::X_monotone_curve_2& c1,
                     X_monotone_curve_2& c2)
{ return *(arr.split_edge(Halfedge_iterator(&e), c1, c2)); }

//
template <typename Aos>
typename Aos::Halfedge& merge_edge(Aos& arr,
                                   typename Aos::Halfedge& e1,
                                   typename Aos::Halfedge& e2,
                     X_monotone_curve_2& c)
{ return *(arr.merge_edge(Halfedge_iterator(&e1), Halfedge_iterator(&e2), c)); }

//
template <typename Aos>
typename Aos::Face& remove_edge(Aos& arr,
                                typename Aos::Halfedge& e) {
  auto handle = e.twin();
  return *(arr.remove_edge(handle));
}

//
template <typename Aos>
typename Aos::Vertex_iterator vertices_begin(Aos& arr)
{ return arr.vertices_begin(); }

//
template <typename Aos>
typename Aos::Vertex_iterator vertices_end(Aos& arr)
{ return arr.vertices_end(); }

//
template <typename Aos>
typename Aos::Halfedge_iterator halfedges_begin(Aos& arr)
{ return arr.halfedges_begin(); }

//
template <typename Aos>
typename Aos::Halfedge_iterator halfedges_end(Aos& arr)
{ return arr.halfedges_end(); }

//
template <typename Aos>
typename Aos::Edge_iterator edges_begin(Aos& arr) { return arr.edges_begin(); }

//
template <typename Aos>
typename Aos::Edge_iterator edges_end(Aos& arr) { return arr.edges_end(); }

//
template <typename Aos>
typename Aos::Face_iterator faces_begin(Aos& arr) { return arr.faces_begin(); }

//
template <typename Aos>
typename Aos::Face_iterator faces_end(Aos& arr) { return arr.faces_end(); }

//
template <typename Aos>
typename Aos::Face_iterator unbounded_faces_begin(Aos& arr)
{ return arr.unbounded_faces_begin(); }

//
template <typename Aos>
typename Aos::Face_iterator unbounded_faces_end(Aos& arr)
{ return arr.unbounded_faces_end(); }

//
template <typename Aos>
typename Aos::Face& unbounded_face(Aos& arr)
{ return *(arr.unbounded_face()); }

//
template <typename Aos>
typename Aos::Face& fictitious_face(Aos& arr)
{ return *(arr.fictitious_face()); }

//
template <typename Aos>
void assign(Aos& arr, Aos& input_arr) { arr.assign(input_arr); }

}

// Export common members of Aos types
void export_aos(py::module_& m, const py::object& traits_c) {
  using Aos = aos2::Arrangement_on_surface_2;
  using GT = aos2::Geometry_traits_2;

  py::class_<Aos> c(m, "Arrangement_on_surface_2");
  c.def(py::init<>())
    .def(py::init<const Aos&>())
    .def(py::init<const GT*>())
    // .def("halfedges", [](Aos& arr) {
    //                     return py::iterator(arr.halfedges_begin(),
    //                                         arr.halfedges_end());
    //                   }) NB
    // .def("halfedges", py::range<RIR>(&aos2::halfedges_begin<Aos>, &aos2::halfedges_end<Aos>)) NB
    // .def("vertices", py::make_iterator(&aos2::vertices_begin<Aos>, &aos2::vertices_end<Aos>), py::py::keep_alive<0, 1>()) NB
    // .def("faces", py::range<RIR>(&aos2::faces_begin<Aos>, &aos2::faces_end<Aos>)) NB
    // .def("edges", py::range<RIR>(&aos2::edges_begin<Aos>, &aos2::edges_end<Aos>)) NB
    // .def("unbounded_faces", py::range<RIR>(&aos2::unbounded_faces_begin<Aos>, &aos2::unbounded_faces_end<Aos>)) NB
    .def("fictitious_face", &aos2::fictitious_face<Aos>)
    .def("insert_from_left_vertex", &aos2::insert_from_left_vertex<Aos>)
    .def("insert_from_right_vertex", &aos2::insert_from_right_vertex<Aos>)
    .def("insert_in_face_interior", &aos2::insert_edge_in_face_interior<Aos>)
    .def("insert_in_face_interior", &aos2::insert_vertex_in_face_interior<Aos>)
    .def("insert_at_vertices", &aos2::insert_at_vertices<Aos>)
    .def("modify_vertex", &aos2::modify_vertex<Aos>)
    .def("remove_isolated_vertex", &aos2::remove_isolated_vertex<Aos>)
    .def("modify_edge", &aos2::modify_edge<Aos>)
    .def("split_edge", &aos2::split_edge<Aos>)
    .def("merge_edge", &aos2::merge_edge<Aos>)
    .def("remove_edge", &aos2::remove_edge<Aos>)
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
    ;

    // supported only by some traits
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  add_insertion(c, "__str__");
  add_insertion(c, "__repr__");
#endif

  export_vertex(c);
  export_halfedge(c);
  export_face(c);

  c.attr("Geometry_traits_2") = traits_c;
}

// Overlay traits
template <bool VertexExtended, bool HalfedgeExtended, bool FaceExtended>
void bind_overlay_function_traits(py::module_& m) {
  py::class_<aos2::Arr_overlay_function_traits>(m, "Arr_overlay_function_traits")
    .def(py::init<>())
    .def(py::init<py::object, py::object, py::object, py::object, py::object,
                  py::object, py::object, py::object, py::object, py::object>())
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

//
template <>
void bind_overlay_function_traits<false, false, false>(py::module_& m) {
  py::class_<aos2::Arr_overlay_function_traits>(m, "Arr_overlay_function_traits")
    .def(py::init<>());
}

//
template <>
void bind_overlay_function_traits<false, false, true>(py::module_& m) {
  py::class_<aos2::Arr_overlay_function_traits>(m, "Arr_overlay_function_traits")
    .def(py::init<>())
    .def(py::init<py::object>())
    ;
}

#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT

//
void export_arr(py::module_& m) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Arr = aos2::Arrangement_2;
  using GT = aos2::Geometry_traits_2;

  auto arr_co = py::class_<Arr, Aos>(m, "Arrangement_2")
    .def(py::init<>())
    .def(py::init<const Arr&>())
    .def(py::init<const GT*>())
    .def("unbounded_face", &aos2::unbounded_face<Arr>)
    .def("number_of_vertices_at_infinity",
         &Arr::number_of_vertices_at_infinity);
}

#endif

//
void export_arrangement_on_surface_2(py::module_& m) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Arr = aos2::Arrangement_2;
  using Awh = aos2::Arrangement_with_history_2;
  using GT = Arr::Geometry_traits_2;
  using Point = GT::Point_2;
  using Cv = GT::Curve_2;
  using Xcv = GT::X_monotone_curve_2;
  using Naive_pl = CGAL::Arr_naive_point_location<Arr>;
  using Wal_pl = CGAL::Arr_walk_along_line_point_location<Arr>;
  using Landmarks_pl = CGAL::Arr_landmarks_point_location<Arr>;
  using Trapezoid_pl = CGAL::Arr_trapezoid_ric_point_location<Arr>;

  py::enum_<CGAL::Arr_halfedge_direction>(m, "Arr_halfedge_direction")
    .value("ARR_RIGHT_TO_LEFT", CGAL::Arr_halfedge_direction::ARR_RIGHT_TO_LEFT)
    .value("ARR_LEFT_TO_RIGHT", CGAL::Arr_halfedge_direction::ARR_LEFT_TO_RIGHT)
    .export_values()
    ;

  py::enum_<CGAL::Arr_curve_end>(m, "Arr_curve_end")
    .value("ARR_MIN_END", CGAL::Arr_curve_end::ARR_MIN_END)
    .value("ARR_MAX_END", CGAL::Arr_curve_end::ARR_MAX_END)
    .export_values()
    ;

  // Export the traits classes
#ifdef CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_gps_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_gps_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_gps_circle_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
  auto traits_c = export_gps_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
  auto traits_c = export_gps_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_gps_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  auto traits_c = export_gps_traits(m);
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_GPS_TRAITS");
#endif
#else
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_arr_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_arr_non_caching_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
  auto traits_c = export_arr_linear_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_arr_circle_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
  auto traits_c = export_arr_conic_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_arr_algebraic_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  auto traits_c = export_arr_geodesic_arc_on_sphere_traits(m);
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif
#endif

  // Arrangement on surface
  if (! add_attr<Aos>("Arrangement_on_surface_2", m)) export_aos(m, traits_c);

#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT
  if (! add_attr<Arr>("Arrangement_2", m)) export_arr(m);
#endif

  using Vh = aos2::Arrangement_on_surface_2::Vertex_handle;
  using Hh = aos2::Arrangement_on_surface_2::Halfedge_handle;

  using Insert_point = Vh(*)(Aos&, const Point&);
  using Insert_point_nv_pl = Vh(*)(Aos&, const Point&, const Naive_pl&);
  using Insert_point_wl_pl = Vh(*)(Aos&, const Point&, const Wal_pl&);
  using Insert_point_tr_pl = Vh(*)(Aos&, const Point&, const Trapezoid_pl&);
  using Insert_point_lm_pl = Vh(*)(Aos&, const Point&, const Landmarks_pl&);

  using Insert_ni_xcv = Hh(*)(Aos&, const Xcv&);
  using Insert_ni_xcv_nv_pl = Hh(*)(Aos&, const Xcv&, const Naive_pl&);
  using Insert_ni_xcv_wl_pl = Hh(*)(Aos&, const Xcv&, const Wal_pl&);
  using Insert_ni_xcv_tr_pl = Hh(*)(Aos&, const Xcv&, const Wal_pl&);
  using Insert_ni_xcv_lm_pl = Hh(*)(Aos&, const Xcv&, const Landmarks_pl&);

  using Insert_cv = void(*)(Aos&, const Cv&);
  using Insert_xcv = void(*)(Aos&, const Xcv&);

  using Do_intersect = bool(*)(Aos&, const Xcv&);
  using Do_intersect_nv_pl = bool(*)(Aos&, const Xcv&, const Naive_pl&);
  using Do_intersect_wl_pl = bool(*)(Aos&, const Xcv&, const Wal_pl&);
  using Do_intersect_tr_pl = bool(*)(Aos&, const Xcv&, const Wal_pl&);
  using Do_intersect_lm_pl = bool(*)(Aos&, const Xcv&, const Landmarks_pl&);

  /// Free functions

  m.def("insert_point", static_cast<Insert_point>(CGAL::insert_point))
    .def("insert_point", static_cast<Insert_point_nv_pl>(CGAL::insert_point))
    .def("insert_point", static_cast<Insert_point_wl_pl>(CGAL::insert_point))
    .def("insert_point", static_cast<Insert_point_tr_pl>(CGAL::insert_point))
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
    .def("insert_point", static_cast<Insert_point_lm_pl>(CGAL::insert_point))
#endif
    ;

  m.def("insert_non_intersecting_curve",
        static_cast<Insert_ni_xcv>(CGAL::insert_non_intersecting_curve))
    .def("insert_non_intersecting_curve",
         static_cast<Insert_ni_xcv_nv_pl>(CGAL::insert_non_intersecting_curve))
    .def("insert_non_intersecting_curve",
         static_cast<Insert_ni_xcv_wl_pl>(CGAL::insert_non_intersecting_curve))
    .def("insert_non_intersecting_curve",
         static_cast<Insert_ni_xcv_tr_pl>(CGAL::insert_non_intersecting_curve))
    .def("insert_non_intersecting_curves",
          &aos2::insert_non_intersecting_curves)
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
    .def("insert_non_intersecting_curve",
         static_cast<Insert_ni_xcv_lm_pl>(CGAL::insert_non_intersecting_curve))
#endif
    ;

  m.def("insert", static_cast<Insert_cv>(CGAL::insert))
    .def("insert", static_cast<Insert_xcv>(CGAL::insert))
    .def("insert", &aos2::insert_curves)
    ;

  m.def("do_intersect", static_cast<Do_intersect>(CGAL::do_intersect))
    .def("do_intersect", static_cast<Do_intersect_nv_pl>(CGAL::do_intersect))
    .def("do_intersect", static_cast<Do_intersect_wl_pl>(CGAL::do_intersect))
    .def("do_intersect", static_cast<Do_intersect_tr_pl>(CGAL::do_intersect))
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
    .def("do_intersect", static_cast<Do_intersect_lm_pl>(CGAL::do_intersect))
#endif
    ;

  m.def("decompose", &aos2::decompose);

  // m.def("zone", &aos2::zone<Naive_pl>); NB
  // m.def("zone", &aos2::zone_default); NB
  // m.def("zone", &aos2::zone<Wal_pl>); NB
  // m.def("zone", &aos2::zone<Trapezoid_pl>); NB
#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  // m.def("zone", &aos2::zone<Landmarks_pl>); NB
#endif

  m.def("remove_edge", &aos2::remove_edge_free);
  m.def("remove_vertex", &aos2::remove_vertex_free);

  // Export overlay & overlay traits
  bind_overlay_function_traits<aos2::is_vertex_extended(),
                               aos2::is_halfedge_extended(),
                               aos2::is_face_extended()>(m);
  py::class_<aos2::Arr_overlay_traits>(m, "Arr_overlay_traits")
    .def(py::init<>())
    .def(py::init<py::object>())
    .def(py::init<py::object, py::object, py::object, py::object, py::object,
                  py::object, py::object, py::object, py::object, py::object>())
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

  m.def("overlay", &aos2::overlay<aos2::Arr_overlay_function_traits>);
  m.def("overlay", &aos2::overlay<aos2::Arr_overlay_traits>);
}
