// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <boost/variant/variant.hpp>
#include <boost/static_assert.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_vertical_decomposition_2.h>
#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>
#include <CGAL/draw_arrangement_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/Arr_observer.hpp"
#include "CGALPY/Arr_overlay_traits.hpp"
#include "CGALPY/Arr_overlay_function_traits.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"

void export_vertex(py::class_<aos2::Arrangement_on_surface_2>&);
void export_halfedge(py::class_<aos2::Arrangement_on_surface_2>&);
void export_face(py::class_<aos2::Arrangement_on_surface_2>&);

py::object export_arr_linear_traits(py::module_&);
py::object export_arr_segment_traits(py::module_&);
py::object export_arr_non_caching_segment_traits(py::module_&);
py::object export_arr_circle_segment_traits(py::module_&);
py::object export_arr_conic_traits(py::module_&);
py::object export_arr_bezier_traits(py::module_&);
py::object export_arr_algebraic_segment_traits(py::module_&);
py::object export_arr_geodesic_arc_on_sphere_traits(py::module_&);

py::object export_gps_segment_traits(py::module_&);
py::object export_gps_circle_segment_traits(py::module_&);
py::object export_gps_traits(py::module_&);

namespace py = nanobind;

namespace aos2 {

typedef typename boost::variant<Vertex_const_handle,
                                Halfedge_const_handle,
                                Face_const_handle>      Cell_const_variant;

typedef Arr_overlay_function_traits<Arrangement_on_surface_2,
                                    Arrangement_on_surface_2,
                                    Arrangement_on_surface_2,
                                    py::object> Arr_overlay_function_traits;
typedef Arr_overlay_traits<Arrangement_on_surface_2, Arrangement_on_surface_2,
                           Arrangement_on_surface_2, py::object>
                                                        Arr_overlay_traits;

typedef Arr_observer<Arrangement_on_surface_2, py::object> Arr_observer;

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
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  else if (py::isinstance<Point_2>(lst[0])) {
    // Points must be wrapped into CGAL::Objects???
    auto begin = Object_input_iterator<Point_2>(lst);
    auto end = Object_input_iterator<Point_2>(lst, false);
    CGAL::insert(arr, begin, end);
  }
#endif
  else std::runtime_error("Attempting to insert a list of of object of unrecognized type to an arrangement!");
}

// Overlay two arrangements
void overlay(Arrangement_on_surface_2& arr1, Arrangement_on_surface_2& arr2,
             Arrangement_on_surface_2& arr_res)
{ CGAL::overlay(arr1, arr2, arr_res); }

