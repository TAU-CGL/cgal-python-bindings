// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/stl/pair.h>

#include "cgalpy/add_attr.hpp"
#include "cgalpy/triangulation_2_types.hpp"
#include "cgalpy/Tri2_docstrings.hpp"

namespace py = nanobind;
namespace tri2_doc = cgalpy::tri2::docstrings;

namespace cgalpy {
namespace tri2 {

//!
template <typename T_out = std::pair<Vertex_handle, Vertex_handle>>
struct vertex_pair_forward_iterator :
  boost::iterator_facade<vertex_pair_forward_iterator<T_out>, T_out, std::forward_iterator_tag, T_out> {

  // Default constructor.
  // Workaround the lack of default constructor for py::detail::fast_iterator.
  // vertex_pair_forward_iterator() {}
  vertex_pair_forward_iterator() : m_it(py::list().end()) {}

  vertex_pair_forward_iterator(const py::list& lst, bool isbegin = true) :
    m_it((isbegin) ? lst.begin() : lst.end())
  {}

  void increment() { ++m_it; }
  auto dereference() const {
    using T_in = std::pair<py::object, py::object>;
    T_in vp = py::cast<T_in>(*m_it);
    Vertex& va = py::cast<Vertex&>(vp.first);
    Vertex& vb = py::cast<Vertex&>(vp.second);
    return std::make_pair(Vertex_handle(&va), Vertex_handle(&vb));
  }

  bool equal(vertex_pair_forward_iterator<T_out> const& o) const { return m_it == o.m_it; }

private:
  py::detail::fast_iterator m_it;
};

//!
void ct2_init(tri2::Constrained_triangulation_2* tri, py::list& lst) {
  auto begin = vertex_pair_forward_iterator(lst);
  auto end = vertex_pair_forward_iterator(lst, false);
  new (tri) tri2::Constrained_triangulation_2(begin, end);  // placement new
}

//
void insert_constraint(Constrained_triangulation_2& tri, const Vertex& va, const Vertex& vb) {
  auto ha = Vertex_handle(const_cast<Vertex*>(&va));
  auto hb = Vertex_handle(const_cast<Vertex*>(&vb));
  tri.insert_constraint(ha, hb);
}

//!
py::object ct2_locate_get_incident(py::handle self, const Point& query) {
  constexpr auto ri(py::rv_policy::reference_internal);
  auto& tri = py::cast<Constrained_triangulation_2&>(self);

  Locate_type lt;
  int li;
  auto fh = tri.locate(query, lt, li);

  switch (lt) {
   case Constrained_triangulation_2::VERTEX:
   case Constrained_triangulation_2::EDGE:
   case Constrained_triangulation_2::FACE:
    return py::make_tuple(
      py::cast(lt),
      py::cast(*fh, ri, self),
      py::int_(li)
    );

   case Constrained_triangulation_2::OUTSIDE_CONVEX_HULL:
    return py::make_tuple(
      py::cast(lt),
      py::cast(*fh, ri, self)
    );

   case Constrained_triangulation_2::OUTSIDE_AFFINE_HULL:
    return py::make_tuple(py::cast(lt));
  }

  return py::make_tuple(py::cast(lt));
}

}
} // namespace cgalpy

