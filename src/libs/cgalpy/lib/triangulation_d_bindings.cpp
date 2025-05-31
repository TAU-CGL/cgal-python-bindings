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

#include "CGALPY/add_attr.hpp"
#include "CGALPY/make_iterator.hpp"
#include "CGALPY/triangulation_d_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/stl_dereference_input_iterator.hpp"
#include "CGALPY/types.hpp"

namespace py = nanobind;

void export_trid_vertex(py::class_<trid::Triangulation_d>&);
void export_trid_full_cell(py::class_<trid::Triangulation_d>&);

namespace trid {

//!
template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

/// Traingulation Data Structure
/// @{

//!
auto gather_full_cells(const Triangulation_ds& tds, Full_cell& start, py::object tp) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto criterion = [tp](const Facet& f){ return py::cast<bool>(tp(f)); };
  auto f = tds.gather_full_cells(Full_cell_handle(&start), criterion, it);
  return py::make_tuple(f, res);
}

//!
py::list tds_incident_full_cells1(const Triangulation_ds& tds, const Face& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tds.incident_full_cells(f, it);
  return res;
}

//!
py::list tds_incident_full_cells2(const Triangulation_ds& tds, const Vertex& v) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tds.incident_full_cells(Vertex_const_handle(&v), it);
  return res;
}

//!
py::list tds_star(const Triangulation_ds& tds, const Face& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tds.star(f, it);
  return res;
}

// comparator not implemented yet
// py::list tds_incident_faces(const Triangulation_ds& tds, const Vertex& v,
//                             int dim, bool upper_faces) {
//   py::list res;
//   auto op = [&] (const Face& f) mutable { res.append(f); };
//   auto it = boost::make_function_output_iterator(std::ref(op));
//   tri.incident_faces(Vertex_const_handle(&v), dim, it, comparator, upper_faces);
//   return res;
// }

//!
bool vertex_is_vertex(const Triangulation_ds& tds, const Vertex& v)
{ return tds.is_vertex(Vertex_const_handle(&v)); }

//!
bool vertex_is_full_cell(const Triangulation_ds& tds, const Full_cell& c)
{ return tds.is_full_cell(Full_cell_const_handle(&c)); }

//!
const Vertex& vertex(const Triangulation_ds& tds, const Full_cell& c, int i)
{ return value(tds.vertex(Full_cell_const_handle(&c), i)); }

//!
const Full_cell& full_cell1(const Triangulation_ds& tds, const Vertex& v)
{ return value(tds.full_cell(Vertex_const_handle(&v))); }

//!
Vertex& insert_increase_dimension1(Triangulation_ds& tds, Vertex& star)
{ return *(tds.insert_increase_dimension(Vertex_handle(&star))); }

//!
Vertex& insert_increase_dimension2(Triangulation_ds& tds)
{ return *(tds.insert_increase_dimension(Vertex_handle())); }

//!
Vertex& tds_insert_in_full_cell(Triangulation_ds& tds, Full_cell& c)
{ return *(tds.insert_in_full_cell(Full_cell_handle(&c))); }

//!
Vertex& tds_insert_in_face(Triangulation_ds& tds, Face& f)
{ return *(tds.insert_in_face(f)); }

//!
Vertex& tds_insert_in_facet(Triangulation_ds& tds, Facet& f)
{ return *(tds.insert_in_facet(f)); }

//!
Full_cell& neighbor(const Triangulation_ds& tds, Full_cell& c, int i)
{ return *(tds.neighbor(Full_cell_handle(&c), i)); }

//!
int mirror_index(const Triangulation_ds& tds, Full_cell& c, int i)
{ return tds.mirror_index(Full_cell_handle(&c), i); }

// int mirror_vertex(const Triangulation_ds& tds, Full_cell& c, int i)
// { return tds.mirror_vertex(Full_cell_handle(&c), i); }

// Bug in CGAL
// Vertex& tds_collapse_face(Triangulation_ds& tds, const Face& f)
// { return *(tds.collapse_face(f)); }

//!
void remove_decrease_dimension(Triangulation_ds& tds, Vertex& v1, Vertex& v2)
{ tds.remove_decrease_dimension(Vertex_handle(&v1), Vertex_handle(&v2)); }

//!
Vertex& tds_insert_in_hole(Triangulation_ds& tds, py::list& full_cells, const Facet f) {
  auto begin = stl_dereference_input_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_input_iterator<Full_cell_handle>(full_cells, false);
  return *(tds.insert_in_hole(begin, end, f));
}

