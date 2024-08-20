// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/operators.h>

#include <CGAL/basic.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Bbox_3.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/aff_transformation_tags.h>
#include <CGAL/boost/graph/properties.h>

#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_extraction.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

extern void export_bbox_2(py::class_<CGAL::Bbox_2>& c);
extern void export_bbox_3(py::class_<CGAL::Bbox_3>& c);

template <typename C, typename T>
C export_vertex_incident_patches(C& c, const std::string& name) {
  py::class_<CGAL::vertex_incident_patches_t<T>>(c, ("Vertex_incident_patches_" + name).c_str())
    .def(py::init<>())
    ;
  return c;
}

//
void export_cgal(py::module_& m) {
  using Bbox_2 = CGAL::Bbox_2;
  using Bbox_3 = CGAL::Bbox_3;
  using Rotation = CGAL::Rotation;
  using Scaling = CGAL::Scaling;
  using Translation = CGAL::Translation;
  using Identity_transformation = CGAL::Identity_transformation;
  using Reflection = CGAL::Reflection;

#ifndef CGAL_DATA_DIR
  // #warning "CGAL_DATA_DIR is not defined. The data_file_path function will not be available." // MSVC
#endif
#ifdef CGAL_DATA_DIR
  m.def("data_file_path", &CGAL::data_file_path);
#endif

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

      .value("DEGENERATE", CGAL::DEGENERATE)

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

  if (! add_attr<CGAL::Bounded_side>(m, "Bounded_side")) {
    py::enum_<CGAL::Bounded_side>(m, "Bounded_side")
      .value("ON_UNBOUNDED_SIDE", CGAL::ON_UNBOUNDED_SIDE)
      .value("ON_BOUNDARY", CGAL::ON_BOUNDARY)
      .value("ON_BOUNDED_SIDE", CGAL::ON_BOUNDED_SIDE)
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

  if (! add_attr<Identity_transformation>(m, "Identity_transformation")) {
    py::class_<Identity_transformation>(m, "Identity_transformation")
      .def(py::init<>())
      ;
  }

  if (! add_attr<Reflection>(m, "Reflection")) {
    py::class_<Reflection>(m, "Reflection")
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

  export_vertex_incident_patches<py::module_, int>(m, "int");

  m.attr("ORIGIN") = &CGAL::ORIGIN;
  // m.attr("NULL_VECTOR") = &CGAL::NULL_VECTOR;

  // Colors
  using Color = CGAL::IO::Color;
  if (! add_attr<Color>(m, "Color")) {
    py::class_<Color> col_c(m, "Color");
      col_c.def(py::init<>(),
           "Creates a color with rgba-value (0,0,0,255), i.e. black.")
      .def(py::init<unsigned char, unsigned char, unsigned char, unsigned char>(),
           py::arg("red"), py::arg("green"), py::arg("blue"), py::arg("alpha")=255,
           "Creates a color with rgba-value (red,green,blue,alpha).")
      .def("red", [](const Color& c) { return c.red(); },
           "Returns the red component.")
      .def("green", [](const Color& c) { return c.green(); },
           "Returns the green component.")
      .def("blue", [](const Color& c) { return c.blue(); },
           "Returns the blue component.")
      .def("alpha", [](const Color& c) { return c.alpha(); },
           "Returns the alpha component.")
      .def("set_rgb", &Color::set_rgb,
           py::arg("red"), py::arg("green"), py::arg("blue"), py::arg("alpha")=255,
           "Replaces the rgb values of the colors by the one given as parameters.")
      .def("set_hsv", &Color::set_hsv,
           py::arg("hue"), py::arg("saturation"), py::arg("value"), py::arg("alpha")=255,
           "Replaces the rgb values of the colors by the conversion to rgb of the hsv values given as parameters.")
      .def("__getitem__", [](const Color& c, std::size_t i) { return c[i]; },
           "Returns the ith component of the rgb color (the 0th is red, the 1st is blue, etc).")
      .def("__setitem__", [](Color& c, std::size_t i, unsigned char v) { c[i] = v; },
           "Sets the ith component of the rgb color (the 0th is red, the 1st is blue, etc).")
      .def("to_rgba", [](const Color& c){
        const auto& rgba = c.to_rgba();
        return py::make_tuple(rgba[0], rgba[1], rgba[2], rgba[3]);
      },
           "Returns the array with rgba values.")
      .def("to_rgb", [](const Color& c){
        const auto& rgb = c.to_rgb();
        return py::make_tuple(rgb[0], rgb[1], rgb[2]);
      },
           "Returns the array with rgb values.")
      .def("to_hsv", [](const Color& c){
        const auto& hsv = c.to_hsv();
        return py::make_tuple(hsv[0], hsv[1], hsv[2]);
      },
           "Computes the hsv (hue, saturation, value) values and returns an array representing them as float values between 0 and 1.")
      ;

    add_insertion(col_c, "__str__");
    add_insertion(col_c, "__repr__");
    add_extraction(col_c);
  }

  m.def("black", &CGAL::IO::black, "Constructs Color(0,0,0).");
  m.def("blue", &CGAL::IO::blue, "Constructs Color(0,0,255).");
  m.def("deep_blue", &CGAL::IO::deep_blue, "Constructs Color(10,0,100).");
  m.def("gray", &CGAL::IO::gray, "Constructs Color(100,100,100).");
  m.def("green", &CGAL::IO::green, "Constructs Color(0,255,0).");
  m.def("orange", &CGAL::IO::orange, "Constructs Color(235,150,0).");
  m.def("purple", &CGAL::IO::purple, "Constructs Color(100,0,70).");
  m.def("red", &CGAL::IO::red, "Constructs Color(255,0,0).");
  m.def("violet", &CGAL::IO::violet, "Constructs Color(255,0,255).");
  m.def("white", &CGAL::IO::white, "Constructs Color(255,255,255).");
  m.def("yellow", &CGAL::IO::yellow, "Constructs Color(255,255,0).");

  py::class_<std::type_index>(m, "type_index") // for Point_set_3
    .def(py::self == py::self)
    .def(py::self < py::self)
    .def(py::self > py::self)
    .def(py::self <= py::self)
    .def(py::self >= py::self)
    .def("name", &std::type_index::name,
         "returns implementation defined name of the type,\n"
         "associated with underlying type_info object")
    .def("__hash__", [](const std::type_index& ti) { return ti.hash_code(); })
    .def("__str__", [](const std::type_index& ti) { return ti.name(); })
    .def("__repr__", [](const std::type_index& ti) { return ti.name(); })
    ;

}
