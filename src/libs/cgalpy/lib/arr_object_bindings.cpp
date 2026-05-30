// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "cgalpy/arrangement_on_surface_2_types.hpp"
#include "cgalpy/Aos2_docstrings.hpp"

namespace py = nanobind;
namespace aos2_doc = cgalpy::aos2::docstrings;

template<typename T0, typename T1>
bool is_type(Object& o) { return o.is<T0>() || o.is<T1>(); }

// Assign the value to t
// Return true iff assignment was successful
template<typename T0, typename T1>
bool get_type(Object& o, typename T0::value_type& t) {
  T0 get0;
  bool res = CGAL::assign<T0>(get0, o);
  if (res) {
    t = *(get0);
    return res;
  }
  else {
    T1 get1;
    bool res = CGAL::assign<T1>(get1, o);
    if (res) t = *(get1);
  }
  return res;
}

void export_object(py::module_& m) {
  using Arr2 = cgalpy::aos2::Arrangement_2;

  py::class_<Object>(m, "Object", "Type-erased arrangement object.")
    .def("empty", &Object::empty, "Return whether the object is empty.")
    .def("is_vertex", &is_type<Arr2::Vertex_handle, cgalpy::aos2::Vertex_const_handle>,
         "Return whether the object stores a vertex handle.")
    .def("get_vertex", &get_type<Arr2::Vertex_handle, cgalpy::aos2::Vertex_const_handle>,
         py::arg("out"), "Assign the stored vertex handle to out, if possible.")
    .def("is_halfedge", &is_type<Arr2::Halfedge_handle, cgalpy::aos2::Halfedge_const_handle>,
         "Return whether the object stores a halfedge handle.")
    .def("get_halfedge", &get_type<Arr2::Halfedge_handle, cgalpy::aos2::Halfedge_const_handle>,
         py::arg("out"), "Assign the stored halfedge handle to out, if possible.")
    .def("is_face", &is_type<Arr2::Face_handle, cgalpy::aos2::Face_const_handle>,
         "Return whether the object stores a face handle.")
    .def("get_face", &get_type<Arr2::Face_handle, cgalpy::aos2::Face_const_handle>,
         py::arg("out"), "Assign the stored face handle to out, if possible.")
    ;
}
