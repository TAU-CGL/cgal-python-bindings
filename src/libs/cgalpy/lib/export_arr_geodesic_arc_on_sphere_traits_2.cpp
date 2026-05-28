// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_geodesic_arc_on_sphere_traits_2.h>

#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosSphericalBoundaryTraits_2.hpp"

#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_construct_x_monotone_curve_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_spherical_boundary_traits_classes.hpp"
#include "CGALPY/add_attr.hpp"
#include "CGALPY/add_insertion.hpp"
#include "CGALPY/add_extraction.hpp"

namespace py = nanobind;

void export_arr_geodesic_arc_on_sphere_traits_2(py::module_& m) {
  using Gt = CGAL::Arr_geodesic_arc_on_sphere_traits_2<Kernel>;
  using Pnt = typename Gt::Point_2;
  using Cv = typename Gt::Curve_2;
  using Xcv = typename Gt::X_monotone_curve_2;
  using Dir = Kernel::Direction_3;
  constexpr auto ri(py::rv_policy::reference_internal);

  if (add_attr<Gt>(m, "Arr_geodesic_arc_on_sphere_traits_2")) return;

  py::class_<Gt, Kernel> traits_c(m, "Arr_geodesic_arc_on_sphere_traits_2");
  traits_c.def(py::init<>())
    .def("construct_point_2_object", &Gt::construct_point_2_object)
    .def("construct_x_monotone_segment_2_object", &Gt::construct_x_monotone_curve_2_object)
    .def("construct_curve_2_object", &Gt::construct_curve_2_object)
    ;

  struct Concepts {
    Aos_basic_traits_classes<Gt, py::class_<Pnt, Direction_3>> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Gt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Gt> m_aos_traits_2_classes;
    Aos_spherical_boundary_traits_classes<Gt> m_aos_spherical_boundary_traits_2_classes;
    Aos_directional_x_monotone_traits_classes<Gt> m_aos_directional_x_monotone_traits_2_classes;
  } concepts;

  if (! add_attr<Pnt>(traits_c, "Point_2")) {
    concepts.m_aos_basic_traits_2_classes.m_point_2 = new py::class_<Pnt, Direction_3>(traits_c, "Point_2");
    concepts.m_aos_basic_traits_2_classes.m_point_2->def(py::init<>());
    concepts.m_aos_basic_traits_2_classes.m_point_2->def(py::init<const Pnt&>());
  }

  export_AosTraits_2<Gt>(traits_c, concepts);
  export_AosDirectionalXMonotoneTraits_2<Gt>(traits_c, concepts);
  export_AosSphericalBoundaryTraits_2<Gt>(traits_c, concepts);

  using Ctr_pnt = Gt::Construct_point_2;
  using Ctr_pnt_op1 = Pnt(Ctr_pnt::*)(const FT&, const FT&, const FT&);
  using Ctr_pnt_op2 = Pnt(Ctr_pnt::*)(const Dir&);
  py::class_<Ctr_pnt>(traits_c, "Construct_point_2")
    .def("__call__", static_cast<Ctr_pnt_op1>(&Ctr_pnt::operator()))
    .def("__call__", static_cast<Ctr_pnt_op2>(&Ctr_pnt::operator()))
  ;

  using Ctr_cv = Gt::Construct_curve_2;
  using Ctr_cv_op = Cv(Ctr_cv::*)(const Pnt&, const Pnt&);
  py::class_<Ctr_cv>(traits_c, "Construct_curve_2")
    .def("__call__", static_cast<Ctr_cv_op>(&Ctr_cv::operator()));
  ;

  using Ctr_xcv = Gt::Construct_x_monotone_curve_2;
  using Ctr_xcv_op = Xcv(Ctr_xcv::*)(const Pnt&, const Pnt&)const;
  py::class_<Ctr_xcv>(traits_c, "Construct_x_monotone_curve_2")
    .def("__call__", static_cast<Ctr_xcv_op>(&Ctr_xcv::operator()));
  ;

  //! \todo Handle the functions that return reference-counted objects.
  auto& xcv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
  xcv_c
    .def("source", &cgalpy::aos2::X_monotone_curve_2::source, ri)
    .def("target", &cgalpy::aos2::X_monotone_curve_2::target, ri)
    .def("normal", &cgalpy::aos2::X_monotone_curve_2::normal, ri)
    .def("left", &cgalpy::aos2::X_monotone_curve_2::left, ri)
    .def("right", &cgalpy::aos2::X_monotone_curve_2::right, ri)
    .def("is_vertical", &cgalpy::aos2::X_monotone_curve_2::is_vertical)
    .def("is_directed_right", &cgalpy::aos2::X_monotone_curve_2::is_directed_right)
    .def("is_meridian", &cgalpy::aos2::X_monotone_curve_2::is_meridian)
    ;
  add_insertion(xcv_c, "__str__");
  add_insertion(xcv_c, "__repr__");
  add_extraction(xcv_c);

  auto& pnt_c = *(concepts.m_aos_basic_traits_2_classes.m_point_2);
  add_insertion(pnt_c, "__str__");
  add_insertion(pnt_c, "__repr__");
  add_extraction(pnt_c);
}
