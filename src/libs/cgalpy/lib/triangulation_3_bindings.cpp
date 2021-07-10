// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/triangulation_3_types.hpp"

namespace bp = boost::python;

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY

tri3::Triangulation_3* dt3_init(bp::list& lst) {
  auto begin = bp::stl_input_iterator<tri3::Point>(lst);
  auto end = bp::stl_input_iterator<tri3::Point>();
  return new tri3::Triangulation_3(begin, end);
}

std::ptrdiff_t insert_points(tri3::Triangulation_3& dt, bp::list& lst) {
  if (! lst) return 0;
  if (! bp::extract<tri3::Point>(lst[0]).check()) return 0;
  auto begin = bp::stl_input_iterator<tri3::Point>(lst);
  auto end = bp::stl_input_iterator<tri3::Point>();
  return dt.insert(begin, end);
}

#if (CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT)

tri3::Vertex_handle insert4(tri3::Triangulation_3& dt,
                            const tri3::Point& p, tri3::Cell_handle start)
{ return dt.insert(p, start); }

tri3::Vertex_handle insert5(tri3::Triangulation_3& dt,
                            const tri3::Point& p, tri3::Vertex_handle hint)
{ return dt.insert(p, hint); }

tri3::Vertex_handle insert6(tri3::Triangulation_3& dt,
                            const tri3::Point& p, tri3::Locate_type lt,
                            tri3::Cell_handle c, int li, int lj)
{ return dt.insert(p, lt, c, li, lj); }

#endif

void export_delaunay_triangulation_3() {
  CGAL::Bounded_side(tri3::Triangulation_3::*side_of_sphere)(tri3::Cell_handle, const tri3::Point&, bool) const =
    &tri3::Triangulation_3::side_of_sphere;
  CGAL::Bounded_side(tri3::Triangulation_3::*side_of_circle1)(const tri3::Facet&, const tri3::Point&, bool) const =
    &tri3::Triangulation_3::side_of_circle;
  CGAL::Bounded_side(tri3::Triangulation_3::*side_of_circle2)(tri3::Cell_handle, int, const tri3::Point& p, bool) const =
    &tri3::Triangulation_3::side_of_circle;

  bool(tri3::Triangulation_3::*is_valid1)(bool, int) const =
    &tri3::Triangulation_3::is_valid;
  // bool(tri3::Triangulation_3::*is_valid2)(tri3::Cell_handle, bool, int) const =
  //   &tri3::Triangulation_3::is_valid;

#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
  tri3::Vertex_handle(tri3::Triangulation_3::*insert1)(const tri3::Point&, tri3::Cell_handle, bool*) =
    &tri3::Triangulation_3::insert;
  tri3::Vertex_handle(tri3::Triangulation_3::*insert2)(const tri3::Point&, tri3::Vertex_handle, bool*) =
    &tri3::Triangulation_3::insert;
  tri3::Vertex_handle(tri3::Triangulation_3::*insert3)(const tri3::Point&, tri3::Locate_type, tri3::Cell_handle, int, int, bool*) =
    &tri3::Triangulation_3::insert;
#else
  tri3::Vertex_handle(tri3::Triangulation_3::*insert1)(const tri3::Point&, tri3::Cell_handle) =
    &tri3::Triangulation_3::insert;
  tri3::Vertex_handle(tri3::Triangulation_3::*insert2)(const tri3::Point&, tri3::Vertex_handle) =
    &tri3::Triangulation_3::insert;
  tri3::Vertex_handle(tri3::Triangulation_3::*insert3)(const tri3::Point&, tri3::Locate_type, tri3::Cell_handle, int, int) =
    &tri3::Triangulation_3::insert;
#endif

  tri3::Vertex_handle(tri3::Triangulation_3::*nearest_vertex)(const tri3::Point&, tri3::Cell_handle) const =
    &tri3::Triangulation_3::nearest_vertex;

  bp::class_<tri3::Triangulation_3>("Triangulation_3")
    .def(bp::init<>())
    .def(bp::init<const tri3::Traits&>())
    .def("__init__", bp::make_constructor(&dt3_init))
    // Insertion
    .def("insert", insert1)
    .def("insert", insert2)
    .def("insert", insert3)
#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
    .def("insert", &insert4)
    .def("insert", &insert5)
    .def("insert", &insert6)
#endif
    .def("insert_points", &insert_points)

    // template<class PointWithInfoInputIterator >
    // std::ptrdiff_t 	insert (PointWithInfoInputIterator first, PointWithInfoInputIterator last)

    // Displacement
    .def("move_if_no_collision", &tri3::Triangulation_3::move_if_no_collision)
    .def("move", &tri3::Triangulation_3::move)

    // Removal
    .def<void(tri3::Triangulation_3::*)(tri3::Vertex_handle)>("remove", &tri3::Triangulation_3::remove)
    // .def<bool(tri3::Triangulation_3::*)(tri3::Vertex_handle, bool*)>("remove", &tri3::Triangulation_3::remove)

    // template<typename InputIterator >
    // int remove (InputIterator first, InputIterator beyond)

    // template<typename InputIterator >
    // int remove_cluster (InputIterator first, InputIterator beyond)

    // Queries
    .def("side_of_sphere", side_of_sphere)
    .def("side_of_circle", side_of_circle1)
    .def("side_of_circle", side_of_circle2)
    .def("nearest_vertex", nearest_vertex)
    .def("nearest_vertex_in_cell", &tri3::Triangulation_3::nearest_vertex_in_cell)
    .def("is_valid", is_valid1)
    // .def("is_valid", is_valid2)
    ;
}

