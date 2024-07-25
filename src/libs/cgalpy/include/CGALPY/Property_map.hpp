#ifndef CGALPY_PROPERTY_MAP_HPP
#define CGALPY_PROPERTY_MAP_HPP

#include "CGALPY/make_iterator.hpp"
#include <CGAL/boost/graph/properties.h>
#include <nanobind/nanobind.h>

namespace py = nanobind;

namespace internal {

  // Mesh::Property_map<Mesh::Edge_index, bool> ecm =
  //   sm.add_property_map<Mesh::Edge_index, bool>("ecm",false).first;
  // for (Mesh::Edge_index e : edges(sm))
  //   ecm[e] = false;

template <typename Mesh, typename Key, typename Value>
class Property_map : public Mesh::template Property_map<Key, Value> {
};

template <typename Mesh, typename Key, typename Value>
void export_property_map(py::module_& m, const std::string& name) {
  using Mesh_property_map = typename Mesh::template Property_map<Key, Value>;

  py::class_<Mesh_property_map>(m, name.c_str())
    .def(py::init<>())
    // .def("__getitem__", [](Mesh_property_map& self, Key key) -> Value& {
    //   return self[key];
    // })
    // .def("__setitem__", [](Mesh_property_map& self, Key key, Value value) {
    //   self[key] = value;
    // })
    // .def("__iter__", [](Mesh_property_map& self) {
    //   return make_iterator(self.begin(), self.end());
    // })
    ;
}

} // namespace internal

#endif // CGALPY_PROPERTY_MAP_HPP
