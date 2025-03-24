// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>

#include "CGALPY/types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_d_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace trid {

// #if CGALPY_TRID == CGALPY_TRID_DELAUNAY

// //
// void dt3_init(trid::Triangulation_3* tri, py::list& lst) {
//   auto begin = stl_input_iterator<trid::Point>(lst);
//   auto end = stl_input_iterator<trid::Point>(lst, false);
//   new (tri) trid::Triangulation_3(begin, end);  // placement new
// }

// //
// std::ptrdiff_t insert_points(trid::Triangulation_3& dt, py::list& lst) {
//   if (! lst) return 0;
//   if (! py::isinstance<trid::Point>(lst[0])) return 0;
//   auto begin = stl_input_iterator<trid::Point>(lst);
//   auto end = stl_input_iterator<trid::Point>(lst, false);
//   return dt.insert(begin, end);
// }

// #if (CGALPY_TRID_LOCATION_POLICY == CGALPY_TRID_LOCATION_POLICY_COMPACT)

// //
// trid::Vertex_handle insert4(trid::Triangulation_3& dt,
//                             const trid::Point& p, trid::Cell_handle start)
// { return dt.insert(p, start); }

// //
// trid::Vertex_handle insert5(trid::Triangulation_3& dt,
//                             const trid::Point& p, trid::Vertex_handle hint)
// { return dt.insert(p, hint); }

// //
// trid::Vertex_handle insert6(trid::Triangulation_3& dt,
//                             const trid::Point& p, trid::Locate_type lt,
//                             trid::Cell_handle c, int li, int lj)
// { return dt.insert(p, lt, c, li, lj); }

// #endif

//
template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

//
const Vertex& vertex(const Triangulation_ds& tds, const Full_cell& s, int i)
{ return value(tds.vertex(Full_cell_const_handle(&s), i)); }

//
py::list incident_full_cells1(const Triangulation_ds& tds, const Face& f) {
  py::list lst;
  return lst;
}

//
py::list incident_full_cells2(const Triangulation_ds& tds, const Vertex& v) {
  py::list lst;
  return lst;
}

//
py::list star(const Triangulation_ds& tds, const Face& v) {
  py::list lst;
  return lst;
}

//
py::list incident_faces(const Triangulation_ds& tds, const Vertex& v) {
  py::list lst;
  return lst;
}

//
void set_full_cell(Face& f, Full_cell& fc)
{ f.set_full_cell(Full_cell_handle(&fc)); }

//
Full_cell& locate1(const Triangulation_d& tri, const Point& p, Locate_type& lt,
                   Face& f1, Facet& f2, Full_cell& start)
{ return *(tri.locate(p, lt, f1, f2, Full_cell_handle(&start))); }

//
Full_cell& locate2(const Triangulation_d& tri, const Point& p, Locate_type& lt,
                   Face& f, Facet& facet, Vertex& v)
{ return *(tri.locate(p, lt, f, facet, Vertex_handle(&v))); }

//
Full_cell& locate3(const Triangulation_d& tri, const Point& p, Full_cell& s)
{ return *(tri.locate(p, Full_cell_handle(&s))); }

//
Full_cell& locate4(const Triangulation_d& tri, const Point& p, Vertex& v)
{ return *(tri.locate(p, Vertex_handle(&v))); }

//
size_type insert1(Triangulation_d& tri, py::list& points) {
  auto begin = stl_input_iterator<Point>(points);
  auto end = stl_input_iterator<Point>(points, false);
  return tri.insert(begin, end);
}

Vertex& insert2(Triangulation_d& tri, const Point& p, Locate_type lt,
                const Face& f, const Facet& ft, Full_cell& fc)
{ return *(tri.insert(p, lt, f, ft, Full_cell_handle(&fc))); }

//
Vertex& insert3(Triangulation_d& tri, const Point& p, Full_cell& hint)
{ return *(tri.insert(p, Full_cell_handle(&hint))); }

//
Vertex& insert4(Triangulation_d& tri, const Point& p, Vertex& v)
{ return *(tri.insert(p, Vertex_handle(&v))); }

//
// std::pair<Vertex&, py::list>
// insert_in_hole1(Triangulation_d& tri, const Point& p, py::list& full_cells,
//                 const Facet& ft) {

//   py::list res;
//   auto op = [&] (const Full_cell& c) mutable { res.append(c); };
//   auto it = boost::make_function_output_iterator(std::ref(op));
//   auto begin = stl_input_iterator<Full_cell>(full_cells);
//   auto end = stl_input_iterator<Full_cell>(full_cells, false);
//   Vertex& v = *(tri.insert_in_hole(p, begin, end, ft, it));
//   return std::make_pair(v, res);
// }

