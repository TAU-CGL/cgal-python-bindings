// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <vector>

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>
// Do not include pair.h; if you must, you will need to fix the bindings of Facet (which is a pair...)
// #include <nanobind/stl/pair.h>
#include <nanobind/stl/tuple.h>

#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_triangulation_3.h>
#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
#include "cgalpy/basic_viewer_types.hpp"
#endif
#endif

#include "cgalpy/add_attr.hpp"
#include "cgalpy/add_extraction.hpp"
#include "cgalpy/add_insertion.hpp"
#include "cgalpy/export_circulator.hpp"
#include "cgalpy/make_iterator.hpp"
#include "cgalpy/stl_dereference_forward_iterator.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/triangulation_3_types.hpp"

#include "cgalpy/Tri3_docstrings.hpp"
#include "cgalpy/types.hpp"

void export_tri3_vertex(py::class_<cgalpy::tri3::Triangulation_3>&);
void export_tri3_cell(py::class_<cgalpy::tri3::Triangulation_3>&);

namespace py = nanobind;
namespace tri3_doc = cgalpy::tri3::docstrings;

namespace cgalpy {
namespace tri3 {

//! Determines whether two facets have the same vertices
bool are_equal1(const Triangulation_3& tri, Cell& c, int i, Cell& n, int j)
{ return tri.are_equal(Cell_handle(&c), i, Cell_handle(&n), j); }

//! Determines whether two facets have the same vertices
bool are_equal2(const Triangulation_3& tri, const Facet& f, Cell& n, int j)
{ return tri.are_equal(f, Cell_handle(&n), j); }

//!
size_type degree(const Triangulation_3& tri, Vertex& v)
{ return tri.degree(Vertex_handle(&v)); }

/// Flipers
/// @{

bool flip1(Triangulation_3& tri, Cell& c, int i) { return tri.flip(Cell_handle(&c), i); }
bool flip2(Triangulation_3& tri, Cell& c, int i, int j) { return tri.flip(Cell_handle(&c), i, j); }
void flip_flippable1(Triangulation_3& tri, Cell& c, int i) { tri.flip_flippable(Cell_handle(&c), i); }
void flip_flippable2(Triangulation_3& tri, Cell& c, int i, int j) { tri.flip_flippable(Cell_handle(&c), i, j); }

/// @}

/// Vertex existance predicates
/// @{

//!
auto has_vertex1(const Triangulation_3& tri, const Facet& f, Vertex& v)
{ return tri.has_vertex(f, Vertex_handle(&v)); }

//!
auto has_vertex2(const Triangulation_3& tri, Cell& c, int i, Vertex& v)
{ return tri.has_vertex(Cell_handle(&c), i, Vertex_handle(&v)); }

//!
auto has_vertex3(const Triangulation_3& tri, const Facet& f, Vertex& v) {
  int j;
  auto res = tri.has_vertex(f, Vertex_handle(&v), j);
  return (res) ? py::make_tuple(true, j) : py::make_tuple(false);
}

//!
auto has_vertex4(const Triangulation_3& tri, Cell& c, int i, Vertex& v) {
  int j;
  auto res = tri.has_vertex(Cell_handle(&c), i, Vertex_handle(&v), j);
  return (res) ? py::make_tuple(true, j) : py::make_tuple(false);
}

/// @}

//!
void tri3_init(tri3::Triangulation_3* tri, py::list& lst) {
  auto begin = stl_forward_iterator<tri3::Point>(lst);
  auto end = stl_forward_iterator<tri3::Point>(lst, false);
  new (tri) tri3::Triangulation_3(begin, end);  // placement new
}

//!
auto insert_points(Triangulation_3& tri, py::list& lst) {
  if (! lst) return 0l;
  if (! py::isinstance<tri3::Point>(lst[0])) return 0l;
  auto begin = stl_forward_iterator<tri3::Point>(lst);
  auto end = stl_forward_iterator<tri3::Point>(lst, false);
  return tri.insert(begin, end);
}

//!
Vertex& insert1(Triangulation_3& tri, const Point& p) {
  auto vh = tri.insert(p, Cell_handle());
  return *vh;
}

//!
Vertex& insert2(Triangulation_3& tri, const Point& p, Cell& start) {
  auto vh = tri.insert(p, tri3::Cell_handle(&start));
  return *vh;
}

//!
Vertex& insert3(Triangulation_3& tri, const Point& p, Vertex& hint) {
  auto vh = tri.insert(p, tri3::Vertex_handle(&hint));
  return *vh;
}

//!
Vertex& insert4(Triangulation_3& tri, const Point& p, Locate_type lt, tri3::Cell& lc, int li, int lj) {
  auto vh = tri.insert(p, lt, tri3::Cell_handle(&lc), li, lj);
  return *vh;
}

//!
Vertex& insert_in_cell(Triangulation_3& tri, const Point& p, Cell& c) {
  auto vh = tri.insert_in_cell(p, Cell_handle(&c));
  return *vh;
}

//!
Vertex& insert_in_edge1(Triangulation_3& tri, const Point& p, const Edge& e) {
  auto vh = tri.insert_in_edge(p, e);
  return *vh;
}

//!
Vertex& insert_in_edge2(Triangulation_3& tri, const Point& p, Cell& c, int i, int j) {
  auto vh = tri.insert_in_edge(p, Cell_handle(&c), i, j);
  return *vh;
}

//!
Vertex& insert_in_facet(Triangulation_3& tri, const Point& p, const Facet& f) {
  auto vh = tri.insert_in_facet(p, f);
  return *vh;
}

//!
Vertex& insert_in_hole1(Triangulation_3& tri, const Point& p, py::list& cells, Cell& start, int i) {
  auto begin = stl_dereference_forward_iterator<Cell_handle, Cell>(cells);
  auto end = stl_dereference_forward_iterator<Cell_handle, Cell>(cells, false);
  auto vh = tri.insert_in_hole(p, begin, end, Cell_handle(&start), i);
  return *vh;
}

//!
Vertex& insert_in_hole2(Triangulation_3& tri, const Point& p, py::list& cells, Cell& start, int i, Vertex& newv) {
  auto begin = stl_dereference_forward_iterator<Cell_handle>(cells);
  auto end = stl_dereference_forward_iterator<Cell_handle>(cells, false);
  auto vh = tri.insert_in_hole(p, begin, end, Cell_handle(&start), i, Vertex_handle(&newv));
  return *vh;
}

//!
Vertex& insert_outside_affine_hull(Triangulation_3& tri, const Point& p) {
  auto vh = tri.insert_outside_affine_hull(p);
  return *vh;
}

//!
Vertex& insert_outside_convex_hull(Triangulation_3& tri, const Point& p, Cell& c) {
  auto vh = tri.insert_outside_convex_hull(p, Cell_handle(&c));
  return *vh;
}

/// Queries
/// @{

//!
bool is_cell1(const Triangulation_3& tri, Cell& c)
{ return tri.is_cell(Cell_handle(&c)); }

//!
py::tuple is_cell2(const Triangulation_3& tri, Vertex& u, Vertex& v, Vertex& w, Vertex& x) {
  Cell_handle ch;
  auto res = tri.is_cell(Vertex_handle(&u), Vertex_handle(&v),
                         Vertex_handle(&w), Vertex_handle(&x), ch);
  if (res) return py::make_tuple(res, *ch);
  return py::make_tuple(false);
}

//!
py::tuple is_cell3(const Triangulation_3& tri, Vertex& u, Vertex& v, Vertex& w, Vertex& x) {
  Cell_handle ch;
  int i, j, k, l;
  auto res = tri.is_cell(Vertex_handle(&u), Vertex_handle(&v),
                         Vertex_handle(&w), Vertex_handle(&x), ch, i, j, k, l);
  if (res) return py::make_tuple(res, *ch, i, j, k, l);
  return py::make_tuple(false);
}

//!
py::tuple is_edge(const Triangulation_3& tri, Vertex& u, Vertex& v) {
  Cell_handle ch;
  int i, j;
  auto res = tri.is_edge(Vertex_handle(&u), Vertex_handle(&v), ch, i, j);
  if (res) return py::make_tuple(res, *ch, i, j);
  return py::make_tuple(false);
}

//!
py::tuple is_facet(const Triangulation_3& tri, Vertex& u, Vertex& v, Vertex& w) {
  Cell_handle ch;
  int i, j, k;
  auto res = tri.is_facet(Vertex_handle(&u), Vertex_handle(&v),
                          Vertex_handle(&w), ch, i, j, k);
  if (res) return py::make_tuple(res, *ch, i, j, k);
  return py::make_tuple(false);
}

//!
bool is_infinite1(const Triangulation_3& tri, Cell& c)
{ return tri.is_infinite(Cell_handle(&c)); }

//!
bool is_infinite2(const Triangulation_3& tri, Cell& c, int i)
{ return tri.is_infinite(Cell_handle(&c), i); }

//!
  bool is_infinite3(const Triangulation_3& tri, Cell& c, int i, int j) {
    return tri.is_infinite(Cell_handle(&c), i, j);
}

//!
bool is_infinite6(const Triangulation_3& tri, Vertex& v)
{ return tri.is_infinite(Vertex_handle(&v)); }

//!
bool is_valid2(const Triangulation_3& tri, Cell& c, bool verbose = false) {
  return tri.is_valid(Cell_handle(&c), verbose);
}

//!
py::tuple is_vertex1(const Triangulation_3& tri, const Point& p) {
  Vertex_handle vh;
  auto res = tri.is_vertex(p, vh);
  if (res) return py::make_tuple(res, *vh);
  return py::make_tuple(false);
}

//!
bool is_vertex2(const Triangulation_3& tri, Vertex& v)
{ return tri.is_vertex(Vertex_handle(&v)); }

/// @}

//!
Cell& locate1(const Triangulation_3& tri, const Point& query) {
  auto ch = tri.locate(query);
  return *ch;
}

//!
Cell& locate2(const Triangulation_3& tri, const Point& query, Cell& start) {
  auto ch = tri.locate(query, Cell_handle(&start));
  return *ch;
}

//!
Cell& locate3(const Triangulation_3& tri, const Point& query, Vertex& hint) {
  auto ch = tri.locate(query, Vertex_handle(&hint));
  return *ch;
}

//!
py::object locate_dispatch(py::handle self, Cell_handle ch, Locate_type lt, int li, int lj) {
  constexpr auto ri(py::rv_policy::reference_internal);

  switch (lt) {
   case Triangulation_3::FACET:
   case Triangulation_3::VERTEX: return py::make_tuple(py::cast(lt), py::cast(*ch, ri, self), py::int_(li));

   case Triangulation_3::EDGE: return py::make_tuple(py::cast(lt), py::cast(*ch, ri, self), py::int_(li), py::int_(lj));

   case Triangulation_3::CELL:
   case Triangulation_3::OUTSIDE_CONVEX_HULL: return py::make_tuple(py::cast(lt), py::cast(*ch, ri, self));

   case Triangulation_3::OUTSIDE_AFFINE_HULL: return py::make_tuple(py::cast(lt));
  }

  return py::make_tuple(py::cast(lt));
}

/*! We want to return a Python tuple of variable length. One element of the
 * returned py::tuple, namely a `tri3::Cell`, must have `reference_internal`
 * policy attached.
 *
 * In nanobind, policies like `reference_internal` are normally attached at
 * function return time via `py::return_value_policy`. However, when manually
 * construct a `py::tuple` to return multiple values, the policies of the
 * individual tuple elements are determined when each element is converted into
 * a Python object (py::cast is internally involved). Thus, to apply
 * `reference_internal` to a specific object inside a tuple, we need to manually
 * py::cast it with the policy we want.
 *
 * When specifying `py::rv_policy::reference_internal`, we must provide the
 * "parent" Python object as an argument to `py::cast()`.  Otherwise, nanobind
 * throws `std::bad_cast` at runtime, because it can't set up the ownership
 * properly. Therefore, we bind the method with self, you pass self itself as
 * the parent.
 */
py::object locate_get_incident1(py::handle self, const Point& query) {
  auto& tri = py::cast<Triangulation_3&>(self);
  Locate_type lt;
  int li, lj;
  auto ch = tri.locate(query, lt, li, lj);
  return locate_dispatch(self, ch, lt, li, lj);
}

//!
py::object locate_get_incident2(py::handle self, const Point& query, Cell& start) {
  auto& tri = py::cast<Triangulation_3&>(self);
  Locate_type lt;
  int li, lj;
  auto ch = tri.locate(query, lt, li, lj, Cell_handle(&start));
  return locate_dispatch(self, ch, lt, li, lj);
}

//!
py::object locate_get_incident3(py::handle self, const Point& query, Vertex& hint) {
  auto& tri = py::cast<Triangulation_3&>(self);
  Locate_type lt;
  int li, lj;
  auto ch = tri.locate(query, lt, li, lj, Vertex_handle(&hint));
  return locate_dispatch(self, ch, lt, li, lj);
}

//!
int mirror_index(const Triangulation_3& tri, Cell& c, int i) { return tri.mirror_index(Cell_handle(&c), i); }

//!
Vertex& mirror_vertex(const Triangulation_3& tri, Cell& c, int i) {
  auto vh = tri.mirror_vertex(Cell_handle(&c), i);
  return *vh;
}

//!
const Point_3& point1(const Triangulation_3& tri, Vertex& v) { return tri.point(Vertex_handle(&v)); }

//!
const Point_3& point2(const Triangulation_3& tri, Cell& c, int i) { return tri.point(Cell_handle(&c), i); }

//!
Segment_3 segment1(const Triangulation_3& tri, Edge& e) { return tri.segment(e); }

//!
Segment_3 segment2(const Triangulation_3& tri, Cell& c, int i, int j) { return tri.segment(Cell_handle(&c), i, j); }

//!
Triangle_3 triangle1(const Triangulation_3& tri, Facet& f) { return tri.triangle(f); }

//!
Triangle_3 triangle2(const Triangulation_3& tri, Cell& c, int i) { return tri.triangle(Cell_handle(&c), i); }

//!
Tetrahedron tetrahedron(const Triangulation_3& tri, Cell& c) { return tri.tetrahedron(Cell_handle(&c)); }

//!
py::object side_of_cell(const Triangulation_3& tri, const Point_3& p, Cell& c) {
  Locate_type lt;
  int li;
  int lj;
  auto res = tri.side_of_cell(p, Cell_handle(&c), lt, li, lj);
  if ((res == CGAL::ON_BOUNDED_SIDE) || (res == CGAL::ON_UNBOUNDED_SIDE)) return py::make_tuple(res);

  switch (lt) {
   case Triangulation_3::FACET:
   case Triangulation_3::VERTEX: return py::make_tuple(py::cast(CGAL::ON_BOUNDARY), py::cast(lt), py::int_(li));

   case Triangulation_3::EDGE:
    return py::make_tuple(py::cast(CGAL::ON_BOUNDARY), py::cast(lt), py::int_(li), py::int_(lj));

   case Triangulation_3::CELL:
   case Triangulation_3::OUTSIDE_CONVEX_HULL:
   case Triangulation_3::OUTSIDE_AFFINE_HULL: return py::make_tuple(py::cast(CGAL::ON_BOUNDARY), py::cast(lt));
  }

  return py::make_tuple(CGAL::ON_BOUNDARY);
}

//!
py::object side_of_facet(const Triangulation_3& tri, const Point_3& p, Facet& f) {
  Locate_type lt;
  int li;
  int lj;
  auto res = tri.side_of_facet(p, f, lt, li, lj);
  if ((res == CGAL::ON_BOUNDED_SIDE) || (res == CGAL::ON_UNBOUNDED_SIDE)) return py::make_tuple(res);

  switch (lt) {
   case Triangulation_3::VERTEX: return py::make_tuple(py::cast(CGAL::ON_BOUNDARY), py::cast(lt), py::int_(li));

   case Triangulation_3::EDGE:
    return py::make_tuple(py::cast(CGAL::ON_BOUNDARY), py::cast(lt), py::int_(li), py::int_(lj));

   case Triangulation_3::FACET:
   case Triangulation_3::CELL:
   case Triangulation_3::OUTSIDE_CONVEX_HULL:
   case Triangulation_3::OUTSIDE_AFFINE_HULL: return py::make_tuple(py::cast(CGAL::ON_BOUNDARY), py::cast(lt));
  }

  return py::make_tuple(CGAL::ON_BOUNDARY);
}

//!
py::object side_of_edge(const Triangulation_3& tri, const Point_3& p, Edge& e) {
  Locate_type lt;
  int li;
  auto res = tri.side_of_edge(p, e, lt, li);
  if ((res == CGAL::ON_BOUNDED_SIDE) || (res == CGAL::ON_UNBOUNDED_SIDE)) return py::make_tuple(res);

  switch (lt) {
   case Triangulation_3::VERTEX:return py::make_tuple(py::cast(CGAL::ON_BOUNDARY), py::cast(lt), py::int_(li));

   case Triangulation_3::EDGE:
   case Triangulation_3::FACET:
   case Triangulation_3::CELL:
   case Triangulation_3::OUTSIDE_CONVEX_HULL:
   case Triangulation_3::OUTSIDE_AFFINE_HULL: return py::make_tuple(py::cast(CGAL::ON_BOUNDARY), py::cast(lt));
  }

  return py::make_tuple(CGAL::ON_BOUNDARY);
}

//!
template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

//!
py::object all_cells(const Triangulation_3& tri)
{ return make_iterator(tri.all_cells_begin(), tri.all_cells_end()); }

//!
py::object all_edges(const Triangulation_3& tri)
{ return make_iterator(tri.all_edges_begin(), tri.all_edges_end()); }

//!
py::object all_facets(const Triangulation_3& tri)
{ return make_iterator(tri.all_facets_begin(), tri.all_facets_end()); }

//!
py::object all_vertices(const Triangulation_3& tri)
{ return make_iterator(tri.all_vertices_begin(), tri.all_vertices_end()); }

//
py::object finite_vertices(const Triangulation_3& tri)
{ return make_iterator(tri.finite_vertices_begin(), tri.finite_vertices_end()); }

//
py::object finite_edges(const Triangulation_3& tri)
{ return make_iterator(tri.finite_edges_begin(), tri.finite_edges_end()); }

//
py::object finite_cells(const Triangulation_3& tri)
{ return make_iterator(tri.finite_cells_begin(), tri.finite_cells_end()); }

//
py::object finite_facets(const Triangulation_3& tri)
{ return make_iterator(tri.finite_facets_begin(), tri.finite_facets_end()); }

//
py::object points(const Triangulation_3& tri)
{ return make_iterator(tri.points_begin(), tri.points_end()); }

//!
py::object segment_traverser_cells1(const Triangulation_3& tri,
                                    const Point_3& ps, const Point& pt) {
  return make_iterator(tri.segment_traverser_cells_begin(ps, pt),
                       tri.segment_traverser_cells_end());
}

//!
py::object segment_traverser_cells2(const Triangulation_3& tri,
                                   const Point_3& ps, const Point& pt,
                                   Cell& hint) {
  return make_iterator(tri.segment_traverser_cells_begin(ps, pt, Cell_handle(&hint)),
                       tri.segment_traverser_cells_end());
}

/// Iterators & Circulators
/// @{

//! Iterators

//! Cell Iterators

//!
py::object incident_cells1(const Triangulation_3& tri, const Edge& e)
{ return make_iterator_from_circulator(tri.incident_cells(e)); }

//!
py::object incident_cells2(const Triangulation_3& tri, Cell& c, int i, int j)
{ return make_iterator_from_circulator(tri.incident_cells(Cell_handle(&c), i, j)); }

//!
py::object incident_cells3(const Triangulation_3& tri, const Edge& e, Cell& start)
{ return make_iterator_from_circulator(tri.incident_cells(e, Cell_handle(&start))); }

//!
py::object incident_cells4(const Triangulation_3& tri, Cell& c, int i, int j, Cell& start) {
  return make_iterator_from_circulator(tri.incident_cells(Cell_handle(&c), i, j, Cell_handle(&start)));
}

//! Facet Iterators
//!
auto incident_facets1(const Triangulation_3& tri, const Edge& e)
{ return make_iterator_from_circulator(tri.incident_facets(e)); }

//!
auto incident_facets2(const Triangulation_3& tri, Cell& c, int i, int j)
{ return make_iterator_from_circulator(tri.incident_facets(Cell_handle(&c), i, j)); }

//!
auto incident_facets3(const Triangulation_3& tri, const Edge& e, const Facet& start)
{ return make_iterator_from_circulator(tri.incident_facets(e, start)); }

//!
auto incident_facets4(const Triangulation_3& tri, Cell& c, int i, int j, const Facet& start)
{ return make_iterator_from_circulator(tri.incident_facets(Cell_handle(&c), i, j, start)); }

//!
auto incident_facets5(const Triangulation_3& tri, const Edge& e, Cell& start, int f)
{ return make_iterator_from_circulator(tri.incident_facets(e, Cell_handle(&start), f)); }

//!
auto incident_facets6(const Triangulation_3& tri, Cell& c, int i, int j, Cell& start, int f) {
  return make_iterator_from_circulator(tri.incident_facets(Cell_handle(&c), i, j, Cell_handle(&start), f));
}

//! Circulators

//! Cell Circulators

//!
auto incident_cells_circulator1(const Triangulation_3& tri, const Edge& e)
{ return tri.incident_cells(e); }

//!
auto incident_cells_circulator2(const Triangulation_3& tri, Cell& c, int i, int j)
{ return tri.incident_cells(Cell_handle(&c), i, j); }

//!
auto incident_cells_circulator3(const Triangulation_3& tri, const Edge& e, Cell& start)
{ return tri.incident_cells(e, Cell_handle(&start)); }

//!
auto incident_cells_circulator4(const Triangulation_3& tri, Cell& c, int i, int j, Cell& start)
{ return tri.incident_cells(Cell_handle(&c), i, j, Cell_handle(&start)); }

//! Facet Circulators
//!
auto incident_facets_circulator1(const Triangulation_3& tri, const Edge& e)
{ return tri.incident_facets(e); }

//!
auto incident_facets_circulator2(const Triangulation_3& tri, Cell& c, int i, int j)
{ return tri.incident_facets(Cell_handle(&c), i, j); }

//!
auto incident_facets_circulator3(const Triangulation_3& tri, const Edge& e, const Facet& start)
{ return tri.incident_facets(e, start); }

//!
auto incident_facets_circulator4(const Triangulation_3& tri, Cell& c, int i, int j, const Facet& start)
{ return tri.incident_facets(Cell_handle(&c), i, j, start); }

//!
auto incident_facets_circulator5(const Triangulation_3& tri, const Edge& e, Cell& start, int f)
{ return tri.incident_facets(e, Cell_handle(&start), f); }

//!
auto incident_facets_circulator6(const Triangulation_3& tri, Cell& c, int i, int j, Cell& start, int f)
{ return tri.incident_facets(Cell_handle(&c), i, j, Cell_handle(&start), f); }

//! @}

//! @{
/// Container returning functions

//! \todo change py::list to narray

//!
auto adjacent_vertices(const Triangulation_3& tri, Vertex& v) {
  py::list res;
  auto op = [&] (const Vertex_handle vh) mutable { res.append(*vh); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.adjacent_vertices(Vertex_handle(&v), it);
  return res;
}

//!
auto finite_incident_cells(const Triangulation_3& tri, Vertex& v) {
  py::list res;
  auto op = [&] (const Cell_handle ch) mutable { res.append(*ch); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.finite_incident_cells(Vertex_handle(&v), it);
  return res;
}

//!
auto finite_incident_edges(const Triangulation_3& tri, Vertex& v) {
  py::list res;
  auto op = [&] (const Edge& e) mutable { res.append(e); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.finite_incident_edges(Vertex_handle(&v), it);
  return res;
}

//!
auto finite_incident_facets(const Triangulation_3& tri, Vertex& v) {
  py::list res;
  auto op = [&] (const Facet& f) mutable { res.append(f); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.finite_incident_facets(Vertex_handle(&v), it);
  return res;
}

//!
auto finite_adjacent_vertices(const Triangulation_3& tri, Vertex& v) {
  py::list res;
  auto op = [&] (const Vertex_handle vh) mutable { res.append(*vh); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.finite_adjacent_vertices(Vertex_handle(&v), it);
  return res;
}

//!
auto incident_edges(const Triangulation_3& tri, Vertex& v) {
  py::list res;
  auto op = [&] (const Edge& e) mutable { res.append(e); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.incident_edges(Vertex_handle(&v), it);
  return res;
}

//!
auto vertices1(const Triangulation_3& tri, const Edge& e) {
  py::list res;
  auto vhs = tri.vertices(e);
  for (auto vh : vhs) res.append(*vh);
  return res;
}

//!
auto vertices2(const Triangulation_3& tri, const Facet& f) {
  py::list res;
  auto vhs = tri.vertices(f);
  for (auto vh : vhs) res.append(*vh);
  return res;
}

//!
auto vertices3(const Triangulation_3& tri, Cell& c) {
  py::list res;
  auto vhs = tri.vertices(Cell_handle(&c));
  for (auto vh : vhs) res.append(*vh);
  return res;
}

//! @}

}
} // namespace cgalpy // End of namespace tri3

//!
void export_tri3_plain(py::module_& m) {
  using Tri = cgalpy::tri3::Triangulation_3;
  using Edge = cgalpy::tri3::Edge;
  using Cell = cgalpy::tri3::Cell;
  using Face = cgalpy::tri3::Facet;
  using Facet = cgalpy::tri3::Facet;
  using Pnt = cgalpy::tri3::Point;
  using Vertex = cgalpy::tri3::Vertex;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Tri>(m, "Triangulation_3")) return;

  py::class_<Tri> tri_c(m, "Triangulation_3",
                            tri3_doc::Triangulation_3_class);

  // Locate type
  py::enum_<cgalpy::tri3::Locate_type>(tri_c, "Locate_type")
    .value("VERTEX", Tri::VERTEX)
    .value("EDGE", Tri::EDGE)
    .value("FACET", Tri::FACET)
    .value("CELL", Tri::CELL)
    .value("OUTSIDE_CONVEX_HULL", Tri::OUTSIDE_CONVEX_HULL)
    .value("OUTSIDE_AFFINE_HULL", Tri::OUTSIDE_AFFINE_HULL)
    .export_values()
    ;

  // Types that have been registered already:
  if constexpr(cgalpy::tri3::is_periodic())
    // \todo: generate bindings for periodic traits
  ;
  else {
    // In the case of non-priodic triangulation the kernel serves as the traits.
    add_attr<cgalpy::tri3::Geom_traits>(tri_c, "Geom_traits");
  }

  if (! add_attr<Pnt>(tri_c, "Point")) std::cerr << "'Point' not registered!\n";
  add_attr<cgalpy::tri3::Segment>(tri_c, "Segment");
  add_attr<cgalpy::tri3::Triangle>(tri_c, "Triangle");
  add_attr<cgalpy::tri3::Tetrahedron>(tri_c, "Tetrahedron");

  export_tri3_vertex(tri_c);
  export_tri3_cell(tri_c);

  py::class_<Facet>(tri_c, "Facet",
                     "A triangulation facet represented by a cell and a local index.")
    .def("cell", [](Facet& f)->cgalpy::tri3::Cell& { return *(f.first); }, ri,
         "Return the cell containing this facet.")
    .def_rw("index", &Facet::second)
    ;

  py::class_<Edge>(tri_c, "Edge",
                    "A triangulation edge represented by a cell and two local indices.")
    .def("cell", [](Edge& e)->cgalpy::tri3::Cell& { return *(e.first); }, ri,
         "Return the cell containing this edge.")
    .def_rw("start_index", &Edge::second)
    .def_rw("end_index", &Edge::third)
    ;

  tri_c.def(py::init<>(),
            tri3_doc::Triangulation_3_Triangulation_3)
    .def(py::init<Tri&>(),
         py::arg("other"),
         tri3_doc::Triangulation_3_Triangulation_3)
    .def(py::init<const cgalpy::tri3::Traits&>(),
         py::arg("traits"),
         tri3_doc::Triangulation_3_Triangulation_3)
    .def("__init__", &cgalpy::tri3::tri3_init,
         py::arg("points"),
         tri3_doc::Triangulation_3_Triangulation_3_2)
    .def("are_equal",
         py::overload_cast<const Facet&, const Facet&>(&Tri::are_equal, py::const_),
         py::arg("f1"), py::arg("f2"),
         tri3_doc::Triangulation_3_are_equal)
    .def("are_equal", cgalpy::tri3::are_equal1,
         py::arg("c1"), py::arg("i1"), py::arg("c2"), py::arg("i2"),
         tri3_doc::Triangulation_3_are_equal_1)
    .def("are_equal", cgalpy::tri3::are_equal2,
         py::arg("f"), py::arg("c"), py::arg("i"),
         tri3_doc::Triangulation_3_are_equal_2)
    .def("clear", &Tri::clear, tri3_doc::Triangulation_3_clear)
    .def("degree", &cgalpy::tri3::degree, py::arg("v"),
         tri3_doc::Triangulation_3_degree)
    .def("dimension", &Tri::dimension,
         tri3_doc::Triangulation_3_dimension)

    // Flip
    .def("flip", py::overload_cast<const Edge&>(&Tri::flip),
         py::arg("e"),
         tri3_doc::Triangulation_3_flip)
    .def("flip", py::overload_cast<const Facet&>(&Tri::flip),
         py::arg("f"),
         tri3_doc::Triangulation_3_flip_2)
    .def("flip", &cgalpy::tri3::flip1,
         py::arg("c"), py::arg("i"),
         tri3_doc::Triangulation_3_flip_3)
    .def("flip", &cgalpy::tri3::flip2,
         py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_flip_1)
    .def("flip_flippable", py::overload_cast<const Edge&>(&Tri::flip_flippable),
         py::arg("e"),
         tri3_doc::Triangulation_3_flip_flippable)
    .def("flip_flippable", py::overload_cast<const Facet&>(&Tri::flip_flippable),
         py::arg("f"),
         tri3_doc::Triangulation_3_flip_flippable_1)
    .def("flip_flippable", &cgalpy::tri3::flip_flippable1,
         py::arg("c"), py::arg("i"),
         tri3_doc::Triangulation_3_flip_flippable_1)
    .def("flip_flippable", &cgalpy::tri3::flip_flippable2,
         py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_flip_flippable)

    // Geometry traits
    .def("geom_traits", &Tri::geom_traits, ri,
         tri3_doc::Triangulation_3_geom_traits)

    // Has vertex
    .def("has_vertex", &cgalpy::tri3::has_vertex1,
         py::arg("f"), py::arg("v"),
         tri3_doc::Triangulation_3_has_vertex_3)
    .def("has_vertex", &cgalpy::tri3::has_vertex2,
         py::arg("c"), py::arg("i"), py::arg("v"),
         tri3_doc::Triangulation_3_has_vertex_3)
    .def("has_vertex_get_index", &cgalpy::tri3::has_vertex3,
         py::arg("f"), py::arg("v"),
         tri3_doc::Triangulation_3_has_vertex)
    .def("has_vertex_get_index", &cgalpy::tri3::has_vertex4,
         py::arg("c"), py::arg("i"), py::arg("v"),
         tri3_doc::Triangulation_3_has_vertex_1)

    // Insertion
    .def("insert", &cgalpy::tri3::insert1, ri,
         py::arg("p"),
         tri3_doc::Triangulation_3_insert)
    .def("insert", &cgalpy::tri3::insert2, ri,
         py::arg("p"), py::arg("start"),
         tri3_doc::Triangulation_3_insert)
    .def("insert", &cgalpy::tri3::insert3, ri,
         py::arg("p"), py::arg("hint"),
         tri3_doc::Triangulation_3_insert_1)
    .def("insert", &cgalpy::tri3::insert4, ri,
         py::arg("p"), py::arg("lt"), py::arg("lc"), py::arg("li"), py::arg("lj"),
         tri3_doc::Triangulation_3_insert_2)
    .def("insert", &cgalpy::tri3::insert_points,
         py::arg("points"),
         tri3_doc::Triangulation_3_insert_4)
    .def("insert_in_cell", &cgalpy::tri3::insert_in_cell, ri,
         py::arg("p"), py::arg("c"),
         tri3_doc::Triangulation_3_insert_in_cell)
    .def("insert_in_edge", &cgalpy::tri3::insert_in_edge1, ri,
         py::arg("p"), py::arg("e"),
         tri3_doc::Triangulation_3_insert_in_edge)
    .def("insert_in_edge", &cgalpy::tri3::insert_in_edge2, ri,
         py::arg("p"), py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_insert_in_edge_1)
    .def("insert_in_facet", &cgalpy::tri3::insert_in_facet, ri,
         py::arg("p"), py::arg("f"),
         tri3_doc::Triangulation_3_insert_in_facet)
    .def("insert_in_hole", &cgalpy::tri3::insert_in_hole1, ri,
         py::arg("p"), py::arg("cells"), py::arg("start"), py::arg("i"),
         tri3_doc::Triangulation_3_insert_in_hole)
    .def("insert_in_hole", &cgalpy::tri3::insert_in_hole2, ri,
         py::arg("p"), py::arg("cells"), py::arg("start"), py::arg("i"), py::arg("newv"),
         tri3_doc::Triangulation_3_insert_in_hole_1)
    .def("insert_outside_affine_hull", &cgalpy::tri3::insert_outside_affine_hull, ri,
         py::arg("p"),
         tri3_doc::Triangulation_3_insert_outside_affine_hull)
    .def("insert_outside_convex_hull", &cgalpy::tri3::insert_outside_convex_hull, ri,
         py::arg("p"), py::arg("c"),
         tri3_doc::Triangulation_3_insert_outside_convex_hull)

    // Queries
    .def("is_cell", &cgalpy::tri3::is_cell1,
         py::arg("c"),
         tri3_doc::Triangulation_3_is_cell)
    .def("is_cell", &cgalpy::tri3::is_cell2,
         py::arg("u"), py::arg("v"), py::arg("w"), py::arg("x"),
         tri3_doc::Triangulation_3_is_cell_1)
    .def("is_cell_get_cell", &cgalpy::tri3::is_cell3,
         py::arg("u"), py::arg("v"), py::arg("w"), py::arg("x"),
         tri3_doc::Triangulation_3_is_cell_2)
    .def("is_edge", &cgalpy::tri3::is_edge,
         py::arg("u"), py::arg("v"),
         tri3_doc::Triangulation_3_is_edge)
    .def("is_facet", &cgalpy::tri3::is_facet,
         py::arg("u"), py::arg("v"), py::arg("w"),
         tri3_doc::Triangulation_3_is_facet)
    .def("is_infinite", &cgalpy::tri3::is_infinite1,
         py::arg("c"),
         tri3_doc::Triangulation_3_is_infinite_1)
    .def("is_infinite", &cgalpy::tri3::is_infinite2,
         py::arg("c"), py::arg("i"),
         tri3_doc::Triangulation_3_is_infinite_2)
    .def("is_infinite", &cgalpy::tri3::is_infinite3,
         py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_is_infinite_4)
    .def("is_infinite", py::overload_cast<const Edge&>(&Tri::is_infinite, py::const_),
         py::arg("e"),
         tri3_doc::Triangulation_3_is_infinite_5)
    .def("is_infinite", py::overload_cast<const Facet&>(&Tri::is_infinite, py::const_),
         py::arg("f"),
         tri3_doc::Triangulation_3_is_infinite_3)
    .def("is_infinite", &cgalpy::tri3::is_infinite6,
         py::arg("v"),
         tri3_doc::Triangulation_3_is_infinite)
    .def("is_valid",
         py::overload_cast<bool, int>(&Tri::is_valid, py::const_),
         py::arg("verbose") = false, py::arg("level") = 0,
         tri3_doc::Triangulation_3_is_valid)
    .def("is_valid", &cgalpy::tri3::is_valid2,
         py::arg("c"), py::arg("verbose") = false,
         tri3_doc::Triangulation_3_is_valid_1)
    .def("is_vertex", &cgalpy::tri3::is_vertex1,
         py::arg("p"),
         tri3_doc::Triangulation_3_is_vertex)
    .def("is_vertex", &cgalpy::tri3::is_vertex2,
         py::arg("v"),
         tri3_doc::Triangulation_3_is_vertex_1)

    // Locators
    .def("locate", &cgalpy::tri3::locate1, ri,
         py::arg("query"),
         tri3_doc::Triangulation_3_locate)
    .def("locate", &cgalpy::tri3::locate2, ri,
         py::arg("query"), py::arg("start"),
         tri3_doc::Triangulation_3_locate)
    .def("locate", &cgalpy::tri3::locate3, ri,
         py::arg("query"), py::arg("hint"),
         tri3_doc::Triangulation_3_locate_1)
    .def("locate_get_incident", &cgalpy::tri3::locate_get_incident1,
         py::arg("query"),
         tri3_doc::Triangulation_3_locate_2)
    .def("locate_get_incident", &cgalpy::tri3::locate_get_incident2,
         py::arg("query"), py::arg("start"),
         tri3_doc::Triangulation_3_locate_2)
    .def("locate_get_incident", &cgalpy::tri3::locate_get_incident3,
         py::arg("query"), py::arg("hint"),
         tri3_doc::Triangulation_3_locate_3)

    // Mirros
    .def("mirror_facet", &Tri::mirror_facet, py::arg("f"),
         tri3_doc::Triangulation_3_mirror_facet)
    .def("mirror_index", &cgalpy::tri3::mirror_index,
         py::arg("c"), py::arg("i"),
         tri3_doc::Triangulation_3_mirror_index)
    .def("mirror_vertex", &cgalpy::tri3::mirror_vertex, ri,
         py::arg("c"), py::arg("i"),
         tri3_doc::Triangulation_3_mirror_vertex)

    // Quantifiers
    .def("number_of_cells", &Tri::number_of_cells,
         tri3_doc::Triangulation_3_number_of_cells)
    .def("number_of_edges", &Tri::number_of_edges,
         tri3_doc::Triangulation_3_number_of_edges)
    .def("number_of_facets", &Tri::number_of_facets,
         tri3_doc::Triangulation_3_number_of_facets)
    .def("number_of_finite_cells", &Tri::number_of_finite_cells,
         tri3_doc::Triangulation_3_number_of_finite_cells)
    .def("number_of_finite_edges", &Tri::number_of_finite_edges,
         tri3_doc::Triangulation_3_number_of_finite_edges)
    .def("number_of_finite_facets", &Tri::number_of_finite_facets,
         tri3_doc::Triangulation_3_number_of_finite_facets)
    .def("number_of_vertices", &Tri::number_of_vertices,
         tri3_doc::Triangulation_3_number_of_vertices)

    // Inequalities
    .def("__eq__", [](const Tri& t1, const Tri& t2)->bool { return t1 == t2; },
         py::arg("other"),
         "Return whether this triangulation equals another triangulation.")
    .def("__ne__", [](const Tri& t1, const Tri& t2)->bool { return t1 != t2; },
         py::arg("other"),
         "Return whether this triangulation differs from another triangulation.")

    // Point, segment, triangle, & tetrahedron
    .def("point", &cgalpy::tri3::point1, ri, py::arg("v"),
         tri3_doc::Triangulation_3_point_1)
    .def("point", &cgalpy::tri3::point2, ri, py::arg("c"), py::arg("i"),
         tri3_doc::Triangulation_3_point)
    .def("segment", &cgalpy::tri3::segment1, py::arg("e"),
         tri3_doc::Triangulation_3_segment)
    .def("segment", &cgalpy::tri3::segment2,
         py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_segment_1)
    .def("triangle", &cgalpy::tri3::triangle1, py::arg("f"),
         tri3_doc::Triangulation_3_triangle_1)
    .def("triangle", &cgalpy::tri3::triangle2,
         py::arg("c"), py::arg("i"),
         tri3_doc::Triangulation_3_triangle)
    .def("tetrahedron", &cgalpy::tri3::tetrahedron, py::arg("c"),
         tri3_doc::Triangulation_3_tetrahedron)

    // Side of
    .def("side_of_cell", &cgalpy::tri3::side_of_cell,
         py::arg("p"), py::arg("c"),
         tri3_doc::Triangulation_3_side_of_cell)
    .def("side_of_edge", &cgalpy::tri3::side_of_edge,
         py::arg("p"), py::arg("e"),
         tri3_doc::Triangulation_3_side_of_edge)
    // .def("side_of_edge", )
    .def("side_of_facet", &cgalpy::tri3::side_of_facet,
         py::arg("p"), py::arg("f"),
         tri3_doc::Triangulation_3_side_of_facet)
    // .def("side_of_facet", )

    // Misc.
    .def("swap", &Tri::swap, py::arg("tri"),
         tri3_doc::Triangulation_3_swap)
    // set_infinite_vertex
    // set_lock_data_structure
    // try_lock_and_get_incident_cells
    // tds
    ;

  using Avi = Tri::All_vertices_iterator;
  using Aei = Tri::All_edges_iterator;
  using Aci = Tri::All_cells_iterator;
  using Afi = Tri::All_facets_iterator;

  using Fvi = Tri::Finite_vertices_iterator;
  using Fei = Tri::Finite_edges_iterator;
  using Fci = Tri::Finite_cells_iterator;
  using Ffi = Tri::Finite_facets_iterator;

  using Pi = Tri::Point_iterator;

  using Sci = Tri::Segment_cell_iterator;

  // Iterators
  add_iterator<Avi, Avi, const Vertex&>("All_vertices_iterator", tri_c);
  add_iterator<Aei, Aei>("All_edges_iterator", tri_c);
  add_iterator<Aci, Aci, const Cell&>("All_cells_iterator", tri_c);
  add_iterator<Afi, Afi, const Face&>("All_facets_iterator", tri_c);

  add_iterator<Fvi, Fvi, const Vertex&>("Finite_vertices_iterator", tri_c);
  add_iterator<Fei, Fei>("Finite_edges_iterator", tri_c);
  add_iterator<Fci, Fci, const Cell&>("Finite_cells_iterator", tri_c);
  add_iterator<Ffi, Ffi, const Face&>("Finite_facets_iterator", tri_c);

  add_iterator<Pi, Pi, const Pnt&>("Point_iterator", tri_c);

  add_iterator<Sci, Sci, const Cell&>("Segment_cell_iterator", tri_c);

  tri_c.def("all_cells", &cgalpy::tri3::all_cells, py::keep_alive<0, 1>(),
            tri3_doc::Triangulation_3_all_cell_handles)
    .def("all_edges", &cgalpy::tri3::all_edges, py::keep_alive<0, 1>(),
         tri3_doc::Triangulation_3_all_edges)
    .def("all_facets", &cgalpy::tri3::all_facets, py::keep_alive<0, 1>(),
         tri3_doc::Triangulation_3_all_facets)
    .def("all_vertices", &cgalpy::tri3::all_vertices, py::keep_alive<0, 1>(),
         tri3_doc::Triangulation_3_all_vertex_handles)

    .def("finite_cells", &cgalpy::tri3::finite_cells, py::keep_alive<0, 1>(),
         tri3_doc::Triangulation_3_finite_cell_handles)
    .def("finite_edges", &cgalpy::tri3::finite_edges, py::keep_alive<0, 1>(),
         tri3_doc::Triangulation_3_finite_edges)
    .def("finite_facets", &cgalpy::tri3::finite_facets, py::keep_alive<0, 1>(),
         tri3_doc::Triangulation_3_finite_facets)
    .def("finite_vertices", &cgalpy::tri3::finite_vertices, py::keep_alive<0, 1>(),
         tri3_doc::Triangulation_3_finite_vertex_handles)

    .def("points", &cgalpy::tri3::points, py::keep_alive<0, 1>(),
         tri3_doc::Triangulation_3_points)

    .def("segment_traverser_cells", &cgalpy::tri3::segment_traverser_cells1,
         py::keep_alive<0, 1>(), py::arg("ps"), py::arg("pt"),
         tri3_doc::Triangulation_3_segment_traverser_cells_begin_1)
    .def("segment_traverser_cells", &cgalpy::tri3::segment_traverser_cells2,
         py::keep_alive<0, 1>(), py::arg("ps"), py::arg("pt"), py::arg("hint"),
         tri3_doc::Triangulation_3_segment_traverser_cells_begin_1)
    ;

  // Iterators
  using Cc = Tri::Cell_circulator;
  using Fc = Tri::Facet_circulator;

  add_iterator_from_circulator<Cc>("Cell_iterator", tri_c);
  tri_c.def("incident_cells", &cgalpy::tri3::incident_cells1,
            py::keep_alive<0, 1>(), py::arg("e"),
            tri3_doc::Triangulation_3_incident_cells)
    .def("incident_cells", &cgalpy::tri3::incident_cells2,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_incident_cells_1)
    .def("incident_cells", &cgalpy::tri3::incident_cells3,
         py::keep_alive<0, 1>(), py::arg("e"), py::arg("start"),
         tri3_doc::Triangulation_3_incident_cells_2)
    .def("incident_cells", &cgalpy::tri3::incident_cells4,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"), py::arg("start"),
         tri3_doc::Triangulation_3_incident_cells_3)
    ;

  add_iterator_from_circulator<Fc>("Facet_iterator", tri_c);
  tri_c.def("incident_facets", &cgalpy::tri3::incident_facets1,
            py::keep_alive<0, 1>(), py::arg("e"),
            tri3_doc::Triangulation_3_incident_facets)
    .def("incident_facets", &cgalpy::tri3::incident_facets2,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_incident_facets_1)
    .def("incident_facets", &cgalpy::tri3::incident_facets3,
         py::keep_alive<0, 1>(), py::arg("e"), py::arg("start"),
         tri3_doc::Triangulation_3_incident_facets_2)
    .def("incident_facets", &cgalpy::tri3::incident_facets4,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"), py::arg("start"),
         tri3_doc::Triangulation_3_incident_facets_4)
    .def("incident_facets", &cgalpy::tri3::incident_facets5,
         py::keep_alive<0, 1>(), py::arg("e"), py::arg("start"), py::arg("f"),
         tri3_doc::Triangulation_3_incident_facets_3)
    .def("incident_facets", &cgalpy::tri3::incident_facets6,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"), py::arg("start"), py::arg("f"),
         tri3_doc::Triangulation_3_incident_facets_5)
    ;

  // Circulators
  export_circulator<Fc>(tri_c, "Facet_circulator");
  tri_c.def("incident_cells_circulator", &cgalpy::tri3::incident_cells_circulator1,
            py::keep_alive<0, 1>(), py::arg("e"),
            tri3_doc::Triangulation_3_incident_cells)
    .def("incident_cells_circulator", &cgalpy::tri3::incident_cells_circulator2,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_incident_cells_1)
    .def("incident_cells_circulator", &cgalpy::tri3::incident_cells_circulator3,
         py::keep_alive<0, 1>(), py::arg("e"), py::arg("start"),
         tri3_doc::Triangulation_3_incident_cells_2)
    .def("incident_cells_circulator", &cgalpy::tri3::incident_cells_circulator4,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"), py::arg("start"),
         tri3_doc::Triangulation_3_incident_cells_3)
    ;

  export_circulator<Cc>(tri_c, "Cell_circulator");
  tri_c.def("incident_facets_circulator", &cgalpy::tri3::incident_facets_circulator1,
            py::keep_alive<0, 1>(), py::arg("e"),
            tri3_doc::Triangulation_3_incident_facets)
    .def("incident_facets_circulator", &cgalpy::tri3::incident_facets_circulator2,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"),
         tri3_doc::Triangulation_3_incident_facets_1)
    .def("incident_facets_circulator", &cgalpy::tri3::incident_facets_circulator3,
         py::keep_alive<0, 1>(), py::arg("e"), py::arg("start"),
         tri3_doc::Triangulation_3_incident_facets_2)
    .def("incident_facets_circulator", &cgalpy::tri3::incident_facets_circulator4,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"), py::arg("start"),
         tri3_doc::Triangulation_3_incident_facets_4)
    .def("incident_facets_circulator", &cgalpy::tri3::incident_facets_circulator5,
         py::keep_alive<0, 1>(), py::arg("e"), py::arg("start"), py::arg("f"),
         tri3_doc::Triangulation_3_incident_facets_3)
    .def("incident_facets_circulator", &cgalpy::tri3::incident_facets_circulator6,
         py::keep_alive<0, 1>(), py::arg("c"), py::arg("i"), py::arg("j"), py::arg("start"), py::arg("f"),
         tri3_doc::Triangulation_3_incident_facets_5)
    ;

  // Container returning functions
  tri_c.def("adjacent_vertices", &cgalpy::tri3::adjacent_vertices,
            py::arg("v"),
            tri3_doc::Triangulation_3_adjacent_vertices)
    .def("finite_incident_cells", &cgalpy::tri3::finite_incident_cells,
         py::arg("v"),
         tri3_doc::Triangulation_3_finite_incident_cells)
    .def("finite_incident_edges", &cgalpy::tri3::finite_incident_edges,
         py::arg("v"),
         tri3_doc::Triangulation_3_finite_incident_edges)
    .def("finite_incident_facets", &cgalpy::tri3::finite_incident_facets,
         py::arg("v"),
         tri3_doc::Triangulation_3_finite_incident_facets)
    .def("finite_adjacent_vertices", &cgalpy::tri3::finite_adjacent_vertices,
         py::arg("v"),
         tri3_doc::Triangulation_3_finite_adjacent_vertices)
    .def("incident_edges", &cgalpy::tri3::incident_edges,
         py::arg("v"),
         tri3_doc::Triangulation_3_incident_edges)
    .def("vertices", &cgalpy::tri3::vertices1,
         py::arg("e"),
         tri3_doc::Triangulation_3_vertices)
    .def("vertices", &cgalpy::tri3::vertices2,
         py::arg("f"),
         tri3_doc::Triangulation_3_vertices_1)
    .def("vertices", &cgalpy::tri3::vertices3,
         py::arg("c"),
         tri3_doc::Triangulation_3_vertices_2)
    ;

#ifdef CGALPY_HAS_VISUAL
  m.def("draw",
        [](const Tri& tri, const char* title)
        { CGAL::draw(tri, title); },
        py::arg("tri"), py::arg("title"),
        "Draw the triangulation with a window title.");

#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
  m.def("draw",
        [](const Tri& tri, const cgalpy::bvr::Graphics_scene_options& gso, const char* title)
        { CGAL::draw(tri, gso, title); },
        py::arg("tri"), py::arg("graphics_scene_options"), py::arg("title"),
        "Draw the triangulation with graphics-scene options and a window title.");
#endif
#endif

  // Todo
  // Simplex;
  // Segment_simplex_iterator;
  // template<class PointWithInfoInputIterator >
  // std::ptrdiff_t insert (PointWithInfoInputIterator first, PointWithInfoInputIterator last)

  // template<typename InputIterator >
  // int remove (InputIterator first, InputIterator beyond)

  add_insertion(tri_c, "__str__");
  add_insertion(tri_c, "__repr__");
  add_extraction(tri_c);
}
