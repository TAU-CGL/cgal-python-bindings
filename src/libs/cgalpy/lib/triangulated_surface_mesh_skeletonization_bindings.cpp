// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Radoslaw Dabkowski <radekaadek@gmail.com

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/vector.h>
#include <nanobind/make_iterator.h>
#include <nanobind/stl/list.h>

#include <CGAL/Mean_curvature_flow_skeletonization.h>
#include <CGAL/extract_mean_curvature_flow_skeleton.h>
#include <CGAL/boost/graph/split_graph_into_polylines.h>

#include "CGALPY/polygon_mesh_processing_types.hpp"
#include "CGALPY/Polyline_visitor.hpp"

namespace py = nanobind;

void export_triangulated_surface_mesh_skeletonization(py::module_& m) {
  using Tm = pmp::Polygonal_mesh;
  using Gt = boost::graph_traits<Tm>;
  using Fd = boost::graph_traits<Tm>::face_descriptor;
  using Hd = boost::graph_traits<Tm>::halfedge_descriptor;
  using Vd = boost::graph_traits<Tm>::vertex_descriptor;
  using Ed = boost::graph_traits<Tm>::edge_descriptor;
  using vertices_size_type = boost::graph_traits<Tm>::vertices_size_type;
  using Mean_curvature_flow_skeletonization = CGAL::Mean_curvature_flow_skeletonization<Tm>;
  using Mcfs_vmap = Mean_curvature_flow_skeletonization::Vmap;
  using Meso_skeleton = Mean_curvature_flow_skeletonization::Meso_skeleton;
  using Skeleton = Mean_curvature_flow_skeletonization::Skeleton;
  using Skeleton_vertex = Skeleton::vertex_descriptor;
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Mean_curvature_flow_skeletonization> skeletonization(m, "Mean_curvature_flow_skeletonization");
  skeletonization
    // Constructor
    .def(py::init<const Tm&>(), // TODO: add vertex_point_map
         py::arg("tmesh"),
         "The constructor of a skeletonization object.\n"
         "The algorithm parameters are initialized such that:\n"
         "• max_triangle_angle() == 110\n"
         "• quality_speed_tradeoff() == 0.1\n"
         "• medially_centered_speed_tradeoff() == 0.2\n"
         "• area_variation_factor() == 0.0001\n"
         "• max_iterations() == 500\n"
         "• is_medially_centered() == true\n"
         "• min_edge_length() == 0.002 * the length of the diagonal of the bounding box of tmesh\n"
         "\n"
         "Precondition\n"
         "• tmesh is a triangulated surface mesh without borders and has exactly one connected component.\n"
         "\n"
         "Parameters\n"
         "• tmesh:	input triangulated surface mesh.\n")
    // Local Remeshing Parameters
    .def("max_triangle_angle", &CGAL::Mean_curvature_flow_skeletonization<Tm>::max_triangle_angle,
         "During the local remeshing step, a triangle will be split if it has an angle larger than max_triangle_angle().\n")
    .def("min_edge_length", &CGAL::Mean_curvature_flow_skeletonization<Tm>::min_edge_length,
         "During the local remeshing step, an edge will be collapse if it is length is less than min_edge_length().\n")
    .def("set_max_triangle_angle", &CGAL::Mean_curvature_flow_skeletonization<Tm>::set_max_triangle_angle,
         py::arg("value"),
         "set function for max_triangle_angle()\n")
    .def("set_min_edge_length", &CGAL::Mean_curvature_flow_skeletonization<Tm>::set_min_edge_length,
         py::arg("value"),
         "set function for min_edge_length()\n")
    // Algorithm Termination Parameters
    .def("max_iterations", &CGAL::Mean_curvature_flow_skeletonization<Tm>::max_iterations,
         "Maximum number of iterations performed by contract_until_convergence().\n")
    .def("area_variation_factor", &CGAL::Mean_curvature_flow_skeletonization<Tm>::area_variation_factor,
         "The convergence is considered to be reached if the variation of the area of the meso-skeleton after one iteration is smaller than area_variation_factor()*original_area where original_area is the area of the input triangle mesh.\n")
    .def("set_max_iterations", &CGAL::Mean_curvature_flow_skeletonization<Tm>::set_max_iterations,
         py::arg("value"),
         "set function for max_iterations()\n")
    .def("set_area_variation_factor", &CGAL::Mean_curvature_flow_skeletonization<Tm>::set_area_variation_factor,
         py::arg("value"),
         "set function for area_variation_factor()\n")
    // Vertex Motion Parameters
    .def("quality_speed_tradeoff", &CGAL::Mean_curvature_flow_skeletonization<Tm>::quality_speed_tradeoff,
         "This is an advanced function.\n"
         "Advanced\n"
         "\n"
         "Controls the velocity of movement and approximation quality: decreasing this value makes the mean curvature flow based contraction converge faster, but results in a skeleton of lower quality. This parameter corresponds to wH in the original publication.\n")
    .def("is_medially_centered", &CGAL::Mean_curvature_flow_skeletonization<Tm>::is_medially_centered,
         "If true, the meso-skeleton placement will be attracted by an approximation of the medial axis of the mesh during the contraction steps, so will be the result skeleton.\n")
    .def("medially_centered_speed_tradeoff", &CGAL::Mean_curvature_flow_skeletonization<Tm>::medially_centered_speed_tradeoff,
         "This is an advanced function.\n"
         "Advanced\n"
         "\n"
         "Controls the smoothness of the medial approximation: increasing this value results in a (less smooth) skeleton closer to the medial axis, as well as a lower convergence speed. It is only used if is_medially_centered()==true. This parameter corresponds to wM in the original publication.\n")
    .def("set_quality_speed_tradeoff", &CGAL::Mean_curvature_flow_skeletonization<Tm>::set_quality_speed_tradeoff,
         py::arg("value"),
         "set function for quality_speed_tradeoff()\n")
    .def("set_is_medially_centered", &CGAL::Mean_curvature_flow_skeletonization<Tm>::set_is_medially_centered,
         py::arg("value"),
         "set function for is_medially_centered()\n")
    .def("set_medially_centered_speed_tradeoff", &CGAL::Mean_curvature_flow_skeletonization<Tm>::set_medially_centered_speed_tradeoff,
         py::arg("value"),
         "set function for medially_centered_speed_tradeoff()\n")
    // TODO: what is the type of VertexIterator?
    // .def("set_fixed_vertices", &CGAL::Mean_curvature_flow_skeletonization<Tm>::set_fixed_vertices<VertexIterator>,
    //      py::arg("begin"), py::arg("end"),
    //      "This is an advanced function.\n"
    //      "Advanced\n"
    //      "\n"
    //      "sets the vertices in the range [begin, end) as fixed. Fixed vertices will not be moved during contraction and this will therefore prevent convergence towards the skeleton if contract_until_convergence() is used. It is useful only if the end goal is to retrieve the meso-skeleton after a number of contract_geometry(), keeping the specified vertices fixed in place.\n"
    //      "\n");
    // High Level Functions
    .def("__call__", &CGAL::Mean_curvature_flow_skeletonization<Tm>::operator(),
        py::arg("skeleton"),
         "Creates the curve skeleton: the input surface mesh is iteratively contracted until convergence, and then turned into a curve skeleton.\n"
         "\n"
         "This is equivalent to calling contract_until_convergence() and convert_to_skeleton().\n"
         "\n"
         "Parameters\n"
         "• skeleton: graph that will contain the skeleton of the input triangulated surface mesh. For each vertex descriptor vd of skeleton, the corresponding point and the set of input vertices that contracted to vd can be retrieved using skeleton[vd].point and skeleton[vd].vertices respectively.\n")
    // Low Level Functions
    .def("contract_geometry", &CGAL::Mean_curvature_flow_skeletonization<Tm>::contract_geometry,
         "Runs one contraction step following the mean curvature flow.\n")
    .def("collapse_edges", &CGAL::Mean_curvature_flow_skeletonization<Tm>::collapse_edges,
         "Collapses edges of the meso-skeleton with length less than min_edge_length() and returns the number of edges collapsed.\n")
    .def("split_faces", &CGAL::Mean_curvature_flow_skeletonization<Tm>::split_faces,
         "Splits faces of the meso-skeleton having one angle greater than max_triangle_angle() and returns the number of faces split.\n")
    .def("detect_degeneracies", &CGAL::Mean_curvature_flow_skeletonization<Tm>::detect_degeneracies,
         "Prevents degenerate vertices to move during the following contraction steps and returns the number of newly fixed vertices.\n")
    .def("contract", &CGAL::Mean_curvature_flow_skeletonization<Tm>::contract,
         "Performs subsequent calls to contract_geometry(), collapse_edges(), split_faces() and detect_degeneracies()\n")
    .def("contract_until_convergence", &CGAL::Mean_curvature_flow_skeletonization<Tm>::contract_until_convergence,
         "Iteratively calls contract() until the change of surface area of the meso-skeleton after one iteration is smaller than area_variation_factor()*original_area where original_area is the area of the input triangle mesh, or if the maximum number of iterations has been reached.\n")
    .def("convert_to_skeleton", &CGAL::Mean_curvature_flow_skeletonization<Tm>::convert_to_skeleton,
         py::arg("skeleton"),
         "Converts the contracted surface mesh to a skeleton curve.\n"
         "\n"
         "Parameters\n"
         "• skeleton	graph that will contain the skeleton of tmesh. It should be empty before passed to the function.\n")
    // Access to the Meso-Skeleton
    .def("meso_skeleton", &CGAL::Mean_curvature_flow_skeletonization<Tm>::meso_skeleton,
         "Access to the collapsed triangulated surface mesh.\n")
    ;

  py::class_<Mcfs_vmap> vmap(skeletonization, "Vmap");
  vmap.def_rw("point", &Mcfs_vmap::point,
              ri)
    .def_rw("vertices", &Mcfs_vmap::vertices,
            ri)
    ;

  // TODO: this is an unspecified_type
  py::class_<Meso_skeleton> meso_skeleton(skeletonization, "Meso_skeleton");

  py::class_<boost::list_edge<unsigned long, boost::no_property>>
      boost_list_edge(skeletonization, "Boost_list_edge");
  boost_list_edge
    .def(py::init<>())
    .def(py::init<const boost::list_edge<unsigned long, boost::no_property>&>())
    .def_ro("m_source", &boost::list_edge<unsigned long, boost::no_property>::m_source)
    .def_ro("m_target", &boost::list_edge<unsigned long, boost::no_property>::m_target)
    ;

  py::class_<Skeleton::stored_vertex> stored_vertex(skeletonization, "Stored_vertex");
  stored_vertex
    .def(py::init<>())
    .def_ro("m_point", &Skeleton::stored_vertex::m_property)
    // .def_ro("m_vertices", &Skeleton::stored_vertex::m_out_edges)
    ;

  py::class_<Skeleton> skeleton(skeletonization, "Skeleton");
  skeleton
    .def(py::init<>(),
         "Default constructor. Creates an empty graph object with zero vertices and zero edges.\n")
    .def(py::init<const Skeleton&>(),
         py::arg("x"),
         "Copy constructor. Creates a new graph that is a copy of graph x, including the edges, vertices, and properties.\n")
    .def("__getitem__", [](Skeleton& skeleton, const Skeleton::vertex_descriptor& v) { return skeleton[v]; },
         py::arg("v"),
         "Returns the vertex descriptor of the vertex with index v.\n",
         ri)
    .def("vertex_set", [](const Skeleton& skeleton) {
         return py::make_iterator(py::type<typename Skeleton::vertex_iterator>(),
                                  "Iterator",
                                  skeleton.vertex_set().begin(), skeleton.vertex_set().end());
         },
         "Return an iterator of vertices of the skeleton.")
    .def_ro("m_vertices",&Skeleton::m_vertices,
            "Return an iterator of vertices of the skeleton.")
    .def_ro("m_edges",&Skeleton::m_edges,
            "Return an iterator of edges of the skeleton.")
    ;
  // m.def("vertices", [](const Skeleton& skeleton) {
  //       auto pair = vertices(skeleton);
        // why is this ambiguous?
        // return py::make_iterator(py::type<typename Skeleton::vertex_iterator>(),
        //                          "Iterator",
        //                          pair.first, pair.second, py::keep_alive<0, 1>());
      // }, "Return an iterator of vertices of the skeleton.");
  m.def("num_vertices", [](const Skeleton& skeleton) { return boost::num_vertices(skeleton); },
        py::arg("skeleton"),
               "Returns the number of vertices in the skeleton.");
  m.def("num_edges", [](const Skeleton& skeleton) { return boost::num_edges(skeleton); },
        py::arg("skeleton"),
               "Returns the number of edges in the skeleton.");
  m.def("source", [](const boost::list_edge<unsigned long, boost::no_property>& e, const Skeleton& skeleton) { return source(e, skeleton); },
        py::arg("edge"), py::arg("skeleton"),
               "Returns the source vertex of the edge e.");
  m.def("target", [](const boost::list_edge<unsigned long, boost::no_property>& e, const Skeleton& skeleton) { return target(e, skeleton); },
        py::arg("edge"), py::arg("skeleton"),
               "Returns the target vertex of the edge e.");

  using Visitor = boost_utils::Polyline_visitor<Skeleton>;
  py::class_<Visitor> pv(m, "Polyline_visitor");
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

  m.def("split_graph_into_polylines", [](const Skeleton& g, Visitor& pv) {
    return CGAL::split_graph_into_polylines(g, pv);
  }, py::arg("g"), py::arg("pv"),
        "splits into polylines the graph g at vertices of degree greater than 2 and at vertices for which is_terminal(v,graph)==true.\n"
        "The polylines are reported using a visitor.\n"
        "An overload without is_terminal is provided if no vertices but those of degree different from 2 are polyline endpoints.\n"
        "Examples\n"
        "• Surface_mesh_skeletonization/simple_mcfskel_example.py.");
  m.def("split_graph_into_polylines", [](const Skeleton& g, Visitor& pv, const std::function<bool(const Skeleton_vertex&, const Skeleton&)>& is_terminal) {
    struct IsTerminal {
      IsTerminal(const std::function<bool(const Skeleton_vertex&, const Skeleton&)>& is_terminal) : is_terminal(is_terminal) {}
      bool operator()(const Skeleton_vertex& v, const Skeleton& g) const { return is_terminal(v, g); }
      std::function<bool(const Skeleton_vertex&, const Skeleton&)> is_terminal;
    };
    return CGAL::split_graph_into_polylines(g, pv, IsTerminal(is_terminal));
  }, py::arg("g"), py::arg("pv"), py::arg("is_terminal"),
        "splits into polylines the graph g at vertices of degree greater than 2 and at vertices for which is_terminal(v,graph)==true.\n"
        "The polylines are reported using a visitor.\n"
        "An overload without is_terminal is provided if no vertices but those of degree different from 2 are polyline endpoints.\n"
        "Examples\n"
        "• Surface_mesh_skeletonization/simple_mcfskel_example.py.");

  m.def("extract_mean_curvature_flow_skeleton", &CGAL::extract_mean_curvature_flow_skeleton<Tm>,
        py::arg("tmesh"), py::arg("skeleton"),
        "extracts a medially centered curve skeleton for the triangle mesh tmesh. This function uses the class CGAL::Mean_curvature_flow_skeletonization with the default parameters. This function is provided only if Eigen 3.2 (or greater) is available and CGAL_EIGEN3_ENABLED is defined.\n"
        "\n"
        "Precondition\n"
        "• tmesh is a triangle mesh without borders and having exactly one connected component. \n"
        "• The specialization boost::property_map<TriangleMesh, CGAL::vertex_point_t>::type and get(vertex_point, tmesh) are defined. \n"
        "• The value type of boost::property_map<TriangleMesh, CGAL::vertex_point_t>::type is a point type from a CGAL Kernel.\n"
        "\n"
        "Parameters\n"
        "• tmesh:	input mesh\n"
        "• skeleton:	graph that will contain the skeleton of tmesh. For each vertex descriptor vd of skeleton, the corresponding point and the set of input vertices that contracted to vd can be retrieved using skeleton[vd].point and skeleton[vd].vertices respectively.\n"
        "\n"
        "Examples\n"
        "    Surface_mesh_skeletonization/segmentation_example.py, Surface_mesh_skeletonization/simple_mcfskel_example.py, and Surface_mesh_skeletonization/simple_mcfskel_sm_example.py.\n");

}
