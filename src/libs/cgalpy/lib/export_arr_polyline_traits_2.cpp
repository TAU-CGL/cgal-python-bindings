// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Arr_polycurve_basic_traits_2.h>
#include <CGAL/Arr_polycurve_traits_2.h>
#include <CGAL/Arr_polyline_traits_2.h>

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
#include "CGALPY/stl_forward_iterator.hpp"

namespace py = nanobind;

template <typename Traits_>
typename Traits_::Curve_2
ctr_cv(const typename Traits_::Construct_curve_2& ctr, py::list& lst) {
  using Pnt = typename Traits_::Point_2;
  if (py::isinstance<Pnt>(lst[0])) {
    auto begin = stl_forward_iterator<Pnt>(lst);
    auto end = stl_forward_iterator<Pnt>(lst, false);
    //! \todo For some reason this doesn't work together with the zip iterator
    // used in ctr(begin, end)
    // return ctr(begin, end);
    std::vector<Pnt> points;
    points.reserve(lst.size());
    for (auto it = begin; it != end; ++it) points.push_back(*it);
    return ctr(points.begin(), points.end());
  }

  using Seg = typename Traits_::Segment_2;
  if (py::isinstance<Seg>(lst[0])) {
    auto begin = stl_forward_iterator<Seg>(lst);
    auto end = stl_forward_iterator<Seg>(lst, false);
    return ctr(begin, end);
  }

  throw std::runtime_error("Attempting to construct a polyline curve from a list of object of unrecognized type!");}

//
void export_arr_polyline_traits_2(py::module_& m) {
  using Sgt = CGAL::Arr_segment_traits_2<Kernel>;
  using Pcbgt = CGAL::Arr_polycurve_basic_traits_2<Sgt>;
  using Pcgt = CGAL::Arr_polycurve_traits_2<Sgt>;
  using Plgt = CGAL::Arr_polyline_traits_2<Sgt>;
  using Pnt = Plgt::Point_2;
  using Cv = Plgt::Curve_2;
  using Xcv = Plgt::X_monotone_curve_2;
  using Seg = Plgt::Segment_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  struct Concepts {
    Aos_basic_traits_classes<Pcbgt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Pcgt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Pcgt> m_aos_traits_2_classes;
    Aos_landmark_traits_classes<Pcgt> m_aos_landmark_traits_2_classes;
    Aos_directional_x_monotone_traits_classes<Pcgt> m_aos_directional_x_monotone_traits_2_classes;
    Aos_approximate_traits_classes<Pcgt> m_aos_approximate_traits_2_classes;
    Aos_construct_x_monotone_curve_traits_classes<Plgt>
      m_aos_construct_x_monotone_curve_traits_2_classes;
  } concepts;

  if (! add_attr<Pcbgt>(m, "Arr_polycurve_basic_traits_2")) {
    py::class_<Pcbgt> pcbgt_c(m, "Arr_polycurve_basic_traits_2");
    export_AosBasicTraits_2<Pcbgt>(pcbgt_c, concepts);
    export_AosApproximateTraits_2<Pcbgt>(pcbgt_c, concepts);
  }

  if (! add_attr<Pcgt>(m, "Arr_polycurve_traits_2")) {
    py::class_<Pcgt, Pcbgt> pcgt_c(m, "Arr_polycurve_traits_2");
    export_AosTraits_2<Pcgt>(pcgt_c, concepts);
    export_AosDirectionalXMonotoneTraits_2<Pcgt>(pcgt_c, concepts);
    //! \todo add Push_back_2, Push_fron_2, Construct_curve_2
  }

  if (add_attr<Plgt>(m, "Arr_polyline_traits_2")) return;
  py::class_<Plgt, Pcgt> plgt_c(m, "Arr_polyline_traits_2");
  export_AosConstructXMonotoneCurveTraits_2<Plgt>(plgt_c, concepts);

  // The sub curve type
  add_attr<Seg>(plgt_c, "Segment_2");

  // Exporter & importer for the x-monotone polyline curve
  // auto& xcv_c = *(concepts.m_aos_basic_traits_2_classes.m_x_monotone_curve_2);
  // add_insertion(xcv_c, "__str__");
  // add_insertion(xcv_c, "__repr__");
  // add_extraction(xcv_c);

  // Export additional curve attributes:
  using Ctr_cv = Plgt::Construct_curve_2;
  py::class_<Ctr_cv>(plgt_c, "Construct_curve_2")
    .def(py::init<const Plgt&>())
    .def("__call__", &ctr_cv<Plgt>)
    ;

  plgt_c.def(py::init<>())
    .def("construct_curve_2_object", &Plgt::construct_curve_2_object)
    ;
}
