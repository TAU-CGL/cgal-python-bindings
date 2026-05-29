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
#include "CGALPY/stl_forward_iterator.hpp"
#include "CGALPY/stl_dereference_forward_iterator.hpp"
#include "CGALPY/types.hpp"
#include "cgalpy/Trid_docstrings.hpp"

namespace trid_doc = cgalpy::trid::docstrings;

namespace py = nanobind;

void export_trid_vertex(py::class_<cgalpy::trid::Triangulation_d>&);
void export_trid_full_cell(py::class_<cgalpy::trid::Triangulation_d>&);

namespace cgalpy {
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
  auto begin = stl_dereference_forward_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_forward_iterator<Full_cell_handle>(full_cells, false);
  return *(tds.insert_in_hole(begin, end, f));
}

//!
py::list tds_insert_in_hole_get_full_cells(Triangulation_ds& tds, py::list& full_cells, const Facet& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin = stl_dereference_forward_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_forward_iterator<Full_cell_handle>(full_cells, false);
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
  auto begin = stl_dereference_forward_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_forward_iterator<Full_cell_handle>(full_cells, false);
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
Full_cell& locate1(const Triangulation_d& tri, const Point& p)
{ return *(tri.locate(p)); }

//!
Full_cell& locate2(const Triangulation_d& tri, const Point& p, Vertex& v)
{ return *(tri.locate(p, Vertex_handle(&v))); }

//!
Full_cell& locate3(const Triangulation_d& tri, const Point& p, Full_cell& s)
{ return *(tri.locate(p, Full_cell_handle(&s))); }

//!
py::object locate_dispatch(py::handle self, Full_cell_handle ch, Locate_type lt,
                           const Face& face, const Facet& facet) {
  constexpr auto ri(py::rv_policy::reference_internal);

  Full_cell& c = *ch;
  switch (lt) {
   case Locate_type::IN_FACE: return py::make_tuple(py::cast(lt), py::cast(c, ri, self), py::cast(face));
   case Locate_type::IN_FACET: return py::make_tuple(py::cast(lt), py::cast(c, ri, self), py::cast(facet));
   case Locate_type::ON_VERTEX:
   case Locate_type::IN_FULL_CELL:
   case Locate_type::OUTSIDE_CONVEX_HULL:
   case Locate_type::OUTSIDE_AFFINE_HULL: return py::make_tuple(py::cast(lt), py::cast(c, ri, self));
  }

  return py::make_tuple(py::cast(lt));
}

//!
auto locate4(py::handle self, const Point& p) {
  auto& tri = py::cast<Triangulation_d&>(self);
  Locate_type lt;
  Face face(tri.maximal_dimension());
  Facet facet;
  auto fch = tri.locate(p, lt, face, facet);
  return locate_dispatch(self, fch, lt, face, facet);
}

//!
auto locate5(py::handle self, const Point& p, Vertex& v) {
  auto& tri = py::cast<Triangulation_d&>(self);
  Locate_type lt;
  Face face(tri.maximal_dimension());
  Facet facet;
  auto fch = tri.locate(p, lt, face, facet, Vertex_handle(&v));
  return locate_dispatch(self, fch, lt, face, facet);
}

//!
auto locate6(py::handle self, const Point& p, Full_cell& c) {
  auto& tri = py::cast<Triangulation_d&>(self);
  Locate_type lt;
  Face face(tri.maximal_dimension());  // There should be an empty constructor
  Facet facet;
  auto fch = tri.locate(p, lt, face, facet, Full_cell_handle(&c));
  return locate_dispatch(self, fch, lt, face, facet);
}

//!
py::list insert_in_hole(Triangulation_d& tri, const Point& p, py::list& full_cells, const Facet& ft) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin = stl_dereference_forward_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_forward_iterator<Full_cell_handle>(full_cells, false);
  py::list final_res;
  final_res.append(*(tri.insert_in_hole(p, begin, end, ft, it)));
  final_res.append(res);
  return final_res;
}

