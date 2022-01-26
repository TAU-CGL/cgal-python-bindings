// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#define BOOST_BIND_GLOBAL_PLACEHOLDERS 1

#include <boost/python.hpp>

#include "CGALPY/kernel_types.hpp"

namespace bp = boost::python;

typedef typename Kernel::Intersect_2                               Intersect_2;

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2) are not a valid overload for do_intersect
// in which case the second version (which does nothing) will be used instead (SFINAE)
template<typename T1, typename T2>
void bind_do_intersect_2T(decltype(CGAL::do_intersect<Kernel>(T1(), T2())))
{
  bp::def<bool(const T1&, const T2&)>("do_intersect", &CGAL::do_intersect<Kernel>);
}

template<typename, typename>
void bind_do_intersect_2T(...) {}

template <typename T>
void bind_do_intersect_1T() {
  bind_do_intersect_2T<T, Point_2>(true);
  bind_do_intersect_2T<T, Line_2>(true);
  bind_do_intersect_2T<T, Ray_2>(true);
  bind_do_intersect_2T<T, Segment_2>(true);
  bind_do_intersect_2T<T, Triangle_2>(true);
  bind_do_intersect_2T<T, Iso_rectangle_2>(true);
  bind_do_intersect_2T<T, Circle_2>(true);
}

void bind_do_intersect() {
  bind_do_intersect_1T<Point_2>();
  bind_do_intersect_1T<Line_2>();
  bind_do_intersect_1T<Ray_2>();
  bind_do_intersect_1T<Segment_2>();
  bind_do_intersect_1T<Triangle_2>();
  bind_do_intersect_1T<Iso_rectangle_2>();
  bind_do_intersect_1T<Circle_2>();
}

class Intersection_visitor : public boost::static_visitor<bp::object>
{
public:
  template<typename T>
  bp::object operator()(T& operand) const { return bp::object(operand); }

  // Overload for vector
  bp::object operator()(std::vector<Point_2>& operand) const {
    bp::list lst;
    for (Point_2 p : operand) lst.append(p);
    return lst;
  }
};

template <typename T1, typename T2>
bp::object cgalpy_intersection(T1& t1, T2& t2) {
  auto result = CGAL::intersection<Kernel>(t1, t2);
  if (!result) { // No intersection, return None
    return bp::object();
  }
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
void bind_intersection_2T(bool) {
  bp::def("intersection", &cgalpy_intersection<T1, T2>);
}

template<typename, typename>
void bind_intersection_2T(...) {}

template <typename T>
void bind_intersection_1T() {
  bind_intersection_2T<T, Iso_rectangle_2>(true);
  bind_intersection_2T<T, Line_2>(true);
  bind_intersection_2T<T, Ray_2>(true);
  bind_intersection_2T<T, Segment_2>(true);
  bind_intersection_2T<T, Triangle_2>(true);
  bind_intersection_2T<T, Point_2>(true);
}

void bind_intersection() {
  bind_intersection_1T<Iso_rectangle_2>();
  bind_intersection_1T<Line_2>();
  bind_intersection_1T<Ray_2>();
  bind_intersection_1T<Segment_2>();
  bind_intersection_1T<Triangle_2>();
  bind_intersection_1T<Point_2>();
}

void export_intersections_2() {
  bind_intersection();
  bind_do_intersect();
}
