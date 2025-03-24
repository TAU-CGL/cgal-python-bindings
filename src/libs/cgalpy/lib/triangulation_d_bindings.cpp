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
#include <nanobind/stl/pair.h>

#include "CGALPY/types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_d_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace trid {

//
template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

/// Traingulation Data Structure
/// @{

//
py::list incident_full_cells1(const Triangulation_ds& tds, const Face& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tds.incident_full_cells(f, it);
  return res;
}

//
py::list incident_full_cells2(const Triangulation_ds& tds, const Vertex& v) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tds.incident_full_cells(Vertex_const_handle(&v), it);
  return res;
}

//
py::list star(const Triangulation_ds& tds, const Face& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tds.star(f, it);
  return res;
}

//
py::list incident_faces(const Triangulation_ds& tds, const Vertex& v) {
  py::list lst;
  return lst;
}

//
bool vertex_is_vertex(const Triangulation_ds& tds, const Vertex& v)
{ return tds.is_vertex(Vertex_const_handle(&v)); }

//
bool vertex_is_full_cell(const Triangulation_ds& tds, const Full_cell& c)
{ return tds.is_full_cell(Full_cell_const_handle(&c)); }

//
const Vertex& vertex(const Triangulation_ds& tds, const Full_cell& s, int i)
{ return value(tds.vertex(Full_cell_const_handle(&s), i)); }

//
const Full_cell& full_cell(const Triangulation_ds& tds, const Vertex& v)
{ return value(tds.full_cell(Vertex_const_handle(&v))); }

//
Vertex& insert_increase_dimension1(Triangulation_ds& tds, Vertex& v)
{ return *(tds.insert_increase_dimension(Vertex_handle(&v))); }

//
Vertex& insert_increase_dimension2(Triangulation_ds& tds)
{ return *(tds.insert_increase_dimension(Vertex_handle())); }

/// @}

/// Face
/// @{

//
void set_full_cell(Face& f, Full_cell& fc)
{ f.set_full_cell(Full_cell_handle(&fc)); }

/// @}

/// dD Triangulation
/// @{

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

template <typename T>
struct stl_dereference_input_iterator :
  boost::iterator_facade<stl_input_iterator<T>, T, std::forward_iterator_tag, T> {

  // Default constructor.
  // Workaround the lack of default constructor for py::detail::fast_iterator.
  // stl_dereference_input_iterator() {}
  stl_dereference_input_iterator() : m_it(py::list().end()) {}

  stl_dereference_input_iterator(const py::list& lst, bool isbegin = true) :
    m_it((isbegin) ? lst.begin() : lst.end())
  {}

  void increment() { ++m_it; }
  T dereference() const {
    auto& tmp = *m_it;
    return py::cast<T>(T(&tmp));
  }

  bool equal(stl_input_iterator<T> const& o) const { return m_it == o.m_it; }

private:
  py::detail::fast_iterator m_it;
};

//
py::list
insert_in_hole1(Triangulation_d& tri, const Point& p, py::list& full_cells,
                const Facet& ft) {

  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin = stl_dereference_input_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_input_iterator<Full_cell_handle>(full_cells, false);
  py::list final_res;
  final_res.append(*(tri.insert_in_hole(p, begin, end, ft, it)));
  final_res.append(res);
  return final_res;
}

//
Vertex& insert_in_hole2(Triangulation_d& tri, const Point& p,
                        py::list& full_cells, const Facet& ft) {
  auto begin = stl_dereference_input_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_input_iterator<Full_cell_handle>(full_cells, false);
  return *(tri.insert_in_hole(p, begin, end, ft));
}

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

//
py::object vertices(const Triangulation_d& tri)
{ return make_iterator(tri.vertices_begin(), tri.vertices_end()); }

//
py::object finite_vertices(const Triangulation_d& tri)
{ return make_iterator(tri.finite_vertices_begin(), tri.finite_vertices_end()); }

//
py::object full_cells(const Triangulation_d& tri)
{ return make_iterator(tri.full_cells_begin(), tri.full_cells_end()); }

//
py::object finite_full_cells(const Triangulation_d& tri)
{ return make_iterator(tri.finite_full_cells_begin(), tri.finite_full_cells_end()); }

