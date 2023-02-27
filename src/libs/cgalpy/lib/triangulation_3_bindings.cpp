// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_3_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

namespace tri3 {

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY

//
void dt3_init(tri3::Triangulation_3* tri, py::list& lst) {
  auto begin = stl_input_iterator<tri3::Point>(lst);
  auto end = stl_input_iterator<tri3::Point>(lst, false);
  new (tri) tri3::Triangulation_3(begin, end);  // placement new
}

//
std::ptrdiff_t insert_points(tri3::Triangulation_3& dt, py::list& lst) {
  if (! lst) return 0;
  if (! py::isinstance<tri3::Point>(lst[0])) return 0;
  auto begin = stl_input_iterator<tri3::Point>(lst);
  auto end = stl_input_iterator<tri3::Point>(lst, false);
  return dt.insert(begin, end);
}

#if (CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT)

//
tri3::Vertex_handle insert4(tri3::Triangulation_3& dt,
                            const tri3::Point& p, tri3::Cell_handle start)
{ return dt.insert(p, start); }

//
tri3::Vertex_handle insert5(tri3::Triangulation_3& dt,
                            const tri3::Point& p, tri3::Vertex_handle hint)
{ return dt.insert(p, hint); }

//
tri3::Vertex_handle insert6(tri3::Triangulation_3& dt,
                            const tri3::Point& p, tri3::Locate_type lt,
                            tri3::Cell_handle c, int li, int lj)
{ return dt.insert(p, lt, c, li, lj); }

#endif

//
template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

//
bool vertex_is_valid1(const tri3::Vertex& vertex, bool verbose, int level)
{ return vertex.is_valid(verbose, level); }

//
bool vertex_is_valid2(const tri3::Vertex& vertex, bool verbose)
{ return vertex.is_valid(verbose); }

//
bool vertex_is_valid3(const tri3::Vertex& vertex) { return vertex.is_valid(); }

//
bool cell_is_valid1(const tri3::Cell& cell, bool verbose, int level)
{ return cell.is_valid(verbose, level); }

//
bool cell_is_valid2(const tri3::Cell& cell, bool verbose)
{ return cell.is_valid(verbose); }

//
bool cell_is_valid3(const tri3::Cell& cell) { return cell.is_valid(); }

//
py::object all_vertices(const Triangulation_3& tri)
{ return make_iterator(tri.all_vertices_begin(), tri.all_vertices_end()); }

//
py::object all_edges(const Triangulation_3& tri)
{ return make_iterator(tri.all_edges_begin(), tri.all_edges_end()); }

//
py::object all_cells(const Triangulation_3& tri)
{ return make_iterator(tri.all_cells_begin(), tri.all_cells_end()); }

//
py::object all_facets(const Triangulation_3& tri)
{ return make_iterator(tri.all_facets_begin(), tri.all_facets_end()); }

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

} // End of namespace tri3

#endif

