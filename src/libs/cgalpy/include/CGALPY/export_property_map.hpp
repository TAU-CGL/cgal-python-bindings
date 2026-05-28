// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#ifndef CGALPY_EXPOST_PROPERTY_MAP_HPP
#define CGALPY_EXPOST_PROPERTY_MAP_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>

#include <CGAL/boost/graph/properties.h>

namespace py = nanobind;

namespace internal {

//!
template <typename Mesh, typename Key, typename Value>
auto export_property_map(py::module_& m, const std::string& name) {
  using Mesh_property_map = typename Mesh::template Property_map<Key, Value>;

  m.def("get", [](const Mesh_property_map& p, const Key& t) { return get(p, t); },
        py::arg("property_map"), py::arg("key"),
        "Returns the value associated with a key in a mesh property map.");

  // return for extending with .def()
  return py::class_<Mesh_property_map>(m, name.c_str(),
                                      "Surface_mesh property map.")
    .def(py::init<>(),
         "Constructs an empty Surface_mesh property map.")
    .def("__getitem__", [](Mesh_property_map& self, Key key) -> Value { return self[key]; },
         py::arg("key"),
         "Returns the value associated with a key.")
    .def("__setitem__", [](Mesh_property_map& self, Key key, Value value) { self[key] = value; },
         py::arg("key"), py::arg("value"),
         "Sets the value associated with a key.")
    .def("__iter__", [](Mesh_property_map& self) {
      return py::make_iterator(py::type<typename Mesh_property_map::iterator>(),
                               "Iterator", self.begin(), self.end(), py::keep_alive<0, 1>());
    })
    .def("transfer", [](Mesh_property_map& self, const Mesh_property_map& other) { return self.transfer(other); },
         py::arg("other"),
         "Transfers values from another property map.")
    .def("transfer", [](Mesh_property_map& self, const Mesh_property_map& other, std::size_t from, std::size_t to)
    { return self.transfer(other, from, to); },
         py::arg("other"), py::arg("from"), py::arg("to"),
         "Transfers values from another property map in an index range.")
    .def("reset", &Mesh_property_map::reset,
         "Resets the property map.")
    ;
}

//!
template <typename Mesh, typename Key>
auto export_property_map_bool(py::module_& m, const std::string& name) {
  using Mesh_property_map = typename Mesh::template Property_map<Key, bool>;

  m.def("get", [](const Mesh_property_map& p, const Key& t) { return py::bool_(get(p, t)); },
        py::arg("property_map"), py::arg("key"),
        "Returns the boolean value associated with a key in a mesh property map.");

  // return for extending with .def()
  return py::class_<Mesh_property_map>(m, name.c_str(),
                                      "Boolean Surface_mesh property map.")
    .def(py::init<>(),
         "Constructs an empty boolean Surface_mesh property map.")
    .def("__getitem__", [](Mesh_property_map& self, Key key) -> bool { return self[key]; },
         py::arg("key"),
         "Returns the boolean value associated with a key.")
    .def("__setitem__", [](Mesh_property_map& self, Key key, bool value) { self[key] = value; },
         py::arg("key"), py::arg("value"),
         "Sets the boolean value associated with a key.")
    // Do not expose __iter__ for bool maps: CGAL Surface_mesh<bool>
    // property maps are backed by std::vector<bool>, whose iterator yields
    // proxy references that nanobind cannot cast safely.
    .def("transfer", [](Mesh_property_map& self, const Mesh_property_map& other) { return self.transfer(other); },
         py::arg("other"),
         "Transfers values from another boolean property map.")
    .def("transfer", [](Mesh_property_map& self, const Mesh_property_map& other, std::size_t from, std::size_t to)
    { return self.transfer(other, from, to); },
         py::arg("other"), py::arg("from"), py::arg("to"),
         "Transfers values from another boolean property map in an index range.")
    .def("reset", &Mesh_property_map::reset,
         "Resets the boolean property map.")
    ;
}

} // namespace internal

#endif
