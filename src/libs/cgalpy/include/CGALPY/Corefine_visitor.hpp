// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_COREFINE_VISITOR_HPP
#define CGALPY_COREFINE_VISITOR_HPP

#include <CGAL/Polygon_mesh_processing/internal/Corefinement/face_graph_utils.h>
#include <boost/graph/graph_traits.hpp>
#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace COREFINEMENT = PMP::Corefinement;

namespace pmp {

//
template <typename PolygonalMesh>
struct Corefine_visitor : public COREFINEMENT::Default_visitor<PolygonalMesh> {
public:
  using Pm = PolygonalMesh;
  using Gt = boost::graph_traits<PolygonalMesh>;
  using Vd = typename Gt::vertex_descriptor;
  using Hd = typename Gt::halfedge_descriptor;
  using Fd = typename Gt::face_descriptor;

  // Construct default
  Corefine_visitor() :
    m_before_subface_creations(py::none()),
    m_after_subface_creations(py::none()),
    m_before_subface_created(py::none()),
    m_after_subface_created(py::none()),
    m_before_face_copy(py::none()),
    m_after_face_copy(py::none()),
    m_before_edge_split(py::none()),
    m_edge_split(py::none()),
    m_after_edge_split(py::none()),
    m_add_retriangulation_edge(py::none()),
    m_before_edge_copy(py::none()),
    m_after_edge_copy(py::none()),
    m_before_edge_duplicated(py::none()),
    m_after_edge_duplicated(py::none()),
    m_intersection_edge_copy(py::none()),
    m_new_vertex_added(py::none()),
    m_intersection_point_detected(py::none()),
    m_before_vertex_copy(py::none()),
    m_after_vertex_copy(py::none()),
    m_start_filtering_intersections(py::none()),
    m_progress_filtering_intersections(py::none()),
    m_end_filtering_intersections(py::none()),
    m_start_triangulating_faces(py::none()),
    m_triangulating_faces_step(py::none()),
    m_end_triangulating_faces(py::none()),
    m_start_handling_intersection_of_coplanar_faces(py::none()),
    m_intersection_of_coplanar_faces_step(py::none()),
    m_end_handling_intersection_of_coplanar_faces(py::none()),
    m_start_handling_edge_face_intersections(py::none()),
    m_edge_face_intersections_step(py::none()),
    m_end_handling_edge_face_intersections(py::none()),
    m_start_building_output(py::none()),
    m_end_building_output(py::none()),
    m_filter_coplanar_edges(py::none()),
    m_detect_patches(py::none()),
    m_classify_patches(py::none()),
    m_classify_intersection_free_patches(py::none()),
    m_out_of_place_operation(py::none()),
    m_in_place_operation(py::none()),
    m_in_place_operations(py::none())
  {}

  /// Setters
  /// @{

