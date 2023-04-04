// Copyright (c) 2022 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <sstream>
#include <iostream>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

namespace py = nanobind;

namespace CGAL {

std::ostream& operator<<(std::ostream& os, const py::object& obj) {
  os << py::cast<std::string>(py::str(obj));
  return os;
}

std::istream& operator>>(std::istream& is, py::object& obj) {
  std::string str;
  is >> str;
  obj = py::cast(str);
  return is;
}

}

#include <CGAL/IO/Arr_iostream.h>
#include <CGAL/IO/Arr_text_formatter.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"

void export_arrangement_2_io(py::class_<aos2::Arrangement_2,
                                        aos2::Arrangement_on_surface_2>& arr_c)
{
  using Aos = aos2::Arrangement_on_surface_2;
  using Arr = aos2::Arrangement_2;
  using Formatter = CGAL::Arr_extended_dcel_text_formatter<Arr>;

#if (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_LINEAR_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_SEGMENT_GEOMETRY_TRAITS) || \
    (CGALPY_AOS2_GEOMETRY_TRAITS == CGALPY_AOS2_NON_CACHING_SEGMENT_GEOMETRY_TRAITS)

  // If all cell types (i.e., Vertex, Halfedge, and Face) are extended, use
  // a dedicated formatter that insert amd exports the auxliray data as well.
#if (defined(CGALPY_AOS2_VERTEX_EXTENDED) &&    \
     defined(CGALPY_AOS2_HALFEDGE_EXTENDED) &&  \
     defined(CGALPY_AOS2_FACE_EXTENDED))
  arr_c.def("__str__", [](const Arr& arr) {
                         std::ostringstream os;
                         Formatter formatter;
                         CGAL::IO::write(arr, os, formatter);
                         return os.str();
                       });
  arr_c.def("__repr__", [](const Arr& arr) {
                          std::ostringstream os;
                          Formatter formatter;
                          CGAL::IO::write(arr, os, formatter);
                          return os.str();
                        });
  arr_c.def("__init__", [](Arr* arr, const std::string& str) {
                          std::istringstream is(str);
                          new (arr) Arr();      // placement new
                          Formatter formatter;
                          CGAL::IO::read(*arr, is, formatter);
                        });

#else
  add_insertion(arr_c, "__str__");
  add_insertion(arr_c, "__repr__");
  add_extraction(arr_c);
#endif
#endif
}
