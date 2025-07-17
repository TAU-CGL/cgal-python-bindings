// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <CGAL/convex_hull_3.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/convex_hull_3_types.hpp"
#include "CGALPY/stl_forward_iterator.hpp"

namespace py = nanobind;

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

//!
void export_convex_hull_3(py::module_& m) {
  using Pm = ch3::Polygonal_mesh;

  m.def("convex_hull_3", &ch3::convex_hull_3);
  m.def("convex_hull_3", &ch3::convex_hull_3_with_kernel);

#if CGALPY_CH3_POLYGONAL_MESH == CGALPY_CH3_POLYHEDRON_3_POLYGONAL_MESH
  m.def("convex_hull_3", &ch3::convex_hull_3_with_traits,
        py::keep_alive<0, 2>());
  m.def("convex_hull_3", &ch3::convex_hull_3_with_traits_with_kernel,
        py::keep_alive<0, 2>());
#endif

  using Isc1 = bool(*)(const Pm&);
  using Isc2 = bool(*)(const Pm&, const Kernel&);
  m.def("is_strongly_convex_3",
        static_cast<Isc1>(&CGAL::is_strongly_convex_3<Pm>));
  m.def("is_strongly_convex_3",
        static_cast<Isc2>(&CGAL::is_strongly_convex_3<Pm>));
}
