//#define BOOST_PYTHON_STATIC_LIB
#define CGAL_HEADER_ONLY 1
//#define INEXACT_KERNEL 1
//#define EXTENDED_DCEL 1
//#define ARR_LINEAR_TRAITS 1
//#define ARR_SEGMENT_TRAITS 1
//#define ARR_NON_CACHING_SEGMENT_TRAITS 1
//#define ARR_CONIC_TRAITS 1
#define ARR_ALGEBRAIC_SEGMENT_TRAITS 1
//#define ARR_CIRCLE_SEGMENT_TRAITS 1
#include <boost/python.hpp>
#include <boost/make_shared.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/stl_iterator.hpp>
#ifdef INEXACT_KERNEL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#else
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#endif // INEXACT_KERNEL
#include <CGAL/CORE_BigInt.h>
#include <CGAL/Sqrt_extension.h>
#ifdef ARR_LINEAR_TRAITS
#include <CGAL/Arr_linear_traits_2.h>
#endif
#ifdef ARR_SEGMENT_TRAITS
#include <CGAL/Arr_segment_traits_2.h>
#endif
#ifdef ARR_NON_CACHING_SEGMENT_TRAITS
#include <CGAL/Arr_non_caching_segment_traits_2.h>
#endif
#ifdef ARR_CIRCLE_SEGMENT_TRAITS
#include <CGAL/Arr_circle_segment_traits_2.h>
#endif
#ifdef ARR_CONIC_TRAITS
#include <CGAL/Arr_conic_traits_2.h>
#endif
#ifdef ARR_ALGEBRAIC_SEGMENT_TRAITS
#include <CGAL/Arr_algebraic_segment_traits_2.h>
#include <CGAL/Polynomial.h>
#include <CGAL/Polynomial_traits_d.h>
#include <CGAL/Polynomial_type_generator.h>
#endif
#include <CGAL/Bbox_2.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Direction_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Aff_transformation_2.h>
#include <CGAL/Arrangement_2.h>
#ifdef EXTENDED_DCEL
#include <CGAL/Arr_extended_dcel.h>
#endif // EXTENDED_DCEL
#include <CGAL/Arr_overlay_2.h>
#include <CGAL/Arr_default_overlay_traits.h>
#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>

using namespace boost::python;
namespace bp = boost::python;

#ifdef INEXACT_KERNEL
typedef typename CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;
typedef typename bp::return_value_policy<bp::copy_const_reference>       Kernel_return_value_policy;
#else
typedef typename CGAL::Exact_predicates_exact_constructions_kernel       Kernel;
typedef typename bp::return_value_policy<bp::return_by_value>            Kernel_return_value_policy;
#endif

