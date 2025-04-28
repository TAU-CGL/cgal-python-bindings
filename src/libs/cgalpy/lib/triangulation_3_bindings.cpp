// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#define CGAL_USE_BASIC_VIEWER

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>

#ifdef CGALPY_HAS_VISUAL
#include <CGAL/draw_triangulation_3.h>
#if defined(CGALPY_BASIC_VIEWER_BINDINGS)
#include "CGALPY/basic_viewer_types.hpp"
#endif
#endif

#include "CGALPY/types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/triangulation_3_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/make_iterator.hpp"

void export_tri3_vertex(py::class_<tri3::Triangulation_3>&);
void export_tri3_cell(py::class_<tri3::Triangulation_3>&);

namespace py = nanobind;

namespace tri3 {

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY

//!
bool are_equal1(const Triangulation_3& tri, Cell& c, int i, Cell& n, int j)
{ return tri.are_equal(Cell_handle(&c), i, Cell_handle(&n), j); }

//!
bool are_equal2(const Triangulation_3& tri, const Facet& f, Cell& n, int j)
{ return tri.are_equal(f, Cell_handle(&n), j); }

//!
size_type degree(const Triangulation_3& tri, Vertex& v)
{ return tri.degree(Vertex_handle(&v)); }

//!
void dt3_init(tri3::Triangulation_3* tri, py::list& lst) {
  auto begin = stl_input_iterator<tri3::Point>(lst);
  auto end = stl_input_iterator<tri3::Point>(lst, false);
  new (tri) tri3::Triangulation_3(begin, end);  // placement new
}

//
std::ptrdiff_t insert_points(Triangulation_3& dt, py::list& lst) {
  if (! lst) return 0;
  if (! py::isinstance<tri3::Point>(lst[0])) return 0;
  auto begin = stl_input_iterator<tri3::Point>(lst);
  auto end = stl_input_iterator<tri3::Point>(lst, false);
  return dt.insert(begin, end);
}

//!
Vertex& insert_point1(Triangulation_3& tri, const Point& p) {
  auto vh = tri.insert(p, Cell_handle());
  return *vh;
}

//!
Vertex& insert_point2(Triangulation_3& tri, const Point& p, Cell& start) {
  auto vh = tri.insert(p, tri3::Cell_handle(&start));
  return *vh;
}

//!
Vertex& insert_point3(Triangulation_3& tri, const Point& p, Vertex& hint) {
  auto vh = tri.insert(p, tri3::Vertex_handle(&hint));
  return *vh;
}

//!
Vertex& insert_point4(Triangulation_3& tri, const Point& p,
                      Locate_type lt, tri3::Cell& c, int li, int lj) {
  auto vh = tri.insert(p, lt, tri3::Cell_handle(&c), li, lj);
  return *vh;
}

#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY

//!
auto insert_del_point1(Triangulation_3& dt, const Point& p,
                       bool lock_zone = false) {
  if (! lock_zone) return &(insert_point1(dt, p));

  bool could_lock_zone;
  auto vh = dt.insert(p, Cell_handle(), &could_lock_zone);
  return could_lock_zone ? &*vh : nullptr;
}

//!
auto insert_del_point2(Triangulation_3& dt, const Point& p, Cell& start,
                       bool lock_zone = false) {
  if (! lock_zone) return &(insert_point2(dt, p, start));

  bool could_lock_zone;
  auto vh = dt.insert(p, tri3::Cell_handle(&start), &could_lock_zone);
  return could_lock_zone ? &*vh : nullptr;
}

//!
auto insert_del_point3(Triangulation_3& dt, const Point& p, Vertex& hint,
                       bool lock_zone = false) {
  if (! lock_zone) return &(insert_point3(dt, p, hint));

  bool could_lock_zone;
  auto vh = dt.insert(p, tri3::Vertex_handle(&hint), &could_lock_zone);
  return could_lock_zone ? &*vh : nullptr;
}

//!
auto insert_del_point4(Triangulation_3& dt, const Point& p, Locate_type lt,
                       Cell& c, int li, int lj, bool lock_zone = false) {
  if (! lock_zone) return &(insert_point4(dt, p, lt, c, li, lj));

  bool could_lock_zone;
  auto vh = dt.insert(p, lt, tri3::Cell_handle(&c), li, lj, &could_lock_zone);
  return could_lock_zone ? &*vh : nullptr;
}

#endif

#if (CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT)

//
Vertex_handle insert4(Triangulation_3& dt, const Point& p, Cell_handle start)
{ return dt.insert(p, start); }

//
Vertex_handle insert5(tri3::Triangulation_3& dt, const Point& p,
                      Vertex_handle hint)
{ return dt.insert(p, hint); }

//
Vertex_handle insert6(tri3::Triangulation_3& dt, const Point& p,
                      Locate_type lt, Cell_handle c, int li, int lj)
{ return dt.insert(p, lt, c, li, lj); }

#endif

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

py::object locate4(const Triangulation_3& tri, const Point& query) {
  Locate_type lt;
  int li;
  int lj;
  auto ch = tri.locate(query, lt, li, lj);
  switch (lt) {
   case Triangulation_3::FACET:
   case Triangulation_3::VERTEX:
     return py::make_tuple(py::cast(lt), py::int_(li));

   case Triangulation_3::EDGE:
    return py::make_tuple(py::cast(lt), py::int_(li), py::int_(lj));

   case Triangulation_3::CELL:
   case Triangulation_3::OUTSIDE_CONVEX_HULL: return py::cast(*ch);
  }

  throw std::runtime_error("Invalid location type");
  return py::none();
}

//
template <typename Handle_>
const typename Handle_::value_type& value(Handle_ handle) { return *handle; }

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

//!
Vertex& nearest_vertex1(const Triangulation_3& tri, const Point& p) {
  auto vh = tri.nearest_vertex(p);
  return *vh;
}

//!
Vertex& nearest_vertex2(const Triangulation_3& tri, const Point& p, Cell& c) {
  auto vh = tri.nearest_vertex(p, Cell_handle(&c));
  return *vh;
}

//!
CGAL::Bounded_side side_of_circle2(const Triangulation_3& tri, Cell& c, int i,
                                   const Point& p, bool perturb = false)
{ return tri.side_of_circle(Cell_handle(&c), i, p, perturb); }

//!
CGAL::Bounded_side side_of_sphere(const Triangulation_3& tri, Cell& c,
                                  const Point& p)
{ return tri.side_of_sphere(Cell_handle(&c), p); }

} // End of namespace tri3

