// Copyright (c) 2025 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

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

}
} // namespace cgalpy

//!
void export_tri2_constrained(py::module_& m) {
  using Ctri = cgalpy::tri2::Constrained_triangulation_2;
  using Tri = cgalpy::tri2::Triangulation_2;
  using Pnt = cgalpy::tri2::Point;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Ctri>(m, "Constrained_triangulation_2")) return;

  py::class_<Ctri, Tri> tri_c(m, "Constrained_triangulation_2",
                                 tri2_doc::Constrained_triangulation_2_class);

  tri_c.def(py::init<>(),
            "Constructs an empty constrained 2D triangulation.")
    .def(py::init<const cgalpy::tri2::Traits&>(),
         py::arg("traits"),
         "Constructs a constrained 2D triangulation with geometric traits.")
    .def("__init__", &cgalpy::tri2::ct2_init,
         py::arg("constraints"),
         "Constructs a constrained 2D triangulation from vertex-pair constraints.")
    .def("insert_constraint", &cgalpy::tri2::insert_constraint,
         py::arg("va"), py::arg("vb"),
         tri2_doc::Constrained_triangulation_2_insert_constraint_1)
    ;
}
