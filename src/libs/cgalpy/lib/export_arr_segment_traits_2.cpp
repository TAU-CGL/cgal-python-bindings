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
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

namespace aos2 {

//
Segment_2 to_segment(CGAL::Arr_segment_traits_2<Kernel>::X_monotone_curve_2& c)
{ return Segment_2(c); }

}

//
void export_arr_segment_traits_2(py::module_& m) {
  using Gt = CGAL::Arr_segment_traits_2<Kernel>;
  using Pnt = Gt::Point_2;
  using Lin = Gt::Line_2;
  using Xcv = Gt::X_monotone_curve_2;

  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Gt>(m, "Arr_segment_traits_2")) return;

  py::class_<Gt, Kernel> traits_c(m, "Arr_segment_traits_2");
  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
    Aos_landmark_traits_classes<Gt> m_aos_landmark_traits_2_classes;
    Aos_directional_x_monotone_traits_classes<Gt> m_aos_directional_x_monotone_traits_2_classes;
    Aos_approximate_traits_classes<Gt> m_aos_approximate_traits_2_classes;
    Aos_construct_x_monotone_curve_traits_classes<Gt>
      m_aos_construct_x_monotone_curve_traits_2_classes;
  } concepts;
  export_AosTraits_2<Gt>(traits_c, concepts);
  export_AosLandmarkTraits_2<Gt>(traits_c, concepts);
  export_AosDirectionalXMonotoneTraits_2<Gt>(traits_c, concepts);
  traits_c
    .def("is_in_x_range_2_object", &Gt::is_in_x_range_2_object)
    .def("is_in_y_range_2_object", &Gt::is_in_y_range_2_object)
    ;

  auto& xcv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
  xcv_c.def(py::init_implicit<Segment_2&>())
    .def(py::init<Pnt&, Pnt&>())
    .def(py::init<Line_2&, Pnt&, Pnt&>())
    .def("flip", &Xcv::flip)
    .def("bbox", &Xcv::bbox)

    // Members defined in the base class:
    .def("source", [](const Xcv& xcv)->const Pnt& { return xcv.source(); }, ri)
    .def("target", [](const Xcv& xcv)->const Pnt& { return xcv.target(); }, ri)
    .def("line", [](const Xcv& xcv)->const Lin& { return xcv.line(); }, ri)
    .def("is_vertical", [](const Xcv& xcv)->bool { return xcv.is_vertical(); })
    .def("left", [](const Xcv& xcv)->const Pnt& { return xcv.left(); }, ri)
    .def("right", [](const Xcv& xcv)->const Pnt& { return xcv.right(); }, ri)
    .def("set_left",
         [](Xcv& xcv, const Pnt& p)->void { return xcv.set_left(p); })
    .def("set_right",
         [](Xcv& xcv, const Pnt& p)->void { return xcv.set_right(p); })
    .def("is_directed_right",
         [](const Xcv& xcv)->bool { return xcv.is_directed_right(); })

    // Deprecated
    // .def("is_in_x_range", &Xcv::is_in_x_range)
    // .def("is_in_y_range", &Xcv::is_in_y_range)

    .def("segment", &aos2::to_segment)
  ;

  add_insertion(xcv_c, "__str__");
  add_insertion(xcv_c, "__repr__");
  add_extraction(xcv_c);

  py::class_<Gt::Is_in_x_range_2>(traits_c, "Is_in_x_range_2")
    .def("__call__", &Gt::Is_in_x_range_2::operator());
  ;

  py::class_<Gt::Is_in_y_range_2>(traits_c, "Is_in_y_range_2")
    .def("__call__", &Gt::Is_in_y_range_2::operator());
  ;
}
