// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARR_OBSERVER_HPP
#define CGALPY_ARR_OBSERVER_HPP

#include <CGAL/Arr_observer.h>

#include <nanobind/nanobind.h>

namespace py = nanobind;

template <typename Aos, typename Data>
class Arr_observer : public CGAL::Arr_observer<Aos> {
public:
  using Vertex_handle = typename Aos::Vertex_handle;
  using Halfedge_handle = typename Aos::Halfedge_handle;
  using Face_handle = typename Aos::Face_handle;

  using Ccb_halfedge_circulator = typename Aos::Ccb_halfedge_circulator;

  using X_monotone_curve_2 = typename Aos::X_monotone_curve_2;
  using Point_2 = typename Aos::Point_2;
  using Arr_curve_end = CGAL::Arr_curve_end;
  using Arr_parameter_space = CGAL::Arr_parameter_space;

  // Construct default
  Arr_observer() :
    m_before_assign(py::none()),
    m_after_assign(py::none()),
    m_before_clear(py::none()),
    m_after_clear(py::none()),
    m_before_global_change(py::none()),
    m_after_global_change(py::none()),
    m_before_attach(py::none()),
    m_after_attach(py::none()),
    m_before_detach(py::none()),
    m_after_detach(py::none()),
    m_before_create_vertex(py::none()),
    m_after_create_vertex(py::none()),
    m_before_create_boundary_vertex1(py::none()),
    m_before_create_boundary_vertex2(py::none()),
    m_after_create_boundary_vertex(py::none()),
    m_before_create_edge(py::none()),
    m_after_create_edge(py::none()),
    m_before_modify_vertex(py::none()),
    m_after_modify_vertex(py::none()),
    m_before_modify_edge(py::none()),
    m_after_modify_edge(py::none()),
    m_before_split_edge(py::none()),
    m_after_split_edge(py::none()),
    m_before_split_fictitious_edge(py::none()),
    m_after_split_fictitious_edge(py::none()),
    m_before_split_face(py::none()),
    m_after_split_face(py::none()),
    m_before_split_outer_ccb(py::none()),
    m_after_split_outer_ccb(py::none()),
    m_before_split_inner_ccb(py::none()),
    m_after_split_inner_ccb(py::none()),
    m_before_add_outer_ccb(py::none()),
    m_after_add_outer_ccb(py::none()),
    m_before_add_inner_ccb(py::none()),
    m_after_add_inner_ccb(py::none()),
    m_before_add_isolated_vertex(py::none()),
    m_after_add_isolated_vertex(py::none()),
    m_before_merge_edge(py::none()),
    m_after_merge_edge(py::none()),
    m_before_merge_fictitious_edge(py::none()),
    m_after_merge_fictitious_edge(py::none()),
    m_before_merge_face(py::none()),
    m_after_merge_face(py::none()),
    m_before_merge_outer_ccb(py::none()),
    m_after_merge_outer_ccb(py::none()),
    m_before_merge_inner_ccb(py::none()),
    m_after_merge_inner_ccb(py::none()),
    m_before_move_outer_ccb(py::none()),
    m_after_move_outer_ccb(py::none()),
    m_before_move_inner_ccb(py::none()),
    m_after_move_inner_ccb(py::none()),
    m_before_move_isolated_vertex(py::none()),
    m_after_move_isolated_vertex(py::none()),
    m_before_remove_vertex(py::none()),
    m_after_remove_vertex(py::none()),
    m_before_remove_edge(py::none()),
    m_after_remove_edge(py::none()),
    m_before_remove_outer_ccb(py::none()),
    m_after_remove_outer_ccb(py::none()),
    m_before_remove_inner_ccb(py::none()),
    m_after_remove_inner_ccb(py::none())
  {}