template <typename OverlayTraits>
void overlay_tr(Arrangement_on_surface_2& arr1, Arrangement_on_surface_2& arr2,
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
{ return CGAL::remove_vertex(arr, Vertex_handle(&v)); }

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
                       const boost::optional<Cell_const_variant>& above,
                       py::list& lst) {
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
                  std::pair<boost::optional<Cell_const_variant>,
                            boost::optional<Cell_const_variant>>>
  Decompose_result;

void decompose_helper(const Decompose_result& res, py::list& lst) {
  const Vertex& vertex = *(res.first);
  const boost::optional<Cell_const_variant>& below = res.second.first;
  const boost::optional<Cell_const_variant>& above = res.second.second;

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

//
class Zone_object_visitor : public boost::static_visitor<py::object> {
public:
  template<typename T>
  py::object operator()(T operand) const { return py::cast(&(*operand)); }
};

//
py::list zone(Arrangement_on_surface_2& arr, X_monotone_curve_2& c) {
  py::list lst;
  auto op = [&] (const Cell_const_variant& o) mutable
    { lst.append(boost::apply_visitor(Zone_object_visitor(), o)); };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  CGAL::zone(arr, c, it);
  return lst;
}

template <typename PointLocation>
py::list zone_pl(Arrangement_on_surface_2& arr, X_monotone_curve_2& c,
                 PointLocation& pl) {
  py::list lst;
  auto op = [&] (const Cell_const_variant& o) mutable
    { lst.append(boost::apply_visitor(Zone_object_visitor(), o)); };
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

// Insert from the endpoint on the right given the halfedge that is incident
// to the vertex of the endpoint.
// The newly halfedge must be added immediately after the given halfedge in
// the circular list around the vertex.
Arrangement_on_surface_2::Halfedge&
insert_from_right_vertex2(Arrangement_on_surface_2& arr,
                          X_monotone_curve_2& c, Halfedge& h)
{ return *(arr.insert_from_right_vertex(c, Halfedge_handle(&h))); }

//
Arrangement_on_surface_2::Halfedge&
insert_xcv_in_face_interior(Arrangement_on_surface_2& arr,
                            X_monotone_curve_2& c, Face& f)
{ return *(arr.insert_in_face_interior(c, Face_handle(&f))); }

//
Arrangement_on_surface_2::Vertex&
insert_pnt_in_face_interior(Arrangement_on_surface_2& arr,
                            Point_2& p, Face& f)
{ return *(arr.insert_in_face_interior(p, Face_handle(&f))); }

// Insert between the endpoints given the two vertices of the endpoints.
Arrangement_on_surface_2::Halfedge&
insert_at_vertices1(Arrangement_on_surface_2& arr,
                    X_monotone_curve_2& c, Vertex& v1, Vertex& v2)
{ return *(arr.insert_at_vertices(c, Vertex_handle(&v1), Vertex_handle(&v2))); }

// // Insert between the endpoints.
// Arrangement_on_surface_2::Halfedge&
// insert_at_vertices2(Arrangement_on_surface_2& arr,
//                     X_monotone_curve_2& c, Vertex& v1, Halfedge& h2)
// { return *(arr.insert_at_vertices(c, Vertex_handle(&v1), Halfedge_handle(&h2))); }

// Insert between the endpoints.
Arrangement_on_surface_2::Halfedge&
insert_at_vertices3(Arrangement_on_surface_2& arr,
                    X_monotone_curve_2& c, Halfedge& h1, Vertex& v2)
{ return *(arr.insert_at_vertices(c, Halfedge_handle(&h1), Vertex_handle(&v2))); }

// Insert between the endpoints.
Arrangement_on_surface_2::Halfedge&
insert_at_vertices4(Arrangement_on_surface_2& arr,
                    X_monotone_curve_2& c, Halfedge& h1, Halfedge& h2)
{ return *(arr.insert_at_vertices(c, Halfedge_handle(&h1), Halfedge_handle(&h2))); }

//
Arrangement_on_surface_2::Vertex& modify_vertex(Arrangement_on_surface_2& arr,
                                                Vertex& v, Point_2& p)
{ return *(arr.modify_vertex(Vertex_handle(&v), p)); }

//
Arrangement_on_surface_2::Face&
remove_isolated_vertex(Arrangement_on_surface_2& arr, Vertex& v)
{ return *(arr.remove_isolated_vertex(Vertex_handle(&v))); }

//
Arrangement_on_surface_2::Halfedge& modify_edge(Arrangement_on_surface_2& arr,
                                                Halfedge& e,
                                                X_monotone_curve_2& c)
{ return *(arr.modify_edge(Halfedge_handle(&e), c)); }

//
Arrangement_on_surface_2::Halfedge& split_edge(Arrangement_on_surface_2& arr,
                                               Halfedge& e,
                                               X_monotone_curve_2& c1,
                                               X_monotone_curve_2& c2)
{ return *(arr.split_edge(Halfedge_handle(&e), c1, c2)); }

//
Arrangement_on_surface_2::Halfedge& merge_edge(Arrangement_on_surface_2& arr,
                                               Halfedge& e1, Halfedge& e2,
                                               X_monotone_curve_2& c)
{ return *(arr.merge_edge(Halfedge_handle(&e1), Halfedge_handle(&e2), c)); }

//
Arrangement_on_surface_2::Face& remove_edge(Arrangement_on_surface_2& arr,
                                            Halfedge& e) {
  auto handle = e.twin();
  return *(arr.remove_edge(handle));
}

//
Arrangement_on_surface_2::Face& fictitious_face(Arrangement_on_surface_2& arr)
{ return *(arr.fictitious_face()); }

/// @}

/// \name Iterators
/// @{

//
py::object vertices(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.vertices_begin(), arr.vertices_end()); }

//
py::object halfedges(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.halfedges_begin(), arr.halfedges_end()); }

//
py::object edges(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.edges_begin(), arr.edges_end()); }

