// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <variant>

#include <boost/variant/variant.hpp>
#include <boost/static_assert.hpp>
#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_vertical_decomposition_2.h>
#include <CGAL/Arr_naive_point_location.h>
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
#include <CGAL/Arr_walk_along_line_point_location.h>
#endif
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/Arr_observer.hpp"
#include "CGALPY/Arr_overlay_traits.hpp"
#include "CGALPY/Arr_overlay_function_traits.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

#if ((CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS))
#ifdef CGALPY_HAS_VISUAL
#define CGAL_USE_BASIC_VIEWER
#include <CGAL/draw_arrangement_2.h>
#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
#include "CGALPY/basic_viewer_types.hpp"
#endif
#endif
#endif

void export_vertex(py::class_<cgalpy::aos2::Arrangement_on_surface_2>&);
void export_halfedge(py::class_<cgalpy::aos2::Arrangement_on_surface_2>&);
void export_face(py::class_<cgalpy::aos2::Arrangement_on_surface_2>&);

void export_arr_algebraic_segment_traits_2(py::module_&);
void export_arr_bezier_traits_2(py::module_&);
void export_arr_circle_segment_traits_2(py::module_&);
void export_arr_conic_traits_2(py::module_&);
void export_arr_geodesic_arc_on_sphere_traits_2(py::module_&);
void export_arr_linear_traits_2(py::module_&);
void export_arr_non_caching_segment_traits_2(py::module_&);
void export_arr_polyline_traits_2(py::module_&);
void export_arr_segment_traits_2(py::module_&);
void export_arr_rational_function_traits_2(py::module_&);

void export_arr_curve_data_traits_2(py::module_&);
void export_arr_consolidated_curve_data_traits_2(py::module_&);

void export_arr_counting_traits_2(py::module_&);
void export_arr_tracing_traits_2(py::module_&);

#if defined(CGALPY_ENVELOPE_3_BINDINGS)
void export_env_plane_traits_3(py::module_&);
void export_env_sphere_traits_3(py::module_&);
void export_env_tri_traits_3(py::module_&);
#if defined(CGALPY_ENV3_SURFACE_DATA)
void export_env_surface_data_traits_3(py::module_&);
#endif
#endif

#if defined(CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
void export_gps_segment_traits_2(py::module_&);
void export_gps_circle_segment_traits_2(py::module_&);
void export_gps_traits_2(py::module_&);
#endif

#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
extern void export_arrangement_2_io(py::class_<cgalpy::aos2::Arrangement_2,
                                    cgalpy::aos2::Arrangement_on_surface_2>&);
#endif

namespace py = nanobind;

