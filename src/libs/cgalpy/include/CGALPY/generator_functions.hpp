// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#ifndef CGALPY_GENERATOR_FUNCTIONS_HPP
#define CGALPY_GENERATOR_FUNCTIONS_HPP

#include <boost/graph/graph_traits.hpp>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>

#include <CGAL/boost/graph/generators.h>
#include <CGAL/Iso_cuboid_3.h>
#include <CGAL/Point_3.h>

namespace py = nanobind;

namespace boost_utils {

template <typename C, typename Pm, typename Kernel_>
void define_generate_functions(py::module_ &m) {
  using Gt = typename boost::graph_traits<Pm>;
  using K = Kernel_;
  using P = typename CGAL::Point_3<K>;
  using Iso_cuboid_3 = typename CGAL::Iso_cuboid_3<K>;
  using H = typename Gt::halfedge_descriptor;
  using vertices_size_type = typename Gt::vertices_size_type;
  using FT = typename K::FT;

  m.def("make_triangle", &CGAL::make_triangle<Pm, P>,
        py::arg("p0"), py::arg("p1"), py::arg("p2"), py::arg("g"),
        "creates an isolated triangle with its vertices initialized to p0, p1 and p2, and adds it to the graph g.");

  m.def("make_quad", &CGAL::make_quad<Pm, P>,
        py::arg("p0"), py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("g"),
        "creates an isolated quad with its vertices initialized to p0, p1, p2, and p3, and adds it to the graph g.");

  m.def("make_hexahedron",
        [](const P &p0, const P &p1, const P &p2, const P &p3, const P &p4, const P &p5, const P &p6,
           const P &p7, Pm &g, const py::dict& np = py::dict()) {
          return CGAL::make_hexahedron(p0, p1, p2, p3, p4, p5, p6, p7, g);
        },
        py::arg("p0"), py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("p4"), py::arg("p5"), py::arg("p6"),
        py::arg("p7"), py::arg("g"), py::arg("np") = py::dict(),
        "creates an isolated hexahedron with its vertices initialized to p0, p1, ... , and p7, and adds it to the graph g.");

  m.def("make_hexahedron",
        [](const Iso_cuboid_3 &c, Pm &g, const py::dict& np = py::dict()) {
          return CGAL::make_hexahedron(c, g);
        },
        py::arg("c"), py::arg("g"), py::arg("np") = py::dict(),
        "creates an isolated hexahedron equivalent to c, and adds it to the graph g.");

  m.def("make_tetrahedron", &CGAL::make_tetrahedron<Pm, P>,
        py::arg("p0"), py::arg("p1"), py::arg("p2"), py::arg("p3"), py::arg("g"),
        "creates an isolated tetrahedron with its vertices initialized to p0, p1, p2, and p3, and adds it to the graph g.");

  m.def("make_regular_prism",
        [](vertices_size_type nb_vertices, Pm &g, const P &base_center = P(0, 0, 0), FT height = 1.0,
           FT radius = 1.0, bool is_closed = true) {
          return CGAL::make_regular_prism(nb_vertices, g, base_center, height, radius, is_closed);
        },
        py::arg("nb_vertices"), py::arg("g"), py::arg("base_center") = P(0, 0, 0), py::arg("height") = 1.0,
    py::arg("radius") = 1.0, py::arg("is_closed") = true,
        "creates a triangulated regular prism, outward oriented, having nb_vertices vertices in each of its bases and adds it to the graph g.");

  m.def("make_pyramid",
        [](vertices_size_type nb_vertices, Pm &g, const P &base_center = P(0, 0, 0), FT height = 1.0, FT radius = 1.0,
           bool is_closed = true) {
          return CGAL::make_pyramid(nb_vertices, g, base_center, height, radius, is_closed);
        },
        py::arg("nb_vertices"), py::arg("g"), py::arg("base_center") = P(0, 0, 0), py::arg("height") = 1.0,
    py::arg("radius") = 1.0, py::arg("is_closed") = true,
        "creates a pyramid, outward oriented, having nb_vertices vertices in its base and adds it to the graph g.");

  m.def("make_icosahedron",
        [](Pm &g, const P &center = P(0, 0, 0), FT radius = 1) {
          return CGAL::make_icosahedron(g, center, radius);
        },
        py::arg("g"), py::arg("center") = P(0, 0, 0), py::arg("radius") = 1,
        "creates an icosahedron, outward oriented, centered in center and adds it to the graph g.");

  m.def("make_grid",
        [](vertices_size_type i, vertices_size_type j, Pm &g,
           const std::function<P(vertices_size_type, vertices_size_type)> &calculator, bool triangulated = false) {
          // create a struct to hold the calculator
          struct Calculator {
            std::function<P(vertices_size_type, vertices_size_type)> calculator;
            Calculator(const std::function<P(vertices_size_type, vertices_size_type)> &calculator) :
              calculator(calculator)
            {}
            P operator()(vertices_size_type i, vertices_size_type j) const { return calculator(i, j); }
          };
          return CGAL::make_grid(i, j, g, Calculator(calculator), triangulated);
        },
        py::arg("i"), py::arg("j"), py::arg("g"), py::arg("calculator"), py::arg("triangulated") = false,
        "creates a row major ordered grid with i cells along the width and j cells along the height and adds it to the graph g.");
}

} // namespace boost_utils

#endif