  void set_before_subface_creations(py::object fnc) { m_before_subface_creations = fnc; }
  void set_after_subface_creations(py::object fnc) { m_after_subface_creations = fnc; }
  void set_before_subface_created(py::object fnc) { m_before_subface_created = fnc; }
  void set_after_subface_created(py::object fnc) { m_after_subface_created = fnc; }
  void set_before_face_copy(py::object fnc) { m_before_face_copy = fnc; }
  void set_after_face_copy(py::object fnc) { m_after_face_copy = fnc; }
  void set_before_edge_split(py::object fnc) { m_before_edge_split = fnc; }
  void set_edge_split(py::object fnc) { m_edge_split = fnc; }
  void set_after_edge_split(py::object fnc) { m_after_edge_split = fnc; }
  void set_add_retriangulation_edge(py::object fnc) { m_add_retriangulation_edge = fnc; }
  void set_before_edge_copy(py::object fnc) { m_before_edge_copy = fnc; }
  void set_after_edge_copy(py::object fnc) { m_after_edge_copy = fnc; }
  void set_before_edge_duplicated(py::object fnc) { m_before_edge_duplicated = fnc; }
  void set_after_edge_duplicated(py::object fnc) { m_after_edge_duplicated = fnc; }
  void set_intersection_edge_copy(py::object fnc) { m_intersection_edge_copy = fnc; }
  void set_new_vertex_added(py::object fnc) { m_new_vertex_added = fnc; }
  void set_intersection_point_detected(py::object fnc) { m_intersection_point_detected = fnc; }
  void set_before_vertex_copy(py::object fnc) { m_before_vertex_copy = fnc; }
  void set_after_vertex_copy(py::object fnc) { m_after_vertex_copy = fnc; }
  void set_start_filtering_intersections(py::object fnc) { m_start_filtering_intersections = fnc; }
  void set_progress_filtering_intersections(py::object fnc) { m_progress_filtering_intersections = fnc; }
  void set_end_filtering_intersections(py::object fnc) { m_end_filtering_intersections = fnc; }
  void set_start_triangulating_faces(py::object fnc) { m_start_triangulating_faces = fnc; }
  void set_triangulating_faces_step(py::object fnc) { m_triangulating_faces_step = fnc; }
  void set_end_triangulating_faces(py::object fnc) { m_end_triangulating_faces = fnc; }
  void set_start_handling_intersection_of_coplanar_faces(py::object fnc)
  { m_start_handling_intersection_of_coplanar_faces = fnc; }
  void set_intersection_of_coplanar_faces_step(py::object fnc)
  { m_intersection_of_coplanar_faces_step = fnc; }
  void set_end_handling_intersection_of_coplanar_faces(py::object fnc)
  { m_end_handling_intersection_of_coplanar_faces = fnc; }
  void set_start_handling_edge_face_intersections(py::object fnc)
  { m_start_handling_edge_face_intersections = fnc; }
  void set_edge_face_intersections_step(py::object fnc) { m_edge_face_intersections_step = fnc; }
  void set_end_handling_edge_face_intersections(py::object fnc)
  { m_end_handling_edge_face_intersections = fnc; }
  void set_start_building_output(py::object fnc) { m_start_building_output = fnc; }
  void set_end_building_output(py::object fnc) { m_end_building_output = fnc; }
  void set_filter_coplanar_edges(py::object fnc) { m_filter_coplanar_edges = fnc; }
  void set_detect_patches(py::object fnc) { m_detect_patches = fnc; }
  void set_classify_patches(py::object fnc) { m_classify_patches = fnc; }
  void set_classify_intersection_free_patches(py::object fnc)
  { m_classify_intersection_free_patches = fnc; }
  void set_out_of_place_operation(py::object fnc) { m_out_of_place_operation = fnc; }
  void set_in_place_operation(py::object fnc) { m_in_place_operation = fnc; }
  void set_in_place_operations(py::object fnc) { m_in_place_operations = fnc; }

  /// @}

  /// \Name face notifications
  /// @{

  void before_subface_creations(Fd f_old, Pm& pm)
  { if (! m_before_subface_creations.is_none()) m_before_subface_creations(f_old, pm); }

  void after_subface_creations(Pm& pm)
  { if (! m_after_subface_creations.is_none()) m_after_subface_creations(pm); }

  void before_subface_created(Pm& pm)
  { if (! m_before_subface_created.is_none()) m_before_subface_created(pm); }

  void after_subface_created(Fd f_new, Pm& pm)
  { if (! m_after_subface_created.is_none()) m_after_subface_created(f_new, pm); }

  void before_face_copy(Fd f_old, const Pm& pm1, Pm& pm2)
  { if (! m_before_face_copy.is_none()) m_before_face_copy(f_old, pm1, pm2); }

  void after_face_copy(Fd f_old, const Pm& pm1, Fd f_new, Pm& pm2)
  { if (! m_after_face_copy.is_none()) m_after_face_copy(f_old, pm1, f_new, pm2); }

  /// @}

  /// \Name edge notifications
  /// @{

  void before_edge_split(Hd h, Pm& pm)
  { if (! m_before_edge_split.is_none()) m_before_edge_split(h, pm); }