//!
void export_constrained_triangulation_2(py::module_& m) {
  using Ctri = cgalpy::tri2::Constrained_triangulation_2;
  using Pnt = cgalpy::tri2::Point;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Ctri>(m, "Constrained_triangulation_2")) return;

  py::class_<Ctri> tri_c(m, "Constrained_triangulation_2",
                                 tri2_doc::Constrained_triangulation_2_class);

  tri_c.def(py::init<>(),
            "Constructs an empty constrained 2D triangulation.")
    .def(py::init<const cgalpy::tri2::Traits&>(),
         py::arg("traits"),
         "Constructs a constrained 2D triangulation with geometric traits.")
    .def("__init__", &cgalpy::tri2::ct2_init,
         py::arg("constraints"),
         "Constructs a constrained 2D triangulation from vertex-pair constraints.")

    // Bind selected inherited operations directly on Ctri.
    // Its non-polymorphic-base hierarchy is unsupported by nanobind
    // inheritance registration.
    .def("dimension",
         [](const Ctri& tri) { return tri.dimension(); },
         tri2_doc::Triangulation_2_dimension)
    .def("number_of_vertices",
         [](const Ctri& tri) { return tri.number_of_vertices(); },
         tri2_doc::Triangulation_2_number_of_vertices)
    .def("number_of_faces",
         [](const Ctri& tri) { return tri.number_of_faces(); },
         tri2_doc::Triangulation_2_number_of_faces)
    .def("is_valid",
         [](const Ctri& tri, bool verbose, int level) {
           return tri.is_valid(verbose, level);
         },
         py::arg("verbose") = false,
         py::arg("level") = 0,
         tri2_doc::Triangulation_2_is_valid)

    // Obtain exact location data directly from Ctri.
    .def("locate_get_incident",
         &cgalpy::tri2::ct2_locate_get_incident,
         py::arg("query"),
         tri2_doc::Triangulation_2_locate_1)

    // Bind constrained-aware point insertion directly on Ctri.
    .def("insert", &cgalpy::tri2::insert_point1<Ctri>, ri,
         py::arg("p"),
         tri2_doc::Constrained_triangulation_2_insert)
    .def("insert", &cgalpy::tri2::insert_point2<Ctri>, ri,
         py::arg("p"), py::arg("start"),
         tri2_doc::Constrained_triangulation_2_insert)
    .def("insert", &cgalpy::tri2::insert_point3<Ctri>, ri,
         py::arg("p"), py::arg("lt"), py::arg("loc"), py::arg("li"),
         tri2_doc::Constrained_triangulation_2_insert_1)

    .def("insert_constraint", &cgalpy::tri2::insert_constraint,
         py::arg("va"), py::arg("vb"),
         tri2_doc::Constrained_triangulation_2_insert_constraint_1)
    ;

  // Re-export shared types already registered on Triangulation_2.
  add_attr<cgalpy::tri2::Geom_traits>(tri_c, "Geom_traits");
  add_attr<cgalpy::tri2::Point>(tri_c, "Point");
  add_attr<cgalpy::tri2::Segment>(tri_c, "Segment");
  add_attr<cgalpy::tri2::Triangle>(tri_c, "Triangle");
  add_attr<cgalpy::tri2::Vertex>(tri_c, "Vertex");
  add_attr<cgalpy::tri2::Face>(tri_c, "Face");
  add_attr<cgalpy::tri2::Edge>(tri_c, "Edge");
  tri_c.attr("Locate_type") =
    m.attr("Triangulation_2").attr("Locate_type");

  // Re-export iterator and circulator types already registered on
  // Triangulation_2.
  tri_c.attr("All_edges_iterator") =
    m.attr("Triangulation_2").attr("All_edges_iterator");
  tri_c.attr("All_faces_iterator") =
    m.attr("Triangulation_2").attr("All_faces_iterator");
  tri_c.attr("All_vertices_iterator") =
    m.attr("Triangulation_2").attr("All_vertices_iterator");
  tri_c.attr("Edge_circulator") =
    m.attr("Triangulation_2").attr("Edge_circulator");
  tri_c.attr("Edge_iterator") =
    m.attr("Triangulation_2").attr("Edge_iterator");
  tri_c.attr("Face_circulator") =
    m.attr("Triangulation_2").attr("Face_circulator");
  tri_c.attr("Face_iterator") =
    m.attr("Triangulation_2").attr("Face_iterator");
  tri_c.attr("Finite_edges_iterator") =
    m.attr("Triangulation_2").attr("Finite_edges_iterator");
  tri_c.attr("Finite_faces_iterator") =
    m.attr("Triangulation_2").attr("Finite_faces_iterator");
  tri_c.attr("Finite_vertices_iterator") =
    m.attr("Triangulation_2").attr("Finite_vertices_iterator");
  tri_c.attr("Point_iterator") =
    m.attr("Triangulation_2").attr("Point_iterator");
  tri_c.attr("Vertex_circulator") =
    m.attr("Triangulation_2").attr("Vertex_circulator");
  tri_c.attr("Vertex_iterator") =
    m.attr("Triangulation_2").attr("Vertex_iterator");

  // Re-export Locate_type values already exported on Triangulation_2.
  tri_c.attr("VERTEX") =
    m.attr("Triangulation_2").attr("VERTEX");
  tri_c.attr("EDGE") =
    m.attr("Triangulation_2").attr("EDGE");
  tri_c.attr("FACE") =
    m.attr("Triangulation_2").attr("FACE");
  tri_c.attr("OUTSIDE_CONVEX_HULL") =
    m.attr("Triangulation_2").attr("OUTSIDE_CONVEX_HULL");
  tri_c.attr("OUTSIDE_AFFINE_HULL") =
    m.attr("Triangulation_2").attr("OUTSIDE_AFFINE_HULL");
}