//
py::object faces(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.faces_begin(), arr.faces_end()); }

//
py::object unbounded_faces(const Arrangement_on_surface_2& arr)
{ return make_iterator(arr.unbounded_faces_begin(), arr.unbounded_faces_end()); }

/// @}

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
  auto begin = stl_input_iterator<X_monotone_curve_2>(lst);
  auto end = stl_input_iterator<X_monotone_curve_2>(lst, false);
  CGAL::insert_non_intersecting_curves(arr, begin, end);
}

/*! Insert an x-monotone curve into an arrangement.
 * \param arr the input arrangement
 * \pre: `xcv` does not intersect any curve in the arrangement in their
 *       interiors.
 */
template <typename PoinLocation>
Halfedge& insert_ni_cv_pl(Arrangement_on_surface_2& arr,
                          const X_monotone_curve_2& xcv, const PoinLocation& pl)
{ return *(CGAL::insert_non_intersecting_curve(arr, xcv, pl)); }

// Insert a curve into an arrangement.
void insert_cv(Arrangement_on_surface_2& arr, const Curve_2& cv)
{ CGAL::insert(arr, cv); }

// Insert a curve into an arrangement.
template <typename PoinLocation>
void insert_cv_pl(Arrangement_on_surface_2& arr, const Curve_2& cv,
                  const PoinLocation& pl)
{ CGAL::insert(arr, cv, pl); }

// Insert a curve into an arrangement.
void insert_xcv(Arrangement_on_surface_2& arr, const X_monotone_curve_2& xcv)
{ CGAL::insert(arr, xcv); }

// Insert a curve into an arrangement.
void insert_xcv_vertex(Arrangement_on_surface_2& arr,
                       const X_monotone_curve_2& xcv, const Vertex& v)
{ CGAL::insert(arr, xcv, Cell_const_variant(Vertex_const_handle(&v))); }

// Insert a curve into an arrangement.
void insert_xcv_halfedge(Arrangement_on_surface_2& arr,
                         const X_monotone_curve_2& xcv, const Halfedge& h)
{ CGAL::insert(arr, xcv, Cell_const_variant(Halfedge_const_handle(&h))); }

// Insert a curve into an arrangement.
void insert_xcv_face(Arrangement_on_surface_2& arr,
                     const X_monotone_curve_2& xcv, const Face& f)
{ CGAL::insert(arr, xcv, Cell_const_variant(Face_const_handle(&f))); }

// Insert a curve into an arrangement.
template <typename PoinLocation>
void insert_xcv_pl(Arrangement_on_surface_2& arr, const X_monotone_curve_2& xcv,
                   const PoinLocation& pl)
{ CGAL::insert(arr, xcv, pl); }

/// @}

/// \name Functions for Arrangement_2
/// @{

// Draw an arrangement.
// void draw(const Arrangement_2& arr) { CGAL::draw(arr); }

// //! Obtain the unbounded face of an arrangement.
typename Arrangement_2::Face& unbounded_face(Arrangement_2& arr)
{ return *(arr.unbounded_face()); }

/// @}

}

