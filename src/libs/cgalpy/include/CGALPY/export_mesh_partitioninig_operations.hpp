#ifndef CGALPY_MESH_PARTITIONING_OPERATIONS_HPP
#define CGALPY_MESH_PARTITIONING_OPERATIONS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/vector.h>

#include <CGAL/boost/graph/alpha_expansion_graphcut.h>
#include <CGAL/boost/graph/selection.h>
#include <CGAL/boost/graph/split_graph_into_polylines.h>

#include "CGALPY/internal.hpp"
#include "CGALPY/Polyline_visitor.hpp"

namespace py = nanobind;

namespace boost_utils {

template <typename C, typename Graph, typename EdgeDoubleMap, typename VertexVectorMap, typename VertexSizetMap>
void define_partitioning_functions(py::module_& m) {
  using GT = boost::graph_traits<Graph>;
  using Vertex = typename GT::vertex_descriptor;

  // m.def("alpha_expansion_graphcut", [](const Graph& input_graph, EdgeDoubleMap& edge_cost_map, // not working
  //                                      VertexVectorMap& vertex_label_cost_map, VertexSizetMap& vertex_label_map,
  //                                      const py::dict& np = py::dict()) {
  //   // return CGAL::alpha_expansion_graphcut(input_graph, edge_cost_map, vertex_label_cost_map, vertex_label_map, internal::parse_named_parameters(np));
  //   if (np.contains("vertex_index_map")) {
  //     auto vim = py::cast<VertexSizetMap>(np["vertex_index_map"]);
  //     return CGAL::alpha_expansion_graphcut(input_graph, edge_cost_map, vertex_label_cost_map, vertex_label_map, internal::parse_named_parameters(np)
  //                                            .vertex_index_map(vim));
  //   }
  //   else {
  //     return CGAL::alpha_expansion_graphcut(input_graph, edge_cost_map, vertex_label_cost_map, vertex_label_map, internal::parse_named_parameters(np));
  //   }
  // }, py::arg("input_graph"), py::arg("edge_cost_map"), py::arg("vertex_label_cost_map"), py::arg("vertex_label_map"), py::arg("np") = py::dict(),
  //       "regularizes a partition of a graph into n labels using the alpha expansion algorithm [1].\n"
  //       "For a graph (V,E), this function computes a partition f that minimizes the following cost function:\n"
  //       "C(f)=∑{v0,v1}∈ECE(v0,v1)+∑v∈VCV(fv)\n"
  //       "where CE(v0,v1) is the edge cost of assigning a different label to v0 and v1, and CV(fv) is the vertex cost of assigning the label f to the vertex v.\n"
  //       "Parameters\n"
  //       "input_graph	the input graph.\n"
  //       "edge_cost_map	a property map providing the weight of each edge.\n"
  //       "vertex_label_map	a property map providing the label of each vertex. This map will be updated by the algorithm with the regularized version of the partition.\n"
  //       "vertex_label_cost_map	a property map providing, for each vertex, an std::vector containing the cost of this vertex to belong to each label. Each std::vector should have the same size n (which is the number of labels), each label being indexed from 0 to n-1. For example, get(vertex_label_cost_map, vd)[label_idx] returns the cost of vertex vd to belong to the label label_idx.\n"
  //       "np	an optional sequence of Named Parameters");

  // TODO: METIS

  export_polyline_visitor<py::module_, typename GT::vertex_descriptor>(m);

  m.def("split_graph_into_polylines", [](const Graph& graph, PolylineVisitor<Vertex>& polyline_visitor, std::function<bool(Vertex, const Graph&)>& is_terminal) {
    struct is_terminal_wrapper {
      std::function<bool(Vertex, const Graph&)> is_terminal;
      bool operator()(Vertex v, const Graph& g) const { return is_terminal(v, g); }
    };
    is_terminal_wrapper is_terminal_wrapper_wrapper{is_terminal};
    return CGAL::split_graph_into_polylines(graph, polyline_visitor, is_terminal_wrapper_wrapper);
  }, py::arg("graph"), py::arg("polyline_visitor"), py::arg("is_terminal"),
        "Splits a graph into polylines.\n"
        "Parameters\n"
        "graph the input graph.\n"
        "polyline_visitor	a PolylineVisitor object.\n"
        "is_terminal a function that returning true if the vertex v of degree 2 is a polyline endpoint and false otherwise.\n");

}

} // namespace boost_utils

#endif // CGALPY_MESH_PARTITIONING_OPERATIONS_HPP