  // Construct
  Arr_observer(Aos& arr) :
    CGAL::Arr_observer<Aos>(arr),
    m_before_assign(py::none()),
    m_after_assign(py::none()),
    m_before_clear(py::none()),
    m_after_clear(py::none()),
    m_before_global_change(py::none()),
    m_after_global_change(py::none()),
    m_before_attach(py::none()),
    m_after_attach(py::none()),
    m_before_detach(py::none()),
    m_after_detach(py::none()),
    m_before_create_vertex(py::none()),
    m_after_create_vertex(py::none()),
    m_before_create_boundary_vertex1(py::none()),
    m_before_create_boundary_vertex2(py::none()),
    m_after_create_boundary_vertex(py::none()),
    m_before_create_edge(py::none()),
    m_after_create_edge(py::none()),
    m_before_modify_vertex(py::none()),
    m_after_modify_vertex(py::none()),
    m_before_modify_edge(py::none()),
    m_after_modify_edge(py::none()),
    m_before_split_edge(py::none()),
    m_after_split_edge(py::none()),
    m_before_split_fictitious_edge(py::none()),
    m_after_split_fictitious_edge(py::none()),
    m_before_split_face(py::none()),
    m_after_split_face(py::none()),
    m_before_split_outer_ccb(py::none()),
    m_after_split_outer_ccb(py::none()),
    m_before_split_inner_ccb(py::none()),
    m_after_split_inner_ccb(py::none()),
    m_before_add_outer_ccb(py::none()),
    m_after_add_outer_ccb(py::none()),
    m_before_add_inner_ccb(py::none()),
    m_after_add_inner_ccb(py::none()),
    m_before_add_isolated_vertex(py::none()),
    m_after_add_isolated_vertex(py::none()),
    m_before_merge_edge(py::none()),
    m_after_merge_edge(py::none()),
    m_before_merge_fictitious_edge(py::none()),
    m_after_merge_fictitious_edge(py::none()),
    m_before_merge_face(py::none()),
    m_after_merge_face(py::none()),
    m_before_merge_outer_ccb(py::none()),
    m_after_merge_outer_ccb(py::none()),
    m_before_merge_inner_ccb(py::none()),
    m_after_merge_inner_ccb(py::none()),
    m_before_move_outer_ccb(py::none()),
    m_after_move_outer_ccb(py::none()),
    m_before_move_inner_ccb(py::none()),
    m_after_move_inner_ccb(py::none()),
    m_before_move_isolated_vertex(py::none()),
    m_after_move_isolated_vertex(py::none()),
    m_before_remove_vertex(py::none()),
    m_after_remove_vertex(py::none()),
    m_before_remove_edge(py::none()),
    m_after_remove_edge(py::none()),
    m_before_remove_outer_ccb(py::none()),
    m_after_remove_outer_ccb(py::none()),
    m_before_remove_inner_ccb(py::none()),
    m_after_remove_inner_ccb(py::none())
  {}

  // Destruct
  ~Arr_observer() {
    m_before_assign = py::none();
    m_after_assign = py::none();
    m_before_clear = py::none();
    m_after_clear = py::none();
    m_before_global_change = py::none();
    m_after_global_change = py::none();
    m_before_attach = py::none();
    m_after_attach = py::none();
    m_before_detach = py::none();
    m_after_detach = py::none();
    m_before_create_vertex = py::none();
    m_after_create_vertex = py::none();
    m_before_create_boundary_vertex1 = py::none();
    m_before_create_boundary_vertex2 = py::none();
    m_after_create_boundary_vertex = py::none();
    m_before_create_edge = py::none();
    m_after_create_edge = py::none();
    m_before_modify_vertex = py::none();
    m_after_modify_vertex = py::none();
    m_before_modify_edge = py::none();
    m_after_modify_edge = py::none();
    m_before_split_edge = py::none();
    m_after_split_edge = py::none();
    m_before_split_fictitious_edge = py::none();
    m_after_split_fictitious_edge = py::none();
    m_before_split_face = py::none();
    m_after_split_face = py::none();
    m_before_split_outer_ccb = py::none();
    m_after_split_outer_ccb = py::none();
    m_before_split_inner_ccb = py::none();
    m_after_split_inner_ccb = py::none();
    m_before_add_outer_ccb = py::none();
    m_after_add_outer_ccb = py::none();
    m_before_add_inner_ccb = py::none();
    m_after_add_inner_ccb = py::none();
    m_before_add_isolated_vertex = py::none();
    m_after_add_isolated_vertex = py::none();
    m_before_merge_edge = py::none();
    m_after_merge_edge = py::none();
    m_before_merge_fictitious_edge = py::none();
    m_after_merge_fictitious_edge = py::none();
    m_before_merge_face = py::none();
    m_after_merge_face = py::none();
    m_before_merge_outer_ccb = py::none();
    m_after_merge_outer_ccb = py::none();
    m_before_merge_inner_ccb = py::none();
    m_after_merge_inner_ccb = py::none();
    m_before_move_outer_ccb = py::none();
    m_after_move_outer_ccb = py::none();
    m_before_move_inner_ccb = py::none();
    m_after_move_inner_ccb = py::none();
    m_before_move_isolated_vertex = py::none();
    m_after_move_isolated_vertex = py::none();
    m_before_remove_vertex = py::none();
    m_after_remove_vertex = py::none();
    m_before_remove_edge = py::none();
    m_after_remove_edge = py::none();
    m_before_remove_outer_ccb = py::none();
    m_after_remove_outer_ccb = py::none();
    m_before_remove_inner_ccb = py::none();
    m_after_remove_inner_ccb = py::none();
  }

  /// Setters
  /// @{

