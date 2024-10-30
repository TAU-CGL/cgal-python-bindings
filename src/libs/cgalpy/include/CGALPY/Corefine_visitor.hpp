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
  // using Boolean_operation_type = COREFINEMENT::Boolean_operation_type;
  using Boolean_operation_type = std::size_t;

  Corefine_visitor<PolygonalMesh>() {
    before_subface_creations_fn = nullptr;
    after_subface_creations_fn = nullptr;
    before_subface_created_fn = nullptr;
    after_subface_created_fn = nullptr;
    before_face_copy_fn = nullptr;
    after_face_copy_fn = nullptr;
    before_edge_split_fn = nullptr;
    edge_split_fn = nullptr;
    after_edge_split_fn = nullptr;
    add_retriangulation_edge_fn = nullptr;
    before_edge_copy_fn = nullptr;
    after_edge_copy_fn = nullptr;
    before_edge_duplicated_fn = nullptr;
    after_edge_duplicated_fn = nullptr;
    intersection_edge_copy_fn = nullptr;
    new_vertex_added_fn = nullptr;
    intersection_point_detected_fn = nullptr;
    before_vertex_copy_fn = nullptr;
    after_vertex_copy_fn = nullptr;
    start_filtering_intersections_fn = nullptr;
    progress_filtering_intersections_fn = nullptr;
    end_filtering_intersections_fn = nullptr;
    start_triangulating_faces_fn = nullptr;
    triangulating_faces_step_fn = nullptr;
    end_triangulating_faces_fn = nullptr;
    start_handling_intersection_of_coplanar_faces_fn = nullptr;
    intersection_of_coplanar_faces_step_fn = nullptr;
    end_handling_intersection_of_coplanar_faces_fn = nullptr;
    start_handling_edge_face_intersections_fn = nullptr;
    edge_face_intersections_step_fn = nullptr;
    end_handling_edge_face_intersections_fn = nullptr;
    start_building_output_fn = nullptr;
    end_building_output_fn = nullptr;
    filter_coplanar_edges_fn = nullptr;
    detect_patches_fn = nullptr;
    classify_patches_fn = nullptr;
    classify_intersection_free_patches_fn = nullptr;
    out_of_place_operation_fn = nullptr;
    in_place_operation_fn = nullptr;
    in_place_operations_fn = nullptr;
  }

  void before_subface_creations(Fd f_old, Pm& tm) const {
    if (before_subface_creations_fn != nullptr) before_subface_creations_fn(f_old, tm);
  }

  void after_subface_creations(Pm& tm) const {
      if (after_subface_creations_fn != nullptr) after_subface_creations_fn(tm);
  }

  void before_subface_created(Pm& tm) const {
      if (before_subface_created_fn != nullptr) before_subface_created_fn(tm);
  }

  void after_subface_created(Fd f_new, Pm& tm) const {
      if (after_subface_created_fn != nullptr) after_subface_created_fn(f_new, tm);
  }

  void before_face_copy(Fd f_old, const Pm& tm_old, Pm& tm_new) const {
      if (before_face_copy_fn != nullptr) before_face_copy_fn(f_old, tm_old, tm_new);
  }

  void after_face_copy(Fd f_old, const Pm& tm_old, Fd f_new, Pm& tm_new) const {
      if (after_face_copy_fn != nullptr) after_face_copy_fn(f_old, tm_old, f_new, tm_new);
  }

  void before_edge_split(Hd h, Pm& tm) const {
      if (before_edge_split_fn != nullptr) before_edge_split_fn(h, tm);
  }

  void edge_split(Hd hnew, Pm& tm) const {
      if (edge_split_fn != nullptr) edge_split_fn(hnew, tm);
  }

  void after_edge_split() const {
      if (after_edge_split_fn != nullptr) after_edge_split_fn();
  }

  void add_retriangulation_edge(Hd h, Pm& tm) const {
      if (add_retriangulation_edge_fn != nullptr) add_retriangulation_edge_fn(h, tm);
  }

  void before_edge_copy(Hd h_old, const Pm& tm_old, Pm& tm_new) const {
      if (before_edge_copy_fn != nullptr) before_edge_copy_fn(h_old, tm_old, tm_new);
  }

  void after_edge_copy(Hd h_old, const Pm& tm_old, Hd h_new, Pm& tm_new) const {
      if (after_edge_copy_fn != nullptr) after_edge_copy_fn(h_old, tm_old, h_new, tm_new);
  }

  void before_edge_duplicated(Hd h_old, Pm& tm) const {
      if (before_edge_duplicated_fn != nullptr) before_edge_duplicated_fn(h_old, tm);
  }

  void after_edge_duplicated(Hd h_old, Hd h_new, Pm& tm) const {
      if (after_edge_duplicated_fn != nullptr) after_edge_duplicated_fn(h_old, h_new, tm);
  }

  void intersection_edge_copy(Hd h_old1, const Pm& tm1, Hd h_old2, const Pm& tm2, Hd h_new, Pm& tm_new) const {
      if (intersection_edge_copy_fn != nullptr) intersection_edge_copy_fn(h_old1, tm1, h_old2, tm2, h_new, tm_new);
  }

  void new_vertex_added(std::size_t node_id, Vd vh, const Pm& tm) const {
      if (new_vertex_added_fn != nullptr) new_vertex_added_fn(node_id, vh, tm);
  }

  void intersection_point_detected(std::size_t node_id, int sdim, Hd principal_edge, Hd additional_edge, const Pm& tm1, const Pm& tm2, bool is_target_coplanar, bool is_source_coplanar) const {
      if (intersection_point_detected_fn != nullptr) intersection_point_detected_fn(node_id, sdim, principal_edge, additional_edge, tm1, tm2, is_target_coplanar, is_source_coplanar);
  }

  void before_vertex_copy(Vd v_src, const Pm& tm_src, Pm& tm_tgt) const {
      if (before_vertex_copy_fn != nullptr) before_vertex_copy_fn(v_src, tm_src, tm_tgt);
  }

  void after_vertex_copy(Vd v_src, const Pm& tm_src, Vd v_tgt, Pm& tm_tgt) const {
      if (after_vertex_copy_fn != nullptr) after_vertex_copy_fn(v_src, tm_src, v_tgt, tm_tgt);
  }

  void start_filtering_intersections() const {
      if (start_filtering_intersections_fn != nullptr) start_filtering_intersections_fn();
  }

  void progress_filtering_intersections(double d) const {
      if (progress_filtering_intersections_fn != nullptr) progress_filtering_intersections_fn(d);
  }

  void end_filtering_intersections() const {
      if (end_filtering_intersections_fn != nullptr) end_filtering_intersections_fn();
  }

  void start_triangulating_faces(std::size_t n) const {
      if (start_triangulating_faces_fn != nullptr) start_triangulating_faces_fn(n);
  }

  void triangulating_faces_step() const {
      if (triangulating_faces_step_fn != nullptr) triangulating_faces_step_fn();
  }

  void end_triangulating_faces() const {
      if (end_triangulating_faces_fn != nullptr) end_triangulating_faces_fn();
  }

  void start_handling_intersection_of_coplanar_faces(std::size_t n) const {
      if (start_handling_intersection_of_coplanar_faces_fn != nullptr) start_handling_intersection_of_coplanar_faces_fn(n);
  }

  void intersection_of_coplanar_faces_step() const {
      if (intersection_of_coplanar_faces_step_fn != nullptr) intersection_of_coplanar_faces_step_fn();
  }

  void end_handling_intersection_of_coplanar_faces() const {
      if (end_handling_intersection_of_coplanar_faces_fn != nullptr) end_handling_intersection_of_coplanar_faces_fn();
  }

  void start_handling_edge_face_intersections(std::size_t n) const {
      if (start_handling_edge_face_intersections_fn != nullptr) start_handling_edge_face_intersections_fn(n);
  }

  void edge_face_intersections_step() const {
      if (edge_face_intersections_step_fn != nullptr) edge_face_intersections_step_fn();
  }

  void end_handling_edge_face_intersections() const {
      if (end_handling_edge_face_intersections_fn != nullptr) end_handling_edge_face_intersections_fn();
  }

  void start_building_output() const {
      if (start_building_output_fn != nullptr) start_building_output_fn();
  }

  void end_building_output() const {
      if (end_building_output_fn != nullptr) end_building_output_fn();
  }

  void filter_coplanar_edges() const {
      if (filter_coplanar_edges_fn != nullptr) filter_coplanar_edges_fn();
  }

  void detect_patches() const {
      if (detect_patches_fn != nullptr) detect_patches_fn();
  }

  void classify_patches() const {
      if (classify_patches_fn != nullptr) classify_patches_fn();
  }

  void classify_intersection_free_patches(const Pm& tm) const {
      if (classify_intersection_free_patches_fn != nullptr) classify_intersection_free_patches_fn(tm);
  }

  void out_of_place_operation(Boolean_operation_type t) const {
      if (out_of_place_operation_fn != nullptr) out_of_place_operation_fn(t);
  }

  void in_place_operation(Boolean_operation_type t) const {
      if (in_place_operation_fn != nullptr) in_place_operation_fn(t);
  }

  void in_place_operations(Boolean_operation_type t1, Boolean_operation_type t2) const {
      if (in_place_operations_fn != nullptr) in_place_operations_fn(t1, t2);
  }



  void set_before_subface_creations(const std::function<void(Fd, Pm&)>& fn) {before_subface_creations_fn = fn;}
  void set_after_subface_creations(const std::function<void(Pm&)>& fn) {after_subface_creations_fn = fn;}
  void set_before_subface_created(const std::function<void(Pm&)>& fn) {before_subface_created_fn = fn;}
  void set_after_subface_created(const std::function<void(Fd, Pm&)>& fn) {after_subface_created_fn = fn;}
  void set_before_face_copy(const std::function<void(Fd, const Pm&, Pm&)>& fn) {before_face_copy_fn = fn;}
  void set_after_face_copy(const std::function<void(Fd, const Pm&, Fd, Pm&)>& fn) {after_face_copy_fn = fn;}
  void set_before_edge_split(const std::function<void(Hd, Pm&)>& fn) {before_edge_split_fn = fn;}
  void set_edge_split(const std::function<void(Hd, Pm&)>& fn) {edge_split_fn = fn;}
  void set_after_edge_split(const std::function<void()>& fn) {after_edge_split_fn = fn;}
  void set_add_retriangulation_edge(const std::function<void(Hd, Pm&)>& fn) {add_retriangulation_edge_fn = fn;}
  void set_before_edge_copy(const std::function<void(Hd, const Pm&, Pm&)>& fn) {before_edge_copy_fn = fn;}
  void set_after_edge_copy(const std::function<void(Hd, const Pm&, Hd, Pm&)>& fn) {after_edge_copy_fn = fn;}
  void set_before_edge_duplicated(const std::function<void(Hd, Pm&)>& fn) {before_edge_duplicated_fn = fn;}
  void set_after_edge_duplicated(const std::function<void(Hd, Hd, Pm&)>& fn) {after_edge_duplicated_fn = fn;}
  void set_intersection_edge_copy(const std::function<void(Hd, const Pm&, Hd, const Pm&, Hd, Pm&)>& fn) {intersection_edge_copy_fn = fn;}
  void set_new_vertex_added(const std::function<void(std::size_t, Vd, const Pm&)>& fn) {new_vertex_added_fn = fn;}
  void set_intersection_point_detected(const std::function<void(std::size_t, int, Hd, Hd, const Pm&, const Pm&, bool, bool)>& fn) {intersection_point_detected_fn = fn;}
  void set_before_vertex_copy(const std::function<void(Vd, const Pm&, Pm&)>& fn) {before_vertex_copy_fn = fn;}
  void set_after_vertex_copy(const std::function<void(Vd, const Pm&, Vd, Pm&)>& fn) {after_vertex_copy_fn = fn;}
  void set_start_filtering_intersections(const std::function<void()>& fn) {start_filtering_intersections_fn = fn;}
  void set_progress_filtering_intersections(const std::function<void(double)>& fn) {progress_filtering_intersections_fn = fn;}
  void set_end_filtering_intersections(const std::function<void()>& fn) {end_filtering_intersections_fn = fn;}
  void set_start_triangulating_faces(const std::function<void(std::size_t)>& fn) {start_triangulating_faces_fn = fn;}
  void set_triangulating_faces_step(const std::function<void()>& fn) {triangulating_faces_step_fn = fn;}
  void set_end_triangulating_faces(const std::function<void()>& fn) {end_triangulating_faces_fn = fn;}
  void set_start_handling_intersection_of_coplanar_faces(const std::function<void(std::size_t)>& fn) {start_handling_intersection_of_coplanar_faces_fn = fn;}
  void set_intersection_of_coplanar_faces_step(const std::function<void()>& fn) {intersection_of_coplanar_faces_step_fn = fn;}
  void set_end_handling_intersection_of_coplanar_faces(const std::function<void()>& fn) {end_handling_intersection_of_coplanar_faces_fn = fn;}
  void set_start_handling_edge_face_intersections(const std::function<void(std::size_t)>& fn) {start_handling_edge_face_intersections_fn = fn;}
  void set_edge_face_intersections_step(const std::function<void()>& fn) {edge_face_intersections_step_fn = fn;}
  void set_end_handling_edge_face_intersections(const std::function<void()>& fn) {end_handling_edge_face_intersections_fn = fn;}
  void set_start_building_output(const std::function<void()>& fn) {start_building_output_fn = fn;}
  void set_end_building_output(const std::function<void()>& fn) {end_building_output_fn = fn;}
  void set_filter_coplanar_edges(const std::function<void()>& fn) {filter_coplanar_edges_fn = fn;}
  void set_detect_patches(const std::function<void()>& fn) {detect_patches_fn = fn;}
  void set_classify_patches(const std::function<void()>& fn) {classify_patches_fn = fn;}
  void set_classify_intersection_free_patches(const std::function<void(const Pm&)>& fn) {classify_intersection_free_patches_fn = fn;}
  void set_out_of_place_operation(const std::function<void(Boolean_operation_type)>& fn) {out_of_place_operation_fn = fn;}
  void set_in_place_operation(const std::function<void(Boolean_operation_type)>& fn) {in_place_operation_fn = fn;}
  void set_in_place_operations(const std::function<void(Boolean_operation_type, Boolean_operation_type)>& fn) {in_place_operations_fn = fn;}

