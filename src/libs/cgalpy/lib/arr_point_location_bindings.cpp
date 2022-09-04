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
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace aos2 {

typedef typename aos2::Arrangement_2 Arrangement_2;
typedef typename aos2::Face_const_handle Face_const_handle;
typedef typename aos2::Halfedge_const_handle Halfedge_const_handle;
typedef typename aos2::Vertex_const_handle Vertex_const_handle;

typedef typename CGAL::Arr_point_location_result<Arrangement_on_surface_2>::Type
  Pl_result;
typedef typename std::pair<Point_2, Pl_result>                  Pl_query_result;

//
class Point_location_result_visitor : public boost::static_visitor<py::object> {
public:
  template<typename T>
  py::object operator()(T& operand) const
  { return py::cast(&(*operand)); }
};

//
py::list locate_batch(const Arrangement_on_surface_2& arr, const py::list& lst)
{
  py::list res;
  auto op = [&] (const Pl_query_result& p) mutable {
      const auto& result =
        boost::apply_visitor(Point_location_result_visitor(), p.second);
      res.append(py::make_tuple(p.first, result));
    };
  // The argument type of boost::function_output_iterator (UnaryFunction) must
  // be Assignable and Copy Constructible; hence the application of std::ref().
  auto it = boost::make_function_output_iterator(std::ref(op));
  auto begin = stl_input_iterator<Point_2>(lst);
  auto end = stl_input_iterator<Point_2>(lst, false);
  locate(arr, begin, end, it);
  return res;
}

//
template <typename PL>
py::object locate(PL& pl, const Point_2& p) {
  auto result = pl.locate(p);
  return boost::apply_visitor(Point_location_result_visitor(), result);
}

//
template <typename PL>
py::object ray_shoot_up(PL& pl, const Point_2& p) {
  auto result = pl.ray_shoot_up(p);
  return boost::apply_visitor(Point_location_result_visitor(), result);
}

//
template <typename PL>
py::object ray_shoot_down(PL& pl, const Point_2& p) {
  auto result = pl.ray_shoot_down(p);
  return boost::apply_visitor(Point_location_result_visitor(), result);
}

// #if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS || \
//   CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS || \
//   CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
// void landmarks_pl_attach(Landmarks_pl& pl, Arrangement_on_surface_2& arr)
// { pl.attach(arr); }
// #endif

}

void export_point_location(py::module_& m) {
  using Aos = aos2::Arrangement_on_surface_2;
  using Naive_pl = CGAL::Arr_naive_point_location<Aos>;
  using Walk_pl = CGAL::Arr_walk_along_line_point_location<Aos>;
  using Landmarks_pl = CGAL::Arr_landmarks_point_location<Aos>;
  using Trapezoid_pl = CGAL::Arr_trapezoid_ric_point_location<Aos>;
  constexpr auto ri(py::rv_policy::reference_internal);

  // Supported only by some of the traits
#if CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS || \
  CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS
  py::class_<Landmarks_pl>(m, "Arr_landmarks_point_location")
    .def(py::init<>())
    .def(py::init<Aos&>())
    .def("attach", &Landmarks_pl::attach)
    .def("detach", &Landmarks_pl::detach)
    .def("locate", &aos2::locate<Landmarks_pl>, ri)
    // .def("ray_shoot_up", &aos2::ray_shoot_up<Landmarks_pl>, ri)
    // .def("ray_shoot_down", &aos2::ray_shoot_down<Landmarks_pl>, ri)
    ;
#endif

  py::class_<Trapezoid_pl>(m, "Arr_trapezoid_ric_point_location")
    .def(py::init<>())
    .def(py::init<Aos&>())
    .def("attach", &Trapezoid_pl::attach)
    .def("detach", &Trapezoid_pl::detach)
    .def("depth", &Trapezoid_pl::depth)
    .def("longest_query_path_length", &Trapezoid_pl::longest_query_path_length)
    .def("with_guarantees", &Trapezoid_pl::with_guarantees)
    // .def<Aos*(Trapezoid_pl::*)()>("arrangement", &Trapezoid_pl::arrangement)
    .def("locate", &aos2::locate<Trapezoid_pl>, ri)
    .def("ray_shoot_up", &aos2::ray_shoot_up<Trapezoid_pl>, ri)
    .def("ray_shoot_down", &aos2::ray_shoot_down<Trapezoid_pl>, ri)
    ;

  py::class_<Walk_pl>(m, "Arr_walk_along_line_point_location")
    .def(py::init<>())
    .def(py::init<Aos&>())
    .def("attach", &Walk_pl::attach)
    .def("detach", &Walk_pl::detach)
    .def("locate", &aos2::locate<Walk_pl>, ri)
    .def("ray_shoot_up", &aos2::ray_shoot_up<Walk_pl>, ri)
    .def("ray_shoot_down", &aos2::ray_shoot_down<Walk_pl>, ri)
    ;

  py::class_<Naive_pl>(m, "Arr_naive_point_location")
    .def(py::init<>())
    .def(py::init<Aos&>())
    .def("attach", &Naive_pl::attach)
    .def("detach", &Naive_pl::detach)
    .def("locate", &aos2::locate<Naive_pl>, ri)
    // .def("ray_shoot_up", &aos2::ray_shoot_up<Naive_pl>, ri)
    // .def("ray_shoot_down", &aos2::ray_shoot_down<Naive_pl>, ri)
    ;

  m.def("locate", &aos2::locate_batch, ri, py::keep_alive<1, 0>());
}