  void set_before_assign(py::object fnc) { m_before_assign = fnc; }
  void set_after_assign(py::object fnc) { m_after_assign = fnc; }
  void set_before_clear(py::object fnc) { m_before_clear = fnc; }
  void set_after_clear(py::object fnc) { m_after_clear = fnc; }
  void set_before_global_change(py::object fnc) { m_before_global_change = fnc; }
  void set_after_global_change(py::object fnc) { m_after_global_change = fnc; }
  void set_before_attach(py::object fnc) { m_before_attach = fnc; }
  void set_after_attach(py::object fnc) { m_after_attach = fnc; }
  void set_before_detach(py::object fnc) { m_before_detach = fnc; }
  void set_after_detach(py::object fnc) { m_after_detach = fnc; }
  void set_before_create_vertex(py::object fnc) { m_before_create_vertex = fnc; }
  void set_after_create_vertex(py::object fnc) { m_after_create_vertex = fnc; }
  void set_before_create_boundary_vertex1(py::object fnc)
  { m_before_create_boundary_vertex1 = fnc; }
  void set_before_create_boundary_vertex2(py::object fnc)
  { m_before_create_boundary_vertex2 = fnc; }
  void set_after_create_boundary_vertex(py::object fnc)
  { m_after_create_boundary_vertex = fnc; }
  void set_before_create_edge(py::object fnc) { m_before_create_edge = fnc; }
  void set_after_create_edge(py::object fnc) { m_after_create_edge = fnc; }
  void set_before_modify_vertex(py::object fnc) { m_before_modify_vertex = fnc; }
  void set_after_modify_vertex(py::object fnc) { m_after_modify_vertex = fnc; }
  void set_before_modify_edge(py::object fnc) { m_before_modify_edge = fnc; }
  void set_after_modify_edge(py::object fnc) { m_after_modify_edge = fnc; }
  void set_before_split_edge(py::object fnc) { m_before_split_edge = fnc; }
  void set_after_split_edge(py::object fnc) { m_after_split_edge = fnc; }
  void set_before_split_fictitious_edge(py::object fnc)
  { m_before_split_fictitious_edge = fnc; }
  void set_after_split_fictitious_edge(py::object fnc)
  { m_after_split_fictitious_edge = fnc; }
  void set_before_split_face(py::object fnc) { m_before_split_face = fnc; }
  void set_after_split_face(py::object fnc) { m_after_split_face = fnc; }
  void set_before_split_outer_ccb(py::object fnc)
  { m_before_split_outer_ccb = fnc; }
  void set_after_split_outer_ccb(py::object fnc)
  { m_after_split_outer_ccb = fnc; }
  void set_before_split_inner_ccb(py::object fnc)
  { m_before_split_inner_ccb = fnc; }
  void set_after_split_inner_ccb(py::object fnc)
  { m_after_split_inner_ccb = fnc; }
  void set_before_add_outer_ccb(py::object fnc) { m_before_add_outer_ccb = fnc; }
  void set_after_add_outer_ccb(py::object fnc) { m_after_add_outer_ccb = fnc; }
  void set_before_add_inner_ccb(py::object fnc) { m_before_add_inner_ccb = fnc; }
  void set_after_add_inner_ccb(py::object fnc) { m_after_add_inner_ccb = fnc; }
  void set_before_add_isolated_vertex(py::object fnc)
  { m_before_add_isolated_vertex = fnc; }
  void set_after_add_isolated_vertex(py::object fnc)
  { m_after_add_isolated_vertex = fnc; }
  void set_before_merge_edge(py::object fnc) { m_before_merge_edge = fnc; }
  void set_after_merge_edge(py::object fnc) { m_after_merge_edge = fnc; }
  void set_before_merge_fictitious_edge(py::object fnc)
  { m_before_merge_fictitious_edge = fnc; }
  void set_after_merge_fictitious_edge(py::object fnc)
  { m_after_merge_fictitious_edge = fnc; }
  void set_before_merge_face(py::object fnc) { m_before_merge_face = fnc; }
  void set_after_merge_face(py::object fnc) { m_after_merge_face = fnc; }
  void set_before_merge_outer_ccb(py::object fnc)
  { m_before_merge_outer_ccb = fnc; }
  void set_after_merge_outer_ccb(py::object fnc)
  { m_after_merge_outer_ccb = fnc; }
  void set_before_merge_inner_ccb(py::object fnc)
  { m_before_merge_inner_ccb = fnc; }
  void set_after_merge_inner_ccb(py::object fnc)
  { m_after_merge_inner_ccb = fnc; }
  void set_before_move_outer_ccb(py::object fnc)
  { m_before_move_outer_ccb = fnc; }
  void set_after_move_outer_ccb(py::object fnc) { m_after_move_outer_ccb = fnc; }
  void set_before_move_inner_ccb(py::object fnc)
  { m_before_move_inner_ccb = fnc; }
  void set_after_move_inner_ccb(py::object fnc) { m_after_move_inner_ccb = fnc; }
  void set_before_move_isolated_vertex(py::object fnc)
  { m_before_move_isolated_vertex = fnc; }
  void set_after_move_isolated_vertex(py::object fnc)
  { m_after_move_isolated_vertex = fnc; }
  void set_before_remove_vertex(py::object fnc) { m_before_remove_vertex = fnc; }
  void set_after_remove_vertex(py::object fnc) { m_after_remove_vertex = fnc; }
  void set_before_remove_edge(py::object fnc) { m_before_remove_edge = fnc; }
  void set_after_remove_edge(py::object fnc) { m_after_remove_edge = fnc; }
  void set_before_remove_outer_ccb(py::object fnc)
  { m_before_remove_outer_ccb = fnc; }
  void set_after_remove_outer_ccb(py::object fnc)
  { m_after_remove_outer_ccb = fnc; }
  void set_before_remove_inner_ccb(py::object fnc)
  { m_before_remove_inner_ccb = fnc; }
  void set_after_remove_inner_ccb(py::object fnc)
  { m_after_remove_inner_ccb = fnc; }