//
void export_triangulation_3(py::module_& m) {
  using Tri = tri3::Triangulation_3;

  constexpr auto ri(py::rv_policy::reference_internal);

  CGAL::Bounded_side(Tri::*side_of_sphere)(tri3::Cell_handle, const tri3::Point&, bool) const =
    &Tri::side_of_sphere;
  CGAL::Bounded_side(Tri::*side_of_circle1)(const tri3::Facet&, const tri3::Point&, bool) const =
    &Tri::side_of_circle;
  CGAL::Bounded_side(Tri::*side_of_circle2)(tri3::Cell_handle, int, const tri3::Point& p, bool) const =
    &Tri::side_of_circle;

  bool(Tri::*is_valid1)(bool, int) const =
    &Tri::is_valid;
  // bool(Tri::*is_valid2)(tri3::Cell_handle, bool, int) const =
  //   &Tri::is_valid;

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
  tri3::Vertex_handle(Tri::*insert1)(const tri3::Point&, tri3::Cell_handle, bool*) =
    &Tri::insert;
  tri3::Vertex_handle(Tri::*insert2)(const tri3::Point&, tri3::Vertex_handle, bool*) =
    &Tri::insert;
  tri3::Vertex_handle(Tri::*insert3)(const tri3::Point&, tri3::Locate_type, tri3::Cell_handle, int, int, bool*) =
    &Tri::insert;
#else
  tri3::Vertex_handle(Tri::*insert1)(const tri3::Point&, tri3::Cell_handle) =
    &Tri::insert;
  tri3::Vertex_handle(Tri::*insert2)(const tri3::Point&, tri3::Vertex_handle) =
    &Tri::insert;
  tri3::Vertex_handle(Tri::*insert3)(const tri3::Point&, tri3::Locate_type, tri3::Cell_handle, int, int) =
    &Tri::insert;
#endif

  tri3::Vertex_handle(Tri::*nearest_vertex)(const tri3::Point&, tri3::Cell_handle) const =
    &Tri::nearest_vertex;

  py::class_<Tri> tri_c(m, "Triangulation_3");
  tri_c.def(py::init<>())
    .def(py::init<const tri3::Traits&>())
#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
    .def("__init__", &tri3::dt3_init)
#endif
    // Insertion
    .def("insert", insert1)
    .def("insert", insert2)
    .def("insert", insert3)
#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
    .def("insert", &tri3::insert4)
    .def("insert", &tri3::insert5)
    .def("insert", &tri3::insert6)
#endif
    .def("insert_points", &tri3::insert_points)

    // template<class PointWithInfoInputIterator >
    // std::ptrdiff_t insert (PointWithInfoInputIterator first, PointWithInfoInputIterator last)

    // Displacement
    .def("move_if_no_collision", &Tri::move_if_no_collision)
    .def("move", &Tri::move)

    // Removal
    .def("remove", static_cast<void(Tri::*)(tri3::Vertex_handle)>(&Tri::remove))
    // .def("remove", py::overload_cast<tri3::Vertex_handle, bool*>(&Tri::remove))

    // template<typename InputIterator >
    // int remove (InputIterator first, InputIterator beyond)

    // template<typename InputIterator >
    // int remove_cluster (InputIterator first, InputIterator beyond)

    // Queries
    .def("side_of_sphere", side_of_sphere)
    .def("side_of_circle", side_of_circle1)
    .def("side_of_circle", side_of_circle2)
    .def("nearest_vertex", nearest_vertex)
    .def("nearest_vertex_in_cell", &Tri::nearest_vertex_in_cell)
    .def("is_valid", is_valid1)
    // .def("is_valid", is_valid2)
    ;

  // Triangulation_data_structure
  // Lock_data_structure
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
#if ((CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_PLAIN) ||         \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_REGULAR) ||       \
     (CGALPY_TRI3 == CGALPY_TRI3_PERIODIC_DELAUNAY))
  // \todo: generate bindings for periodic traits
  ;
#else
  // In the case of non-priodic triangulation the kernel serves as the traits.
  add_attr<tri3::Geom_traits>(tri_c, "Geom_traits");
