#include <Common.h>

void locate(Arrangement_2& arr, boost::python::list& lst)
{
  auto v = std::vector< Point_2 >(boost::python::stl_input_iterator< Point_2 >(lst),
    boost::python::stl_input_iterator< Point_2 >());
}





void export_Point_location()
{
  using namespace boost::python;
  class_<Landmarks_pl>("Arr_landmarks_point_location")
    .def(init<>())
    .def(init<Arrangement_2&>())
    .def("attach", &Landmarks_pl::attach)
    .def("detach", &Landmarks_pl::detach)
    //.def("depth", &Trapezoid_pl::depth)
    //.def("longest_query_path_length", &Trapezoid_pl::longest_query_path_length)
    //.def("arrangement", &arrangement, return_value_policy<reference_existing_object>())
    .def("locate", &Landmarks_pl::locate)
   ;
}