namespace cgalpy {
namespace aos2 {

using Cell_variant = std::variant<Vertex_handle, Halfedge_handle, Face_handle>;

using Cell_const_variant = std::variant<Vertex_const_handle,
                                        Halfedge_const_handle,
                                        Face_const_handle>;

using Arr_overlay_function_traits =
  Arr_overlay_function_traits<Arrangement_on_surface_2,
                              Arrangement_on_surface_2,
                              Arrangement_on_surface_2,
                              py::object>;
using Arr_overlay_traits =
  Arr_overlay_traits<Arrangement_on_surface_2, Arrangement_on_surface_2,
                     Arrangement_on_surface_2, py::object>;

using Arr_observer = Arr_observer<Arrangement_on_surface_2, py::object>;

// Free functions

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
template <typename T>
struct Object_input_iterator :
  boost::iterator_facade<Object_input_iterator<T>, CGAL::Object, std::input_iterator_tag, CGAL::Object>
{

  // Default constructor.
  // Workaround the lack of default constructor for py::detail::fast_iterator.
  // Object_input_iterator() {}
  Object_input_iterator() : m_it(py::list().end()) {}

  Object_input_iterator(const py::list& lst, bool isbegin = true) :
    m_it((isbegin) ? lst.begin() : lst.end())
  {}

  void increment() { ++m_it; }
  CGAL::Object dereference() const
  { return CGAL::make_object(py::cast<T>(*m_it)); }

  bool equal(Object_input_iterator<T> const& o) const { return m_it == o.m_it; }

private:
  py::detail::fast_iterator m_it;
};
#endif

// Insert a list of curves into an arrangement.
void insert_curves(Arrangement_on_surface_2& arr, py::list& lst) {
  if (lst.size() == 0) return;
  using Gt = Arrangement_on_surface_2::Geometry_traits_2;
  using Xcv = Gt::X_monotone_curve_2;
  if (py::isinstance<Xcv>(lst[0])) {
    auto begin = stl_forward_iterator<Xcv>(lst);
    auto end = stl_forward_iterator<Xcv>(lst, false);
    CGAL::insert(arr, begin, end);
    return;
  }

  using Cv = Gt::Curve_2;
  if (py::isinstance<Cv>(lst[0])) {
    auto begin = stl_forward_iterator<const Cv&>(lst);
    auto end = stl_forward_iterator<const Cv&>(lst, false);
    CGAL::insert(arr, begin, end);
    return;
  }

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  using Pnt = Gt::Point_2;
  if (py::isinstance<Pnt>(lst[0])) {
    // Points must be wrapped into CGAL::Objects???
    auto begin = Object_input_iterator<const Pnt&>(lst);
    auto end = Object_input_iterator<const Pnt&>(lst, false);
    CGAL::insert(arr, begin, end);
    return;
  }
#endif

  throw std::runtime_error("Attempting to insert a list of object of unrecognized type to an arrangement!");
}

//! Overlay two arrangements
Arrangement_on_surface_2 overlay(Arrangement_on_surface_2& arr1,
                                 Arrangement_on_surface_2& arr2) {
  Arrangement_on_surface_2 arr_res;
  CGAL::overlay(arr1, arr2, arr_res);
  return arr_res;
}

//! Overlay two arrangements
template <typename OverlayTraits>
Arrangement_on_surface_2 overlay_tr(Arrangement_on_surface_2& arr1,
                                    Arrangement_on_surface_2& arr2,
                                    OverlayTraits& traits) {
  Arrangement_on_surface_2 arr_res;
  CGAL::overlay(arr1, arr2, arr_res, traits);
  return arr_res;
}

//!
Face& remove_edge_free(Arrangement_on_surface_2& aos, Halfedge& e) {
  auto handle = e.twin();
  return *(CGAL::remove_edge(aos, handle));
}

//!
bool remove_vertex_free(Arrangement_on_surface_2& aos, Vertex& v)
{ return CGAL::remove_vertex(aos, Vertex_handle(&v)); }

//!
void decompose_helper2(const Vertex& vertex,
                       const py::object& below, const py::object& above,
                       py::list& lst) {
  py::tuple inner = py::make_tuple(below, above);
  py::tuple outer = py::make_tuple(vertex, inner);
  lst.append(outer);
}

//!
void decompose_helper1(const Vertex& vertex, const py::object& below,
                       const std::optional<Cell_const_variant>& above,
                       py::list& lst) {
  if (! above) {
    auto none = py::none();
    decompose_helper2(vertex, below, none, lst);
    return;
  }
  if (auto* v = std::get_if<Vertex_const_handle>(&*above)) {
    decompose_helper2(vertex, below, py::cast(&*(*v)), lst);
    return;
  }
  if (auto* e = std::get_if<Halfedge_const_handle>(&*above)) {
    decompose_helper2(vertex, below, py::cast(&*(*e)), lst);
    return;
  }
  if (auto* f = std::get_if<Face_const_handle>(&*above)) {
    decompose_helper2(vertex, below, py::cast(&*(*f)), lst);
    return;
  }
}

//!
using Decompose_result =
  std::pair<Arrangement_on_surface_2::Vertex_const_handle,
            std::pair<std::optional<Cell_const_variant>,
                      std::optional<Cell_const_variant>>>;

void decompose_helper(const Decompose_result& res, py::list& lst) {
  const Vertex& vertex = *(res.first);
  const auto& below = res.second.first;
  const auto& above = res.second.second;

  if (! below) {
    auto none = py::none();
    decompose_helper1(vertex, none, above, lst);
    return;
  }
  if (auto* v = std::get_if<Vertex_const_handle>(&*below)) {
    decompose_helper1(vertex, py::cast(&*(*v)), above, lst);
    return;
  }
  if (auto* e = std::get_if<Halfedge_const_handle>(&*below)) {
    decompose_helper1(vertex, py::cast(&*(*e)), above, lst);
    return;
  }
  if (auto* f = std::get_if<Face_const_handle>(&*below)) {
    decompose_helper1(vertex, py::cast(&*(*f)), above, lst);
    return;
  }
}

//!
py::list decompose(Arrangement_on_surface_2& arr) {
  py::list lst;
  auto op = [&] (const Decompose_result& res) mutable
            { decompose_helper(res, lst); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::decompose(arr, it);
  return lst;
}

//!
class Zone_object_visitor {
public:
  template<typename T>
  py::object operator()(T operand) const { return py::cast(&(*operand)); }
};

//!
py::list zone(Arrangement_on_surface_2& arr, X_monotone_curve_2& c) {
  py::list lst;
  auto op = [&] (const Cell_variant& o) mutable
    { lst.append(std::visit(Zone_object_visitor(), o)); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::zone(arr, c, it);
  return lst;
}

//!
template <typename PointLocation>
py::list zone_pl(Arrangement_on_surface_2& arr, X_monotone_curve_2& c,
                 PointLocation& pl) {
  py::list lst;
  auto op = [&] (const Cell_variant& o) mutable
    { lst.append(std::visit(Zone_object_visitor(), o)); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::zone(arr, c, it, pl);
  return lst;
}

/// Arrangement on surface methods
/// @{

// Insert from the endpoint on the left given the vertex of the endpoint.
Arrangement_on_surface_2::Halfedge&
insert_from_left_vertex1(Arrangement_on_surface_2& arr,
                         X_monotone_curve_2& c, Vertex& v)
{ return *(arr.insert_from_left_vertex(c, Vertex_handle(&v))); }

// Insert from the endpoint on the left given the halfedge that is incident
// to the vertex of the endpoint.
// The newly halfedge must be added immediately after the given halfedge in
// the circular list around the vertex.
Arrangement_on_surface_2::Halfedge&
insert_from_left_vertex2(Arrangement_on_surface_2& arr,
                         X_monotone_curve_2& c, Halfedge& h)
{ return *(arr.insert_from_left_vertex(c, Halfedge_handle(&h))); }

// Insert from the endpoint on the right given the vertex of the endpoint.
Arrangement_on_surface_2::Halfedge&
insert_from_right_vertex1(Arrangement_on_surface_2& arr,
                         Arrangement_on_surface_2::X_monotone_curve_2& c,
                         Arrangement_on_surface_2::Vertex& v)
{ return *(arr.insert_from_right_vertex(c, Vertex_handle(&v))); }

/*! Insert from the endpoint on the right given the halfedge that is incident
 * to the vertex of the endpoint.
 * The newly halfedge must be added immediately after the given halfedge in
 * the circular list around the vertex.
 */
Arrangement_on_surface_2::Halfedge&
insert_from_right_vertex2(Arrangement_on_surface_2& arr,
                          X_monotone_curve_2& c, Halfedge& h)
{ return *(arr.insert_from_right_vertex(c, Halfedge_handle(&h))); }

//!
Arrangement_on_surface_2::Halfedge&
insert_xcv_in_face_interior(Arrangement_on_surface_2& arr,
                            X_monotone_curve_2& c, Face& f)
{ return *(arr.insert_in_face_interior(c, Face_handle(&f))); }

//!
Arrangement_on_surface_2::Vertex&
insert_pnt_in_face_interior(Arrangement_on_surface_2& arr,
                            Point_2& p, Face& f)
{ return *(arr.insert_in_face_interior(p, Face_handle(&f))); }

//! Insert between the endpoints given the two vertices of the endpoints.
Arrangement_on_surface_2::Halfedge&
insert_at_vertices1(Arrangement_on_surface_2& arr,
                    X_monotone_curve_2& c, Vertex& v1, Vertex& v2)
{ return *(arr.insert_at_vertices(c, Vertex_handle(&v1), Vertex_handle(&v2))); }

//! Insert between the endpoints.
// Arrangement_on_surface_2::Halfedge&
// insert_at_vertices2(Arrangement_on_surface_2& arr,
//                     X_monotone_curve_2& c, Vertex& v1, Halfedge& h2)
// { return *(arr.insert_at_vertices(c, Vertex_handle(&v1), Halfedge_handle(&h2))); }

//! Insert between the endpoints.
Arrangement_on_surface_2::Halfedge&
insert_at_vertices3(Arrangement_on_surface_2& arr,
                    X_monotone_curve_2& c, Halfedge& h1, Vertex& v2)
{ return *(arr.insert_at_vertices(c, Halfedge_handle(&h1), Vertex_handle(&v2))); }

//! Insert between the endpoints.
Arrangement_on_surface_2::Halfedge&
insert_at_vertices4(Arrangement_on_surface_2& arr,
                    X_monotone_curve_2& c, Halfedge& h1, Halfedge& h2)
{ return *(arr.insert_at_vertices(c, Halfedge_handle(&h1), Halfedge_handle(&h2))); }

//!
Arrangement_on_surface_2::Vertex& modify_vertex(Arrangement_on_surface_2& arr,
                                                Vertex& v, Point_2& p)
{ return *(arr.modify_vertex(Vertex_handle(&v), p)); }

//!
Arrangement_on_surface_2::Face&
remove_isolated_vertex(Arrangement_on_surface_2& arr, Vertex& v)
{ return *(arr.remove_isolated_vertex(Vertex_handle(&v))); }

//!
Arrangement_on_surface_2::Halfedge& modify_edge(Arrangement_on_surface_2& arr,
                                                Halfedge& e,
                                                X_monotone_curve_2& c)
{ return *(arr.modify_edge(Halfedge_handle(&e), c)); }

//!
Arrangement_on_surface_2::Halfedge& split_edge(Arrangement_on_surface_2& arr,
                                               Halfedge& e,
                                               X_monotone_curve_2& c1,
                                               X_monotone_curve_2& c2)
{ return *(arr.split_edge(Halfedge_handle(&e), c1, c2)); }

//!
Arrangement_on_surface_2::Halfedge& merge_edge(Arrangement_on_surface_2& arr,
                                               Halfedge& e1, Halfedge& e2,
                                               X_monotone_curve_2& c)
{ return *(arr.merge_edge(Halfedge_handle(&e1), Halfedge_handle(&e2), c)); }

//!
Arrangement_on_surface_2::Face& remove_edge(Arrangement_on_surface_2& arr,
                                            Halfedge& e) {
  auto handle = e.twin();
  return *(arr.remove_edge(handle));
}

#if ((CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS) || \
       (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
       (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS))
//! \brief
//! \todo Turn into a template and compile in conditionaly
Arrangement_on_surface_2::Face& fictitious_face(Arrangement_on_surface_2& arr)
{ return *(arr.fictitious_face()); }
#endif

/// @}

/// \name Aos Iterators
/// @{

//!
py::object vertices(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.vertices_begin(), arr.vertices_end()); }

//!
py::object halfedges(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.halfedges_begin(), arr.halfedges_end()); }

//!
py::object edges(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.edges_begin(), arr.edges_end()); }

//!
py::object faces(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.faces_begin(), arr.faces_end()); }

//!
py::object unbounded_faces(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.unbounded_faces_begin(), arr.unbounded_faces_end()); }

