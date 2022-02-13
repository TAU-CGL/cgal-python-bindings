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

/// Handle do_intersect
///@{

typedef typename Kernel::Intersect_2                               Intersect_2;

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2)
// are not a valid overload for do_intersect in which case the second version
// (which does nothing) will be used instead (SFINAE)
template<typename T1, typename T2>
void bind_do_intersect_pair(decltype(CGAL::do_intersect<Kernel>(T1(), T2())))
{
  bp::def<bool(const T1&, const T2&)>("do_intersect", &CGAL::do_intersect<Kernel>);
}

template<typename, typename>
void bind_do_intersect_pair(...) {}

template<typename T> void bind_do_intersect_inner(T) {}

template<typename T1, typename T2, typename... Ts>
void bind_do_intersect_inner(T1 arg1, T2 arg2, Ts... args) {
  bind_do_intersect_pair<T1, T2>(true);
  bind_do_intersect_pair<T2, T1>(true);
  bind_do_intersect_inner(arg1, args...);
}

template<typename T>
void bind_do_intersect(T arg) { bind_do_intersect_pair<T, T>(true); }

template <typename T1, typename... Ts>
void bind_do_intersect(T1 arg, Ts... args) {
  bind_do_intersect_inner(arg, args...);
  bind_do_intersect(args...);
  bind_do_intersect_pair<T1, T1>(true);
}
///@}

/// Handle intersections
///@{
class Intersection_visitor : public boost::static_visitor<bp::object> {
public:
  template<typename T>
  bp::object operator()(T& operand) const { return bp::object(operand); }

  // Overload for vector
  bp::object operator()(std::vector<Point_2>& operand) const {
    bp::list lst;
    for (const auto& p : operand) lst.append(p);
    return lst;
  }
};

template <typename T1, typename T2>
bp::object cgalpy_intersection(const T1& t1, const T2& t2) {
  auto result = CGAL::intersection<Kernel>(t1, t2);
  if (! result) return bp::object();    // no intersection
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
void bind_intersection_pair(bool) {
  bp::def("intersection", &cgalpy_intersection<T1, T2>);
}

template<typename, typename>
void bind_intersection_pair(...) {}

template<typename T> void bind_intersection_inner(T) {}

template<typename T1, typename T2, typename... Ts>
void bind_intersection_inner(T1 arg1, T2 arg2, Ts... args) {
  bind_intersection_pair<T1, T2>(true);
  bind_intersection_pair<T2, T1>(true);
  bind_intersection_inner(arg1, args...);
}

template<typename T>
void bind_intersection(T arg) { bind_intersection_pair<T, T>(true); }

template <typename T1, typename... Ts>
void bind_intersection(T1 arg, Ts... args) {
  bind_intersection_inner(arg, args...);
  bind_intersection(args...);
  bind_intersection_pair<T1, T1>(true);
}

void export_intersections_2() {
  bind_intersection(Iso_rectangle_2(), Line_2(), Ray_2(), Segment_2(),
                    Triangle_2(), Point_2(), Circle_2());
  bind_do_intersect(Iso_rectangle_2(), Line_2(), Ray_2(), Segment_2(),
                    Triangle_2(), Point_2(), Circle_2());
}
///@}
