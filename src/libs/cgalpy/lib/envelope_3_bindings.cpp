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
#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_input_iterator.hpp"

void export_env_plane_traits_3(py::module_&);
void export_env_sphere_traits_3(py::module_&);
void export_env_tri_traits_3(py::module_&);

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

void export_envelope_3(py::module_& m) {
  using Ed = env3::Envelope_diagram_2;
  using Edos = env3::Envelope_diagram_on_surface_2;
  using Aos = Edos::Base;
  using Traits = env3::Geometry_traits_3;
  using Srf = env3::Surface_3;

#if CGALPY_ENV3_GEOMETRY_TRAITS == CGALPY_ENV3_PLANE_GEOMETRY_TRAITS
  export_env_plane_traits_3(m);
#elif CGALPY_ENV3_GEOMETRY_TRAITS == CGALPY_ENV3_SPHERE_GEOMETRY_TRAITS
  export_env_sphere_traits_3(m);
#elif CGALPY_ENV3_GEOMETRY_TRAITS == CGALPY_ENV3_TRIANGLE_GEOMETRY_TRAITS
    export_env_tri_traits_3(m);
#else
  BOOST_STATIC_ASSERT_MSG(false, "CGALPY_ENV3_TRAITS");
#endif

  if (! add_attr<Aos>(m, "Arrangement_on_surface_2")) {
    py::class_<Aos> aos_c(m, "Envelope_diagram_on_surface_2");
    aos_c.def(py::init<>())
      .def(py::init<const Aos&>())
      .def(py::init<const Traits*>())
      ;
  }

  if (! add_attr<Edos>(m, "Envelope_diagram_on_surface_2")) {
    py::class_<Edos, Aos> edos_c(m, "Envelope_diagram_on_surface_2");
    edos_c.def(py::init<>())
      .def(py::init<const Edos&>())
      .def(py::init<const Traits*>())
      ;
  }

  if (! add_attr<Ed>(m, "Envelope_diagram_2")) {
    py::class_<Ed, Edos> ed_c(m, "Envelope_diagram_2");
    ed_c.def(py::init<>())
      .def(py::init<const Ed&>())
      .def(py::init<const Traits*>())
      ;
  }

  m.def("lower_envelope_3", &env3::lower_envelope_3);
  m.def("lower_envelope_xy_monotone_3", &env3::lower_envelope_3);
  m.def("upper_envelope_3", &env3::upper_envelope_3);
  m.def("upper_envelope_xy_monotone_3", &env3::upper_envelope_3);
}
