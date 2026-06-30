// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_COREFINE_VISITOR_HPP
#define CGALPY_COREFINE_VISITOR_HPP

#include <CGAL/Polygon_mesh_processing/internal/Corefinement/face_graph_utils.h>
#include <boost/graph/graph_traits.hpp>
#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace COREFINEMENT = PMP::Corefinement;

namespace cgalpy {
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
    before_subface_creations_fn_object = py::none();
    after_subface_creations_fn_object = py::none();
    before_subface_created_fn_object = py::none();
    after_subface_created_fn_object = py::none();
    before_face_copy_fn_object = py::none();
    after_face_copy_fn_object = py::none();
    before_edge_split_fn_object = py::none();
    edge_split_fn_object = py::none();
    after_edge_split_fn_object = py::none();
    add_retriangulation_edge_fn_object = py::none();
    before_edge_copy_fn_object = py::none();
    after_edge_copy_fn_object = py::none();
    before_edge_duplicated_fn_object = py::none();
    after_edge_duplicated_fn_object = py::none();
    intersection_edge_copy_fn_object = py::none();
    new_vertex_added_fn_object = py::none();
    intersection_point_detected_fn_object = py::none();
    before_vertex_copy_fn_object = py::none();
    after_vertex_copy_fn_object = py::none();
    start_filtering_intersections_fn_object = py::none();
    progress_filtering_intersections_fn_object = py::none();
    end_filtering_intersections_fn_object = py::none();
    start_triangulating_faces_fn_object = py::none();
    triangulating_faces_step_fn_object = py::none();
    end_triangulating_faces_fn_object = py::none();
    start_handling_intersection_of_coplanar_faces_fn_object = py::none();
    intersection_of_coplanar_faces_step_fn_object = py::none();
    end_handling_intersection_of_coplanar_faces_fn_object = py::none();
    start_handling_edge_face_intersections_fn_object = py::none();
    edge_face_intersections_step_fn_object = py::none();
    end_handling_edge_face_intersections_fn_object = py::none();
    start_building_output_fn_object = py::none();
    end_building_output_fn_object = py::none();
    filter_coplanar_edges_fn_object = py::none();
    detect_patches_fn_object = py::none();
    classify_patches_fn_object = py::none();
    classify_intersection_free_patches_fn_object = py::none();
    out_of_place_operation_fn_object = py::none();
    in_place_operation_fn_object = py::none();
    in_place_operations_fn_object = py::none();
  }

  void before_subface_creations(Fd f_old, Pm& tm) const {
    if (! before_subface_creations_fn_object.is_none()) before_subface_creations_fn_object(f_old, tm);
  }

  void after_subface_creations(Pm& tm) const {
      if (! after_subface_creations_fn_object.is_none()) after_subface_creations_fn_object(tm);
  }

  void before_subface_created(Pm& tm) const {
      if (! before_subface_created_fn_object.is_none()) before_subface_created_fn_object(tm);
  }

  void after_subface_created(Fd f_new, Pm& tm) const {
      if (! after_subface_created_fn_object.is_none()) after_subface_created_fn_object(f_new, tm);
  }

  void before_face_copy(Fd f_old, const Pm& tm_old, Pm& tm_new) const {
      if (! before_face_copy_fn_object.is_none()) before_face_copy_fn_object(f_old, tm_old, tm_new);
  }

  void after_face_copy(Fd f_old, const Pm& tm_old, Fd f_new, Pm& tm_new) const {
      if (! after_face_copy_fn_object.is_none()) after_face_copy_fn_object(f_old, tm_old, f_new, tm_new);
  }

  void before_edge_split(Hd h, Pm& tm) const {
      if (! before_edge_split_fn_object.is_none()) before_edge_split_fn_object(h, tm);
  }

  void edge_split(Hd hnew, Pm& tm) const {
      if (! edge_split_fn_object.is_none()) edge_split_fn_object(hnew, tm);
  }

  void after_edge_split() const {
      if (! after_edge_split_fn_object.is_none()) after_edge_split_fn_object();
  }

  void add_retriangulation_edge(Hd h, Pm& tm) const {
      if (! add_retriangulation_edge_fn_object.is_none()) add_retriangulation_edge_fn_object(h, tm);
  }

  void before_edge_copy(Hd h_old, const Pm& tm_old, Pm& tm_new) const {
      if (! before_edge_copy_fn_object.is_none()) before_edge_copy_fn_object(h_old, tm_old, tm_new);
  }

  void after_edge_copy(Hd h_old, const Pm& tm_old, Hd h_new, Pm& tm_new) const {
      if (! after_edge_copy_fn_object.is_none()) after_edge_copy_fn_object(h_old, tm_old, h_new, tm_new);
  }

  void before_edge_duplicated(Hd h_old, Pm& tm) const {
      if (! before_edge_duplicated_fn_object.is_none()) before_edge_duplicated_fn_object(h_old, tm);
  }

  void after_edge_duplicated(Hd h_old, Hd h_new, Pm& tm) const {
      if (! after_edge_duplicated_fn_object.is_none()) after_edge_duplicated_fn_object(h_old, h_new, tm);
  }

  void intersection_edge_copy(Hd h_old1, const Pm& tm1, Hd h_old2, const Pm& tm2, Hd h_new, Pm& tm_new) const {
      if (! intersection_edge_copy_fn_object.is_none()) intersection_edge_copy_fn_object(h_old1, tm1, h_old2, tm2, h_new, tm_new);
  }

  void new_vertex_added(std::size_t node_id, Vd vh, const Pm& tm) const {
      if (! new_vertex_added_fn_object.is_none()) new_vertex_added_fn_object(node_id, vh, tm);
  }

  void intersection_point_detected(std::size_t node_id, int sdim, Hd principal_edge, Hd additional_edge, const Pm& tm1, const Pm& tm2, bool is_target_coplanar, bool is_source_coplanar) const {
      if (! intersection_point_detected_fn_object.is_none()) intersection_point_detected_fn_object(node_id, sdim, principal_edge, additional_edge, tm1, tm2, is_target_coplanar, is_source_coplanar);
  }

  void before_vertex_copy(Vd v_src, const Pm& tm_src, Pm& tm_tgt) const {
      if (! before_vertex_copy_fn_object.is_none()) before_vertex_copy_fn_object(v_src, tm_src, tm_tgt);
  }

  void after_vertex_copy(Vd v_src, const Pm& tm_src, Vd v_tgt, Pm& tm_tgt) const {
      if (! after_vertex_copy_fn_object.is_none()) after_vertex_copy_fn_object(v_src, tm_src, v_tgt, tm_tgt);
  }

  void start_filtering_intersections() const {
      if (! start_filtering_intersections_fn_object.is_none()) start_filtering_intersections_fn_object();
  }

  void progress_filtering_intersections(double d) const {
      if (! progress_filtering_intersections_fn_object.is_none()) progress_filtering_intersections_fn_object(d);
  }

  void end_filtering_intersections() const {
      if (! end_filtering_intersections_fn_object.is_none()) end_filtering_intersections_fn_object();
  }

  void start_triangulating_faces(std::size_t n) const {
      if (! start_triangulating_faces_fn_object.is_none()) start_triangulating_faces_fn_object(n);
  }

  void triangulating_faces_step() const {
      if (! triangulating_faces_step_fn_object.is_none()) triangulating_faces_step_fn_object();
  }

  void end_triangulating_faces() const {
      if (! end_triangulating_faces_fn_object.is_none()) end_triangulating_faces_fn_object();
  }

  void start_handling_intersection_of_coplanar_faces(std::size_t n) const {
      if (! start_handling_intersection_of_coplanar_faces_fn_object.is_none()) start_handling_intersection_of_coplanar_faces_fn_object(n);
  }

  void intersection_of_coplanar_faces_step() const {
      if (! intersection_of_coplanar_faces_step_fn_object.is_none()) intersection_of_coplanar_faces_step_fn_object();
  }

  void end_handling_intersection_of_coplanar_faces() const {
      if (! end_handling_intersection_of_coplanar_faces_fn_object.is_none()) end_handling_intersection_of_coplanar_faces_fn_object();
  }

  void start_handling_edge_face_intersections(std::size_t n) const {
      if (! start_handling_edge_face_intersections_fn_object.is_none()) start_handling_edge_face_intersections_fn_object(n);
  }

  void edge_face_intersections_step() const {
      if (! edge_face_intersections_step_fn_object.is_none()) edge_face_intersections_step_fn_object();
  }

  void end_handling_edge_face_intersections() const {
      if (! end_handling_edge_face_intersections_fn_object.is_none()) end_handling_edge_face_intersections_fn_object();
  }

  void start_building_output() const {
      if (! start_building_output_fn_object.is_none()) start_building_output_fn_object();
  }

  void end_building_output() const {
      if (! end_building_output_fn_object.is_none()) end_building_output_fn_object();
  }

  void filter_coplanar_edges() const {
      if (! filter_coplanar_edges_fn_object.is_none()) filter_coplanar_edges_fn_object();
  }

  void detect_patches() const {
      if (! detect_patches_fn_object.is_none()) detect_patches_fn_object();
  }

  void classify_patches() const {
      if (! classify_patches_fn_object.is_none()) classify_patches_fn_object();
  }

  void classify_intersection_free_patches(const Pm& tm) const {
      if (! classify_intersection_free_patches_fn_object.is_none()) classify_intersection_free_patches_fn_object(tm);
  }

  void out_of_place_operation(Boolean_operation_type t) const {
      if (! out_of_place_operation_fn_object.is_none()) out_of_place_operation_fn_object(t);
  }

  void in_place_operation(Boolean_operation_type t) const {
      if (! in_place_operation_fn_object.is_none()) in_place_operation_fn_object(t);
  }

  void in_place_operations(Boolean_operation_type t1, Boolean_operation_type t2) const {
      if (! in_place_operations_fn_object.is_none()) in_place_operations_fn_object(t1, t2);
  }



  void set_before_subface_creations(const py::object& fn) { before_subface_creations_fn_object = fn; }
  void set_after_subface_creations(const py::object& fn) { after_subface_creations_fn_object = fn; }
  void set_before_subface_created(const py::object& fn) { before_subface_created_fn_object = fn; }
  void set_after_subface_created(const py::object& fn) { after_subface_created_fn_object = fn; }
  void set_before_face_copy(const py::object& fn) { before_face_copy_fn_object = fn; }
  void set_after_face_copy(const py::object& fn) { after_face_copy_fn_object = fn; }
  void set_before_edge_split(const py::object& fn) { before_edge_split_fn_object = fn; }
  void set_edge_split(const py::object& fn) { edge_split_fn_object = fn; }
  void set_after_edge_split(const py::object& fn) { after_edge_split_fn_object = fn; }
  void set_add_retriangulation_edge(const py::object& fn) { add_retriangulation_edge_fn_object = fn; }
  void set_before_edge_copy(const py::object& fn) { before_edge_copy_fn_object = fn; }
  void set_after_edge_copy(const py::object& fn) { after_edge_copy_fn_object = fn; }
  void set_before_edge_duplicated(const py::object& fn) { before_edge_duplicated_fn_object = fn; }
  void set_after_edge_duplicated(const py::object& fn) { after_edge_duplicated_fn_object = fn; }
  void set_intersection_edge_copy(const py::object& fn) { intersection_edge_copy_fn_object = fn; }
  void set_new_vertex_added(const py::object& fn) { new_vertex_added_fn_object = fn; }
  void set_intersection_point_detected(const py::object& fn) { intersection_point_detected_fn_object = fn; }
  void set_before_vertex_copy(const py::object& fn) { before_vertex_copy_fn_object = fn; }
  void set_after_vertex_copy(const py::object& fn) { after_vertex_copy_fn_object = fn; }
  void set_start_filtering_intersections(const py::object& fn) { start_filtering_intersections_fn_object = fn; }
  void set_progress_filtering_intersections(const py::object& fn) { progress_filtering_intersections_fn_object = fn; }
  void set_end_filtering_intersections(const py::object& fn) { end_filtering_intersections_fn_object = fn; }
  void set_start_triangulating_faces(const py::object& fn) { start_triangulating_faces_fn_object = fn; }
  void set_triangulating_faces_step(const py::object& fn) { triangulating_faces_step_fn_object = fn; }
  void set_end_triangulating_faces(const py::object& fn) { end_triangulating_faces_fn_object = fn; }
  void set_start_handling_intersection_of_coplanar_faces(const py::object& fn) { start_handling_intersection_of_coplanar_faces_fn_object = fn; }
  void set_intersection_of_coplanar_faces_step(const py::object& fn) { intersection_of_coplanar_faces_step_fn_object = fn; }
  void set_end_handling_intersection_of_coplanar_faces(const py::object& fn) { end_handling_intersection_of_coplanar_faces_fn_object = fn; }
  void set_start_handling_edge_face_intersections(const py::object& fn) { start_handling_edge_face_intersections_fn_object = fn; }
  void set_edge_face_intersections_step(const py::object& fn) { edge_face_intersections_step_fn_object = fn; }
  void set_end_handling_edge_face_intersections(const py::object& fn) { end_handling_edge_face_intersections_fn_object = fn; }
  void set_start_building_output(const py::object& fn) { start_building_output_fn_object = fn; }
  void set_end_building_output(const py::object& fn) { end_building_output_fn_object = fn; }
  void set_filter_coplanar_edges(const py::object& fn) { filter_coplanar_edges_fn_object = fn; }
  void set_detect_patches(const py::object& fn) { detect_patches_fn_object = fn; }
  void set_classify_patches(const py::object& fn) { classify_patches_fn_object = fn; }
  void set_classify_intersection_free_patches(const py::object& fn) { classify_intersection_free_patches_fn_object = fn; }
  void set_out_of_place_operation(const py::object& fn) { out_of_place_operation_fn_object = fn; }
  void set_in_place_operation(const py::object& fn) { in_place_operation_fn_object = fn; }
  void set_in_place_operations(const py::object& fn) { in_place_operations_fn_object = fn; }


  static int tp_traverse(PyObject* self, visitproc visit, void* arg) {
    Corefine_visitor<PolygonalMesh>* w =
      py::inst_ptr<Corefine_visitor<PolygonalMesh>>(self);

    py::handle value_before_subface_creations_fn = py::find(w->before_subface_creations_fn_object);
    py::handle value_after_subface_creations_fn = py::find(w->after_subface_creations_fn_object);
    py::handle value_before_subface_created_fn = py::find(w->before_subface_created_fn_object);
    py::handle value_after_subface_created_fn = py::find(w->after_subface_created_fn_object);
    py::handle value_before_face_copy_fn = py::find(w->before_face_copy_fn_object);
    py::handle value_after_face_copy_fn = py::find(w->after_face_copy_fn_object);
    py::handle value_before_edge_split_fn = py::find(w->before_edge_split_fn_object);
    py::handle value_edge_split_fn = py::find(w->edge_split_fn_object);
    py::handle value_after_edge_split_fn = py::find(w->after_edge_split_fn_object);
    py::handle value_add_retriangulation_edge_fn = py::find(w->add_retriangulation_edge_fn_object);
    py::handle value_before_edge_copy_fn = py::find(w->before_edge_copy_fn_object);
    py::handle value_after_edge_copy_fn = py::find(w->after_edge_copy_fn_object);
    py::handle value_before_edge_duplicated_fn = py::find(w->before_edge_duplicated_fn_object);
    py::handle value_after_edge_duplicated_fn = py::find(w->after_edge_duplicated_fn_object);
    py::handle value_intersection_edge_copy_fn = py::find(w->intersection_edge_copy_fn_object);
    py::handle value_new_vertex_added_fn = py::find(w->new_vertex_added_fn_object);
    py::handle value_intersection_point_detected_fn = py::find(w->intersection_point_detected_fn_object);
    py::handle value_before_vertex_copy_fn = py::find(w->before_vertex_copy_fn_object);
    py::handle value_after_vertex_copy_fn = py::find(w->after_vertex_copy_fn_object);
    py::handle value_start_filtering_intersections_fn = py::find(w->start_filtering_intersections_fn_object);
    py::handle value_progress_filtering_intersections_fn = py::find(w->progress_filtering_intersections_fn_object);
    py::handle value_end_filtering_intersections_fn = py::find(w->end_filtering_intersections_fn_object);
    py::handle value_start_triangulating_faces_fn = py::find(w->start_triangulating_faces_fn_object);
    py::handle value_triangulating_faces_step_fn = py::find(w->triangulating_faces_step_fn_object);
    py::handle value_end_triangulating_faces_fn = py::find(w->end_triangulating_faces_fn_object);
    py::handle value_start_handling_intersection_of_coplanar_faces_fn = py::find(w->start_handling_intersection_of_coplanar_faces_fn_object);
    py::handle value_intersection_of_coplanar_faces_step_fn = py::find(w->intersection_of_coplanar_faces_step_fn_object);
    py::handle value_end_handling_intersection_of_coplanar_faces_fn = py::find(w->end_handling_intersection_of_coplanar_faces_fn_object);
    py::handle value_start_handling_edge_face_intersections_fn = py::find(w->start_handling_edge_face_intersections_fn_object);
    py::handle value_edge_face_intersections_step_fn = py::find(w->edge_face_intersections_step_fn_object);
    py::handle value_end_handling_edge_face_intersections_fn = py::find(w->end_handling_edge_face_intersections_fn_object);
    py::handle value_start_building_output_fn = py::find(w->start_building_output_fn_object);
    py::handle value_end_building_output_fn = py::find(w->end_building_output_fn_object);
    py::handle value_filter_coplanar_edges_fn = py::find(w->filter_coplanar_edges_fn_object);
    py::handle value_detect_patches_fn = py::find(w->detect_patches_fn_object);
    py::handle value_classify_patches_fn = py::find(w->classify_patches_fn_object);
    py::handle value_classify_intersection_free_patches_fn = py::find(w->classify_intersection_free_patches_fn_object);
    py::handle value_out_of_place_operation_fn = py::find(w->out_of_place_operation_fn_object);
    py::handle value_in_place_operation_fn = py::find(w->in_place_operation_fn_object);
    py::handle value_in_place_operations_fn = py::find(w->in_place_operations_fn_object);

    Py_VISIT(value_before_subface_creations_fn.ptr());
    Py_VISIT(value_after_subface_creations_fn.ptr());
    Py_VISIT(value_before_subface_created_fn.ptr());
    Py_VISIT(value_after_subface_created_fn.ptr());
    Py_VISIT(value_before_face_copy_fn.ptr());
    Py_VISIT(value_after_face_copy_fn.ptr());
    Py_VISIT(value_before_edge_split_fn.ptr());
    Py_VISIT(value_edge_split_fn.ptr());
    Py_VISIT(value_after_edge_split_fn.ptr());
    Py_VISIT(value_add_retriangulation_edge_fn.ptr());
    Py_VISIT(value_before_edge_copy_fn.ptr());
    Py_VISIT(value_after_edge_copy_fn.ptr());
    Py_VISIT(value_before_edge_duplicated_fn.ptr());
    Py_VISIT(value_after_edge_duplicated_fn.ptr());
    Py_VISIT(value_intersection_edge_copy_fn.ptr());
    Py_VISIT(value_new_vertex_added_fn.ptr());
    Py_VISIT(value_intersection_point_detected_fn.ptr());
    Py_VISIT(value_before_vertex_copy_fn.ptr());
    Py_VISIT(value_after_vertex_copy_fn.ptr());
    Py_VISIT(value_start_filtering_intersections_fn.ptr());
    Py_VISIT(value_progress_filtering_intersections_fn.ptr());
    Py_VISIT(value_end_filtering_intersections_fn.ptr());
    Py_VISIT(value_start_triangulating_faces_fn.ptr());
    Py_VISIT(value_triangulating_faces_step_fn.ptr());
    Py_VISIT(value_end_triangulating_faces_fn.ptr());
    Py_VISIT(value_start_handling_intersection_of_coplanar_faces_fn.ptr());
    Py_VISIT(value_intersection_of_coplanar_faces_step_fn.ptr());
    Py_VISIT(value_end_handling_intersection_of_coplanar_faces_fn.ptr());
    Py_VISIT(value_start_handling_edge_face_intersections_fn.ptr());
    Py_VISIT(value_edge_face_intersections_step_fn.ptr());
    Py_VISIT(value_end_handling_edge_face_intersections_fn.ptr());
    Py_VISIT(value_start_building_output_fn.ptr());
    Py_VISIT(value_end_building_output_fn.ptr());
    Py_VISIT(value_filter_coplanar_edges_fn.ptr());
    Py_VISIT(value_detect_patches_fn.ptr());
    Py_VISIT(value_classify_patches_fn.ptr());
    Py_VISIT(value_classify_intersection_free_patches_fn.ptr());
    Py_VISIT(value_out_of_place_operation_fn.ptr());
    Py_VISIT(value_in_place_operation_fn.ptr());
    Py_VISIT(value_in_place_operations_fn.ptr());

#if PY_VERSION_HEX >= 0x03090000
    Py_VISIT(Py_TYPE(self));
#endif

    return 0;
  }

  static int tp_clear(PyObject* self) {
    Corefine_visitor<PolygonalMesh>* w =
      py::inst_ptr<Corefine_visitor<PolygonalMesh>>(self);

    w->before_subface_creations_fn_object = {};
    w->after_subface_creations_fn_object = {};
    w->before_subface_created_fn_object = {};
    w->after_subface_created_fn_object = {};
    w->before_face_copy_fn_object = {};
    w->after_face_copy_fn_object = {};
    w->before_edge_split_fn_object = {};
    w->edge_split_fn_object = {};
    w->after_edge_split_fn_object = {};
    w->add_retriangulation_edge_fn_object = {};
    w->before_edge_copy_fn_object = {};
    w->after_edge_copy_fn_object = {};
    w->before_edge_duplicated_fn_object = {};
    w->after_edge_duplicated_fn_object = {};
    w->intersection_edge_copy_fn_object = {};
    w->new_vertex_added_fn_object = {};
    w->intersection_point_detected_fn_object = {};
    w->before_vertex_copy_fn_object = {};
    w->after_vertex_copy_fn_object = {};
    w->start_filtering_intersections_fn_object = {};
    w->progress_filtering_intersections_fn_object = {};
    w->end_filtering_intersections_fn_object = {};
    w->start_triangulating_faces_fn_object = {};
    w->triangulating_faces_step_fn_object = {};
    w->end_triangulating_faces_fn_object = {};
    w->start_handling_intersection_of_coplanar_faces_fn_object = {};
    w->intersection_of_coplanar_faces_step_fn_object = {};
    w->end_handling_intersection_of_coplanar_faces_fn_object = {};
    w->start_handling_edge_face_intersections_fn_object = {};
    w->edge_face_intersections_step_fn_object = {};
    w->end_handling_edge_face_intersections_fn_object = {};
    w->start_building_output_fn_object = {};
    w->end_building_output_fn_object = {};
    w->filter_coplanar_edges_fn_object = {};
    w->detect_patches_fn_object = {};
    w->classify_patches_fn_object = {};
    w->classify_intersection_free_patches_fn_object = {};
    w->out_of_place_operation_fn_object = {};
    w->in_place_operation_fn_object = {};
    w->in_place_operations_fn_object = {};


    return 0;
  }

