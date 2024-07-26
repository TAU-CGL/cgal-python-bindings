#ifndef CGALPY_GET_HPP
#define CGALPY_GET_HPP

#include <CGAL/boost/graph/properties.h>

#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace internal {

// gets
// mutable
template <typename Pm, typename T>
typename boost::property_map<Pm, CGAL::dynamic_vertex_property_t<T> >::type
get_d_v(CGAL::dynamic_vertex_property_t<T> p, Pm& sm, const T& default_value = T()) {
  return get(p, sm, default_value);
}

template <typename Pm, typename T>
typename boost::property_map<Pm, CGAL::dynamic_face_property_t<T> >::type
get_d_f(CGAL::dynamic_face_property_t<T> p, Pm& sm, const T& default_value = T()) {
  return get(p, sm, default_value);
}

template <typename Pm, typename T>
typename boost::property_map<Pm, CGAL::dynamic_edge_property_t<T> >::type
get_d_e(CGAL::dynamic_edge_property_t<T> p, Pm& sm, const T& default_value = T()) {
  return get(p, sm, default_value);
}

template <typename Pm, typename T>
typename boost::property_map<Pm, CGAL::dynamic_halfedge_property_t<T> >::type
get_d_h(CGAL::dynamic_halfedge_property_t<T> p, Pm& sm, const T& default_value = T()) {
  return get(p, sm, default_value);
}

// const
template <typename Pm, typename T>
typename boost::property_map<Pm, CGAL::dynamic_face_property_t<T> >::const_type
get_c_f(CGAL::dynamic_face_property_t<T> p, const Pm& sm, const T& default_value = T()) {
  return get(p, sm, default_value);
}

template <typename Pm, typename T>
typename boost::property_map<Pm, CGAL::dynamic_vertex_property_t<T> >::const_type
get_c_v(CGAL::dynamic_vertex_property_t<T> p, const Pm& sm, const T& default_value = T()) {
  return get(p, sm, default_value);
}

template <typename Pm, typename T>
typename boost::property_map<Pm, CGAL::dynamic_halfedge_property_t<T> >::const_type
get_c_h(CGAL::dynamic_halfedge_property_t<T> p, const Pm& sm, const T& default_value = T()) {
  return get(p, sm, default_value);
}

template <typename Pm, typename T>
typename boost::property_map<Pm, CGAL::dynamic_edge_property_t<T> >::const_type
get_c_e(CGAL::dynamic_edge_property_t<T> p, const Pm& sm, const T& default_value = T()) {
  return get(p, sm, default_value);
}


// get polyhedron, it doesn't have default values
//dynamic
template <typename Pm, typename T>
auto get_d_v_p(CGAL::dynamic_vertex_property_t<T> p, Pm& sm) {
  return get(p, sm);
}

template <typename Pm, typename T>
auto get_d_f_p(CGAL::dynamic_face_property_t<T> p, Pm& sm) {
  return get(p, sm);
}

template <typename Pm, typename T>
auto get_d_e_p(CGAL::dynamic_edge_property_t<T> p, Pm& sm) {
  return get(p, sm);
}

template <typename Pm, typename T>
auto get_d_h_p(CGAL::dynamic_halfedge_property_t<T> p, Pm& sm) {
  return get(p, sm);
}

//const
template <typename Pm, typename T>
auto get_c_f_p(CGAL::dynamic_face_property_t<T> p, const Pm& sm) {
  return get(p, sm);
}

template <typename Pm, typename T>
auto get_c_v_p(CGAL::dynamic_vertex_property_t<T> p, const Pm& sm) {
  return get(p, sm);
}

template <typename Pm, typename T>
auto get_c_h_p(CGAL::dynamic_halfedge_property_t<T> p, const Pm& sm) {
  return get(p, sm);
}

template <typename Pm, typename T>
auto get_c_e_p(CGAL::dynamic_edge_property_t<T> p, const Pm& sm) {
  return get(p, sm);
}


