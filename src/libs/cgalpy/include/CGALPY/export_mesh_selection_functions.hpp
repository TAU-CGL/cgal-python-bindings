#ifndef CGALPY_BOOST_UTILS_EXPORT_MESH_SELECTION_FUNCTIONS_HPP
#define CGALPY_BOOST_UTILS_EXPORT_MESH_SELECTION_FUNCTIONS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/vector.h>

#include <CGAL/boost/graph/selection.h>

#include "CGALPY/internal.hpp"

namespace py = nanobind;

namespace boost_utils {


template <typename C, typename Graph, typename IsEdgeSelectedPMap, typename IsFaceSelectedPMap, typename IsVertexSelectedPMap>
C define_boost_selection_functions(py::module_& m) {
  using EBMap = IsEdgeSelectedPMap;
  using FBMap = IsFaceSelectedPMap;
  using VBMap = IsVertexSelectedPMap;
  using GT = boost::graph_traits<Graph>;
  using Edge = typename GT::edge_descriptor;
  using Face = typename GT::face_descriptor;
  using Vertex = typename GT::vertex_descriptor;
  using ER = std::vector<Edge>;
  using FR = std::vector<Face>;
  using VR = std::vector<Vertex>;

  m.def("expand_edge_selection", [](const ER& selection, Graph& fg, unsigned int k, EBMap& is_selected) {
    std::vector<Edge> out;
    CGAL::expand_edge_selection(selection, fg, k, is_selected, std::back_inserter(out));
    return out;
  }, py::arg("selection"), py::arg("fg"), py::arg("k"), py::arg("is_selected"),
        "expands a selection of edges from edges adjacent to a non-selected edge.\n"
        "This process is applied k times considering all edges added in the previous steps. Two edges are said to be adjacent if they are incident to the same face or vertex. Each new edge added in the selection is added exactly once in out.\n"
        "Parameters\n"
        "selection	the initial selection of edges that will be expanded.\n"
        "fg	the graph containing the selected edges.\n"
        "k	the number of times the expansion procedure is iteratively applied.\n"
        "is_selected	indicates if an edge is part of the selection. It is updated by the function to accommodate new edges added to the selection.\n"
        "Returns\n"
        "out	new edges added to the selection are added exactly once in out.");

  m.def("expand_face_selection", [](const FR& selection, Graph& fg, unsigned int k, FBMap& is_selected) {
    std::vector<Face> out;
    CGAL::expand_face_selection(selection, fg, k, is_selected, std::back_inserter(out));
    return out;
  }, py::arg("selection"), py::arg("fg"), py::arg("k"), py::arg("is_selected"),
        "expands a selection of faces from faces adjacent to a non-selected face.\n"
        "This process is applied k times considering all faces added in the previous steps. Two faces are said to be adjacent if they share a vertex or an edge. Each new face added in the selection is added exactly once in out.\n"
        "Parameters\n"
        "selection	the initial selection of faces that will be expanded.\n"
        "fg	the graph containing the selected faces.\n"
        "k	the number of times the expansion procedure is iteratively applied.\n"
        "is_selected	indicates if a face is part of the selection. It is updated by the function to accommodate new faces added to the selection.\n"
        "Returns\n"
        "out	new faces added to the selection are added exactly once in out.");

  m.def("expand_face_selection_for_removal", [](const FR& faces_to_be_deleted, Graph& tm, FBMap& is_selected) {
    return CGAL::expand_face_selection_for_removal(faces_to_be_deleted, tm, is_selected);
  }, py::arg("faces_to_be_deleted"), py::arg("tm"), py::arg("is_selected"),
        "Expands a selection of faces so that their removal does not create any non manifold vertex.\n"
        "For each vertex that is incident to a selected face, we turn around that vertex and check if there is exactly one set of consecutive selected faces. If not, additional faces around that vertex are selected to match this condition.\n"
        "Parameters\n"
        "faces_to_be_deleted	the range of selected faces.\n"
        "tm	the triangle mesh.\n"
        "is_selected	a property map containing the selected-or-not status of each face of tm. It must associate true to each face of faces_to_be_deleted and false to any other face of tm. It will be modified if the face selection must be expanded.\n"
        "Returns\n"
        "out	faces added to the selection are added exactly once in out.");

  m.def("expand_vertex_selection", [](const VR& selection, Graph& fg, unsigned int k, VBMap& is_selected) {
    std::vector<Vertex> out;
    CGAL::expand_vertex_selection(selection, fg, k, is_selected, std::back_inserter(out));
    return out;
  }, py::arg("selection"), py::arg("fg"), py::arg("k"), py::arg("is_selected"),
        "expands a selection of vertices from vertices adjacent to a non-selected vertex.\n"
        "This process is applied k times considering all vertices added in the previous steps. Two vertices are said to be adjacent if they are part of the same face. Each new vertex added in the selection is added exactly once in out.\n"
        "Parameters\n"
        "selection	the initial selection of vertices that will be expanded.\n"
        "fg	the graph containing the selected vertices.\n"
        "k	the number of times the expansion procedure is iteratively applied.\n"
        "is_selected	indicates if a vertex is part of the selection. It is updated by the function to accommodate new vertices added to the selection.\n"
        "Returns\n"
        "out	new vertices added to the selection are added exactly once in out.");

  m.def("reduce_edge_selection", [](const ER& selection, Graph& fg, unsigned int k, EBMap& is_selected) {
    std::vector<Edge> out;
    CGAL::reduce_edge_selection(selection, fg, k, is_selected, std::back_inserter(out));
    return out;
  }, py::arg("selection"), py::arg("fg"), py::arg("k"), py::arg("is_selected"),
        "diminishes a selection of edges from edges adjacent to a non-selected edge.\n"
        "This process is applied k times considering all edges removed in the previous steps. Two edges are said to be adjacent if they are incident to the same face or vertex. Each edge removed from the selection is added exactly once in out.\n"
        "Parameters\n"
        "selection	the initial selection of edges that will be reduced.\n"
        "fg	the graph containing the selected edges.\n"
        "k	the number of times the reduction procedure is iteratively applied.\n"
        "is_selected	indicates if an edge is part of the selection. It is updated by the function to accommodate edges removed from the selection.\n"
        "Returns\n"
        "out	edges removed from the selection are added exactly once in out.");
  m.def("reduce_face_selection", [](const FR& selection, Graph& fg, unsigned int k, FBMap& is_selected) {
    std::vector<Face> out;
    CGAL::reduce_face_selection(selection, fg, k, is_selected, std::back_inserter(out));
    return out;
  }, py::arg("selection"), py::arg("fg"), py::arg("k"), py::arg("is_selected"),
        "diminishes a selection of faces from faces adjacent to a non-selected face.\n"
        "This process is applied k times considering all faces removed in the previous steps. Two faces are said to be adjacent if they share a vertex or an edge. Each face removed from the selection is added exactly once in out.\n"
        "Parameters\n"
        "selection	the initial selection of faces that will be expanded.\n"
        "fg	the graph containing the selected faces.\n"
        "k	the number of times the reduction procedure is iteratively applied.\n"
        "is_selected	indicates if a face is part of the selection. It is updated by the function to accommodate faces removed from the selection.\n"
        "Returns\n"
        "out	faces removed from the selection are added exactly once in out.");
  m.def("reduce_vertex_selection", [](const VR& selection, Graph& fg, unsigned int k, VBMap& is_selected) {
    std::vector<Vertex> out;
    CGAL::reduce_vertex_selection(selection, fg, k, is_selected, std::back_inserter(out));
    return out;
  }, py::arg("selection"), py::arg("fg"), py::arg("k"), py::arg("is_selected"),
        "diminishes a selection of vertices from vertices adjacent to a non-selected vertex.\n"
        "This process is applied k times considering all vertices removed in the previous steps. Two vertices are said to be adjacent if they are part of the same face. Each vertex removed from the selection is added exactly once in out.\n"
        "Parameters\n"
        "selection	the initial selection of vertices that will be reduced.\n"
        "fg	the graph containing the selected vertices.\n"
        "k	the number of times the reduction procedure is iteratively applied.\n"
        "is_selected	indicates if a vertex is part of the selection. It is updated by the function to accommodate vertices removed from the selection.\n"
        "Returns\n"
        "out	vertices removed from the selection are added exactly once in out.");

  m.def("regularize_face_selection_borders", [](Graph& mesh, FBMap& is_selected, double weight, const py::dict& np = py::dict()) {
    // return CGAL::regularize_face_selection_borders(mesh, is_selected, weight, internal::parse_named_parameters(np));
    if (np.contains("face_index_map")) {
      auto fim = py::cast<FBMap>(np["face_index_map"]);
      return CGAL::regularize_face_selection_borders(mesh, is_selected, weight, internal::parse_named_parameters(np)
                                                     .face_index_map(fim));
    }
    else {
      return CGAL::regularize_face_selection_borders(mesh, is_selected, weight, internal::parse_named_parameters(np));
    }
  }, py::arg("mesh"), py::arg("is_selected"), py::arg("weight"), py::arg("np") = py::dict(),
        "regularizes a selection in order to minimize the length of the border of the selection.\n"
        "The alpha expansion algorithm is used (see CGAL::alpha_expansion_graphcut()) using the length of the edge between two faces as the edge cost and the initial selected/unselected property of a face as the face cost.\n"
        "if prevent_unselection is set to true, the cost of unselecting a face is set to infinity, which forces the regularization to only select new faces and ensures that the regularization keeps all selected faces.\n"
        "Parameters\n"
        "mesh	the mesh containing the selected faces.\n"
        "is_selected	indicates if a face is part of the selection. It is updated by the function to accommodate faces added or removed from the selection.\n"
        "weight	sets the tradeoff between data fidelity and regularity, ranging from 0 (no regularization at all, selection is left unaltered) to 1 (maximum regularization, usually selects or unselects everything so that the length of the border of the selection is 0)\n");




  return m;
}

} // namespace boost_utils

#endif // CGALPY_BOOST_UTILS_EXPORT_MESH_ITERATORS_HPP

