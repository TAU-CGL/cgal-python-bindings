// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/kernel_types.hpp"

namespace py = nanobind;

/// Handle do_intersect
///@{

typedef typename Kernel::Intersect_2                               Intersect_2;

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2)
// are not a valid overload for do_intersect in which case the second version
// (which does nothing) will be used instead (SFINAE)
template<typename T1, typename T2>
void bind_do_intersect_pair(py::module_& m, decltype(CGAL::do_intersect<Kernel>(T1(), T2())))
{
  m.def("do_intersect", static_cast<bool(*)(const T1&, const T2&)>(&CGAL::do_intersect<Kernel>));
}

template<typename, typename>
void bind_do_intersect_pair(py::module_&, ...) {}

template<typename T> void bind_do_intersect_inner(py::module_&, T) {}

template<typename T1, typename T2, typename... Ts>
void bind_do_intersect_inner(py::module_& m, T1 arg1, T2 arg2, Ts... args) {
  typedef typename std::remove_pointer<T1>::type        PT1;
  typedef typename std::remove_pointer<T2>::type        PT2;
  bind_do_intersect_pair<PT1, PT2>(m, true);
  bind_do_intersect_pair<PT2, PT1>(m, true);
  bind_do_intersect_inner(m, arg1, args...);
}

template<typename T>
void bind_do_intersect(py::module_& m, T arg) {
  typedef typename std::remove_pointer<T>::type         PT;
  bind_do_intersect_pair<PT, PT>(m, true);
}

template <typename T1, typename... Ts>
void bind_do_intersect(py::module_& m, T1 arg, Ts... args) {
  bind_do_intersect_inner(m, arg, args...);
  bind_do_intersect(m, args...);
  typedef typename std::remove_pointer<T1>::type         PT1;
  bind_do_intersect_pair<PT1, PT1>(m, true);
}
///@}

/// Handle intersections
///@{
class Intersection_visitor : public boost::static_visitor<py::object> {
public:
  template<typename T>
  py::object operator()(T& operand) const { return py::cast(operand); }

  // Overload for vector
  py::object operator()(std::vector<Point_2>& operand) const {
    py::list lst;
    for (const auto& p : operand) lst.append(p);
    return lst;
  }
};

template <typename T1, typename T2>
py::object cgalpy_intersection(const T1& t1, const T2& t2) {
  auto result = CGAL::intersection<Kernel>(t1, t2);
  if (! result) return py::object();    // no intersection
  return boost::apply_visitor(Intersection_visitor(), *result);
}

// The supported overloaded functions CGAL::intersection(T1& t1, T2& t2) have
// a complicated return value; it's a polymorphic object. It can be nothing or
// one of several types  that depends on the type of the input arguments.
// Therefore, the selection is implemented in a different way as follows. We
// still use the type of the return value (of the particular
// CGAL::intersection(T1& t1, T2& t2) function, but we do not try to match
// this type to the type of an argument. Instead, we use this type as the
// default value of an unnamed template parameter.
template <typename T1, typename T2,
          typename = decltype(CGAL::intersection<Kernel>(T1(), T2()))>
void bind_intersection_pair(py::module_& m, bool) {
  m.def("intersection", &cgalpy_intersection<T1, T2>);
}

template<typename, typename>
void bind_intersection_pair(py::module_&, ...) {}

template<typename T> void bind_intersection_inner(py::module_&, T) {}

template<typename T1, typename T2, typename... Ts>
void bind_intersection_inner(py::module_& m, T1 arg1, T2 arg2, Ts... args) {
  typedef typename std::remove_pointer<T1>::type        PT1;
  typedef typename std::remove_pointer<T2>::type        PT2;
  bind_intersection_pair<PT1, PT2>(m, true);
  bind_intersection_pair<PT2, PT1>(m, true);
  bind_intersection_inner(m, arg1, args...);
}

template<typename T>
void bind_intersection(py::module_& m, T arg) {
  typedef typename std::remove_pointer<T>::type         PT;
  bind_intersection_pair<PT, PT>(m, true);
}

template <typename T1, typename... Ts>
void bind_intersection(py::module_& m, T1 arg, Ts... args) {
  bind_intersection_inner(m, arg, args...);
  bind_intersection(m, args...);
  typedef typename std::remove_pointer<T1>::type        PT1;
  bind_intersection_pair<PT1, PT1>(m, true);
}

void export_intersections_2(py::module_& m) {
  Iso_rectangle_2* iso_rectangle_2(nullptr);
  Line_2* line_2(nullptr);
  Ray_2* ray_2(nullptr);
  Segment_2* segment_2(nullptr);
  Triangle_2* triangle_2(nullptr);
  Point_2* point_2(nullptr);
  Circle_2* circle_2(nullptr);
  bind_intersection(m, iso_rectangle_2, line_2, ray_2, segment_2,
                    triangle_2, point_2, circle_2);
  bind_do_intersect(m, iso_rectangle_2, line_2, ray_2, segment_2,
                    triangle_2, point_2, circle_2);
}
///@}
