// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>

#include "CGALPY/arr_point_location_config.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace bp = boost::python;
typedef typename aos2::Arrangement_2 Arrangement_2;
typedef typename aos2::Face_const_handle Face_const_handle;
typedef typename aos2::Halfedge_const_handle Halfedge_const_handle;
typedef typename aos2::Vertex_const_handle Vertex_const_handle;

typedef typename CGAL::Arr_naive_point_location<Arrangement_2>         Naive_pl;
typedef typename CGAL::Arr_walk_along_line_point_location<Arrangement_2> Wal_pl;
typedef typename CGAL::Arr_landmarks_point_location<Arrangement_2> Landmarks_pl;
typedef typename CGAL::Arr_trapezoid_ric_point_location<Arrangement_2>
  Trapezoid_pl;

typedef typename CGAL::Arr_point_location_result<Arrangement_2>::Type Pl_result;
typedef typename std::pair<Point_2, Pl_result>                  Pl_query_result;

class Point_location_result_visitor : public boost::static_visitor<bp::object> {
public:
  template<typename T>
  bp::object operator()(T& operand) const {
    // reference existing object
    typename bp::reference_existing_object::apply<T*>::type converter;
    bp::handle<> handle(converter(&(*operand)));
    return bp::object(handle);
  }
};

bp::list locate_batch(Arrangement_2& arr, const bp::list& lst) {
  bp::list res;
  auto v = std::vector<Point_2>(bp::stl_input_iterator< Point_2 >(lst),
                                bp::stl_input_iterator< Point_2 >());
  auto op =
    [&] (const Pl_query_result& p) mutable {
      const auto& result =
        boost::apply_visitor(Point_location_result_visitor(), p.second);
      res.append(bp::make_tuple(p.first, result));
    };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  locate(arr, v.begin(), v.end(), it);
  return res;
}

template <typename PL>
bp::object locate(PL& pl, Point_2& p) {
  auto result = pl.locate(p);
  return boost::apply_visitor(Point_location_result_visitor(), result);
}

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
void landmarks_pl_attach(Landmarks_pl& pl, Arrangement_2& arr)
{ pl.attach(arr); }
#endif

void export_point_location() {
  // Supported only by some of the traits
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  bp::class_<Landmarks_pl>("Arr_landmarks_point_location")
    .def(bp::init<>())
    .def(bp::init<Arrangement_2&>()[bp::with_custodian_and_ward<1, 2>()])
    .def("attach", &landmarks_pl_attach, bp::with_custodian_and_ward<1, 2>())
    .def("detach", &Landmarks_pl::detach)
    .def("locate", &locate<Landmarks_pl>, bp::with_custodian_and_ward_postcall<0,1>())
    ;
#endif
  bp::class_<Trapezoid_pl, boost::noncopyable>("Arr_trapezoid_ric_point_location")
    .def(bp::init<>())
    .def(bp::init<Arrangement_2&>()[bp::with_custodian_and_ward<1, 2>()])
    .def("attach", &Trapezoid_pl::attach, bp::with_custodian_and_ward<1,2>())
    .def("detach", &Trapezoid_pl::detach)
    .def("depth", &Trapezoid_pl::depth)
    .def("longest_query_path_length", &Trapezoid_pl::longest_query_path_length)
    .def("with_guarantees", &Trapezoid_pl::with_guarantees)
    .def<Arrangement_2* (Trapezoid_pl::*)()>("arrangement",
                                             &Trapezoid_pl::arrangement,
                                             bp::return_value_policy<bp::reference_existing_object>())
    .def("locate", &locate<Trapezoid_pl>, bp::with_custodian_and_ward_postcall<0, 1>())
    .def("ray_shoot_up", &Trapezoid_pl::ray_shoot_up)
    .def("ray_shoot_down", &Trapezoid_pl::ray_shoot_down)
    ;

  bp::class_<Wal_pl>("Arr_walk_along_line_point_location")
    .def(bp::init<>())
    .def(bp::init<Arrangement_2&>()[bp::with_custodian_and_ward<1, 2>()])
    .def("attach", &Wal_pl::attach, bp::with_custodian_and_ward<1, 2>())
    .def("detach", &Wal_pl::detach)
    .def("locate", &locate<Wal_pl>, bp::with_custodian_and_ward_postcall<0, 1>())
    .def("ray_shoot_up", &Wal_pl::ray_shoot_up)
    .def("ray_shoot_down", &Wal_pl::ray_shoot_down)
    ;

  bp::class_<Naive_pl>("Arr_naive_point_location")
    .def(bp::init<>())
    .def(bp::init<Arrangement_2&>()[bp::with_custodian_and_ward<1, 2>()])
    .def("attach", &Naive_pl::attach, bp::with_custodian_and_ward<1, 2>())
    .def("detach", &Naive_pl::detach)
    .def("locate", &locate<Naive_pl>, bp::with_custodian_and_ward_postcall<0, 1>())
    ;

  bp::def("locate", &locate_batch, bp::with_custodian_and_ward_postcall<1, 0>());
}