//
// Vertex& insert_in_hole2(Triangulation_d& tri, const Point& p,
//                         py::list& full_cells, const Facet& ft) {
//   auto begin = stl_input_iterator<Full_cell>(full_cells);
//   auto end = stl_input_iterator<Full_cell>(full_cells, false);
//   return *(tri.insert_in_hole(p, begin, end, ft));
// }

//
Vertex& insert_in_face(Triangulation_d& tri, const Point& p, const Face& f)
{ return *(tri.insert_in_face(p, f)); }

//
Vertex& insert_in_facet(Triangulation_d& tri, const Point& p, const Facet& ft)
{ return *(tri.insert_in_facet(p, ft)); }

//
Vertex& insert_in_full_cell(Triangulation_d& tri, const Point& p, Full_cell& fc)
{ return *(tri.insert_in_full_cell(p, Full_cell_handle(&fc))); }

//
Vertex& insert_outside_convex_hull(Triangulation_d& tri, const Point& p,
                                   Full_cell& fc)
{ return *(tri.insert_outside_convex_hull(p, Full_cell_handle(&fc))); }

//
Vertex& insert_outside_affine_hull(Triangulation_d& tri, const Point& p)
{ return *(tri.insert_outside_affine_hull(p)); }

} // End of namespace trid

//
void export_triangulation_d(py::module_& m) {
  using Tri = trid::Triangulation_d;
  using Vertex = trid::Vertex;
  using Fc = trid::Full_cell;
  using Facet = trid::Facet;
  using Face = trid::Face;
  using Rotor = trid::Rotor;
  using Tds = trid::Triangulation_ds;
  using Point = trid::Point;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Tds>(m, "Triangulation_ds")) {
    py::class_<Tds>(m, "Triangulation_ds")
      .def(py::init<int>())
      .def(py::init<const Tds&>())
      .def("maximal_dimension", &Tds::maximal_dimension)
      .def("current_dimension", &Tds::current_dimension)
      .def("number_of_vertices", &Tds::number_of_vertices)
      .def("number_of_full_cells", &Tds::number_of_full_cells)
      .def("empty", &Tds::empty)
      .def("vertex", &trid::vertex);
    ;
  }

  if (! add_attr<Tri>(m, "Triangulation")) {
    py::class_<Tri> tri_c(m, "Triangulation");
    tri_c.def(py::init<int, const trid::Traits&>())
      .def(py::init<const Tri&>())
      .def("full_cell",
           [](Tri& tri, const Facet& f)->Fc& { return *(tri.full_cell(f)); }, ri)
      .def("index_of_covertex", &Tri::index_of_covertex)
      // .def("rotate_rotor", &Tri::rotate_rotor)
      .def("tds", py::overload_cast<>(&Tri::tds, py::const_))
      .def("geom_traits", &Tri::geom_traits, ri)
      .def("maximal_dimension", &Tri::maximal_dimension)
      .def("current_dimension", &Tri::current_dimension)
      .def("empty", &Tri::empty)
      .def("number_of_vertices", &Tri::number_of_vertices)
      .def("number_of_full_cells", &Tri::number_of_full_cells)
      .def("infinite_vertex",
           [](Tri& tri)->Vertex&
           { return *(tri.infinite_vertex()); }, ri)
      .def("infinite_full_cell",
           [](Tri& tri)->Fc&
           { return *(tri.infinite_full_cell()); }, ri)
      .def("number_of_finite_full_cells", &Tri::number_of_finite_full_cells)
      .def("is_infinite",
           py::overload_cast<const Vertex&>(&Tri::is_infinite, py::const_))
      .def("is_infinite",
           py::overload_cast<const Fc&>(&Tri::is_infinite, py::const_))
      .def("is_infinite",
           py::overload_cast<const Facet&>(&Tri::is_infinite, py::const_))
      .def("is_infinite",
           py::overload_cast<const Face&>(&Tri::is_infinite, py::const_))
      .def("incident_full_cells", &trid::incident_full_cells1)
      .def("incident_full_cells", &trid::incident_full_cells2)
      .def("star", &trid::star)
      .def("incident_faces", &trid::incident_faces)
      .def("orientation", &Tri::orientation)
      .def("clear", &Tri::clear)
      .def("set_current_dimension", &Tri::set_current_dimension)
      .def("new_full_cell",
           [](Tri& tri)->Fc& { return *(tri.new_full_cell()); }, ri)
      // .def("new_vertex", &Tri::new_vertex)
      // .def("new_vertex", &Tri::new_vertex)
      // .def("set_neighbors", &Tri::set_neighbors)
      .def("is_valid", &Tri::is_valid)
      // .def("are_incident_full_cells_valid", &Tri::are_incident_full_cells_valid)
      .def("locate", &trid::locate1)
      .def("locate", &trid::locate2)
      .def("locate", &trid::locate3)
      .def("locate", &trid::locate4)
      // .def("contract_face",
      //      [](Tri& tri, const Point& p, const Face& f)->Vertex&
      //      { return *(tri.contract_face(p, f)); }, ri)
      .def("insert", &trid::insert1)
      .def("insert", &trid::insert2)
      .def("insert", &trid::insert3)
      .def("insert", &trid::insert4)
      // .def("insert_in_hole", &trid::insert_in_hole1)
      // .def("insert_in_hole", &trid::insert_in_hole2)
      .def("insert_in_face", &trid::insert_in_face)
      .def("insert_in_facet", &trid::insert_in_facet)
      .def("insert_in_full_cell", &trid::insert_in_full_cell)
      .def("insert_outside_convex_hull", &trid::insert_outside_convex_hull)
      .def("insert_outside_affine_hull", &trid::insert_outside_affine_hull)
      .def("reorient_full_cells", &Tri::reorient_full_cells)
      ;

    // Lock_data_structure
    py::enum_<trid::Locate_type>(tri_c, "Locate_type")
      .value("ON_VERTEX", Tri::ON_VERTEX)
      .value("IN_FACE", Tri::IN_FACE)
      .value("IN_FACET", Tri::IN_FACET)
      .value("IN_FULL_CELL", Tri::IN_FULL_CELL)
      .value("OUTSIDE_CONVEX_HULL", Tri::OUTSIDE_CONVEX_HULL)
      .value("OUTSIDE_AFFINE_HULL", Tri::OUTSIDE_AFFINE_HULL)
      .export_values()
     ;

    if (! add_attr<Vertex>(tri_c, "Vertex")) {
      py::class_<Vertex>(tri_c, "Vertex")
        .def(py::init<const Point&>())
        .def("point", &trid::Vertex::point, ri)
        .def("set_point", &trid::Vertex::set_point)

#ifdef CGALPY_TRID_VERTEX_WITH_DATA
        .def("data", py::overload_cast<>(&Vertex::data, py::const_), ri)
        .def("set_data",
             [](Vertex& v, py::object data)->void{ v.data() = data; })
#endif
        ;
    }

    if (! add_attr<Fc>(tri_c, "Full_cell")) {
      py::class_<Fc>(tri_c, "Full_cell")
        .def(py::init<int>())
        .def(py::init<const Fc&>())

 #ifdef CGALPY_TRID_FULL_CELL_WITH_DATA
        .def("data", py::overload_cast<>(&Fc::data, py::const_), ri)
        .def("set_data", [](Fc& c, py::object data)->void{ c.data() = data; })
#endif
        ;
    }

    if (! add_attr<Face>(tri_c, "Face")) {
      py::class_<Face>(tri_c, "Face")
        .def(py::init<int>())
        .def(py::init<const Face&>())
        .def("face_dimension", &Face::face_dimension)
        .def("index", &Face::index)
        .def("vertex",
             [](Face& f, int i)->Vertex& { return *(f.vertex(i)); }, ri)
        .def("Full_cell", [](Face& f)->Fc& { return *(f.full_cell()); }, ri)
        .def("clear", &Face::clear)
        .def("set_full_cell", trid::set_full_cell)
        .def("set_index", &Face::set_index)
        ;
    }

    add_attr<trid::Geom_traits>(tri_c, "Geom_traits");

    if (! add_attr<trid::Vertex_handle>(tri_c, "Vertex_handle")) {
      py::class_<trid::Vertex_handle>(tri_c, "Vertex_handle")
        .def(py::init<>())
        .def("value", &trid::value<trid::Vertex_handle>, ri)
        ;
    }

    if (! add_attr<trid::Full_cell_handle>(tri_c, "Full_cell_handle")) {
      py::class_<trid::Full_cell_handle>(tri_c, "Full_cell_handle")
        .def(py::init<>())
        .def("value", &trid::value<trid::Full_cell_handle>, ri)
        ;
    }
  }
    // #if CGALPY_TRID == CGALPY_TRID_DELAUNAY
