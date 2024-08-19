#ifndef CGALPY_BOOST_UTILS_EXPORT_MESH_HELPERS_HPP
#define CGALPY_BOOST_UTILS_EXPORT_MESH_HELPERS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>

#include <CGAL/boost/graph/copy_face_graph.h>
#include <CGAL/boost/graph/helpers.h>

#include "CGALPY/internal.hpp"

namespace py = nanobind;

namespace boost_utils {

template<typename Graph, typename Graph2>
auto my_copy_face_graph(const Graph& sm, Graph2& tm, const py::dict& np1 = py::dict(), const py::dict& np2 = py::dict()) {
  // TODO: handle nps
  return CGAL::copy_face_graph<Graph, Graph>(sm, tm, internal::parse_named_parameters(np1), internal::parse_named_parameters(np2));
}

template<typename C, typename Graph1, typename Graph2>
C define_boost_helpers(py::module_& m) {
  using Graph = Graph1;
  using namespace boost_utils;

  m.def("clear", &CGAL::clear<Graph>,
        "removes all vertices, faces and halfedges from a graph\n"
        "Calls remove_vertex(), remove_edge(), and remove_face(), for each vertex, edge, and face.\n"
        "If the graph has a member function clear(), it will be called instead.\n"
        "\n"
        "Warning\n"
        "If it exists, the clear() function of a graph might do more than simply remove elements. For example, clear() collects garbage and removes all property maps added by a call to add_property_map() for all simplex types.\n"
        "\n"
        "Parameters\n"
        "g  the graph to clear\n",
        py::arg("g"));

  m.def("copy_face_graph", &my_copy_face_graph<Graph1, Graph2>,
        "copies a source model into a target.\n"
        "\n"
        "Parameters\n"
        "sm  the source mesh\n"
        "tm  the target mesh\n"
        "np1  an optional sequence of Named Parameters among the ones listed below\n"
        "np2  an optional sequence of Named Parameters among the ones listed below\n",
        py::arg("sm"), py::arg("tm"), py::arg("np1") = py::dict(), py::arg("np2") = py::dict());

  m.def("halfedge_index_in_face", &CGAL::halfedge_index_in_face<Graph>,
        "returns the number of calls to next(hd, tm) one has to apply to hd for hd == he to be true, starting from hd = halfedge(face(he, tm), tm).\n"
        "\n"
        "Parameters\n"
        "he  a halfedge of g whose index in face(he, tm) is sought\n"
        "g  an object of type Graph\n",
        py::arg("he"), py::arg("g"));

  m.def("is_empty", &CGAL::is_empty<Graph>,
        "checks whether the graph is empty, by checking that it does not contain any vertex.\n"
        "\n"
        "Parameters\n"
        "g  the graph to test\n",
        py::arg("g"));

  m.def("is_isolated_triangle", &CGAL::is_isolated_triangle<Graph>,
        "returns true iff the connected component denoted by hd is a triangle.\n"
        "\n"
        "Precondition\n"
        "g must be valid.\n",
        py::arg("hd"), py::arg("g"));

  m.def("is_valid_face_graph", &CGAL::is_valid_face_graph<Graph>,
        "checks the integrity of the graph g.\n"
        "The graph g is a valid face graph if it is a valid halfedge graph, and if it follows the rules of the FaceListGraph concept and all of its associations are reciprocal (for example, face(halfedge(f,g),g) must be f).\n"
        "\n"
        "Parameters\n"
        "g  the graph to test\n"
        "verb  if true, the details of the check will be written in the standard output\n",
        "Returns\n"
        "true if g is valid, false otherwise.\n",
        py::arg("g"), py::arg("verb") = false);

  m.def("is_valid_halfedge_graph", &CGAL::is_valid_halfedge_graph<Graph>,
        "checks the integrity of the graph g.\n"
        "The graph g is valid if it follows the rules of the HalfedgeListGraph concept and all of its associations are reciprocal (for example, prev(next(h, g), g) must be h, and next(prev(h, g), g) must be h).\n"
        "\n"
        "Parameters\n"
        "g  the graph to test\n"
        "verb  if true, the details of the check will be written in the standard output.\n",
        "Returns\n"
        "true if g is valid, false otherwise.\n",
        py::arg("g"), py::arg("verb") = false);

  m.def("is_valid_polygon_mesh", &CGAL::is_valid_polygon_mesh<Graph>,
        "checks the integrity of the mesh g.\n"
        "The mesh g is a valid polygon mesh if it is a valid face graph and if it follows the rules defined in PolygonMesh.\n"
        "\n"
        "Parameters\n"
        "g  the Mesh to test\n"
        "verb  if true, the details of the check will be written in the standard output\n"
        "\n"
        "Returns\n"
        "true if g is valid, false otherwise.\n",
        py::arg("g"), py::arg("verb") = false);

  m.def("remove_all_elements", &CGAL::remove_all_elements<Graph>,
        "removes all vertices, faces and halfedges from a graph.\n"
        "Calls remove_vertex(), remove_edge(), and remove_face(), for each vertex, edge, and face.\n"
        "Note that some graphs have a specialized version of this function to improve complexity.\n"
        "\n"
        "Warning\n"
        "This function does not perform anything more than what is advertised above. It is up to the user to e.g. clean garbage or remove internal property maps (if relevant, and desired).\n"
        "\n"
        "Parameters\n"
        "g  the graph whose elements will be removed\n",
        py::arg("g"));

  m.def("vertex_index_in_face", &CGAL::vertex_index_in_face<Graph>,
        "returns the number of calls to next() one has to apply to the halfedge hd for source(hd, mesh) == vd to be true, starting from hd = halfedge(fd, tm).\n"
        "\n"
        "Parameters\n"
        "vd  a vertex of g whose index is sought\n"
        "fd  a face of g in which the index of vd is sought\n"
        "g  a mesh of type Graph\n"
        "\n"
        "Precondition\n"
        "vd is a vertex of fd.\n",
        py::arg("vd"), py::arg("fd"), py::arg("g"));

  return m;
}

} // namespace boost_utils

