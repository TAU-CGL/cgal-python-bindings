// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/envelope_3_types.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/stl_input_iterator.hpp"

namespace py = nanobind;

namespace env3 {

//
Envelope_diagram_2 lower_envelope_3(const py::list& surfaces) {
  Envelope_diagram_2 ed;
  auto begin = stl_input_iterator<Surface_3>(surfaces);
  auto end = stl_input_iterator<Surface_3>(surfaces, false);
  CGAL::lower_envelope_3(begin,	end, ed);
  return ed;
}

//
Envelope_diagram_2 lower_envelope_xy_monotone_3(const py::list& surfaces) {
  Envelope_diagram_2 ed;
  auto begin = stl_input_iterator<Surface_3>(surfaces);
  auto end = stl_input_iterator<Surface_3>(surfaces, false);
  CGAL::lower_envelope_3(begin,	end, ed);
  return ed;
}

//
Envelope_diagram_2 upper_envelope_3(const py::list& surfaces) {
  Envelope_diagram_2 ed;
  auto begin = stl_input_iterator<Surface_3>(surfaces);
  auto end = stl_input_iterator<Surface_3>(surfaces, false);
  CGAL::lower_envelope_3(begin,	end, ed);
  return ed;
}

//
Envelope_diagram_2 upper_envelope_xy_monotone_3(const py::list& surfaces) {
  Envelope_diagram_2 ed;
  auto begin = stl_input_iterator<Surface_3>(surfaces);
  auto end = stl_input_iterator<Surface_3>(surfaces, false);
  CGAL::lower_envelope_3(begin,	end, ed);
  return ed;
}

}

void export_envelope3(py::module_& m) {
  using Edos = env3::Envelope_diagram_on_surface_2;
  using Aos = Ed::Base;
  using Traits = env3::Traits;

#if CGALPY_ENV3_TRAITS == CGALPY_ENV3_PLANE_TRAITS
  if (! add_attr<Traits>(m, "Env_plane_traits_2"))
  py::class_<Traits> traits_c(m, "Env_plane_traits_2");
#elif CGALPY_ENV3_TRAITS == CGALPY_ENV3_SPHERE_TRAITS
  if (! add_attr<Traits>(m, "Env_sphere_traits_2"))
  py::class_<Traits> traits_c(m, "Env_sphere_traits_2");
#elif CGALPY_ENV3_TRAITS == CGALPY_ENV3_TRIANGLE_TRAITS
  if (! add_attr<Traits>(m, "Env_triangle_traits_2"))
  py::class_<Traits> traits_c(m, "Env_triangle_traits_2");
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_ENV3_TRAITS");
#endif

  if (! add_attr<Ed>(m, "Envelope_diagram_2"))
  py::class_<Edos, Aos> ed_c(m, "Envelope_diagram_on_surface_2")
    .def(py::init<>())
    .def(py::init<const Ed&>())
    .def(py::init<const Traits*>())
    ;

  m.def("lower_envelope_3", &dev3::lower_envelope_3);
  m.def("lower_envelope_xy_monotone_3", &dev3::lower_envelope_3);
  m.def("upper_envelope_3", &dev3::upper_envelope_3);
  m.def("upper_envelope_xy_monotone_3", &dev3::upper_envelope_3);
}