/// @}

#if defined(CGALPY_AOS2_WITH_HISTORY)

//! Insert a curve into an arrangement with history
Arrangement_on_surface_with_history_2::Curve_halfedges&
insert_cv_with_history(Arrangement_on_surface_with_history_2& arr,
                       const Arrangement_on_surface_with_history_2::
                       Geometry_traits_2::Curve_2& cv) {
  using Cha = Arrangement_on_surface_with_history_2::Curve_handle;
  using Che = Arrangement_on_surface_with_history_2::Curve_halfedges;
  Cha cha = CGAL::insert(arr, cv);
  Che& x = *cha;
  return x;
}

//! Insert a list of curves into an arrangement with history.
void insert_curves_with_history(Arrangement_on_surface_with_history_2& arr,
                                py::list& lst) {
  if (lst.size() == 0) return;
  using Gt = Arrangement_on_surface_with_history_2::Geometry_traits_2;
  using Cv = Gt::Curve_2;
  if (py::isinstance<Cv>(lst[0])) {
    auto begin = stl_forward_iterator<const Cv&>(lst);
    auto end = stl_forward_iterator<const Cv&>(lst, false);
    CGAL::insert(arr, begin, end);
    return;
  }

  throw std::runtime_error("Attempting to insert a list of object of unrecognized type to an arrangement with history!");
}

//! Remove a curve from an arrangement with history.
Arrangement_on_surface_with_history_2::Size
remove_curve_with_history(Arrangement_on_surface_with_history_2& arr,
                          Arrangement_on_surface_with_history_2::Curve_halfedges& ch) {
  using Aos_wh = Arrangement_on_surface_with_history_2;
  return CGAL::remove_curve(arr, Aos_wh::Curve_handle(&ch));
}

/// \name Aos With History Iterators
/// @{

//!
std::size_t
number_of_originating_curves(const Arrangement_on_surface_with_history_2& arr_wh,
                             Halfedge& h) {
  return arr_wh.number_of_originating_curves(Halfedge_handle(&h));
}

//!
py::object
originating_curves(const Arrangement_on_surface_with_history_2& arr_wh,
                   Halfedge& e) {
  return make_iterator(arr_wh.originating_curves_begin(Halfedge_handle(&e)),
                       arr_wh.originating_curves_end(Halfedge_handle(&e)));
}

//!
py::object curves(const Arrangement_on_surface_with_history_2& arr_wh)
{ return make_iterator(arr_wh.curves_begin(), arr_wh.curves_end()); }

//!
py::object induced_edges(const Arrangement_on_surface_with_history_2& arr_wh,
                         const Arrangement_on_surface_with_history_2::
                         Curve_halfedges& ch) {
  using Aos_wh = Arrangement_on_surface_with_history_2;
  using Cch = Aos_wh::Curve_const_handle;
  return make_iterator(arr_wh.induced_edges_begin(Cch(&ch)),
                       arr_wh.induced_edges_end(Cch(&ch)));
}

//!
Arrangement_on_surface_with_history_2::Size
number_of_induced_edges(const Arrangement_on_surface_with_history_2& arr,
                        const Arrangement_on_surface_with_history_2::
                        Curve_halfedges& ch) {
  using Aos_wh = Arrangement_on_surface_with_history_2;
  return arr.number_of_induced_edges(Aos_wh::Curve_const_handle(&ch));
}

//
Arrangement_on_surface_with_history_2::Halfedge&
split_edge_with_history(Arrangement_on_surface_with_history_2& arr,
                        Arrangement_on_surface_with_history_2::Halfedge& e,
                        const Arrangement_on_surface_with_history_2::Point_2& p)
{ return *(arr.split_edge(Halfedge_handle(&e), p)); }

//
Arrangement_on_surface_with_history_2::Halfedge&
merge_edge_with_history(Arrangement_on_surface_with_history_2& arr,
                        Arrangement_on_surface_with_history_2::Halfedge& e1,
                        Arrangement_on_surface_with_history_2::Halfedge& e2)
{ return *(arr.merge_edge(Halfedge_handle(&e1), Halfedge_handle(&e2))); }

/// @}

#endif

/// \name Points and curves insertions
/// @{

/*! Insert a point into an arrangement.
 * \param arr the input arrangement
 */
Vertex& insert_point(Arrangement_on_surface_2& arr, const Point_2& p)
{ return *(CGAL::insert_point(arr, p)); }

/*! Insert a point into an arrangement.
 * \param arr the input arrangement
 */
template <typename PoinLocation>
Vertex& insert_point_pl(Arrangement_on_surface_2& arr, const Point_2& p,
                        const PoinLocation& pl)
{ return *(CGAL::insert_point(arr, p, pl)); }

/*! Insert an x-monotone curve into an arrangement.
 * \param arr the input arrangement
 * \pre: `xcv` does not intersect any curve in the arrangement in their
 *       interiors.
 */
Halfedge& insert_ni_cv(Arrangement_on_surface_2& arr,
                       const X_monotone_curve_2& xcv)
{ return *(CGAL::insert_non_intersecting_curve(arr, xcv)); }

