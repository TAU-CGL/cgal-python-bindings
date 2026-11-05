// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>
//            Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

#include <CGAL/convex_hull_3.h>

#include "cgalpy/kernel_types.hpp"
#include "cgalpy/convex_hull_3_types.hpp"
#include "cgalpy/iterators/py_list_forward_iterator.hpp"
#include "cgalpy/Ch3_docstrings.hpp"
#include "cgalpy/Named_parameter_wrapper.hpp"
#include "cgalpy/named_parameter_applicator.hpp"
#include "cgalpy/Named_parameter_vertex_point_map.hpp"
#include "cgalpy/Named_parameter_geom_traits.hpp"

namespace py = nanobind;
namespace ch3_doc = cgalpy::ch3::docstrings;

namespace cgalpy {
namespace ch3 {

//! convex_hull_3 implementation
void convex_hull_3_impl(py::list& lst, ch3::Polygonal_mesh& ch) {
  auto begin = py_list_forward_iterator<Point_3>(lst);
  auto end = py_list_forward_iterator<Point_3>(lst, false);
  CGAL::convex_hull_3(begin, end, ch);
}

//! convex_hull_3
ch3::Polygonal_mesh convex_hull_3(py::list& lst) {
  ch3::Polygonal_mesh ch;
  convex_hull_3_impl(lst, ch);
  return ch;
}

//!
void convex_hull_3_with_kernel_impl(py::list& lst, ch3::Polygonal_mesh& ch, const Kernel& kernel) {
  auto begin = py_list_forward_iterator<Point_3>(lst);
  auto end = py_list_forward_iterator<Point_3>(lst, false);
  CGAL::convex_hull_3(begin, end, ch, kernel);
}

//!
ch3::Polygonal_mesh convex_hull_3_with_kernel(py::list& lst, const Kernel& kernel) {
  ch3::Polygonal_mesh ch;
  convex_hull_3_with_kernel_impl(lst, ch, kernel);
  return ch;
}

#if CGALPY_CH3_POLYGONAL_MESH == CGALPY_CH3_POLYHEDRON_3_POLYGONAL_MESH

//! convex_hull_3
ch3::Polygonal_mesh
convex_hull_3_with_traits(py::list& lst, const ch3::Polygonal_mesh::Traits& traits) {
  ch3::Polygonal_mesh ch(traits);
  convex_hull_3_impl(lst, ch);
  return ch;
}

//! convex_hull_3
ch3::Polygonal_mesh
convex_hull_3_with_traits_with_kernel(py::list& lst, const ch3::Polygonal_mesh::Traits& traits, const Kernel& kernel) {
  ch3::Polygonal_mesh ch(traits);
  convex_hull_3_with_kernel_impl(lst, ch, kernel);
  return ch;
}

/*! A class template that wraps the function template
 * convex_hull_3()
 */
template <typename T, typename... Args>
struct Convex_hull_3_wrapper {
  static auto call(T np, Args&&... args)
  { CGAL::convex_hull_3(std::forward<Args>(args)..., std::forward<T>(np)); }
};

//! convex_hull_3
ch3::Polygonal_mesh convex_hull_3_from_pm(ch3::Polygonal_mesh& pm, const py::dict& params = py::dict()) {
  using Pm = ch3::Polygonal_mesh;
  Pm ch;

  auto np = CGAL::parameters::default_values();
  cgalpy::Named_parameter_vertex_point_map<Pm> vpm_op;
  cgalpy::Named_parameter_geom_traits gt_op;
  cgalpy::Named_parameter_wrapper<Convex_hull_3_wrapper, const Pm&, Pm&> wrapper(pm, ch);
  cgalpy::named_parameter_applicator(wrapper, np, params, vpm_op, gt_op);
  return ch;
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
  m.def("convex_hull_3", &cgalpy::ch3::convex_hull_3_from_pm,
        py::arg("pm"), py::arg("np") = py::dict(), ch3_doc::convex_hull_3);
#endif

  using Isc1 = bool(*)(const Pm&);
  using Isc2 = bool(*)(const Pm&, const Kernel&);
  m.def("is_strongly_convex_3",
        static_cast<Isc1>(&CGAL::is_strongly_convex_3<Pm>),
        py::arg("ch"), ch3_doc::is_strongly_convex_3);
  m.def("is_strongly_convex_3",
        static_cast<Isc2>(&CGAL::is_strongly_convex_3<Pm>),
        py::arg("ch"), py::arg("kernel"), ch3_doc::is_strongly_convex_3);
}