//     .def("__init__", &trid::dt3_init)
// #endif
//     // Insertion
//     .def("insert", insert1)
//     .def("insert", insert2)
//     .def("insert", insert3)
// #if CGALPY_TRID_LOCATION_POLICY == CGALPY_TRID_LOCATION_POLICY_COMPACT
//     .def("insert", &trid::insert4)
//     .def("insert", &trid::insert5)
//     .def("insert", &trid::insert6)
// #endif
//     .def("insert_points", &trid::insert_points)

//     // template<class PointWithInfoInputIterator >
//     // std::ptrdiff_t insert (PointWithInfoInputIterator first, PointWithInfoInputIterator last)

//     // Displacement
//     .def("move_if_no_collision", &Tri::move_if_no_collision)
//     .def("move", &Tri::move)

//     // Removal
//     .def("remove", static_cast<void(Tri::*)(trid::Vertex_handle)>(&Tri::remove))
//     // .def("remove", py::overload_cast<trid::Vertex_handle, bool*>(&Tri::remove))

//     // template<typename InputIterator >
//     // int remove (InputIterator first, InputIterator beyond)

//     // template<typename InputIterator >
//     // int remove_cluster (InputIterator first, InputIterator beyond)

//     // Queries
//     .def("side_of_sphere", side_of_sphere)
//     .def("side_of_circle", side_of_circle1)
//     .def("side_of_circle", side_of_circle2)
//     .def("nearest_vertex", nearest_vertex)
//     .def("nearest_vertex_in_cell", &Tri::nearest_vertex_in_cell)
//     .def("is_valid", is_valid1)
//     // .def("is_valid", is_valid2)