private:

  py::object before_subface_creations_fn_object;
  py::object after_subface_creations_fn_object;
  py::object before_subface_created_fn_object;
  py::object after_subface_created_fn_object;
  py::object before_face_copy_fn_object;
  py::object after_face_copy_fn_object;
  py::object before_edge_split_fn_object;
  py::object edge_split_fn_object;
  py::object after_edge_split_fn_object;
  py::object add_retriangulation_edge_fn_object;
  py::object before_edge_copy_fn_object;
  py::object after_edge_copy_fn_object;
  py::object before_edge_duplicated_fn_object;
  py::object after_edge_duplicated_fn_object;
  py::object intersection_edge_copy_fn_object;
  py::object new_vertex_added_fn_object;
  py::object intersection_point_detected_fn_object;
  py::object before_vertex_copy_fn_object;
  py::object after_vertex_copy_fn_object;
  py::object start_filtering_intersections_fn_object;
  py::object progress_filtering_intersections_fn_object;
  py::object end_filtering_intersections_fn_object;
  py::object start_triangulating_faces_fn_object;
  py::object triangulating_faces_step_fn_object;
  py::object end_triangulating_faces_fn_object;
  py::object start_handling_intersection_of_coplanar_faces_fn_object;
  py::object intersection_of_coplanar_faces_step_fn_object;
  py::object end_handling_intersection_of_coplanar_faces_fn_object;
  py::object start_handling_edge_face_intersections_fn_object;
  py::object edge_face_intersections_step_fn_object;
  py::object end_handling_edge_face_intersections_fn_object;
  py::object start_building_output_fn_object;
  py::object end_building_output_fn_object;
  py::object filter_coplanar_edges_fn_object;
  py::object detect_patches_fn_object;
  py::object classify_patches_fn_object;
  py::object classify_intersection_free_patches_fn_object;
  py::object out_of_place_operation_fn_object;
  py::object in_place_operation_fn_object;
  py::object in_place_operations_fn_object;
};

}
} // namespace cgalpy

#endif