  void edge_split(Hd hnew, Pm& pm)
  { if (! m_edge_split.is_none()) m_edge_split(hnew, pm); }

  void after_edge_split()
  { if (! m_after_edge_split.is_none()) m_after_edge_split(); }

   // edges added during split face retriangulation
  void add_retriangulation_edge(Hd h, Pm& pm)
  { if (! m_add_retriangulation_edge.is_none()) m_add_retriangulation_edge(h, pm); }

  void before_edge_copy(Hd h_old, const Pm& pm1, Pm& pm2)
  { if (! m_before_edge_copy.is_none()) m_before_edge_copy(h_old, pm1, pm2); }

  void after_edge_copy(Hd h_old, const Pm& pm1, Hd f_new, Pm& pm2)
  { if (! m_after_edge_copy.is_none()) m_after_edge_copy(h_old, pm1, f_new, pm2); }

  // called before a patch border edge is duplicated
  void before_edge_duplicated(Hd h_old, Pm& pm)
  { if (! m_before_edge_duplicated.is_none()) m_before_edge_duplicated(h_old, pm); }

  // called after a patch border edge is duplicated
  void after_edge_duplicated(Hd h_old, Hd f_new, Pm& pm)
  { if (! m_after_edge_duplicated.is_none()) m_after_edge_duplicated(h_old, f_new, pm); }

  void intersection_edge_copy(Hd h_old1, const Pm& pm1, Hd h_old2, const Pm& pm2, Hd h_new, Pm& pm_new) {
    if (! m_intersection_edge_copy.is_none())
      m_intersection_edge_copy(h_old1, pm1, h_old2, pm2, h_new, pm_new);
  }

  /// @}

  /// \Name vertex notifications
  /// @{

  void new_vertex_added(std::size_t node_id, Vd vh, const Pm& pm)
  { if (! m_new_vertex_added.is_none()) m_new_vertex_added(node_id, vh, pm); }

  void intersection_point_detected(std::size_t node_id, int sdim, Hd principal_edge, Hd additional_edge,
                                   const Pm& pm1, const Pm& pm2,
                                   bool is_target_coplanar, bool is_source_coplanar) {
    if (! m_intersection_point_detected.is_none())
      m_intersection_point_detected(node_id, sdim, principal_edge, additional_edge, pm1, pm2,
                                    is_target_coplanar, is_source_coplanar);
  }

  void before_vertex_copy(Vd v_src, const Pm& pm_src, Pm& pm_tgt)
  { if (! m_before_vertex_copy.is_none()) m_before_vertex_copy(v_src, pm_src, pm_tgt); }

  void after_vertex_copy(Vd v_src, const Pm& pm_src, Vd v_tgt, Pm& pm_tgt)
  { if (! m_after_vertex_copy.is_none()) m_after_vertex_copy(v_src, pm_src, v_tgt, pm_tgt); }

  /// @}

  /// \Name progress tracking
  /// @{

  void start_filtering_intersections() const
  { if (! m_start_filtering_intersections.is_none()) m_start_filtering_intersections(); }

  void progress_filtering_intersections(double d) const
  { if (! m_progress_filtering_intersections.is_none()) m_progress_filtering_intersections(d); }

  void end_filtering_intersections() const
  { if (! m_end_filtering_intersections.is_none()) m_end_filtering_intersections(); }


  void start_triangulating_faces(std::size_t n) const
  { if (! m_start_triangulating_faces.is_none()) m_start_triangulating_faces(n); }

  void triangulating_faces_step() const
  { if (! m_triangulating_faces_step.is_none()) m_triangulating_faces_step(); }

  void end_triangulating_faces() const
  { if (! m_end_triangulating_faces.is_none()) m_end_triangulating_faces(); }

  void start_handling_intersection_of_coplanar_faces(std::size_t n) const {
    if (! m_start_handling_intersection_of_coplanar_faces.is_none())
      m_start_handling_intersection_of_coplanar_faces(n);
  }

