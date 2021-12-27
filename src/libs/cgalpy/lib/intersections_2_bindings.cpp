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

template<typename result, typename type>
bool get_type(result& intersection, type& t)
{
  if (!intersection) return false;
  type* get;
  bool res = (get = boost::get<type>(&*intersection));
  if (res) t = get;
  return res;
}

template <typename T1, typename T2>
bp::object cgalpy_intersection(T1& t1, T2& t2) {
  auto result = CGAL::intersection<Kernel>(t1, t2);
  // Return python object containing the correct result
  Iso_rectangle_2* ir;
  if (get_type(result, ir)) {
    return bp::object(*ir);
  }
  Line_2* l;
  if (get_type(result, l)) {
    return bp::object(*l);
  }
  Ray_2* r;
  if (get_type(result, r)) {
    return bp::object(*r);
  }
  Segment_2* s;
  if (get_type(result, s)) {
    return bp::object(*s);
  }
  Triangle_2* t;
  if (get_type(result, t)) {
    return bp::object(*t);
  }
  std::vector<Point_2>* v;
  if (get_type(result, v)) {
    bp::list lst;
    for (Point_2 p : *v) lst.append(p);
    return lst;
  }
  // None
  return bp::object();
}

template <typename T1, typename T2>
void bind_intersection_2T(decltype(CGAL::intersection<Kernel>(T1(), T2()))) {
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
}

void bind_intersection() {
  bind_intersection_1T<Iso_rectangle_2>();
  bind_intersection_1T<Line_2>();
  bind_intersection_1T<Ray_2>();
  bind_intersection_1T<Segment_2>();
  bind_intersection_1T<Triangle_2>();
}

void export_intersections_2() {
  bind_intersection();
  bind_do_intersect();
}