typedef typename CORE::BigInt                                            BigInt;
typedef typename CGAL::Gmpz                                              Gmpz;
typedef typename CGAL::Gmpq                                              Gmpq;
typedef typename Kernel::FT                                              FT;
//typedef typename CGAL::Sqrt_extension <FT, FT>                           CoordNT;
typedef typename Kernel::RT                                              RT;
typedef typename Kernel::Point_2                                         Point_2;
typedef typename Kernel::Segment_2                                       Segment_2;
typedef typename Kernel::Line_2                                          Line_2;
typedef typename Kernel::Ray_2                                           Ray_2;
typedef typename CGAL::Bbox_2                                            Bbox_2;
typedef typename Kernel::Direction_2                                     Direction_2;
typedef typename Kernel::Vector_2                                        Vector_2;
typedef typename Kernel::Circle_2                                        Circle_2;
typedef typename Kernel::Triangle_2                                      Triangle_2;
typedef typename Kernel::Iso_rectangle_2                                 Iso_rectangle_2;
typedef typename CGAL::Aff_transformation_2<Kernel>                      Aff_Transformation_2;
typedef typename CGAL::Rotation                                          Rotation;
typedef typename CGAL::Scaling                                           Scaling;
typedef typename CGAL::Translation                                       Translation;
#ifdef ARR_SEGMENT_TRAITS
typedef typename CGAL::Arr_segment_traits_2<Kernel>                      Traits;
# endif // ARR_SEGMENT_TRAITS
#ifdef ARR_NON_CACHING_SEGMENT_TRAITS
typedef typename CGAL::Arr_non_caching_segment_traits_2<Kernel>          Traits;
# endif // ARR_NON_CHACHING_SEGMENT_TRAITS
#ifdef ARR_LINEAR_TRAITS
typedef typename CGAL::Arr_linear_traits_2<Kernel>                       Traits;
#endif // ARR_LINEAR_TRAITS
#ifdef ARR_CIRCLE_SEGMENT_TRAITS
typedef typename CGAL::Arr_circle_segment_traits_2<Kernel>               Traits;
typedef typename Traits::CoordNT                                         CoordNT;
#endif // ARR_CIRCLE_SEGMENT_TRAITS
#ifdef ARR_ALGEBRAIC_SEGMENT_TRAITS
typedef typename BigInt                                                  Number;
typedef typename CGAL::Arr_algebraic_segment_traits_2<Number>            Traits;
typedef typename Traits::Construct_curve_2                               Construct_curve_2;
typedef typename Traits::Construct_point_2                               Construct_point_2;
typedef typename Traits::Construct_x_monotone_segment_2                  Construct_x_monotone_segment_2;
typedef typename Traits::Polynomial_2                                    Polynomial_2;
typedef CGAL::Polynomial_traits_d<Polynomial_2>                          PT_2;
typedef PT_2::Construct_polynomial                                       Construct_polynomial_2;
typedef PT_2::Coefficient_type                                           Polynomial_1;
typedef CGAL::Polynomial_traits_d<Polynomial_1>                          PT_1;
typedef PT_1::Construct_polynomial                                       Construct_polynomial_1;
typedef typename Traits::Algebraic_kernel_d_1                            Algebraic_kernel_d_1;
typedef typename Algebraic_kernel_d_1::Polynomial_1                      Polynomial_1;
typedef typename Traits::Algebraic_real_1                                Algebraic_real_1;
typedef typename Traits::Bound                                           Bound;
#endif // ARR_ALGEBRAIC_SEMGNET_TRAITS
typedef typename Traits::Point_2                                         TPoint_2;
typedef typename Traits::Curve_2                                         Curve_2;
typedef typename Traits::X_monotone_curve_2                              X_monotone_curve_2;
#ifdef EXTENDED_DCEL
typedef CGAL::Arr_extended_dcel<Traits, int, int, int>                   Dcel;
typedef CGAL::Arrangement_2<Traits, Dcel>                                Arrangement_2;
#else
typedef typename CGAL::Arrangement_2<Traits>                             Arrangement_2;
#endif // EXTENDED_DCEL
typedef typename Kernel::Intersect_2                                     Intersect_2;
typedef typename CGAL::Arr_naive_point_location<Arrangement_2>           Naive_pl;
typedef typename CGAL::Arr_walk_along_line_point_location<Arrangement_2> Wal_pl;
typedef typename CGAL::Arr_landmarks_point_location<Arrangement_2>       Landmarks_pl;
typedef typename CGAL::Arr_trapezoid_ric_point_location<Arrangement_2>   Trapezoid_pl;
//typedef typename CGAL::Arr_point_location_result<Arrangement_2>          Pl_result;
//typedef typename std::pair<Point_2, Pl_result::Type>                     Pl_query_result;