// these helpers don't work for some reason
// surface mesh
// template <typename Pm, typename Prop>
// void register_prop_map(py::module_& m, const std::string& map_name) {
//   using propmap = typename boost::property_map<Pm, Prop>::type;
//   py::class_<propmap>(m, map_name.c_str())
//     .def(py::init<>())
//     .def_ro("map_", &propmap::map_);
// }
//
// template <typename Pm, typename Prop>
// void register_get(py::module_& m, const std::string& prop_name) {
//   py::class_<Prop> prop(m, prop_name.c_str());
//   prop.def(py::init<>());
//   m.def("get", &internal::get_d_v<Pm, Prop>,
//         py::arg("property_map"), py::arg("sm"), py::arg("default_value") = Prop());
// }
//
// template <typename Pm, typename Prop>
// void register_prop_map_const(py::module_& m, const std::string& map_name) {
//   using propmap = typename boost::property_map<Pm, Prop>::const_type;
//   py::class_<propmap>(m, map_name.c_str())
//     .def(py::init<>())
//     .def_ro("map_", &propmap::map_);
// }
//
// template <typename Pm, typename Prop>
// void register_get_const(py::module_& m, const std::string& prop_name) {
//   py::class_<Prop> prop(m, prop_name.c_str());
//   prop.def(py::init<>());
//   m.def("get", &internal::get_c_v<Pm, Prop>,
//         py::arg("property_map"), py::arg("sm"), py::arg("default_value") = Prop());
// }
//
// template <typename Pm, typename Prop>
// void register_get_with_map(py::module_& m, const std::string& map_name, const std::string& prop_name) {
//   register_prop_map<Pm, Prop>(m, map_name);
//   register_get<Pm, Prop>(m, prop_name);
// }
//
// template <typename Pm, typename Prop>
// void register_get_with_map_const(py::module_& m, const std::string& map_name, const std::string& prop_name) {
//   register_prop_map_const<Pm, Prop>(m, map_name);
//   register_get_const<Pm, Prop>(m, prop_name);
// }
//
// polyhedron
// template <typename Pm, typename Prop>
// void register_prop_map_polyhedron(py::module_& m, const std::string& map_name) {
//   using propmap = typename boost::property_map<Pm, Prop>::type;
//   py::class_<propmap>(m, map_name.c_str())
//     .def(py::init<>())
//     .def_ro("map_", &propmap::map_);
// }
//
// template <typename Pm, typename Prop>
// void register_get_polyhedron(py::module_& m, const std::string& prop_name) {
//   py::class_<Prop> prop(m, prop_name.c_str());
//   prop.def(py::init<>());
//   m.def("get", &internal::get_d_f_p<Pm, Prop>,
//         py::arg("property_map"), py::arg("sm"));
// }
//
// template <typename Pm, typename Prop>
// void register_prop_map_polyhedron_const(py::module_& m, const std::string& map_name) {
//   using propmap = typename boost::property_map<Pm, Prop>::const_type;
//   py::class_<propmap>(m, map_name.c_str())
//     .def(py::init<>())
//     .def_ro("map_", &propmap::map_);
// }
//
// template <typename Pm, typename Prop>
// void register_get_polyhedron_const(py::module_& m, const std::string& prop_name) {
//   py::class_<Prop> prop(m, prop_name.c_str());
//   prop.def(py::init<>());
//   m.def("get", &internal::get_c_f_p<Pm, Prop>,
//         py::arg("property_map"), py::arg("sm"));
// }
//
//
// template <typename Pm, typename Prop>
// void register_get_with_map_polyhedron(py::module_& m, const std::string& map_name, const std::string& prop_name) {
//   register_prop_map_polyhedron<Pm, Prop>(m, map_name);
//   register_get_polyhedron<Pm, Prop>(m, prop_name);
// }
//
// template <typename Pm, typename Prop>
// void register_get_with_map_polyhedron_const(py::module_& m, const std::string& map_name, const std::string& prop_name) {
//   register_prop_map_polyhedron_const<Pm, Prop>(m, map_name);
//   register_get_polyhedron_const<Pm, Prop>(m, prop_name);
// }

} // namespace internal

#endif // CGALPY_GET_HPP