/*! Insert a list of x-monotone curves into an arrangement.
 * \pre: The curves in the input list are pairwise interior disjoint any of
 *       them does not intersect any curve in the arrangement in their
 *       interiors.
 */
void insert_ni_cvs(Arrangement_on_surface_2& arr, py::list& lst) {
  auto begin = stl_forward_iterator<X_monotone_curve_2>(lst);
  auto end = stl_forward_iterator<X_monotone_curve_2>(lst, false);
  CGAL::insert_non_intersecting_curves(arr, begin, end);
}

/*! Insert an x-monotone curve into an arrangement.
 * \param arr the input arrangement
 * \pre: `xcv` does not intersect any curve in the arrangement in their
 *       interiors.
 */
template <typename Aos, typename PoinLocation>
Halfedge&
insert_ni_xcv_pl(Arrangement_on_surface_2& arr,
                 const typename Aos::Geometry_traits_2::X_monotone_curve_2& xcv,
                 const PoinLocation& pl)
{ return *(CGAL::insert_non_intersecting_curve(arr, xcv, pl)); }

//! Insert a curve into an arrangement.
template <typename Aos>
void insert_cv(Aos& arr, const typename Aos::Geometry_traits_2::Curve_2& cv)
{ CGAL::insert(arr, cv); }

//! Insert a curve into an arrangement.
template <typename Aos, typename PoinLocation>
void insert_cv_pl(Aos& arr,
                  const typename Aos::Geometry_traits_2::Curve_2& cv,
                  const PoinLocation& pl)
{ CGAL::insert(arr, cv, pl); }

// Insert a curve into an arrangement.
template <typename Aos>
void insert_xcv(Aos& aos,
                const typename Aos::Geometry_traits_2::X_monotone_curve_2& xcv)
{ CGAL::insert(aos, xcv); }

//! Insert a curve into an arrangement.
template <typename Aos>
void
insert_xcv_vertex(Aos& arr,
                  const typename Aos::Geometry_traits_2::X_monotone_curve_2& xcv,
                  const Vertex& v)
{ CGAL::insert(arr, xcv, Cell_const_variant(Vertex_const_handle(&v))); }

//! Insert a curve into an arrangement.
template <typename Aos>
void
insert_xcv_halfedge(Aos& arr,
                    const typename Aos::Geometry_traits_2::X_monotone_curve_2& xcv,
                    const Halfedge& h)
{ CGAL::insert(arr, xcv, Cell_const_variant(Halfedge_const_handle(&h))); }

//! Insert a curve into an arrangement.
template <typename Aos>
void
insert_xcv_face(Aos& arr,
                const typename Aos::Geometry_traits_2::X_monotone_curve_2& xcv,
                const Face& f)
{ CGAL::insert(arr, xcv, Cell_const_variant(Face_const_handle(&f))); }

//! Insert a curve into an arrangement.
template <typename Aos, typename PoinLocation>
void
insert_xcv_pl(Aos& arr,
              const typename Aos::Geometry_traits_2::X_monotone_curve_2& xcv,
              const PoinLocation& pl)
{ CGAL::insert(arr, xcv, pl); }

//! Obtain the geometry traits.
const Geometry_traits_2& geometry_traits(const Arrangement_on_surface_2& aos)
{ return *(aos.geometry_traits()); }

//! Obtain the topology traits.
const Topology_traits& topology_traits(const Arrangement_on_surface_2& aos)
{ return *(aos.topology_traits()); }

/// @}

/// \name Functions for Arrangement_2
/// @{

//! Obtain the unbounded face of an arrangement.
template <typename Arrangement_>
typename Arrangement_::Face& unbounded_face(Arrangement_& arr)
{ return *(arr.unbounded_face()); }

/// @}

//!
static PyType_Slot aos_observer_slots[] = {
  {Py_tp_traverse, (void*) Arr_observer::tp_traverse},
  {Py_tp_clear, (void*) Arr_observer::tp_clear},
  {0, nullptr}
};

//!
static PyType_Slot aos_overlay_function_traits_slots[] = {
  {Py_tp_traverse, (void*) Arr_overlay_function_traits::tp_traverse},
  {Py_tp_clear, (void*) Arr_overlay_function_traits::tp_clear},
  {0, nullptr}
};

//!
static PyType_Slot aos_overlay_traits_slots[] = {
  {Py_tp_traverse, (void*) Arr_overlay_traits::tp_traverse},
  {Py_tp_clear, (void*) Arr_overlay_traits::tp_clear},
  {0, nullptr}
};

}
} // namespace cgalpy

//! Export draw
template <typename Aos>
void export_draw(py::module_& m) {
#if ((CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS))
#ifdef CGALPY_HAS_VISUAL
  //! \todo The draw function should be applied only to arrangement on surface
  m.def("draw",
        [](const Aos& aos, const char* title)
        { CGAL::draw(aos, title); });

#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
  m.def("draw",
        [](const Aos& aos, const cgalpy::bvr::Graphics_scene_options& gso, const char* title)
        { CGAL::draw(aos, gso, title); });
#endif

#endif
#endif
}

