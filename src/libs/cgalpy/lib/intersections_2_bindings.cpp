// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>

#include "CGALPY/common.hpp"

typedef typename Kernel::Intersect_2                               Intersect_2;
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

Point_iso_rectangle_intersection_result
point_iso_rectangle_intersection(Point_2& p1, Iso_rectangle_2& i1)
{
  return intersection(p1, i1);
}

Point_line_intersection_result point_line_intersection(Point_2& p1, Line_2& l1)
{
  return intersection(p1, l1);
}

Point_ray_intersection_result point_ray_intersection(Point_2& p1, Ray_2& r1)
{
  return intersection(p1, r1);
}

Point_segment_intersection_result point_segment_intersection(Point_2& p1,
                                                             Segment_2& s1)
{
  return intersection(p1, s1);
}

Point_triangle_intersection_result point_triangle_intersection(Point_2& p1,
                                                               Triangle_2& t1)
{
  return intersection(p1, t1);
}

Iso_rectangle_iso_rectangle_intersection_result
iso_rectangle_iso_rectangle_intersection(Iso_rectangle_2& i1,
                                         Iso_rectangle_2& i2)
{
  return intersection(i1, i2);
}

Iso_rectangle_line_intersection_result
iso_rectangle_line_intersection(Iso_rectangle_2& i1, Line_2& l1)
{
  return intersection(i1, l1);
}

Iso_rectangle_ray_intersection_result
iso_rectangle_ray_intersection(Iso_rectangle_2& i1, Ray_2& r1)
{
  return intersection(i1, r1);
}

Iso_rectangle_segment_intersection_result
iso_rectangle_segment_intersection(Iso_rectangle_2& i1, Segment_2& s1)
{
  return intersection(i1, s1);
}

Iso_rectangle_triangle_intersection_result
iso_rectangle_triangle_intersection(Iso_rectangle_2& i1, Triangle_2& t1)
{
  return intersection(i1, t1);
}

Line_line_intersection_result line_line_intersection(Line_2& l1, Line_2& l2)
{
  return intersection(l1, l2);
}

Line_ray_intersection_result line_ray_intersection(Line_2& l1, Ray_2& r1)
{
  return intersection(l1, r1);
}

Line_segment_intersection_result line_segment_intersection(Line_2& l1,
                                                           Segment_2& s1)
{
  return intersection(l1, s1);
}

Line_triangle_intersection_result line_triangle_intersection(Line_2& l1,
                                                             Triangle_2& t1)
{
  return intersection(l1, t1);
}

Ray_ray_intersection_result ray_ray_intersection(Ray_2& r1, Ray_2& r2)
{
  return intersection(r1, r2);
}

Ray_segment_intersection_result ray_segment_intersection(Ray_2& r1,
                                                         Segment_2& s1)
{
  return intersection(r1, s1);
}

Ray_triangle_intersection_result ray_triangle_intersection(Ray_2& r1,
                                                           Triangle_2& t1)
{
  return intersection(r1, t1);
}

Segment_segment_intersection_result segment_segment_intersection(Segment_2& s1,
                                                                 Segment_2& s2)
{
  return intersection(s1, s2);
}

Segment_triangle_intersection_result
segment_triangle_intersection(Segment_2& s1, Triangle_2& t1)
{
  return intersection(s1, t1);
}

Triangle_triangle_intersection_result
triangle_triangle_intersection(Triangle_2& t1, Triangle_2& t2)
{
  return intersection(t1, t2);
}

template<typename result>
bool empty(result& res)
{
  if (res) return false;
  return true;
}

template<typename result, typename type>
bool is_type(result& intersection)
{
  if (!intersection) return false;
  type* get;
  bool res = (get = boost::get<type>(&*intersection));
  return res;
}

template<typename result, typename type>
bool get_type(result& intersection, type& t)
{
  if (!intersection) return false;
  type* get;
  bool res = (get = boost::get<type>(&*intersection));
  if (res) t = *get;
  return res;
}

template<typename result>
bool is_points(result& intersection)
{
  if (!intersection) return false;
  std::vector<Point_2>* get;
  bool res = (get = boost::get< std::vector<Point_2> >(&*intersection));
  return res;
}

template<typename result>
bool get_points(result& intersection, boost::python::list& lst)
{
  if (!intersection) return false;
  std::vector<Point_2>* get;
  bool res = (get = boost::get< std::vector<Point_2> >(&*intersection));
  if (res)
  {
    for (Point_2 p : *get)
    {
      lst.append(p);
    }
  }
  return res;
}

template<typename result>
boost::python::class_<result> bind_intersection_result(const char* python_name)
{
  using namespace boost::python;
  auto c = class_<result>(python_name, no_init)
    .def("empty", &empty<result>)
    ;
  return c;
}

// Two versions exist since some pairs of types (i.e Circle_2 and Triangle_2) are not a valid overload for do_intersect
// in which case the second version (which does nothing) will be used instead (SFINAE)
template<typename T1, typename T2>
void bind_do_intersect_2T(decltype(CGAL::do_intersect<Kernel>(T1(), T2())))
{
  using namespace boost::python;
  def<bool(const T1&, const T2&)>("do_intersect", &CGAL::do_intersect<Kernel>);
}

template<typename, typename>
void bind_do_intersect_2T(...) {}

template <typename T>
void bind_do_intersect_1T()
{
  bind_do_intersect_2T<T, Point_2>(true);
  bind_do_intersect_2T<T, Line_2>(true);
  bind_do_intersect_2T<T, Ray_2>(true);
  bind_do_intersect_2T<T, Segment_2>(true);
  bind_do_intersect_2T<T, Triangle_2>(true);
  bind_do_intersect_2T<T, Iso_rectangle_2>(true);
  bind_do_intersect_2T<T, Circle_2>(true);
}

