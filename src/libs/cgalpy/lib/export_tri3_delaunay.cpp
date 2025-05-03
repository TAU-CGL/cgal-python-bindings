// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <functional>

#include <boost/iterator/function_output_iterator.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/function.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_input_iterator.hpp"
#include "CGALPY/triangulation_3_types.hpp"

namespace py = nanobind;

namespace tri3 {

//!
void dt3_init(tri3::Delaunay_triangulation_3* tri, py::list& lst) {
  auto begin = stl_input_iterator<tri3::Point>(lst);
  auto end = stl_input_iterator<tri3::Point>(lst, false);
  new (tri) tri3::Delaunay_triangulation_3(begin, end);  // placement new
}

//!
auto insert_del1(Delaunay_triangulation_3& dt, const Point& p,
                 bool lock_zone = false) {
  if (! lock_zone) {
    auto vh = dt.insert(p, Cell_handle());
    return &*vh;
  }

  bool could_lock_zone;
  auto vh = dt.insert(p, Cell_handle(), &could_lock_zone);
  return could_lock_zone ? &*vh : nullptr;
}

//!
auto insert_del2(Delaunay_triangulation_3& dt, const Point& p, Cell& start,
                 bool lock_zone = false) {
  if (! lock_zone) {
    auto vh = dt.insert(p, Cell_handle(&start));
    return &*vh;
  }

  bool could_lock_zone;
  auto vh = dt.insert(p, Cell_handle(&start), &could_lock_zone);
  return could_lock_zone ? &*vh : nullptr;
}

//!
auto insert_del3(Delaunay_triangulation_3& dt, const Point& p,
                 Vertex& hint, bool lock_zone = false) {
  if (! lock_zone) {
    auto vh = dt.insert(p, Vertex_handle(&hint));
    return &*vh;
  }

  bool could_lock_zone;
  auto vh = dt.insert(p, Vertex_handle(&hint), &could_lock_zone);
  return could_lock_zone ? &*vh : nullptr;
}

//!
auto insert_del4(Delaunay_triangulation_3& dt, const Point& p, Locate_type lt,
                 Cell& c, int li, int lj, bool lock_zone = false) {
  if (! lock_zone) {
    auto vh = dt.insert(p, lt, Cell_handle(&c), li, lj);
    return &*vh;
  }

  bool could_lock_zone;
  auto vh = dt.insert(p, lt, Cell_handle(&c), li, lj, &could_lock_zone);
  return could_lock_zone ? &*vh : nullptr;
}

//! See manual of locate_face()
py::tuple find_conflicts(py::handle self, const Point& p, Cell& start) {
  constexpr auto ri(py::rv_policy::reference_internal);

  auto& tri = py::cast<Delaunay_triangulation_3&>(self);
  py::list bfs;
  auto op1 = [&] (const Facet& f) mutable { bfs.append(f); };
  auto it1 = boost::make_function_output_iterator(std::ref(op1));
  py::list cells;
  auto op2 = [&] (const Cell_handle& ch) mutable {
    Cell& c = *ch;
    cells.append(py::cast(c, ri, self));
  };
  auto it2 = boost::make_function_output_iterator(std::ref(op2));
  tri.find_conflicts(p, Cell_handle(&start), it1, it2);
  return py::make_tuple(bfs, cells);
}

//!
py::tuple find_conflicts_ifs(py::handle self, const Point& p, Cell& start) {
  constexpr auto ri(py::rv_policy::reference_internal);

  auto& tri = py::cast<Delaunay_triangulation_3&>(self);
  py::list bfs;
  auto op1 = [&] (const Facet& f) mutable { bfs.append(f); };
  auto it1 = boost::make_function_output_iterator(std::ref(op1));
  py::list cells;
  auto op2 = [&] (const Cell_handle& ch) mutable {
    Cell& c = *ch;
    cells.append(py::cast(c, ri, self));
  };
  auto it2 = boost::make_function_output_iterator(std::ref(op2));
  py::list ifs;
  auto op3 = [&] (const Facet& f) mutable { ifs.append(f); };
  auto it3 = boost::make_function_output_iterator(std::ref(op3));
  tri.find_conflicts(p, Cell_handle(&start), it1, it2, it3);
  return py::make_tuple(bfs, cells, ifs);
}

//!
CGAL::Bounded_side side_of_circle2(const Delaunay_triangulation_3& tri, Cell& c,
                                   int i, const Point& p, bool perturb = false)
{ return tri.side_of_circle(Cell_handle(&c), i, p, perturb); }

//!
CGAL::Bounded_side side_of_sphere(const Delaunay_triangulation_3& tri, Cell& c,
                                  const Point& p)
{ return tri.side_of_sphere(Cell_handle(&c), p); }

//!
Vertex& nearest_vertex1(const Delaunay_triangulation_3& tri, const Point& p) {
  auto vh = tri.nearest_vertex(p);
  return *vh;
}

//!
Vertex& nearest_vertex2(const Delaunay_triangulation_3& tri, const Point& p,
                        Cell& c) {
  auto vh = tri.nearest_vertex(p, Cell_handle(&c));
  return *vh;
}

}

//!
void export_tri3_delaunay(py::module_& m) {
  using Tri = tri3::Delaunay_triangulation_3;
  using Btri = tri3::Triangulation_3;
  using Pnt = tri3::Point;
  using Facet = tri3::Facet;
  CGAL::Bounded_side(Tri::*side_of_circle1)(const Facet&, const Pnt&, bool) const =
    &Tri::side_of_circle;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Tri>(m, "Delaunay_triangulation_3")) return;

  py::class_<Tri, Btri> tri_c(m, "Delaunay_triangulation_3");

  tri_c.def(py::init<>())
    .def(py::init<const tri3::Traits&>())
    .def("__init__", &tri3::dt3_init)
    .def("insert", &tri3::insert_del1,
         py::arg("point"), py::arg("lock_zone") = false, ri)
    .def("insert", &tri3::insert_del2,
         py::arg("point"), py::arg("start"), py::arg("lock_zone") = false, ri)
    .def("insert", &tri3::insert_del3,
         py::arg("point"), py::arg("hint"), py::arg("lock_zone") = false, ri)
    .def("insert", &tri3::insert_del4,
         py::arg("point"), py::arg("lt"), py::arg("c"), py::arg("li"),
         py::arg("lj"), py::arg("lock_zone") = false, ri)

    .def("side_of_circle", side_of_circle1)
    .def("side_of_circle", tri3::side_of_circle2,
         py::arg("c"), py::arg("i"), py::arg("p"), py::arg("perturb") = false)
    .def("side_of_sphere", &tri3::side_of_sphere)
    .def("nearest_vertex", &tri3::nearest_vertex1)
    .def("nearest_vertex", &tri3::nearest_vertex2)
    .def("nearest_vertex_in_cell", &Tri::nearest_vertex_in_cell)

    // Displacement
    // .def("move_if_no_collision", &tri3::move_if_no_collision)
    // .def("move", &tri3::move)

    // Removal
    // .def("remove", &tri3::remove1)
    // .def("remove", &tri3::remove2)

    .def("find_conflicts", &tri3::find_conflicts, ri)
    .def("find_conflicts_if", &tri3::find_conflicts_ifs, ri)
    ;
}