//! Export common members of Aos types
void export_aos(py::module_& m) {
  using Aos = cgalpy::aos2::Arrangement_on_surface_2;
  using Gt = Aos::Geometry_traits_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Aos> aos_c(m, "Arrangement_on_surface_2");
  aos_c.def(py::init<>())
    .def(py::init<const Aos&>())
    .def(py::init<const Gt*>(), py::keep_alive<1, 2>())
    .def("geometry_traits", &cgalpy::aos2::geometry_traits, ri)
    .def("topology_traits", &cgalpy::aos2::topology_traits, ri)
#if ((CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS) || \
       (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
       (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS))
    .def("fictitious_face", &cgalpy::aos2::fictitious_face, ri)
#endif
    .def("insert_from_left_vertex", &cgalpy::aos2::insert_from_left_vertex1, ri)
    .def("insert_from_left_vertex", &cgalpy::aos2::insert_from_left_vertex2, ri)
    .def("insert_from_right_vertex", &cgalpy::aos2::insert_from_right_vertex1, ri)
    .def("insert_from_right_vertex", &cgalpy::aos2::insert_from_right_vertex2, ri)
    .def("insert_in_face_interior", &cgalpy::aos2::insert_xcv_in_face_interior, ri)
    .def("insert_in_face_interior", &cgalpy::aos2::insert_pnt_in_face_interior, ri)
    .def("insert_at_vertices", &cgalpy::aos2::insert_at_vertices1, ri)
    // .def("insert_at_vertices", &cgalpy::aos2::insert_at_vertices2, ri)
    .def("insert_at_vertices", &cgalpy::aos2::insert_at_vertices3, ri)
    .def("insert_at_vertices", &cgalpy::aos2::insert_at_vertices4, ri)
    .def("modify_vertex", &cgalpy::aos2::modify_vertex, ri)
    .def("remove_isolated_vertex", &cgalpy::aos2::remove_isolated_vertex, ri)
    .def("modify_edge", &cgalpy::aos2::modify_edge, ri)
    .def("split_edge", &cgalpy::aos2::split_edge, ri)
    .def("merge_edge", &cgalpy::aos2::merge_edge, ri)
    .def("remove_edge", &cgalpy::aos2::remove_edge, ri)
    .def("is_empty", &Aos::is_empty)
    .def("is_valid", &Aos::is_valid)
    .def("number_of_edges", &Aos::number_of_edges)
    .def("number_of_faces", &Aos::number_of_faces)
    .def("number_of_halfedges", &Aos::number_of_halfedges)
    .def("number_of_isolated_vertices", &Aos::number_of_isolated_vertices)
    .def("number_of_unbounded_faces", &Aos::number_of_unbounded_faces)
    .def("number_of_vertices", &Aos::number_of_vertices)
    .def("assign", &Aos::assign)
    .def("clear", &Aos::clear)
    ;

  using Vci = Aos::Vertex_const_iterator;
  using Hci = Aos::Halfedge_const_iterator;
  using Eci = Aos::Edge_const_iterator;
  using Fci = Aos::Face_const_iterator;
  using Ufci = Aos::Unbounded_face_const_iterator;
  using Vertex = Aos::Vertex;
  using Halfedge = Aos::Halfedge;
  using Face = Aos::Face;

  // Iterators
  add_iterator<Vci, Vci, const Vertex&>("Vertex_iterator", aos_c);
  add_iterator<Hci, Hci, const Halfedge&>("Halfedge_iterator", aos_c);
  add_iterator<Eci, Eci, const Halfedge&>("Edge_iterator", aos_c);
  add_iterator<Fci, Fci, const Face&>("Face_iterator", aos_c);
  add_iterator<Ufci, Ufci, const Face&>("Unbounded_face_iterator", aos_c);

  aos_c.def("vertices", &cgalpy::aos2::vertices, py::keep_alive<0, 1>())
    .def("halfedges", &cgalpy::aos2::halfedges, py::keep_alive<0, 1>())
    .def("edges", &cgalpy::aos2::edges, py::keep_alive<0, 1>())
    .def("faces", &cgalpy::aos2::faces, py::keep_alive<0, 1>())
    .def("unbounded_faces", &cgalpy::aos2::unbounded_faces, py::keep_alive<0, 1>());

  export_vertex(aos_c);
  export_halfedge(aos_c);
  export_face(aos_c);

  // Add convenient attributes:
  add_attr<Gt>(aos_c, "Geometry_traits_2");
  add_attr<Gt::Point_2>(aos_c, "Point_2");
  add_attr<Gt::Curve_2>(aos_c, "Curve_2");
  add_attr<Gt::X_monotone_curve_2>(aos_c, "X_monotone_curve_2");

// #if ((CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS))
// #ifdef CGALPY_HAS_VISUAL
//   using Draw_aos = void(*)(const Aos&, const char*);
//   m.def("draw", static_cast<Draw_aos>(CGAL::draw));
// #endif
// #endif
  export_draw<Aos>(m);
}

#if defined(CGALPY_AOS2_WITH_HISTORY)

//!
void export_aos_with_history(py::module_& m) {
  using Aos = cgalpy::aos2::Arrangement_on_surface_2;
  using Aos_wh = cgalpy::aos2::Arrangement_on_surface_with_history_2;
  using Gt = Aos_wh::Geometry_traits_2;
  using Cv = Gt::Curve_2;
  constexpr auto ri(py::rv_policy::reference_internal);
  constexpr auto ref(py::rv_policy::reference);

  py::class_<Aos_wh, Aos> awh_c(m, "Arrangement_on_surface_with_history_2");
  awh_c.def(py::init<>())
    .def(py::init<const Aos_wh&>())
    .def(py::init<const Gt*>(), py::keep_alive<1, 2>())
    .def("number_of_originating_curves", &cgalpy::aos2::number_of_originating_curves)
    .def("originating_curves", &cgalpy::aos2::originating_curves, py::keep_alive<0, 1>())
    .def("number_of_curves", &Aos_wh::number_of_curves)
    .def("curves", &cgalpy::aos2::curves, py::keep_alive<0, 1>())
    .def("number_of_induced_edges", &cgalpy::aos2::number_of_induced_edges)
    .def("induced_edges", &cgalpy::aos2::edges, py::keep_alive<0, 1>())
    .def("split_edge", &cgalpy::aos2::split_edge_with_history, ri)
    .def("merge_edge", &cgalpy::aos2::merge_edge_with_history, ri)
    ;

  using Oci = Aos_wh::Originating_curve_iterator;
  using Cci = Aos_wh::Curve_const_iterator;
  using Iei = Aos_wh::Originating_curve_iterator;
  add_iterator<Oci, Oci>("Originating_curve_iterator", awh_c);
  add_iterator<Cci, Cci>("Curve_iterator", awh_c);
  add_iterator<Iei, Iei>("Induced_edge_iterator", awh_c);

  using Ch = Aos_wh::Curve_halfedges;
  if (! add_attr<Ch>(awh_c, "Curve_halfedges")) {
    py::class_<Ch, Cv> ch_c(awh_c, "Curve_halfedges");
    ch_c.def(py::init<>())
      .def(py::init_implicit<const Cv&>())
      ;
  }

  //! \todo Why the f... reference_internal doesn't work?
  m.def("insert", &cgalpy::aos2::insert_curves_with_history)
    .def("insert", &cgalpy::aos2::insert_cv_with_history, ref)
    .def("remove_curve", &cgalpy::aos2::remove_curve_with_history)
    ;

  export_draw<Aos_wh>(m);
}

#endif

// Overlay function traits
template <bool VertexExtended, bool HalfedgeExtended, bool FaceExtended>
void bind_overlay_function_traits(py::module_& m) {
  using Aoft = cgalpy::aos2::Arr_overlay_function_traits;
  py::class_<Aoft>(m, "Arr_overlay_function_traits",
                   py::type_slots(cgalpy::aos2::aos_overlay_function_traits_slots))
    .def(py::init<>())
    .def(py::init<py::object>())
    .def(py::init<py::object, py::object, py::object, py::object, py::object,
         py::object, py::object, py::object, py::object, py::object>())
    .def("set_vv_v", &Aoft::set_vv_v)
    .def("set_ve_v", &Aoft::set_ve_v)
    .def("set_vf_v", &Aoft::set_vf_v)
    .def("set_ev_v", &Aoft::set_ev_v)
    .def("set_fv_v", &Aoft::set_fv_v)
    .def("set_ee_v", &Aoft::set_ee_v)
    .def("set_ee_e", &Aoft::set_ee_e)
    .def("set_ef_e", &Aoft::set_ef_e)
    .def("set_fe_e", &Aoft::set_fe_e)
    .def("set_ff_f", &Aoft::set_ff_f)
    ;
}

