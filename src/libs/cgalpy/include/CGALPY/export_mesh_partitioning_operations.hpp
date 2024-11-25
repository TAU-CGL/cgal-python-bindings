// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#ifndef CGALPY_EXPORT_MESH_PARTITIONING_OPERATIONS_HPP
#define CGALPY_EXPORT_MESH_PARTITIONING_OPERATIONS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/function.h>

#include <CGAL/boost/graph/split_graph_into_polylines.h>
#include <CGAL/boost/graph/alpha_expansion_graphcut.h>

#include "CGALPY/Polyline_visitor.hpp"
#include "CGALPY/parse_named_parameters.hpp"

namespace py = nanobind;

namespace boost_utils {

template <typename C, typename Graph, typename EdgeDoubleMap, typename VertexVectorDoubleMap, typename VertexSizeTMap>
C define_boost_partitioning_operations(C& c) {
  using namespace boost_utils;
  using Visitor = Polyline_visitor<Graph>;

  using Gt = boost::graph_traits<Graph>;
  using Vd = typename Gt::vertex_descriptor;

  c.def("alpha_expansion_graphcut", [](const Graph& g, EdgeDoubleMap& edge_cost_map, VertexVectorDoubleMap& vertex_label_cost_map, VertexSizeTMap& vertex_label_map, const py::dict& np = py::dict()) {
    // return CGAL::alpha_expansion_graphcut(g, edge_cost_map, vertex_label_cost_map, vertex_label_map, internal::parse_named_parameters(np));
    if (np.contains("vertex_index_map")) {
      auto vim = py::cast<VertexSizeTMap>(np["vertex_index_map"]);
      return CGAL::alpha_expansion_graphcut(g, edge_cost_map, vertex_label_cost_map, vertex_label_map, internal::parse_named_parameters(np)
                                             .vertex_index_map(vim));
    }
    else {
      return CGAL::alpha_expansion_graphcut(g, edge_cost_map, vertex_label_cost_map, vertex_label_map, internal::parse_named_parameters(np));
    }
  }, py::arg("g"), py::arg("edge_cost_map"), py::arg("vertex_label_cost_map"), py::arg("vertex_label_map"), py::arg("np") = py::dict(),
        "regularizes a partition of a graph into n labels using the alpha expansion algorithm [1].\n"
        "For a graph (V,E), this function computes a partition f that minimizes the following cost function:\n"
        "C(f)=∑{v0,v1}∈ECE(v0,v1)+∑v∈VCV(fv)\n"
        "where CE(v0,v1) is the edge cost of assigning a different label to v0 and v1, and CV(fv) is the vertex cost of assigning the label f to the vertex v.\n"
        "Parameters\n"
        "• g: the input graph.\n"
        "• edge_cost_map: a property map providing the weight of each edge.\n"
        "• vertex_label_cost_map: a property map providing, for each vertex, a list containing the cost of this vertex to belong to each label. Each list should have the same size n (which is the number of labels), each label being indexed from 0 to n-1. For example, get(vertex_label_cost_map, vd)[label_idx] returns the cost of vertex vd to belong to the label label_idx.");

    // TODO: add METIS

  py::class_<Visitor> pv(c, "Polyline_visitor");
  pv.def(py::init<>())
    .def("set_start_new_polyline", &Visitor::set_start_new_polyline,
         py::arg("fn"),
         "set function for start_new_polyline()\n")
    .def("set_add_node", &Visitor::set_add_node,
         py::arg("fn"),
         "set function for add_node()\n")
    .def("set_end_polyline", &Visitor::set_end_polyline,
         py::arg("fn"),
         "set function for end_polyline()\n");

  c.def("split_graph_into_polylines", [](const Graph& g, Visitor& pv, const std::function<bool(const Vd&, const Graph&)>& is_terminal) {
    struct IsTerminal {
      IsTerminal(const std::function<bool(const Vd&, const Graph&)>& is_terminal) : is_terminal(is_terminal) {}
      bool operator()(const Vd& v, const Graph& g) const { return is_terminal(v, g); }
      std::function<bool(const Vd&, const Graph&)> is_terminal;
    };
    return CGAL::split_graph_into_polylines(g, pv, IsTerminal(is_terminal));
  }, py::arg("g"), py::arg("pv"), py::arg("is_terminal"),
        "splits into polylines the graph g at vertices of degree greater than 2 and at vertices for which is_terminal(v,graph)==true.\n"
        "The polylines are reported using a visitor.\n"
        "An overload without is_terminal is provided if no vertices but those of degree different from 2 are polyline endpoints.\n"
        "Examples\n"
        "• Surface_mesh_skeletonization/simple_mcfskel_example.py.");

  c.def("split_graph_into_polylines", [](const Graph& g, Visitor& pv) {
    return CGAL::split_graph_into_polylines(g, pv);
  }, py::arg("g"), py::arg("pv"),
        "splits into polylines the graph g at vertices of degree greater than 2 and at vertices for which is_terminal(v,graph)==true.\n"
        "The polylines are reported using a visitor.\n"
        "An overload without is_terminal is provided if no vertices but those of degree different from 2 are polyline endpoints.\n"
        "Examples\n"
        "• Surface_mesh_skeletonization/simple_mcfskel_example.py.");

  return c;
}



} // namespace boost_utils

#endif // CGALPY_EXPORT_MESH_PARTITIONING_OPERATIONS_HPP
