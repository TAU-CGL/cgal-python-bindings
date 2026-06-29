// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#ifndef CGALPY_EXPOST_PROPERTY_MAP_HPP
#define CGALPY_EXPOST_PROPERTY_MAP_HPP

#include <nanobind/nanobind.h>
#include <nanobind/make_iterator.h>

#include <type_traits>

#include <CGAL/boost/graph/properties.h>

namespace py = nanobind;

namespace internal {

//!
template <typename Graph, typename Key, typename Value>
auto export_property_map(py::module_& m, const std::string& name) {
  using Graph_property_map = typename Graph::template Property_map<Key, Value>;

  if constexpr (std::is_same_v<Value, bool>) {
    m.def("get", [](const Graph_property_map& p, const Key& t) -> bool {
        return static_cast<bool>(get(p, t));
      },
      py::arg("property_map"), py::arg("key"),
      "Returns the boolean value associated with a key in a graph property map.");

    // return for extending with .def()
    return py::class_<Graph_property_map>(m, name.c_str(), "Boolean graph property map.")
      .def(py::init<>(),
           "Constructs an empty boolean graph property map.")
      .def("__getitem__", [](Graph_property_map& self, Key key) -> bool {
          return static_cast<bool>(self[key]);
        },
        py::arg("key"),
        "Returns the boolean value associated with a key.")
      .def("__setitem__", [](Graph_property_map& self, Key key, bool value) {
          self[key] = value;
        },
        py::arg("key"), py::arg("value"),
        "Sets the boolean value associated with a key.")
      /* Boolean property maps are backed by `std::vector<bool>`. Their iterator
       * dereferences to a proxy reference, which nanobind cannot expose safely.
       * Force the iterator value type to `bool` so Python receives ordinary
       * `bool` values instead of `std::vector<bool>` proxy objects.
       */
      .def("__iter__",
           [](Graph_property_map& self) {
             using Iterator = typename Graph_property_map::iterator;
             return py::make_iterator<py::rv_policy::copy, Iterator, Iterator, bool>
               (py::type<Iterator>(), "Iterator", self.begin(), self.end());
           })
      .def("transfer", [](Graph_property_map& self, const Graph_property_map& other) {
          return self.transfer(other);
        },
        py::arg("other"),
        "Transfers values from another boolean property map.")
      .def("transfer",
           [](Graph_property_map& self, const Graph_property_map& other,
              std::size_t from, std::size_t to) {
             return self.transfer(other, from, to);
           },
        py::arg("other"), py::arg("from"), py::arg("to"),
        "Transfers values from another boolean property map in an index range.")
      .def("reset", &Graph_property_map::reset,
           "Resets the boolean property map.")
      ;
  }
  else {
    m.def("get", [](const Graph_property_map& p, const Key& t) { return get(p, t); },
          py::arg("property_map"), py::arg("key"),
          "Returns the value associated with a key in a graph property map.");

    // return for extending with .def()
    return py::class_<Graph_property_map>(m, name.c_str(), "Graph property map.")
      .def(py::init<>(),
           "Constructs an empty graph property map.")
      .def("__getitem__", [](Graph_property_map& self, Key key) -> Value { return self[key]; },
           py::arg("key"),
           "Returns the value associated with a key.")
      .def("__setitem__", [](Graph_property_map& self, Key key, Value value) { self[key] = value; },
           py::arg("key"), py::arg("value"),
           "Sets the value associated with a key.")
      .def("__iter__",
           [](Graph_property_map& self) {
             return py::make_iterator(py::type<typename Graph_property_map::iterator>(),
                                      "Iterator", self.begin(), self.end(), py::keep_alive<0, 1>());
           })
      .def("transfer", [](Graph_property_map& self, const Graph_property_map& other) { return self.transfer(other); },
           py::arg("other"),
           "Transfers values from another property map.")
      .def("transfer", [](Graph_property_map& self, const Graph_property_map& other, std::size_t from, std::size_t to)
      { return self.transfer(other, from, to); },
           py::arg("other"), py::arg("from"), py::arg("to"),
           "Transfers values from another property map in an index range.")
      .def("reset", &Graph_property_map::reset,
           "Resets the property map.")
      ;
  }
}


} // namespace internal

#endif
