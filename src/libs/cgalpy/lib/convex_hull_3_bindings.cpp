// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <CGAL/convex_hull_3.h>

#include "cgalpy/kernel_types.hpp"
#include "cgalpy/convex_hull_3_types.hpp"
#include "cgalpy/stl_forward_iterator.hpp"
#include "cgalpy/Ch3_docstrings.hpp"

namespace py = nanobind;
namespace ch3_doc = cgalpy::ch3::docstrings;

namespace cgalpy {
namespace ch3 {

//! convex_hull_3
void convex_hull_3_impl(py::list& lst, ch3::Polygonal_mesh& pm) {
  auto begin = stl_forward_iterator<Point_3>(lst);
  auto end = stl_forward_iterator<Point_3>(lst, false);
  CGAL::convex_hull_3(begin, end, pm);
}

//! convex_hull_3
ch3::Polygonal_mesh convex_hull_3(py::list& lst) {
  ch3::Polygonal_mesh pm;
  convex_hull_3_impl(lst, pm);
  return pm;
}

//!
void convex_hull_3_with_kernel_impl(py::list& lst, ch3::Polygonal_mesh& pm,
                                    const Kernel& kernel) {
  auto begin = stl_forward_iterator<Point_3>(lst);
  auto end = stl_forward_iterator<Point_3>(lst, false);
  CGAL::convex_hull_3(begin, end, pm, kernel);
}

//!
ch3::Polygonal_mesh convex_hull_3_with_kernel(py::list& lst,
                                              const Kernel& kernel) {
  ch3::Polygonal_mesh pm;
  convex_hull_3_with_kernel_impl(lst, pm, kernel);
  return pm;
}

#if CGALPY_CH3_POLYGONAL_MESH == CGALPY_CH3_POLYHEDRON_3_POLYGONAL_MESH

//! convex_hull_3
ch3::Polygonal_mesh
convex_hull_3_with_traits(py::list& lst,
                          const ch3::Polygonal_mesh::Traits& traits) {
  ch3::Polygonal_mesh pm(traits);
  convex_hull_3_impl(lst, pm);
  return pm;
}

//!
ch3::Polygonal_mesh
convex_hull_3_with_traits_with_kernel(py::list& lst,
                                      const ch3::Polygonal_mesh::Traits& traits,
                                      const Kernel& kernel) {
  ch3::Polygonal_mesh pm(traits);
  convex_hull_3_with_kernel_impl(lst, pm, kernel);
  return pm;
}

#endif

}
} // namespace cgalpy

//!
void export_convex_hull_3(py::module_& m) {
  using Pm = cgalpy::ch3::Polygonal_mesh;

  m.def("convex_hull_3", &cgalpy::ch3::convex_hull_3,
        py::arg("points"), ch3_doc::convex_hull_3);
  m.def("convex_hull_3", &cgalpy::ch3::convex_hull_3_with_kernel,
        py::arg("points"), py::arg("kernel"), ch3_doc::convex_hull_3);

#if CGALPY_CH3_POLYGONAL_MESH == CGALPY_CH3_POLYHEDRON_3_POLYGONAL_MESH
  m.def("convex_hull_3", &cgalpy::ch3::convex_hull_3_with_traits,
        py::arg("points"), py::arg("traits"), py::keep_alive<0, 2>(),
        ch3_doc::convex_hull_3);
  m.def("convex_hull_3", &cgalpy::ch3::convex_hull_3_with_traits_with_kernel,
        py::arg("points"), py::arg("traits"), py::arg("kernel"),
        py::keep_alive<0, 2>(), ch3_doc::convex_hull_3);
#endif

  using Isc1 = bool(*)(const Pm&);
  using Isc2 = bool(*)(const Pm&, const Kernel&);
  m.def("is_strongly_convex_3",
        static_cast<Isc1>(&CGAL::is_strongly_convex_3<Pm>),
        py::arg("pm"), ch3_doc::is_strongly_convex_3);
  m.def("is_strongly_convex_3",
        static_cast<Isc2>(&CGAL::is_strongly_convex_3<Pm>),
        py::arg("pm"), py::arg("kernel"), ch3_doc::is_strongly_convex_3);
}
