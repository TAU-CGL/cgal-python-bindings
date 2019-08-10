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
bool empty(result& res)
{
  if (res) return false;
  return true;
}

template<typename result, typename type>
bool is_type(result& intersection)
{
  type* get;
  bool res = (get = boost::get<type>(&*intersection));
  return res;
}

template<typename result, typename type>
bool get_type(result& intersection, type& t)
{
  type* get;
  bool res = (get = boost::get<type>(&*intersection));
  if (res) t = *get;
  return res;
}

template<typename result>
boost::python::class_<result> bind_intersection_result(const char* python_name)
{
  using namespace boost::python;
  auto c = class_<result>(python_name, no_init)
    .def("empty", &empty<result>)
    .def("is_point", &is_type<result, Point_2>)
    .def("get_point", &get_type<result, Point_2>)
    ;
  return c;
}

void export_Intersections_2()
{
  using namespace boost::python;
  //intersections
  def("intersection", &line_line_intersection);
  bind_intersection_result<Line_line_intersection_result>("Line_line_intersection_result")
    .def("is_line", &is_type<Line_line_intersection_result, Line_2>)
    .def("get_line", &get_type<Line_line_intersection_result, Line_2>)
    ;

  def("intersection", &line_ray_intersection);
  bind_intersection_result<Line_ray_intersection_result>("Line_ray_intersection_result")
    .def("is_ray", &is_type<Line_ray_intersection_result, Ray_2>)
    .def("get_ray", &get_type<Line_ray_intersection_result, Ray_2>)
    ;

  def("intersection", &line_segment_intersection);
  bind_intersection_result<Line_segment_intersection_result>("Line_segment_intersection_result")
    .def("is_segment", &is_type<Line_segment_intersection_result, Segment_2>)
    .def("get_segment", &get_type<Line_segment_intersection_result, Segment_2>)
    ;

  def("intersection", &ray_ray_intersection);
  /*bind_intersection_result<Ray_ray_intersection_result>("Ray_ray_intersection_result")
    .def("get_ray", &get_ray<Ray_ray_intersection_result>)
    .def("get_segment", &get_segment<Ray_ray_intersection_result>);
    */

  def("intersection", &ray_segment_intersection);
  /*bind_intersection_result<Ray_segment_intersection_result>("Ray_segment_intersection_result")
    .def("get_segment", &get_segment<Ray_segment_intersection_result>);*/

  def("intersection", &segment_segment_intersection);
  /*bind_intersection_result<Segment_segment_intersection_result>("Segment_segment_intersection_result")
    .def("get_segment", &get_segment<Segment_segment_intersection_result>);*/
}