//!
template <>
void bind_overlay_function_traits<false, false, false>(py::module_& m) {
  using Aoft = cgalpy::aos2::Arr_overlay_function_traits;
  py::class_<Aoft>(m, "Arr_overlay_function_traits", py::type_slots(cgalpy::aos2::aos_overlay_function_traits_slots))
    .def(py::init<>())
    ;
}

//!
template <>
void bind_overlay_function_traits<false, false, true>(py::module_& m) {
  using Aoft = cgalpy::aos2::Arr_overlay_function_traits;

  py::class_<Aoft>(m, "Arr_overlay_function_traits",
                   py::type_slots(cgalpy::aos2::aos_overlay_function_traits_slots))
    .def(py::init<>())
    .def(py::init<py::object>())
    .def("set_ff_f", &Aoft::set_ff_f)
    ;
}

#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
//!
void export_arr(py::module_& m) {
  using Aos = cgalpy::aos2::Arrangement_on_surface_2;
  using Arr = cgalpy::aos2::Arrangement_2;
  using Gt = cgalpy::aos2::Geometry_traits_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Arr>(m, "Arrangement_2")) {
    py::class_<Arr, Aos> arr_c(m, "Arrangement_2");
    arr_c.def(py::init<>())
      .def(py::init<const Arr&>())
      .def(py::init<const Gt*>(), py::keep_alive<1, 2>())
      .def("unbounded_face", &cgalpy::aos2::unbounded_face<Arr>, ri)
      .def("number_of_vertices_at_infinity", &Arr::number_of_vertices_at_infinity)
      ;

    export_arrangement_2_io(arr_c);
  }

// #if ((CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS) || \
//      (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS))
// #ifdef CGALPY_HAS_VISUAL
//   //! \todo The draw function should be applied only to arrangement on surface
//   m.def("draw",
//         [](const Arr& arr, const char* title)
//         { CGAL::draw(arr, title); });

// #if defined(CGALPY_BASIC_VIEWER_BINDINGS)
//   m.def("draw",
//         [](const Arr& arr, const cgalpy::bvr::Graphics_scene_options& gso,
//            const char* title)
//         { CGAL::draw(arr, gso, title); });
// #endif
// #endif
// #endif
  export_draw<Arr>(m);
}
#endif

#if defined(CGALPY_AOS2_WITH_HISTORY)

#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
//
void export_arr_with_history(py::module_& m) {
  using Aos_wh = cgalpy::aos2::Arrangement_on_surface_with_history_2;
  using Arr_wh = cgalpy::aos2::Arrangement_with_history_2;
  using Gt = Arr_wh::Geometry_traits_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Arr_wh, Aos_wh> awh_c(m, "Arrangement_with_history_2");
  awh_c.def(py::init<>())
    .def(py::init<const Arr_wh&>())
    .def(py::init<const Gt*>(), py::keep_alive<1, 2>())
    .def("unbounded_face", &cgalpy::aos2::unbounded_face<Arr_wh>, ri)
    ;

  export_draw<Arr_wh>(m);
}
#endif

#endif