//!
py::list tds_insert_in_hole_get_full_cells(Triangulation_ds& tds, py::list& full_cells, const Facet& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin = stl_dereference_input_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_input_iterator<Full_cell_handle>(full_cells, false);
  py::list final_res;
  final_res.append(*(tds.insert_in_hole(begin, end, f, it)));
  final_res.append(res);
  return final_res;
}

//!
py::list insert_in_tagged_hole(Triangulation_ds& tds, Vertex& v, Facet& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  py::list lst;
  lst.append(*(tds.insert_in_tagged_hole(Vertex_handle(&v), f, it)));
  lst.append(res);
  return lst;
}

//!
Vertex& tds_new_vertex(Triangulation_ds& tds) { return *(tds.new_vertex()); }

//!
Full_cell& tds_new_full_cell(Triangulation_ds& tds)
{ return *(tds.new_full_cell()); }

//!
void delete_vertex(Triangulation_ds& tds, Vertex& v)
{ tds.delete_vertex(Vertex_handle(&v)); }

//!
void delete_full_cell(Triangulation_ds& tds, Full_cell& fc)
{ tds.delete_full_cell(Full_cell_handle(&fc)); }

//!
void delete_full_cells(Triangulation_ds& tds, py::list& full_cells) {
  auto begin = stl_dereference_input_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_input_iterator<Full_cell_handle>(full_cells, false);
  tds.delete_full_cells(begin, end);
}

//!
void associate_vertex_with_full_cell(Triangulation_ds& tds, Full_cell& s, int i, Vertex& v)
{ tds.associate_vertex_with_full_cell(Full_cell_handle(&s), i, Vertex_handle(&v)); }

//!
void tds_set_neighbors(Triangulation_ds& tds, Full_cell& c1, int i1, Full_cell& c2, int i2)
{ tds.set_neighbors(Full_cell_handle(&c1), i1, Full_cell_handle(&c2), i2); }

/// @}

/// Face
/// @{

//!
void set_full_cell(Face& f, Full_cell& fc)
{ f.set_full_cell(Full_cell_handle(&fc)); }

/// @}

/// dD Triangulation
/// @{

//!
const Full_cell& infinite_full_cell(const Triangulation_d& tri)
{ return value(tri.infinite_full_cell()); }

//!
py::list incident_full_cells1(const Triangulation_d& tri, const Face& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.incident_full_cells(f, it);
  return res;
}

//!
py::list incident_full_cells2(const Triangulation_d& tri, const Vertex& v) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.incident_full_cells(Vertex_const_handle(&v), it);
  return res;
}

//!
py::list star(const Triangulation_d& tri, const Face& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.star(f, it);
  return res;
}

