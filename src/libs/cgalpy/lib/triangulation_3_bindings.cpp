// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/static_assert.hpp>

#include "CGALPY/triangulation_3_types.hpp"

namespace bp = boost::python;

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY

Delaunay_triangulation_3* dt3_init(boost::python::list& lst)
{
  auto begin = boost::python::stl_input_iterator<Tri3_point>(lst);
  auto end = boost::python::stl_input_iterator<Tri3_point>();
  return new Delaunay_triangulation_3(begin, end);
}

std::ptrdiff_t insert_points(Delaunay_triangulation_3& dt, boost::python::list& lst)
{
  if (! lst) return 0;
  if (! bp::extract<Tri3_point>(lst[0]).check()) return 0;
  auto begin = boost::python::stl_input_iterator<Tri3_point>(lst);
  auto end = boost::python::stl_input_iterator<Tri3_point>();
  return dt.insert(begin, end);
}

#if (CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT)

Tri3_vertex_handle insert4(Delaunay_triangulation_3& dt, const Tri3_point& p, Tri3_cell_handle start)
{ return dt.insert(p, start); }

Tri3_vertex_handle insert5(Delaunay_triangulation_3& dt, const Tri3_point& p, Tri3_vertex_handle hint)
{ return dt.insert(p, hint); }

Tri3_vertex_handle insert6(Delaunay_triangulation_3& dt, const Tri3_point& p, Tri3_locate_type lt, Tri3_cell_handle c, int li, int lj)
{ return dt.insert(p, lt, c, li, lj); }

void export_delaunay_triangulation_3()
{
  using namespace boost::python;

  CGAL::Bounded_side(Delaunay_triangulation_3::*side_of_sphere)(Tri3_cell_handle, const Tri3_point&, bool) const =
    &Delaunay_triangulation_3::side_of_sphere;
  CGAL::Bounded_side(Delaunay_triangulation_3::*side_of_circle1)(const Tri3_facet&, const Tri3_point&, bool) const =
    &Delaunay_triangulation_3::side_of_circle;
  CGAL::Bounded_side(Delaunay_triangulation_3::*side_of_circle2)(Tri3_cell_handle, int, const Tri3_point& p, bool) const =
    &Delaunay_triangulation_3::side_of_circle;

#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert1)(const Tri3_point&, Tri3_cell_handle, bool*) =
    &Delaunay_triangulation_3::insert;
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert2)(const Tri3_point&, Tri3_vertex_handle, bool*) =
    &Delaunay_triangulation_3::insert;
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert3)(const Tri3_point&, Tri3_locate_type, Tri3_cell_handle, int, int, bool*) =
    &Delaunay_triangulation_3::insert;
#else
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert1)(const Tri3_point&, Tri3_cell_handle) =
    &Delaunay_triangulation_3::insert;
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert2)(const Tri3_point&, Tri3_vertex_handle) =
    &Delaunay_triangulation_3::insert;
  Tri3_vertex_handle(Delaunay_triangulation_3::*insert3)(const Tri3_point&, Tri3_locate_type, Tri3_cell_handle, int, int) =
    &Delaunay_triangulation_3::insert;
#endif

  Tri3_vertex_handle(Delaunay_triangulation_3::*nearest_vertex)(const Tri3_point&, Tri3_cell_handle) const =
    &Delaunay_triangulation_3::nearest_vertex;

  class_<Delaunay_triangulation_3>("Delaunay_triangulation_3")
    .def(init<>())
    .def(init<const Tri3_traits&>())
    .def("__init__", make_constructor(&dt3_init))
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
    .def("move_if_no_collision", &Delaunay_triangulation_3::move_if_no_collision)
    .def("move", &Delaunay_triangulation_3::move)

    // Removal
    .def<void(Delaunay_triangulation_3::*)(Tri3_vertex_handle)>("remove", &Delaunay_triangulation_3::remove)
    // .def<bool(Delaunay_triangulation_3::*)(Tri3_vertex_handle, bool*)>("remove", &Delaunay_triangulation_3::remove)

    // template<typename InputIterator >
    // int remove (InputIterator first, InputIterator beyond)

    // template<typename InputIterator >
    // int remove_cluster (InputIterator first, InputIterator beyond)

    // Queries
    .def("side_of_sphere", side_of_sphere)
    .def("side_of_circle", side_of_circle1)
    .def("side_of_circle", side_of_circle2)
    .def("nearest_vertex", nearest_vertex)
    .def("nearest_vertex_in_cell", &Delaunay_triangulation_3::nearest_vertex_in_cell)
    ;
}