  /// @}

  /// \name Notification functions on global arrangement operations.
  //@{

  /*! Notification before the arrangement is assigned with another
   * arrangement.
   * \param arr The arrangement to be copied.
   */
  virtual void before_assign(const Aos& arr)
  { if (! m_before_assign.is_none()) m_before_assign(&arr); }

  /*! Notification after the arrangement has been assigned with another
   * arrangement.
   */
  virtual void after_assign()
  { if (! m_before_assign.is_none()) m_before_assign(); }

  /*! Notification before the arrangement is cleared. */
  virtual void before_clear()
  { if (! m_before_clear.is_none()) m_before_clear(); }

  /*! Notification after the arrangement is cleared. */
  virtual void after_clear()
  { if (! m_after_clear.is_none()) m_after_clear(); }

  /*! Notification before a global operation modifies the arrangement. */
  virtual void before_global_change()
  { if (! m_before_global_change.is_none()) m_before_global_change(); }

  /*! Notification after a global operation is completed. */
  virtual void after_global_change()
  { if (! m_after_global_change.is_none()) m_after_global_change(); }
  //@}

  /// \name Notification functions on observer attachment or detachment.
  //@{

  /*! Notification before the observer is attached to an arrangement.
   * \param arr The arrangement we are about to attach the observer to.
   */
  virtual void before_attach(const Aos& arr)
  { if (! m_before_attach.is_none()) m_before_attach(&arr); }

  /*! Notification after the observer has been attached to an arrangement. */
  virtual void after_attach()
  { if (! m_after_attach.is_none()) m_after_attach(); }

  /*! Notification before the observer is detached from the arrangement. */
  virtual void before_detach()
  { if (! m_before_detach.is_none()) m_before_detach(); }

  /*! Notification after the observer has been detached to the arrangement. */
  virtual void after_detach()
  { if (! m_after_detach.is_none()) m_after_detach(); }
  //@}

  /// \name Notification functions on local changes in the arrangement.
  //@{

  /*!
   * Notification before the creation of a new vertex.
   * \param p The point to be associated with the vertex.
   *          This point cannot lies on the surface boundaries.
   */
  virtual void before_create_vertex(const Point_2& p)
  { if (! m_before_create_vertex.is_none()) m_before_create_vertex(&p); }

  /*! Notification after the creation of a new vertex.
   * \param v A handle to the created vertex.
   */
  virtual void after_create_vertex(Vertex_handle v)
  { if (! m_after_create_vertex.is_none()) m_after_create_vertex(&*v); }

  /*! Notification before the creation of a new boundary vertex.
   * \param p The on the surface boundary.
   * \param ps_x The boundary condition of the vertex in x.
   * \param ps_y The boundary condition of the vertex in y.
   */
  virtual void before_create_boundary_vertex(const Point_2& p,
                                             Arr_parameter_space ps_x,
                                             Arr_parameter_space ps_y)
  {
    if (! m_before_create_boundary_vertex1.is_none())
      m_before_create_boundary_vertex1(&p, ps_x, ps_y);
  }

  /*! Notification before the creation of a new boundary vertex.
   * \param cv The curve incident to the surface boundary.
   * \param ind The relevant curve-end.
   * \param ps_x The boundary condition of the vertex in x.
   * \param ps_y The boundary condition of the vertex in y.
   */
  virtual void before_create_boundary_vertex(const X_monotone_curve_2& cv,
                                             Arr_curve_end ind,
                                             Arr_parameter_space ps_x,
                                             Arr_parameter_space ps_y)
  {
    if (! m_before_create_boundary_vertex2.is_none())
      m_before_create_boundary_vertex2(&cv, ind, ps_x, ps_y);
  }

  /*! Notification after the creation of a new vertex at infinity.
   * \param v A handle to the created vertex.
   */
  virtual void after_create_boundary_vertex(Vertex_handle v)
  {
    if (! m_after_create_boundary_vertex.is_none())
      m_after_create_boundary_vertex(&*v);
  }

  /*! Notification before the creation of a new edge.
   * \param c The x-monotone curve to be associated with the edge.
   * \param v1 A handle to the first end-vertex of the edge.
   * \param v2 A handle to the second end-vertex of the edge.
   */
  virtual void before_create_edge(const X_monotone_curve_2& c,
                                  Vertex_handle v1, Vertex_handle v2)
  { if (! m_before_create_edge.is_none()) m_before_create_edge(&c, &*v1, &*v2); }