//!
py::list incident_faces(const Triangulation_d& tri, const Vertex& v, int d) {
  py::list res;
  auto op = [&] (const Face& f) mutable { res.append(f); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.incident_faces(Vertex_const_handle(&v), d, it);
  return res;
}

//!
Full_cell& new_full_cell(Triangulation_d& tri)
{ return *(tri.new_full_cell()); }

//!
Vertex& new_vertex1(Triangulation_d& tri) { return *(tri.new_vertex()); }

//!
Vertex& new_vertex2(Triangulation_d& tri, const Point& p)
{ return *(tri.new_vertex(p)); }

//!
void set_neighbors(Triangulation_d& tri, Full_cell& s, int i,
                   Full_cell& s1, int j)
{ tri.set_neighbors(Full_cell_handle(&s), i, Full_cell_handle(&s1), j); }

//!
bool are_incident_full_cells_valid(const Triangulation_d& tri,
                                   const Vertex& v, bool verbose, int level) {
  return tri.are_incident_full_cells_valid(Vertex_const_handle(&v), verbose, level);
}

//!
Full_cell& locate1(const Triangulation_d& tri, const Point& p, Locate_type& lt,
                   Face& f1, Facet& f2, Full_cell& start)
{ return *(tri.locate(p, lt, f1, f2, Full_cell_handle(&start))); }

//!
Full_cell& locate2(const Triangulation_d& tri, const Point& p, Locate_type& lt,
                   Face& f, Facet& facet, Vertex& v)
{ return *(tri.locate(p, lt, f, facet, Vertex_handle(&v))); }

//!
Full_cell& locate3(const Triangulation_d& tri, const Point& p, Full_cell& s)
{ return *(tri.locate(p, Full_cell_handle(&s))); }

//!
Full_cell& locate4(const Triangulation_d& tri, const Point& p, Vertex& v)
{ return *(tri.locate(p, Vertex_handle(&v))); }

//!
size_type insert1(Triangulation_d& tri, py::list& points) {
  auto begin = stl_input_iterator<Point>(points);
  auto end = stl_input_iterator<Point>(points, false);
  return tri.insert(begin, end);
}

Vertex& insert2(Triangulation_d& tri, const Point& p, Locate_type lt,
                const Face& f, const Facet& ft, Full_cell& c)
{ return *(tri.insert(p, lt, f, ft, Full_cell_handle(&c))); }

//!
Vertex& insert3(Triangulation_d& tri, const Point& p, Full_cell& hint)
{ return *(tri.insert(p, Full_cell_handle(&hint))); }

//!
Vertex& insert4(Triangulation_d& tri, const Point& p, Vertex& hint)
{ return *(tri.insert(p, Vertex_handle(&hint))); }

//!
Vertex& insert5(Triangulation_d& tri, const Point& p)
{ return *(tri.insert(p)); }

//!
py::list insert_in_hole(Triangulation_d& tri, const Point& p, py::list& full_cells, const Facet& ft) {
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

//!
Vertex& insert_in_hole_get_full_cells(Triangulation_d& tri, const Point& p, py::list& full_cells, const Facet& ft) {
  auto begin = stl_dereference_input_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_input_iterator<Full_cell_handle>(full_cells, false);
  return *(tri.insert_in_hole(p, begin, end, ft));
}

//!
Vertex& insert_in_face(Triangulation_d& tri, const Point& p, const Face& f)
{ return *(tri.insert_in_face(p, f)); }

//!
Vertex& insert_in_facet(Triangulation_d& tri, const Point& p, const Facet& ft)
{ return *(tri.insert_in_facet(p, ft)); }

//!
Vertex& insert_in_full_cell(Triangulation_d& tri, const Point& p, Full_cell& fc)
{ return *(tri.insert_in_full_cell(p, Full_cell_handle(&fc))); }

//!
Vertex& insert_outside_convex_hull(Triangulation_d& tri, const Point& p, Full_cell& fc)
{ return *(tri.insert_outside_convex_hull(p, Full_cell_handle(&fc))); }

//!
Vertex& insert_outside_affine_hull(Triangulation_d& tri, const Point& p)
{ return *(tri.insert_outside_affine_hull(p)); }

//!
py::object finite_vertices(const Triangulation_d& tri)
{ return make_iterator(tri.finite_vertices_begin(), tri.finite_vertices_end()); }

//!
py::object finite_full_cells(const Triangulation_d& tri)
{ return make_iterator(tri.finite_full_cells_begin(), tri.finite_full_cells_end()); }

//!
py::object finite_facets(Triangulation_d& tri)
{ return make_iterator(tri.finite_facets_begin(), tri.finite_facets_end()); }

/// @}

/// Common
/// @{

//!
template <typename Triangulation_>
const Full_cell& full_cell2(const Triangulation_& tri, const Facet& f)
{ return value(tri.full_cell(f)); }

//!
template <typename Triangulation_>
py::object facets(Triangulation_& tri)
{ return make_iterator(tri.facets_begin(), tri.facets_end()); }

//!
template <typename Triangulation_>
py::object full_cells(const Triangulation_& tri)
{ return make_iterator(tri.full_cells_begin(), tri.full_cells_end()); }

//!
template <typename Triangulation_>
py::object vertices(const Triangulation_& tri)
{ return make_iterator(tri.vertices_begin(), tri.vertices_end()); }

/// @}

} // End of namespace trid

//!
void export_triangulation_d(py::module_& m) {
  using Tri = trid::Triangulation_d;
  using Vertex = trid::Vertex;
  using Fc = trid::Full_cell;
  using Facet = trid::Facet;
  using Face = trid::Face;
  using Rotor = trid::Rotor;
  using Tds = trid::Triangulation_ds;
  using Tds_vertex = trid::Tds_vertex;
  using Tds_fc = trid::Tds_full_cell;
  using Point = trid::Point;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Tds>(m, "Triangulation_ds")) {
    py::class_<Tds> tds_c(m, "Triangulation_ds");

    tds_c.def(py::init<int>())
      .def(py::init<const Tds&>())
      .def("associate_vertex_with_full_cell", &trid::associate_vertex_with_full_cell,
           py::arg("c"), py::arg("i"), py::arg("v"),
           "Set the i-th vertex of c to v and, set c as the incident full cell of v")
      .def("clear", &Tds::clear, "Initialize the triangulation to the empty complex")
      // .def("collapse_face", &trid::tds_collapse_face) // bug in CGAL
      .def("current_dimension", &Tds::current_dimension,
           "Obtain the dimension of the full dimensional cells stored in the triangulation\n"
           "Return:\n"
           "  int the dimension\n")
      .def("delete_full_cell", &trid::delete_full_cell, py::arg("c"),
           "Remove a full cell from the triangulation\n"
           "Parameters:\n"
           "  c (Full_cell) the full cell to remove\n")
      .def("delete_full_cells", &trid::delete_full_cells, py::arg("full_cells"),
           "Remove a set of full cells\n"
           "Parameters:\n"
           "  full_cels (list) the set of gull cells to remove\n")
      .def("delete_vertex", &trid::delete_vertex, py::arg("v"),
           "Remove a vertex from the triangulation\n"
           "Parameters:\n"
           "  v (Vertex) the vertex to remove\n")
      .def("empty", &Tds::empty, "Determine whether the triangulation is empty")
      .def("full_cell", &trid::full_cell1, ri, py::arg("v"),
           "Obtain a full cell incident to a vertex\n"
           "Parameters:\n"
           "  v (Vertex) the input vertex\n")
      .def("full_cell", &trid::full_cell2<Tds>, ri, py::arg("f"),
           "Obtain a full cell containing a facet\n"
           "Parameters:\n"
           "  f (Facet) the input facet\n")
      .def("gather_full_cells", &trid::gather_full_cells, ri, py::arg("start"), py::arg("tp"),
           "computes (gathers) a connected set of full cells satisfying a given criterion \n"
           "Parameters:\n"
           "  start (Full_cell): the starting point of the recursive traversal of adjacent full cells\n"
           "  tp (Callable): a predicate that accepts a facet implementing the criterion\n"
           "Return:\n"
           "  tuple [Facet, list] the 1st element is a facet on the boundary of the set of cells, and the 2nd is a list of full cells that satisfy the criterion\n")
      .def("incident_full_cells", &trid::tds_incident_full_cells1, py::arg("f"),
           "Obtain all the full cells that are incident to a face\n"
           "Parameters:\n"
           "  f (Facet): the input facet\n"
           "Return:\n"
           "  list: the full cells incident to f\n")
      .def("incident_full_cells", &trid::tds_incident_full_cells2, py::arg("v"),
           "Obtain all the full cells that are incident to a vertex\n"
           "Parameters:\n"
           "  v (Vertex): the input vertex\n"
           "Return:\n"
           "  list: the full cells incident to v\n")
      .def("index_of_covertex", &Tds::index_of_covertex<Facet>, py::arg("f"),
           "Obtain the index of the vertex of a full cell containing a facet, which does not belong to the full cell\n"
           "Parameters:\n"
           "  f (Facet) the input facet\n"
           "Return:\n"
           "  int: the index of the vertex\n")

      // Insertions
      .def("insert_in_face", &trid::tds_insert_in_face, ri, py::arg("f"),
           "Inserts a new vertex into the triangulation data structure by subdividing a face\n"
           "Parameters:\n"
           "  f (Face) the input face\n"
           "Return:\n"
           "  Vertex: the new vertex\n")
      .def("insert_in_facet", &trid::tds_insert_in_facet, ri, py::arg("f"),
           "Inserts a new vertex into the triangulation data structure by subdividing a facet\n"
           "Parameters:\n"
           "  f (Facet) the input facet\n"
           "Return:\n"
           "  Vertex: the new vertex\n")
      .def("insert_in_full_cell", &trid::tds_insert_in_full_cell, ri, py::arg("c"),
           "Insert a new vertex in a full cell into the triangulation data structure\n"
           "Parameters:\n"
           "  c (Full_cell) the input full cell\n"
           "Return:\n"
           "  Vertex: the new vertex\n")
      .def("insert_in_hole", &trid::tds_insert_in_hole, ri, py::arg("full_cells"), py::arg("f"),
           "Remove a set of full cells, insert a new vertex, and connect each face on the boundary of a given set of cells to the new vertex\n"
           "Parameters:\n"
           "  full_cells (list): the input full cells\n"
           "  f (Facet): the input facet\n"
           "Return:\n"
           "  Vertex: the new vertex\n")
      .def("insert_in_hole_get_full_cells", &trid::tds_insert_in_hole_get_full_cells, ri, py::arg("full_cells"), py::arg("f"),
           "Remove a set of full cells, insert a new vertex, and connect each face on the boundary of a given set of cells to the new vertex\n"
           "Parameters:\n"
           "  full_cells (list): the input full cells\n"
           "  f (Facet): the input facet\n"
           "Return:\n"
           "  tuple [Vertex, list]: the 1st element is the new vertex; the 2nd is a list of the newly created full cells\n")
      .def("insert_increase_dimension", &trid::insert_increase_dimension1, ri, py::arg("star"),
           "Add a new vertex, therby transform a triangulation of the sphere 𝕊^d into the triangulation of 𝕊^{d+1}\n",
           "Parameters:\n"
           "  star (Vertex) the anchor vertex for triangulating one hemi sphere\n"
           "Return:\n"
           "  Vertex: the new vertex\n")
      .def("insert_increase_dimension", &trid::insert_increase_dimension2, ri)
           .def("is_full_cell", &trid::vertex_is_full_cell, py::arg("c"),
                "Determine whether a full cell is part of the triangulation\n"
                "Parameters:\n"
                "  c (Full_cell): the input full cell\n"
                "Return:\n"
                "  bool\n")

      .def("is_valid", &Tds::is_valid, py::arg("verbose") = true, py::arg("level") = 0,
           "Partially determine whether the triangulation is valid\n"
           "Parameters:\n"
           "  verbos (bool) indicates whether to be verbose\n"
           "  level (int) the verbosity level\n"
           "Return:\n"
           "  bool\n")
      .def("is_vertex", &trid::vertex_is_vertex, py::arg("v"),
           "Deteremines whether a given vertex is in the triangulation\n"
           "Parameters:\n"
           "  v (Vertex): the input vertex\n"
           "Return:\n"
           "  bool\n")
      .def("maximal_dimension", &Tds::maximal_dimension,
           "Obtain the maximal dimension of the full dimensional cells that can be stored in the triangulation\n"
            "Return:\n"
           "  int\n")
      .def("mirror_index", &trid::mirror_index, py::arg("c"), py::arg("i"),
           "Obtain the index of a full cell as a neighbor of its i-th neighbor\n"
           "Parameters:\n"
           "  c (Full_cell): the input full cell\n"
           "  i (int) the input index\n"
           "Return:\n"
           "  int\n")
      // .def("mirror_vertex", &trid::mirror_vertex) // bug in CGAL
      .def("neighbor", &trid::neighbor, ri, py::arg("c"), py::arg("i"),
           "Obtain a full cell opposite to the i-th vertex of a given full cell\n"
           "Parameters:\n"
           "  c (Full_cell): the input full cell\n"
           "  i (int) the input index\n"
           "Return:\n"
           "  Full_cell: the neighbor full cell\n")
      .def("new_full_cell", &trid::tds_new_full_cell, ri,
           "Inserts a new full cell into the triangulation\n"
           "Return:\n"
           "  c (Full_cell): the new full cell\n")
      .def("new_vertex", &trid::tds_new_vertex, ri,
           "Inserts a new vertex into the triangulation\n"
           "Return:\n"
           "  v (Vertex): the new vertex\n")
      .def("number_of_full_cells", &Tds::number_of_full_cells,
           "Obtain the number of full cells in the triangulation\n"
           "Return:\n"
           "  int\n")
      .def("number_of_vertices", &Tds::number_of_vertices,
           "Obtain the number of vertices in the triangulation\n"
           "Return:\n"
           "  int\n")
      .def("remove_decrease_dimension", &trid::remove_decrease_dimension, py::arg("v"), py::arg("star"),
           "Do exactly the opposite of insert_increase_dimension()\n"
           "Parameters:\n"
           "  v (Vertex)\n"
           "  star (Vertex)\n")
      .def("set_current_dimension", &Tds::set_current_dimension,
           "Forces the current dimension of the complex to to a given one\n"
           "Parameters:\n"
           "  d (int): the new dimension\n")
      .def("set_neighbors", &trid::tds_set_neighbors, py::arg("c1"), py::arg("i1"), py::arg("c2"), py::arg("i2"),
           "Sets the neighbor opposite to vertex i1 of Full_cell c1 to c2\n"
           "Sets the neighbor opposite to vertex i2 of Full_cell c2 to c1\n"
           "Parameters:\n"
           "  c1 (Full_Cell)\n"
           "  i1 (int)\n"
           "  c2 (Full_Cell)\n"
           "  i2 (int)\n")
      .def("star", &trid::tds_star, ri, py::arg("f"),
           "Obtain all the full cells that share at least one vertex with a face"
           "Parameters:\n"
           "  f (Face): the input face\n")
      .def("vertex", &trid::vertex, ri, py::arg("c"), py::arg("i"),
           "Obtain the i-th vertex of a full cell\n"
           "Parameters:\n"
           "  c (Full_cell): the input full cell\n"
           "  i (int) the input index\n"
           "Return:\n"
           "  Vertex: the new vertex\n")

      // Non concept
      .def("insert_in_tagged_hole", &trid::insert_in_tagged_hole, ri, py::arg("v"), py::arg("f"),
           "Connect each face on the boundary of a full cell c to a vertex\n"
           "Parameters:\n"
           "  v (Vertex) the input vertex\n"
           "  f (Facet) the facet contained by the full cell c\n"
           "Return:\n"
           "  list: a list of the newly created full cells\n")

      // .def("incident_upper_faces", &trid::tds_incident_upper_faces) // not implemented yet
      // .def("incident_faces", &trid::tds_incident_faces) // not implemented yet
      // read_full_cells
      // write_full_cells
      ;

    // Iterators
    using Vci = Tri::Vertex_const_iterator;
    using Fvci = Tri::Finite_vertex_const_iterator;
    using Fcci = Tri::Full_cell_const_iterator;
    using Ffcci = Tri::Finite_full_cell_const_iterator;
    using Fi = Tri::Facet_iterator;
    using Ffi = Tri::Finite_facet_iterator;

    add_iterator<Fi, Fi, const Facet&>("Facet_iterator", tds_c);
    add_iterator<Fcci, Fcci, const Fc&>("Full_cell_iterator", tds_c);
    add_iterator<Vci, Vci, const Vertex&>("Vertex_iterator", tds_c);

    tds_c
      .def("facets", &trid::facets<Tds>, py::keep_alive<0, 1>(),
           "Obtain an iterator that traverses all facets")
      .def("full_cells", &trid::full_cells<Tds>, py::keep_alive<0, 1>(),
           "Obtain an iterator that traverses all full cells")
      .def("vertices", &trid::vertices<Tds>, py::keep_alive<0, 1>(),
           "Obtain an iterator that traverses all vertices")
      ;
  }

  // Triangulation
  if (! add_attr<Tri>(m, "Triangulation")) {
    py::class_<Tri> tri_c(m, "Triangulation");
    tri_c.def(py::init<int, const trid::Traits&>())
      .def(py::init<const Tri&>())
      .def("are_incident_full_cells_valid", &trid::are_incident_full_cells_valid,
           py::arg("vertex"), py::arg("verbose") = false, py::arg("level") = 0,
           "Determine whether all finite full cells incident to v have positive orientation")
      .def("clear", &Tri::clear, "Initialize the triangulation to the empty complex")
      // .def("collapse_face", &trid::tds_collapse_face) // bug in CGAL
      .def("current_dimension", &Tri::current_dimension,
           "Obtain the dimension of the full dimensional cells stored in the triangulation")
      .def("empty", &Tri::empty, "Determine whether triangulation is empty")
      .def("full_cell", &trid::full_cell2<Tri>, ri, py::arg("f"),
           "Obtain a full cell containing a facet\n"
           "Parameters:\n"
           "  f (Facet) the input facet\n")
      .def("geom_traits", &Tri::geom_traits, ri, "Obtain the geometric traits")
      .def("index_of_covertex", &Tri::index_of_covertex, py::arg("f"),
           "Obtain the index of the vertex of the full cell c containing f, which does not belong to c")
      .def("infinite_full_cell", &trid::infinite_full_cell, ri,
           "Obtain a full cell incident to the vertex at infinity\n"
           "Return:\n"
           "  Full_cell\n")

      // Insertion
      .def("insert", &trid::insert1, py::arg("points"),
           "Insert a set of points into the triangulation\n"
           "Parameters:\n"
           "  points (list): the input points\n"
           "Return:\n"
           "  Vertex: the newly created vertex\n")
      .def("insert", &trid::insert2, py::arg("p"), py::arg("lt"), py::arg("f"), py::arg("ft"), py::arg("c"),
           "Insert a point into the triangulation according to a given location type\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "  lt (Location_type): the location type\n"
           "  f (Face)\n"
           "  ft (Facet)\n"
           "  c (Full_cell)\n"
           "Return:\n"
           "  Vertex: the newly created vertex\n")
      .def("insert", &trid::insert3, py::arg("p"), py::arg("hint"),
           "Insert a point into the triangulation, using a hint for its location\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "  hint (Full_cell): the starting place of the search for the point location\n"
           "Return:\n"
           "  Vertex: the newly created vertex\n")
      .def("insert", &trid::insert4, py::arg("p"), py::arg("hint"),
           "Insert a point into the triangulation, using a hint for its location\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "  hint (Vertex): the starting place of the search for the point location\n"
           "Return:\n"
           "  Vertex: the newly created vertex\n")
      .def("insert", &trid::insert5, py::arg("p"),
           "Insert a point into the triangulation\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "Return:\n"
           "  Vertex: the newly created vertex\n")
      .def("insert_in_face", &trid::insert_in_face, py::arg("p"), py::arg("f"),
           "Inserts a point into the triangulation in a given face\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "  f (Face) the containing face\n")
      .def("insert_in_facet", &trid::insert_in_facet, py::arg("p"), py::arg("f"),
           "Inserts a point into the triangulation in a given facet\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "  f (Facet) the containing facet\n")
      .def("insert_in_full_cell", &trid::insert_in_full_cell, py::arg("p"), py::arg("c"),
           "Inserts a point into the triangulation in a given full cell\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "  c (Full_cell) the containing full cell\n")
      .def("insert_in_hole", &trid::insert_in_hole,  py::arg("p"),  py::arg("full_cells"),  py::arg("f"),
           "Remove a set of full cells, insert a new vertex at a given point, and connect each face on the boundary of a given set of cells to the new vertex\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "  full_cells (list): the input full cells\n"
           "  f (Facet): the input facet\n"
           "Return:\n"
           "  Vertex: the new vertex\n")
      .def("insert_in_hole_get_full_cells", &trid::insert_in_hole_get_full_cells, py::arg("p"),
           py::arg("full_cells"), py::arg("f"),
           "Remove a set of full cells, insert a new vertex at a given point, and connect each face on the boundary of a given set of cells to the new vertex\n"
           "Parameters:\n"
           "  p (Point): the point to insert\n"
           "  full_cells (list): the input full cells\n"
           "  f (Facet): the input facet\n"
           "Return:\n"
           "  tuple [Vertex, list]: the 1st element is the new vertex; the 2nd is a list of the newly created full cells\n")
      .def("insert_outside_convex_hull", &trid::insert_outside_convex_hull, py::arg("p"), py::arg("c"),
           "Insert a point that outside of the convex hull of the triangulation")
      .def("insert_outside_affine_hull", &trid::insert_outside_affine_hull, py::arg("p"),
           "Insert a point that outside of the affine hull of the triangulation")

      // .def("rotate_rotor", &Tri::rotate_rotor)
      .def("tds", py::overload_cast<>(&Tri::tds, py::const_),
           "Obtain the underlying triangulation data structure\n"
           "Return:\n"
           "  Traiangulation_ds\n")
      .def("maximal_dimension", &Tri::maximal_dimension)
      .def("number_of_vertices", &Tri::number_of_vertices)
      .def("number_of_full_cells", &Tri::number_of_full_cells)
      .def("infinite_vertex", [](Tri& tri)->Vertex& { return *(tri.infinite_vertex()); }, ri)
      .def("number_of_finite_full_cells", &Tri::number_of_finite_full_cells)
      .def("is_infinite", py::overload_cast<const Vertex&>(&Tri::is_infinite, py::const_))
      .def("is_infinite", py::overload_cast<const Fc&>(&Tri::is_infinite, py::const_))
      .def("is_infinite", py::overload_cast<const Facet&>(&Tri::is_infinite, py::const_))
      .def("is_infinite", py::overload_cast<const Face&>(&Tri::is_infinite, py::const_))
      .def("incident_full_cells", &trid::incident_full_cells1)
      .def("incident_full_cells", &trid::incident_full_cells2)
      .def("star", &trid::star)
      .def("incident_faces", &trid::incident_faces)
      .def("orientation", &Tri::orientation)
      .def("set_current_dimension", &Tri::set_current_dimension)
      .def("new_full_cell", &trid::new_full_cell, ri)
      .def("new_vertex", &trid::new_vertex1, ri)
      .def("new_vertex", &trid::new_vertex2, ri)
      .def("set_neighbors", &trid::set_neighbors)
      .def("is_valid", &Tri::is_valid, py::arg("verbos") = false, py::arg("level") = 0)
      .def("locate", &trid::locate1)
      .def("locate", &trid::locate2)
      .def("locate", &trid::locate3)
      .def("locate", &trid::locate4)
      // .def("contract_face",
      //      [](Tri& tri, const Point& p, const Face& f)->Vertex&
      //      { return *(tri.contract_face(p, f)); }, ri)

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

    export_trid_vertex(tri_c);
    export_trid_full_cell(tri_c);

    // Facet
    if (! add_attr<Facet>(tri_c, "Facet")) {
      py::class_<Facet>(tri_c, "Facet")
        .def("full_cell", [](const Facet& f) { return *(f.full_cell()); }, ri)
        .def("index_of_covertex", &Facet::index_of_covertex)
        ;
    }

    // Face
    if (! add_attr<Face>(tri_c, "Face")) {
      py::class_<Face>(tri_c, "Face")
        .def(py::init<int>())
        .def(py::init<const Face&>())
        .def("face_dimension", &Face::face_dimension)
        .def("index", &Face::index)
        .def("vertex", [](Face& f, int i)->Vertex& { return *(f.vertex(i)); }, ri)
        .def("Full_cell", [](Face& f)->Fc& { return *(f.full_cell()); }, ri)
        .def("clear", &Face::clear)
        .def("set_full_cell", trid::set_full_cell)
        .def("set_index", &Face::set_index)
        ;
    }

    add_attr<trid::Geom_traits>(tri_c, "Geom_traits");

    // using Pi = Tri::Point_iterator;
    // add_iterator<Pi, Pi, const Point&>("Point_iterator", tri_c);
    using Vci = Tri::Vertex_const_iterator;
    using Fvci = Tri::Finite_vertex_const_iterator;
    using Fcci = Tri::Full_cell_const_iterator;
    using Ffcci = Tri::Finite_full_cell_const_iterator;
    using Fi = Tri::Facet_iterator;
    using Ffi = Tri::Finite_facet_iterator;

    add_iterator<Vci, Vci, const Vertex&>("Vertex_iterator", tri_c);
    add_iterator<Fvci, Fvci, const Vertex&>("Finite_vertex_iterator", tri_c);
    add_iterator<Fcci, Fcci, const Fc&>("Full_cell_iterator", tri_c);
    add_iterator<Ffcci, Ffcci, const Fc&>("Finite_full_cell_iterator", tri_c);
    add_iterator<Fi, Fi, const Facet&>("Facet_iterator", tri_c);
    add_iterator<Ffi, Ffi, const Facet&>("Finite_facet_iterator", tri_c);

    tri_c
      .def("facets", &trid::facets<Tri>, py::keep_alive<0, 1>())
      .def("full_cells", &trid::full_cells<Tri>, py::keep_alive<0, 1>())
      .def("vertices", &trid::vertices<Tri>, py::keep_alive<0, 1>())
      .def("finite_facets", &trid::finite_facets, py::keep_alive<0, 1>())
      .def("finite_full_cells", &trid::finite_full_cells, py::keep_alive<0, 1>())
      .def("finite_vertices", &trid::finite_vertices, py::keep_alive<0, 1>())
      ;

#if CGALPY_TRID == CGALPY_TRID_REGULAR
    using Rtri = trid::Regular_triangulation;

    if (! add_attr<Rtri>(m, "Regular_triangulation")) {
      py::class_<Rtri, Tri> rtri_c(m, "Regular_triangulation");
    }
#endif

#if CGALPY_TRID == CGALPY_TRID_DELAUNAY
    using Dtri = trid::Delaunay_triangulation;

    if (! add_attr<Dtri>(m, "Delaunay_triangulation")) {
      py::class_<Dtri, Tri> dtri_c(m, "Delaunay_triangulation");

      dtri_c.def(py::init<int, const trid::Traits&>(), py::arg("dim"), py::arg("traits"))
        ;
    }
#endif
  }
}