typedef typename CGAL::cpp11::result_of<Intersect_2(Point_2, Iso_rectangle_2)>::type
Point_iso_rectangle_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Point_2, Line_2)>::type
Point_line_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Point_2, Ray_2)>::type
Point_ray_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Point_2, Segment_2)>::type
Point_segment_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Point_2, Triangle_2)>::type
Point_triangle_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Iso_rectangle_2, Iso_rectangle_2)>::type
Iso_rectangle_iso_rectangle_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Iso_rectangle_2, Line_2)>::type
Iso_rectangle_line_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Iso_rectangle_2, Ray_2)>::type
Iso_rectangle_ray_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Iso_rectangle_2, Segment_2)>::type
Iso_rectangle_segment_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Iso_rectangle_2, Triangle_2)>::type
Iso_rectangle_triangle_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Line_2, Line_2)>::type
Line_line_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Line_2, Ray_2)>::type
Line_ray_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Line_2, Segment_2)>::type
Line_segment_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Line_2, Triangle_2)>::type
Line_triangle_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Ray_2, Ray_2)>::type
Ray_ray_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Ray_2, Segment_2)>::type
Ray_segment_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Ray_2, Triangle_2)>::type
Ray_triangle_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type
Segment_segment_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Segment_2, Triangle_2)>::type
Segment_triangle_intersection_result;
typedef typename CGAL::cpp11::result_of<Intersect_2(Triangle_2, Triangle_2)>::type
Triangle_triangle_intersection_result;

typedef typename CGAL::Arr_point_location_result<Arrangement_2>::Type
Pl_result;

typedef typename Arrangement_2::Vertex_iterator							              Vertex_iterator;
typedef typename Arrangement_2::Vertex_const_handle                       Vertex_const_handle;
typedef typename Arrangement_2::Isolated_vertex_iterator                  Isolated_vertex_iterator;
typedef typename Arrangement_2::Vertex									                  Vertex;
typedef typename Arrangement_2::Inner_ccb_iterator						            Inner_ccb_iterator;
typedef typename Arrangement_2::Ccb_halfedge_circulator					          Ccb_halfedge_circulator;
typedef typename Arrangement_2::Halfedge_around_vertex_circulator		      Halfedge_around_vertex_circulator;
typedef typename Arrangement_2::Halfedge_iterator									        Halfedge_iterator;
typedef typename Arrangement_2::Halfedge_const_handle                     Halfedge_const_handle;
typedef typename Arrangement_2::Halfedge									                Halfedge;
//typedef typename Arrangement_2::Edge_iterator							                Edge_iterator;
typedef typename Arrangement_2::Face_iterator							                Face_iterator;
typedef typename Arrangement_2::Face_const_handle                         Face_const_handle;
typedef typename Arrangement_2::Face										                  Face;

inline boost::python::object pass_through(boost::python::object const& o) { return o; }

//these template classes are used to allow more natural iteration in python

template <typename circulator>
class Iterator_from_circulator
{
private:
  bool first = true;
  circulator m_first;
  circulator m_curr;

public:
  Iterator_from_circulator(circulator first) : m_first(first), m_curr(first) {}
  typename circulator::value_type& next()
  {
    if (m_curr != 0)
    {
      if (first || m_curr != m_first)
      {
        first = false;
        return *(m_curr++);
      }
    }
    PyErr_SetString(PyExc_StopIteration, "No more data.");
    bp::throw_error_already_set();
    return *m_curr;
  }
};

template <typename iterator>
class Iterator_of_circulators
{
  typedef Iterator_from_circulator<typename iterator::value_type> modified_circulator;
  iterator m_curr;
  iterator m_end;
public:
  Iterator_of_circulators(iterator begin, iterator end) : m_curr(begin), m_end(end) {}
  modified_circulator* next()
  {
    if (m_curr != m_end)
      return new modified_circulator(modified_circulator(*(m_curr++)));
    PyErr_SetString(PyExc_StopIteration, "No more data.");
    boost::python::throw_error_already_set();
    return new modified_circulator(modified_circulator(*m_curr));
  }
};

template<typename iterator>
void bind_iterator_of_circulators(const char* python_name)
{
  using namespace boost::python;
  class_<iterator>(python_name, no_init)
    .def("__iter__", &pass_through)
    .def("__next__", &iterator::next, return_value_policy<manage_new_object>())
    ;
}

template<typename iterator>
void bind_iterator(const char* python_name)
{
  using namespace boost::python;
  class_<iterator>(python_name, no_init)
    .def("__iter__", &pass_through)
    .def("__next__", &iterator::next, return_value_policy<reference_existing_object>())
    ;
}