#endif

//
void export_triangulation_3(py::module_& m) {
  using Tri = tri3::Triangulation_3;
  using Ch = tri3::Cell_handle;
  using Vh = tri3::Vertex_handle;
  using Pnt = tri3::Point;
  using Vertex = tri3::Vertex;
  using Facet = tri3::Facet;

  constexpr auto ri(py::rv_policy::reference_internal);

  CGAL::Bounded_side(Tri::*side_of_circle1)(const Facet&, const Pnt&, bool) const =
    &Tri::side_of_circle;

  bool(Tri::*is_valid1)(bool, int) const = &Tri::is_valid;
  // bool(Tri::*is_valid2)(Ch, bool, int) const =
  //   &Tri::is_valid;

  py::class_<Tri> tri_c(m, "Triangulation_3");
  tri_c.def(py::init<>())
    .def(py::init<const tri3::Traits&>())
#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
    .def("__init__", &tri3::dt3_init)
#endif

    .def("are_equal",
         py::overload_cast<const Facet&, const Facet&>(&Tri::are_equal, py::const_))
    .def("are_equal", tri3::are_equal1)
    .def("are_equal", tri3::are_equal2)
    .def("clear", &Tri::clear)
    .def("degree", &tri3::degree)
    .def("dimension", &Tri::dimension)

    .def("number_of_cells", &Tri::number_of_cells)
    .def("number_of_edges", &Tri::number_of_edges)
    .def("number_of_facets", &Tri::number_of_facets)
    .def("number_of_finite_cells", &Tri::number_of_finite_cells)
    .def("number_of_finite_edges", &Tri::number_of_finite_edges)
    .def("number_of_finite_facets", &Tri::number_of_finite_facets)
    .def("number_of_vertices", &Tri::number_of_vertices)

    // Insertion
#if CGALPY_TRI3 != CGALPY_TRI3_DELAUNAY
    .def("insert", &tri3::insert_point1, ri)
    .def("insert", &tri3::insert_point2, ri)
    .def("insert", &tri3::insert_point3, ri)
    .def("insert", &tri3::insert_point4, ri)
#else
    .def("insert", &tri3::insert_del_point1,
         py::arg("point"), py::arg("lock_zone") = false, ri)
    .def("insert", &tri3::insert_del_point2,
         py::arg("point"), py::arg("start"), py::arg("lock_zone") = false, ri)
    .def("insert", &tri3::insert_del_point3,
         py::arg("point"), py::arg("hint"), py::arg("lock_zone") = false, ri)
    .def("insert", &tri3::insert_del_point4,
         py::arg("point"), py::arg("lt"), py::arg("c"), py::arg("li"),
         py::arg("lj"), py::arg("lock_zone") = false, ri)
#endif

#if CGALPY_TRI3_LOCATION_POLICY == CGALPY_TRI3_LOCATION_POLICY_COMPACT
    .def("insert", &tri3::insert4)
    .def("insert", &tri3::insert5)
    .def("insert", &tri3::insert6)
#endif

    .def("insert_points", &tri3::insert_points)

    .def("locate", &tri3::locate1, ri)
    .def("locate", &tri3::locate2, ri)
    .def("locate", &tri3::locate3, ri)
    .def("locate_face", &tri3::locate4, ri)

    // template<class PointWithInfoInputIterator >
    // std::ptrdiff_t insert (PointWithInfoInputIterator first, PointWithInfoInputIterator last)

    // Displacement
    .def("move_if_no_collision", &Tri::move_if_no_collision)
    .def("move", &Tri::move)

    // Removal
    .def("remove", static_cast<void(Tri::*)(Vh)>(&Tri::remove))
    // .def("remove", py::overload_cast<Vh, bool*>(&Tri::remove))

    // template<typename InputIterator >
    // int remove (InputIterator first, InputIterator beyond)

    // template<typename InputIterator >
    // int remove_cluster (InputIterator first, InputIterator beyond)

    // Queries


#if CGALPY_TRI3 == CGALPY_TRI3_DELAUNAY
    .def("side_of_circle", side_of_circle1)
    .def("side_of_circle", tri3::side_of_circle2,
         py::arg("c"), py::arg("i"), py::arg("p"), py::arg("perturb") = false)
    .def("side_of_sphere", &tri3::side_of_sphere)
    .def("nearest_vertex", &tri3::nearest_vertex1)
    .def("nearest_vertex", &tri3::nearest_vertex2)
#endif

    .def("nearest_vertex_in_cell", &Tri::nearest_vertex_in_cell)
    .def("is_valid", is_valid1)
    // .def("is_valid", is_valid2)
    ;

  // Triangulation_data_structure
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

  if (! add_attr<Pnt>(tri_c, "Point")) std::cerr << "'Point' not registered!\n";
  add_attr<tri3::Segment>(tri_c, "Segment");
  add_attr<tri3::Triangle>(tri_c, "Triangle");
  add_attr<tri3::Tetrahedron>(tri_c, "Tetrahedron");

  export_tri3_vertex(tri_c);
  export_tri3_cell(tri_c);

  py::class_<tri3::Facet>(tri_c, "Facet")
    .def_rw("first", &tri3::Facet::first)
    .def_rw("second", &tri3::Facet::second)
    ;

  py::class_<tri3::Edge>(tri_c, "Edge")
    .def_rw("first", &tri3::Edge::first)
    .def_rw("second", &tri3::Edge::second)
    .def_rw("third", &tri3::Edge::third)
    ;

  py::class_<Vh>(tri_c, "Vertex_handle")
    .def(py::init<>())
    .def("value", &tri3::value<Vh>, ri)
    ;

  py::class_<Ch>(tri_c, "Cell_handle")
    .def(py::init<>())
    .def("value", &tri3::value<Ch>, ri)
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
  // Segment_cell_iterator;
  // Segment_simplex_iterator;
}