void bind_do_intersect()
{
  bind_do_intersect_1T<Point_2>();
  bind_do_intersect_1T<Line_2>();
  bind_do_intersect_1T<Segment_2>();
  bind_do_intersect_1T<Triangle_2>();
  bind_do_intersect_1T<Iso_rectangle_2>();
  bind_do_intersect_1T<Circle_2>();
}

void export_intersections_2()
{
  using namespace boost::python;
  //intersections

  def("intersection", &point_iso_rectangle_intersection);
  bind_intersection_result<Point_iso_rectangle_intersection_result>("Point_iso_rectangle_intersection_result")
    .def("get_point", &get_type<Point_iso_rectangle_intersection_result, Point_2>)
    ;
  def("intersection", &point_iso_rectangle_intersection);
  def("intersection", &point_line_intersection);
  def("intersection", &point_ray_intersection);
  def("intersection", &point_segment_intersection);
  def("intersection", &point_triangle_intersection);

  def("intersection", &iso_rectangle_iso_rectangle_intersection);
  bind_intersection_result<Iso_rectangle_iso_rectangle_intersection_result>("Iso_rectangle_iso_rectangle_intersection_result")
    .def("is_iso_rectangle", &is_type<Iso_rectangle_iso_rectangle_intersection_result, Iso_rectangle_2>)
    .def("get_iso_rectangle", &get_type<Iso_rectangle_iso_rectangle_intersection_result, Iso_rectangle_2>)
    ;

  def("intersection", &iso_rectangle_line_intersection);
  bind_intersection_result<Iso_rectangle_line_intersection_result>("Iso_rectangle_line_intersection_result")
    .def("is_point", &is_type<Iso_rectangle_line_intersection_result, Point_2>)
    .def("get_point", &get_type<Iso_rectangle_line_intersection_result, Point_2>)
    .def("is_segment", &is_type<Iso_rectangle_line_intersection_result, Segment_2>)
    .def("get_segment", &get_type<Iso_rectangle_line_intersection_result, Segment_2>)
    ;

  def("intersection", &iso_rectangle_ray_intersection);
  def("intersection", &iso_rectangle_segment_intersection);

  def("intersection", &iso_rectangle_triangle_intersection);
  bind_intersection_result<Iso_rectangle_triangle_intersection_result>("Iso_rectangle_triangle_intersection_result")
    .def("is_point", &is_type<Iso_rectangle_triangle_intersection_result, Point_2>)
    .def("get_point", &get_type<Iso_rectangle_triangle_intersection_result, Point_2>)
    .def("is_segment", &is_type<Iso_rectangle_triangle_intersection_result, Segment_2>)
    .def("get_segment", &get_type<Iso_rectangle_triangle_intersection_result, Segment_2>)
    .def("is_triangle", &get_type<Iso_rectangle_triangle_intersection_result, Triangle_2>)
    .def("is_points", &is_points< Iso_rectangle_triangle_intersection_result>)
    .def("get_points", &get_points< Iso_rectangle_triangle_intersection_result>)
    ;

  def("intersection", &line_line_intersection);
  bind_intersection_result<Line_line_intersection_result>("Line_line_intersection_result")
    .def("is_point", &is_type<Line_line_intersection_result, Point_2>)
    .def("get_point", &get_type<Line_line_intersection_result, Point_2>)
    .def("is_line", &is_type<Line_line_intersection_result, Line_2>)
    .def("get_line", &get_type<Line_line_intersection_result, Line_2>)
    ;

  def("intersection", &line_ray_intersection);
  bind_intersection_result<Line_ray_intersection_result>("Line_ray_intersection_result")
    .def("is_point", &is_type<Line_ray_intersection_result, Point_2>)
    .def("get_point", &get_type<Line_ray_intersection_result, Point_2>)
    .def("is_ray", &is_type<Line_ray_intersection_result, Ray_2>)
    .def("get_ray", &get_type<Line_ray_intersection_result, Ray_2>)
    ;

  def("intersection", &line_segment_intersection);
  def("intersection", &line_triangle_intersection);

  def("intersection", &ray_ray_intersection);
  bind_intersection_result<Ray_ray_intersection_result>("Ray_ray_intersection_result")
    .def("is_point", &is_type<Ray_ray_intersection_result, Point_2>)
    .def("get_point", &get_type<Ray_ray_intersection_result, Point_2>)
    .def("is_segment", &is_type<Ray_ray_intersection_result, Segment_2>)
    .def("get_segment", &get_type<Ray_ray_intersection_result, Segment_2>)
    .def("is_ray", &is_type<Ray_ray_intersection_result, Ray_2>)
    .def("get_ray", &get_type<Ray_ray_intersection_result, Ray_2>)
    ;
  def("intersection", &ray_segment_intersection);
  def("intersection", &ray_triangle_intersection);

  def("intersection", &segment_segment_intersection);
  def("intersection", &segment_triangle_intersection);

  def("intersection", &triangle_triangle_intersection);
  bind_intersection_result<Triangle_triangle_intersection_result>("Triangle_triangle_intersection_result")
    .def("is_point", &is_type<Triangle_triangle_intersection_result, Point_2>)
    .def("get_point", &get_type<Triangle_triangle_intersection_result, Point_2>)
    .def("is_segment", &is_type<Triangle_triangle_intersection_result, Segment_2>)
    .def("get_segment", &get_type<Triangle_triangle_intersection_result, Segment_2>)
    .def("is_triangle", &get_type<Triangle_triangle_intersection_result, Triangle_2>)
    .def("is_points", &is_points< Triangle_triangle_intersection_result>)
    .def("get_points", &get_points< Triangle_triangle_intersection_result>)
    ;

  bind_do_intersect();
}