// Export common members of Aos types
void export_aos(py::module_& m) {
  using Aos = aos2::Arrangement_on_surface_2;
  using GT = aos2::Geometry_traits_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Aos> aos_c(m, "Arrangement_on_surface_2");
  aos_c.def(py::init<>())
    .def(py::init<const Aos&>())
    .def(py::init<const GT*>())
    .def("fictitious_face", &aos2::fictitious_face, ri)
    .def("insert_from_left_vertex", &aos2::insert_from_left_vertex1, ri)
    .def("insert_from_left_vertex", &aos2::insert_from_left_vertex2, ri)
    .def("insert_from_right_vertex", &aos2::insert_from_right_vertex1, ri)
    .def("insert_from_right_vertex", &aos2::insert_from_right_vertex2, ri)
    .def("insert_in_face_interior", &aos2::insert_xcv_in_face_interior, ri)
    .def("insert_in_face_interior", &aos2::insert_pnt_in_face_interior, ri)
    .def("insert_at_vertices", &aos2::insert_at_vertices1, ri)
    // .def("insert_at_vertices", &aos2::insert_at_vertices2, ri)
    .def("insert_at_vertices", &aos2::insert_at_vertices3, ri)
    .def("insert_at_vertices", &aos2::insert_at_vertices4, ri)
    .def("modify_vertex", &aos2::modify_vertex, ri)
    .def("remove_isolated_vertex", &aos2::remove_isolated_vertex, ri)
    .def("modify_edge", &aos2::modify_edge, ri)
    .def("split_edge", &aos2::split_edge, ri)
    .def("merge_edge", &aos2::merge_edge, ri)
    .def("remove_edge", &aos2::remove_edge, ri)
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
  using Vertex = Aos::Vertex;
  using Halfedge = Aos::Halfedge;
  using Face = Aos::Face;

  // Iterators
  add_iterator<Vci, Vci, const Vertex&>("Vertex_iterator", aos_c);
  add_iterator<Hci, Hci, const Halfedge&>("Halfedge_iterator", aos_c);
  add_iterator<Eci, Eci, const Halfedge&>("Edge_iterator", aos_c);
  add_iterator<Fci, Fci, const Face&>("Face_iterator", aos_c);

  aos_c.def("vertices", &aos2::vertices, py::keep_alive<0, 1>())
    .def("halfedges", &aos2::halfedges, py::keep_alive<0, 1>())
    .def("edges", &aos2::edges, py::keep_alive<0, 1>())
    .def("faces", &aos2::faces, py::keep_alive<0, 1>())
    .def("unbounded_faces", &aos2::unbounded_faces, py::keep_alive<0, 1>());

  export_vertex(aos_c);
  export_halfedge(aos_c);
  export_face(aos_c);

  // Add convenient attributes:
  add_attr<GT>(aos_c, "Geometry_traits_2");
  add_attr<GT::Point_2>(aos_c, "Point_2");
  add_attr<GT::Curve_2>(aos_c, "Curve_2");
  add_attr<GT::X_monotone_curve_2>(aos_c, "X_monotone_curve_2");
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
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Arr, Aos> arr_c(m, "Arrangement_2");
  arr_c.def(py::init<>())
    .def(py::init<const Arr&>())
    .def(py::init<const GT*>())
    .def("unbounded_face", &aos2::unbounded_face, ri)
    .def("number_of_vertices_at_infinity", &Arr::number_of_vertices_at_infinity)
    ;

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  add_insertion(arr_c, "__str__");
  add_insertion(arr_c, "__repr__");
#endif
  // m.def("draw", &aos2::draw);
}

#endif

//
void export_arrangement_on_surface_2(py::module_& m) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Arr = aos2::Arrangement_2;
  using Awh = aos2::Arrangement_with_history_2;
  using GT = Aos::Geometry_traits_2;
  using Dcel = Aos::Dcel;
  using Pnt = GT::Point_2;
  using Cv = GT::Curve_2;
  using Xcv = GT::X_monotone_curve_2;
  using Naive_pl = CGAL::Arr_naive_point_location<Aos>;
  using Wal_pl = CGAL::Arr_walk_along_line_point_location<Aos>;
  using Landmarks_pl = CGAL::Arr_landmarks_point_location<Aos>;
  using Trapezoid_pl = CGAL::Arr_trapezoid_ric_point_location<Aos>;

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
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_BEZIER_GEOMETRY_TRAITS
  auto traits_c = export_arr_bezier_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_ALGEBRAIC_SEGMENT_GEOMETRY_TRAITS
  auto traits_c = export_arr_algebraic_segment_traits(m);
#elif CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_GEODESIC_ARC_ON_SPHERE_GEOMETRY_TRAITS
  auto traits_c = export_arr_geodesic_arc_on_sphere_traits(m);
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_AOS2_GEOMETRY_TRAITS");
#endif
#endif

  // Arrangement on surface
  if (! add_attr<Aos>(m, "Arrangement_on_surface_2")) export_aos(m);

#if CGALPY_AOS2_TYPE == CGALPY_AOS2_ARRANGEMENT
  if (! add_attr<Arr>(m, "Arrangement_2")) export_arr(m);
