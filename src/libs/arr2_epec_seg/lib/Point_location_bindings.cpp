#include <Common.h>

//void locate(Arrangement_2& arr, boost::python::list& lst)
//{
//  auto v = std::vector< Point_2 >(boost::python::stl_input_iterator< Point_2 >(lst),
//    boost::python::stl_input_iterator< Point_2 >());
//}

template <typename type>
bool is_type(Pl_result& pl_result)
{
  type* get;
  bool res = (get = boost::get<type>(&pl_result));
  return res;
}

template <typename type>
bool get_type(Pl_result& pl_result, typename type::value_type& t)
{
  type* get;
  bool res = (get = boost::get<type>(&pl_result));
  if (res) t = *(*get);
  return res;
}

void Landmarks_pl_attach(Landmarks_pl& pl, Arrangement_2& arr)
{
  pl.attach(arr);
}


void export_Point_location()
{
  using namespace boost::python;
  class_<Landmarks_pl>("Arr_landmarks_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>())
    .def("attach", &Landmarks_pl_attach)
    .def("detach", &Landmarks_pl::detach)
    .def("locate", &Landmarks_pl::locate)
    ;

  class_<Trapezoid_pl, boost::noncopyable>("Arr_trapezoid_ric_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>())
    .def("attach", &Trapezoid_pl::attach)
    .def("detach", &Trapezoid_pl::detach)
    .def("depth", &Trapezoid_pl::depth)
    .def("longest_query_path_length", &Trapezoid_pl::longest_query_path_length)
    .def("with_guarantees", &Trapezoid_pl::with_guarantees)
    .def<Arrangement_2* (Trapezoid_pl::*)()>("arrangement", &Trapezoid_pl::arrangement, return_value_policy<reference_existing_object>())
    .def("locate", &Trapezoid_pl::locate)
    .def("ray_shoot_up", &Trapezoid_pl::ray_shoot_up)
    .def("ray_shoot_down", &Trapezoid_pl::ray_shoot_down)
    ;

  class_<Wal_pl>("Arr_walk_along_line_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>())
    .def("attach", &Wal_pl::attach)
    .def("detach", &Wal_pl::detach)
    .def("locate", &Wal_pl::locate)
    .def("ray_shoot_up", &Wal_pl::ray_shoot_up)
    .def("ray_shoot_down", &Wal_pl::ray_shoot_down)
    ;

  class_<Naive_pl>("Arr_naive_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>())
    .def("attach", &Naive_pl::attach)
    .def("detach", &Naive_pl::detach)
    .def("locate", &Naive_pl::locate)
    ;

  class_<Pl_result>("Arr_point_location_result")
    .def("empty", &Pl_result::empty)
    .def("is_face", &is_type<Face_const_handle>)
    .def("is_halfedge", &is_type<Halfedge_const_handle>)
    .def("is_vertex", &is_type<Vertex_const_handle>)
    .def("get_face", &get_type<Face_const_handle>)
    .def("get_halfedge", &get_type<Halfedge_const_handle>)
    .def("get_vertex", &get_type<Vertex_const_handle>)
    ;
}