  /*! Notification after the creation of a new edge.
   * \param e A handle to one of the twin halfedges that were created.
   */
  virtual void after_create_edge(Halfedge_handle e)
  { if (! m_after_create_edge.is_none()) m_after_create_edge(&*e); }

  /*! Notification before the modification of an existing vertex.
   * \param v A handle to the vertex to be updated.
   * \param p The point to be associated with the vertex.
   */
  virtual void before_modify_vertex(Vertex_handle v, const Point_2& p)
  { if (! m_before_modify_vertex.is_none()) m_before_modify_vertex(&*v, &p); }

  /*! Notification after a vertex was modified.
   * \param v A handle to the updated vertex.
   */
  virtual void after_modify_vertex(Vertex_handle v)
  { if (! m_after_modify_vertex.is_none()) m_after_modify_vertex(&*v); }

  /*! Notification before the modification of an existing edge.
   * \param e A handle to one of the twin halfedges to be updated.
   * \param c The x-monotone curve to be associated with the edge.
   */
  virtual void before_modify_edge(Halfedge_handle e,
                                  const X_monotone_curve_2& c)
  { if (! m_before_modify_edge.is_none()) m_before_modify_edge(&*e, &c); }

  /*! Notification after an edge was modified.
   * \param e A handle to one of the twin halfedges that were updated.
   */
  virtual void after_modify_edge(Halfedge_handle e)
  { if (! m_after_modify_edge.is_none()) m_after_modify_edge(&*e); }

  /*! Notification before the splitting of an edge into two.
   * \param e A handle to one of the existing halfedges.
   * \param v A vertex representing the split point.
   * \param c1 The x-monotone curve to be associated with the first edge.
   * \param c2 The x-monotone curve to be associated with the second edge.
   */
  virtual void before_split_edge(Halfedge_handle e, Vertex_handle v,
                                 const X_monotone_curve_2& c1,
                                 const X_monotone_curve_2& c2)
  {
    if (! m_before_split_edge.is_none())
      m_before_split_edge(&*e, &*v, &c1, &c2);
  }

  /*! Notification after an edge was split.
   * \param e1 A handle to one of the twin halfedges forming the first edge.
   * \param e2 A handle to one of the twin halfedges forming the second edge.
   */
  virtual void after_split_edge(Halfedge_handle e1, Halfedge_handle e2)
  { if (! m_after_split_edge.is_none()) m_after_split_edge(&*e1, &*e2); }

  /*! Notification before the splitting of a fictitious edge into two.
   * \param e A handle to one of the existing halfedges.
   * \param v A vertex representing the unbounded split point.
   */
  virtual void before_split_fictitious_edge(Halfedge_handle e,
                                            Vertex_handle v)
  {
    if (! m_before_split_fictitious_edge.is_none())
      m_before_split_fictitious_edge(&*e, &*v);
  }

  /*! Notification after a fictitious edge was split.
   * \param e1 A handle to one of the twin halfedges forming the first edge.
   * \param e2 A handle to one of the twin halfedges forming the second edge.
   */
  virtual void after_split_fictitious_edge(Halfedge_handle e1,
                                           Halfedge_handle e2)
  {
    if (! m_after_split_fictitious_edge.is_none())
      m_after_split_fictitious_edge(&*e1, &*e2);
  }

  /*! Notification before the splitting of a face into two.
   * \param f A handle to the existing face.
   * \param e The new edge whose insertion causes the face to split.
   */
  virtual void before_split_face(Face_handle f, Halfedge_handle e)
  { if (! m_before_split_face.is_none()) m_before_split_face(&*f, &*e); }

  /*! Notification after a face was split.
   * \param f A handle to the face we have just split.
   * \param new_f A handle to the new face that has been created.
   * \param is_hole Whether the new face forms a hole inside f.
   */
  virtual void after_split_face(Face_handle f, Face_handle nf, bool is_hole)
  { if (! m_after_split_face.is_none()) m_after_split_face(&*f, &*nf, is_hole); }

  /*! Notification before the splitting of an outer CCB into two.
   * \param f A handle to the face that owns the outer CCB.
   * \param h A circulator representing the component boundary.
   * \param e The new edge whose removal causes the outer CCB to split.
   */
  virtual void before_split_outer_ccb(Face_handle f,
                                      Ccb_halfedge_circulator h,
                                      Halfedge_handle e)
  {
    if (! m_before_split_outer_ccb.is_none())
      m_before_split_outer_ccb(&*f, &*h, &*e);
  }

  /*! Notification after an outer CCB was split.
   * \param f A handle to the face that owns the outer CCBs.
   * \param h1 A circulator representing the boundary of the first component.
   * \param h2 A circulator representing the boundary of the second component.
   */
  virtual void after_split_outer_ccb(Face_handle f,
                                     Ccb_halfedge_circulator h1,
                                     Ccb_halfedge_circulator h2)
  {
    if (! m_after_split_outer_ccb.is_none())
      m_after_split_outer_ccb(&*f, &*h1, &*h2);
  }

