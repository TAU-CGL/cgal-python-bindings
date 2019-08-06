#include <Common.h>

Line_line_intersection_result line_line_intersection(Line_2& l1, Line_2& l2)
{
  return intersection(l1, l2);
}

Line_ray_intersection_result line_ray_intersection(Line_2& l1, Ray_2& r1)
{
  return intersection(l1, r1);
}

Line_segment_intersection_result line_segment_intersection(Line_2& l1, Segment_2& s1)
{
  return intersection(l1, s1);
}

Ray_ray_intersection_result ray_ray_intersection(Ray_2& r1, Ray_2& r2)
{
  return intersection(r1, r2);
}

Ray_segment_intersection_result ray_segment_intersection(Ray_2& r1, Segment_2& s1)
{
  return intersection(r1, s1);
}

Segment_segment_intersection_result segment_segment_intersection(Segment_2& s1, Segment_2& s2)
{
  return intersection(s1, s2);
}


template<typename result>
bool is_empty(result& res)
{
  if (res) return false;
  return true;
}

template<typename result>
bool is_point(result& intersection)
{
  Point_2* get;
  bool res = (get = boost::get<Point_2>(&*intersection));
  return res;
}

template<typename result>
bool is_segment(result& intersection)
{
  Segment_2* get;
  bool res = (get = boost::get<Segment_2>(&*intersection));
  return res;
}

template<typename result>
bool is_ray(result& intersection)
{
  Ray_2* get;
  bool res = (get = boost::get<Ray_2>(&*intersection));
  return res;
}

template<typename result>
bool is_line(result& intersection)
{
  Line_2* get;
  bool res = (get = boost::get<Line_2>(&*intersection));
  return res;
}

template<typename result>
bool get_point(result& intersection, Point_2& p)
{
  Point_2* get;
  bool res = (get = boost::get<Point_2>(&*intersection));
  if (res) p = *get;
  return res;
}

template<typename result>
bool get_segment(result& intersection, Segment_2& p)
{
  Segment_2* get;
  bool res = (get = boost::get<Segment_2>(&*intersection));
  if (res) p = *get;
  return res;
}

template<typename result>
bool get_ray(result& intersection, Ray_2& p)
{
  Ray_2* get;
  bool res = (get = boost::get<Ray_2>(&*intersection));
  if (res) p = *get;
  return res;
}

template<typename result>
bool get_line(result& intersection, Line_2& p)
{
  Line_2* get;
  bool res = (get = boost::get<Line_2>(&*intersection));
  if (res) p = *get;
  return res;
}

template<typename result>
boost::python::class_<result> bind_result(const char* python_name)
{
  using namespace boost::python;
  auto c = class_<result>(python_name, no_init)
    .def("is_empty", &is_empty<result>)
    .def("is_point", &is_point<result>)
    .def("get_point", &get_point<result>)
    //.def("value", &get_result<result>)
    //.def("get_segment", &get_segment<result>)
    //.def("get_ray", &get_ray<result>)
    //.def("get_line", &get_line<result>)
    ;
  return c;
}

void export_Intersections_2()
{
  using namespace boost::python;
  //intersections
  def("intersection", &line_line_intersection);
  bind_result<Line_line_intersection_result>("Line_line_intersection_result")
    .def("is_line", &is_line<Line_line_intersection_result>)
    .def("get_line", &get_line<Line_line_intersection_result>)
    ;

  def("intersection", &line_ray_intersection);
  bind_result<Line_ray_intersection_result>("Line_ray_intersection_result")
    .def("is_ray", &is_ray<Line_ray_intersection_result>)
    .def("get_ray", &get_ray<Line_ray_intersection_result>)
    ;

  def("intersection", &line_segment_intersection);
  bind_result<Line_segment_intersection_result>("Line_segment_intersection_result")
    .def("is_segment", &is_segment<Line_segment_intersection_result>)
    .def("get_segment", &get_segment<Line_segment_intersection_result>)
    ;

  def("intersection", &ray_ray_intersection);
  /*bind_result<Ray_ray_intersection_result>("Ray_ray_intersection_result")
    .def("get_ray", &get_ray<Ray_ray_intersection_result>)
    .def("get_segment", &get_segment<Ray_ray_intersection_result>);
    */

  def("intersection", &ray_segment_intersection);
  /*bind_result<Ray_segment_intersection_result>("Ray_segment_intersection_result")
    .def("get_segment", &get_segment<Ray_segment_intersection_result>);*/

  def("intersection", &segment_segment_intersection);
  /*bind_result<Segment_segment_intersection_result>("Segment_segment_intersection_result")
    .def("get_segment", &get_segment<Segment_segment_intersection_result>);*/
}