#endif // CGALPY_BOOST_UTILS_EXPORT_MESH_HELPERS_HPP


// Function Documentation
// ◆ clear()
// template<typename FaceGraph >
// void CGAL::clear 	( 	FaceGraph &  	g	) 	
//
// #include <CGAL/boost/graph/helpers.h>
//
// removes all vertices, faces and halfedges from a graph.
//
// Calls remove_vertex(), remove_edge(), and remove_face(), for each vertex, edge, and face.
//
// If the graph has a member function clear(), it will be called instead.
//
// Warning
//     If it exists, the clear() function of a graph might do more than simply remove elements. For example, CGAL::Surface_mesh::clear() collects garbage and removes all property maps added by a call to CGAL::Surface_mesh::add_property_map() for all simplex types.
//
// Template Parameters
//     FaceGraph	model of MutableHalfedgeGraph and MutableFaceGraph
//
// Parameters
//     g	the graph to clear
//
// See also
//     CGAL::remove_all_elements() 
//
// ◆ copy_face_graph()
// template<typename SourceMesh , typename TargetMesh , typename NamedParameters1 = parameters::Default_named_parameters, typename NamedParameters2 = parameters::Default_named_parameters>
// void CGAL::copy_face_graph 	( 	const SourceMesh &  	sm,
// 		TargetMesh &  	tm,
// 		const NamedParameters1 &  	np1 = parameters::default_values(),
// 		const NamedParameters2 &  	np2 = parameters::default_values() 
// 	) 		
//
// #include <CGAL/boost/graph/copy_face_graph.h>
//
// copies a source model of FaceListGraph into a target model of a FaceListGraph.
//
// OutputIterators can be provided to produce a mapping between source and target elements. The target graph is not cleared.
//
// Template Parameters
//     SourceMesh	a model of FaceListGraph. The descriptor types boost::graph_traits<SourceMesh>::vertex_descriptor and boost::graph_traits<SourceMesh>::face_descriptor must be models of Hashable.
//     TargetMesh	a model of FaceListGraph
//     NamedParameters1	a sequence of Named Parameters
//     NamedParameters2	a sequence of Named Parameters
//
// The types sm_vertex_descriptor and sm_face_descriptor must be models of the concept Hashable.
//
// Parameters
//     sm	the source mesh
//     tm	the target mesh
//     np1	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     vertex_point_map
//
//         a property map associating points to the vertices of sm
//         Type: a class model of ReadWritePropertyMap with boost::graph_traits<SourceMesh>::vertex_descriptor as key type and Point_3 as value type
//         Default: boost::get(CGAL::vertex_point, sm)
//         Extra: If this parameter is omitted, an internal property map for CGAL::vertex_point_t must be available in SourceMesh.
//
//     	
//
//     vertex_to_vertex_output_iterator
//
//         an OutputIterator containing the pairs source-vertex, target-vertex.
//         Type: a class model of OutputIterator accepting std::pair<boost::graph_traits<SourceMesh>::vertex_descriptor, boost::graph_traits<TargetMesh>::vertex_descriptor>
//         Default: Emptyset_iterator
//         Extra: If this parameter is given, then vertex_to_vertex_map cannot be used.
//
//     	
//
//     halfedge_to_halfedge_output_iterator
//
//         an OutputIterator containing the pairs source-halfedge, target-halfedge.
//         Type: a class model of OutputIterator accepting std::pair<boost::graph_traits<SourceMesh>::halfedge_descriptor, boost::graph_traits<TargetMesh>::halfedge_descriptor>
//         Default: Emptyset_iterator
//         Extra: If this parameter is given, then halfedge_to_halfedge_map cannot be used.
//
//     	
//
//     face_to_face_output_iterator
//
//         an OutputIterator containing the pairs source-face, target-face.
//         Type: a class model of OutputIterator accepting std::pair<boost::graph_traits<SourceMesh>::face_descriptor, boost::graph_traits<TargetMesh>::face_descriptor>
//         Default: Emptyset_iterator
//         Extra: If this parameter is given, then face_to_face_map cannot be used.
//
//     	
//
//     vertex_to_vertex_map
//
//         a property map storing for each vertex of a source mesh the corresponding vertex of another mesh
//         Type: a class model of ReadWritePropertyMap with boost::graph_traits<SourceMesh>::vertex_descriptor as key type and boost::graph_traits<TargetMesh>::vertex_descriptor as value type.
//         Default: unused
//         Extra: A typical use case is mapping the vertices from a source mesh to its copy's after a copy_face_graph() operation.
//
//     	
//
//     halfedge_to_halfedge_map
//
//         a property map storing for each halfedge of a source mesh the corresponding halfedge of another mesh
//         Type: a class model of ReadWritePropertyMap with boost::graph_traits<SourceMesh>::halfedge_descriptor as key type and boost::graph_traits<TargetMesh>::halfedge_descriptor as value type
//         Default: unused
//         Extra: A typical use case is mapping the halfedges from a source mesh to its copy's after a copy_face_graph()operation.
//
//     	
//
//     face_to_face_map
//
//         a property map storing for each face of a source mesh the corresponding face of another mesh
//         Type: a class model of ReadWritePropertyMap with boost::graph_traits<SourceMesh>::face_descriptor as key type and boost::graph_traits<TargetMesh>::face_descriptor as value type
//         Default: unused
//         Extra: A typical use case is mapping the faces from a source mesh to its copy's after a copy_face_graph() operation
//
//     	
//
// Parameters
//     np2	an optional sequence of Named Parameters among the ones listed below
//
// Optional Named Parameters
//     vertex_point_map
//
//         a property map associating points to the vertices of tm
//         Type: a class model of ReadWritePropertyMap with boost::graph_traits<TargetMesh>::vertex_descriptor as key type and Point_3 as value type
//         Default: boost::get(CGAL::vertex_point, tm)
//         Extra: If this parameter is omitted, an internal property map for CGAL::vertex_point_t must be available in TargetMesh.
//
//     	
//
// The points from sm to tm are converted using CGAL::Cartesian_converter<SourceKernel, TargetKernel>. SourceKernel and TargetKernel are deduced using CGAL::Kernel_traits from the value types of the vertex point maps.
//
// Other properties are not copied.
//
// Examples
//     BGL_polyhedron_3/copy_polyhedron.cpp, BGL_surface_mesh/surface_mesh_partition.cpp, and Surface_mesh_segmentation/extract_segmentation_into_mesh_example.cpp.
//
// ◆ halfedge_index_in_face()
// template<typename Graph >
// int CGAL::halfedge_index_in_face 	( 	typename boost::graph_traits< Graph >::halfedge_descriptor  	he,
// 		const Graph &  	g 
// 	) 		
//
// #include <CGAL/boost/graph/helpers.h>
//
// returns the number of calls to next(hd, tm) one has to apply to hd for hd == he to be true, starting from hd = halfedge(face(he, tm), tm).
//
// Template Parameters
//     Graph	a model of FaceGraph.
//
// Parameters
//     he	a halfedge of g whose index in face(he, tm) is sought
//     g	an object of type Graph
//
// ◆ is_empty()
// template<typename FaceGraph >
// bool CGAL::is_empty 	( 	const FaceGraph &  	g	) 	
//
// #include <CGAL/boost/graph/helpers.h>
//
// checks whether the graph is empty, by checking that it does not contain any vertex.
//
// Template Parameters
//     FaceGraph	model of FaceGraph
//
// Parameters
//     g	the graph to test
//
// ◆ is_isolated_triangle()
// template<typename FaceGraph >
// bool CGAL::is_isolated_triangle 	( 	typename boost::graph_traits< FaceGraph >::halfedge_descriptor  	hd,
// 		const FaceGraph &  	g 
// 	) 		
//
// #include <CGAL/boost/graph/helpers.h>
//
// returns true iff the connected component denoted by hd is a triangle.
//
// Precondition
//     g must be valid. 
//
// ◆ is_valid_face_graph()
// template<typename FaceGraph >
// bool CGAL::is_valid_face_graph 	( 	const FaceGraph &  	g,
// 		bool  	verb = false 
// 	) 		
//
// #include <CGAL/boost/graph/helpers.h>
//
// checks the integrity of the graph g.
//
// The graph g is a valid face graph if it is a valid halfedge graph, and if it follows the rules of the FaceListGraph concept and all of its associations are reciprocal (for example, face(halfedge(f,g),g) must be f).
//
// Parameters
//     g	the graph to test
//     verb	if true, the details of the check will be written in the standard output
//
// Template Parameters
//     FaceGraph	a model of FaceListGraph and HalfedgeListGraph
//
// Returns
//     true if g is valid, false otherwise.
//
// See also
//     is_valid_halfedge_graph() 
//
// ◆ is_valid_halfedge_graph()
// template<typename Graph >
// bool CGAL::is_valid_halfedge_graph 	( 	const Graph &  	g,
// 		bool  	verb = false 
// 	) 		
//
// #include <CGAL/boost/graph/helpers.h>
//
// checks the integrity of the graph g.
//
// The graph g is valid if it follows the rules of the HalfedgeListGraph concept and all of its associations are reciprocal (for example, prev(next(h, g), g) must be h, and next(prev(h, g), g) must be h).
//
// Parameters
//     g	the graph to test
//     verb	if true, the details of the check will be written in the standard output.
//
// Template Parameters
//     Graph	a model of HalfedgeListGraph
//
// Returns
//     true if g is valid, false otherwise. 
//
// ◆ is_valid_polygon_mesh()
// template<typename Mesh >
// bool CGAL::is_valid_polygon_mesh 	( 	const Mesh &  	g,
// 		bool  	verb = false 
// 	) 		
//
// #include <CGAL/boost/graph/helpers.h>
//
// checks the integrity of the mesh g.
//
// The mesh g is a valid polygon mesh if it is a valid face graph and if it follows the rules defined in PolygonMesh.
//
// Parameters
//     g	the Mesh to test
//     verb	if true, the details of the check will be written in the standard output
//
// Template Parameters
//     Mesh	a model of FaceListGraph and HalfedgeListGraph
//
// Returns
//     true if g is valid, false otherwise.
//
// See also
//     is_valid_face_graph() 
//     is_valid_halfedge_graph() 
//
// Examples
//     BGL_polyhedron_3/copy_polyhedron.cpp.
//
// ◆ remove_all_elements()
// template<typename FaceGraph >
// void CGAL::remove_all_elements 	( 	FaceGraph &  	g	) 	
//
// #include <CGAL/boost/graph/helpers.h>
//
// removes all vertices, faces and halfedges from a graph.
//
// Calls remove_vertex(), remove_edge(), and remove_face(), for each vertex, edge, and face.
//
// Note that some graphs have a specialized version of this function to improve complexity.
//
// Warning
//     This function does not perform anything more than what is advertised above. It is up to the user to e.g. clean garbage or remove internal property maps (if relevant, and desired).
//
// Template Parameters
//     FaceGraph	model of MutableHalfedgeGraph and MutableFaceGraph
//
// Parameters
//     g	the graph whose elements will be removed
//
// See also
//     CGAL::clear() 
//
// ◆ vertex_index_in_face()
// template<typename Graph >
// int CGAL::vertex_index_in_face 	( 	const typename boost::graph_traits< Graph >::vertex_descriptor  	vd,
// 		const typename boost::graph_traits< Graph >::face_descriptor  	fd,
// 		const Graph &  	g 
// 	) 		
//
// #include <CGAL/boost/graph/helpers.h>
//
// returns the number of calls to next() one has to apply to the halfedge hd for source(hd, mesh) == vd to be true, starting from hd = halfedge(fd, tm).
//
// Template Parameters
//     Graph	a model of FaceGraph
//
// Parameters
//     vd	a vertex of g whose index is sought
//     fd	a face of g in which the index of vd is sought
//     g	a mesh of type Graph
//
// Precondition
//     vd is a vertex of fd. 
//