  /*! Notification before the splitting of an inner CCB into two.
   * \param f A handle to the face containing the inner CCB.
   * \param h A circulator representing the component boundary.
   * \param e The new edge whose removal causes the inner CCB to split.
   */
  virtual void before_split_inner_ccb(Face_handle f,
                                      Ccb_halfedge_circulator h,
                                      Halfedge_handle e)
  {
    if (! m_before_split_inner_ccb.is_none())
      m_before_split_inner_ccb(&*f, &*h, &*e);
  }

  /*! Notification after an inner CCB was split.
   * \param f A handle to the face containing the inner CCBs.
   * \param h1 A circulator representing the boundary of the first component.
   * \param h2 A circulator representing the boundary of the second component.
   */
  virtual void after_split_inner_ccb(Face_handle f,
                                     Ccb_halfedge_circulator h1,
                                     Ccb_halfedge_circulator h2)
  {
    if (! m_after_split_inner_ccb.is_none())
      m_after_split_inner_ccb(&*f, &*h1, &*h2);
  }

  /*! Notification before the creation of a new outer CCB of a face.
   * \param f A handle to the face that owns the outer CCB.
   * \param e A halfedge along the new outer CCB.
   */
  virtual void before_add_outer_ccb(Face_handle f,
                                    Halfedge_handle e)
  { if (! m_before_add_outer_ccb.is_none()) m_before_add_outer_ccb(&*f, &*e); }

  /*! Notification after an outer CCB was added to a face.
   * \param h A circulator representing the boundary of the new outer CCB.
   */
  virtual void after_add_outer_ccb(Ccb_halfedge_circulator h)
  { if (! m_after_add_outer_ccb.is_none()) m_after_add_outer_ccb(&*h); }

  /*! Notification before the creation of a new inner CCB inside a face.
   * \param f A handle to the face containing the inner CCB.
   * \param e The new halfedge that forms the new inner CCB.
   */
  virtual void before_add_inner_ccb(Face_handle f, Halfedge_handle e)
  { if (! m_before_add_inner_ccb.is_none()) m_before_add_inner_ccb(&*f, &*e); }

  /*! Notification after an inner CCB was created inside a face.
   * \param h A circulator representing the boundary of the new inner CCB.
   */
  virtual void after_add_inner_ccb(Ccb_halfedge_circulator h)
  { if (! m_after_add_inner_ccb.is_none()) m_after_add_inner_ccb(&*h); }

  /*! Notification before the creation of a new isolated vertex inside a face.
   * \param f A handle to the face containing the isolated vertex.
   * \param v The isolated vertex.
   */
  virtual void before_add_isolated_vertex(Face_handle f, Vertex_handle v)
  {
    if (! m_before_add_isolated_vertex.is_none())
      m_before_add_isolated_vertex(&*f, &*v);
  }

  /*! Notification after an isolated vertex was created inside a face.
   * \param v The isolated vertex.
   */
  virtual void after_add_isolated_vertex(Vertex_handle v)
  {
    if (! m_after_add_isolated_vertex.is_none())
      m_after_add_isolated_vertex(&*v);
  }

  /*! Notification before the merging of two edges.
   * \param e1 A handle to one of the halfedges forming the first edge.
   * \param e2 A handle to one of the halfedges forming the second edge.
   * \param c The x-monotone curve to be associated with the merged edge.
   */
  virtual void before_merge_edge(Halfedge_handle e1, Halfedge_handle e2,
                                 const X_monotone_curve_2& c)
  { if (! m_before_merge_edge.is_none()) m_before_merge_edge(&*e1, &*e2, &c); }

  /*! Notification after an edge was merged.
   * \param e A handle to one of the twin halfedges forming the merged edge.
   */
  virtual void after_merge_edge(Halfedge_handle e)
  { if (! m_after_merge_edge.is_none()) m_after_merge_edge(&*e); }

  /*! Notification before the merging of two fictitious edges.
   * \param e1 A handle to one of the halfedges forming the first edge.
   * \param e2 A handle to one of the halfedges forming the second edge.
   */
  virtual void before_merge_fictitious_edge(Halfedge_handle e1,
                                            Halfedge_handle e2)
  {
    if (! m_before_merge_fictitious_edge.is_none())
      m_before_merge_fictitious_edge(&*e1, &*e2);
  }

  /*! Notification after a fictitious edge was merged.
   * \param e A handle to one of the twin halfedges forming the merged edge.
   */
  virtual void after_merge_fictitious_edge(Halfedge_handle e)
  {
    if (! m_after_merge_fictitious_edge.is_none())
      m_after_merge_fictitious_edge(&*e);
  }

