//#define BOOST_PYTHON_STATIC_LIB
#define CGAL_HEADER_ONLY 1
#include <boost/python.hpp>
#include <boost/make_shared.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/stl_iterator.hpp>
#include <CGAL/Arrangement_2.h>
//#include <CGAL/Arr_extended_dcel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Arr_segment_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel   Kernel;
typedef Kernel::FT                                          FT;
typedef Kernel::Point_2                                     Point_2;
typedef Kernel::Segment_2                                   Segment_2;
typedef Kernel::Line_2                                      Line_2;
typedef Kernel::Ray_2                                       Ray_2;
typedef CGAL::Arr_segment_traits_2<Kernel>                  Traits;
typedef Traits::Curve_2                                     Curve;
typedef CGAL::Arrangement_2<Traits>                         Arrangement_2;
typedef Kernel::Intersect_2                                 Intersect_2;

typedef CGAL::cpp11::result_of<Intersect_2(Line_2, Line_2)>::type Line_line_intersection_result;
typedef CGAL::cpp11::result_of<Intersect_2(Line_2, Ray_2)>::type Line_ray_intersection_result;
typedef CGAL::cpp11::result_of<Intersect_2(Line_2, Segment_2)>::type Line_segment_intersection_result;
typedef CGAL::cpp11::result_of<Intersect_2(Ray_2, Ray_2)>::type Ray_ray_intersection_result;
typedef CGAL::cpp11::result_of<Intersect_2(Ray_2, Segment_2)>::type Ray_segment_intersection_result;
typedef CGAL::cpp11::result_of<Intersect_2(Segment_2, Segment_2)>::type Segment_segment_intersection_result;

typedef Arrangement_2::Vertex_iterator							            Vertex_iterator;
typedef Arrangement_2::Vertex									                  Vertex;
typedef Arrangement_2::Inner_ccb_iterator						            Inner_ccb_iterator;
typedef Arrangement_2::Halfedge_iterator						            Halfedge_iterator;
typedef Arrangement_2::Ccb_halfedge_circulator					        Ccb_halfedge_circulator;
typedef Arrangement_2::Halfedge_around_vertex_circulator		    Halfedge_around_vertex_circulator;
typedef Arrangement_2::Halfedge									                Halfedge;
typedef Arrangement_2::Edge_iterator							              Edge_iterator;
typedef Arrangement_2::Face_iterator							              Face_iterator;
typedef Arrangement_2::Face										                  Face;