//!
void export_arrangement_on_surface_2(py::module_& m) {
  using Aos = cgalpy::aos2::Arrangement_on_surface_2;
  using Aos_wh = cgalpy::aos2::Arrangement_on_surface_with_history_2;
  using Gt = Aos::Geometry_traits_2;
  using Dcel = Aos::Dcel;
  using Pnt = Gt::Point_2;
  using Cv = Gt::Curve_2;
  using Xcv = Gt::X_monotone_curve_2;
  using Naive_pl = CGAL::Arr_naive_point_location<Aos>;

#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  using Wal_pl = CGAL::Arr_walk_along_line_point_location<Aos>;
#endif

  using Landmarks_pl = CGAL::Arr_landmarks_point_location<Aos>;
  using Trapezoid_pl = CGAL::Arr_trapezoid_ric_point_location<Aos>;
  constexpr auto ri(py::rv_policy::reference_internal);

  using Arr_halfedge_direction = CGAL::Arr_halfedge_direction;
  const py::handle info_ahd = py::type<Arr_halfedge_direction>();
  if (info_ahd.is_valid()) m.attr("Arr_halfedge_direction") = info_ahd;
  else {
    py::enum_<Arr_halfedge_direction>(m, "Arr_halfedge_direction")
      .value("ARR_RIGHT_TO_LEFT", Arr_halfedge_direction::ARR_RIGHT_TO_LEFT)
      .value("ARR_LEFT_TO_RIGHT", Arr_halfedge_direction::ARR_LEFT_TO_RIGHT)
      .export_values()
      ;
  }

  const py::handle info_ce = py::type<CGAL::Arr_curve_end>();
  if (info_ce.is_valid()) m.attr("Arr_curve_end") = info_ce;
  else {
    py::enum_<CGAL::Arr_curve_end>(m, "Arr_curve_end")
      .value("ARR_MIN_END", CGAL::Arr_curve_end::ARR_MIN_END)
      .value("ARR_MAX_END", CGAL::Arr_curve_end::ARR_MAX_END)
      .export_values()
      ;
  }

  add_attr<CGAL::Box_parameter_space_2>(m, "Arr_parameter_space");
  m.attr("ARR_LEFT_BOUNDARY") = &CGAL::ARR_LEFT_BOUNDARY;
  m.attr("ARR_RIGHT_BOUNDARY") = &CGAL::ARR_RIGHT_BOUNDARY;
  m.attr("ARR_BOTTOM_BOUNDARY") = &CGAL::ARR_BOTTOM_BOUNDARY;
  m.attr("ARR_TOP_BOUNDARY") = &CGAL::ARR_TOP_BOUNDARY;
  m.attr("ARR_INTERIOR") = &CGAL::ARR_INTERIOR;

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  export_arr_algebraic_segment_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS
  export_arr_bezier_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
  export_arr_circle_segment_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CONIC_GEOMETRY_TRAITS
  export_arr_conic_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  export_arr_geodesic_arc_on_sphere_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS
  export_arr_linear_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  export_arr_non_caching_segment_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_POLYLINE_OF_SEGMENTS_GEOMETRY_TRAITS
  export_arr_segment_traits_2(m);
  export_arr_polyline_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS
  export_arr_segment_traits_2(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_RATIONAL_FUNCTION_GEOMETRY_TRAITS
  export_arr_rational_function_traits_2(m);
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif

// Curve data
#if defined(CGALPY_AOS2_CURVE_DATA)
  export_arr_curve_data_traits_2(m);
#endif

// Consolidated curve data
#if defined(CGALPY_AOS2_CONSOLIDATED_CURVE_DATA)
  export_arr_consolidated_curve_data_traits_2(m);
#endif

#if defined(CGALPY_AOS2_COUNTING_TRAITS)
  export_arr_counting_traits_2(m);
#endif

#if defined(CGALPY_AOS2_TRACING_TRAITS)
  export_arr_tracing_traits_2(m);
#endif

// 3D Envelopes
#if defined(CGALPY_ENVELOPE_3_BINDINGS)
#if CGALPY_ENV3_GEOMETRY_TRAITS == CGALPY_ENV3_PLANE_GEOMETRY_TRAITS
  export_env_plane_traits_3(m);
#elif CGALPY_ENV3_GEOMETRY_TRAITS == CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS
  export_env_sphere_traits_3(m);
#elif CGALPY_ENV3_GEOMETRY_TRAITS == CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS
  export_env_tri_traits_3(m);
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_ENV3_TRAITS");
#endif
#if defined(CGALPY_ENV3_SURFACE_DATA)
  export_env_surface_data_traits_3(m);
#endif

#endif

  // 2D Regularized Boolean set operations
#if defined(CGALPY_BOOLEAN_SET_OPERATIONS_2_BINDINGS)
#if ((CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
     (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS))
  export_gps_segment_traits_2(m);
#else
  export_gps_traits_2(m);
#endif

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_CIRCLE_SEGMENT_GEOMETRY_TRAITS
  export_gps_circle_segment_traits_2(m);
#endif
#endif

  // Arrangement on surface
  if (! add_attr<Aos>(m, "Arrangement_on_surface_2")) export_aos(m);
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  using Arr = cgalpy::aos2::Arrangement_2;
  export_arr(m);
#endif

#if defined(CGALPY_AOS2_WITH_HISTORY)
  if constexpr(cgalpy::aos2::aos2_with_history()) {
    if (! add_attr<Aos_wh>(m, "Arrangement_on_surface_with_history_2"))
      export_aos_with_history(m);
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
    using Arr_wh = cgalpy::aos2::Arrangement_with_history_2;
    if (! add_attr<Arr_wh>(m, "Arrangement_with_history_2"))
      export_arr_with_history(m);
#endif
  }
#endif

  /// Free functions

  m.def("insert_point", &cgalpy::aos2::insert_point)
    .def("insert_point", &cgalpy::aos2::insert_point_pl<Naive_pl>)
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
    .def("insert_point", &cgalpy::aos2::insert_point_pl<Wal_pl>)
#endif
    .def("insert_point", &cgalpy::aos2::insert_point_pl<Trapezoid_pl>)
    ;

  m.def("insert_non_intersecting_curve", &cgalpy::aos2::insert_ni_cv)
    .def("insert_non_intersecting_curve", &cgalpy::aos2::insert_ni_xcv_pl<Aos, Naive_pl>)
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
    .def("insert_non_intersecting_curve", &cgalpy::aos2::insert_ni_xcv_pl<Aos, Wal_pl>)
#endif
    .def("insert_non_intersecting_curve", &cgalpy::aos2::insert_ni_xcv_pl<Aos, Trapezoid_pl>)
    .def("insert_non_intersecting_curves", &cgalpy::aos2::insert_ni_cvs)
    ;

  using Do_intersect = bool(*)(Aos&, const Xcv&);
  using Do_intersect_nv_pl = bool(*)(Aos&, const Xcv&, const Naive_pl&);
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  using Do_intersect_wl_pl = bool(*)(Aos&, const Xcv&, const Wal_pl&);
#endif
  using Do_intersect_tr_pl = bool(*)(Aos&, const Xcv&, const Trapezoid_pl&);
  using Do_intersect_lm_pl = bool(*)(Aos&, const Xcv&, const Landmarks_pl&);

  m.def("insert", &cgalpy::aos2::insert_cv<Aos>)
    .def("insert", &cgalpy::aos2::insert_cv_pl<Aos, Naive_pl>)
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
    .def("insert", &cgalpy::aos2::insert_cv_pl<Aos, Wal_pl>)
#endif
    .def("insert", &cgalpy::aos2::insert_cv_pl<Aos, Trapezoid_pl>)
    .def("insert", &cgalpy::aos2::insert_xcv<Aos>)
    .def("insert", &cgalpy::aos2::insert_xcv_pl<Aos, Naive_pl>)
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
    .def("insert", &cgalpy::aos2::insert_xcv_pl<Aos, Wal_pl>)
#endif
    .def("insert", &cgalpy::aos2::insert_xcv_pl<Aos, Trapezoid_pl>)
    .def("insert", &cgalpy::aos2::insert_xcv_vertex<Aos>)
    .def("insert", &cgalpy::aos2::insert_xcv_halfedge<Aos>)
    .def("insert", &cgalpy::aos2::insert_xcv_face<Aos>)
    .def("insert", &cgalpy::aos2::insert_curves)
    ;

  m.def("do_intersect", static_cast<Do_intersect>(CGAL::do_intersect))
    .def("do_intersect", static_cast<Do_intersect_nv_pl>(CGAL::do_intersect))
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
    .def("do_intersect", static_cast<Do_intersect_wl_pl>(CGAL::do_intersect))
#endif
    .def("do_intersect", static_cast<Do_intersect_tr_pl>(CGAL::do_intersect))
    ;

  m.def("decompose", &cgalpy::aos2::decompose, ri, py::keep_alive<1, 0>());

  m.def("zone", &cgalpy::aos2::zone)
    .def("zone", &cgalpy::aos2::zone_pl<Naive_pl>)
#if CGALPY_AOS2_GEOMETRY_TRAITS != CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
    .def("zone", &cgalpy::aos2::zone_pl<Wal_pl>)
#endif
    .def("zone", &cgalpy::aos2::zone_pl<Trapezoid_pl>)
    ;

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  m.def("insert_point", &cgalpy::aos2::insert_point_pl<Landmarks_pl>)
    .def("insert_non_intersecting_curve", &cgalpy::aos2::insert_ni_xcv_pl<Aos, Landmarks_pl>)
    .def("insert", &cgalpy::aos2::insert_cv_pl<Aos, Landmarks_pl>)
    .def("insert", &cgalpy::aos2::insert_xcv_pl<Aos, Landmarks_pl>)
    .def("do_intersect", static_cast<Do_intersect_lm_pl>(CGAL::do_intersect))
    .def("zone", &cgalpy::aos2::zone_pl<Landmarks_pl>)
    ;
#endif

  m.def("remove_edge", &cgalpy::aos2::remove_edge_free);
  m.def("remove_vertex", &cgalpy::aos2::remove_vertex_free);

  // Export overlay & overlay traits
  using Aoft = cgalpy::aos2::Arr_overlay_function_traits;
  if (! add_attr<Aoft>(m, "Arr_overlay_function_traits"))
    bind_overlay_function_traits<cgalpy::aos2::is_vertex_extended(), cgalpy::aos2::is_halfedge_extended(), cgalpy::aos2::is_face_extended()>(m);

  using Aot = cgalpy::aos2::Arr_overlay_traits;
  if (! add_attr<Aot>(m, "Arr_overlay_traits")) {
    py::class_<Aot>(m, "Arr_overlay_traits", py::type_slots(cgalpy::aos2::aos_overlay_traits_slots))
      .def(py::init<>())
      .def(py::init<py::object>())
      .def(py::init<py::object, py::object, py::object, py::object, py::object,
           py::object, py::object, py::object, py::object, py::object>())
      .def("set_vv_v", &Aot::set_vv_v)
      .def("set_ve_v", &Aot::set_ve_v)
      .def("set_vf_v", &Aot::set_vf_v)
      .def("set_ev_v", &Aot::set_ev_v)
      .def("set_fv_v", &Aot::set_fv_v)
      .def("set_ee_v", &Aot::set_ee_v)
      .def("set_ee_e", &Aot::set_ee_e)
      .def("set_ef_e", &Aot::set_ef_e)
      .def("set_fe_e", &Aot::set_fe_e)
      .def("set_ff_f", &Aot::set_ff_f)
      ;
  }

  m.def("overlay", &cgalpy::aos2::overlay);
  m.def("overlay", &cgalpy::aos2::overlay_tr<Aoft>);
  m.def("overlay", &cgalpy::aos2::overlay_tr<Aot>);

  using Aob = CGAL::Arr_observer<Aos>;
  if (! add_attr<Aob>(m, "Arr_observer_base")) {
    py::class_<Aob>(m, "Arr_observer_base")
      .def(py::init<>())
      .def(py::init<Aos&>(), py::keep_alive<1, 2>())
      .def("attach", &Aob::attach)
      .def("detach", &Aob::detach)
      ;
  }

  using Ao = cgalpy::aos2::Arr_observer;
  if (! add_attr<Ao>(m, "Arr_observer")) {
    py::class_<Ao, Aob>(m, "Arr_observer", py::type_slots(cgalpy::aos2::aos_observer_slots))
      .def(py::init<>())
      .def(py::init<Aos&>(), py::keep_alive<1, 2>())
      //
      .def("set_after_split_face", &Ao::set_after_split_face)
      .def("set_before_assign", &Ao::set_before_assign)
      .def("set_after_assign", &Ao::set_after_assign)
      .def("set_before_clear", &Ao::set_before_clear)
      .def("set_after_clear", &Ao::set_after_clear)
      .def("set_before_global_change", &Ao::set_before_global_change)
      .def("set_after_global_change", &Ao::set_after_global_change)
      .def("set_before_attach", &Ao::set_before_attach)
      .def("set_after_attach", &Ao::set_after_attach)
      .def("set_before_detach", &Ao::set_before_detach)
      .def("set_after_detach", &Ao::set_after_detach)
      .def("set_before_create_vertex", &Ao::set_before_create_vertex)
      .def("set_after_create_vertex", &Ao::set_after_create_vertex)
      .def("set_before_create_boundary_vertex1", &Ao::set_before_create_boundary_vertex1)
      .def("set_before_create_boundary_vertex2", &Ao::set_before_create_boundary_vertex2)
      .def("set_after_create_boundary_vertex", &Ao::set_after_create_boundary_vertex)
      .def("set_before_create_edge", &Ao::set_before_create_edge)
      .def("set_after_create_edge", &Ao::set_after_create_edge)
      .def("set_before_modify_vertex", &Ao::set_before_modify_vertex)
      .def("set_after_modify_vertex", &Ao::set_after_modify_vertex)
      .def("set_before_modify_edge", &Ao::set_before_modify_edge)
      .def("set_after_modify_edge", &Ao::set_after_modify_edge)
      .def("set_before_split_edge", &Ao::set_before_split_edge)
      .def("set_after_split_edge", &Ao::set_after_split_edge)
      .def("set_before_split_fictitious_edge", &Ao::set_before_split_fictitious_edge)
      .def("set_after_split_fictitious_edge", &Ao::set_after_split_fictitious_edge)
      .def("set_before_split_face", &Ao::set_before_split_face)
      .def("set_after_split_face", &Ao::set_after_split_face)
      .def("set_before_split_outer_ccb", &Ao::set_before_split_outer_ccb)
      .def("set_after_split_outer_ccb", &Ao::set_after_split_outer_ccb)
      .def("set_before_split_inner_ccb", &Ao::set_before_split_inner_ccb)
      .def("set_after_split_inner_ccb", &Ao::set_after_split_inner_ccb)
      .def("set_before_add_outer_ccb", &Ao::set_before_add_outer_ccb)
      .def("set_after_add_outer_ccb", &Ao::set_after_add_outer_ccb)
      .def("set_before_add_inner_ccb", &Ao::set_before_add_inner_ccb)
      .def("set_after_add_inner_ccb", &Ao::set_after_add_inner_ccb)
      .def("set_before_add_isolated_vertex", &Ao::set_before_add_isolated_vertex)
      .def("set_after_add_isolated_vertex", &Ao::set_after_add_isolated_vertex)
      .def("set_before_merge_edge", &Ao::set_before_merge_edge)
      .def("set_after_merge_edge", &Ao::set_after_merge_edge)
      .def("set_before_merge_fictitious_edge", &Ao::set_before_merge_fictitious_edge)
      .def("set_after_merge_fictitious_edge", &Ao::set_after_merge_fictitious_edge)
      .def("set_before_merge_face", &Ao::set_before_merge_face)
      .def("set_after_merge_face", &Ao::set_after_merge_face)
      .def("set_before_merge_outer_ccb", &Ao::set_before_merge_outer_ccb)
      .def("set_after_merge_outer_ccb", &Ao::set_after_merge_outer_ccb)
      .def("set_before_merge_inner_ccb", &Ao::set_before_merge_inner_ccb)
      .def("set_after_merge_inner_ccb", &Ao::set_after_merge_inner_ccb)
      .def("set_before_move_outer_ccb", &Ao::set_before_move_outer_ccb)
      .def("set_after_move_outer_ccb", &Ao::set_after_move_outer_ccb)
      .def("set_before_move_inner_ccb", &Ao::set_before_move_inner_ccb)
      .def("set_after_move_inner_ccb", &Ao::set_after_move_inner_ccb)
      .def("set_before_move_isolated_vertex", &Ao::set_before_move_isolated_vertex)
      .def("set_after_move_isolated_vertex", &Ao::set_after_move_isolated_vertex)
      .def("set_before_remove_vertex", &Ao::set_before_remove_vertex)
      .def("set_after_remove_vertex", &Ao::set_after_remove_vertex)
      .def("set_before_remove_edge", &Ao::set_before_remove_edge)
      .def("set_after_remove_edge", &Ao::set_after_remove_edge)
      .def("set_before_remove_outer_ccb", &Ao::set_before_remove_outer_ccb)
      .def("set_after_remove_outer_ccb", &Ao::set_after_remove_outer_ccb)
      .def("set_before_remove_inner_ccb", &Ao::set_before_remove_inner_ccb)
      .def("set_after_remove_inner_ccb", &Ao::set_after_remove_inner_ccb)
      ;
  }
}