  /*! Notification before the merging of two faces.
   * \param f1 A handle to the first face.
   * \param f2 A handle to the second face.
   * \param e The edge whose removal causes the faces to merge.
   */
  virtual void before_merge_face(Face_handle f1, Face_handle f2,
                                 Halfedge_handle e)
  { if (! m_before_merge_face.is_none()) m_before_merge_face(&*f1, &*f2, &*e); }

  /*! Notification after a face was merged.
   * \param f A handle to the merged face.
   */
  virtual void after_merge_face(Face_handle f)
  { if (! m_after_merge_face.is_none()) m_after_merge_face(&*f); }

  /*! Notification before the merging of two outer CCBs.
   * \param f A handle to the face that owns the outer CCBs.
   * \param h1 A circulator representing the boundary of the first component.
   * \param h2 A circulator representing the boundary of the second component.
   * \param e The edge whose insertion or removal causes the CCBs to merge.
   */
  virtual void before_merge_outer_ccb(Face_handle f,
                                      Ccb_halfedge_circulator h1,
                                      Ccb_halfedge_circulator h2,
                                      Halfedge_handle e)
  {
    if (! m_before_merge_outer_ccb.is_none())
      m_before_merge_outer_ccb(&*f, &*h1, &*h2, &*e);
  }

  /*! Notification after an outer CCB was merged.
   * \param f A handle to the face that owns the outer CCBs.
   * \param h A circulator representing the boundary of the merged component.
   */
  virtual void after_merge_outer_ccb(Face_handle f,
                                     Ccb_halfedge_circulator h)
  { if (! m_after_merge_outer_ccb.is_none()) m_after_merge_outer_ccb(&*f, &*h); }

  /*! Notification before the merging of two inner CCBs (holes).
   * \param f A handle to the face that contains the inner CCBs.
   * \param h1 A circulator representing the boundary of the first component.
   * \param h2 A circulator representing the boundary of the second component.
   * \param e The edge whose insertion causes the inner CCBs to merge.
   */
  virtual void before_merge_inner_ccb(Face_handle f,
                                      Ccb_halfedge_circulator h1,
                                      Ccb_halfedge_circulator h2,
                                      Halfedge_handle e)
  {
    if (! m_before_merge_inner_ccb.is_none())
      m_before_merge_inner_ccb(&*f, &*h1, &*h2, &*e);
  }

  /*! Notification after an inner CCB was merged.
   * \param f A handle to the face that contains the inner CCBs.
   * \param h A circulator representing the boundary of the merged component.
   */
  virtual void after_merge_inner_ccb(Face_handle f,
                                     Ccb_halfedge_circulator h)
  { if (! m_after_merge_inner_ccb.is_none()) m_after_merge_inner_ccb(&*f, &*h); }

  /*! Notification before an outer CCB is moved from one face to another.
   * \param from_f A handle to the face that currently owns the outer CCB.
   * \param to_f A handle to the face that should own the outer CCB.
   * \param h A circulator representing the boundary of the component.
   */
  virtual void before_move_outer_ccb(Face_handle from_f, Face_handle to_f,
                                     Ccb_halfedge_circulator h)
  {
    if (! m_before_move_outer_ccb.is_none())
      m_before_move_outer_ccb(&*from_f, &*to_f, &*h);
  }

  /*! Notification after an outer CCB is moved from one face to another.
   * \param h A circulator representing the boundary of the component.
   */
  virtual void after_move_outer_ccb(Ccb_halfedge_circulator h)
  { if (! m_after_move_outer_ccb.is_none()) m_after_move_outer_ccb(&*h); }


  /*! Notification before an inner CCB is moved from one face to another.
   * \param from_f A handle to the face currently containing the inner CCB.
   * \param to_f A handle to the face that should contain the inner CCB.
   * \param h A circulator representing the boundary of the component.
   */
  virtual void before_move_inner_ccb(Face_handle from_f, Face_handle to_f,
                                     Ccb_halfedge_circulator h)
  {
    if (! m_before_move_inner_ccb.is_none())
      m_before_move_inner_ccb(&*from_f, &*to_f, &*h);
  }

  /*!
   * Notification after an inner CCB is moved from one face to another.
   * \param h A circulator representing the boundary of the component.
   */
  virtual void after_move_inner_ccb(Ccb_halfedge_circulator h)
  { if (! m_after_move_inner_ccb.is_none()) m_after_move_inner_ccb(&*h); }

  /*! Notification before an isolated vertex is moved from one face to another.
   * \param from_f A handle to the face currently containing the vertex.
   * \param to_f A handle to the face that should contain the vertex.
   * \param v The isolated vertex.
   */
  virtual void before_move_isolated_vertex(Face_handle from_f, Face_handle to_f,
                                           Vertex_handle v)
  {
    if (! m_before_move_isolated_vertex.is_none())
      m_before_move_isolated_vertex(&*from_f, &*to_f, &*v);
  }

