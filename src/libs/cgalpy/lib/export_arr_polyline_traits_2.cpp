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

namespace py = nanobind;

//
void export_arr_polyline_traits_2(py::module_& m) {
  using Sgt = CGAL::Arr_segment_traits_2<Kernel>;
  using Pcbgt = CGAL::Arr_polycurve_basic_traits_2<Sgt>;
  using Pcgt = CGAL::Arr_polycurve_traits_2<Sgt>;
  using Plgt = CGAL::Arr_polyline_traits_2<Sgt>;
  using Pnt = Plgt::Point_2;
  using Xcv = Plgt::X_monotone_curve_2;
  constexpr auto ri(py::rv_policy::reference_internal);

  py::class_<Plgt, Pcgt> plgt_c(m, "Arr_polyline_traits_2");
  struct Concepts {
    Aos_basic_traits_classes<Plgt> m_aos_basic_traits_2_classes;
    Aos_x_monotone_traits_classes<Plgt> m_aos_x_monotone_traits_2_classes;
    Aos_traits_classes<Plgt> m_aos_traits_2_classes;
    Aos_landmark_traits_classes<Plgt> m_aos_landmark_traits_2_classes;
    Aos_directional_x_monotone_traits_classes<Plgt> m_aos_directional_x_monotone_traits_2_classes;
    Aos_approximate_traits_classes<Plgt> m_aos_approximate_traits_2_classes;
    Aos_construct_x_monotone_curve_traits_classes<Plgt>
      m_aos_construct_x_monotone_curve_traits_2_classes;
  } concepts;

  if (! add_attr<Pcbgt>(m, "Arr_polycurve_basic_traits_2")) {
    py::class_<Pcbgt> pcbgt_c(m, "Arr_polycurve_basic_traits_2");
  }

  if (! add_attr<Pcgt>(m, "Arr_polycurve_traits_2")) {
    py::class_<Pcgt, Pcbgt> pcgt_c(m, "Arr_segment_traits_2");
  }

  if (add_attr<Plgt>(m, "Arr_polyline_traits_2")) return;

  //export_AosLandmarkTraits_2<Plgt>(plgt_c, concepts);
  //export_AosDirectionalXMonotoneTraits_2<Plgt>(plgt_c, concepts);

  // add_insertion(xcv_c, "__str__");
  // add_insertion(xcv_c, "__repr__");
  // add_extraction(xcv_c);
}
