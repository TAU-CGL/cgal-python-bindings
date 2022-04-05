// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARR_OVERLAY_TRAITS_HPP
#define CGALPY_ARR_OVERLAY_TRAITS_HPP

#include <boost/python.hpp>

#include <CGAL/Arr_default_dcel.h>
#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/Surface_sweep_2/Arr_default_overlay_traits_base.h>

#include "CGALPY/config.hpp"
#include "CGALPY/Python_functor.hpp"

/*! \class
 *
 * An overlay-traits class for computing the overlay of two arrangement whose
 * all records are extended with auxiliary data fields, of type data_type.
 * The resulting arrangement is also assumed to be
 * templated with the extended DCEL.
 * The resulting data object that corresponds to the overlay of two data
 * object of type data_type is computed using the corresponding python functor
 */
namespace bp = boost::python;

template <typename ArrangementA, typename ArrangementB, typename ArrangementR,
          typename Data>
class Arr_overlay_traits :
  public CGAL::_Arr_default_overlay_traits_base<ArrangementA, ArrangementB,
                                                ArrangementR>
{
private:
  typedef ArrangementA                          Arrangement_a;
  typedef ArrangementB                          Arrangement_b;
  typedef ArrangementR                          Arrangement_r;

  typedef typename Arrangement_a::Vertex        Vertex_a;
  typedef typename Arrangement_b::Vertex        Vertex_b;
  typedef typename Arrangement_r::Vertex        Vertex_r;

  typedef typename Arrangement_a::Halfedge      Halfedge_a;
  typedef typename Arrangement_b::Halfedge      Halfedge_b;
  typedef typename Arrangement_r::Halfedge      Halfedge_r;

  typedef typename Arrangement_a::Face          Face_a;
  typedef typename Arrangement_b::Face          Face_b;
  typedef typename Arrangement_r::Face          Face_r;

public:
  typedef typename Arrangement_a::Vertex_const_handle   Vertex_handle_a;
  typedef typename Arrangement_b::Vertex_const_handle   Vertex_handle_b;
  typedef typename Arrangement_r::Vertex_handle         Vertex_handle_r;

  typedef typename Arrangement_a::Halfedge_const_handle Halfedge_handle_a;
  typedef typename Arrangement_b::Halfedge_const_handle Halfedge_handle_b;
  typedef typename Arrangement_r::Halfedge_handle       Halfedge_handle_r;

  typedef typename Arrangement_a::Face_const_handle     Face_handle_a;
  typedef typename Arrangement_b::Face_const_handle     Face_handle_b;
  typedef typename Arrangement_r::Face_handle           Face_handle_r;

  typedef Python_functor_3_ref<Vertex_a, Vertex_b, Vertex_r>       Function_vv_v;
  typedef Python_functor_3_ref<Vertex_a, Halfedge_b, Vertex_r>     Function_ve_v;
  typedef Python_functor_3_ref<Vertex_a, Face_b, Vertex_r>         Function_vf_v;
  typedef Python_functor_3_ref<Halfedge_a, Vertex_b, Vertex_r>     Function_ev_v;
  typedef Python_functor_3_ref<Face_a, Vertex_b, Vertex_r>         Function_fv_v;
  typedef Python_functor_3_ref<Halfedge_a, Halfedge_b, Vertex_r>   Function_ee_v;
  typedef Python_functor_3_ref<Halfedge_a, Halfedge_b, Halfedge_r> Function_ee_e;
  typedef Python_functor_3_ref<Halfedge_a, Face_b, Halfedge_r>     Function_ef_e;
  typedef Python_functor_3_ref<Face_a, Halfedge_b, Halfedge_r>     Function_fe_e;
  typedef Python_functor_3_ref<Face_a, Face_b, Face_r>             Function_ff_f;

private:
  typedef CGAL::_Arr_default_overlay_traits_base<Arrangement_a, Arrangement_b,
                                                 Arrangement_r>
    Arr_default_overlay_traits;

  Function_vv_v m_vv_v;
  Function_ve_v m_ve_v;
  Function_vf_v m_vf_v;
  Function_ev_v m_ev_v;
  Function_fv_v m_fv_v;
  Function_ee_v m_ee_v;
  Function_ee_e m_ee_e;
  Function_ef_e m_ef_e;
  Function_fe_e m_fe_e;
  Function_ff_f m_ff_f;

public:
  /// Constructors
  /// @{

  // Default constructor
  Arr_overlay_traits() : Arr_default_overlay_traits() {}

  // Constructor with one operator
  Arr_overlay_traits(bp::object py_function) :
    Arr_default_overlay_traits(),
    m_ff_f(py_function)
  {}

  // Constructor with all operators
  Arr_overlay_traits(bp::object py_function0, bp::object py_function1,
                     bp::object py_function2, bp::object py_function3,
                     bp::object py_function4, bp::object py_function5,
                     bp::object py_function6, bp::object py_function7,
                     bp::object py_function8, bp::object py_function9) :
    Arr_default_overlay_traits(),
    m_vv_v(py_function0),
    m_ve_v(py_function1),
    m_vf_v(py_function2),
    m_ev_v(py_function3),
    m_fv_v(py_function4),
    m_ee_v(py_function5),
    m_ee_e(py_function6),
    m_ef_e(py_function7),
    m_fe_e(py_function8),
    m_ff_f(py_function9)
    {}

  /// @}

  /// Setters
  /// @{

  void set_vv_v(bp::object vv_v) { m_vv_v = vv_v; }
  void set_ve_v(bp::object ve_v) { m_ve_v = ve_v; }
  void set_vf_v(bp::object vf_v) { m_vf_v = vf_v; }
  void set_ev_v(bp::object ev_v) { m_ev_v = ev_v; }
  void set_fv_v(bp::object fv_v) { m_fv_v = fv_v; }
  void set_ee_v(bp::object ee_v) { m_ee_v = ee_v; }
  void set_ee_e(bp::object ee_e) { m_ee_e = ee_e; }
  void set_ef_e(bp::object ef_e) { m_ef_e = ef_e; }
  void set_fe_e(bp::object fe_e) { m_fe_e = fe_e; }
  void set_ff_f(bp::object ff_f) { m_ff_f = ff_f; }

  /// @}

  /// Creators
  /// @{

  /*! Create the vertex v induced by the coinciding vertices v1 and v2.
   */
  void create_vertex(Vertex_handle_a v1, Vertex_handle_b v2, Vertex_handle_r v)
    const
  { m_vv_v(*v1, *v2, *v); }

  /*! Create the vertex v induced by the vertex v1 that lies on the halfedge e2.
   */
  void create_vertex(Vertex_handle_a v1, Halfedge_handle_b e2, Vertex_handle_r v)
    const
  { m_ve_v(*v1, *e2, *v); }

  /*! Create the vertex v induced by the vertex v1 that lies inside the face f2.
   */
  void create_vertex(Vertex_handle_a v1, Face_handle_b f2, Vertex_handle_r v)
  { m_vf_v(*v1, *f2, *v); }

  /* Create the vertex v induced by the vertex v2 that lies on the halfedge e1.
   */
  void create_vertex(Halfedge_handle_a e1, Vertex_handle_b v2,
                     Vertex_handle_r v) const
  { m_ev_v(*e1, *v2, *v); }

  /* Create the vertex v induced by the vertex v2 that lies inside the face f1.
   */
  void create_vertex(Face_handle_a f1, Vertex_handle_b v2, Vertex_handle_r v)
    const
  { m_fv_v(*f1, *v2, *v); }

  /* Create the vertex v induced by the intersection of the halfedges e1 and e2.
   */
  void create_vertex(Halfedge_handle_a e1, Halfedge_handle_b e2,
                     Vertex_handle_r v) const
  { m_ee_v(*e1, *e2, *v); }

  /* Create the halfedge e induced by an overlap between the halfedges e1 and e2.
   */
  void create_edge(Halfedge_handle_a e1, Halfedge_handle_b e2,
                   Halfedge_handle_r e) const
  { m_ee_e(*e1, *e2, *e); }

  /* Create the halfedge e induced by the halfedge e1 contained the face f2.
   */
  void create_edge(Halfedge_handle_a e1, Face_handle_b f2, Halfedge_handle_r e)
    const
  { m_ef_e(*e1, *f2, *e); }

  /* Create the halfedge e induced by the halfedge e2 contained in the face f1.
   */
  void create_edge(Face_handle_a f1, Halfedge_handle_b e2, Halfedge_handle_r e)
    const
  { m_fe_e(*f1, *e2, *e); }

  /*! Create a face f that matches the overlapping region between f1 and f2.
   */
  void create_face(Face_handle_a f1, Face_handle_b f2, Face_handle_r f) const
  {
    m_ff_f(*f1, *f2, *f);
    // f->set_data(f1->data() + f2->data());
  }

  /// @}
};

#endif