//   if (! add_attr<trid::Point>(tri_c, "Point"))
//     std::cerr << "'trid::Point' not registered!\n";
//   add_attr<trid::Segment>(tri_c, "Segment");
//   add_attr<trid::Triangle>(tri_c, "Triangle");
//   add_attr<trid::Tetrahedron>(tri_c, "Tetrahedron");

//   void(trid::Cell::*set_vertices)(trid::Vertex_handle, trid::Vertex_handle,
//                                   trid::Vertex_handle, trid::Vertex_handle) =
//     &trid::Cell::set_vertices;
//   void(trid::Cell::*set_neighbors)(trid::Cell_handle, trid::Cell_handle,
//                                    trid::Cell_handle, trid::Cell_handle) =
//     &trid::Cell::set_neighbors;

//   py::class_<trid::Facet>(tri_c, "Facet")
//     .def_rw("first", &trid::Facet::first)
//     .def_rw("second", &trid::Facet::second)
//     ;

//   py::class_<trid::Edge>(tri_c, "Edge")
//     .def_rw("first", &trid::Edge::first)
//     .def_rw("second", &trid::Edge::second)
//     .def_rw("third", &trid::Edge::third)
//     ;

//   using Avi = Tri::All_vertices_iterator;
//   using Aei = Tri::All_edges_iterator;
//   using Aci = Tri::All_cells_iterator;
//   using Afi = Tri::All_facets_iterator;

//   using Fvi = Tri::Finite_vertices_iterator;
//   using Fei = Tri::Finite_edges_iterator;
//   using Fci = Tri::Finite_cells_iterator;
//   using Ffi = Tri::Finite_facets_iterator;

//   using Pi = Tri::Point_iterator;

//   using Vertex = Tri::Vertex;
//   using Edge = Tri::Vertex;
//   using Cell = Tri::Cell;
//   using Face = Tri::Facet;

//   using Point = Tri::Point;

//   // Iterators
//   add_iterator<Avi, Avi, const Vertex&>("All_vertices_iterator", tri_c);
//   add_iterator<Aei, Aei>("All_edges_iterator", tri_c);
//   add_iterator<Aci, Aci, const Cell&>("All_cells_iterator", tri_c);
//   add_iterator<Afi, Afi, const Face&>("All_facets_iterator", tri_c);

//   add_iterator<Fvi, Fvi, const Vertex&>("Finite_vertices_iterator", tri_c);
//   add_iterator<Fei, Fei>("Finite_edges_iterator", tri_c);
//   add_iterator<Fci, Fci, const Cell&>("Finite_cells_iterator", tri_c);
//   add_iterator<Ffi, Ffi, const Face&>("Finite_facets_iterator", tri_c);

//   add_iterator<Pi, Pi, const Point&>("Point_iterator", tri_c);

//   tri_c.def("all_vertices", &trid::all_vertices, py::keep_alive<0, 1>())
//     .def("all_edges", &trid::all_edges, py::keep_alive<0, 1>())
//     .def("all_cells", &trid::all_cells, py::keep_alive<0, 1>())
//     .def("all_facets", &trid::all_facets, py::keep_alive<0, 1>())
//     .def("finite_vertices", &trid::finite_vertices, py::keep_alive<0, 1>())
//     .def("finite_edges", &trid::finite_edges, py::keep_alive<0, 1>())
//     .def("finite_cells", &trid::finite_cells, py::keep_alive<0, 1>())
//     .def("finite_facets", &trid::finite_facets, py::keep_alive<0, 1>())
//     .def("points", &trid::points, py::keep_alive<0, 1>())
//     ;

  // Todo
  // Simplex;

  // Facet_iterator;
  // Cell_iterator;
  // Segment_cell_iterator;
  // Segment_simplex_iterator;
}