private:
  std::function<void(Fd, Pm&)> before_subface_creations_fn;
  std::function<void(Pm&)> after_subface_creations_fn;
  std::function<void(Pm&)> before_subface_created_fn;
  std::function<void(Fd, Pm&)> after_subface_created_fn;
  std::function<void(Fd, const Pm&, Pm&)> before_face_copy_fn;
  std::function<void(Fd, const Pm&, Fd, Pm&)> after_face_copy_fn;
  std::function<void(Hd, Pm&)> before_edge_split_fn;
  std::function<void(Hd, Pm&)> edge_split_fn;
  std::function<void()> after_edge_split_fn;
  std::function<void(Hd, Pm&)> add_retriangulation_edge_fn;
  std::function<void(Hd, const Pm&, Pm&)> before_edge_copy_fn;
  std::function<void(Hd, const Pm&, Hd, Pm&)> after_edge_copy_fn;
  std::function<void(Hd, Pm&)> before_edge_duplicated_fn;
  std::function<void(Hd, Hd, Pm&)> after_edge_duplicated_fn;
  std::function<void(Hd, const Pm&, Hd, const Pm&, Hd, Pm&)> intersection_edge_copy_fn;
  std::function<void(std::size_t, Vd, const Pm&)> new_vertex_added_fn;
  std::function<void(std::size_t, int, Hd, Hd, const Pm&, const Pm&, bool, bool)> intersection_point_detected_fn;
  std::function<void(Vd, const Pm&, Pm&)> before_vertex_copy_fn;
  std::function<void(Vd, const Pm&, Vd, Pm&)> after_vertex_copy_fn;
  std::function<void()> start_filtering_intersections_fn;
  std::function<void(double)> progress_filtering_intersections_fn;
  std::function<void()> end_filtering_intersections_fn;
  std::function<void(std::size_t)> start_triangulating_faces_fn;
  std::function<void()> triangulating_faces_step_fn;
  std::function<void()> end_triangulating_faces_fn;
  std::function<void(std::size_t)> start_handling_intersection_of_coplanar_faces_fn;
  std::function<void()> intersection_of_coplanar_faces_step_fn;
  std::function<void()> end_handling_intersection_of_coplanar_faces_fn;
  std::function<void(std::size_t)> start_handling_edge_face_intersections_fn;
  std::function<void()> edge_face_intersections_step_fn;
  std::function<void()> end_handling_edge_face_intersections_fn;
  std::function<void()> start_building_output_fn;
  std::function<void()> end_building_output_fn;
  std::function<void()> filter_coplanar_edges_fn;
  std::function<void()> detect_patches_fn;
  std::function<void()> classify_patches_fn;
  std::function<void(const Pm&)> classify_intersection_free_patches_fn;
  std::function<void(Boolean_operation_type)> out_of_place_operation_fn;
  std::function<void(Boolean_operation_type)> in_place_operation_fn;
  std::function<void(Boolean_operation_type, Boolean_operation_type)> in_place_operations_fn;
};

}

#endif
