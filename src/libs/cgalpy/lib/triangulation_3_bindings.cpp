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
#include <nanobind/stl/array.h>
#include <nanobind/stl/tuple.h>

#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_triangulation_3.h>
#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
#include "CGALPY/basic_viewer_types.hpp"
#endif
#endif

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/export_circulator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/make_circulator.hpp"
#include "CGALPY/stl_dereference_input_iterator.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/triangulation_3_types.hpp"
#include "CGALPY/types.hpp"

void export_tri3_vertex(py::class_<tri3::Triangulation_3>&);
void export_tri3_cell(py::class_<tri3::Triangulation_3>&);

namespace py = nanobind;

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
  auto begin = stl_input_iterator<tri3::Point>(lst);
  auto end = stl_input_iterator<tri3::Point>(lst, false);
  new (tri) tri3::Triangulation_3(begin, end);  // placement new
}

//!
auto insert_points(Triangulation_3& tri, py::list& lst) {
  if (! lst) return 0l;
  if (! py::isinstance<tri3::Point>(lst[0])) return 0l;
  auto begin = stl_input_iterator<tri3::Point>(lst);
  auto end = stl_input_iterator<tri3::Point>(lst, false);
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
Vertex& insert4(Triangulation_3& tri, const Point& p,
                Locate_type lt, tri3::Cell& lc, int li, int lj) {
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
#if 1
  auto begin = stl_dereference_input_iterator<Cell_handle, Cell>(cells);
  auto end = stl_dereference_input_iterator<Cell_handle, Cell>(cells, false);
  auto vh = tri.insert_in_hole(p, begin, end, Cell_handle(&start), i);
#else
  std::vector<Cell_handle> chs(cells.size());
  for (auto i = 0; i < cells.size(); ++i)
    chs[i] = Cell_handle(py::cast<Cell*>(cells[i]));
  auto vh = tri.insert_in_hole(p, chs.begin(), chs.end(), Cell_handle(&     53 start), i);
#endif
  return *vh;
}

//!
Vertex& insert_in_hole2(Triangulation_3& tri, const Point& p, py::list& cells, Cell& start, int i, Vertex& newv) {
  auto begin = stl_dereference_input_iterator<Cell_handle>(cells);
  auto end = stl_dereference_input_iterator<Cell_handle>(cells, false);
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
  return tri.is_infinite(Cell_handle(&c), verbose);
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
py::object locate_face(py::handle self, const Point& query) {
  constexpr auto ri(py::rv_policy::reference_internal);

  auto& tri = py::cast<Triangulation_3&>(self);
  Locate_type lt;
  int li, lj;
  auto ch = tri.locate(query, lt, li, lj);
  Cell& c = *ch;
  switch (lt) {
   case Triangulation_3::FACET:
   case Triangulation_3::VERTEX:
    return py::make_tuple(py::cast(lt), py::cast(c, ri, self), py::int_(li));

   case Triangulation_3::EDGE:
    return py::make_tuple(py::cast(lt), py::cast(c, ri, self), py::int_(li),
                          py::int_(lj));

   case Triangulation_3::CELL:
   case Triangulation_3::OUTSIDE_CONVEX_HULL:
    return py::make_tuple(py::cast(lt), py::cast(c, ri, self));
  }

  throw std::runtime_error("Invalid location type");
  return py::none();
}

//
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
  return make_iterator_from_circulator(tri.incident_cells(Cell_handle(&c), i, j,
                                                          Cell_handle(&start)));
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
  return make_iterator_from_circulator(tri.incident_facets(Cell_handle(&c), i, j,
                                                           Cell_handle(&start), f));
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

} // End of namespace tri3

//!
void export_triangulation_3(py::module_& m) {
  using Tri = tri3::Triangulation_3;
  using Edge = tri3::Edge;
  using Cell = tri3::Cell;
  using Face = tri3::Facet;
  using Facet = tri3::Facet;
  using Pnt = tri3::Point;
  using Vertex = tri3::Vertex;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Tri>(m, "Triangulation_3")) return;

  py::class_<Tri> tri_c(m, "Triangulation_3");

  // Locate type
  py::enum_<tri3::Locate_type>(tri_c, "Locate_type")
    .value("VERTEX", Tri::VERTEX)
    .value("EDGE", Tri::EDGE)
    .value("FACET", Tri::FACET)
    .value("CELL", Tri::CELL)
    .value("OUTSIDE_CONVEX_HULL", Tri::OUTSIDE_CONVEX_HULL)
    .value("OUTSIDE_AFFINE_HULL", Tri::OUTSIDE_AFFINE_HULL)
    .export_values()
    ;

  // Types that have been registered already:
  if constexpr(tri3::is_periodic())
    // \todo: generate bindings for periodic traits
  ;
  else {
    // In the case of non-priodic triangulation the kernel serves as the traits.
    add_attr<tri3::Geom_traits>(tri_c, "Geom_traits");
  }

  if (! add_attr<Pnt>(tri_c, "Point")) std::cerr << "'Point' not registered!\n";
  add_attr<tri3::Segment>(tri_c, "Segment");
  add_attr<tri3::Triangle>(tri_c, "Triangle");
  add_attr<tri3::Tetrahedron>(tri_c, "Tetrahedron");

  export_tri3_vertex(tri_c);
  export_tri3_cell(tri_c);

  py::class_<Facet>(tri_c, "Facet")
    .def("cell", [](Facet& f)->tri3::Cell& { return *(f.first); } , ri)
    .def_rw("index", &Facet::second)
    ;

  py::class_<Edge>(tri_c, "Edge")
    .def("cell", [](Edge& e)->tri3::Cell& { return *(e.first); } , ri)
    .def_rw("start_index", &Edge::second)
    .def_rw("end_index", &Edge::third)
    ;

  tri_c.def(py::init<>())
    .def(py::init<const tri3::Traits&>())
    .def("__init__", &tri3::tri3_init)
    .def("are_equal",
         py::overload_cast<const Facet&, const Facet&>(&Tri::are_equal, py::const_),
         py::arg("f1"), py::arg("f2"),
         "Determine whether two facets have the same vertices\n"
         "Parameters:\n"
         "  f1: The first facet\n"
         "  f2: The second facet\n")
    .def("are_equal", tri3::are_equal1,
         py::arg("c1"), py::arg("i1"), py::arg("c2"), py::arg("i2"),
         "Determine whether two facets have the same vertices\n"
         "Parameters:\n"
         "  c1: Together with i1 represent the first facet\n"
         "  i1 (int)\n"
         "  c2: Together with i2 Represent the second facet\n"
         "  i2 (int)\n")
    .def("are_equal", tri3::are_equal2,
         py::arg("f"), py::arg("c"), py::arg("i"),
         "Determine whether two facets have the same vertices\n"
         "Parameters:\n"
         "  f: The first facet\n"
         "  c: Together with i represent the second facet\n"
         "  i (int)\n")
    .def("clear", &Tri::clear, "deletes all finite vertices and all cells")
    .def("degree", &tri3::degree,
         py::arg("v"),
         "Obtain the degree of a vertex, that is, the number of incident vertices\n"
         "The infinite vertex is counted"
         "Parameters:\n"
         " v: the vertex\n")
    .def("dimension", &Tri::dimension, "Obtain the dimension of the affine hull\n")

    // Flip
    .def("flip", py::overload_cast<const Edge&>(&Tri::flip),
         py::arg("e"),
         "Check whether a given edge is flipapble; if so flips it"
         "Parameters:\n"
         "e: the edge\n")
    .def("flip", py::overload_cast<const Facet&>(&Tri::flip),
         py::arg("f"),
         "Check whether a given facet is flipapble; if so flip it\n"
         "Parameters:\n"
         "f: the facet\n")
    .def("flip", &tri3::flip1,
         py::arg("c"), py::arg("i"),
         "Cehck whether a represented facet is flipapble; if so flip it\n"
         "Parameters:\n"
         "  c: Together with i represent the facet\n"
         "  i (int)\n")
    .def("flip", &tri3::flip2,
         py::arg("c"), py::arg("i"), py::arg("j"),
         "Cehck whether a represented edge is flipapble; if so flip it\n"
         "Parameters:\n"
         "  c: Together with i and j represent the edge\n"
         "  i (int)\n"
         "  j (int)\n")
    .def("flip_flippable", py::overload_cast<const Edge&>(&Tri::flip_flippable),
         py::arg("e"),
         "Flip a flippable edge\n"
         "Parameters:\n"
         "e: the edge\n")
    .def("flip_flippable", py::overload_cast<const Facet&>(&Tri::flip_flippable),
         py::arg("f"),
         "Flip a flippable facet\n"
         "Parameters:\n"
         "f: the facet\n")
    .def("flip_flippable", &tri3::flip_flippable1,
         py::arg("c"), py::arg("i"),
         "Flip a flippable facet\n"
         "Parameters:\n"
         "  c: Together with i represent the facet\n"
         "  i (int)\n")
    .def("flip_flippable", &tri3::flip_flippable2,
         py::arg("c"), py::arg("i"), py::arg("j"),
         "Flip a flippable edge\n"
         "Parameters:\n"
         "  c: Together with i and j represent the edge\n"
         "  i (int)\n"
         "  j (int)\n")

    // Geometry traits
    .def("geom_traits", &Tri::geom_traits, ri, "Obtain the geometric traits object")

    // Has vertex
    .def("has_vertex", &tri3::has_vertex1,
         py::arg("f"), py::arg("v"),
         "Determine whether a facet has a vertex\n"
         "Parameters:\n"
         "  f: the facet\n"
         "  v: the vertex\n")
    .def("has_vertex", &tri3::has_vertex2,
         py::arg("c"), py::arg("i"), py::arg("v"),
         "Determine whether a represented facet has a vertex\n"
         "Parameters:\n"
         "  c: Together with i represent the facet\n"
         "  i (int)\n"
         "  v: the vertex\n")
    .def("has_vertex_get_index", &tri3::has_vertex3,
         py::arg("f"), py::arg("v"),
         "Determine whether a facet has a vertex\n"
         "Parameters:\n"
         "  f: the facet\n"
         "  v: the vertex\n"
         "Return:\n"
         "  tuple[bool, int]: If the facet does not have the vertex, return False;\n"
         "  otherwise, return tuple (True, i), where i is the vertex index\n")
    .def("has_vertex_get_index", &tri3::has_vertex4,
         "Determine whether a represented facet has a vertex\n"
         "Parameters:\n"
         "  c: Together with i represent the facet\n"
         "  i (int)\n"
         "  v: the vertex\n"
         "Return:\n"
         "  tuple[bool, int]: If the facet does not have the vertex, return False;\n"
         "  otherwise, return tuple (True, i), where i is the vertex index\n")

    // Insertion
    .def("insert", &tri3::insert1, ri,
         py::arg("p"),
         "Insert a point\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri3::insert2, ri,
         py::arg("p"), py::arg("start"),
         "Insert a point\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  start (Cell): Start the search at this cell\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri3::insert3, ri,
         py::arg("p"), py::arg("start"),
         "Insert a point\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  start (Vertex): Start the search at this vertex\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri3::insert4, ri,
         py::arg("p"), py::arg("lt"), py::arg("lc"), py::arg("li"), py::arg("lj"),
         "Insert a point using the return values of a previous location query\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  lt (Locate_type): together with lc, li, and lj the return values of a previous location query\n"
         "  lc (Cell)\n"
         "  li (int)\n"
         "  lj (int)\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert", &tri3::insert_points,
         py::arg("points"),
         "Insert a list of points\n"
         "Parameters:\n"
         "  points (list) the list of points\n"
         "Return:\n"
         "  The number of inserted points\n")
    .def("insert_in_cell", &tri3::insert_in_cell, ri,
         py::arg("p"), py::arg("c"),
         "Insert a point in a given cell\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  c: The cell\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert_in_edge", &tri3::insert_in_edge1, ri,
         py::arg("p"), py::arg("e"),
         "Insert a point in a given edge\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  e: The edge\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert_in_edge", &tri3::insert_in_edge2, ri,
         py::arg("p"), py::arg("c"), py::arg("i"), py::arg("j"),
         "Insert a point in a represented edge\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  c (Cell): Together with i and j represent the edge\n"
         "  i (int)\n"
         "  j (int)\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert_in_facet", &tri3::insert_in_facet, ri,
         py::arg("p"), py::arg("f"),
         "Insert a point in a given facet\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  f: The facet\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert_in_hole", &tri3::insert_in_hole1, ri,
         py::arg("p"), py::arg("cells"), py::arg("start"), py::arg("i"),
         "Insert a point in a hole defined by a list of cells\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  cells (list): The list of cells\n"
         "  start (Cell): Together with i represent a facet on the boundary of the hole\n"
         "  i (int)\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert_in_hole", &tri3::insert_in_hole2, ri,
         py::arg("p"), py::arg("cells"), py::arg("start"), py::arg("i"), py::arg("newv"),
         "Insert a vertex in a hole defined by a list of cells\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  cells (list): The list of cells\n"
         "  start (Cell): Together with i represent a facet on the boundary of the hole\n"
         "  i (int)\n"
         "  newv (Vertex): \n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert_outside_affine_hull", &tri3::insert_outside_affine_hull, ri,
         "Insert a point (can be used to insert the first point in an empty triangulation)\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "Return:\n"
         "  The corresponding vertex\n")
    .def("insert_outside_convex_hull", &tri3::insert_outside_convex_hull, ri,
         "Insert a point in an infinite cell\n"
         "Parameters:\n"
         "  p (Point_3): The point\n"
         "  c (Cell): the infinite cell\n"
         "Return:\n"
         "  The corresponding vertex\n")

    // Queries
    .def("is_cell", &tri3::is_cell1,
         py::arg("c"),
         "Determine whether a given cell belongs to the triangulation\n"
         "Parameters:\n"
         "  c (Cell): The cell\n"
         "Return:\n"
         "  bool")
    .def("is_cell", &tri3::is_cell2,
         py::arg("u"), py::arg("v"), py::arg("w"), py::arg("x"),
         "Determine whether a represented cell belongs to the triangulation\n"
         "Parameters:\n"
         "  u (Vertex): together with v, w, and x represent the cell\n"
         "  v (vertex)\n"
         "  w (vertex)\n"
         "  x (Vertex)\n"
         "Return:\n"
         "  bool\n")
    .def("is_cell_get_cell", &tri3::is_cell3,
         py::arg("u"), py::arg("v"), py::arg("w"), py::arg("x"),
         "Determine whether a represented cell belongs to the triangulation\n"
         "Parameters:\n"
         "  u (Vertex): together with v, w, and x represent the cell\n"
         "  v (vertex)\n"
         "  w (vertex)\n"
         "  x (Vertex)\n"
         "Return:\n"
         "  tuple[bool, Cell, int, int, int, int]: If the cell does not belong to the triangulation, return False;\n"
         "  otherwise, return tuple (True, c, i, j, k, l), where c is the cell and i, j, k, and l are the indices of the vertices u, v, w, and x, respectively, in c\n")
    .def("is_edge", &tri3::is_edge,
         py::arg("u"), py::arg("v"),
         "Determine whether a represented edge belongs to the triangulation\n"
         "Parameters:\n"
         "  u (Vertex): together with v represent the edge\n"
         "Return:\n"
         "  tuple[bool, Cell, int, int]: If the edgedoes not belong to the triangulation, return False;\n"
         "  otherwise, return tuple (True, c, i, j), where c is the containing cell and i and, j are the indices of the vertices u and v, respectively, in c\n")
    .def("is_facet", &tri3::is_facet,
         py::arg("u"), py::arg("v"), py::arg("w"),
         "Determine whether a represented facet belongs to the triangulation\n"
         "Parameters:\n"
         "  u (Vertex): together with v and w represent the facet\n"
         "  v (vertex)\n"
         "  w (vertex)\n"
         "Return:\n"
         "  tuple[bool, Cell, int, int, int]: If the facet does not belong to the triangulation, return False;\n"
         "  otherwise, return tuple (True, c, i, j, k), where c is the containing cell and i, j, and k are the indices of the vertices u, v, and w, respectively, in c\n")
    .def("is_infinite", &tri3::is_infinite1)
    .def("is_infinite", &tri3::is_infinite2)
    .def("is_infinite", &tri3::is_infinite3)
    .def("is_infinite",
         py::overload_cast<const Edge&>(&Tri::is_infinite, py::const_))
    .def("is_infinite",
         py::overload_cast<const Facet&>(&Tri::is_infinite, py::const_))
    .def("is_infinite", &tri3::is_infinite6)
    .def("is_valid",
         py::overload_cast<bool, int>(&Tri::is_valid, py::const_),
         py::arg("verbose") = false, py::arg("level") = 0)
    .def("is_valid", &tri3::is_valid2, py::arg("c"), py::arg("verbose") = false)
    .def("is_vertex", &tri3::is_vertex1)
    .def("is_vertex", &tri3::is_vertex2)

    // Locators
    .def("locate", &tri3::locate1, ri)
    .def("locate", &tri3::locate2, ri)
    .def("locate", &tri3::locate3, ri)
    .def("locate_face", &tri3::locate_face, ri)

    // Quantifiers
    .def("number_of_cells", &Tri::number_of_cells)
    .def("number_of_edges", &Tri::number_of_edges)
    .def("number_of_facets", &Tri::number_of_facets)
    .def("number_of_finite_cells", &Tri::number_of_finite_cells)
    .def("number_of_finite_edges", &Tri::number_of_finite_edges)
    .def("number_of_finite_facets", &Tri::number_of_finite_facets)
    .def("number_of_vertices", &Tri::number_of_vertices)
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

  tri_c.def("all_cells", &tri3::all_cells, py::keep_alive<0, 1>(),
            "Obtain an iterator that traverses all cells")
    .def("all_edges", &tri3::all_edges, py::keep_alive<0, 1>(),
         "Obtain an iterator that traverses all edges")
    .def("all_facets", &tri3::all_facets, py::keep_alive<0, 1>(),
         "Obtain an iterator that traverses all facets")
    .def("all_vertices", &tri3::all_vertices, py::keep_alive<0, 1>(),
         "Obtain an iterator that traverses all vertices")

    .def("finite_cells", &tri3::finite_cells, py::keep_alive<0, 1>(),
         "Obtain an iterator that traverses all finite cells")
    .def("finite_edges", &tri3::finite_edges, py::keep_alive<0, 1>(),
         "Obtain an iterator that traverses all finite edges")
    .def("finite_facets", &tri3::finite_facets, py::keep_alive<0, 1>(),
         "Obtain an iterator that traverses all finite facets")
    .def("finite_vertices", &tri3::finite_vertices, py::keep_alive<0, 1>(),
         "Obtain an iterator that traverses all finite vertices")

    .def("points", &tri3::points, py::keep_alive<0, 1>())
    .def("segment_traverser_cells", &tri3::segment_traverser_cells1,
         py::keep_alive<0, 1>())
    .def("segment_traverser_cells", &tri3::segment_traverser_cells2,
         py::keep_alive<0, 1>())
    ;

  // Iterators
  using Cc = Tri::Cell_circulator;
  using Fc = Tri::Facet_circulator;

  add_iterator_from_circulator<Cc>("Cell_iterator", tri_c);
  tri_c.def("incident_cells", &tri3::incident_cells1, py::keep_alive<0, 1>(),
            "Traverse all cells or all facets incident to a given edge")
    .def("incident_cells", &tri3::incident_cells2, py::keep_alive<0, 1>(),
         "Traverse all cells or all facets incident to a given edge")
    .def("incident_cells", &tri3::incident_cells3, py::keep_alive<0, 1>(),
         "Traverse all cells or all facets incident to a given edge starting at a given cell")
    .def("incident_cells", &tri3::incident_cells4, py::keep_alive<0, 1>(),
         "Traverse all cells or all facets incident to a given edge starting at a given cell")
    ;

  add_iterator_from_circulator<Fc>("Facet_iterator", tri_c);
  tri_c.def("incident_facets", &tri3::incident_facets1, py::keep_alive<0, 1>(),
            "Traverse all facets incident to a given edge")
    .def("incident_facets", &tri3::incident_facets2, py::keep_alive<0, 1>(),
            "Traverse all facets incident to a given edge")
    .def("incident_facets", &tri3::incident_facets3, py::keep_alive<0, 1>(),
            "Traverse all facets incident to a given edge starting at a given facet")
    .def("incident_facets", &tri3::incident_facets4, py::keep_alive<0, 1>(),
            "Traverse all facets incident to a given edge starting at a given facet")
    .def("incident_facets", &tri3::incident_facets5, py::keep_alive<0, 1>(),
            "Traverse all facets incident to a given edge starting at a given facet")
    .def("incident_facets", &tri3::incident_facets6, py::keep_alive<0, 1>(),
            "Traverse all facets incident to a given edge starting at a given facet")
    ;

  // Circulators
  export_circulator<Fc>(tri_c, "Facet_circulator");
  tri_c.def("incident_cells_circulator", &tri3::incident_cells_circulator1, py::keep_alive<0, 1>(),
            "Circulate through all cells or all facets incident to a given edge")
    .def("incident_cells_circulator", &tri3::incident_cells_circulator2, py::keep_alive<0, 1>(),
            "Circulate through all cells or all facets incident to a given edge")
    .def("incident_cells_circulator", &tri3::incident_cells_circulator3, py::keep_alive<0, 1>(),
            "Circulate through all cells or all facets incident to a given edge starting at a given cell")
    .def("incident_cells_circulator", &tri3::incident_cells_circulator4, py::keep_alive<0, 1>(),
            "Circulate through all cells or all facets incident to a given edge starting at a given cell")
    ;

  export_circulator<Cc>(tri_c, "Cell_circulator");
  tri_c.def("incident_facets_circulator", &tri3::incident_facets_circulator1, py::keep_alive<0, 1>(),
            "Circulate through all facets incident to a given edge")
    .def("incident_facets_circulator", &tri3::incident_facets_circulator2, py::keep_alive<0, 1>(),
            "Circulate through all facets incident to a given edge")
    .def("incident_facets_circulator", &tri3::incident_facets_circulator3, py::keep_alive<0, 1>(),
            "Circulate through all facets incident to a given edge starting at a given facet")
    .def("incident_facets_circulator", &tri3::incident_facets_circulator4, py::keep_alive<0, 1>(),
            "Circulate through all facets incident to a given edge starting at a given facet")
    .def("incident_facets_circulator", &tri3::incident_facets_circulator5, py::keep_alive<0, 1>(),
            "Circulate through all facets incident to a given edge starting at a given facet")
    .def("incident_facets_circulator", &tri3::incident_facets_circulator6, py::keep_alive<0, 1>(),
            "Circulate through all facets incident to a given edge starting at a given facet")
    ;

  // Container returning functions
  tri_c.def("adjacent_vertices", &tri3::adjacent_vertices,
            "Obtain the vertices adjacent to given vertex")
    .def("finite_incident_cells", &tri3::finite_incident_cells,
         "Obtain the finite cells incident to given vertex")
    .def("finite_incident_edges", &tri3::finite_incident_edges,
         "Obtain the finite edges incident to given vertex")
    .def("finite_incident_facets", &tri3::finite_incident_facets,
         "Obtain the finite facets incident to given vertex")
    .def("finite_adjacent_vertices", &tri3::finite_adjacent_vertices,
         "Obtain the finite vertices adjacent to given vertex")
    .def("incident_edges", &tri3::incident_edges,
         "Obtain the edges incident to given vertex")
    .def("vertices", &tri3::vertices1)
    .def("vertices", &tri3::vertices2)
    .def("vertices", &tri3::vertices3)
    ;

#ifdef CGALPY_HAS_VISUAL
  m.def("draw",
        [](const Tri& tri, const char* title)
        { CGAL::draw(tri, title); });

#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
  m.def("draw",
        [](const Tri& tri, const bvr::Graphics_scene_options& gso,
           const char* title)
        { CGAL::draw(tri, gso, title); });
#endif
#endif

  // Todo
  // Simplex;

  // Facet_iterator;
  // Cell_iterator;
  // Segment_simplex_iterator;

  // template<class PointWithInfoInputIterator >
  // std::ptrdiff_t insert (PointWithInfoInputIterator first, PointWithInfoInputIterator last)

  // template<typename InputIterator >
  // int remove (InputIterator first, InputIterator beyond)

  // template<typename InputIterator >
  // int remove_cluster (InputIterator first, InputIterator beyond)

  add_insertion(tri_c, "__str__");
  add_insertion(tri_c, "__repr__");
  add_extraction(tri_c);
}
