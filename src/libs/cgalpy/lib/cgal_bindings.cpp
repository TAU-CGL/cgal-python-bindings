// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/basic.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/boost/graph/helpers.h>

#include "CGALPY/add_attr.hpp"

namespace py = nanobind;

extern void export_bbox_2(py::class_<CGAL::Bbox_2>& c);
extern void export_bbox_3(py::class_<CGAL::Bbox_3>& c);

//
void export_cgal(py::module_& m) {
  using Bbox_2 = CGAL::Bbox_2;
  using Bbox_3 = CGAL::Bbox_3;
  using Rotation = CGAL::Rotation;
  using Scaling = CGAL::Scaling;
  using Translation = CGAL::Translation;

  if (! add_attr<CGAL::Sign>(m, "Result")) {
    py::enum_<CGAL::Sign>(m, "Result")

      //CGAL::Sign
      .value("NEGATIVE", CGAL::NEGATIVE)
      .value("ZERO", CGAL::ZERO)
      .value("POSITIVE", CGAL::POSITIVE)

      //CGAL::Comparison_result
      .value("SMALLER", CGAL::SMALLER)
      .value("EQUAL", CGAL::EQUAL)
      .value("LARGER", CGAL::LARGER)

      //CGAL::Oriented_side
      .value("ON_NEGATIVE_SIDE", CGAL::ON_NEGATIVE_SIDE)
      .value("ON_ORIENTED_BOUNDARY", CGAL::ON_ORIENTED_BOUNDARY)
      .value("ON_POSITIVE_SIDE", CGAL::ON_POSITIVE_SIDE)

      //CGAL::Orientation
      .value("LEFT_TURN", CGAL::LEFT_TURN)
      .value("RIGHT_TURN", CGAL::RIGHT_TURN)
      .value("COLLINEAR", CGAL::COLLINEAR)
      .value("CLOCKWISE", CGAL::CLOCKWISE)
      .value("COUNTERCLOCKWISE", CGAL::COUNTERCLOCKWISE)
      .value("COPLANAR", CGAL::COPLANAR)
      .export_values()
      ;
  }

  if (! add_attr<CGAL::Origin>(m, "Origin")) {
    py::class_<CGAL::Origin>(m, "Origin")
      .def(py::init<>())
      ;
  }

  if (! add_attr<CGAL::Angle>(m, "Angle")) {
    py::enum_<CGAL::Angle>(m, "Angle")
      .value("OBTUSE", CGAL::OBTUSE)
      .value("RIGHT", CGAL::RIGHT)
      .value("ACUTE", CGAL::ACUTE)
      .export_values()
      ;
  }

  if (! add_attr<Rotation>(m, "Rotation")) {
    py::class_<Rotation>(m, "Rotation")
      .def(py::init<>())
      ;
  }

  if (! add_attr<Scaling>(m, "Scaling")) {
    py::class_<Scaling>(m, "Scaling")
      .def(py::init<>())
      ;
  }

  if (! add_attr<Translation>(m, "Translation")) {
    py::class_<Translation>(m, "Translation")
      .def(py::init<>())
      ;
  }

  // Bbox_2
  if (! add_attr<CGAL::Bbox_2>(m, "Bbox_2")) {
    py::class_<CGAL::Bbox_2> bbox_c(m, "Bbox_2");
    export_bbox_2(bbox_c);
  }

  // Bbox_3
  if (! add_attr<CGAL::Bbox_3>(m, "Bbox_3")) {
    py::class_<CGAL::Bbox_3> bbox_c(m, "Bbox_3");
    export_bbox_3(bbox_c);
  }

  // Box_parameter_space_2
  if (! add_attr<CGAL::Box_parameter_space_2>(m, "Box_parameter_space_2")) {
    py::enum_<CGAL::Box_parameter_space_2>(m, "Box_parameter_space_2")
      .value("LEFT_BOUNDARY", CGAL::LEFT_BOUNDARY)
      .value("RIGHT_BOUNDARY", CGAL::RIGHT_BOUNDARY)
      .value("BOTTOM_BOUNDARY", CGAL::BOTTOM_BOUNDARY)
      .value("TOP_BOUNDARY", CGAL::TOP_BOUNDARY)
      .value("INTERIOR", CGAL::INTERIOR)
      .value("EXTERIOR", CGAL::EXTERIOR)
      .export_values()
      ;
  }

  m.attr("ORIGIN") = &CGAL::ORIGIN;
  // m.attr("NULL_VECTOR") = &CGAL::NULL_VECTOR;
}
