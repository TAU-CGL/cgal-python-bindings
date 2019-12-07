#include "config.hpp"
#if (CGALPY_DCEL == CGALPY_FACE_EXTENDED_DCEL) || \
  (CGALPY_DCEL == CGALPY_EXTENDED_DCEL)
#ifndef ARR_PYTHON_FACE_OVERLAY_TRAITS
#define ARR_PYTHON_FACE_OVERLAY_TRAITS

#include <boost/python.hpp>

#include <CGAL/Arr_default_dcel.h>
#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/Surface_sweep_2/Arr_default_overlay_traits_base.h>

#include "Python_functor.hpp"

/*! \class
 *
 * An overlay-traits class for computing the overlay of two arrangement whose
 * face records are extended with auxiliary data fields, of type data_type
 * The resulting arrangement is also assumed to be templated with the
 * face-extended DCEL, where each face stores an auxiliary data_type field.
 * The resulting data object that corresponds to the overlay of two data
 * object of type data_type is computed using the python functor
 * Overlay_face_data.
 */
namespace bp = boost::python;

template <typename ArrangementA, typename ArrangementB, typename ArrangementR,
          typename data_type>
class Arr_python_face_overlay_traits :
  public CGAL::_Arr_default_overlay_traits_base<ArrangementA, ArrangementB,
                                                ArrangementR>
{
public:
  typedef typename ArrangementA::Face_const_handle    Face_handle_A;
  typedef typename ArrangementB::Face_const_handle    Face_handle_B;
  typedef typename ArrangementR::Face_handle          Face_handle_R;

  typedef  Python_functor_2<data_type, data_type, data_type>   Overlay_face_data;

private:
  typedef CGAL::_Arr_default_overlay_traits_base<ArrangementA, ArrangementB,
    ArrangementR>           Arr_default_overlay_traits;

  Overlay_face_data         overlay_face_data;

public:
  Arr_python_face_overlay_traits(bp::object py_functor) :
    Arr_default_overlay_traits()
  {
    overlay_face_data = Overlay_face_data(py_functor);
  }


  /*! Create a face f that matches the overlapping region between f1 and f2.
   */
  virtual void create_face(Face_handle_A f1, Face_handle_B f2,
    Face_handle_R f)
  {
    // Overlay the data objects associated with f1 and f2 and store the result
    // with f.
    f->set_data(overlay_face_data(f1->data(), f2->data()));
    return;
  }
};

#endif //ARR_PYTHON_FACE_OVERLAY_TRAITS
#endif
