// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_ARR_OVERLAY_FUNCTION_TRAITS_HPP
#define CGALPY_ARR_OVERLAY_FUNCTION_TRAITS_HPP

#include <nanobind/nanobind.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/function.h>

#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/Surface_sweep_2/Arr_default_overlay_traits_base.h>

#include "CGALPY/config.hpp"

#if 1
// Fall through; T::data() does not exist
template <typename A> py::object data_a(...) { return py::none(); }

// T::data() exists
template <typename A, typename = decltype(std::declval<A>().data())>
const py::object& data_a(const A* a) { return a->data(); }

// Fall through; target does not exist
template <typename Fnc, typename A, typename B, typename R>
void apply(Fnc fnc, ...) {}

// Target does exsist
template <typename Fnc, typename A, typename B, typename R,
          typename = decltype(std::declval<R>().set_data(std::declval<typename R::Data>()))>
void apply(Fnc fnc, const A* a, const B* b, R* r) {
  if (fnc.is_none()) return;
  r->set_data(fnc(data_a<A>(a), data_a<B>(b)));
}

#else
#include "CGALPY/if_.hpp"

// First operand (A) does exist; second operand (B) does not exsist
template <typename Fnc, typename A, typename B, typename R, typename = void>
struct ApplyAB {
  void operator()(Fnc fnc, const A* a, const B* b, R* r) {
    r->set_data(fnc(a->data(), py::object()));
  }
};

// First operand (A) does exist; second operand (B) does exsist
template <typename Fnc, typename A, typename B, typename R>
struct ApplyAB<Fnc, A, B, R,
               typename if_<false, decltype(std::declval<B>().data())>::type> {
  void operator()(Fnc fnc, const A* a, const B* b, R* r) {
    r->set_data(fnc(a->data(), b->data()));
  }
};

// First operand (A) does not exist; second operand (B) does not exsist
template <typename Fnc, typename A, typename B, typename R, typename = void>
struct ApplyB {
  void operator()(Fnc fnc, const A* a, const B* b, R* r) {
    r->set_data(fnc(py::object(), py::object()));
  }
};

// First operand (A) does not exist; second operand (B) does exsist
template <typename Fnc, typename A, typename B, typename R>
struct ApplyB<Fnc, A, B, R,
              typename if_<false, decltype(std::declval<B>().data())>::type> {
  void operator()(Fnc fnc, const A* a, const B* b, R* r) {
    r->set_data(fnc(py::object(), b->data()));
  }
};

// First operand (A) does not exsist
template <typename Fnc, typename A, typename B, typename R, typename = void>
struct ApplyA {
  void operator()(Fnc fnc, const A* a, const B* b, R* r) {
    ApplyB<Fnc, A, B, R>()(fnc, a, b, r);
  }
};

// First operand (A) does exsist
template <typename Fnc, typename A, typename B, typename R>
struct ApplyA<Fnc, A, B, R,
              typename if_<false, decltype(std::declval<A>().data())>::type> {
  void operator()(Fnc fnc, const A* a, const B* b, R* r) {
    ApplyAB<Fnc, A, B, R>()(fnc, a, b, r);
  }
};

// Fall through; target does not exist
template <typename Fnc, typename A, typename B, typename R> void apply(Fnc fnc, ...) {}

// Target does exist
template <typename Fnc, typename A, typename B, typename R,
          typename = decltype(std::declval<R>().set_data(std::declval<typename R::Data>()))>
void apply(Fnc fnc, const A* a, const B* b, R* r) {
  ApplyA<Fnc, A, B, R>()(fnc, a, b, r);
}

#endif

/*! \class
 *
 * An overlay-traits class for computing the overlay of two arrangement whose
 * all records are extended with auxiliary data fields, of type data_type.
 * The resulting arrangement is also assumed to be
 * templated with the extended DCEL.
 * The resulting data object that corresponds to the overlay of two data
 * object of type data_type is computed using the corresponding python functor
 */