#endif

template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

bool vertex_is_valid1(const tri3::Vertex& vertex, bool verbose, int level) { return vertex.is_valid(verbose, level); }
bool vertex_is_valid2(const tri3::Vertex& vertex, bool verbose) { return vertex.is_valid(verbose); }
bool vertex_is_valid3(const tri3::Vertex& vertex) { return vertex.is_valid(); }

bool cell_is_valid1(const tri3::Cell& cell, bool verbose, int level) { return cell.is_valid(verbose, level); }
bool cell_is_valid2(const tri3::Cell& cell, bool verbose) { return cell.is_valid(verbose); }
bool cell_is_valid3(const tri3::Cell& cell) { return cell.is_valid(); }

void export_triangulation_3() {
  bp::class_<tri3::Vertex_handle>("Vertex_handle")
    .def(bp::init<>())
    .def("value", &value<tri3::Vertex_handle>, bp::return_value_policy<bp::reference_existing_object>())
    ;

  bp::class_<tri3::Cell_handle>("Cell_handle")
    .def(bp::init<>())
    .def("value", &value<tri3::Cell_handle>, bp::return_value_policy<bp::reference_existing_object>())
    ;

  bp::class_<tri3::Vertex>("Vertex")
    .def(bp::init<>())
    // Access Functions
    .def<tri3::Cell_handle(tri3::Vertex::*)()const>("cell", &tri3::Vertex::cell)
    .def<const tri3::Point&(tri3::Vertex::*)() const>("point", &tri3::Vertex::point, bp::return_internal_reference<>())
    // Setting
    .def("set_cell", &tri3::Vertex::set_cell)
    .def("set_point", &tri3::Vertex::set_point)
    // Checking
    .def("is_valid", &vertex_is_valid1)
    .def("is_valid", &vertex_is_valid2)
    .def("is_valid", &vertex_is_valid3)
    ;

  void(tri3::Cell::*set_vertices)(tri3::Vertex_handle, tri3::Vertex_handle, tri3::Vertex_handle, tri3::Vertex_handle) =
    &tri3::Cell::set_vertices;
  void(tri3::Cell::*set_neighbors)(tri3::Cell_handle, tri3::Cell_handle, tri3::Cell_handle, tri3::Cell_handle) =
    &tri3::Cell::set_neighbors;

  bp::class_<tri3::Cell>("Cell")
    .def(bp::init<>())
    // Access Functions
    .def("vertex", &tri3::Cell::vertex)
    .def<int(tri3::Cell::*)(tri3::Vertex_handle) const>("index", &tri3::Cell::index)
    .def<int(tri3::Cell::*)(tri3::Cell_handle) const>("index", &tri3::Cell::index)
    .def<bool(tri3::Cell::*)(tri3::Vertex_handle) const>("has_vertex", &tri3::Cell::has_vertex)
    .def<bool(tri3::Cell::*)(tri3::Vertex_handle, int&) const>("has_vertex", &tri3::Cell::has_vertex)
    .def("neighbor", &tri3::Cell::neighbor)
    .def<bool(tri3::Cell::*)(tri3::Cell_handle n) const>("has_neighbor", &tri3::Cell::has_neighbor)
    .def<bool(tri3::Cell::*)(tri3::Cell_handle n, int &i) const>("has_neighbor", &tri3::Cell::has_neighbor)
    // Setting
    .def("set_vertex", &tri3::Cell::set_vertex)
    .def("set_vertices", set_vertices)
    .def("set_neighbor", &tri3::Cell::set_neighbor)
    .def("set_neighbors", set_neighbors)
    //  Checking
    .def("is_valid", &cell_is_valid1)
    .def("is_valid", &cell_is_valid2)
    .def("is_valid", &cell_is_valid3)
    ;

  bp::class_<tri3::Facet>("Facet")
    .def_readwrite("first", &tri3::Facet::first)
    .def_readwrite("second", &tri3::Facet::second)
    ;

  bp::class_<tri3::Edge>("Edge")
    .def_readwrite("first", &tri3::Edge::first)
    .def_readwrite("second", &tri3::Edge::second)
    .def_readwrite("third", &tri3::Edge::third)
    ;

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
  export_delaunay_triangulation_3();
#endif
}
