// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_naive_point_location.h>
#include <CGAL/Arr_walk_along_line_point_location.h>
#include <CGAL/Arr_trapezoid_ric_point_location.h>
#include <CGAL/Arr_landmarks_point_location.h>

#include "CGALPY/arr_point_location_config.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"

namespace py = nanobind;

namespace aos2 {

typedef typename aos2::Arrangement_2 Arrangement_2;
typedef typename aos2::Face_const_handle Face_const_handle;
typedef typename aos2::Halfedge_const_handle Halfedge_const_handle;
typedef typename aos2::Vertex_const_handle Vertex_const_handle;

typedef typename CGAL::Arr_naive_point_location<Arrangement_2>
  Naive_pl;
typedef typename CGAL::Arr_walk_along_line_point_location<Arrangement_2>
  Walk_pl;
typedef typename CGAL::Arr_landmarks_point_location<Arrangement_2>
  Landmarks_pl;
typedef typename CGAL::Arr_trapezoid_ric_point_location<Arrangement_2>
  Trapezoid_pl;

typedef typename CGAL::Arr_point_location_result<Arrangement_2>::Type Pl_result;
typedef typename std::pair<Point_2, Pl_result>                  Pl_query_result;

class Point_location_result_visitor : public boost::static_visitor<py::object> {
public:
  template<typename T>
  py::object operator()(T& operand) const { return py::handle(*operand); }
};

py::list locate_batch(Arrangement_2& arr, const py::list& lst) {
  py::list res;
  auto v = std::vector<Point_2>(py::iterator<Point_2>(lst),
                                py::iterator<Point_2>());
  auto op =
    [&] (const Pl_query_result& p) mutable {
      const auto& result =
        boost::apply_visitor(Point_location_result_visitor(), p.second);
      res.append(py::make_tuple(p.first, result));
    };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  locate(arr, v.begin(), v.end(), it);
  return res;
}

template <typename PL>
py::object locate(PL& pl, Point_2& p) {
  auto result = pl.locate(p);
  return boost::apply_visitor(Point_location_result_visitor(), result);
}

#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
void landmarks_pl_attach(Landmarks_pl& pl, Arrangement_2& arr)
{ pl.attach(arr); }
#endif

}

void export_point_location(py::module_& m) {
  typedef aos2::Arrangement_2                           Arr;
  typedef CGAL::Arr_naive_point_location<Arr>           Naive_pl;
  typedef CGAL::Arr_walk_along_line_point_location<Arr> Walk_pl;
  typedef CGAL::Arr_landmarks_point_location<Arr>       Landmarks_pl;
  typedef CGAL::Arr_trapezoid_ric_point_location<Arr>   Trapezoid_pl;

  // Supported only by some of the traits
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  py::class_<Landmarks_pl>(m, "Arr_landmarks_point_location")
    .def(py::init<>())
    .def(py::init<Arr&>())
    .def("attach", &aos2::landmarks_pl_attach)
    .def("detach", &Landmarks_pl::detach)
    .def("locate", &aos2::locate<Landmarks_pl>)
    ;
#endif
  py::class_<Trapezoid_pl>(m, "Arr_trapezoid_ric_point_location")
    .def(py::init<>())
    .def(py::init<Arr&>())
    .def("attach", &Trapezoid_pl::attach)
    .def("detach", &Trapezoid_pl::detach)
    .def("depth", &Trapezoid_pl::depth)
    .def("longest_query_path_length", &Trapezoid_pl::longest_query_path_length)
    .def("with_guarantees", &Trapezoid_pl::with_guarantees)
    .def<Arr*(Trapezoid_pl::*)()>("arrangement", &Trapezoid_pl::arrangement)
    .def("locate", &aos2::locate<Trapezoid_pl>)
    .def("ray_shoot_up", &Trapezoid_pl::ray_shoot_up)
    .def("ray_shoot_down", &Trapezoid_pl::ray_shoot_down)
    ;

  py::class_<Walk_pl>(m, "Arr_walk_along_line_point_location")
    .def(py::init<>())
    .def(py::init<Arr&>())
    .def("attach", &Walk_pl::attach)
    .def("detach", &Walk_pl::detach)
    .def("locate", &aos2::locate<Walk_pl>)
    .def("ray_shoot_up", &Walk_pl::ray_shoot_up)
    .def("ray_shoot_down", &Walk_pl::ray_shoot_down)
    ;

  py::class_<Naive_pl>(m, "Arr_naive_point_location")
    .def(py::init<>())
    .def(py::init<Arr&>())
    .def("attach", &Naive_pl::attach)
    .def("detach", &Naive_pl::detach)
    .def("locate", &aos2::locate<Naive_pl>)
    ;

  m.def("locate", &aos2::locate_batch);
}
