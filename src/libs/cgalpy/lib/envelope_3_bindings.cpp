// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include "CGALPY/envelope_3_types.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/stl_forward_iterator.hpp"
#include "generated/cgalpy/Envelope_3_docstrings.hpp"

namespace py = nanobind;
namespace doc = cgalpy::docstrings::Envelope_3;

// Docstrings
namespace {
  const char* LOWER_ENVELOPE_3_DOC = R"pbdoc(
Compute the lower envelope of a range of 3D surfaces.
)pbdoc";

  const char* LOWER_ENVELOPE_XY_MONOTONE_3_DOC = R"pbdoc(
Compute the lower envelope of a range of xy-monotone 3D surfaces.
)pbdoc";

  const char* UPPER_ENVELOPE_3_DOC = R"pbdoc(
Compute the upper envelope of a range of 3D surfaces.
)pbdoc";

  const char* UPPER_ENVELOPE_XY_MONOTONE_3_DOC = R"pbdoc(
Compute the upper envelope of a range of xy-monotone 3D surfaces.
)pbdoc";
}

namespace env3 {

//
Envelope_diagram_2 lower_envelope_3(const py::list& surfaces) {
  Envelope_diagram_2 ed;
  auto begin = stl_forward_iterator<Surface_3>(surfaces);
  auto end = stl_forward_iterator<Surface_3>(surfaces, false);
  CGAL::lower_envelope_3(begin, end, ed);
  return ed;
}

//
Envelope_diagram_2 lower_envelope_xy_monotone_3(const py::list& surfaces) {
  Envelope_diagram_2 ed;
  auto begin = stl_forward_iterator<Xy_monotone_surface_3>(surfaces);
  auto end = stl_forward_iterator<Xy_monotone_surface_3>(surfaces, false);
  CGAL::lower_envelope_xy_monotone_3(begin, end, ed);
  return ed;
}

//
Envelope_diagram_2 upper_envelope_3(const py::list& surfaces) {
  Envelope_diagram_2 ed;
  auto begin = stl_forward_iterator<Surface_3>(surfaces);
  auto end = stl_forward_iterator<Surface_3>(surfaces, false);
  CGAL::upper_envelope_3(begin, end, ed);
  return ed;
}

//
Envelope_diagram_2 upper_envelope_xy_monotone_3(const py::list& surfaces) {
  Envelope_diagram_2 ed;
  auto begin = stl_forward_iterator<Xy_monotone_surface_3>(surfaces);
  auto end = stl_forward_iterator<Xy_monotone_surface_3>(surfaces, false);
  CGAL::upper_envelope_xy_monotone_3(begin, end, ed);
  return ed;
}

}

void export_envelope_3(py::module_& m) {
  using Dd = CGAL::Dac_decision;
  if (! add_attr<Dd>(m, "Dac_decision")) {
    py::enum_<Dd>(m, "Dac_decision")
      .value("DAC_DECISION_FIRST", CGAL::DAC_DECISION_FIRST)
      .value("DAC_DECISION_BOTH", CGAL::DAC_DECISION_BOTH)
      .value("DAC_DECISION_SECOND", CGAL::DAC_DECISION_SECOND)
      .value("DAC_DECISION_NOT_SET", CGAL::DAC_DECISION_NOT_SET)
      ;
  }

  using Ed = env3::Envelope_diagram_2;
  using Edos = env3::Envelope_diagram_on_surface_2;
  using Aos = Edos::Base;
  using Traits = env3::Geometry_traits_3;
  using Srf = env3::Surface_3;

  if (! add_attr<Aos>(m, "Arrangement_on_surface_2")) {
    PyErr_SetString(PyExc_StopIteration,
                    "Arrangement_on_surface_2 hasn't been registered");
    py::python_error();
  }

  if (! add_attr<Edos>(m, "Envelope_diagram_on_surface_2")) {
    py::class_<Edos, Aos> edos_c(m, "Envelope_diagram_on_surface_2");
    edos_c.def(py::init<>())
      .def(py::init<const Edos&>(), py::arg("other"))
      .def(py::init<const Traits*>(), py::arg("traits"))
      ;
  }

  if (! add_attr<Ed>(m, "Envelope_diagram_2")) {
    py::class_<Ed, Edos> ed_c(m, "Envelope_diagram_2", doc::CGAL_Envelope_diagram_2__class__);
    ed_c.def(py::init<>(), doc::CGAL_Envelope_diagram_2_Envelope_diagram_2)
      .def(py::init<const Ed&>(), py::arg("other"),
           doc::CGAL_Envelope_diagram_2_Envelope_diagram_2_1)
      .def(py::init<const Traits*>(), py::arg("traits"))
      ;
  }

  m.def("lower_envelope_3", &env3::lower_envelope_3,
        LOWER_ENVELOPE_3_DOC,
        py::arg("surfaces"));
  m.def("lower_envelope_xy_monotone_3", &env3::lower_envelope_xy_monotone_3,
        LOWER_ENVELOPE_XY_MONOTONE_3_DOC,
        py::arg("surfaces"));
  m.def("upper_envelope_3", &env3::upper_envelope_3,
        UPPER_ENVELOPE_3_DOC,
        py::arg("surfaces"));
  m.def("upper_envelope_xy_monotone_3", &env3::upper_envelope_xy_monotone_3,
        UPPER_ENVELOPE_XY_MONOTONE_3_DOC,
        py::arg("surfaces"));
}
