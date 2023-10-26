// Copyright (c) 2023 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: LGPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_linear_traits_2.h>
#include <CGAL/Env_plane_traits_3.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/envelope_3_types.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_landmark_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_open_boundary_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_approximate_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"

#include "CGALPY/env_3_concepts/Env_traits_classes.hpp"
#include "CGALPY/env_3_concepts/export_EnvelopeTraits_3.hpp"
#include "CGALPY/Kernel/export_ft.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"
#include "CGALPY/Kernel/export_circle_2.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

void export_env_plane_traits_3(py::module_& m) {
  using Aos2_gt = CGAL::Arr_linear_traits_2<Kernel>;
  using Gt = CGAL::Env_plane_traits_3<Kernel>;
  // constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Gt>(m, "Env_plane_traits_3")) return;

  py::class_<Gt, Aos2_gt> traits_c(m, "Env_plane_traits_3");

  struct Concepts {
    Aos_basic_traits_classes<Gt> m_aos_basic_traits_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_classes;
    Aos_traits_classes<Gt> m_aos_traits_classes;
    Aos_landmark_traits_classes<Gt> m_aos_landmark_traits_classes;
    Aos_open_boundary_traits_classes<Gt> m_aos_open_boundary_traits_classes;
    Aos_approximate_traits_classes<Gt> m_aos_approximate_traits_classes;
    Aos_construct_x_monotone_curve_traits_classes<Gt>
      m_aos_construct_x_monotone_curve_traits_classes;
    Env_traits_classes<Gt> m_env_traits_classes
  };
  Concepts concepts;
  export_AosTraits_2<Gt>(traits_c, concepts);
  export_AosLandmarkTraits_2<Gt>(traits_c, concepts);
  export_AosOpenBoundaryTraits_2<Gt>(traits_c, concepts);

  // auto& cv_c = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
  // cv_c.def(py::init_implicit<const Segment&>())
  //   .def(py::init_implicit<const Ray&>())
  //   .def(py::init_implicit<const Line&>())
  //   .def(py::init<const Pnt&, const Pnt&>())
  //   .def("source", &Cv::source, ri)
  //   .def("target", &Cv::target, ri)
  //   .def("is_segment", &Cv::is_segment)
  //   .def("segment", &Cv::segment)
  //   .def("is_ray", &Cv::ray)
  //   .def("ray", &Cv::ray)
  //   .def("is_line", &Cv::is_line)
  //   .def("line", &Cv::line)
  //   .def("supporting_line", &Cv::supporting_line, ri)
  //   .def("is_vertical", &Cv::is_vertical)
  //   .def("left", &Cv::left, ri)
  //   .def("right", &Cv::right, ri)
  //   .def("set_left", static_cast<overload>(&Cv::set_left))
  //   .def("set_left", set_left)
  //   .def("set_right", static_cast<overload>(&Cv::set_right))
  //   .def("set_right", set_right)
  //   .def("is_directed_right", &Cv::is_directed_right)
  //   .def("is_in_x_range", &Cv::is_in_x_range)
  //   .def("is_in_y_range", &Cv::is_in_y_range)
  //   .def("is_degenerate", &Cv::is_degenerate)
  //   .def("bbox", &Cv::bbox)
  //   ;

  // add_insertion(cv_c, "__str__");
  // add_insertion(cv_c, "__repr__");
}
