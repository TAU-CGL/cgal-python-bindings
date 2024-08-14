#ifndef CGALPY_PROPERTY_MAP_HPP
#define CGALPY_PROPERTY_MAP_HPP

#include <nanobind/nanobind.h>

#include <CGAL/boost/graph/properties.h>

#include "CGALPY/make_iterator.hpp"

namespace py = nanobind;

namespace internal {

template <typename Mesh, typename Key, typename Value>
auto export_property_map(py::module_& m, const std::string& name) {
  using Mesh_property_map = typename Mesh::template Property_map<Key, Value>;

  m.def("get", [](const Mesh_property_map& p, const Key& t) {
    return get(p, t);
  });

  // return for extending with .def()
  return py::class_<Mesh_property_map>(m, name.c_str())
    .def(py::init<>())
    .def("__getitem__", [](Mesh_property_map& self, Key key) -> Value {
      return self[key];
    })
    .def("__setitem__", [](Mesh_property_map& self, Key key, Value value) {
      self[key] = value;
    })
    .def("__iter__", [](Mesh_property_map& self) {
      return make_iterator(self.begin(), self.end());
    })
    .def("transfer", [](Mesh_property_map& self, const Mesh_property_map& other) {
      return self.transfer(other);
    })
    .def("transfer", [](Mesh_property_map& self, const Mesh_property_map& other, std::size_t from, std::size_t to) {
      return self.transfer(other, from, to);
    })
    .def("reset", &Mesh_property_map::reset)
    ;
}

template <typename Mesh, typename Key>
auto export_property_map_bool(py::module_& m, const std::string& name) {
  using Mesh_property_map = typename Mesh::template Property_map<Key, bool>;

  m.def("get", [](const Mesh_property_map& p, const Key& t) {
    return py::bool_(get(p, t));
  });

  // return for extending with .def()
  return py::class_<Mesh_property_map>(m, name.c_str())
    .def(py::init<>())
    .def("__getitem__", [](Mesh_property_map& self, Key key) -> bool {
      return self[key];
    })
    .def("__setitem__", [](Mesh_property_map& self, Key key, bool value) {
      self[key] = value;
    })
    .def("__iter__", [](Mesh_property_map& self) {
      return make_iterator(self.begin(), self.end());
    })
    .def("transfer", [](Mesh_property_map& self, const Mesh_property_map& other) {
      return self.transfer(other);
    })
    .def("transfer", [](Mesh_property_map& self, const Mesh_property_map& other, std::size_t from, std::size_t to) {
      return self.transfer(other, from, to);
    })
    .def("reset", &Mesh_property_map::reset)
    ;
}



} // namespace internal

#endif // CGALPY_PROPERTY_MAP_HPP
