// Copyright (c) 2019 Israel.
// All rights reserved to Tel Aviv University.
//
// SPDX-License-Identifier: GPL-3.0-or-later.
// Commercial use is authorized only through a concession contract to purchase a commercial license for CGAL.
//
// Author(s): Nir Goren         <nirgoren@mail.tau.ac.il>
//            Efi Fogel         <efifogel@gmail.com>

#include <nanobind/nanobind.h>

#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>

#include "CGALPY/general_polygon_set_2_types.hpp"
#include "CGALPY/gps_2_concepts/export_GpsTraits_2.hpp"
#include "CGALPY/gps_2_concepts/Gps_traits_classes.hpp"

#include "cgalpy/Bso2_docstrings.hpp"

namespace py = nanobind;
namespace bso2_doc = cgalpy::docstrings::Boolean_set_operations_2;

// Export the instance Gps_circle_segment_traits_2 traits <Kernel>.
void export_gps_circle_segment_traits_2(py::module_& m) {
  using Agt = aos2::Arr_geometry_traits_2;
  using Xcv = Agt::X_monotone_curve_2;
  using Bgt = CGAL::Gps_traits_2<Agt>;
  using Pgn = Bgt::Polygon_2;
  using Ggt = CGAL::Gps_circle_segment_traits_2<Kernel>;

  if (add_attr<Ggt>(m, "Gps_circle_segment_traits_2")) return;

  py::class_<Ggt, Bgt> traits_c(
    m, "Gps_circle_segment_traits_2",
    bso2_doc::Gps_circle_segment_traits_2_class);
  traits_c.def(py::init<>());
  struct Concepts {
    Gps_traits_classes<Ggt> m_gps_traits_2_classes;
  } concepts;

  // `Gps_circle_segment_traits_2<Kernel>` derives from
  // `Gps_traits_2<Arr_circle_segment_traits_2<Kernel>>`, but nothing is added
  // to the API. Thus, the following call is redundant (but harmless, because
  // all wrappers are guarded).
  export_GpsTraits_2<Ggt>(traits_c, concepts);
}