//!
Vertex& insert_in_hole_get_full_cells(Triangulation_d& tri, const Point& p, py::list& full_cells, const Facet& ft) {
  auto begin = stl_dereference_forward_iterator<Full_cell_handle>(full_cells);
  auto end = stl_dereference_forward_iterator<Full_cell_handle>(full_cells, false);
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

//!
const Full_cell& infinite_full_cell(const Triangulation_d& tri)
{ return value(tri.infinite_full_cell()); }

//!
const Vertex& infinite_vertex(const Triangulation_d& tri)
{ return value(tri.infinite_vertex()); }

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
py::list incident_faces(const Triangulation_& tri, const Vertex& v, int dim) {
  py::list res;
  auto op = [&] (const Face& f) mutable { res.append(f); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.incident_faces(Vertex_const_handle(&v), dim, it);
  return res;
}

//!
template <typename Triangulation_>
py::list incident_full_cells1(py::handle self, const Face& f) {
  auto& tri = py::cast<Triangulation_&>(self);
  py::list res;
  constexpr auto ri(py::rv_policy::reference_internal);
  auto op = [&] (Full_cell_handle ch) mutable { res.append(py::cast(*ch,ri, self)); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.incident_full_cells(f, it);
  return res;
}

//!
template <typename Triangulation_>
py::list incident_full_cells2(py::handle self, const Vertex& v) {
  auto& tri = py::cast<Triangulation_&>(self);
  py::list res;
  constexpr auto ri(py::rv_policy::reference_internal);
  auto op = [&] (Full_cell_handle ch) mutable { res.append(py::cast(*ch, ri, self)); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.incident_full_cells(Vertex_const_handle(&v), it);
  return res;
}

//!
template <typename Triangulation_>
size_type insert1(Triangulation_& tri, py::list& points) {
  auto begin = stl_forward_iterator<Point>(points);
  auto end = stl_forward_iterator<Point>(points, false);
  return tri.insert(begin, end);
}

//!
template <typename Triangulation_>
Vertex& insert2(Triangulation_& tri, const Point& p, Locate_type lt,
                const Face& f, const Facet& ft, Full_cell& c)
{ return *(tri.insert(p, lt, f, ft, Full_cell_handle(&c))); }

//!
template <typename Triangulation_>
Vertex& insert3(Triangulation_& tri, const Point& p, Full_cell& hint)
{ return *(tri.insert(p, Full_cell_handle(&hint))); }

//!
template <typename Triangulation_>
Vertex& insert4(Triangulation_& tri, const Point& p, Vertex& hint)
{ return *(tri.insert(p, Vertex_handle(&hint))); }

//!
template <typename Triangulation_>
Vertex& insert5(Triangulation_& tri, const Point& p)
{ return *(tri.insert(p)); }

//!
template <typename Triangulation_>
py::list star(const Triangulation_& tri, const Face& f) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  tri.star(f, it);
  return res;
}

//!
template <typename Triangulation_>
py::object vertices(const Triangulation_& tri)
{ return make_iterator(tri.vertices_begin(), tri.vertices_end()); }

/// @}

/// Regular triangulation
/// @{

#if CGALPY_TRID == CGALPY_TRID_REGULAR

//!
Vertex& insert_if_in_star1(Regular_triangulation_d& rtri, const Weighted_point& p, Vertex& star_center) {
  auto v = rtri.insert_if_in_star(p, Vertex_handle(&star_center));
  return *v;
}

//!
Vertex& insert_if_in_star2(Regular_triangulation_d& rtri, const Weighted_point& p, Vertex& star_center,
                           Full_cell& start) {
  auto v = rtri.insert_if_in_star(p, Vertex_handle(&star_center), Full_cell_handle(&start));
  return *v;
}

//!
Vertex& insert_if_in_star3(Regular_triangulation_d& rtri, const Weighted_point& p, Vertex& star_center,
                           Vertex& hint) {
  auto v = rtri.insert_if_in_star(p, Vertex_handle(&star_center), Vertex_handle(&hint));
  return *v;
}

//!
auto insert_if_in_star4(Regular_triangulation_d& rtri, const Weighted_point& p, Vertex& star_center,
                        Full_cell& start) {
  Locate_type lt;
  Face face(rtri.maximal_dimension());
  Facet facet;
  auto res = rtri.insert_if_in_star(p, Vertex_handle(&star_center), lt, face, facet, Full_cell_handle(&start));
  switch (lt) {
   case Locate_type::ON_VERTEX: return py::make_tuple(*res, lt);
   case Locate_type::IN_FACE: return py::make_tuple(*res, lt, face);
   case Locate_type::IN_FACET: return py::make_tuple(*res, lt, facet);
   case Locate_type::IN_FULL_CELL: py::make_tuple(*res, lt);
   case Locate_type::OUTSIDE_CONVEX_HULL: py::make_tuple(*res, lt);
  }
  return py::make_tuple();
}

#endif

/// @}

/// Delaunay triangulation
/// @{

#if (CGALPY_TRID == CGALPY_TRID_DELAUNAY)

//!
void remove1(Delaunay_triangulation_d& dtri, py::list vertices) {
  auto begin = stl_dereference_forward_iterator<Vertex_handle>(vertices);
  auto end = stl_dereference_forward_iterator<Vertex_handle>(vertices, false);
  dtri.remove(begin, end);
}

//!
Full_cell& remove2(Delaunay_triangulation_d& dtri, Vertex& v) {
  auto c = dtri.remove(Vertex_handle(&v));
  return *c;
}

#endif

/// @}

/// Common to Regular and Delaunay triangulation
/// @{

#if ((CGALPY_TRID == CGALPY_TRID_REGULAR) || (CGALPY_TRID == CGALPY_TRID_DELAUNAY))

  //!
template <typename Triangulation_>
auto compute_conflict_zone(const Triangulation_& rtri, const Point& p, Full_cell& c) {
  py::list res;
  auto op = [&] (const Full_cell_handle& c) mutable { res.append(&c); };
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto facet = rtri.compute_conflict_zone(p, Full_cell_handle(&c), it);
  return py::make_tuple(facet, res);
}

//!
template <typename Triangulation_>
bool is_in_conflict(const Triangulation_& rtri, const Point& p, const Full_cell& c)
{ return rtri.is_in_conflict(p, Full_cell_const_handle(&c)); }

/// @}

#endif

}
} // namespace cgalpy // End of namespace trid

//!
void export_triangulation_d(py::module_& m) {
  using Tri = cgalpy::trid::Triangulation_d;
  using Vertex = cgalpy::trid::Vertex;
  using Fc = cgalpy::trid::Full_cell;
  using Facet = cgalpy::trid::Facet;
  using Face = cgalpy::trid::Face;
  using Rotor = cgalpy::trid::Rotor;
  using Tds = cgalpy::trid::Triangulation_ds;
  using Tds_vertex = cgalpy::trid::Tds_vertex;
  using Tds_fc = cgalpy::trid::Tds_full_cell;
  using Point = cgalpy::trid::Point;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (! add_attr<Tds>(m, "Triangulation_ds")) {
    py::class_<Tds> tds_c(m, "Triangulation_ds",
                         trid_doc::Triangulation_data_structure_class);

    tds_c.def(py::init<int>(), py::arg("dim") = 0,
              trid_doc::TriangulationDataStructure_TriangulationDataStructure)
      .def(py::init<const Tds&>(), py::arg("other"),
           trid_doc::Triangulation_data_structure_Triangulation_data_structure)
      .def("associate_vertex_with_full_cell", &cgalpy::trid::associate_vertex_with_full_cell,
           py::arg("c"), py::arg("i"), py::arg("v"),
           trid_doc::TriangulationDataStructure_associate_vertex_with_full_cell)
      .def("clear", &Tds::clear,
           trid_doc::TriangulationDataStructure_clear)
      // .def("collapse_face", &cgalpy::trid::tds_collapse_face) // bug in CGAL
      .def("current_dimension", &Tds::current_dimension,
           trid_doc::TriangulationDataStructure_current_dimension)
      .def("delete_full_cell", &cgalpy::trid::delete_full_cell, py::arg("c"),
           trid_doc::TriangulationDataStructure_delete_full_cell)
      .def("delete_full_cells", &cgalpy::trid::delete_full_cells, py::arg("full_cells"),
           trid_doc::TriangulationDataStructure_delete_full_cells)
      .def("delete_vertex", &cgalpy::trid::delete_vertex, py::arg("v"),
           trid_doc::TriangulationDataStructure_delete_vertex)
      .def("empty", &Tds::empty,
           trid_doc::TriangulationDataStructure_empty)
      .def("full_cell", &cgalpy::trid::full_cell1, ri, py::arg("v"),
           trid_doc::TriangulationDataStructure_full_cell)
      .def("full_cell", &cgalpy::trid::full_cell2<Tds>, ri, py::arg("f"),
           trid_doc::TriangulationDataStructure_full_cell_1)
      .def("gather_full_cells", &cgalpy::trid::gather_full_cells, ri,
           py::arg("start"), py::arg("tp"),
           trid_doc::TriangulationDataStructure_gather_full_cells)
      .def("incident_faces", &cgalpy::trid::incident_faces<Tds>, ri,
           py::arg("v"), py::arg("dim"),
           trid_doc::TriangulationDataStructure_incident_faces)
      .def("incident_full_cells", &cgalpy::trid::incident_full_cells1<Tds>, ri,
           py::arg("f"),
           trid_doc::TriangulationDataStructure_incident_full_cells)
      .def("incident_full_cells", &cgalpy::trid::incident_full_cells2<Tds>, ri,
           py::arg("v"),
           trid_doc::TriangulationDataStructure_incident_full_cells_1)
      .def("index_of_covertex", &Tds::index_of_covertex<Facet>, py::arg("f"),
           trid_doc::TriangulationDataStructure_index_of_covertex)

      // Insertions
      .def("insert_in_face", &cgalpy::trid::tds_insert_in_face, ri,
           py::arg("f"),
           trid_doc::TriangulationDataStructure_insert_in_face)
      .def("insert_in_facet", &cgalpy::trid::tds_insert_in_facet, ri,
           py::arg("f"),
           trid_doc::TriangulationDataStructure_insert_in_facet)
      .def("insert_in_full_cell", &cgalpy::trid::tds_insert_in_full_cell, ri,
           py::arg("c"),
           trid_doc::TriangulationDataStructure_insert_in_full_cell)
      .def("insert_in_hole", &cgalpy::trid::tds_insert_in_hole, ri,
           py::arg("full_cells"), py::arg("f"),
           trid_doc::TriangulationDataStructure_insert_in_hole)
      .def("insert_in_hole_get_full_cells", &cgalpy::trid::tds_insert_in_hole_get_full_cells, ri,
           py::arg("full_cells"), py::arg("f"),
           trid_doc::TriangulationDataStructure_insert_in_hole_1)
      .def("insert_increase_dimension", &cgalpy::trid::insert_increase_dimension1, ri,
           py::arg("star"),
           trid_doc::TriangulationDataStructure_insert_increase_dimension)
      .def("insert_increase_dimension", &cgalpy::trid::insert_increase_dimension2, ri,
           trid_doc::TriangulationDataStructure_insert_increase_dimension)
      .def("is_full_cell", &cgalpy::trid::vertex_is_full_cell, py::arg("c"),
           trid_doc::TriangulationDataStructure_is_full_cell)

      .def("is_valid", &Tds::is_valid,
           py::arg("verbose") = true, py::arg("level") = 0,
           trid_doc::TriangulationDataStructure_is_valid)
      .def("is_vertex", &cgalpy::trid::vertex_is_vertex, py::arg("v"),
           trid_doc::TriangulationDataStructure_is_vertex)
      .def("maximal_dimension", &Tds::maximal_dimension,
           trid_doc::TriangulationDataStructure_maximal_dimension)
      .def("mirror_index", &cgalpy::trid::mirror_index,
           py::arg("c"), py::arg("i"),
           trid_doc::TriangulationDataStructure_mirror_index)
      // .def("mirror_vertex", &cgalpy::trid::mirror_vertex) // bug in CGAL
      .def("neighbor", &cgalpy::trid::neighbor, ri,
           py::arg("c"), py::arg("i"),
           trid_doc::TriangulationDataStructure_neighbor)
      .def("new_full_cell", &cgalpy::trid::tds_new_full_cell, ri,
           trid_doc::TriangulationDataStructure_new_full_cell)
      .def("new_vertex", &cgalpy::trid::tds_new_vertex, ri,
           trid_doc::TriangulationDataStructure_new_vertex)
      .def("number_of_full_cells", &Tds::number_of_full_cells,
           trid_doc::TriangulationDataStructure_number_of_full_cells)
      .def("number_of_vertices", &Tds::number_of_vertices,
           trid_doc::TriangulationDataStructure_number_of_vertices)
      .def("remove_decrease_dimension", &cgalpy::trid::remove_decrease_dimension,
           py::arg("v"), py::arg("star"),
           trid_doc::TriangulationDataStructure_remove_decrease_dimension)
      .def("set_current_dimension", &Tds::set_current_dimension, py::arg("d"),
           trid_doc::TriangulationDataStructure_set_current_dimension)
      .def("set_neighbors", &cgalpy::trid::tds_set_neighbors,
           py::arg("c1"), py::arg("i1"), py::arg("c2"), py::arg("i2"),
           trid_doc::TriangulationDataStructure_set_neighbors)
      .def("star", &cgalpy::trid::star<Tds>, ri, py::arg("f"),
           trid_doc::TriangulationDataStructure_star)
      .def("vertex", &cgalpy::trid::vertex, ri,
           py::arg("c"), py::arg("i"),
           trid_doc::TriangulationDataStructure_vertex)

      // Non concept
      .def("insert_in_tagged_hole", &cgalpy::trid::insert_in_tagged_hole, ri,
           py::arg("v"), py::arg("f"),
           trid_doc::Triangulation_data_structure_insert_in_tagged_hole)
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
      .def("facets", &cgalpy::trid::facets<Tds>, py::keep_alive<0, 1>(),
           trid_doc::TriangulationDataStructure_facets_begin)
      .def("full_cells", &cgalpy::trid::full_cells<Tds>, py::keep_alive<0, 1>(),
           trid_doc::TriangulationDataStructure_full_cells_begin)
      .def("vertices", &cgalpy::trid::vertices<Tds>, py::keep_alive<0, 1>(),
           trid_doc::TriangulationDataStructure_vertices_begin)
      ;
  }

  // Triangulation
  if (! add_attr<Tri>(m, "Triangulation")) {
    py::class_<Tri> tri_c(m, "Triangulation",
                         trid_doc::Triangulation_class);
    tri_c
      .def(py::init<int>(), py::arg("dim") = 0,
           trid_doc::Triangulation_Triangulation)
      .def(py::init<int, const cgalpy::trid::Geom_traits&>(),
           py::arg("dim") = 0, py::arg("gt"),
           trid_doc::Triangulation_Triangulation)
      .def(py::init<const Tri&>(), py::arg("other"),
           trid_doc::Triangulation_Triangulation_1)
      .def("are_incident_full_cells_valid", &cgalpy::trid::are_incident_full_cells_valid,
           py::arg("vertex"), py::arg("verbose") = false, py::arg("level") = 0,
           trid_doc::Triangulation_are_incident_full_cells_valid)
      .def("clear", &Tri::clear, "Initialize the triangulation to the empty complex")
      // .def("collapse_face", &cgalpy::trid::tds_collapse_face) // bug in CGAL
      .def("current_dimension", &Tri::current_dimension,
           trid_doc::Triangulation_current_dimension)
      .def("empty", &Tri::empty,
           trid_doc::Triangulation_empty)
      .def("full_cell", &cgalpy::trid::full_cell2<Tri>, ri, py::arg("f"),
           trid_doc::Triangulation_full_cell)
      .def("geom_traits", &Tri::geom_traits, ri,
           trid_doc::Triangulation_geom_traits)

      .def("incident_full_cells", &cgalpy::trid::incident_full_cells1<Tri>, py::arg("f"),
           "Obtain all the full cells that are incident to a face\n"
           "Parameters:\n"
           "  f (Facet): the input facet\n"
           "Return:\n"
           "  list: the full cells incident to f\n")
      .def("incident_full_cells", &cgalpy::trid::incident_full_cells2<Tri>, py::arg("v"),
           "Obtain all the full cells that are incident to a vertex\n"
           "Parameters:\n"
           "  v (Vertex): the input vertex\n"
           "Return:\n"
           "  list: the full cells incident to v\n")
      .def("incident_faces", &cgalpy::trid::incident_faces<Tri>, ri, py::arg("v"), py::arg("dim"),
           "Obtain all faces of dimension dim incident to a given vertex\n"
           "Parameters:\n"
           "  v (Vertex: the input vertex\n"
           "  dim (int): the dimension\n"
           "Return:\n"
           "  list [Face Face ...])\n")

      .def("index_of_covertex", &Tri::index_of_covertex, py::arg("f"),
           trid_doc::Triangulation_index_of_covertex)
      .def("infinite_full_cell", &cgalpy::trid::infinite_full_cell, ri,
           trid_doc::Triangulation_infinite_full_cell)
      .def("infinite_vertex",  &cgalpy::trid::infinite_vertex, ri,
           trid_doc::Triangulation_infinite_vertex)

      // Insertion
      .def("insert", &cgalpy::trid::insert1<Tri>, py::arg("points"),
           trid_doc::Triangulation_insert)
      .def("insert", &cgalpy::trid::insert2<Tri>,
           py::arg("p"), py::arg("lt"), py::arg("f"), py::arg("ft"), py::arg("c"),
           trid_doc::Triangulation_insert_3)
      .def("insert", &cgalpy::trid::insert3<Tri>,
           py::arg("p"), py::arg("hint"),
           trid_doc::Triangulation_insert_1)
      .def("insert", &cgalpy::trid::insert4<Tri>,
           py::arg("p"), py::arg("hint"),
           trid_doc::Triangulation_insert_2)
      .def("insert", &cgalpy::trid::insert5<Tri>, py::arg("p"),
           trid_doc::Triangulation_insert_1)
      .def("insert_in_face", &cgalpy::trid::insert_in_face,
           py::arg("p"), py::arg("f"),
           trid_doc::Triangulation_insert_in_face)
      .def("insert_in_facet", &cgalpy::trid::insert_in_facet,
           py::arg("p"), py::arg("f"),
           trid_doc::Triangulation_insert_in_facet)
      .def("insert_in_full_cell", &cgalpy::trid::insert_in_full_cell,
           py::arg("p"), py::arg("c"),
           trid_doc::Triangulation_insert_in_full_cell)
      .def("insert_in_hole", &cgalpy::trid::insert_in_hole,
           py::arg("p"), py::arg("full_cells"), py::arg("f"),
           trid_doc::Triangulation_insert_in_hole)
      .def("insert_in_hole_get_full_cells", &cgalpy::trid::insert_in_hole_get_full_cells,
           py::arg("p"), py::arg("full_cells"), py::arg("f"),
           trid_doc::Triangulation_insert_in_hole_1)
      .def("insert_outside_convex_hull", &cgalpy::trid::insert_outside_convex_hull,
           py::arg("p"), py::arg("c"),
           trid_doc::Triangulation_insert_outside_convex_hull)
      .def("insert_outside_affine_hull", &cgalpy::trid::insert_outside_affine_hull,
           py::arg("p"),
           trid_doc::Triangulation_insert_outside_affine_hull)

      .def("is_infinite", py::overload_cast<const Vertex&>(&Tri::is_infinite, py::const_),
           py::arg("v"), trid_doc::Triangulation_is_infinite)
      .def("is_infinite", py::overload_cast<const Fc&>(&Tri::is_infinite, py::const_),
           py::arg("c"), trid_doc::Triangulation_is_infinite_1)
      .def("is_infinite", py::overload_cast<const Facet&>(&Tri::is_infinite, py::const_),
           py::arg("f"), trid_doc::Triangulation_is_infinite_2)
      .def("is_infinite", py::overload_cast<const Face&>(&Tri::is_infinite, py::const_),
           py::arg("f"), trid_doc::Triangulation_is_infinite_3)
      .def("is_valid", &Tri::is_valid,
           py::arg("verbose") = false, py::arg("level") = 0,
           trid_doc::Triangulation_is_valid)
      .def("locate", &cgalpy::trid::locate1, ri, py::arg("q"),
           trid_doc::Triangulation_locate)
      .def("locate", &cgalpy::trid::locate2, ri,
           py::arg("q"), py::arg("v"),
           trid_doc::Triangulation_locate_1)
      .def("locate", &cgalpy::trid::locate3, ri,
           py::arg("q"), py::arg("c"),
           trid_doc::Triangulation_locate)
      .def("locate_get_incident", &cgalpy::trid::locate4, ri, py::arg("q"),
           trid_doc::Triangulation_locate_2)
      .def("locate_get_incident", &cgalpy::trid::locate5, ri,
           py::arg("q"), py::arg("v"),
           trid_doc::Triangulation_locate_3)
      .def("locate_get_incident", &cgalpy::trid::locate6, ri,
           py::arg("q"), py::arg("c"),
           trid_doc::Triangulation_locate_2)
      .def("maximal_dimension", &Tri::maximal_dimension,
           trid_doc::Triangulation_maximal_dimension)
      .def("number_of_finite_full_cells", &Tri::number_of_finite_full_cells,
           trid_doc::Triangulation_number_of_finite_full_cells)
      .def("number_of_full_cells", &Tri::number_of_full_cells,
           trid_doc::Triangulation_number_of_full_cells)
      .def("number_of_vertices", &Tri::number_of_vertices,
           trid_doc::Triangulation_number_of_vertices)
      // .def("rotate_rotor", &Tri::rotate_rotor)
      .def("star", &cgalpy::trid::star<Tri>, ri, py::arg("f"),
           "Obtain all the full cells that share at least one vertex with a face"
           "Parameters:\n"
           "  f (Face): the input face\n")
      .def("tds", py::overload_cast<>(&Tri::tds, py::const_),
           trid_doc::Triangulation_tds)
     ;

    // Lock_data_structure
    py::enum_<cgalpy::trid::Locate_type>(tri_c, "Locate_type")
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
      py::class_<Facet>(tri_c, "Facet", trid_doc::Triangulation_Facet)
        .def("full_cell", [](const Facet& f) { return *(f.full_cell()); }, ri,
             trid_doc::Triangulation_full_cell)
        .def("index_of_covertex", &Facet::index_of_covertex,
             trid_doc::Triangulation_index_of_covertex)
        ;
    }

    // Face
    if (! add_attr<Face>(tri_c, "Face")) {
      py::class_<Face>(tri_c, "Face", trid_doc::TriangulationDSFace_class)
        .def(py::init<int>(), py::arg("dim"),
             trid_doc::TriangulationDSFace_TriangulationDSFace)
        .def(py::init<const Face&>(), py::arg("other"), "Copy constructor.")
        .def("face_dimension", &Face::face_dimension,
             trid_doc::TriangulationDSFace_face_dimension)
        .def("index", &Face::index, py::arg("i"),
             trid_doc::TriangulationDSFace_index)
        .def("vertex", [](Face& f, int i)->Vertex& { return *(f.vertex(i)); }, ri,
             py::arg("i"), trid_doc::TriangulationDSFace_vertex)
        .def("Full_cell", [](Face& f)->Fc& { return *(f.full_cell()); }, ri,
             trid_doc::TriangulationDSFace_full_cell)
        .def("full_cell", [](Face& f)->Fc& { return *(f.full_cell()); }, ri,
             trid_doc::TriangulationDSFace_full_cell)
        .def("clear", &Face::clear,
             trid_doc::TriangulationDSFace_clear)
        .def("set_full_cell", cgalpy::trid::set_full_cell, py::arg("c"),
             trid_doc::TriangulationDSFace_set_full_cell)
        .def("set_index", &Face::set_index, py::arg("i"), py::arg("j"),
             trid_doc::TriangulationDSFace_set_index)
        ;
    }

    add_attr<cgalpy::trid::Geom_traits>(tri_c, "Geom_traits");

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
      .def("facets", &cgalpy::trid::facets<Tri>, py::keep_alive<0, 1>(),
           trid_doc::Triangulation_facets_begin)
      .def("full_cells", &cgalpy::trid::full_cells<Tri>, py::keep_alive<0, 1>(),
           trid_doc::Triangulation_full_cells_begin)
      .def("vertices", &cgalpy::trid::vertices<Tri>, py::keep_alive<0, 1>(),
           trid_doc::Triangulation_vertices_begin)
      .def("finite_facets", &cgalpy::trid::finite_facets, py::keep_alive<0, 1>(),
           trid_doc::Triangulation_finite_facets_begin)
      .def("finite_full_cells", &cgalpy::trid::finite_full_cells, py::keep_alive<0, 1>(),
           trid_doc::Triangulation_finite_full_cells_begin)
      .def("finite_vertices", &cgalpy::trid::finite_vertices, py::keep_alive<0, 1>(),
           trid_doc::Triangulation_finite_vertices_begin)
      ;

#if CGALPY_TRID == CGALPY_TRID_REGULAR
    using Rtri = cgalpy::trid::Regular_triangulation_d;

    if (! add_attr<Rtri>(m, "Regular_triangulation")) {
      py::class_<Rtri, Tri> rtri_c(m, "Regular_triangulation",
                                  trid_doc::Regular_triangulation_class);
      rtri_c.def(py::init<int>(), py::arg("dim") = 0,
                  trid_doc::Regular_triangulation_Regular_triangulation)
        .def(py::init<int, const cgalpy::trid::Geom_traits&>(),
             py::arg("dim") = 0, py::arg("gt"),
             trid_doc::Regular_triangulation_Regular_triangulation)
        .def(py::init<const Rtri&>(), py::arg("other"), "Copy constructor.")
        .def("compute_conflict_zone", &cgalpy::trid::compute_conflict_zone<Rtri>,
             ri, py::arg("p"), py::arg("c"),
             trid_doc::Regular_triangulation_compute_conflict_zone)
        .def("insert_if_in_star", &cgalpy::trid::insert_if_in_star1,
             ri, py::arg("p"), py::arg("star_center"),
             trid_doc::Regular_triangulation_insert_if_in_star)
        .def("insert_if_in_star", &cgalpy::trid::insert_if_in_star2,
             ri, py::arg("p"), py::arg("star_center"), py::arg("start"),
             trid_doc::Regular_triangulation_insert_if_in_star)
        .def("insert_if_in_star", &cgalpy::trid::insert_if_in_star3,
             ri, py::arg("p"), py::arg("star_center"), py::arg("hint"),
             trid_doc::Regular_triangulation_insert_if_in_star_1)
        .def("insert_if_in_star", &cgalpy::trid::insert_if_in_star4,
             ri, py::arg("p"), py::arg("star_center"), py::arg("hint"),
             trid_doc::Regular_triangulation_insert_if_in_star_2)
        .def("is_in_conflict", &cgalpy::trid::is_in_conflict<Rtri>,
             py::arg("p"), py::arg("c"),
             trid_doc::Regular_triangulation_is_in_conflict)
        .def("number_of_hidden_vertices", &Rtri::number_of_hidden_vertices,
             trid_doc::Regular_triangulation_number_of_hidden_vertices)
        .def("number_of_vertices", &Rtri::number_of_vertices,
             trid_doc::Regular_triangulation_number_of_vertices)
        ;

      add_attr<cgalpy::trid::Weighted_point>(rtri_c, "Weighted_point");
    }
#endif

#if CGALPY_TRID == CGALPY_TRID_DELAUNAY
    using Dtri = cgalpy::trid::Delaunay_triangulation_d;

    if (! add_attr<Dtri>(m, "Delaunay_triangulation")) {
      py::class_<Dtri, Tri> dtri_c(m, "Delaunay_triangulation",
                                  trid_doc::Delaunay_triangulation_class);
      dtri_c.def(py::init<int>(), py::arg("dim") = 0,
                  trid_doc::Delaunay_triangulation_Delaunay_triangulation)
        .def(py::init<int, const cgalpy::trid::Geom_traits&>(),
             py::arg("dim") = 0, py::arg("traits"),
             trid_doc::Delaunay_triangulation_Delaunay_triangulation)
        .def(py::init<const Dtri&>(), py::arg("other"), "Copy constructor.")
        .def("compute_conflict_zone", &cgalpy::trid::compute_conflict_zone<Dtri>,
             ri, py::arg("p"), py::arg("c"),
             trid_doc::Delaunay_triangulation_compute_conflict_zone)

        // Insertion
        .def("insert", &cgalpy::trid::insert1<Dtri>, py::arg("points"),
             trid_doc::Delaunay_triangulation_insert)
        .def("insert", &cgalpy::trid::insert2<Dtri>,
             py::arg("p"), py::arg("lt"), py::arg("f"), py::arg("ft"), py::arg("c"),
             trid_doc::Delaunay_triangulation_insert_3)
        .def("insert", &cgalpy::trid::insert3<Dtri>,
             py::arg("p"), py::arg("hint"),
             trid_doc::Delaunay_triangulation_insert_1)
        .def("insert", &cgalpy::trid::insert4<Dtri>,
             py::arg("p"), py::arg("hint"),
             trid_doc::Delaunay_triangulation_insert_2)
        .def("insert", &cgalpy::trid::insert5<Dtri>, py::arg("p"),
             trid_doc::Delaunay_triangulation_insert_1)

        .def("is_in_conflict", &cgalpy::trid::is_in_conflict<Dtri>,
             py::arg("p"), py::arg("c"),
             trid_doc::Delaunay_triangulation_is_in_conflict)
        .def("remove", &cgalpy::trid::remove1, py::arg("vertices"),
             trid_doc::Delaunay_triangulation_remove_1)
        .def("remove", &cgalpy::trid::remove2, ri, py::arg("v"),
             trid_doc::Delaunay_triangulation_remove)
        ;
    }
#endif
  }
}
