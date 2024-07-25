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

  Corefine_visitor<PolygonalMesh>() :
    before_subface_creations_fn(),
    after_subface_creations_fn(),
    before_subface_created_fn(),
    after_subface_created_fn(),
    before_face_copy_fn(),
    after_face_copy_fn(),
    before_edge_split_fn(),
    edge_split_fn(),
    after_edge_split_fn(),
    add_retriangulation_edge_fn(),
    before_edge_copy_fn(),
    after_edge_copy_fn(),
    before_edge_duplicated_fn(),
    after_edge_duplicated_fn(),
    intersection_edge_copy_fn(),
    new_vertex_added_fn(),
    intersection_point_detected_fn(),
    before_vertex_copy_fn(),
    after_vertex_copy_fn(),
    start_filtering_intersections_fn(),
    progress_filtering_intersections_fn(),
    end_filtering_intersections_fn(),
    start_triangulating_faces_fn(),
    triangulating_faces_step_fn(),
    end_triangulating_faces_fn(),
    start_handling_intersection_of_coplanar_faces_fn(),
    intersection_of_coplanar_faces_step_fn(),
    end_handling_intersection_of_coplanar_faces_fn(),
    start_handling_edge_face_intersections_fn(),
    edge_face_intersections_step_fn(),
    end_handling_edge_face_intersections_fn(),
    start_building_output_fn(),
    end_building_output_fn(),
    filter_coplanar_edges_fn(),
    detect_patches_fn(),
    classify_patches_fn(),
    classify_intersection_free_patches_fn(),
    out_of_place_operation_fn(),
    in_place_operation_fn(),
    in_place_operations_fn()
  {}

  inline void before_subface_creations(Fd f_old, Pm& tm) const {before_subface_creations_fn(f_old, tm);}
  inline void after_subface_creations(Pm& tm) const {after_subface_creations_fn(tm);}
  inline void before_subface_created(Pm& tm) const {before_subface_created_fn(tm);}
  inline void after_subface_created(Fd f_new, Pm& tm) const {after_subface_created_fn(f_new, tm);}
  inline void before_face_copy(Fd f_old, const Pm& tm_old, Pm& tm_new) const {before_face_copy_fn(f_old, tm_old, tm_new);}
  inline void after_face_copy(Fd f_old, const Pm& tm_old, Fd f_new, Pm& tm_new) const {after_face_copy_fn(f_old, tm_old, f_new, tm_new);}
  inline void before_edge_split(Hd h, Pm& tm) const {before_edge_split_fn(h, tm);}
  inline void edge_split(Hd hnew, Pm& tm) const {edge_split_fn(hnew, tm);}
  inline void after_edge_split() const {after_edge_split_fn();}
  inline void add_retriangulation_edge(Hd h, Pm& tm) const {add_retriangulation_edge_fn(h, tm);}
  inline void before_edge_copy(Hd h_old, const Pm& tm_old, Pm& tm_new) const {before_edge_copy_fn(h_old, tm_old, tm_new);}
  inline void after_edge_copy(Hd h_old, const Pm& tm_old, Hd h_new, Pm& tm_new) const {after_edge_copy_fn(h_old, tm_old, h_new, tm_new);}
  inline void before_edge_duplicated(Hd h_old, Pm& tm) const {before_edge_duplicated_fn(h_old, tm);}
  inline void after_edge_duplicated(Hd h_old, Hd h_new, Pm& tm) const {after_edge_duplicated_fn(h_old, h_new, tm);}
  inline void intersection_edge_copy(Hd h_old1, const Pm& tm1, Hd h_old2, const Pm& tm2, Hd h_new, Pm& tm_new) const {intersection_edge_copy_fn(h_old1, tm1, h_old2, tm2, h_new, tm_new);}
  inline void new_vertex_added(std::size_t node_id, Vd vh, const Pm& tm) const {new_vertex_added_fn(node_id, vh, tm);}
  inline void intersection_point_detected(std::size_t node_id, int sdim, Hd principal_edge, Hd additional_edge, const Pm& tm1, const Pm& tm2, bool is_target_coplanar, bool is_source_coplanar) const {intersection_point_detected_fn(node_id, sdim, principal_edge, additional_edge, tm1, tm2, is_target_coplanar, is_source_coplanar);}
  inline void before_vertex_copy(Vd v_src, const Pm& tm_src, Pm& tm_tgt) const {before_vertex_copy_fn(v_src, tm_src, tm_tgt);}
  inline void after_vertex_copy(Vd v_src, const Pm& tm_src, Vd v_tgt, Pm& tm_tgt) const {after_vertex_copy_fn(v_src, tm_src, v_tgt, tm_tgt);}
  inline void start_filtering_intersections() const {start_filtering_intersections_fn();}
  inline void progress_filtering_intersections(double d) const {progress_filtering_intersections_fn(d);}
  inline void end_filtering_intersections() const {end_filtering_intersections_fn();}
  inline void start_triangulating_faces(std::size_t n) const {start_triangulating_faces_fn(n);}
  inline void triangulating_faces_step() const {triangulating_faces_step_fn();}
  inline void end_triangulating_faces() const {end_triangulating_faces_fn();}
  inline void start_handling_intersection_of_coplanar_faces(std::size_t n) const {start_handling_intersection_of_coplanar_faces_fn(n);}
  inline void intersection_of_coplanar_faces_step() const {intersection_of_coplanar_faces_step_fn();}
  inline void end_handling_intersection_of_coplanar_faces() const {end_handling_intersection_of_coplanar_faces_fn();}
  inline void start_handling_edge_face_intersections(std::size_t n) const {start_handling_edge_face_intersections_fn(n);}
  inline void edge_face_intersections_step() const {edge_face_intersections_step_fn();}
  inline void end_handling_edge_face_intersections() const {end_handling_edge_face_intersections_fn();}
  inline void start_building_output() const {start_building_output_fn();}
  inline void end_building_output() const {end_building_output_fn();}
  inline void filter_coplanar_edges() const {filter_coplanar_edges_fn();}
  inline void detect_patches() const {detect_patches_fn();}
  inline void classify_patches() const {classify_patches_fn();}
  inline void classify_intersection_free_patches(const Pm& tm) const {classify_intersection_free_patches_fn(tm);}
  inline void out_of_place_operation(Boolean_operation_type t) const {out_of_place_operation_fn(t);}
  inline void in_place_operation(Boolean_operation_type t) const {in_place_operation_fn(t);}
  inline void in_place_operations(Boolean_operation_type t1, Boolean_operation_type t2) const {in_place_operations_fn(t1, t2);}


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