  /*! Notification after an isolated vertex is moved from one face to another.
   * \param v The isolated vertex.
   */
  virtual void after_move_isolated_vertex(Vertex_handle v)
  {
    if (! m_after_move_isolated_vertex.is_none())
      m_after_move_isolated_vertex(&*v);
  }

  /*! Notificaion before the removal of a vertex.
   * \param v A handle to the vertex to be deleted.
   */
  virtual void before_remove_vertex(Vertex_handle v)
  { if (! m_before_remove_vertex.is_none()) m_before_remove_vertex(&*v); }

  /*! Notificaion after the removal of a vertex. */
  virtual void after_remove_vertex()
  {  if (! m_after_remove_vertex.is_none()) m_after_remove_vertex(); }

  /*! Notification before the removal of an edge.
   * \param e A handle to one of the twin halfedges to be deleted.
   */
  virtual void before_remove_edge(Halfedge_handle e)
  { if (! m_before_remove_edge.is_none()) m_before_remove_edge(&*e); }

  /*! Notificaion after the removal of an edge. */
  virtual void after_remove_edge() {}

  /*! Notification before the removal of an outer CCB.
   * \param f The face that owns the outer CCB.
   * \param h A circulator representing the boundary of the component.
   */
  virtual void before_remove_outer_ccb(Face_handle f,
                                       Ccb_halfedge_circulator h)
  {
    if (! m_before_remove_outer_ccb.is_none())
      m_before_remove_outer_ccb(&*f, &*h);
  }

  /*! Notificaion after the removal of an outer CCB.
   * \param f The face that used to own the outer CCB.
   */
  virtual void after_remove_outer_ccb(Face_handle f)
  { if (! m_after_remove_outer_ccb.is_none()) m_after_remove_outer_ccb(&*f); }

  /*! Notification before the removal of an inner CCB.
   * \param f The face containing the inner CCB.
   * \param h A circulator representing the boundary of the component.
   */
  virtual void before_remove_inner_ccb(Face_handle f,
                                       Ccb_halfedge_circulator h)
  {
    if (! m_before_remove_inner_ccb.is_none())
      m_before_remove_inner_ccb(&*f, &*h);
  }

  /*! Notificaion after the removal of an inner CCB.
   * \param f The face that used to contain the inner CCB.
   */
  virtual void after_remove_inner_ccb(Face_handle f)
  { if (! m_after_remove_inner_ccb.is_none()) m_after_remove_inner_ccb(&*f); }

  /// @}

private:
  py::object m_before_assign;
  py::object m_after_assign;
  py::object m_before_clear;
  py::object m_after_clear;
  py::object m_before_global_change;
  py::object m_after_global_change;
  py::object m_before_attach;
  py::object m_after_attach;
  py::object m_before_detach;
  py::object m_after_detach;
  py::object m_before_create_vertex;
  py::object m_after_create_vertex;
  py::object m_before_create_boundary_vertex1;
  py::object m_before_create_boundary_vertex2;
  py::object m_after_create_boundary_vertex;
  py::object m_before_create_edge;
  py::object m_after_create_edge;
  py::object m_before_modify_vertex;
  py::object m_after_modify_vertex;
  py::object m_before_modify_edge;
  py::object m_after_modify_edge;
  py::object m_before_split_edge;
  py::object m_after_split_edge;
  py::object m_before_split_fictitious_edge;
  py::object m_after_split_fictitious_edge;
  py::object m_before_split_face;
  py::object m_after_split_face;
  py::object m_before_split_outer_ccb;
  py::object m_after_split_outer_ccb;
  py::object m_before_split_inner_ccb;
  py::object m_after_split_inner_ccb;
  py::object m_before_add_outer_ccb;
  py::object m_after_add_outer_ccb;
  py::object m_before_add_inner_ccb;
  py::object m_after_add_inner_ccb;
  py::object m_before_add_isolated_vertex;
  py::object m_after_add_isolated_vertex;
  py::object m_before_merge_edge;
  py::object m_after_merge_edge;
  py::object m_before_merge_fictitious_edge;
  py::object m_after_merge_fictitious_edge;
  py::object m_before_merge_face;
  py::object m_after_merge_face;
  py::object m_before_merge_outer_ccb;
  py::object m_after_merge_outer_ccb;
  py::object m_before_merge_inner_ccb;
  py::object m_after_merge_inner_ccb;
  py::object m_before_move_outer_ccb;
  py::object m_after_move_outer_ccb;
  py::object m_before_move_inner_ccb;
  py::object m_after_move_inner_ccb;
  py::object m_before_move_isolated_vertex;
  py::object m_after_move_isolated_vertex;
  py::object m_before_remove_vertex;
  py::object m_after_remove_vertex;
  py::object m_before_remove_edge;
  py::object m_after_remove_edge;
  py::object m_before_remove_outer_ccb;
  py::object m_after_remove_outer_ccb;
  py::object m_before_remove_inner_ccb;
  py::object m_after_remove_inner_ccb;
};

#endif
