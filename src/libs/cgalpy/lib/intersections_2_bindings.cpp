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
#include "CGALPY/cartesian_product.hpp"

namespace py = nanobind;

/// Handle Intersections
///@{

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2)
// are not a valid overload for do_intersect in which case the first version
// (which does nothing) will be used instead (SFINAE)
template<typename, typename> void bind_do_intersect(py::module_&, ...) {}

template<typename T1, typename T2>
void bind_do_intersect(py::module_& m,
                       decltype(CGAL::do_intersect<Kernel>(T1(), T2()))) {
  using Do_intersect = bool(*)(const T1&, const T2&);
  m.def("do_intersect", static_cast<Do_intersect>(&CGAL::do_intersect<Kernel>));
}

// Intersections
class Intersection_visitor {
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
  if (! result) return py::none();    // no intersection
  return std::visit(Intersection_visitor(), *result);
}

// The supported overloaded functions CGAL::intersection(T1& t1, T2& t2) have
// a complicated return value; it's a polymorphic object. It can be nothing or
// one of several types  that depends on the type of the input arguments.
// Therefore, the selection is implemented in a different way as follows. We
// still use the type of the return value (of the particular
// CGAL::intersection(T1& t1, T2& t2) function, but we do not try to match
// this type to the type of an argument. Instead, we use this type as the
// default value of an unnamed template parameter.
template<typename, typename> void bind_intersection(py::module_&, ...) {}

template <typename T1, typename T2,
          typename = decltype(CGAL::intersection<Kernel>(T1(), T2()))>
void bind_intersection(py::module_& m, bool) {
  m.def("intersection", &cgalpy_intersection<T1, T2>);
}

template <typename Arg, typename ... Types> struct Wrapper {
  void operator()(Arg& arg) {
    bind_do_intersect<Types...>(arg, true);
    bind_intersection<Types...>(arg, true);
    // ((std::cout << typeid(Types).name() << " "), ...);
    // std::cout << std::endl;
  }
};

void export_intersections(py::module_& m) {
  CGALPY::Type_list<Iso_rectangle_2, Line_2, Ray_2, Segment_2, Triangle_2,
                    Point_2, Circle_2> type_list_2;
  CGALPY::Type_list<Iso_cuboid_3, Line_3, Ray_3, Segment_3, Tetrahedron_3,
                    Triangle_3, Point_3, Sphere_3> type_list_3;
  CGALPY::cartesian_product<Wrapper>(m, type_list_2, type_list_2);
  CGALPY::cartesian_product<Wrapper>(m, type_list_3, type_list_3);
}

///@}