#endif

  /// Free functions

  m.def("insert_point", &aos2::insert_point)
    .def("insert_point", &aos2::insert_point_pl<Naive_pl>)
    .def("insert_point", &aos2::insert_point_pl<Wal_pl>)
    .def("insert_point", &aos2::insert_point_pl<Trapezoid_pl>)
    ;

  m.def("insert_non_intersecting_curve", &aos2::insert_ni_cv)
    .def("insert_non_intersecting_curve", &aos2::insert_ni_cv_pl<Naive_pl>)
    .def("insert_non_intersecting_curve", &aos2::insert_ni_cv_pl<Wal_pl>)
    .def("insert_non_intersecting_curve", &aos2::insert_ni_cv_pl<Trapezoid_pl>)
    .def("insert_non_intersecting_curves", &aos2::insert_ni_cvs)
    ;

  using Do_intersect = bool(*)(Aos&, const Xcv&);
  using Do_intersect_nv_pl = bool(*)(Aos&, const Xcv&, const Naive_pl&);
  using Do_intersect_wl_pl = bool(*)(Aos&, const Xcv&, const Wal_pl&);
  using Do_intersect_tr_pl = bool(*)(Aos&, const Xcv&, const Trapezoid_pl&);
  using Do_intersect_lm_pl = bool(*)(Aos&, const Xcv&, const Landmarks_pl&);

  m.def("insert", &aos2::insert_cv)
    .def("insert", &aos2::insert_cv_pl<Naive_pl>)
    .def("insert", &aos2::insert_cv_pl<Wal_pl>)
    .def("insert", &aos2::insert_cv_pl<Trapezoid_pl>)
    .def("insert", &aos2::insert_xcv)
    .def("insert", &aos2::insert_xcv_pl<Naive_pl>)
    .def("insert", &aos2::insert_xcv_pl<Wal_pl>)
    .def("insert", &aos2::insert_xcv_pl<Trapezoid_pl>)
    .def("insert", &aos2::insert_xcv_vertex)
    .def("insert", &aos2::insert_xcv_halfedge)
    .def("insert", &aos2::insert_xcv_face)
    .def("insert", &aos2::insert_curves)
    ;

  m.def("do_intersect", static_cast<Do_intersect>(CGAL::do_intersect))
    .def("do_intersect", static_cast<Do_intersect_nv_pl>(CGAL::do_intersect))
    .def("do_intersect", static_cast<Do_intersect_wl_pl>(CGAL::do_intersect))
    .def("do_intersect", static_cast<Do_intersect_tr_pl>(CGAL::do_intersect))
    ;

  m.def("decompose", &aos2::decompose);

  m.def("zone", &aos2::zone)
    .def("zone", &aos2::zone_pl<Naive_pl>)
    .def("zone", &aos2::zone_pl<Wal_pl>)
    .def("zone", &aos2::zone_pl<Trapezoid_pl>)
    ;

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)
  m.def("insert_point", &aos2::insert_point_pl<Landmarks_pl>)
    .def("insert_non_intersecting_curve", &aos2::insert_ni_cv_pl<Landmarks_pl>)
    .def("insert", &aos2::insert_cv_pl<Landmarks_pl>)
    .def("insert", &aos2::insert_xcv_pl<Landmarks_pl>)
    .def("do_intersect", static_cast<Do_intersect_lm_pl>(CGAL::do_intersect))
    .def("zone", &aos2::zone_pl<Landmarks_pl>)
    ;
#endif

  m.def("remove_edge", &aos2::remove_edge_free);
  m.def("remove_vertex", &aos2::remove_vertex_free);

  // Export overlay & overlay traits
  // bind_overlay_function_traits<aos2::is_vertex_extended(),
  //                              aos2::is_halfedge_extended(),
  //                              aos2::is_face_extended()>(m);

  constexpr auto ri(py::rv_policy::reference_internal);
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

  // m.def("overlay", &aos2::overlay);
  // m.def("overlay", &aos2::overlay_tr<aos2::Arr_overlay_function_traits>);
  m.def("overlay", &aos2::overlay_tr<aos2::Arr_overlay_traits>);

  // py::class_<aos2::Arr_observer>(m, "Arr_observer")
  //   .def(py::init<>())
  //   .def(py::init<Aos&>())
  //   .def("set_after_split_face", &aos2::Arr_observer::set_after_split_face)
  //   ;
}