#endif

template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

bool vertex_is_valid1(const Tri3_vertex& vertex, bool verbose, int level) { return vertex.is_valid(verbose, level); }
bool vertex_is_valid2(const Tri3_vertex& vertex, bool verbose) { return vertex.is_valid(verbose); }
bool vertex_is_valid3(const Tri3_vertex& vertex) { return vertex.is_valid(); }

bool cell_is_valid1(const Tri3_cell& cell, bool verbose, int level) { return cell.is_valid(verbose, level); }
bool cell_is_valid2(const Tri3_cell& cell, bool verbose) { return cell.is_valid(verbose); }
bool cell_is_valid3(const Tri3_cell& cell) { return cell.is_valid(); }

void export_triangulation_3()
{
  using namespace boost::python;

  class_<Tri3_vertex_handle>("Vertex_handle")
    .def(init<>())
    .def("value", &value<Tri3_vertex_handle>, return_value_policy<reference_existing_object>())
    ;

  class_<Tri3_cell_handle>("Cell_handle")
    .def(init<>())
    .def("value", &value<Tri3_cell_handle>, return_value_policy<reference_existing_object>())
    ;

  class_<Tri3_vertex>("Vertex")
    .def(init<>())
    // Access Functions
    .def<Tri3_cell_handle(Tri3_vertex::*)()const>("cell", &Tri3_vertex::cell)
    .def<const Tri3_point&(Tri3_vertex::*)() const>("point", &Tri3_vertex::point, return_internal_reference<>())
    // Setting
    .def("set_cell", &Tri3_vertex::set_cell)
    .def("set_point", &Tri3_vertex::set_point)
    // Checking
    .def("is_valid", &vertex_is_valid1)
    .def("is_valid", &vertex_is_valid2)
    .def("is_valid", &vertex_is_valid3)
    ;

  void(Tri3_cell::*set_vertices)(Tri3_vertex_handle, Tri3_vertex_handle, Tri3_vertex_handle, Tri3_vertex_handle) =
    &Tri3_cell::set_vertices;
  void(Tri3_cell::*set_neighbors)(Tri3_cell_handle, Tri3_cell_handle, Tri3_cell_handle, Tri3_cell_handle) =
    &Tri3_cell::set_neighbors;

  class_<Tri3_cell>("Cell")
    .def(init<>())
    // Access Functions
    .def("vertex", &Tri3_cell::vertex)
    .def<int(Tri3_cell::*)(Tri3_vertex_handle) const>("index", &Tri3_cell::index)
    .def<int(Tri3_cell::*)(Tri3_cell_handle) const>("index", &Tri3_cell::index)
    .def<bool(Tri3_cell::*)(Tri3_vertex_handle) const>("has_vertex", &Tri3_cell::has_vertex)
    .def<bool(Tri3_cell::*)(Tri3_vertex_handle, int&) const>("has_vertex", &Tri3_cell::has_vertex)
    .def("neighbor", &Tri3_cell::neighbor)
    .def<bool(Tri3_cell::*)(Tri3_cell_handle n) const>("has_neighbor", &Tri3_cell::has_neighbor)
    .def<bool(Tri3_cell::*)(Tri3_cell_handle n, int &i) const>("has_neighbor", &Tri3_cell::has_neighbor)
    // Setting
    .def("set_vertex", &Tri3_cell::set_vertex)
    .def("set_vertices", set_vertices)
    .def("set_neighbor", &Tri3_cell::set_neighbor)
    .def("set_neighbors", set_neighbors)
    //  Checking
    .def("is_valid", &cell_is_valid1)
    .def("is_valid", &cell_is_valid2)
    .def("is_valid", &cell_is_valid3)
    ;

  class_<Tri3_facet>("Facet")
    .def_readwrite("first", &Tri3_facet::first)
    .def_readwrite("second", &Tri3_facet::second)
    ;

  class_<Tri3_edge>("Edge")
    .def_readwrite("first", &Tri3_edge::first)
    .def_readwrite("second", &Tri3_edge::second)
    .def_readwrite("third", &Tri3_edge::third)
    ;

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
  export_delaunay_triangulation_3();
#endif
}

#endif
