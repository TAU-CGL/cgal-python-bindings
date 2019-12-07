#include "config.hpp"
#if CGALPY_DCEL == CGALPY_EXTENDED_DCEL
#ifndef ARR_PYTHON_OVERLAY_TRAITS
#define ARR_PYTHON_OVERLAY_TRAITS

#include <CGAL/Arr_default_dcel.h>
#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/Surface_sweep_2/Arr_default_overlay_traits_base.h>
#include <Python_functor.hpp>
#include <boost/python.hpp>
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
          typename data_type>
class Arr_python_overlay_traits :
  public CGAL::_Arr_default_overlay_traits_base<ArrangementA, ArrangementB,
                                                ArrangementR>
{
public:
  typedef typename ArrangementA::Vertex_const_handle    Vertex_handle_A;
  typedef typename ArrangementB::Vertex_const_handle    Vertex_handle_B;
  typedef typename ArrangementR::Vertex_handle          Vertex_handle_R;

  typedef typename ArrangementA::Halfedge_const_handle  Halfedge_handle_A;
  typedef typename ArrangementB::Halfedge_const_handle  Halfedge_handle_B;
  typedef typename ArrangementR::Halfedge_handle        Halfedge_handle_R;

  typedef typename ArrangementA::Face_const_handle      Face_handle_A;
  typedef typename ArrangementB::Face_const_handle      Face_handle_B;
  typedef typename ArrangementR::Face_handle            Face_handle_R;

  typedef Python_functor_2<data_type, data_type, data_type>    Overlay_data;

private:
  typedef CGAL::_Arr_default_overlay_traits_base<ArrangementA, ArrangementB,
                                                 ArrangementR>
    Arr_default_overlay_traits;

  Overlay_data overlay_vertex_data0;
  Overlay_data overlay_vertex_data1;
  Overlay_data overlay_vertex_data2;
  Overlay_data overlay_vertex_data3;
  Overlay_data overlay_vertex_data4;
  Overlay_data overlay_vertex_data5;
  Overlay_data overlay_edge_data0;
  Overlay_data overlay_edge_data1;
  Overlay_data overlay_edge_data2;
  Overlay_data overlay_face_data;

public:
  Arr_python_overlay_traits(bp::object py_functor0, bp::object py_functor1,
                            bp::object py_functor2, bp::object py_functor3,
                            bp::object py_functor4, bp::object py_functor5,
                            bp::object py_functor6, bp::object py_functor7,
                            bp::object py_functor8, bp::object py_functor9) :
    Arr_default_overlay_traits()
  {
    overlay_vertex_data0 = Overlay_data(py_functor0);
    overlay_vertex_data1 = Overlay_data(py_functor1);
    overlay_vertex_data2 = Overlay_data(py_functor2);
    overlay_vertex_data3 = Overlay_data(py_functor3);
    overlay_vertex_data4 = Overlay_data(py_functor4);
    overlay_vertex_data5 = Overlay_data(py_functor5);
    overlay_edge_data0 = Overlay_data(py_functor6);
    overlay_edge_data1 = Overlay_data(py_functor7);
    overlay_edge_data2 = Overlay_data(py_functor8);
    overlay_face_data = Overlay_data(py_functor9);
  }

  /*!constructs the vertex v induced by the coinciding vertices v1 and v2.
   */
  virtual void create_vertex(Vertex_handle_A v1, Vertex_handle_B v2, Vertex_handle_R v)
  {
    v->set_data(overlay_vertex_data0(v1->data(), v2->data()));
  }

  /*!constructs the vertex v induced by the vertex v1 that lies on the halfedge e2.
 */
  virtual void create_vertex(Vertex_handle_A v1, Halfedge_handle_B e2, Vertex_handle_R v)
  {
    v->set_data(overlay_vertex_data1(v1->data(), e2->data()));
  }

  /*!constructs the vertex v induced by the vertex v1 that lies inside the face f2.
*/
  virtual void create_vertex(Vertex_handle_A v1, Face_handle_B f2, Vertex_handle_R v)
  {
    v->set_data(overlay_vertex_data2(v1->data(), f2->data()));
  }

  /*constructs the vertex v induced by the vertex v2 that lies on the halfedge e1.
*/
  virtual void create_vertex(Halfedge_handle_A e1, Vertex_handle_B v2, Vertex_handle_R v)
  {
    v->set_data(overlay_vertex_data3(e1->data(), v2->data()));
  }

  /*constructs the vertex v induced by the vertex v2 that lies inside the face f1.
*/
  virtual void create_vertex(Face_handle_A f1, Vertex_handle_B v2, Vertex_handle_R v)
  {
    v->set_data(overlay_vertex_data4(f1->data(), v2->data()));
  }

  /*constructs the vertex v induced by the intersection of the halfedges e1 and e2.
*/
  virtual void create_vertex(Halfedge_handle_A e1, Halfedge_handle_B e2, Vertex_handle_R v)
  {
    v->set_data(overlay_vertex_data5(e1->data(), e2->data()));
  }

  /*constructs the halfedge e induced by an overlap between the halfedges e1 and e2.
  */
  virtual void create_edge(Halfedge_handle_A e1, Halfedge_handle_B e2, Halfedge_handle_R e)
  {
    e->set_data(overlay_edge_data0(e1->data(), e2->data()));
  }

  /*constructs the halfedge e induced by the halfedge e1 that lies inside the face f2.
*/
  virtual void create_edge(Halfedge_handle_A e1, Face_handle_B f2, Halfedge_handle_R e)
  {
    e->set_data(overlay_edge_data1(e1->data(), f2->data()));
  }

  /*constructs the halfedge e induced by the halfedge e2 that lies inside the face f1.
*/
  virtual void create_edge(Face_handle_A f1, Halfedge_handle_B e2, Halfedge_handle_R e)
  {
    e->set_data(overlay_edge_data2(f1->data(), e2->data()));
  }

  /*! Create a face f that matches the overlapping region between f1 and f2.
   */
  virtual void create_face(Face_handle_A f1, Face_handle_B f2, Face_handle_R f)
  {
    // Overlay the data objects associated with f1 and f2 and store the result
    // with f.
    f->set_data(overlay_face_data(f1->data(), f2->data()));
    return;
  }
};

#endif //ARR_PYTHON_OVERLAY_TRAITS
#endif