  void intersection_of_coplanar_faces_step() const
  { if (! m_intersection_of_coplanar_faces_step.is_none()) m_intersection_of_coplanar_faces_step(); }

  void end_handling_intersection_of_coplanar_faces() const {
    if (! m_end_handling_intersection_of_coplanar_faces.is_none())
      m_end_handling_intersection_of_coplanar_faces();
  }

  void start_handling_edge_face_intersections(std::size_t n) const {
    if (! m_start_handling_edge_face_intersections.is_none())
      m_start_handling_edge_face_intersections(n);
  }

  void edge_face_intersections_step() const
  { if (! m_edge_face_intersections_step.is_none()) m_edge_face_intersections_step(); }

  void end_handling_edge_face_intersections() const
  { if (! m_end_handling_edge_face_intersections.is_none()) m_end_handling_edge_face_intersections(); }

  void start_building_output() const
  { if (! m_start_building_output.is_none()) m_start_building_output(); }

  void end_building_output() const
  { if (! m_end_building_output.is_none()) m_end_building_output(); }

  /// @}

  /// \Name Required by Face_graph_output_builder
  /// @{

  void filter_coplanar_edges() const
  { if (! m_filter_coplanar_edges.is_none()) m_filter_coplanar_edges(); }

  void detect_patches() const
  { if (! m_detect_patches.is_none()) m_detect_patches(); }

  void classify_patches() const
  { if (! m_classify_patches.is_none()) m_classify_patches(); }

  void classify_intersection_free_patches(const Pm& pm) const
  { if (! m_classify_intersection_free_patches.is_none()) m_classify_intersection_free_patches(pm); }

  void out_of_place_operation(COREFINEMENT::Boolean_operation_type t) const
  { if (! m_out_of_place_operation.is_none()) m_out_of_place_operation(t); }

  void in_place_operation(COREFINEMENT::Boolean_operation_type t) const
  { if (! m_in_place_operation.is_none()) m_in_place_operation(t); }

  void in_place_operations(COREFINEMENT::Boolean_operation_type t1,
                           COREFINEMENT::Boolean_operation_type t2) const
  { if (! m_in_place_operations.is_none()) m_in_place_operations(t1, t2); }

  /// @}

private:
  // face visitor functions
  py::object m_before_subface_creations;
  py::object m_after_subface_creations;
  py::object m_before_subface_created;
  py::object m_after_subface_created;
  py::object m_before_face_copy;
  py::object m_after_face_copy;

  // edge visitor functions
  py::object m_before_edge_split;
  py::object m_edge_split;
  py::object m_after_edge_split;

  py::object m_add_retriangulation_edge;

  py::object m_before_edge_copy;
  py::object m_after_edge_copy;

  py::object m_before_edge_duplicated;
  py::object m_after_edge_duplicated;

  py::object m_intersection_edge_copy;

  // vertex visitor functions
  py::object m_new_vertex_added;

  py::object m_intersection_point_detected;
  py::object m_before_vertex_copy;
  py::object m_after_vertex_copy;

  // progress tracking
  py::object m_start_filtering_intersections;
  py::object m_progress_filtering_intersections;
  py::object m_end_filtering_intersections;

  py::object m_start_triangulating_faces;
  py::object m_triangulating_faces_step;
  py::object m_end_triangulating_faces;

  py::object m_start_handling_intersection_of_coplanar_faces;
  py::object m_intersection_of_coplanar_faces_step;
  py::object m_end_handling_intersection_of_coplanar_faces;

  py::object m_start_handling_edge_face_intersections;
  py::object m_edge_face_intersections_step;
  py::object m_end_handling_edge_face_intersections;

  py::object m_start_building_output;
  py::object m_end_building_output;

  // Required by Face_graph_output_builder
  py::object m_filter_coplanar_edges;
  py::object m_detect_patches;
  py::object m_classify_patches;
  py::object m_classify_intersection_free_patches;
  py::object m_out_of_place_operation;
  py::object m_in_place_operation;
  py::object m_in_place_operations;
};

}

#endif