namespace py = nanobind;

template <typename ArrangementA, typename ArrangementB, typename ArrangementR,
          typename Data>
class Arr_overlay_function_traits :
  public CGAL::_Arr_default_overlay_traits_base<ArrangementA, ArrangementB,
                                                ArrangementR>
{
private:
  using Arrangement_a = ArrangementA;
  using Arrangement_b = ArrangementB;
  using Arrangement_r = ArrangementR;

  using Vertex_a = typename Arrangement_a::Vertex;
  using Vertex_b = typename Arrangement_b::Vertex;
  using Vertex_r = typename Arrangement_r::Vertex;

  using Halfedge_a = typename Arrangement_a::Halfedge;
  using Halfedge_b = typename Arrangement_b::Halfedge;
  using Halfedge_r = typename Arrangement_r::Halfedge;

  using Face_a = typename Arrangement_a::Face;
  using Face_b = typename Arrangement_b::Face;
  using Face_r = typename Arrangement_r::Face;

public:
  using Vertex_handle_a = typename Arrangement_a::Vertex_const_handle;
  using Vertex_handle_b = typename Arrangement_b::Vertex_const_handle;
  using Vertex_handle_r = typename Arrangement_r::Vertex_handle;

  using Halfedge_handle_a = typename Arrangement_a::Halfedge_const_handle;
  using Halfedge_handle_b = typename Arrangement_b::Halfedge_const_handle;
  using Halfedge_handle_r = typename Arrangement_r::Halfedge_handle;

  using Face_handle_a = typename Arrangement_a::Face_const_handle;
  using Face_handle_b = typename Arrangement_b::Face_const_handle;
  using Face_handle_r = typename Arrangement_r::Face_handle;

private:
  typedef CGAL::_Arr_default_overlay_traits_base<Arrangement_a, Arrangement_b,
                                                 Arrangement_r>
    Arr_default_overlay_traits;

  py::object m_vv_v;
  py::object m_ve_v;
  py::object m_vf_v;
  py::object m_ev_v;
  py::object m_fv_v;
  py::object m_ee_v;
  py::object m_ee_e;
  py::object m_ef_e;
  py::object m_fe_e;
  py::object m_ff_f;

public:
  /// Constructors
  /// @{

  // Default constructor
  Arr_overlay_function_traits() :
    Arr_default_overlay_traits(),
    m_vv_v(py::none()),
    m_ve_v(py::none()),
    m_vf_v(py::none()),
    m_ev_v(py::none()),
    m_fv_v(py::none()),
    m_ee_v(py::none()),
    m_ee_e(py::none()),
    m_ef_e(py::none()),
    m_fe_e(py::none()),
    m_ff_f(py::none())
  {}

  // Constructor with one operator
  Arr_overlay_function_traits(py::object py_function) :
    Arr_default_overlay_traits(),
    m_vv_v(py::none()),
    m_ve_v(py::none()),
    m_vf_v(py::none()),
    m_ev_v(py::none()),
    m_fv_v(py::none()),
    m_ee_v(py::none()),
    m_ee_e(py::none()),
    m_ef_e(py::none()),
    m_fe_e(py::none()),
    m_ff_f(py_function)
  {}

  // Constructor with all operators
  Arr_overlay_function_traits(py::object py_function0, py::object py_function1,
                              py::object py_function2, py::object py_function3,
                              py::object py_function4, py::object py_function5,
                              py::object py_function6, py::object py_function7,
                              py::object py_function8, py::object py_function9) :
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

  // Destruct
  ~Arr_overlay_function_traits() {}

  /// @}

  /// Setters
  /// @{

  void set_vv_v(py::object vv_v) { m_vv_v = vv_v; }
  void set_ve_v(py::object ve_v) { m_ve_v = ve_v; }
  void set_vf_v(py::object vf_v) { m_vf_v = vf_v; }
  void set_ev_v(py::object ev_v) { m_ev_v = ev_v; }
  void set_fv_v(py::object fv_v) { m_fv_v = fv_v; }
  void set_ee_v(py::object ee_v) { m_ee_v = ee_v; }
  void set_ee_e(py::object ee_e) { m_ee_e = ee_e; }
  void set_ef_e(py::object ef_e) { m_ef_e = ef_e; }
  void set_fe_e(py::object fe_e) { m_fe_e = fe_e; }
  void set_ff_f(py::object ff_f) { m_ff_f = ff_f; }

  /// @}

  /// Creators
  /// @{

  /*! Create the vertex v induced by the coinciding vertices v1 and v2.
   */
  void create_vertex(Vertex_handle_a v1, Vertex_handle_b v2, Vertex_handle_r v)
    const
  { apply<py::object, Vertex_a, Vertex_b, Vertex_r>(m_vv_v, &*v1, &*v2, &*v); }

  /*! Create the vertex v induced by the vertex v1 that lies on the halfedge e2.
   */
  void create_vertex(Vertex_handle_a v1, Halfedge_handle_b e2, Vertex_handle_r v)
    const
  { apply<py::object, Vertex_a, Halfedge_b, Vertex_r>(m_ve_v, &*v1, &*e2, &*v); }

  /*! Create the vertex v induced by the vertex v1 that lies inside the face f2.
   */
  void create_vertex(Vertex_handle_a v1, Face_handle_b f2, Vertex_handle_r v)
    const
  { apply<py::object, Vertex_a, Face_b, Vertex_r>(m_vf_v, &*v1, &*f2, &*v); }

  /* Create the vertex v induced by the vertex v2 that lies on the halfedge e1.
   */
  void create_vertex(Halfedge_handle_a e1, Vertex_handle_b v2,
                     Vertex_handle_r v) const
  { apply<py::object, Halfedge_a, Vertex_b, Vertex_r>(m_ev_v, &*e1, &*v2, &*v); }

  /* Create the vertex v induced by the vertex v2 that lies inside the face f1.
   */
  void create_vertex(Face_handle_a f1, Vertex_handle_b v2, Vertex_handle_r v)
    const
  { apply<py::object, Face_a, Vertex_b, Vertex_r>(m_fv_v, &*f1, &*v2, &*v); }

  /* Create the vertex v induced by the intersection of the halfedges e1 and e2.
   */
  void create_vertex(Halfedge_handle_a e1, Halfedge_handle_b e2,
                     Vertex_handle_r v) const
  { apply<py::object, Halfedge_a, Halfedge_b, Vertex_r>(m_ee_v, &*e1, &*e2, &*v); }

  /* Create the halfedge e induced by an overlap between the halfedges e1 and e2.
   */
  void create_edge(Halfedge_handle_a e1, Halfedge_handle_b e2,
                   Halfedge_handle_r e) const
  {
    apply<py::object, Halfedge_a, Halfedge_b, Halfedge_r>(m_ee_e, &*e1, &*e2, &*e);
  }

  /* Create the halfedge e induced by the halfedge e1 contained in the face f2.
   */
  void create_edge(Halfedge_handle_a e1, Face_handle_b f2, Halfedge_handle_r e)
    const
  { apply<py::object, Halfedge_a, Face_b, Halfedge_r>(m_ef_e, &*e1, &*f2, &*e); }

  /* Create the halfedge e induced by the halfedge e2 contained in the face f1.
   */
  void create_edge(Face_handle_a f1, Halfedge_handle_b e2, Halfedge_handle_r e)
    const
  { apply<py::object, Face_a, Halfedge_b, Halfedge_r>(m_fe_e, &*f1, &*e2, &*e); }

  /*! Create a face f that matches the overlapping region between f1 and f2.
   */
  void create_face(Face_handle_a f1, Face_handle_b f2, Face_handle_r f) const
  { apply<py::object, Face_a, Face_b, Face_r>(m_ff_f, &*f1, &*f2, &*f); }

  /// @}
};

#endif
