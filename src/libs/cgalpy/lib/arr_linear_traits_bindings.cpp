// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// This file is private property of Tel Aviv University.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/arrangement_2_concepts/export_ArrangementTraits_2.hpp"
#include "CGALPY/arrangement_2_concepts/export_ArrangementLandmarkTraits_2.hpp"
#include "CGALPY/arrangement_2_concepts/export_ArrangementOpenBoundaryTraits_2.hpp"

void set_left(Curve_2& c, Point_2& p) { c.set_left(p); }
void set_right(Curve_2& c, Point_2& p) { c.set_right(p); }

bp::object export_arr_linear_traits() {
  auto traits = bp::class_<Traits>("Geometry_traits_2");
  bp::scope traits_scope(traits);
  export_ArrangementTraits_2<Traits, bp::return_value_policy<bp::copy_const_reference>>(traits);
  export_ArrangementLandmarkTraits_2<Traits, bp::return_value_policy<bp::copy_const_reference>>(traits);
  export_ArrangementOpenBoundaryTraits_2<Traits, bp::return_value_policy<bp::copy_const_reference>>(traits);

  bp::class_<Curve_2>("Curve_2")
    .def(bp::init<>())
    .def(bp::init<Segment_2&>())
    .def(bp::init<Ray_2&>())
    .def(bp::init<Line_2&>())
    .def("source", &Curve_2::source, bp::return_value_policy<bp::copy_const_reference>())
    .def("target", &Curve_2::target, bp::return_value_policy<bp::copy_const_reference>())
    .def("line", &Curve_2::line)
    .def("is_vertical", &Curve_2::is_vertical)
    .def("is_segment", &Curve_2::is_segment)
    .def("segment", &Curve_2::segment)
    .def("is_ray", &Curve_2::ray)
    .def("is_line", &Curve_2::is_line)
    .def("line", &Curve_2::line)
    .def("supporting_line", &Curve_2::supporting_line, bp::return_value_policy<bp::copy_const_reference>())
    .def("left", &Curve_2::left, bp::return_value_policy<bp::copy_const_reference>())
    .def("right", &Curve_2::right, bp::return_value_policy<bp::copy_const_reference>())
    .def<void (Curve_2::*)()>("set_left", &Curve_2::set_left)
    .def("set_left", set_left)
    .def<void (Curve_2::*)()>("set_right", &Curve_2::set_right)
    .def("set_right", set_right)
    .def("is_directed_right", &Curve_2::is_directed_right)
    .def("is_in_x_range", &Curve_2::is_in_x_range)
    .def("is_in_y_range", &Curve_2::is_in_y_range)
    .def("is_degenerate", &Curve_2::is_degenerate)
    .def("bbox", &Curve_2::bbox)
    .def(bp::self_ns::str(bp::self_ns::self))
    .def(bp::self_ns::repr(bp::self_ns::self))
    ;

  return traits;
}