//
py::object facets(Triangulation_d& tri)
{ return make_iterator(tri.facets_begin(), tri.facets_end()); }

//
py::object finite_facets(Triangulation_d& tri)
{ return make_iterator(tri.finite_facets_begin(), tri.finite_facets_end()); }

/// @}

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
      .def("vertex", &trid::vertex, ri)
      .def("is_vertex", trid::vertex_is_vertex)
      .def("full_cell", trid::full_cell, ri)
      .def("is_full_cell", trid::vertex_is_full_cell)
    //   neighbor
    //   mirror_index
    //   index_of_covertex
    //   is_boundary_facet
    //   rotate_rotor
    //   collapse_face
    //   remove_decrease_dimension
      .def("insert_in_full_cell",
           [](Tds& tds, Fc& c)->Vertex&
           { return *(tds.insert_in_full_cell(trid::Full_cell_handle(&c))); },
           ri)
      .def("insert_in_face",
           [](Tds& tds, Face& f)->Vertex& { return *(tds.insert_in_face(f)); },
           ri)
      .def("insert_in_facet",
           [](Tds& tds, Facet& ft)->Vertex&
           { return *(tds.insert_in_facet(ft)); },
           ri)
      // insert_in_hole
      // insert_in_tagged_hole
      .def("insert_increase_dimension", &trid::insert_increase_dimension1, ri)
      .def("insert_increase_dimension", &trid::insert_increase_dimension2, ri)
      .def("clear", &Tds::clear)
      // .def("new_vertex",
      // .def("new_full_cell",
      // .def("set_current_dimension",
      // .def("delete_full_cell",
      // .def("delete_full_cells",
      // .def("delete_vertex",
      // .def("associate_vertex_with_full_cell",
      // .def("set_neighbors
      .def("is_valid", &Tds::is_valid, py::arg("verbose") = true, py::arg("level") = 0)
      // .def("gather_full_cells",
      // .def("incident_full_cells",
      // .def("star",
      // .def("incident_upper_faces",
      // .def("incident_faces",
      // .def("read_full_cells",
      // .def("write_full_cells",
      ;

    // Iterators:
    // vertices
    // full_cells
    // facets
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
      .def("insert_in_hole", &trid::insert_in_hole2)
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
        .def("full_cell",
             [](const Vertex& v)->Fc& { return *(v.full_cell()); }, ri)
        .def("set_full_cell",
             [](Vertex& v, Fc& c)->void
             {v.set_full_cell(trid::Full_cell_handle(&c)); })
        .def("is_valid",
             [](const Vertex& v, bool verbose, int level)->bool
             { return v.is_valid(verbose, level); },
             py::arg("verbose") = false, py::arg("level") = 0)

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

    // using Pi = Tri::Point_iterator;
    // add_iterator<Pi, Pi, const Point&>("Point_iterator", tri_c);
    using Vi = Tri::Vertex_iterator;
    using Fvi = Tri::Finite_vertex_iterator;
    using Fci = Tri::Full_cell_iterator;
    using Ffci = Tri::Finite_full_cell_iterator;
    using Fi = Tri::Facet_iterator;
    using Ffi = Tri::Finite_facet_iterator;

    add_iterator<Vi, Vi, const Vertex&>("Vertex_iterator", tri_c);
    add_iterator<Fvi, Fvi, const Vertex&>("Finite_vertex_iterator", tri_c);
    add_iterator<Fci, Fci, const Fc&>("Full_cell_iterator", tri_c);
    add_iterator<Ffci, Ffci, const Fc&>("Finite_full_cell_iterator", tri_c);
    add_iterator<Fi, Fi, const Facet&>("Facet_iterator", tri_c);
    add_iterator<Ffi, Ffi, const Facet&>("Finite_facet_iterator", tri_c);

    tri_c
      .def("vertices", &trid::vertices, py::keep_alive<0, 1>())
      .def("finite_vertices", &trid::finite_vertices, py::keep_alive<0, 1>())
      .def("full_cells", &trid::full_cells, py::keep_alive<0, 1>())
      .def("finite_full_cells", &trid::finite_full_cells, py::keep_alive<0, 1>())
      .def("facets", &trid::facets, py::keep_alive<0, 1>())
      .def("finite_facets", &trid::finite_facets, py::keep_alive<0, 1>())
      ;
  }
}
