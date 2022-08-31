// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_segment_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosLandmarkTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_landmark_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

namespace aos2 {

//
Segment_2 to_segment(X_monotone_curve_2& c) { return Segment_2(c); }

}

//
py::object export_arr_segment_traits(py::module_& m) {
  using GT = CGAL::Arr_segment_traits_2<Kernel>;
  using Pnt = GT::Point_2;
  using Xcv = aos2::X_monotone_curve_2;

  py::class_<GT> traits_c(m, "Arr_segment_traits_2");
  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_landmark_traits_classes<GT> m_landmark_traits_classes;
    Aos_directional_x_monotone_traits_classes<GT> m_directional_x_monotone_traits_classes;
    Aos_approximate_traits_classes<GT> m_approximate_traits_classes;
    Aos_construct_x_monotone_curve_traits_classes<GT>
      m_construct_x_monotone_curve_traits_classes;
  } concepts;
  export_AosTraits_2<GT>(traits_c, concepts);
  export_AosLandmarkTraits_2<GT>(traits_c, concepts);
  export_AosDirectionalXMonotoneTraits_2<GT>(traits_c, concepts);
  traits_c
    .def("is_in_x_range_2_object", &GT::is_in_x_range_2_object)
    .def("is_in_y_range_2_object", &GT::is_in_y_range_2_object)
    ;

  auto& xcv_c = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  xcv_c.def(py::init<Segment_2&>())
    .def(py::init<Pnt&, Pnt&>())
    .def(py::init<Line_2&, Pnt&, Pnt&>())
    .def("source", &Xcv::source)
    .def("target", &Xcv::target)
    .def("line", &Xcv::line)
    .def("is_vertical", &Xcv::is_vertical)
    .def("flip", &Xcv::flip)
    .def("left", &Xcv::left)
    .def("right", &Xcv::right)
    .def("set_left", &Xcv::set_left)
    .def("set_right", &Xcv::set_right)
    .def("is_directed_right", &Xcv::is_directed_right)

    // Deprecated
    // .def("is_in_x_range", &X_monotone_curve_2::is_in_x_range)
    // .def("is_in_y_range", &X_monotone_curve_2::is_in_y_range)

    .def("bbox", &Xcv::bbox)
    .def("segment", &aos2::to_segment)
  ;

  add_insertion(xcv_c, "__str__");
  add_insertion(xcv_c, "__repr__");

  py::class_<GT::Is_in_x_range_2>(traits_c, "Is_in_x_range_2")
    .def("__call__", &GT::Is_in_x_range_2::operator());
  ;

  py::class_<GT::Is_in_y_range_2>(traits_c, "Is_in_y_range_2")
    .def("__call__", &GT::Is_in_y_range_2::operator());
  ;

  return traits_c;
}
