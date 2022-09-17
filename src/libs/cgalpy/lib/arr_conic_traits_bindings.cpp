// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_conic_traits_2.h>
#include <CGAL/Cartesian.h>
#include <CGAL/CORE_algebraic_number_traits.h>

#include "CGALPY/kernel_types.hpp"
#include "CGALPY/arrangement_on_surface_2_types.hpp"
#include "CGALPY/aos_2_concepts/export_AosTraits_2.hpp"
#include "CGALPY/aos_2_concepts/export_AosDirectionalXMonotoneTraits_2.hpp"
#include "CGALPY/aos_2_concepts/Aos_basic_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_x_monotone_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_traits_classes.hpp"
#include "CGALPY/aos_2_concepts/Aos_directional_x_monotone_traits_classes.hpp"
#include "CGALPY/Kernel/export_ft.hpp"
#include "CGALPY/Kernel/export_point_2.hpp"
#include "CGALPY/Kernel/export_segment_2.hpp"
#include "CGALPY/Kernel/export_circle_2.hpp"
#include "CGALPY/add_insertion.hpp"

namespace py = nanobind;

py::object export_arr_conic_traits(py::module_& m) {
  //TODO export RatKernel, AlgKernel
  using Nt_traits = CGAL::CORE_algebraic_number_traits;
  using Rat_kernel = CGAL::Cartesian <Nt_traits::Rational>;
  using Alg_kernel = CGAL::Cartesian <Nt_traits::Algebraic>;
  using GT = CGAL::Arr_conic_traits_2<Rat_kernel, Alg_kernel, Nt_traits>;
  using Pnt = GT::Point_2;
  using Cv = GT::Curve_2;
  using Xcv = GT::X_monotone_curve_2;
  using Integer = Nt_traits::Integer;
  using Rational = Nt_traits::Rational;
  using Algebraic = Alg_kernel::FT;
  using Rat_pnt = Rat_kernel::Point_2;
  using Rat_seg = Rat_kernel::Segment_2;
  using Rat_circle = Rat_kernel::Circle_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<GT> traits_c(m, "Arr_conic_traits_2");
  traits_c.def(py::init<>());

  struct Concepts {
    Aos_basic_traits_classes<GT> m_basic_traits_classes;
    Aos_x_monotone_traits_classes<GT> m_x_monotone_traits_classes;
    Aos_traits_classes<GT> m_traits_classes;
    Aos_directional_x_monotone_traits_classes<GT>
      m_directional_x_monotone_traits_classes;
  } concepts;

  if (! add_attr<Integer>(traits_c, "Integer")) {
    py::class_<Integer> int_c(traits_c, "Integer");
    int_c.def(py::init<const Integer&>())
      .def(py::init_implicit<int>())
      ;

    add_insertion(int_c, "__str__");
    add_insertion(int_c, "__repr__");
}

  if (! add_attr<Rational>(traits_c, "Rational")) {
    py::class_<Rational> rat_c(traits_c, "Rational");
    export_ft(rat_c);
    rat_c.def(py::init_implicit<Integer>())
      .def(py::init<const Integer&, const Integer&>())
      ;

    add_insertion(rat_c, "__str__");
    add_insertion(rat_c, "__repr__");
  }

  if (! add_attr<Algebraic>(traits_c, "Algebraic")) {
    py::class_<Algebraic> alg_c(traits_c, "Algebraic");
    alg_c.def(py::init_implicit<double>())
      .def(py::init_implicit<Rational>())
      ;

    add_insertion(alg_c, "__str__");
    add_insertion(alg_c, "__repr__");
  }

  if (! add_attr<Rat_pnt>(traits_c, "Rat_point_2")) {
    py::class_<Rat_pnt> rat_pnt_c(traits_c, "Rat_point_2");
    export_point_2<Rat_kernel>(rat_pnt_c);
  }

  if (! add_attr<Rat_seg>(traits_c, "Rat_segment_2")) {
    py::class_<Rat_seg> rat_seg_c(traits_c, "Rat_segment_2");
    export_segment_2<Rat_kernel>(rat_seg_c);
  }

  if (! add_attr<Rat_circle>(traits_c, "Rat_circle_2")) {
    py::class_<Rat_circle> rat_circle_c(traits_c, "Rat_circle_2");
    export_circle_2<Rat_kernel>(rat_circle_c);
  }

  if (! add_attr<Pnt>(traits_c, "Point_2")) {
    concepts.m_basic_traits_classes.m_point_2 =
      new py::class_<Pnt>(traits_c, "Point_2");
    auto& pnt_c = *(concepts.m_basic_traits_classes.m_point_2);
    pnt_c.def(py::init<const Algebraic&, const Algebraic&>())
      .def(py::init<const Algebraic&, const Algebraic&, const Algebraic&>())
      ;

    add_insertion(pnt_c, "__str__");
    add_insertion(pnt_c, "__repr__");
  }

  // Export Curve_2 and X_monotone_curve_2 (which derives from Curve_2)
  // explicitly here, instead of exporting them in export_AosTraits_2 and
  // aos_TraitsBasicTraitse_2, since X_monotone_curve_2 derives from Curve_2.
  // Export Cv:
  if (! add_attr<Cv>(traits_c, "Curve_2")) {
    concepts.m_traits_classes.m_curve_2 =
      new py::class_<Cv>(traits_c, "Curve_2");
    auto& cv_c = *(concepts.m_traits_classes.m_curve_2);
    cv_c.def(py::init<const Pnt&, const Pnt&>())
      .def(py::init<const Rat_seg&>())
      .def(py::init<const Rat_circle&>())
      .def(py::init<const Rat_circle&, CGAL::Orientation,
           const Pnt&, const Pnt&>())
      .def(py::init<const Rat_pnt&, const Rat_pnt&, const Rat_pnt&>())
      .def(py::init<const Rat_pnt&, const Rat_pnt&, const Rat_pnt&,
           const Rat_pnt&, const Rat_pnt&>())
      .def(py::init<const Rational&, const Rational&, const Rational&,
           const Rational&, const Rational&, const Rational&>())
      .def(py::init<const Rational&, const Rational&, const Rational&,
           const Rational&, const Rational&, const Rational&,
           CGAL::Orientation, const Pnt&, const Pnt&>())
      .def(py::init<Rational, Rational, Rational, Rational, Rational, Rational,
           CGAL::Orientation, const Pnt&,
           Rational, Rational, Rational, Rational, Rational, Rational,
           const Pnt&,
           Rational, Rational, Rational, Rational, Rational, Rational>())
      .def("is_valid", &Cv::is_valid)
      .def("r", &Cv::r, ri)
      .def("s", &Cv::s, ri)
      .def("t", &Cv::t, ri)
      .def("u", &Cv::u, ri)
      .def("v", &Cv::v, ri)
      .def("w", &Cv::w, ri)
      .def("is_x_monotone", &Cv::is_x_monotone)
      .def("is_y_monotone", &Cv::is_y_monotone)
      .def("is_full_conic", &Cv::is_full_conic)
      .def("source", &Cv::source, ri)
      .def("target", &Cv::target, ri)
      .def("orientation", &Cv::orientation)
      .def("bbox", &Cv::bbox)
      .def("set_source", &Cv::set_source)
      .def("set_target", &Cv::set_target)
      ;

    add_insertion(cv_c, "__str__");
    add_insertion(cv_c, "__repr__");
  }

  // Export X_monotone_curve_2:
  if (! add_attr<Xcv>(traits_c, "X_monotone_curve_2")) {
    // The inheritance Curve_2 - X_monotone_curve_2 is currently private:
    concepts.m_basic_traits_classes.m_x_monotone_curve_2 =
      // new py::class_<Xcv, Cv>(traits_c, "X_monotone_curve_2");
      new py::class_<Xcv>(traits_c, "X_monotone_curve_2");
    auto& xcv_c = *(concepts.m_basic_traits_classes.m_x_monotone_curve_2);
    xcv_c.def(py::init<>())
      .def(py::init<const Xcv&>())
      .def(py::init<const Pnt&, const Pnt&>())
      .def(py::init<const Algebraic&, const Algebraic&, const Algebraic&,
           const Pnt&, const Pnt&>())
      .def("r", &Xcv::r, ri)
      .def("s", &Xcv::s, ri)
      .def("t", &Xcv::t, ri)
      .def("u", &Xcv::u, ri)
      .def("v", &Xcv::v, ri)
      .def("w", &Xcv::w, ri)
      .def("left", &Xcv::left, ri)
      .def("right", &Xcv::right, ri)
      .def("is_directed_right", &Xcv::is_directed_right, ri)
      ;

    add_insertion(xcv_c, "__str__");
    add_insertion(xcv_c, "__repr__");
  }

  // Export additional concepts
  export_AosTraits_2<GT>(traits_c, concepts);
  export_AosDirectionalXMonotoneTraits_2<GT>(traits_c, concepts);

  // Convenient attributes
  add_attr<Integer>(traits_c, "Integer");
  add_attr<Rational>(traits_c, "Rational");
  add_attr<Algebraic>(traits_c, "Algebraic");

  return traits_c;
}