#endif

  if (! add_attr<tri3::Point>(tri_c, "Point"))
    std::cerr << "'tri3::Point' not registered!\n";
  add_attr<tri3::Segment>(tri_c, "Segment");
  add_attr<tri3::Triangle>(tri_c, "Triangle");
  add_attr<tri3::Tetrahedron>(tri_c, "Tetrahedron");

  py::class_<tri3::Vertex>(tri_c, "Vertex")
    .def(py::init<>())
    // Access Functions
    .def("cell", py::overload_cast<>(&tri3::Vertex::cell, py::const_))
    .def("point", py::overload_cast<>(&tri3::Vertex::point, py::const_), ri)
    // Setting
    .def("set_cell", &tri3::Vertex::set_cell)
    .def("set_point", &tri3::Vertex::set_point)
    // Checking
    .def("is_valid", &tri3::vertex_is_valid1)
    .def("is_valid", &tri3::vertex_is_valid2)
    .def("is_valid", &tri3::vertex_is_valid3)
    ;

  void(tri3::Cell::*set_vertices)(tri3::Vertex_handle, tri3::Vertex_handle,
                                  tri3::Vertex_handle, tri3::Vertex_handle) =
    &tri3::Cell::set_vertices;
  void(tri3::Cell::*set_neighbors)(tri3::Cell_handle, tri3::Cell_handle,
                                   tri3::Cell_handle, tri3::Cell_handle) =
    &tri3::Cell::set_neighbors;

  py::class_<tri3::Cell>(tri_c, "Cell")
    .def(py::init<>())
    // Access Functions
    .def("vertex", &tri3::Cell::vertex)
    .def("index", py::overload_cast<tri3::Vertex_handle>(&tri3::Cell::index, py::const_))
    .def("index", py::overload_cast<tri3::Cell_handle>(&tri3::Cell::index, py::const_))
    .def("has_vertex", py::overload_cast<tri3::Vertex_handle>(&tri3::Cell::has_vertex, py::const_))
    .def("has_vertex", py::overload_cast<tri3::Vertex_handle, int&>(&tri3::Cell::has_vertex, py::const_))
    .def("neighbor", &tri3::Cell::neighbor)
    .def("has_neighbor", py::overload_cast<tri3::Cell_handle>(&tri3::Cell::has_neighbor, py::const_))
    .def("has_neighbor", py::overload_cast<tri3::Cell_handle, int&>(&tri3::Cell::has_neighbor, py::const_))
    // Setting
    .def("set_vertex", &tri3::Cell::set_vertex)
    .def("set_vertices", set_vertices)
    .def("set_neighbor", &tri3::Cell::set_neighbor)
    .def("set_neighbors", set_neighbors)
    //  Checking
    .def("is_valid", &tri3::cell_is_valid1)
    .def("is_valid", &tri3::cell_is_valid2)
    .def("is_valid", &tri3::cell_is_valid3)
    ;

  py::class_<tri3::Facet>(tri_c, "Facet")
    .def_readwrite("first", &tri3::Facet::first)
    .def_readwrite("second", &tri3::Facet::second)
    ;

  py::class_<tri3::Edge>(tri_c, "Edge")
    .def_readwrite("first", &tri3::Edge::first)
    .def_readwrite("second", &tri3::Edge::second)
    .def_readwrite("third", &tri3::Edge::third)
    ;

  py::class_<tri3::Vertex_handle>(tri_c, "Vertex_handle")
    .def(py::init<>())
    .def("value", &tri3::value<tri3::Vertex_handle>, ri)
    ;

  py::class_<tri3::Cell_handle>(tri_c, "Cell_handle")
    .def(py::init<>())
    .def("value", &tri3::value<tri3::Cell_handle>, ri)
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

  using Vertex = Tri::Vertex;
  using Edge = Tri::Vertex;
  using Cell = Tri::Cell;
  using Face = Tri::Facet;

  using Point = Tri::Point;

  // Iterators
  add_iterator<Avi, Avi, const Vertex&>("All_vertices_iterator", tri_c);
  add_iterator<Aei, Aei>("All_edges_iterator", tri_c);
  add_iterator<Aci, Aci, const Cell&>("All_cells_iterator", tri_c);
  add_iterator<Afi, Afi, const Face&>("All_facets_iterator", tri_c);

  add_iterator<Fvi, Fvi, const Vertex&>("Finite_vertices_iterator", tri_c);
  add_iterator<Fei, Fei>("Finite_edges_iterator", tri_c);
  add_iterator<Fci, Fci, const Cell&>("Finite_cells_iterator", tri_c);
  add_iterator<Ffi, Ffi, const Face&>("Finite_facets_iterator", tri_c);

  add_iterator<Pi, Pi, const Point&>("Point_iterator", tri_c);

  tri_c.def("all_vertices", &tri3::all_vertices, py::keep_alive<0, 1>())
    .def("all_edges", &tri3::all_edges, py::keep_alive<0, 1>())
    .def("all_cells", &tri3::all_cells, py::keep_alive<0, 1>())
    .def("all_facets", &tri3::all_facets, py::keep_alive<0, 1>())
    .def("finite_vertices", &tri3::finite_vertices, py::keep_alive<0, 1>())
    .def("finite_edges", &tri3::finite_edges, py::keep_alive<0, 1>())
    .def("finite_cells", &tri3::finite_cells, py::keep_alive<0, 1>())
    .def("finite_facets", &tri3::finite_facets, py::keep_alive<0, 1>())
    .def("points", &tri3::points, py::keep_alive<0, 1>())
    ;

  // Todo
  // Simplex;

  // Facet_iterator;
  // Cell_iterator;
  // Segment_cell_iterator;
  // Segment_simplex_iterator;
}
